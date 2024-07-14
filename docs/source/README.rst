McMini
======

:ref:`**McMini**<mcmini_paper>` is an explicit-state model checker, based on :ref:`DPOR<dpor>`.
It analyzes a target that
is a multi-threaded executable (binary), and identifies thread schedules
that produce deadlock, segfaults, assertion failures, and other errors.

Unlike some model checkers, it runs directly on a Linux binary.  It is as easy to use as:

.. parsed-literal::

   :command:`mcmini` :program:`./a.out`

If you use McMini, please cite :ref:`the McMini paper<mcmini_paper>`.
