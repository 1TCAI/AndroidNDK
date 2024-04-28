//
// Created by 96302 on 2024-04-07.
//

#ifndef PLAYER_SAFE_QUEUE_H
#define PLAYER_SAFE_QUEUE_H

#include <queue>
#include <pthread.h>

using namespace std;
template <typename T>
class SafeQueue{

private:
    queue<T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int work;
    typedef void (*ReleaseCallback)(T *);   //函数指针名是ReleaseCallback 参数是 T* 返回 void
    ReleaseCallback releaseCallback;        //函数指针是函数指针类型，再定义一个 releaseCallback 函数指针变量
public:
    void setReleaseCallback(ReleaseCallback releaseCallback){
        this->releaseCallback = releaseCallback;
    }

    SafeQueue(){
        pthread_mutex_init(&mutex,0);
        pthread_cond_init(&cond,0);
    }
    ~SafeQueue(){
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void insertToQueue(T value){
        pthread_mutex_lock(&mutex);
        if(work){
            queue.push(value);
            pthread_cond_signal(&cond);
        } else{     //不在工作状态释放，但是T类型不知道怎么释放，函数指针传给外面去释放。
            if(releaseCallback){
                releaseCallback(&value);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    int getQueueAndDel(T &value){   //给引用传入赋值再传出。
        int ret = 0;

        pthread_mutex_lock(&mutex);
        while (work && queue.empty()){
            pthread_cond_wait(&cond,&mutex);
        }
        if(!queue.empty()){
            value = queue.front();
            queue.pop();
            ret = 1;
        }
        pthread_mutex_unlock(&mutex);
        return ret;
    }

    void setWork(int work){
        pthread_mutex_lock(&mutex);
        this->work = work;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    int empty(){
        return queue.empty();
    }
    int size(){
        return queue.size();
    }

    void clear(){
        pthread_mutex_lock(&mutex);
        int size = queue.size();
        for (int i = 0; i < size; ++i) {
            T value = queue.front();
            if(releaseCallback){
                releaseCallback(&value);
            }
            queue.pop();
        }
        pthread_mutex_unlock(&mutex);
    }
};





#endif //PLAYER_SAFE_QUEUE_H
