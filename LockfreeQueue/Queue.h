// Queue.h: interface for the Queue class.
//
//  lock free����
//  ��д����ģʽ��֧��1��n����֧��n��n���ȿ���1�̶߳�n���߳�д����������n���̶߳�1���߳�д����
//  ������n���̶߳�n���߳�д����
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
 
//ԭ�Ӽ�
inline uint32 AtomAdd(void * var, const uint32 value)
{
#ifdef WIN32
    return InterlockedExchangeAdd((long *)(var), value); // NOLINT
#else
    return __sync_fetch_and_add((uint32 *)(var), value);  // NOLINT
#endif
}
 
//ԭ�Ӽ�
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
    void Clear();//�������
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