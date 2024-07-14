// https://gcc.gnu.org/onlinedocs/gcc-4.1.1/gcc/Atomic-Builtins.html
// 
// Shared Variables, Lock-free Algorithms, etc.

// Usage:  gcc -O3 THIS_FILE  -pthread
//         (Note: -pthread required only if MCMINI defined.)

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

#define NUM_THREADS 2
#define NUM_TASKS 2

#define MCMINI
#ifdef MCMINI
pthread_rwlock_t mylock = PTHREAD_RWLOCK_INITIALIZER;
#endif

typedef struct stackItem {
  struct stackItem *next;
  char data[10];
} stackItem_t;

// Stack
stackItem_t *topOfStack = NULL;

void push(stackItem_t *item) {
  while (1) {
    stackItem_t *curTopOfStack = topOfStack;
#ifdef MCMINI
    // Reading topOfStack
    pthread_rwlock_rdlock(&mylock); pthread_rwlock_unlock(&mylock);
#endif
    item->next = topOfStack;
    // if (topOfStack == curTopOfStack) { topOfStack = item; } // but atomic
    if ( __sync_bool_compare_and_swap(&topOfStack, curTopOfStack, item) ) {
#ifdef MCMINI
      // Atomically reading/writing topOfStack
      pthread_rwlock_wrlock(&mylock); pthread_rwlock_unlock(&mylock);
#endif
      topOfStack = item;
      return; // return on success
    }
#ifdef MCMINI
    // Atomically reading/writing topOfStack
    pthread_rwlock_wrlock(&mylock); pthread_rwlock_unlock(&mylock);
#endif
  }
}

stackItem_t *pop() {
  while (1) {
    stackItem_t *curTopOfStack = topOfStack;
#ifdef MCMINI
    // Reading topOfStack
    pthread_rwlock_rdlock(&mylock); pthread_rwlock_unlock(&mylock);
#endif
    if (curTopOfStack == NULL) {
      return curTopOfStack;
    }
    stackItem_t *nextItem = curTopOfStack->next;
    // if (topOfStack == curTopOfStack) { topOfStack = nextItem; } // but atomic
    if ( __sync_bool_compare_and_swap(&topOfStack, curTopOfStack, nextItem) ) {
#ifdef MCMINI
      // if (topOfStack == curTopOfStack) { topOfStack = nextItem; } // but atomic
      pthread_rwlock_wrlock(&mylock); pthread_rwlock_unlock(&mylock); // Writing vars
#endif
      curTopOfStack->next = NULL;
      return curTopOfStack; // return on success
    }
#ifdef MCMINI
    // Atomically reading/writing topOfStack
    pthread_rwlock_wrlock(&mylock); pthread_rwlock_unlock(&mylock);
#endif
  }
}

void init_stack() {
  for (int i = 1; i < 4; i++) {
    stackItem_t *item = malloc(sizeof(*item));
    int data_size = snprintf(item->data, sizeof(item->data), "%d", i);
    push(item);
    assert(data_size < sizeof(item->data));
  }
  return;
}

void *do_work(void *arg) {
  char self = *(int *)arg;
  self = self + 0; // Avoids warning of unused variable.
  stackItem_t *item = NULL; // FIXME: not used??
  stackItem_t *last_pop = NULL;
  for (int task = 0; task < NUM_TASKS; task++) {
#if 0
    if (random() % 2 == 0) {
      if (item) {
        push(item);
#ifdef VERBOSE
        printf("Thread %c pushed item %s onto stack.\n", self, item->data);
#endif
      }
    } else {
      item = pop();
#ifdef VERBOSE
      if (item) {
        printf("Thread %c popped item %s from stack.\n", self, item->data);
      }
#endif
    }
#else
    if (self == 'A' && task == 0) {
        last_pop = pop();
#ifdef VERBOSE
        printf("Thread %c pushed item %s onto stack.\n", self, item->data);
#endif
    } else if (self == 'B' && task == 0) {
        last_pop = pop();
#ifdef VERBOSE
        printf("Thread %c pushed item %s onto stack.\n", self, item->data);
#endif
    } else if (self == 'B' && task == 1) {
        push(last_pop);
#ifdef VERBOSE
        printf("Thread %c pushed item %s onto stack.\n", self, item->data);
#endif
    }
#endif
  }
  return NULL;
}

int main() {
  pthread_t thread[NUM_THREADS];
  char thread_number[NUM_THREADS];

  // FIXME:  McMini should recognize PTHREAD_RWLOCK_INITIALIZER
  pthread_rwlock_init(&mylock, NULL);

  init_stack();

  for (int i = 0; i < NUM_THREADS; i++) {
    thread_number[i] = 'A' + i;
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&thread[i], NULL, do_work, &thread_number[i]);
  }
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(thread[i], NULL);
  }
  return 0;
}
