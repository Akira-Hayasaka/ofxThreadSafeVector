//
//  ofxThreadSafeVector.h
//  ofxThreadSafeVector
//
//  Created by Akira Hayasaka on 1/24/12.
//  Copyright (c) 2012 ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆ﾟ･:,｡ﾟ･:,｡★ﾟ･:,｡ﾟ･:,｡☆. All rights reserved.
//

#ifndef ofxThreadSafeVector_ofxThreadSafeVector_h
#define ofxThreadSafeVector_ofxThreadSafeVector_h

#include "ofMain.h"
#include "sms_thread_safe.h"

using namespace sms;

template<class T>
class ofxThreadSafeVector: public ThreadSafe<vector<T> >
{
    typedef size_t          size_type;
    
public:
    ofxThreadSafeVector(){}
    virtual ~ofxThreadSafeVector(){}
    
    T at(const int& i) 
    {
        ScopeGuard<ofxThreadSafeVector> s(*this);
        return this->ref().at(i);        
    }
    
    void push_back(const T& t)
    {
        ScopeGuard<ofxThreadSafeVector> s(*this);
        this->ref().push_back(t);
    }
    
    size_type size() 
    {
        ScopeGuard<ofxThreadSafeVector<T> > s(*this);
        return this->ref().size();
    }    
    
    bool empty() {
        ScopeGuard<ofxThreadSafeVector> s(*this);
        return this->ref().empty();
    }

    typename vector<T>::iterator begin()
    {
        ScopeGuard<ofxThreadSafeVector> s(*this);
        return this->ref().begin();
    }

    typename vector<T>::iterator end()
    {
        ScopeGuard<ofxThreadSafeVector> s(*this);
        return this->ref().end();
    }    
    
    void erase(const typename vector<T>::iterator& first, 
               const typename vector<T>::iterator& last)
    {
        ScopeGuard<ofxThreadSafeVector<T> > s(*this);
        this->ref().erase(first, last);
    }

    void clear()
    {
        ScopeGuard<ofxThreadSafeVector> s(*this);
        this->ref().clear();
    }
    
};

template<class T, class BoolFunction>
void threadSafeRemove(ofxThreadSafeVector<T>& values, BoolFunction shouldErase) {
    ScopeGuard<ofxThreadSafeVector<T> > g(values);
	values.erase(remove_if(values.begin(), values.end(), shouldErase), values.end());
}

#endif
