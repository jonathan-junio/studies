# Virtualization

Give to process the illusion of
- exclusivity
- continuity
- unlimited resources

**CPU Virtualization:** Multiple programs "running" at the same time
**Memory Virtualization:** Programs think they have access to the full memory
**Cuncurrency problems:** Instructions that do not finish in one clock
**Data persistance:** Data needs to be saved in memory and persist between reinitializations

### CPU Virtualization

- Active process[^1] swap -> context switch
  [^1]: Running program.
- Program Counter(Instruction Pointer): Next process instruction
- Stack Pointer: Top element of the stack
- Frame Pointer: Bottom element of the stack frame

#### Process API

Create, Destroy, Wait, Miscellaneous Control, Status

**Process Creation:** Load code and data -> Memory allocation for runtime stack[^2] -> Heap allocation [^3] -> I/O initiallization [^4] -> Jump to entry point and run
[^2]: Variables, retuen addresses, function parameters
[^3]: Dynamically allocated data
[^4]: i.e. file desctiptors in UNIX for standard input/output/error

**Process States:** Running, Ready, Blocked

- Running -> [Deschedule] -> Ready
- Ready -> [Schedule] -> Running
- Running -> [I/O Request] -> Blocked
- Blocked -> [I/O done] -> Ready

```
fork()
wait()
waitpid()
exec()
```

**Process Control:** Signals to stop, continue, terminate etc.
##### Limited direct execution protocol
Kernel mode vs User mode
user mode -> sys call -> trap table -> kernel mode -> return from trap -> user mode
**Trap table:** trap instructions location, initialized on boot, protected
Interrupt -> non cooperative schedule method