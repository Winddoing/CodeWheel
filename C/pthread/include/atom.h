#ifndef __ATOM_H__
#define __ATOM_H__

#if defined(__GNUC__)
// v += a ; return v;
#define ATOM_ADD(v, a)        __sync_add_and_fetch(&(v), (a))
// type tmp = v ; v = a; return tmp;
#define ATOM_SET(v, a)        __sync_lock_test_and_set(&(v), (a))
// v &= a; return v;
#define ATOM_AND(v, a)        __sync_and_and_fetch(&(v), (a))
// return ++v;
#define ATOM_INC(v)         __sync_add_and_fetch(&(v), 1)
// return --v;
#define ATOM_DEC(v)         __sync_sub_and_fetch(&(v), 1)
// bool b = v == c; b ? v=a : ; return b;
#define ATOM_CAS(v, c, a)    __sync_bool_compare_and_swap(&(v), (c), (a))

// memory barrier
#define ATOM_SYNC()         __sync_synchronize()

// unlock, v = 0;
#define ATOM_UNLOCK(v)        __sync_lock_release(&(v))

#endif // __GNUC__

/*
 * 试图加锁, 用法举例

 if(ATOM_TRYLOCK(v)) {
// 已经有人加锁了, 处理返回事件
...
}

// 得到锁资源, 开始处理
...

ATOM_UNLOCK(v);

 * 返回1表示已经有人加锁了, 竞争锁失败.
 * 返回0表示得到锁资源, 竞争锁成功
 */
#define ATOM_TRYLOCK(v)        ATOM_SET(v, 1)

//
// 使用方式:
//  int lock = 0;
//  ATOM_LOCK(lock);
//  ...
//  ATOM_UNLOCK(lock);
//
#define ATOM_LOCK(v)        while(ATOM_SET(v, 1))

#endif // !__ATOM_H__
