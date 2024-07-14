#include <stdio.h>
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *child_thread(void *dummy) {
  pthread_mutex_lock(&mutex);
  printf("child: I locked the mutex!\n");
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main() {
  pthread_t thread;
  pthread_create(&thread, NULL, child_thread, NULL);
  pthread_mutex_lock(&mutex);
  printf("parent: I locked the mutex!\n");
  pthread_mutex_unlock(&mutex);
  pthread_join(thread, NULL);
  printf("*** Done.\n");
  return 0;
}
