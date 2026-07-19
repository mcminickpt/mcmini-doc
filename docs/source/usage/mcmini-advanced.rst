===============
Advanced McMini
===============

.. toctree::
   :hidden:
   :maxdepth: 2

   mcmini-advanced

**TODO:** Advanced features of McMini

.. contents:: Contents of this page
   :backlinks: entry
   :local:
   :depth: 2


------------------------------------------------
Analyzing a trace: ``mcmini --trace <traceSeq>``
------------------------------------------------

**TODO:** Explain that ``traceSeq`` is '0 0 0 1 2', as opposed to ``traceId`` (e.g., 13).

Note that when the traceSeq is completed, McMini continues
to execute new transitions.  It will print:

.. code::

   ... continuing beyond trace sequence:  At transition # <num>

This information can be used later, using mcmini-gdb, to do:

.. code::

   (gdb) mcmini forward <num>
   (gdb) mcmini printPendingTransitions

to examine the pending transitions for each thread.
In this way, we can append a thread number to our ``traceSeq``,
and repeat `-t <traceSeq>`, in order to "steer" McMini
into the path of interest.

-----------------------------------------
Annotate for improved "THREAD BACKTRACE"
-----------------------------------------

  :command:`python3 mcmini-annotate -t <traceSeq>`

-------------------------------------
Using GDB with ``--trace <traceSeq>``
-------------------------------------

**TODO:**

Start with::
  :command:`mcmini-gdb  -t <traceSeq>`

Then consider::
  :command:`mcmini help`

You will see the following commands, all using a :command:`mcmini` prefix:

> mcmini: forward back printTransactions where help, etc.

For example,

.. code::

   (gdb) mcmini help
   (gdb) mcmini <TAB>
   (gdb) mcmini forward 9
   (gdb) mcmini where
   (gdb) layout src

Recall also that in GDB, 'ctrl-Xa' toggles between full-screen display
and traditional command mode. Further, in full-screen mode, 'ctrl-Xo'
toggles between the focus on the source (cursor keys browse the source
code) and the focus on the GDB command window (editing and re-executing
previous GDB commands).  The GDB commands "focus src" and "focus cmd"
also exist to change the focus for the full-screen mode.

**TODO:** McMini could ask GDB to watch any variable.
In this case, for each call frame, determine if the variable is present
(or else in global scope), and if so, then issue the ``watch`` command.
(**NOTE:** *McMini is open-source, and we welcome additional
developers.*)

-----------------------------------------------------
Extensibility:  Defining new operations, new policies
-----------------------------------------------------

**FILL IN**

(**NOTE:** *McMini is open-source, and we welcome additional
writers of documentation.  A good starting point for the McMini manual
is the McMini paper, cited in the* :ref:`citations` *section.*)

-----------------------------------------------------
Using Livelock Detection
-----------------------------------------------------

Livelock is a concurrency bug where threads keep executing transitions
without making progress. For example, two threads repeatedly backing
off and retrying a failed lock acquisition. McMini reports a potential
livelock when it observes a repeating execution cycle in which enabled
threads continue executing but no progress point is reached.

Enabling livelock detection (``-l``) performs this analysis in addition
to McMini's normal deadlock checking.

.. _livelock-getting-started:

Getting Started
-------------------

A good starting point for most programs is:

.. code:: shell

   mcmini -l -r ./program

This enables livelock detection (``-l``) with round-robin scheduling
(``-r``), which explores more thread interleavings and is recommended
for livelock detection.

If the execution takes too long to complete, see :ref:`livelock-search-limits`
for ways to reduce the search space. For details on ``-r``, see
:ref:`livelock-scheduling`.

.. _livelock-progress:

Progress Annotations
---------------------

McMini requires developers to annotate progress points in their code
using the ``MC_PROGRESS()`` macro. (If a program has no ``MC_PROGRESS`` macro,
then McMini will always report livelock.) This macro should be called after
each operation that represents meaningful progress toward completing
the thread's work.

For example, in a producer-consumer program, progress occurs when a
producer successfully adds an item to the buffer or when a consumer
successfully removes an item:

.. code:: C

   #include "MCProgress.h"

   void *producer(void *arg) {
     int work_item = 1;
     while (work_item < LAST_WORK_ITEM) {
       sem_wait(&sem_producer);
       pthread_mutex_lock(&mut_buf);
         push_buf(work_item++);
         MC_PROGRESS();  // Progress: item added to buffer
       pthread_mutex_unlock(&mut_buf);
       sem_post(&sem_consumer);
     }
   }

.. note::

   Progress annotations are only required for livelock detection; they
   do not affect deadlock detection.

.. note::

   Progress annotations should mark meaningful milestones rather than
   every successful instruction. Overly frequent annotations may prevent
   genuine livelock cycles from being recognized.

.. _livelock-scheduling:

Scheduling Policy
-------------------------------

The ``-r`` (round-robin) flag changes the order in which threads are explored.

By default, McMini tries threads in lexicographic order: thread 1
executes until it blocks, then thread 2, and so on. If a thread does
not block, McMini explores its transitions exhaustively before moving
to the next thread, unless ``-m`` is used to bound the depth. McMini may
explore only a single branch, therefore missing livelocks on other branches.

With ``-r``, McMini cycles through enabled threads in round-robin
order, ensuring each thread gets a turn. This better approximates how
real schedulers operate and is essential for detecting livelocks that
depend on fair interleaving.

.. code:: shell

   mcmini -l -r ./program

**When to omit** ``-r`` : If you know the livelock involves a specific
thread ordering, or if you want faster results and suspect the
lexicographic order is sufficient.

.. _livelock-search-limits:

Controlling the Search
-------------------------------

Livelock detection increases the search space. The flags ``-m``, ``-M``,
and ``-lc`` let you reduce how long the DPOR exploration runs.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Depth limits: ``-m<num>`` and ``-M<num>``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Both flags limit how many transitions McMini explores:

- ``-m`` limits total transitions per thread. The default is the maximum
  possible value. Setting ``-m`` prevents the scheduler from repeatedly
  selecting the same thread while other enabled threads remain unexplored.

- ``-M`` limits transitions per trace. The default is the maximum stack
  size. Setting ``-M`` to a lower value cuts off traces earlier.

.. code:: shell

   mcmini -l -r -m100 -M1000 ./program

**When to use:** If McMini is taking too long: use ``-m`` when one thread
executes many transitions while others remain idle and/or ``-M`` to prune
deeper branches of the search tree.

**Trade-off:** Smaller values speed up the search but may prevent McMini
from observing livelock patterns that occur deeper in the search.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Livelock cycle window: ``-lc<num>``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Controls how many transitions McMini examines when looking for a
repeating pattern (cycle). The default is 5.

.. code:: shell

   mcmini -l -r -lc10 ./program

**When to use:** Increase ``-lc`` if progress may occur only after
several transitions.

**Trade-off:** A larger ``-lc`` reduces false positives but adds to
the search cost.

.. _livelock-common-workflows:

Common Workflows
-------------------------------

**Long-running exploration**

If one thread spins in a loop while others wait, limit per-thread
depth to keep the search tractable:

.. code:: shell

   mcmini -l -r -m50 ./program

**Long retry loops**

If you suspect threads cycle through many states before repeating,
increase the cycle window:

.. code:: shell

   mcmini -l -r -lc15 ./program

**Large programs**

When you need to limit both per-thread and total depth (for example,
to keep runtime manageable on a large program):

.. code:: shell

   mcmini -l -r -m50 -M1000 ./program

.. _livelock-example:

Example: Producer-Consumer
-------------------------------

The McMini repository includes a producer-consumer example that
demonstrates livelock detection. That program implements custom semaphores
with busy-waiting, which can lead to livelock when threads are trapped
in polling loops.

**Building the example:**

.. code:: shell

   cd test/producer-consumer
   make

**Running with livelock detection:**

.. code:: shell

   mcmini -l -r -M50 ./producer-consumer

McMini detects the livelock and decomposes the witness into independent
cycles:

.. code:: text

   About to exec into ./producer-consumer
   *** POTENTIAL LIVELOCK DETECTED ***
   THREAD BACKTRACE
     1. thread 0: starts
     2. thread 0: pthread_create(thr:1, _, _, _)
    ...
    32. thread 6: starts

   CYCLE 1:
    52. thread 6: pthread_mutex_lock(mut:3)
    53. thread 6: pthread_mutex_unlock(mut:3)
    54. thread 6: pthread_mutex_lock(mut:3)
    55. thread 6: pthread_mutex_unlock(mut:3)

   CYCLE 2:
    56. thread 2: pthread_mutex_unlock(mut:2)
    57. thread 2: pthread_mutex_lock(mut:2)
    58. thread 2: pthread_mutex_unlock(mut:2)

   CYCLE 3:
    59. thread 1: pthread_mutex_lock(mut:2)
    60. thread 1: pthread_mutex_unlock(mut:2)
    61. thread 1: pthread_mutex_lock(mut:2)
    62. thread 1: pthread_mutex_unlock(mut:2)
   ...

Each cycle involves a small subset of threads, making it easier to
understand the livelock pattern. In this example, threads are repeatedly
acquiring and releasing locks without making progress on the actual
producer-consumer work.

.. _livelock-output:

Reading the Output
-------------------------------

When McMini detects a livelock, it reports the thread backtrace, cycle
decomposition, and pending operations:

.. code:: text

   *** POTENTIAL LIVELOCK DETECTED ***
   THREAD BACKTRACE
     1. thread 0: starts
     2. thread 0: pthread_create(thr:1, _, _, _)
    ...
   CYCLE 1:
    52. thread 6: pthread_mutex_lock(mut:3)
    53. thread 6: pthread_mutex_unlock(mut:3)
    54. thread 6: pthread_mutex_lock(mut:3)
    55. thread 6: pthread_mutex_unlock(mut:3)
   CYCLE 2:
    56. thread 2: pthread_mutex_unlock(mut:2)
    57. thread 2: pthread_mutex_lock(mut:2)
    ...
   THREAD PENDING OPERATIONS
    * thread 0: pthread_join(thr:1, _) [ Blocked ]
      thread 1: pthread_mutex_lock(mut:2) [ Blocked ]
      thread 2: pthread_mutex_lock(mut:2) [ Blocked ]
      thread 3: pthread_mutex_unlock(mut:2) [ Enabled ]
   END

The witness is decomposed into independent cycles, each involving a
small subset of threads. This makes it easier to understand the
livelock pattern.

Each transition shown in a cycle repeats indefinitely unless some
external condition changes. The **THREAD PENDING OPERATIONS** section
shows what each thread is waiting on, helping you confirm the
livelock pattern in your source code.

.. _livelock-choosing-options:

Choosing the Right Options
-------------------------------

Use this table as a quick reference:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Situation
     - Recommendation
   * - First run
     - ``mcmini -l -r ./program``
   * - Search takes too long
     - Reduce search with ``-m`` or ``-M``
   * - Long retry loops
     - Increase ``-lc``
   * - Large program
     - Combine ``-m`` and ``-M``

**Tips for best results:**

- Combine ``-l`` with either ``-m`` or ``-r`` for best coverage:

  a) Use ``-r`` for most analyses. Without round-robin scheduling,
     McMini explores threads in lexicographic order. If no thread
     blocks, only one branch is created, and livelocks requiring
     interleaving will be missed.

  b) Use ``-m`` to limit long-running threads. This prevents the
     scheduler from repeatedly selecting the same thread, ensuring
     other threads are explored.

- Find suitable values for ``-m`` and ``-M``. Both limit the search
  space, but in different ways: ``-m`` caps per-thread transitions,
  while ``-M`` caps total trace transitions. Set them high enough
  to observe livelock cycles, but low enough to keep the search
  tractable.
- Increase ``-lc`` if threads take several transitions before making
  progress. The default is 5; if progress occurs on the 6th transition
  or later, McMini may report a false positive.

.. hint::

   McMini may report a false positive if an ``MC_PROGRESS()`` annotation
   is missing. When this happens, inspect the independent cycles in the
   witness.

   If a thread repeatedly performs work that should be annotated, but is
   not, its cycle will often reveal that repeated operation. Insert
   ``MC_PROGRESS()`` at the appropriate point, then re-run McMini to
   determine whether the reported livelock disappears.

..   For example, if thread 2 updates a shared variable that enables thread
..   1 to make progress, but the update is not annotated, McMini may report two
..   short cycles: one where thread 1 repeatedly checks the condition, and
..   another where thread 2 repeatedly performs the update. Marking the
..   update with ``MC_PROGRESS()`` allows McMini to recognize that progress
..   is being made and report ``NO_LIVELOCK``.
