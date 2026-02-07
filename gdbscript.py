import gdb
import re
import subprocess

vmlinux = "linux-6.18.7/vmlinux"
bootloader = "bootloader/build/bootloader.out"
GDB_SERVER = "localhost:1234"

def get_sections_map():
    output = gdb.execute("info files", to_string=True)
    sections = []
    pattern = r"(0x[0-9a-f]+) - (0x[0-9a-f]+) is (\.\S+)"

    for line in output.splitlines():
        match = re.search(pattern, line)

        if match:
            base = match.group(1)
            name = match.group(3)
            sections.append((name, int(base, 16)))
    return sections

class BreakPointCallback(gdb.Breakpoint):
    def __init__(self, symbol, func, arg=None):
        super(BreakPointCallback, self).__init__(symbol, gdb.BP_BREAKPOINT)
        self.symbol = symbol
        self.func = func
        self.arg = arg
        print(self.func)
    def stop(self):
        print(f"[+] Hit {self.symbol}")
        if self.arg:
            return self.func(self.arg)
        return self.func()

# Be careful of the command injection here!!!
def add_file(info: tuple[tuple, list[tuple], int]):
    filename, sections, offset = info
    cmdline = f"add-symbol-file {filename}"
    for name, base in sections:
        cmdline += f" -s {name} {hex(base + offset)}"
    gdb.execute(cmdline)

def stop_handler(event):
    sal = gdb.selected_frame().find_sal()
    if sal.symtab:
        file_path = sal.symtab.fullname()
        line = sal.line
        subprocess.Popen(
            ["code", "--goto", f"{file_path}:{line}:1"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
            start_new_session=True
        )
    pass

file_infos = []

gdb.execute(f"file {bootloader}")
file_infos.append((bootloader, get_sections_map(), 0))

gdb.execute(f"file {vmlinux}")
file_infos.append((vmlinux, get_sections_map(), 0))
gdb.execute(f"file init.gdb")
user_map = get_sections_map()
# file_infos.append(("init.gdb", get_sections_map(), 0x90600100 - 0x40))

gdb.execute(f"file")
for i in file_infos:
    add_file(i)


gdb.execute(f"target remote {GDB_SERVER}")

def skip_hook():
    lr = int(gdb.parse_and_eval("$lr")) & 0xffffffff
    gdb.execute(f"set $pc = {lr - 1}")
    return False

def break_at_return_hook(callback):
    lr = int(gdb.parse_and_eval("$lr")) & 0xffffffff
    BreakPointCallback(f"*{hex(lr - 1)}", callback)
    return False

def add_user_symbol():
    start_code = int(gdb.parse_and_eval("libinfo.lib_list[0].start_code"))
    global user_map
    add_file(("init.gdb", user_map, start_code))
    # gdb.execute("b *_start")
    return False

BreakPointCallback("rcc_init", skip_hook)
BreakPointCallback("qspi_init", skip_hook)
gdb.events.stop.connect(stop_handler)
BreakPointCallback("finalize_exec", break_at_return_hook, add_user_symbol)

gdb.execute("c")
