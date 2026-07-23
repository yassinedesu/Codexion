*This project has been created as part of the 42 curriculum by yael-kha.*

# Codexion

## Description

Codexion is a concurrency simulation written in C using POSIX threads.

A number of coders sit around a circular table. Between every pair of adjacent coders
there is one shared USB dongle, so there are exactly as many dongles as coders. To
compile, a coder must hold **two** dongles at the same time — the one on its left and
the one on its right. Because neighbours share a dongle, two adjacent coders can never
compile simultaneously, which caps the number of concurrent compiles at
`floor(number_of_coders / 2)`.

Each coder runs its own thread and repeats the same cycle: acquire two dongles, compile,
release both dongles, debug, refactor, then immediately try to acquire dongles again. A
coder that does not *start* a new compile within `time_to_burnout` milliseconds of the
start of its previous compile (or of the start of the simulation, for the first compile)
burns out. That clock never pauses: it keeps running while the coder debugs, refactors,
and while it is blocked waiting for a dongle.

The simulation ends in one of two ways:

- **Burnout** — any coder misses its deadline. A dedicated monitor thread detects this
  and prints the burnout line within the required 10 ms window.
- **Completion** — every coder has compiled at least `number_of_compiles_required`
  times.

The goal of the project is to coordinate access to the shared dongles without deadlock,
without starvation, without data races, and without memory leaks, while honouring a
mandatory arbitration policy (`fifo` or `edf`) implemented on top of a hand-written
priority queue.

## Instructions

### Compilation

```
make
```

The `Makefile` sits at the repository root and builds the binary `codexion` with
`cc -Wall -Wextra -Werror -pthread`. Object files carry the project header as a
prerequisite, so editing `src/codexion.h` rebuilds the translation units that depend on
it and nothing else is relinked unnecessarily.

Available rules:

| Rule | Effect |
| --- | --- |
| `all` | Default target, builds `codexion` |
| `$(NAME)` | Links the object files into `codexion` |
| `clean` | Removes the object files |
| `fclean` | Runs `clean`, then removes the binary |
| `re` | Runs `fclean`, then `all` |

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All eight arguments are mandatory and are read in exactly that order.

| Argument | Meaning |
| --- | --- |
| `number_of_coders` | Number of coder threads, and also the number of dongles |
| `time_to_burnout` | Milliseconds a coder may go without starting a compile |
| `time_to_compile` | Milliseconds spent compiling, holding two dongles |
| `time_to_debug` | Milliseconds spent debugging |
| `time_to_refactor` | Milliseconds spent refactoring |
| `number_of_compiles_required` | Compiles each coder must reach for a clean stop |
| `dongle_cooldown` | Milliseconds a released dongle stays unavailable |
| `scheduler` | Arbitration policy, exactly `fifo` or `edf` |

### Usage examples

```
./codexion 1 800 200 200 200 5 100 fifo
```
One coder, one dongle. It picks up its single dongle, can never obtain a second one, and
burns out at roughly 800 ms. The program then terminates cleanly.

```
./codexion 5 2000 200 100 100 3 100 fifo
```
Five coders with comfortable timings. Every coder reaches three compiles and the
simulation stops on the completion condition.

```
./codexion 5 2000 200 100 100 3 100 edf
```
The same run under earliest-deadline-first arbitration.

### Input validation

Arguments are validated in two stages before the simulation is built.

1. **Format** — every numeric argument is walked character by character. Any character
   that is not a digit (after optional leading whitespace and an optional sign) rejects
   the input, so strings such as `5abc` are refused rather than silently truncated to a
   valid-looking number. The scheduler argument is compared with `strcmp` against the two
   accepted values.
2. **Value** — conversion is done by a hand-written `ft_atol` that detects overflow
   *inside* the accumulation loop, before the next multiplication can corrupt the result.
   Values that exceed `INT_MAX`, and any value that is not strictly positive, are
   rejected. `dongle_cooldown` is the one numeric argument allowed to be zero, since a
   zero cooldown is meaningful (a dongle becomes immediately reusable).

On invalid input the program prints an error message and exits with status `1` without
starting any thread.

## Blocking cases handled

### Deadlock prevention — Coffman's four conditions

Deadlock requires all four of Coffman's conditions to hold at once, so breaking any one
of them is sufficient. Mapped onto this project:

- **Mutual exclusion.** A dongle can be held by only one coder at a time. This condition
  is inherent to the problem and is not removed — it is the whole point of the
  simulation.
- **Hold and wait.** A coder holds its first dongle while requesting its second. This
  condition is present during normal contention. It is bounded on the shutdown path: if
  the acquisition of the second dongle is aborted because the simulation is stopping, the
  first dongle is released before the coder leaves.
- **No preemption.** A dongle is never taken away from the coder holding it; it is only
  released voluntarily after compiling.
- **Circular wait.** *This is the condition the implementation breaks.* Acquisition
  order is asymmetric: coders at an even array index request their lower-indexed dongle
  first and their higher-indexed one second, while coders at an odd index request them in
  the opposite order. Because not every coder reaches around the ring in the same
  direction, the chain "each coder holds one dongle and waits on the next coder's dongle"
  can never close into a full cycle. Two neighbours that reverse relative to each other
  contend for the *same* dongle as their first request, so one of them wins outright and
  the loop is broken.

### Starvation prevention

Every dongle owns its own wait queue, implemented as a min-heap. A coder that wants a
dongle inserts itself into that dongle's queue with a priority key, then sleeps on the
dongle's condition variable. It is only allowed to proceed when it is at the root of the
heap, that is, when it is the coder the policy has selected. It removes itself from the
queue at that moment.

The key depends on the scheduler chosen at startup:

- **`fifo`** — the key is the timestamp at which the request was made, so the dongle is
  granted in arrival order. FIFO is starvation-free by construction: a waiting coder's
  position can only improve as earlier requests are served.
- **`edf`** — the key is `last_compile_start + time_to_burnout`, the coder's burnout
  deadline, so the dongle is granted to whichever waiting coder is closest to burning
  out.

Ties are broken deterministically. When two waiting coders hold the same key, the one
with the lower coder number is served first. Equal keys are rare in practice given
millisecond timestamps, but without an explicit rule the outcome would depend on the
incidental layout of the heap array, so the comparison used by the queue falls back to
the coder number whenever the two keys are equal.

The two policies genuinely diverge under contention. If one coder queued slightly earlier
but has time to spare, while another queued a moment later and is close to its deadline,
FIFO serves the first and may let the second burn out, whereas EDF serves the coder in
danger. That difference is the reason both policies are required.

Liveness is guaranteed only for feasible parameters. Because at most
`floor(number_of_coders / 2)` coders can compile at once, a parameter set in which the
minimum round-trip time (`time_to_compile + time_to_debug + time_to_refactor`, plus the
unavoidable waiting caused by contention and cooldown) already exceeds `time_to_burnout`
makes burnout mathematically unavoidable, and no arbitration policy can prevent it.

### Cooldown handling

Each dongle records the timestamp at which it was last released. It is considered
available only when it is not currently held **and** the elapsed time since that release
is at least `dongle_cooldown` milliseconds. Both conditions are re-checked inside the
predicate loop, so a coder that reaches the front of the queue while the cooldown is
still running does not take the dongle: it releases the dongle mutex briefly, waits, and
re-evaluates.

At startup the last-release timestamp of every dongle is initialised to
`-dongle_cooldown`, so that all dongles are immediately available at t = 0 instead of
being artificially blocked for one cooldown period before the simulation has begun.

### Precise burnout detection

A dedicated monitor thread polls every coder once per millisecond. For each coder it
reads the compile counter and the last compile start under a mutex, then compares the
elapsed time against `time_to_burnout`. A 1 ms poll interval is comfortably inside the
10 ms precision requirement demanded by the subject.

Because the burnout clock is anchored to the *start* of a compile and never pauses, the
monitor detects burnout regardless of what the coder is doing at that instant — it may be
debugging, refactoring, or blocked waiting for a dongle.

When burnout is detected the monitor sets the stop flag, prints the burnout line, and
then wakes every coder blocked on a dongle. A coder that has already reached
`number_of_compiles_required` is excluded from the burnout check, so a coder that has
finished its work is never falsely reported as burned out while other coders are still
running.

### Log serialization

All output passes through a single function guarded by a dedicated log mutex, so a line
is written in full before another thread can write. Two state messages can therefore
never interleave on the same line.

The same function also checks the stop flag: once the simulation has stopped, ordinary
state messages are suppressed and only the burnout line is allowed through. This prevents
a coder that is still unwinding from printing state changes after the terminating event
has been reported.

### Clean shutdown

Setting the stop flag is not by itself enough to release a thread that is asleep on a
condition variable — a sleeping thread cannot observe a flag. After setting the flag, the
monitor therefore broadcasts on every dongle's condition variable. Each woken coder
re-evaluates its predicate, observes the stop flag, abandons the acquisition and returns
from its routine. `main` then joins every coder thread and the monitor, destroys all
mutexes and condition variables, and frees every allocation. Both termination paths, the
burnout path and the completion path, exit without hanging.

Failures during start-up unwind in the same disciplined way. If an allocation fails
partway through building the dongle array, the wait queues already created are released
before the error is propagated. If a mutex or condition variable fails to initialise, only
those already initialised are destroyed, tracked by index, so nothing uninitialised is
destroyed and nothing initialised is left behind. If `pthread_create` fails partway
through spawning coders, the stop flag is raised and the threads already running are
joined before cleanup, since joining a thread that was never created is undefined
behaviour.

## Thread synchronization mechanisms

### Threads

There are `number_of_coders + 1` threads: one per coder, plus one monitor. Dongles are
not threads; they are passive shared structures.

### Data ownership without globals

Global variables are forbidden, so no thread reaches shared state by naming it. All
shared state lives in a heap-allocated simulation structure. Each coder owns its own
structure holding its identifier, its compile counter, its last compile start, and a
pointer back to the simulation structure. `pthread_create` accepts a single `void *`, so
each thread is handed a pointer to its own coder structure and reaches everything shared
by following the embedded pointer. Sharing is achieved entirely by explicit hand-off of
addresses.

### Mutexes

| Mutex | Protects |
| --- | --- |
| `log_mutex` | Standard output. Held for the duration of a single line so messages cannot interleave. |
| `stop_mutex` | The stop flag, plus each coder's `last_compile_start` and `number_of_compiles`. Written by coders, read by the monitor. |
| `dongles[i].mutex` | Dongle `i`'s held/free state, its last-release timestamp, and its wait queue. |

### Condition variables

| Condition variable | Purpose |
| --- | --- |
| `dongles[i].cond` | Where coders sleep while dongle `i` is unavailable, and where they are woken when it is released or when the simulation stops. |

Every wait is wrapped in a predicate loop rather than a single `if`. This is required for
two reasons: a wait can return spuriously without the condition having become true, and
between being woken and re-acquiring the mutex another thread may have changed the state
again. The loop forces a fresh evaluation of the full predicate — not held, cooldown
expired, and this coder is at the head of the queue — before the coder proceeds.

`pthread_cond_broadcast` is used rather than `pthread_cond_signal`. `signal` releases one
arbitrary waiter, which is unsuitable here because the arbitration policy has already
chosen *which specific* coder should proceed; the arbitrary waiter may not be that coder.
Broadcast wakes every waiter, each re-checks the predicate, the selected coder proceeds,
and the rest return to sleep. Broadcast is also what makes shutdown work, since every
sleeping coder must be given the chance to observe the stop flag.

### Race conditions prevented

- **Dongle state.** Without a mutex, two coders could both read a dongle as free and both
  mark it taken, duplicating a dongle. Each dongle's state is only ever read or written
  while its own mutex is held.
- **Wait queue.** Each dongle's heap is inserted into and removed from by several coders.
  It is protected by that dongle's mutex, which is held across every queue operation.
- **Compile counter.** Written by its coder, read by the monitor. Both accesses are taken
  under `stop_mutex`, so the monitor never observes a partially updated count.
- **Burnout timestamp.** Written by its coder at the start of each compile, read by the
  monitor on every poll. Guarded by `stop_mutex`; the monitor copies the value into a
  local variable inside the critical section and does its arithmetic outside it.
- **Stop flag.** Written by the monitor, read by every coder at every dongle acquisition
  and before every log line. Reading an integer while another thread writes it is a data
  race, so all accesses are serialized by `stop_mutex`.

### Lock ordering

Where two locks are involved, the order is fixed and consistent so that no inversion can
occur. A coder holding a dongle mutex may acquire `stop_mutex` to test the stop flag,
which establishes the order dongle → stop. The monitor therefore never holds `stop_mutex`
while acquiring a dongle mutex: it sets the flag, releases `stop_mutex`, and only then
broadcasts on the dongle condition variables. Reversing this would produce the classic
lock-order inversion, with the coder holding the dongle mutex and waiting for
`stop_mutex` while the monitor holds `stop_mutex` and waits for the dongle mutex.

### Priority queue

The arbitration queue is a hand-written binary min-heap stored in a flat array; no
standard-library priority queue is used. Insertion appends at the end of the array and
sifts the new node up while its key is smaller than its parent's. Extraction moves the
last node to the root, shrinks the size, and sifts that node down, swapping with the
smaller of its two children until the heap property is restored. For a node at index `i`,
the parent is at `(i - 1) / 2` and the children are at `2i + 1` and `2i + 2`. Each queue
is sized to the number of coders, which is the maximum number of coders that can ever
wait on a single dongle.

Both directions of movement share a single comparison helper, which compares keys first
and falls back to the coder number when keys are equal. Keeping the rule in one place
means the sift-up and sift-down paths cannot disagree about ordering, which would
otherwise corrupt the heap property.

## Verification

The build was checked with Valgrind for memory errors and with Helgrind for data races.
Helgrind reports no errors in the project's own code across both termination paths.

## Resources

### Documentation and references

- `man` pages for `pthread_create`, `pthread_join`, `pthread_mutex_init`,
  `pthread_mutex_lock`, `pthread_cond_wait`, `pthread_cond_broadcast`, `gettimeofday`
  and `usleep`
- The POSIX threads specification (IEEE Std 1003.1) for the semantics of condition
  variables, spurious wakeups and the predicate-loop requirement
- E. W. Dijkstra's dining philosophers problem, and the standard asymmetric
  resource-ordering solution to it
- E. G. Coffman, M. J. Elphick and A. Shoshani, *System Deadlocks* (1971), for the four
  necessary conditions for deadlock
- C. L. Liu and J. W. Layland, *Scheduling Algorithms for Multiprogramming in a Hard
  Real-Time Environment* (1973), for earliest-deadline-first scheduling
- Standard references on binary heaps for the array representation and the sift-up /
  sift-down procedures
- Valgrind and Helgrind documentation for leak and race detection

### Use of AI

An AI assistant (Claude) was used throughout this project as a **mentor and reviewer**,
not as a code generator. Specifically:

- **Concept explanation.** Understanding condition variables and the predicate-loop
  pattern, the distinction between `broadcast` and `signal`, Coffman's four conditions
  mapped onto this project, the `floor(n / 2)` concurrency ceiling, and `struct timeval`
  arithmetic. These were explained with generic examples unrelated to this project.
- **Design review.** Discussing how to split per-coder state from shared state, how to
  pass both through the single `void *` accepted by `pthread_create` without using
  globals, and why the coder display identifier (1-based) must be kept distinct from the
  array index (0-based) used for dongle arithmetic.
- **Code review and debugging.** Reviewing code I had already written and pointing out
  defects for me to fix myself: a non-integer input such as `5abc` being accepted by the
  parser, an overflow check placed after the accumulation loop instead of inside it, an
  unnecessary duplication of the scheduler string from `argv`, an off-by-one in the
  partial-initialisation cleanup path, a swapped debug/refactor order, a missing sleep in
  the debug phase, a lock-order inversion between the stop mutex and the dongle mutexes,
  a missing null check that would have crashed on an allocation-failure path, and the
  absence of a deterministic tie-breaker in the priority queue.
- **Norm compliance.** Suggesting where to extract helper functions so that individual
  functions stayed within the line limits of the 42 Norm.
- **Test design.** Suggesting which parameter sets to run in order to distinguish a
  genuine synchronization bug from a parameter set that is simply infeasible, and which
  Valgrind and Helgrind invocations to use.
- **This README.** The assistant produced the initial draft of this file from my source
  code; I reviewed and edited it, and I am responsible for the accuracy of every claim in
  it.

No implementation code was generated by the assistant. Every function in `src/` was
written by me, and the design decisions — the asymmetric acquisition order, the per-dongle
heap as a wait queue, the single monitor poll loop, the mutex layout — are mine to
justify.
