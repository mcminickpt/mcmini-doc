====================
Extensions to McMini
====================

.. toctree::
   :hidden:
   :maxdepth: 2

   mcmini-extensions

**TODO:** *This is still a work in progress.  It is hoped that these
descriptions are sufficient for anyone to quickly hard-wire these features
into their target multi-threaded application.  It is planned to more
fully integrate these features directly into McMini in the future.*
(**NOTE:** *McMini is open-source, and we welcome additional
developers.*)


.. contents:: Contents of this page
   :backlinks: entry
   :local:
   :depth: 2

-------------------------------------
Shared variables:  READ() and WRITE()
-------------------------------------

**TODO:** *This section is still experimental.
We have not tested the older code originally created in the
academic prototype of McMini.*

**TODO:** Use `pthread_rwlock_rdlock` after a read and `pthread_rwlock_wrlock` after a write.

**TODO:** We could add a tutorial for this, using the ABA problem as a running example.  THe ABA problem is described in:
  https://en.wikipedia.org/wiki/ABA_problem
and:
  https://course.khoury.northeastern.edu/cs7600/parent/thread-synch/aba-problem.pdf

*In this tutorial, we could implement
'#ifdef MCMINI; pthread_rwlock_rdlock(); #endif', etc.
And then we get to use 'choose()' to decide if a thread should do 'push()', 'pop()', etc.  And we can include diagram for ABA problem.*

--------------------------------
McMini choose(): Modeling inputs
--------------------------------

Until now, we have looked only at target programs that did not read
from their standard input.
A common question is what to do to model check the various possible inputs
to a program that does take an input.
For example, in a typical producer-consumer program, one might wish to
test the program for different patterns of a producer thread and a
consumer thread requesting an operation.

One possibility would be to encode the input as arguments on the command
line, and then to write a script that calls McMini for each of
the interesting patterns of arguments, representing different inputs.

Luckily, there is an easier way to do this.  One can use a
formal `choose()` function within the program to be tested.
The `choose()` function has a signature:

.. code:: C

   int choose(int count);

The return value can range from `0` to `count-1` (inclusive).
In this case, McMini will view the `choose()` function as
a pseudo-transition, with a different trace for
each of the return values from `0` to `count-1`.

Currently, McMini does not have a built-in `choose()` function.
However, it is easy for the user to include in their program
the `choose()` function supplied in the following code.

* :doc:`the choose() program<choose.c>`

In addition, we must include the mutex, the two
semaphores, and the extra thread executing
`choose_helper_thread()`.

The supplied program, `choose.c`, is self-contained.
One can compile it into a target, `choose`, and then execute

> ./mcmini -v ./choose

to verify that the function does indeed create a new
trace for each of the possible return values of `choose()`.
(In this case, the program has defined `CHOOSE_ARG` to `3`,
and so there will be three traces in the McMini output.

---------------------------------
Policy on which thread to wake up
---------------------------------

**TODO:** *Fill out this section.*

(**NOTE:** *McMini is open-source, and we welcome additional
writers of documentation.  A good starting point for the McMini manual
is the McMini paper, cited in the* :ref:`citations` *section.*)

*The current default policy is "arbitrary" (create a new
branch to search according to which thread is woken up.*

*The arbitrary policy requires mmore branches, and hence
is slower.  A FIFO policy allows McMini to search deeper.*
