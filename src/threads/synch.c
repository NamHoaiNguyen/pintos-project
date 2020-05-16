/* This file is derived from source code for the Nachos
   instructional operating system.  The Nachos copyright notice
   is reproduced in full below. */

/* Copyright (c) 1992-1996 The Regents of the University of California.
   All rights reserved.

   Permission to use, copy, modify, and distribute this software
   and its documentation for any purpose, without fee, and
   without written agreement is hereby granted, provided that the
   above copyright notice and the following two paragraphs appear
   in all copies of this software.

   IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO
   ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
   CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE
   AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA
   HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS"
   BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
   MODIFICATIONS.
*/

#include "threads/synch.h"
#include <stdio.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

<<<<<<< HEAD

static bool thread_priority_compare (const struct list_elem *a, 
                                      const struct list_elem *b, void *aux UNUSED)
{
  struct thread *thread1 = list_entry(a, struct thread, elem);
  struct thread *thread2 = list_entry(b, struct thread, elem);

  return (thread1->priority < thread2->priority); 
}

=======
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
/* Initializes semaphore SEMA to VALUE.  A semaphore is a
   nonnegative integer along with two atomic operators for
   manipulating it:

   - down or "P": wait for the value to become positive, then
     decrement it.

   - up or "V": increment the value (and wake up one waiting
     thread, if any). */
void
sema_init (struct semaphore *sema, unsigned value) 
{
  ASSERT (sema != NULL);

  sema->value = value;
  list_init (&sema->waiters);
}

/* Down or "P" operation on a semaphore.  Waits for SEMA's value
   to become positive and then atomically decrements it.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but if it sleeps then the next scheduled
   thread will probably turn interrupts back on. */
void
sema_down (struct semaphore *sema) 
{
  enum intr_level old_level;

  ASSERT (sema != NULL);
  ASSERT (!intr_context ());

  old_level = intr_disable ();
  while (sema->value == 0) 
    {
<<<<<<< HEAD
      /* It makes no sense to do an ordered insert as the priority of
         the waiting thread can change at any time due to priority donation. */
=======
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
      list_push_back (&sema->waiters, &thread_current ()->elem);
      thread_block ();
    }
  sema->value--;
  intr_set_level (old_level);
}

/* Down or "P" operation on a semaphore, but only if the
   semaphore is not already 0.  Returns true if the semaphore is
   decremented, false otherwise.

   This function may be called from an interrupt handler. */
bool
sema_try_down (struct semaphore *sema) 
{
  enum intr_level old_level;
  bool success;

  ASSERT (sema != NULL);

  old_level = intr_disable ();
  if (sema->value > 0) 
    {
      sema->value--;
      success = true; 
    }
  else
    success = false;
  intr_set_level (old_level);

  return success;
}

/* Up or "V" operation on a semaphore.  Increments SEMA's value
<<<<<<< HEAD
   and wakes up the highest priority thread of those waiting for SEMA, 
   if any.
=======
   and wakes up one thread of those waiting for SEMA, if any.
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538

   This function may be called from an interrupt handler. */
void
sema_up (struct semaphore *sema) 
{
<<<<<<< HEAD
  // enum intr_level old_level;
  // struct list_elem *e;

  // ASSERT (sema != NULL);

  // old_level = intr_disable ();
  // sema->value++;
  // e = list_max (&sema->waiters, thread_priority_compare, NULL);
  // if (e != list_end (&sema->waiters))
  //   {
  //     list_remove (e);
  //     thread_unblock (list_entry (e, struct thread, elem));
  //   }
  // intr_set_level (old_level);

  enum intr_level old_level;
  struct list_elem *e;
=======
  enum intr_level old_level;
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538

  ASSERT (sema != NULL);

  old_level = intr_disable ();
<<<<<<< HEAD

  sema->value++;
  if (!list_empty(&sema->waiters)) {
    e = list_max(&sema->waiters, thread_priority_compare, NULL);
    if (e != list_end(&sema->waiters)) {
      list_remove(e);
      thread_unblock(list_entry(e, struct thread, elem));
    }
  }

  intr_set_level (old_level);

}

static struct thread *
sema_get_highest_priority_waiting_thread (struct semaphore *sema)
{
  struct list_elem *e;
  
  e = list_max (&sema->waiters, thread_priority_compare, NULL);
  if (e != list_end (&sema->waiters))
    return list_entry (e, struct thread, elem);
  else
    return NULL;
=======
  if (!list_empty (&sema->waiters)) 
    thread_unblock (list_entry (list_pop_front (&sema->waiters),
                                struct thread, elem));
  sema->value++;
  intr_set_level (old_level);
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
}

static void sema_test_helper (void *sema_);

/* Self-test for semaphores that makes control "ping-pong"
   between a pair of threads.  Insert calls to printf() to see
   what's going on. */
void
sema_self_test (void) 
{
  struct semaphore sema[2];
  int i;

  printf ("Testing semaphores...");
  sema_init (&sema[0], 0);
  sema_init (&sema[1], 0);
  thread_create ("sema-test", PRI_DEFAULT, sema_test_helper, &sema);
  for (i = 0; i < 10; i++) 
    {
      sema_up (&sema[0]);
      sema_down (&sema[1]);
    }
  printf ("done.\n");
}

/* Thread function used by sema_self_test(). */
static void
sema_test_helper (void *sema_) 
{
  struct semaphore *sema = sema_;
  int i;

  for (i = 0; i < 10; i++) 
    {
      sema_down (&sema[0]);
      sema_up (&sema[1]);
    }
}

/* Initializes LOCK.  A lock can be held by at most a single
   thread at any given time.  Our locks are not "recursive", that
   is, it is an error for the thread currently holding a lock to
   try to acquire that lock.

   A lock is a specialization of a semaphore with an initial
   value of 1.  The difference between a lock and such a
   semaphore is twofold.  First, a semaphore can have a value
   greater than 1, but a lock can only be owned by a single
   thread at a time.  Second, a semaphore does not have an owner,
   meaning that one thread can "down" the semaphore and then
   another one "up" it, but with a lock the same thread must both
   acquire and release it.  When these restrictions prove
   onerous, it's a good sign that a semaphore should be used,
   instead of a lock. */
void
lock_init (struct lock *lock)
{
  ASSERT (lock != NULL);

  lock->holder = NULL;
  sema_init (&lock->semaphore, 1);
}

/* Acquires LOCK, sleeping until it becomes available if
   necessary.  The lock must not already be held by the current
<<<<<<< HEAD
   thread.  If the thread will sleep and it has a higher priority
   than the owner of the lock, it will donate its priority to
   the owning thread to prevent priority inversion.
=======
   thread.
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
<<<<<<< HEAD


/* Called after a thread has acquired a lock, adds the lock to the list
   of locks owned by the thread. */
// void
// thread_lock_acquired (struct lock *lock)
// {
//   if (thread_mlfqs)
//     return;
  
//   ASSERT (intr_get_level () == INTR_OFF);
//   ASSERT (!intr_context ());
//   ASSERT (lock != NULL);
//   ASSERT (lock_get_holder (lock) == thread_current());
//   ASSERT (!lock_in_thread_locks_owned_list (lock));

//   /* Push to front as locks are usually released in reverse
//      order of acquisition. */
//   list_push_front (&thread_current ()->locks_owned_list, &lock->elem);
//   thread_current ()->waiting_lock = NULL;
// }

/* Called when a thread will wait on a lock.  If the effective priority of the
   thread is higher than that of the lock owner, the higher priority will be 
   donated to the owner and so on. */
// void
// thread_lock_will_wait (struct lock *lock)
// {    
//   struct thread *thread;
//   int nesting;

//   if (thread_mlfqs)
//     return;
  
//   ASSERT (intr_get_level () == INTR_OFF);
//   ASSERT (!intr_context ());
//   ASSERT (lock != NULL);
//   ASSERT (lock_get_holder (lock) != thread_current());
//   ASSERT (!lock_in_thread_locks_owned_list (lock));

//   thread = lock_get_holder (lock);
//   nesting = 0;





//   while (thread != NULL && nesting < PRI_MAX_DONATION_NESTING)
//     {
//       if (thread->status == THREAD_BLOCKED)
//         {
//           maybe_raise_priority (thread, thread_current ()->priority);
//           if (thread->waiting_lock != NULL)
//             thread = lock_get_holder (thread->waiting_lock);
//           else
//             thread = NULL;
//         }
//       else
//         {
//           if (thread->status == THREAD_READY
//               && maybe_raise_priority (thread, thread_current ()->priority))
//             shuffle_ready_thread (thread);
//           thread = NULL;
//         }
//       nesting++;
//     }
//   thread_current ()->waiting_lock = lock;
// }



void
lock_acquire (struct lock *lock)
{
  enum intr_level old_level;
  struct thread *thread;
  int nesting;

  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (!lock_held_by_current_thread (lock));
  
  

  old_level = intr_disable ();
  if (!sema_try_down (&lock->semaphore))
  {
    if (!thread_mlfqs){
    thread = lock_get_holder(lock);
    nesting = 0;
    while (thread != NULL && nesting < 8)
    {
      if (thread->status == THREAD_BLOCKED)
        {
       //   maybe_raise_priority (thread, thread_current ()->priority);
          if (thread_current ()->priority > thread->priority) {
            thread->priority = thread_current ()->priority;
          }
          if (thread->waiting_lock != NULL)
            thread = lock_get_holder (thread->waiting_lock);
          else
            thread = NULL;
        }
      else
        {
          if (thread->status == THREAD_READY)
            //  && maybe_raise_priority (thread, thread_current ()->priority))//
          {
             if (thread_current ()->priority > thread->priority){
            thread->priority = thread_current ()->priority;
            shuffle_ready_thread (thread);
            };
          }
          thread = NULL;
        }
      nesting++;
    }
  thread_current ()->waiting_lock = lock;
}
  sema_down (&lock->semaphore);
  }
  
  lock->holder = thread_current ();
  

  list_push_front (&thread_current ()->locks_owned_list, &lock->elem);
  thread_current ()->waiting_lock = NULL;

  intr_set_level (old_level);
=======
void
lock_acquire (struct lock *lock)
{
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (!lock_held_by_current_thread (lock));

  sema_down (&lock->semaphore);
  lock->holder = thread_current ();
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
}

/* Tries to acquires LOCK and returns true if successful or false
   on failure.  The lock must not already be held by the current
   thread.

   This function will not sleep, so it may be called within an
   interrupt handler. */
bool
lock_try_acquire (struct lock *lock)
{
  bool success;

  ASSERT (lock != NULL);
  ASSERT (!lock_held_by_current_thread (lock));

  success = sema_try_down (&lock->semaphore);
  if (success)
    lock->holder = thread_current ();
  return success;
}

<<<<<<< HEAD
/* Releases LOCK, which must be owned by the current thread 
   and wakes up the highest priority thread waiting on the lock,
   if any.
=======
/* Releases LOCK, which must be owned by the current thread.
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to release a lock within an interrupt
   handler. */
void
lock_release (struct lock *lock) 
{
<<<<<<< HEAD
  // enum intr_level old_level;
    
  // ASSERT (lock != NULL);
  // ASSERT (lock_held_by_current_thread (lock));

  // old_level = intr_disable ();
  // lock->holder = NULL;
  // sema_up (&lock->semaphore);
  // thread_lock_released (lock);
  // intr_set_level (old_level);

  enum intr_level old_level;
  struct list *lock_owned_list;
  struct list_elem *e, *max, *max1;
  struct lock *owned_lock;
  struct thread *t;
  int highest_priority = 0;

  ASSERT(lock != NULL);
  ASSERT(lock_held_by_current_thread (lock));



  old_level = intr_disable();
  lock->holder = NULL;
  sema_up(&lock->semaphore);


  //thread_lock_released(lock);

  if (!thread_mlfqs) {
  lock_owned_list = &thread_current()->locks_owned_list;
  for (e = list_begin(lock_owned_list); e != list_end(lock_owned_list); ) {
    owned_lock = list_entry(e, struct lock, elem);
    if (lock == owned_lock) {
      e = list_remove(e);
    } else {
     // t = lock_get_highest_priority_waiting_thread (owned_lock);

     max = list_max((&(owned_lock->semaphore).waiters), thread_priority_compare, NULL);
     if (max != list_end(&(owned_lock->semaphore).waiters)) {
        t = list_entry(max, struct thread, elem);
        
        if (t != NULL && t->priority > highest_priority) {
          highest_priority = t->priority;
        }
      }
      e = list_next(e);
    }
  }

  if (highest_priority < thread_current()->base_priority) {
    thread_current()->priority = thread_current()->base_priority;
  } else {
    thread_current()->priority = highest_priority;
  }

  maybe_yield_to_ready_thread ();

  }
  intr_set_level(old_level);
}

struct thread *
lock_get_holder (struct lock *lock)
{
  ASSERT (intr_get_level () == INTR_OFF);
  ASSERT (lock != NULL);

  return lock->holder;
}

struct thread *
lock_get_highest_priority_waiting_thread (struct lock *lock)
{
  ASSERT (intr_get_level () == INTR_OFF);
  ASSERT (lock != NULL);

  return sema_get_highest_priority_waiting_thread (&lock->semaphore);
=======
  ASSERT (lock != NULL);
  ASSERT (lock_held_by_current_thread (lock));

  lock->holder = NULL;
  sema_up (&lock->semaphore);
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
}

/* Returns true if the current thread holds LOCK, false
   otherwise.  (Note that testing whether some other thread holds
   a lock would be racy.) */
bool
lock_held_by_current_thread (const struct lock *lock) 
{
  ASSERT (lock != NULL);

  return lock->holder == thread_current ();
}

/* One semaphore in a list. */
struct semaphore_elem 
<<<<<<< HEAD
{
  struct list_elem elem;              /* List element. */
  struct semaphore semaphore;         /* This semaphore. */
  struct thread *thread;              /* Thread waiting on semaphore. */
};

static bool
waiters_priority_compare (const struct list_elem *a, const struct list_elem *b,
                          void *aux UNUSED)
{
  return list_entry (a, struct semaphore_elem, elem)->thread->priority
    < list_entry (b, struct semaphore_elem, elem)->thread->priority;
}
=======
  {
    struct list_elem elem;              /* List element. */
    struct semaphore semaphore;         /* This semaphore. */
  };
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538

/* Initializes condition variable COND.  A condition variable
   allows one piece of code to signal a condition and cooperating
   code to receive the signal and act upon it. */
void
cond_init (struct condition *cond)
{
  ASSERT (cond != NULL);

  list_init (&cond->waiters);
}

/* Atomically releases LOCK and waits for COND to be signaled by
   some other piece of code.  After COND is signaled, LOCK is
   reacquired before returning.  LOCK must be held before calling
   this function.

   The monitor implemented by this function is "Mesa" style, not
   "Hoare" style, that is, sending and receiving a signal are not
   an atomic operation.  Thus, typically the caller must recheck
   the condition after the wait completes and, if necessary, wait
   again.

   A given condition variable is associated with only a single
   lock, but one lock may be associated with any number of
   condition variables.  That is, there is a one-to-many mapping
   from locks to condition variables.

   This function may sleep, so it must not be called within an
   interrupt handler.  This function may be called with
   interrupts disabled, but interrupts will be turned back on if
   we need to sleep. */
void
cond_wait (struct condition *cond, struct lock *lock) 
{
  struct semaphore_elem waiter;

  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));
  
  sema_init (&waiter.semaphore, 0);
<<<<<<< HEAD
  waiter.thread = thread_current ();

  /* It makes no sense to do an ordered insert as the priority of
     the waiting thread can change at any time due to priority donation. */
  list_push_back (&cond->waiters, &waiter.elem);
  
=======
  list_push_back (&cond->waiters, &waiter.elem);
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
  lock_release (lock);
  sema_down (&waiter.semaphore);
  lock_acquire (lock);
}

/* If any threads are waiting on COND (protected by LOCK), then
<<<<<<< HEAD
   this function signals the highest priority thread to wake up. 
=======
   this function signals one of them to wake up from its wait.
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
   LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_signal (struct condition *cond, struct lock *lock UNUSED) 
{
<<<<<<< HEAD
  enum intr_level old_level;
  struct list_elem *e;
  
=======
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);
  ASSERT (!intr_context ());
  ASSERT (lock_held_by_current_thread (lock));

<<<<<<< HEAD
  old_level = intr_disable ();
  e = list_max (&cond->waiters, waiters_priority_compare, NULL);
  intr_set_level (old_level);
  if (e != list_end (&cond->waiters))
    {
      list_remove (e);
      sema_up (&list_entry (e, struct semaphore_elem, elem)->semaphore);
    }
=======
  if (!list_empty (&cond->waiters)) 
    sema_up (&list_entry (list_pop_front (&cond->waiters),
                          struct semaphore_elem, elem)->semaphore);
>>>>>>> 930079f5cddde336bd0f6ae751434c089ff3a538
}

/* Wakes up all threads, if any, waiting on COND (protected by
   LOCK).  LOCK must be held before calling this function.

   An interrupt handler cannot acquire a lock, so it does not
   make sense to try to signal a condition variable within an
   interrupt handler. */
void
cond_broadcast (struct condition *cond, struct lock *lock) 
{
  ASSERT (cond != NULL);
  ASSERT (lock != NULL);

  while (!list_empty (&cond->waiters))
    cond_signal (cond, lock);
}
