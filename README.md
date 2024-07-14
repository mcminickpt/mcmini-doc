McMini: A small, extensible model checker
=========================================

[McMini](https://github.com/mcminickpt/mcmini) is a small, extensible
model checker which inspects multithreaded
programs using Dynamic Partial Order Reduction (or DPOR for short).
The DPOR algorithm was described in the seminal 2005 paper by Flannagan
and Godefroid bearing the algorithm's title.  McMini uses DPOR to search
the initial trace of all thread schedules for a multithreaded program.
Within this constraint, McMini  detects deadlock, segfaults, assertion
failures, and other errors.

Unlike some model checkers, it runs directly on a Linux binary executable.
It is as easy to use as:

>   `mcmini ./a.out` <ARGS>`

McMini can be used standalone, but also within GDB, so as, for example,
to examine a thread schedule leading to deadlock.

>   `mcmini-gdb ./a.out` <ARGS>`

When using GDB, be sure to compile your target (e.g., "a.out") with
debugging (`-g` or `-g3`).  McMini adds extra GDB commands, such as
`mcmini forward` (next multithreaded operation), `mcmini back` (last
multithreaded operation), `mcmini printTransitions` (list all previous
multithreaded operations), and more.

Below is a screenshot of a McMini session within GDB.

![Screenshot of a McMini session](https://github.com/mcminickpt/mcmini-doc/docs/source/usage/transitions-middle-subtle-small.png?raw=true)

If you'd like to try out McMini, then:
* [Read the docs](https://mcmini-doc.readthedocs.io/en/latest/); and
* Download and install McMini: `git clone https://github.com/mcminickpt/mcmini.git`
