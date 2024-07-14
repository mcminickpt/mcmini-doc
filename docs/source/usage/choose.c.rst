:orphan:

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Implementation of `choose()` function
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code:: C
   :number-lines:

   // USAGE:  gcc -g3 -o choose choose.c -pthread
   
   #include <pthread.h>
   #include <semaphore.h>
   #include <stdio.h>
   
   #define CHOOSE_ARG 3
   
   pthread_mutex_t mut_choose = PTHREAD_MUTEX_INITIALIZER;
   sem_t sem_choose_out;
   sem_t sem_choose_in;
   
   // Shared variable: protected by the mut_choose mutex.
   int choose_arg_retvalue;
   
   void *choose_helper_thread(void *notused) {
     int choose_arg;
   
     while (1) {
       // Get argument of choose
       sem_wait(&sem_choose_out);
   
       pthread_mutex_lock(&mut_choose);
       choose_arg = choose_arg_retvalue;
       pthread_mutex_unlock(&mut_choose);
   
   printf("choose(%d) received %d.\n", CHOOSE_ARG, choose_arg);
   
       sem_post(&sem_choose_in);
   
       choose_arg_retvalue = 0;
       for (int i = 1; i < choose_arg; i++) {
         pthread_mutex_lock(&mut_choose);
         choose_arg_retvalue = i;
         pthread_mutex_unlock(&mut_choose);
       }
   
       sem_post(&sem_choose_in);
     }
     return NULL;
   }
   
   int choose(int choose_arg) {
     pthread_mutex_lock(&mut_choose);
     choose_arg_retvalue = choose_arg;
     pthread_mutex_unlock(&mut_choose);
   
     sem_post(&sem_choose_out);
     sem_wait(&sem_choose_in);
   
     int choose_retvalue = -1;
     pthread_mutex_lock(&mut_choose);
     choose_retvalue = choose_arg_retvalue;
     pthread_mutex_unlock(&mut_choose);
   
     sem_wait(&sem_choose_in);
     return choose_retvalue;
   }
   
   int main() {
     pthread_t choose_thread;
     sem_init(&sem_choose_in, 0, 0);
     sem_init(&sem_choose_out, 0, 0);
   #if 1
     pthread_mutex_init(&mut_choose, NULL);
   #endif
     pthread_create(&choose_thread, NULL, &choose_helper_thread, NULL);
   
   #if 0
     pthread_mutex_lock(&mut_choose);
     pthread_mutex_unlock(&mut_choose);
   #endif
   
     int choose_retvalue = choose(3);
   
     printf("choose(%d) returned %d.\n", CHOOSE_ARG, choose_retvalue);
     return 0;
   }
