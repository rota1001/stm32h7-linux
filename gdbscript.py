import gdb
import re

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
    def __init__(self, symbol, func):
        super(BreakPointCallback, self).__init__(symbol, gdb.BP_BREAKPOINT)
        self.symbol = symbol
        self.func = func
    def stop(self):
        print(f"[+] Hit {self.symbol}")
        return self.func()

# Be careful of the command injection here!!!
def add_file(info: tuple[tuple, list[tuple]]):
    filename, sections = info
    cmdline = f"add-symbol-file {filename}"
    for name, base in sections:
        cmdline += f" -s {name} {hex(base)}"
    gdb.execute(cmdline)


file_infos = []

gdb.execute(f"file {bootloader}")
file_infos.append((bootloader, get_sections_map()))
gdb.execute(f"file {vmlinux}")
file_infos.append((bootloader, get_sections_map()))

for i in file_infos:
    add_file(i)


gdb.execute(f"target remote {GDB_SERVER}")

def skip_hook():
    lr = int(gdb.parse_and_eval("$lr"))
    gdb.execute(f"set $pc = {lr - 1}")
    return False

BreakPointCallback("rcc_init", skip_hook)
BreakPointCallback("qspi_init", skip_hook)

gdb.execute("c")
