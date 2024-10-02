.. McMini documentation master file, created by
   sphinx-quickstart on Sat May 18 20:18:19 2024.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. COMMENT:  How to figure out toctree in alabaster theme:
   https://stackoverflow.com/questions/59397527/managing-sphinx-toctrees-combined-with-inline-sections
   COPY THIS: https://github.com/sphinx-doc/alabaster  for toctree
   CONSIDER contents:: https://docutils.sourceforge.io/docs/ref/rst/directives.html#table-of-contents
     IT USES: https://docutils.sourceforge.io/docs/ref/rst/directives.html#topic
   EXAMPLE: https://docs.python.org/3/reference/index.html
   EXAMPLE: https://docs.python.org/3/library/index.html
   https://www.sphinx-doc.org/en/master/search.html?q=toctree
   https://stackoverflow.com/questions/16123951/how-do-i-include-the-homepage-in-the-sphinx-toc
   https://stackoverflow.com/questions/18969093/how-to-include-the-toctree-in-the-sidebar-of-each-page
   https://stackoverflow.com/questions/33029008/how-to-edit-sidebar-under-sphinx-alabaster-theme
   https://stackoverflow.com/questions/54348962/sphinx-toctree-in-sidebar-only
   https://stackoverflow.com/questions/34249244/how-to-make-toctree-link-refer-to-the-separate-file-like-it-refers-to-the-subsec

.. The following is a substitution reference
   https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html#substitution-definitions
   https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html#inline-markup
   https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html#substitution-references

.. |nbsp| unicode:: 0xA0
   :trim:

.. |br| raw:: html

   <br />

.. only:: text

 `  .. |br| raw:: text

       :

.. CO<MENT:  define a hard line break for HTML
   But maybe unicode newline is better.

.. comment Welcome to McMini's documentation!
   ==================================

McMini
======

**McMini** is an explicit-state model checker, based on :ref:`DPOR<dpor>`.
It analyzes a target that
is a multi-threaded executable (binary), and identifies thread schedules
that produce deadlock, segfaults, assertion failures, and other errors.
(See the :ref:`McMini paper<mcmini_paper>` for technical details of
its design.)

Unlike some model checkers, McMini runs directly on a Linux binary.
It is as easy to use as:

.. parsed-literal::

   :command:`mcmini` :program:`./a.out`

.. contents:: Contents of this page
   :backlinks: entry
   :local:
   :depth: 2

.. COMMENT
   If this is not a real directive, then it's a comment.

------------
Installation
------------

McMini is installed as follows.

.. code:: shell

   git clone https://github.com/mcminickpt/mcmini.git
   cd mcmini
   ./configure
   make -j8

If you are debugging, use ``./configure --enable-debug``, but McMini
will then be compiled with ``-g3 -O0``, which implies lower performance.

.. _quick start:

-----------
Quick Start
-----------

The formal 'man page' for McMini follows in the next subsection.  However,
this subsection may be all that you need to gain a basic understanding
of McMini.  If McMini appeals to you, we recommend also looking at the
section:  :ref:`Tutorial: Running McMini<tutorial-basic>`.

To begin, copy the following program into a file, :file:`deadlock.c`.

.. code:: C

   #include <stdio.h>
   #include <pthread.h>
   pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

   void *child_thread(void *dummy) {
     pthread_mutex_lock(&mutex2);
     pthread_mutex_lock(&mutex1);
     printf("child thread: I have both mutexes.  I win!\n");
     pthread_mutex_unlock(&mutex1);
     pthread_mutex_unlock(&mutex2);
     return NULL;
   }

   int main() {
     pthread_t thread;
     pthread_create(&thread, NULL, child_thread, NULL);
     pthread_mutex_lock(&mutex1);
     pthread_mutex_lock(&mutex2);
     printf("parent thread: I have both mutexes.  I win!\n");
     pthread_mutex_unlock(&mutex2);
     pthread_mutex_unlock(&mutex1);
     pthread_join(thread, NULL);
     printf("*** Done.\n");
     return 0;
   }

Now compile it::

   gcc deadlock.c -o deadlock
   ./deadlock

When we run it, we see::

  parent thread: I have both mutexes.  I win!
  child thread: I have both mutexes.  I win!
  *** Done.

Now let's test this code with McMini, to see if there's a bug.
We execute:


.. code:: shell

   ./mcmini -f -q -m15 ./deadlock

McMini then uncovers the following deadlock:

.. _McMini output:

.. code:: shell
   :number-lines:

   About to exec into ./deadlock
   TraceId 1, *** DEADLOCK DETECTED ***
   THREAD BACKTRACE
    1. thread 0: starts
    2. thread 0: pthread_create(thr:1, _, _, _)
    3. thread 0: pthread_mutex_lock(mut:1)
    4. thread 1: starts
    5. thread 1: pthread_mutex_lock(mut:2)
   0, 0, 0, 1, 1,
   END
   THREAD PENDING OPERATIONS
    * thread 0: pthread_mutex_lock(mut:2) [ Blocked ]
      thread 1: pthread_mutex_lock(mut:1) [ Blocked ]
   END
   ***** Model checking completed! *****
   *** DEADLOCK DETECTED ***
   (Trace number (traceId): 1)
     Number of traces: 2
   Total number of transitions: 15
   Elapsed time: 0 seconds

Before analyzing this, we introduce some terminology.

**Definition:** *transition*
  A :dfn:`thread transition` (or "transition", for short) is a thread
  operation.  As seen above, McMini supports ``pthread_create``,
  ``pthread_join``, ``starts`` (thread start), ``exits`` (thread exit),
  operations for mutex, semaphore, condition variables, and others.

**Definition:** *trace*
  An :dfn:`execution trace` (or "trace", for short) is a particular thread
  schedule for a single execution of a *target program*.  The target above
  is for the program :program:`a.out` (:program:`deadlock`, in our case).
  The trace above is for traceId number |nbsp| 1.  (The traceId's begin
  at traceId |nbsp| 0.)

We see that McMini found a deadlock (sometimes known as a "deadly
embrace").
(And McMini can similarly diagnose
if some trace causes a segmentation fault or an assertion failure.)
Here is a guide to McMini's report of this deadlock:

* Line |nbsp| 2 (and also line |nbsp| 17): This thread schedule is for traceId |nbsp| 1.
* Line |nbsp| 3:  The "THREAD BACKTRACE" (thread schedule) is shown compactly as a single line:  one threadId for each transition of a thread.
* Line |nbsp| 9:  A more concise summary of the sequence of threads shown from line |nbsp| 3.
* **Analysis of the transitions (thread operations) for this trace:**
   * Lines |nbsp| 6 and |nbsp| 12:  The parent thread acquired mutex |nbsp| 1 (line |nbsp| 6) and waits for
     mutex |nbsp| 1 (line |nbsp| 12) in its pending operation.
   * Lines |nbsp| 8 and |nbsp| 13:  The child thread acquired mutex |nbsp| 2 (line |nbsp| 8) and waits for
     mutex |nbsp| 1 (line |nbsp| 13) in its pending operation.
   *  In summary, Lines |nbsp| 12 and |nbsp| 13 show that both threads are "Blocked".  So, this is true deadlock.

So, why didn't we see this deadlock when executing ``./a.out`` by itself?
It's because the operating system scheduled the threads according to
traceId |nbsp| 0.  To verify this, let's ask McMini to print a
more "common" thread schedule.  Let's inspect traceId
|nbsp| 0.

.. code:: shell

   USER% ./mcmini -p0 -q -m15 ./deadlock

   About to exec into ./deadlock
   *** -p or --print-at-trace requested.  Printing trace:
   THREAD BACKTRACE
    1. thread 0: starts
    2. thread 0: pthread_create(thr:1, _, _, _)
    3. thread 0: pthread_mutex_lock(mut:1)
    4. thread 0: pthread_mutex_lock(mut:2)
    5. thread 0: pthread_mutex_unlock(mut:2)
    6. thread 0: pthread_mutex_unlock(mut:1)
    7. thread 1: starts
    8. thread 1: pthread_mutex_lock(mut:2)
    9. thread 1: pthread_mutex_lock(mut:1)
   10. thread 1: pthread_mutex_unlock(mut:1)
   11. thread 1: pthread_mutex_unlock(mut:2)
   12. thread 1: exits
   13. thread 0: pthread_join(thr:1, _)
   0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
   END
   THREAD PENDING OPERATIONS
    * thread 0: exits [ Done ]
      thread 1: exits [ Done ]
   END
   ***** Model checking completed! *****
         (NO FAILURE DETECTED)
   Number of traces: 1
   Total number of transitions: 13
   Elapsed time: 0 seconds

This is the end of the quick start.  If you want to dig deeper into McMini, it's time to dig into:

1. The McMini man page: `sec-man`_
2. Next, to analyze a non-trivial multithreaded program, consider the :doc:`usage/tutorial-subtle` section.

.. _sec-man:

------------------------------
McMini man page
------------------------------

.. program:: mcmini

**NAME**

.. parsed-literal::
   mcmini - McMini is a full-featured, in-situ, extensible model checker.

**SYNOPSIS**

.. parsed-literal::

   :program:`mcmini` [-m <num>] [-f] [-q] [-p <traceId|traceSeq>] [-v [-v]] [-h] target_executable

.. option:: -m <num>, --max-depth-per-thread <num>

   Maximum number of transitions executed for any given thread.
   If a thread reaches this maximum depth of transitions, then
   no further transitions for this thread will be considered.
   But transitions by other threads can continue to extend the current trace.

.. option:: -f, --first, --first-deadlock

   Stop search at the first deadlock found, and print that trace.

.. option:: -q, --quiet

   Don't print the target executable to standard output, while executing traces.

.. option:: -p <traceId|traceSeq>, --print-at-traceId <traceId|traceSeq>

   There are two forms, depending on whether *traceId* or *traceSeq*
   is supplied.  If *traceId* (a number) is supplied, then the
   search of traces will stop after it completes *traceId*.
   If *traceSeq*, see below.

.. option:: -v, --verbose

   For each trace explored, print its trace sequence.  If '-v -v', print all.

.. option:: -h, --help

   Display the command-line options and exit.

.. option:: target_executable

   Target multi-threaded executable (binary)

|nbsp|

**ADDITIONAL PROGRAMS**

.. parsed-literal::

   :program:`mcmini-gdb` ...<same as mcmini args>...
   :program:`mcmini-annotate` -p <traceSeq> ...<same as mcmini args>...

|nbsp|

**DESCRIPTION**

McMini searches all possible *traces* within certain constraints.
A *trace* is a sequences of *transitions* (thread operations).
McMini searches in lexicographic order according to the id of the
thread executing the thread operation.  Certain branches of the
search are pruned if it can be proved that a different branch
produces identical semantics.  Each trace is numbered sequentially.

McMini stops its search according to three stop criteria:

  a. when a trace number is reached, as specified by :option:`-p` <traceId>; or
  b. when a trace results in deadlock, if :option:`-f` was specified; or
  c. when a specific trace sequence is specified by :option:`-p` <traceSeq>.

(See the subsection on :ref:`advanced features`
for a description of trace sequences for :option:`-p`.)

The three stop criteria can also be used in combination, including (for example)
``-p 2 -p '0,0,1,2'`` .  In this example, McMini begins each trace
with the prefix given by thread ids '0,0,1,2' running in sequence.
Within those searches for that prefix, McMini :option:`-p` |nbsp| 3
will examine thread traces: traceId 0, 1, and finally
stopping at traceID |nbsp| 2.

**BUG:** ``-p 0 -p '0,0,1,2'`` works, but not ``-p 2 -p '0,0,1'`` for ./deadlock

**TODO:** *Check if assertion and segfault also stop the search when
:option:`-f` is specified.*

Another option, which modifies the traces that McMini searches,
is :option:`-m` (the maximum depth, or number of transitions,
executed by any one thread).  Any traces reaching this maximum depth
are then truncated, and the following branch is then searched.

Finally :option:`-v` (verbose) prints the trace sequence for
each trace that is searched.  The option :option:`-v` |nbsp| :option:`-v`
also prints out the full transitions encountered for that trace.

And :option:`-q` (quiet) discards any output to the screen (stdout and stderr).

.. _advanced features:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
McMini advanced features using trace sequences
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. parsed-literal:: **McMini alternative: traceSeq:**

   mcmini -p 0  -p <traceSeq> [-m <num>] [-f] [-q] target_executable

In this variation, a :abbr:`trace sequence (traceSeq)` is supplied (such as the trace
sequence `0, 0, 0, 1, 1,` from line |nbsp| 9 of the `McMini output`_
in the Quick Start section).  The option `-p 0` was supplied to ensure
that only this one trace sequence is explored.  If `-p 0` is omitted,
then the trace sequence will be viewed as a prefix, and McMini will
search all traces with this prefix.

**TODO:** *mcmini break* and *mcmini continue*:  Declare user breakpoints,
and then McMini uses continue_until to stop on user breakpoint while
ignoring 'mcmini forward', etc.  After this, 'mcmini where' will still work.

**TODO:** *mcmini printTraceSequence*:  call ``programState->printTraceSequence()``.
But Python can parse it.  If it's too long, then print it in abbreviated form:

>  ..[transitionId:].. 1, 2, 1, 1, 2, 1, ...

**TODO:** In future, have a one-line window for TUI to display the trace sequence
dynamically, and always.

**TODO:** Add a :program:`mcmini-gdb` tutorial on the next page, describing also break and continue, and including
some pictures of the GDB TUI at certain stages.  Start with something like:

**TODO:** Then show mcmini forward/back/where/break/continue on dining philosophers or
maybe on semaphore-pingpong.

.. parsed-literal:: **Annotated addition to the McMini output:**

   :command:`python3 mcmini-annotate` :option:`-p` <traceSeq> [:option:`-q`] :program:`target_executable`

In this variation, a more expansive description of the McMini output is
produced, including the function, file and line number at which each
transition occurred.

.. parsed-literal:: **McMini extensions to GDB for analyzing a trace:**

   :command:`mcmini-gdb` :option:`-p` <traceSeq> [:option:`-q`] :program:`target_executable`

In this variation, the command opens a GDB session.  Additional
GDB commands are provided.  Each new GDB command requires the
prefix `mcmini`.  For a quick orientation, do:

> (gdb) mcmini help

.. code:: shell

   mcmini -- mcmini <TAB> : show all mcmini commands
   mcmini back -- Go back one transition of current trace, by re-executing
   mcmini forward -- Execute until next transition; Accepts optional arg: <count>; or: end
   mcmini help -- Prints help for getting started in McMini
   mcmini printTransitions -- Prints the transitions currently on the stack
   mcmini where -- Execute where, while hiding McMini internal call frames

For further details, see:
 * :doc:`advanced features of McMini<usage/mcmini-advanced>`

----

.. _citations:

---------
Citations
---------

.. [mcmini_paper] \- "McMini: A Programmable DPOR-based Model Checker
   for Multithreaded Programs", Maxwell Pirtle, Luka Jovanovic, and Gene
   Cooperman, *The Art, Science, and Engineering of Programming* **8**\(1),
   Jun. 15, 2023, https://programming-journal.org/2024/8/1/

.. [dpor] \- "Dynamic Partial-order Reduction for Model Checking Software",
   Cormac Flanagan and Patrice Godefroid,
   *ACM Sigplan Notices* **40**.1 (2005): pp. |nbsp| 110-121.

-----------
Search page
-----------

.. COMMENT
   * :ref:`genindex`
   * :ref:`modindex`

* :ref:`search`

.. COMMENT: sidebar_collapse is supported by alabaster theme

.. toctree::
   :hidden:
   :maxdepth: 2

   index
   usage/tutorial-basic
   Tutorial: Analyzing a Bug <usage/tutorial-subtle>
   usage/mcmini-advanced
   usage/mcmini-extensions
