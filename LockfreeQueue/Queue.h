// Queue.h: interface for the Queue class.
//
//  lock free队列
//  读写并发模式：支持1对n，不支持n对n，既可以1线程读n个线程写并发，或者n个线程读1个线程写并发
//  不可以n个线程读n个线程写并发
//////////////////////////////////////////////////////////////////////
 
#ifndef QUEUE_H
#define QUEUE_H
 
#ifdef WIN32
#include <windows.h>
#else
#include <sys/types.h>
#endif
 
 
namespace lock_free
{
 
typedef int                 int32;  
typedef unsigned int        uint32;  
 
//原子加
inline uint32 AtomAdd(void * var, const uint32 value)
{
#ifdef WIN32
    return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
    return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}
 
//原子减
inline uint32 AtomDec(void * var, int32 value)
{
    value = value * -1;
#ifdef WIN32
    return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
    return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}  
 
typedef struct QUEUE_NODE
{
    bool IsEmpty;
    void *pObject;
}QUEUE_NODE;
 
class Queue  
{
public:
    Queue( int32 nSize );
    virtual ~Queue();
 
public:
    bool Push( void *pObject );
    void* Pop();
    void Clear();//清除数据
protected:
     
private:
    QUEUE_NODE *m_queue;
    uint32 m_push;
    uint32 m_pop;
    uint32 m_nSize;
    int32 m_nWriteAbleCount;
    int32 m_nReadAbleCount;
};
 
}
 
#endif // QUEUE_H