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
