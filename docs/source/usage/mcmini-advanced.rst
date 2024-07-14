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


---------------------------------------------------------
Analyzing a trace: ``mcmini --print-at-trace <traceSeq>``
---------------------------------------------------------

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
and repeat `-p <traceSeq>`, in order to "steer" McMini
into the path of interest.

-----------------------------------------
Annotate for improved "THREAD BACKTRACE"
-----------------------------------------

  :command:`python3 mcmini-annotate.py -p <traceSeq>`

----------------------------------------------
Using GDB with ``--print-at-trace <traceSeq>``
----------------------------------------------

**TODO:**

Start with::
  :command:`mcmini-gdb  -p <traceSeq>`

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
FUTURE:  Livelock
-----------------------------------------------------

The McMini architecture is also sufficient to add a *heuristic* for
detecting livelock.  **We have a roadmap for a small addition that
integrates This capability into McMini, and we hope to have this feature
in the future.**
(**NOTE:** *McMini is open-source, and we welcome additional
writers of documentation.  We can guide you in this modest
enhancement of McMini.*)
