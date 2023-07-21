/*! \file    mutex.h
 * \author   Lorenzo Miniero <lorenzo@meetecho.com>
 * \brief    Semaphors, Mutexes and Conditions
 * \details  Implementation (based on GMutex or pthread_mutex) of a locking mechanism based on mutexes and conditions.
 *
 * \ingroup core
 * \ref core
 */

#ifndef JANUS_MUTEX_H
#define JANUS_MUTEX_H

#include <pthread.h>
#include <errno.h>

#include "debug.h"

extern int lock_debug;

#ifdef USE_PTHREAD_MUTEX

/*! \brief Janus mutex implementation */
typedef pthread_mutex_t janus_mutex;
/*! \brief Janus mutex initialization */
#define janus_mutex_init(a) pthread_mutex_init(a,NULL)
/*! \brief Janus static mutex initializer */
#define JANUS_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
/*! \brief Janus mutex destruction */
#define janus_mutex_destroy(a) pthread_mutex_destroy(a)
/*! \brief Janus mutex lock without debug */
#define janus_mutex_lock_nodebug(a) pthread_mutex_lock(a)
/*! \brief Janus mutex lock with debug (prints the line that locked a mutex) */
#define janus_mutex_lock_debug(a) { JANUS_PRINT("[%s:%s:%d:lock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); pthread_mutex_lock(a); }
/*! \brief Janus mutex lock wrapper (selective locking debug) */
#define janus_mutex_lock(a) { if(!lock_debug) { janus_mutex_lock_nodebug(a); } else { janus_mutex_lock_debug(a); } }
/*! \brief Janus mutex try lock without debug */
#define janus_mutex_trylock_nodebug(a) { ret = !pthread_mutex_trylock(a); }
/*! \brief Janus mutex try lock with debug (prints the line that tried to lock a mutex) */
#define janus_mutex_trylock_debug(a) { JANUS_PRINT("[%s:%s:%d:trylock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); ret = !pthread_mutex_trylock(a); }
/*! \brief Janus mutex try lock wrapper (selective locking debug) */
#define janus_mutex_trylock(a) ({ int ret; if(!lock_debug) { janus_mutex_trylock_nodebug(a); } else { janus_mutex_trylock_debug(a); } ret; })
/*! \brief Janus mutex unlock without debug */
#define janus_mutex_unlock_nodebug(a) pthread_mutex_unlock(a)
/*! \brief Janus mutex unlock with debug (prints the line that unlocked a mutex) */
#define janus_mutex_unlock_debug(a) { JANUS_PRINT("[%s:%s:%d:unlock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); pthread_mutex_unlock(a); }
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_mutex_unlock(a) { if(!lock_debug) { janus_mutex_unlock_nodebug(a); } else { janus_mutex_unlock_debug(a); } }

/*! \brief Janus condition implementation */
typedef pthread_cond_t janus_condition;
/*! \brief Janus condition initialization */
#define janus_condition_init(a) pthread_cond_init(a,NULL)
/*! \brief Janus condition destruction */
#define janus_condition_destroy(a) pthread_cond_destroy(a)
/*! \brief Janus condition wait */
#define janus_condition_wait(a, b) pthread_cond_wait(a, b);
/*! \brief Janus condition timed wait */
#define janus_condition_wait_until(a, b, c) { \
	const struct timespec jct = { \
		.tv_sec = c / G_USEC_PER_SEC, \
		.tv_nsec = (c % G_USEC_PER_SEC)*1000 \
	}; \
	pthread_cond_timedwait(a, b, &jct); \
}
/*! \brief Janus condition signal */
#define janus_condition_signal(a) pthread_cond_signal(a);
/*! \brief Janus condition broadcast */
#define janus_condition_broadcast(a) pthread_cond_broadcast(a);

#else

/*! \brief Janus mutex implementation */
typedef GMutex janus_mutex;
/*! \brief Janus mutex initialization */
#define janus_mutex_init(a) g_mutex_init(a)
/*! \brief Janus static mutex initializer */
#define JANUS_MUTEX_INITIALIZER {0}
/*! \brief Janus mutex destruction */
#define janus_mutex_destroy(a) g_mutex_clear(a)
/*! \brief Janus mutex lock without debug */
#define janus_mutex_lock_nodebug(a) g_mutex_lock(a)
/*! \brief Janus mutex lock with debug (prints the line that locked a mutex) */
#define janus_mutex_lock_debug(a) { JANUS_PRINT("[%s:%s:%d:lock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); g_mutex_lock(a); }
/*! \brief Janus mutex lock wrapper (selective locking debug) */
#define janus_mutex_lock(a) { if(!lock_debug) { janus_mutex_lock_nodebug(a); } else { janus_mutex_lock_debug(a); } }
/*! \brief Janus mutex try lock without debug */
#define janus_mutex_trylock_nodebug(a) { ret = g_mutex_trylock(a); }
/*! \brief Janus mutex try lock with debug (prints the line that tried to lock a mutex) */
#define janus_mutex_trylock_debug(a) { JANUS_PRINT("[%s:%s:%d:trylock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); ret = g_mutex_trylock(a); }
/*! \brief Janus mutex try lock wrapper (selective locking debug) */
#define janus_mutex_trylock(a) ({ gboolean ret; if(!lock_debug) { janus_mutex_trylock_nodebug(a); } else { janus_mutex_trylock_debug(a); } ret; })
/*! \brief Janus mutex unlock without debug */
#define janus_mutex_unlock_nodebug(a) g_mutex_unlock(a)
/*! \brief Janus mutex unlock with debug (prints the line that unlocked a mutex) */
#define janus_mutex_unlock_debug(a) { JANUS_PRINT("[%s:%s:%d:unlock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); g_mutex_unlock(a); }
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_mutex_unlock(a) { if(!lock_debug) { janus_mutex_unlock_nodebug(a); } else { janus_mutex_unlock_debug(a); } }

/*! \brief Janus condition implementation */
typedef GCond janus_condition;
/*! \brief Janus condition initialization */
#define janus_condition_init(a) g_cond_init(a)
/*! \brief Janus condition destruction */
#define janus_condition_destroy(a) g_cond_clear(a)
/*! \brief Janus condition wait */
#define janus_condition_wait(a, b) g_cond_wait(a, b);
/*! \brief Janus condition wait until */
#define janus_condition_wait_until(a, b, c) g_cond_wait_until(a, b, c);
/*! \brief Janus condition signal */
#define janus_condition_signal(a) g_cond_signal(a);
/*! \brief Janus condition broadcast */
#define janus_condition_broadcast(a) g_cond_broadcast(a);

/*! \brief Janus mutex implementation */
typedef GRWLock janus_rwlock;

/*! \brief Janus mutex initialization */
/*! \brief Janus mutex lock without debug */
#define janus_rwlock_init_nodebug(a) g_rw_lock_init(a)
/*! \brief Janus mutex lock with debug (prints the line that locked a mutex) */
#define janus_rwlock_init_debug(a) { JANUS_PRINT("[%s:%s:%d:rw_init] %p\n", __FILE__, __FUNCTION__, __LINE__, a); g_rw_lock_init(a); }
/*! \brief Janus mutex destruction */
#define janus_rwlock_init(a) { if(!lock_debug) { janus_rwlock_init_nodebug(a); } else { janus_rwlock_init_debug(a); } }
/*! \brief Janus static mutex initializer */
#define JANUS_RWLOCK_INITIALIZER {0}
/*! \brief Janus mutex lock without debug */
#define janus_rwlock_destroy_nodebug(a) g_rw_lock_clear(a)
/*! \brief Janus mutex lock with debug (prints the line that locked a mutex) */
#define janus_rwlock_destroy_debug(a) { JANUS_PRINT("[%s:%s:%d:rw_dectroy] %p\n", __FILE__, __FUNCTION__, __LINE__, a); janus_rwlock_destroy_nodebug(a); }
/*! \brief Janus mutex destruction */
#define janus_rwlock_destroy(a) { if(!lock_debug) { janus_rwlock_destroy_nodebug(a); } else { janus_rwlock_destroy_debug(a); } }
/*! \brief Janus mutex lock without debug */
#define janus_rwlock_reader_lock_nodebug(a) g_rw_lock_reader_lock(a)
/*! \brief Janus mutex lock with debug (prints the line that locked a mutex) */
#define janus_rwlock_reader_lock_debug(a) { JANUS_PRINT("[%s:%s:%d:reader_lock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); janus_rwlock_reader_lock_nodebug(a); }
/*! \brief Janus mutex lock wrapper (selective locking debug) */
#define janus_rwlock_reader_lock(a) { if(!lock_debug) { janus_rwlock_reader_lock_nodebug(a); } else { janus_rwlock_reader_lock_debug(a); } }
/*! \brief Janus mutex try lock without debug */
#define janus_rwlock_reader_trylock_nodebug(a) { ret = g_rw_lock_reader_trylock(a); }
/*! \brief Janus mutex try lock with debug (prints the line that tried to lock a mutex) */
#define janus_rwlock_reader_trylock_debug(a) { JANUS_PRINT("[%s:%s:%d:reader_trylock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); ret = janus_rwlock_reader_trylock(a); }
/*! \brief Janus mutex try lock wrapper (selective locking debug) */
#define janus_rwlock_reader_trylock(a) ({ if(!lock_debug) { janus_rwlock_reader_trylock_nodebug(a); } else { janus_rwlock_reader_trylock_debug(a); } ret; })
/*! \brief Janus mutex unlock without debug */
#define janus_rwlock_reader_unlock_nodebug(a) g_rw_lock_reader_unlock(a)
/*! \brief Janus mutex unlock with debug (prints the line that unlocked a mutex) */
#define janus_rwlock_reader_unlock_debug(a) { JANUS_PRINT("[%s:%s:%d:reader_unlock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); janus_rwlock_reader_unlock_nodebug(a); }
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_reader_unlock(a) { if(!lock_debug) { janus_rwlock_reader_unlock_nodebug(a); } else { janus_rwlock_reader_unlock_debug(a); } }
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_writer_lock_debug(a) { JANUS_PRINT("[%s:%s:%d:writer_unlock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); janus_rwlock_writer_lock_nodebug(a); }
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_writer_lock_nodebug(a)  g_rw_lock_writer_lock(a)
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_writer_lock(a) { if(!lock_debug) { janus_rwlock_writer_lock_nodebug(a); } else { janus_rwlock_writer_lock_debug(a); } }
/*! \brief Janus mutex try lock without debug */
#define janus_rwlock_writer_trylock_nodebug(a) { ret = g_rw_lock_writer_trylock(a); }
/*! \brief Janus mutex try lock with debug (prints the line that tried to lock a mutex) */
#define janus_rwlock_writer_trylock_debug(a) { JANUS_PRINT("[%s:%s:%d:writer_trylock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); ret = janus_rwlock_writer_trylock(a); }
/*! \brief Janus mutex try lock wrapper (selective locking debug) */
#define janus_rwlock_writer_trylock(a) ({ gboolean ret; if(!lock_debug) { janus_rwlock_reader_trylock_nodebug(a); } else { janus_rwlock_writer_trylock_debug(a); } ret; })
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_writer_unlock_nodebug(a) g_rw_lock_writer_unlock(a)
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_writer_unlock_debug(a) { JANUS_PRINT("[%s:%s:%d:writer_unlock] %p\n", __FILE__, __FUNCTION__, __LINE__, a); janus_rwlock_writer_unlock_nodebug(a); }
/*! \brief Janus mutex unlock wrapper (selective locking debug) */
#define janus_rwlock_writer_unlock(a) { if(!lock_debug) { janus_rwlock_writer_unlock_nodebug(a); } else { janus_rwlock_writer_unlock_debug(a); } }

#endif

#endif
