#include<stdio.h>
#include "freertos/FreeRTOS.h" 
#include"freeRTOS/semphr.h"
#include "BucketManager.h"

// the user can recieve a pointer to a group now bc users of the header wont be able to access and change the details of a pointer to a group(opaque)
// one of the main issues is the the bucket itself is not allocated on the heap yet
struct bucket{
    topic* tpc;// pointer to the topic that this bucket is in
    bool inUse;// the bucket is not in use by default
    void* buf;
    int size;
    int capacity;
    int dataSize;
};
struct topic{
    int numBuckets;
    SemaphoreHandle_t count;// this is the counting semaphore that determines if there are any available buckets
    bucket* bkList;
};

// makes a group and fills it with the buckets of the sizes determined by bucket_sizes
// must be called like _declare_group<type>(args) where the type determines the type that the buckets are set up to store 

bucket* declare_topic( int bucket_sizes,int numBuckets,int dataSize){
    topic* tpcPtr =(topic *)malloc(sizeof(topic*)); 
    bucket lst[numBuckets];
    tpcPtr->bkList = &lst;
    // fill the topic with its buckets
    for(int i=0;i<numBuckets;i++){
        //stack allocated bucket struct that contains heap allocated buffer
        bucket b;
        void *buf_ptr = (void *)malloc(bucket_sizes*dataSize);
        b.buf = buf_ptr;
        b.tpc = tpcPtr;
        b.dataSize = dataSize;
        //puts the queue handle into a map that will map it to a pointer to the bucket that queue handle belongs to
        
        tpcPtr->bkList[i]=b;
    } 
    
    tpcPtr->count = xSemaphoreCreateCounting(bucket_sizes,bucket_sizes);
    tpcPtr->numBuckets = numBuckets;
    return tpcPtr;
}
// finds the first available bucket to read from and returns a pointer to it to the calling task
// this would also give a very similar effect to subscribing to a topic in a pub sub model
bucket* get_read_bucket(topic *tpcPtr){
    if(xSemaphoreTake(tpcPtr->count, (TickType_t) 10)==pdTRUE){    
        for(int i=0; i<tpcPtr->numBuckets;i++){
            if(!(tpcPtr->bkList[i].inUse)){
                
                tpcPtr->bkList[i].inUse = true;
                return &(tpcPtr->bkList[i]);
            }
        }
    }
    //return null pointer if it fails to be blocked and cannot find an open bucket
    return NULL;
}

bucket* getWriteBucket(topic* tpcPtr){
    if(xSemaphoreTake(tpcPtr->count,(TickType_t) 10)==pdTRUE){    
        for(int i=0; i<tpcPtr->numBuckets;i++){
            if(!tpcPtr->bkList[i].inUse){
                tpcPtr->bkList[i].inUse = true;
                return &(tpcPtr->bkList[i]);
            }
        }
    }
    return NULL;
}

void giveBucket(bucket* bkPtr){
    xSemaphoreGive(bkPtr->tpc->count);
    bkPtr->inUse=false;
}
// reads num elements from the bucket pointed to by b_ptr
// ex: bMessageRecieve(receiver,bucket,num=4) on a bucket storing ints would read 4 ints from the bucket into the reciever pointer
// note the reciever pointer must be a void pointer that is then cast to the intended type and dereferenced 
void bMessageRecieve(void* rPtr,bucket* bPtr,int num){
    //checks if the number of bytes in the bucket is enough for what is being asked for
    // then copies into the reciever pointer starting at the end of the bucket array - bytes
    if(bPtr->size >= num){
        memcpy(rPtr,bPtr+bPtr->size*bPtr->dataSize-num*bPtr->dataSize,num*bPtr->dataSize);
    }else{
        // generic debug message such as bucket does not have enough data to read num bytes
    }

}
// writes num typed data points to the bucket pointed to by b_ptr
// ex: bMessageWrite(sender,bucket,num=4) on a bucket storing ints would copy 4 ints from the sender pointer into the bucket
void bMessageWrite(void* sPtr,bucket* bPtr,int num){
    if(bPtr->capacity-bPtr->size>=num){
        memcpy(bPtr+bPtr->size*bPtr->dataSize,sPtr,num*bPtr->dataSize);
    }else{
        //generic error message warning about a full bucket
    }
}