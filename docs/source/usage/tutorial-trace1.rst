:orphan:

-------------------------------------
Tutorial code trace (bufferSize == 1)
-------------------------------------

.. code:: shell

   USER% ./mcmini -q -f -m18 ./subtle 1

   About to exec into ./subtle
   TraceId 13, *** DEADLOCK DETECTED ***
   THREAD BACKTRACE
    1. thread 0: starts
    2. thread 0: pthread_mutex_init(mut:1, _)
    3. thread 0: pthread_mutex_init(mut:2, _)
    4. thread 0: pthread_mutex_init(mut:3, _)
    5. thread 0: pthread_mutex_init(mut:4, _)
    6. thread 0: pthread_cond_init(cond:1, _)
    7. thread 0: pthread_cond_init(cond:2, _)
    8. thread 0: pthread_create(thr:1, _, _, _)
    9. thread 0: pthread_create(thr:2, _, _, _)
   10. thread 1: starts
   11. thread 1: pthread_mutex_lock(mut:1)
   12. thread 1: pthread_mutex_unlock(mut:1)
   13. thread 1: pthread_mutex_lock(mut:3)
   14. thread 1: pthread_mutex_unlock(mut:3)
   15. thread 1: pthread_mutex_lock(mut:4)
   16. thread 1: pthread_mutex_unlock(mut:4)
   17. thread 1: pthread_mutex_lock(mut:1)
   18. thread 1: pthread_mutex_unlock(mut:1)
   19. thread 1: pthread_mutex_lock(mut:2)
   20. thread 1: pthread_cond_signal(cond:1) [Lost wakeup? (No thread waiting on cond) ]
   21. thread 1: pthread_mutex_unlock(mut:2)
   22. thread 1: pthread_mutex_lock(mut:1)
   23. thread 1: pthread_mutex_unlock(mut:1)
   24. thread 1: pthread_mutex_lock(mut:3)
   25. thread 1: pthread_mutex_unlock(mut:3)
   26. thread 2: starts
   27. thread 2: pthread_mutex_lock(mut:3)
   28. thread 2: pthread_mutex_unlock(mut:3)
   29. thread 2: pthread_mutex_lock(mut:1)
   30. thread 2: pthread_mutex_unlock(mut:1)
   31. thread 2: pthread_mutex_lock(mut:2)
   32. thread 2: pthread_mutex_unlock(mut:2)
   33. thread 2: pthread_mutex_lock(mut:3)
   34. thread 2: pthread_mutex_unlock(mut:3)
   35. thread 2: pthread_mutex_lock(mut:4)
   36. thread 2: pthread_cond_signal(cond:2) [Lost wakeup? (No thread waiting on cond) ]
   37. thread 2: pthread_mutex_unlock(mut:4)
   38. thread 1: pthread_mutex_lock(mut:4)
   39. thread 1: pthread_cond_wait(cond:2, mut:4) (awake -> asleep)
   40. thread 2: pthread_mutex_lock(mut:3)
   41. thread 2: pthread_mutex_unlock(mut:3)
   42. thread 2: pthread_mutex_lock(mut:1)
   43. thread 2: pthread_mutex_unlock(mut:1)
   44. thread 2: pthread_mutex_lock(mut:2)
   45. thread 2: pthread_cond_wait(cond:1, mut:2) (awake -> asleep)
   0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2,
   END
   THREAD PENDING OPERATIONS
    * thread 0: pthread_join(thr:1, _) [ Blocked ]
      thread 1: pthread_cond_wait(cond:2, mut:4) (asleep -> awake) [ Blocked ]
      thread 2: pthread_cond_wait(cond:1, mut:2) (asleep -> awake) [ Blocked ]
   END
   ***** Model checking completed! *****
   *** DEADLOCK DETECTED ***
     (Trace number (traceId): 13)
   Number of traces: 14
   Total number of transitions: 116
   Elapsed time: 0 seconds
