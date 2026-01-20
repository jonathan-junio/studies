# Virtualization

**CPU Virtualization:** Process thinks it has access to all CPU
**Memory Virtualization:** Process thinks it has access to all memory

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
**Process Control:** Signals to stop, continue, terminate etc

