*This project has been created as part of the 42 curriculum by pigiacom.*

# Codexion

## Description

Codexion is a multithreaded C simulation inspired by the dining philosophers
problem. Each coder runs in an independent thread and needs two shared dongles
to compile. After compiling, the coder debugs and refactors before starting a
new cycle.

The simulation ends when every coder reaches the required number of compiles,
when a coder burns out, or when an internal error occurs.

The project implements two scheduling policies for waiting requests:

- `fifo`: the request that arrived first has priority.
- `edf`: the request with the earliest burnout deadline has priority. Arrival
  order is used to break ties.

## Build and execution

Compile the program with:

```sh
make
```

Run it with:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor required_compiles dongle_cooldown scheduler
```

All times are expressed in milliseconds. The scheduler must be `fifo` or
`edf`.

Example:

```sh
./codexion 5 800 100 100 100 3 10 fifo
```

Useful Makefile rules:

```sh
make clean
make fclean
make re
make test_heap_compare
```

## Output

Each line has a timestamp in milliseconds, a coder ID starting at 1, and a
state:

```text
0 1 has taken a dongle
0 1 is compiling
100 1 is debugging
200 1 is refactoring
```

Only the first burnout is printed, and no normal state is printed after the
simulation stops.

## Synchronization and blocking cases

- Each dongle has a mutex protecting its owner, cooldown and waiting heap.
- Condition variables suspend waiting threads and wake them when the shared
  state changes or when a short timed recheck is required.
- The global state mutex protects the stop reason and its condition variable
  makes sleeps interruptible.
- A dedicated log mutex prevents output lines from overlapping.
- Dongle pairs are always acquired by increasing dongle ID. This removes the
  circular-wait condition and prevents deadlock.
- FIFO and EDF priority heaps make the waiting order explicit and reduce
  starvation risk.
- A released dongle cannot be acquired before its cooldown expires.
- The monitor checks compile targets and burnout deadlines independently of
  coder activity.
- The one-coder case acquires its only dongle once and waits for the expected
  burnout, instead of trying to acquire the same mutex twice.

## Project structure

```text
include/          Public and internal headers
src/coder/        Coder lifecycle, work cycle and resource pairing
src/dongle/       Dongle initialization, waiting and cooldown
src/heap/         FIFO/EDF binary priority heap
src/monitor/      Completion and burnout monitoring
src/parsing/      Command-line validation
src/simulation/   Global initialization and stop state
src/time/         Time measurement and interruptible waits
src/log/          Serialized state output
tests/            Heap comparison, removal and stress tests
```

## Resources

The implementation was developed using the following references and concepts:

- POSIX Threads manual pages for `pthread_create`, mutexes and condition
  variables.
- The `gettimeofday` and `clock_gettime` manual pages for timestamps and timed
  waits.
- The Coffman conditions for understanding and preventing deadlocks.
- Binary heap, FIFO and Earliest Deadline First scheduling concepts.
- Valgrind Memcheck, Helgrind and DRD for memory and concurrency validation.

AI assistance was used to explain synchronization concepts, review the code,
suggest test cases, help reorganize functions for Norminette.
