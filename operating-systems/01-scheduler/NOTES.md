## Scheduling policies

### Metrics
**Turnaround time:** $\tau$~time~ = $\tau$~completion~ - $\tau$~arrival~
**Response time:** $\tau$~rtime~ = $\tau$~firstrun~ - $\tau$~arrival~

### Algorithms
**First In, First Out:** Good for processes with same size
**Shortest Job First:** Bad if first process is long and other processes comes after
**Shortest Time To Completion:** If another process arrive, choses the shotest to complete and runs it -> bad for response time
**Round Robin:** Does not run to completion, run in time slices -> _schedule quantum_
**Multi-Level Feedback Queue:** Based on _Priority Levels_
 - Rule 1: ```if priority(A) > priority(B) -> A runs```
 - Rule 2: ```if priority(A) == priority(B) -> RR with schedule quantum```
 - Rule 3: When a job enters the queue it is placed at highest priority
 - Rule 4: Once a program uses its time allotment (regardless of how many times has given up control) its priority is reduced
 - Rule 5: After some time period S all jobs are elevated to highest priority
**Proportional Share:**  Each job obtain a certain percentage of the CPU. Based on ticket shares and ramdomness -> can implement ticket inflation, ticket transfer (client/server)
**Deterministic Proportional Share:** Stride schedule, after it runs a value is added to itself, next process choses lowest value
**Completely Fair Scheduler:** Divides CPU fairly -> based on _virtual runtime_
Scheduler choses process with lowest vruntime -> after run the process vruntime increases based on real time. 
_sched_latency_: how long process should run until context switch -> n processes: _sched_latency_ / n
_min_granularity_: minimal for time slices to prevento too many CS (overhead) if we have too many processes
_weighting_: priotiry based on _nice_ level (-20 to +19) lowest number -> higher priority
Uses red-black tree DS
**Multiprocessor Scheduler:**
