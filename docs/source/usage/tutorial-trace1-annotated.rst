:orphan:

------------------------------------------------
Tutorial code trace (annotated; bufferSize == 1)
------------------------------------------------

.. code:: shell

  USER% ./mcmini-annotate -q -f -m18 ./subtle 1

  ** Generating trace sequence for:
       gdb -x /home/gene/mcmini.git/gdbinit -x /home/gene/mcmini.git/gdbinit_annotate --args /home/gene/mcmini.git/mcmini -t 0  -q -f -m18 ./subtle 1
       (This may take a while ...)
  
  *** transition: 1; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:219]:
             ... pthread_mutex_init(&allVars.inEvents.critical, NULL)
  *** transition: 2; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:220]:
             ... pthread_mutex_init(&allVars.inEvents.signalM, NULL)
  *** transition: 3; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:221]:
             ... pthread_mutex_init(&allVars.outEvents.critical, NULL)
  *** transition: 4; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:222]:
             ... pthread_mutex_init(&allVars.outEvents.signalM, NULL)
  *** transition: 5; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:223]:
             ... pthread_cond_init(&allVars.inEvents.signalC, NULL)
  *** transition: 6; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:224]:
             ... pthread_cond_init(&allVars.outEvents.signalC, NULL)
  *** transition: 7; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:229]:
             ... pthread_create(&thread, NULL, childThread, &allVars)
  *** transition: 8; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:234]:
             ... pthread_create(&thread2, NULL, childThread2, &allVars)
  *** transition: 9; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside start_thread() [pthread_create.c:442]:
             ... mc_thread_routine_wrapper(...) [thread starts]
  *** transition: 10; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside putBuffer() [subtle.c:118]:
             ... pthread_mutex_lock(&allVars->inEvents.critical)
  *** transition: 11; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside putBuffer() [subtle.c:120]:
             ... pthread_mutex_unlock(&allVars->inEvents.critical)
  *** transition: 12; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:89]:
             ... pthread_mutex_lock(&outEvents->critical)
  *** transition: 13; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:91]:
             ... pthread_mutex_unlock(&outEvents->critical)
  *** transition: 14; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:94]:
             ... pthread_mutex_lock(&outEvents->signalM)
  *** transition: 15; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:108]:
             ... pthread_mutex_unlock(&outEvents->signalM)
  *** transition: 16; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside advanceIn() [subtle.c:32]:
             ... pthread_mutex_lock(&inEvents->critical)
  *** transition: 17; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside advanceIn() [subtle.c:34]:
             ... pthread_mutex_unlock(&inEvents->critical)
  *** transition: 18; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside advanceIn() [subtle.c:37]:
             ... pthread_mutex_lock(&inEvents->signalM)
  *** transition: 19; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside advanceIn() [subtle.c:38]:
             ... pthread_cond_signal(&inEvents->signalC)
  *** transition: 20; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside advanceIn() [subtle.c:39]:
             ... pthread_mutex_unlock(&inEvents->signalM)
  *** transition: 21; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside putBuffer() [subtle.c:118]:
             ... pthread_mutex_lock(&allVars->inEvents.critical)
  *** transition: 22; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside putBuffer() [subtle.c:120]:
             ... pthread_mutex_unlock(&allVars->inEvents.critical)
  *** transition: 23; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:89]:
             ... pthread_mutex_lock(&outEvents->critical)
  *** transition: 24; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:91]:
             ... pthread_mutex_unlock(&outEvents->critical)
  *** transition: 25; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:94]:
             ... pthread_mutex_lock(&outEvents->signalM)
  *** transition: 26; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:99]:
             ... pthread_cond_wait(&outEvents->signalC, &outEvents->signalM)
  *** transition: 27; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside start_thread() [pthread_create.c:442]:
             ... mc_thread_routine_wrapper(...) [thread starts]
  *** transition: 28; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside getBuffer() [subtle.c:139]:
             ... pthread_mutex_lock(&allVars->outEvents.critical)
  *** transition: 29; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside getBuffer() [subtle.c:141]:
             ... pthread_mutex_unlock(&allVars->outEvents.critical)
  *** transition: 30; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside awaitIn() [subtle.c:63]:
             ... pthread_mutex_lock(&inEvents->critical)
  *** transition: 31; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside awaitIn() [subtle.c:65]:
             ... pthread_mutex_unlock(&inEvents->critical)
  *** transition: 32; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside awaitIn() [subtle.c:68]:
             ... pthread_mutex_lock(&inEvents->signalM)
  *** transition: 33; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside awaitIn() [subtle.c:82]:
             ... pthread_mutex_unlock(&inEvents->signalM)
  *** transition: 34; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside advanceOut() [subtle.c:43]:
             ... pthread_mutex_lock(&outEvents->critical)
  *** transition: 35; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside advanceOut() [subtle.c:45]:
             ... pthread_mutex_unlock(&outEvents->critical)
  *** transition: 36; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside advanceOut() [subtle.c:48]:
             ... pthread_mutex_lock(&outEvents->signalM)
  *** transition: 37; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside advanceOut() [subtle.c:49]:
             ... pthread_cond_signal(&outEvents->signalC)
  *** transition: 38; thread: 2.3 (thread 3 of inferior 2)
    > Thr 2: ... Inside advanceOut() [subtle.c:50]:
             ... pthread_mutex_unlock(&outEvents->signalM)
  *** transition: 39; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:99]:
             ... pthread_cond_wait(&outEvents->signalC, &outEvents->signalM)
  *** transition: 40; thread: 2.2 (thread 2 of inferior 2)
    > Thr 1: ... Inside awaitOut() [subtle.c:102]:
             ... pthread_mutex_lock(&outEvents->critical)
  *** transition: 41; thread: 2.1 (thread 1 of inferior 2)
    > Thr 0: ... Inside main() [subtle.c:239]:
             ... pthread_join(thread, NULL)
