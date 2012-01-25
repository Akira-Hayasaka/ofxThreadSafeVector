/**
 * @file
 * @brief Thread Safe Utility
 *
 * @author Taka Miyamura (99blues@gmail.com)
 *
 */
#ifndef SMS_THREAD_SAFE_H__
#define SMS_THREAD_SAFE_H__

namespace sms/* Stop Making Sense */{

#if !defined(WIN32)
#include <pthread.h>

/**
 * @brief Lock Object
 *
 * compatible to MFC lock class (CCriticalSection)
 *
 * @note use pthread (non Visual C++)
 *
 */
template <int MUTEX_TYPE>
class GuardTemplate
{
private:
	pthread_mutex_t	mutex_;
public:
	GuardTemplate(){
		//pthread_mutex_init(&mutex_, 0 );
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, MUTEX_TYPE);
		pthread_mutex_init(&mutex_, &attr);
		pthread_mutexattr_destroy(&attr);
	}
	~GuardTemplate(){pthread_mutex_destroy(&mutex_ );}

	int Lock(){return pthread_mutex_lock(&mutex_);}
	int Unlock(){return pthread_mutex_unlock(&mutex_);}

	bool try_lock(){return (pthread_mutex_trylock(&mutex_) == 0 );}
};

#ifdef TARGET_OSX    
typedef GuardTemplate<PTHREAD_MUTEX_NORMAL> 		Guard;
typedef GuardTemplate<PTHREAD_MUTEX_ERRORCHECK> 	ErrorCheckGuard;
typedef GuardTemplate<PTHREAD_MUTEX_RECURSIVE>      RecursiveGuard;
#else
typedef GuardTemplate<PTHREAD_MUTEX_FAST_NP> 		Guard;
typedef GuardTemplate<PTHREAD_MUTEX_ERRORCHECK_NP> 	ErrorCheckGuard;
typedef GuardTemplate<PTHREAD_MUTEX_RECURSIVE_NP> 	RecursiveGuard;    
#endif
#endif

/**
 * @brief auto lock/unlock
 *
 */
template<class GUARD>
class ScopeGuard
{
private:
	GUARD&	g_;
public:
	ScopeGuard(GUARD& guard):g_(guard){g_.Lock();}
	~ScopeGuard(){g_.Unlock();}
};


/**
 * @brief easy thread safe class
 *
 */
#if defined(WIN32)
#include "afxmt.h"
template<class VALUE, class GUARD=CCriticalSection>
#else
template<class VALUE, class GUARD=Guard>
#endif
class ThreadSafe
{
private:
	VALUE v_;
	mutable GUARD g_;

public:
	ThreadSafe(){}
	explicit ThreadSafe(const VALUE& c):v_(c){}
	virtual ~ThreadSafe(){}

	VALUE value()const{
		ScopeGuard<GUARD> s(g_);
		return v_;
	}
	VALUE operator=(const VALUE& c){
		ScopeGuard<GUARD> s(g_);
		return (v_ = c);
	}
	bool operator==(const VALUE& c)const{
		ScopeGuard<GUARD> s(g_);
		return (v_ == c);
	}
	bool operator!=(const VALUE& c)const{
		return not this == c;
	}

	int operator()(int (*fn)(VALUE& )){
		ScopeGuard<GUARD> s(g_);
		return fn(v_);
	}
	int operator()(int (*fn)(const VALUE& ))const{
		ScopeGuard<GUARD> s(g_);
		return fn(v_);
	}

	int Lock()const    {return g_.Lock(); }
	int Unlock()const  {return g_.Unlock(); }
#if !defined(WIN32)
	bool try_lock()const{return g_.try_lock(); }
#endif

	VALUE& ref(){return v_;}
	const VALUE& ref()const{return v_;}
};


} // end of namespace

#endif // SMS_THREAD_SAFE_H__
