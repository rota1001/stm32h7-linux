import gdb

vmlinux = "linux-6.18.7/vmlinux"
GDB_SERVER = "localhost:1234"


class BreakPointCallback(gdb.Breakpoint):
    def __init__(self, symbol, func):
        super(BreakPointCallback, self).__init__(symbol, gdb.BP_BREAKPOINT)
        self.symbol = symbol
        self.func = func
    def stop(self):
        print(f"[+] Hit {self.symbol}")
        return self.func()


gdb.execute(f"file {vmlinux}")
gdb.execute(f"target remote {GDB_SERVER}")

def calibrate_delay_hook():
    lr = int(gdb.parse_and_eval("$lr"))
    gdb.execute(f"set $pc = {lr - 1}")
    return False

BreakPointCallback("calibrate_delay", calibrate_delay_hook)

# gdb.execute("b *schedule_preempt_disabled")
gdb.execute("b start_kernel")
# gdb.execute("b *stm32_usart_console_putchar")
# devtmpfsd
gdb.execute("c")
