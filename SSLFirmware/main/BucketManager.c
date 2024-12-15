#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include "freertos/FreeRTOS.h" 
#include"freeRTOS/semphr.h"
#include "BucketManager.h"

struct bucket {
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
    bucket** bkList;
};

topic* declareTopic(int bucket_sizes, int numBuckets, int dataSize) {
    topic* tpcPtr = (topic*)malloc(sizeof(topic*));
    bucket *lst= (bucket*)malloc(sizeof(bucket)*numBuckets);
    // remember that you havent handled the case that malloc fails and returns a null pointer
    tpcPtr->bkList = lst;
    // fill the topic with its buckets
    for (int i = 0;i < numBuckets;i++) {
        //stack allocated bucket struct that contains heap allocated buffer
        bucket* b_ptr = (bucket*)malloc(sizeof(bucket));
        void* buf_ptr = (void*)malloc(bucket_sizes * dataSize);
        b_ptr->buf = buf_ptr;
        b_ptr->tpc = tpcPtr;
        b_ptr->dataSize = dataSize;
        b_ptr->inUse = false;
        b_ptr->size = 0;
        b_ptr->capacity = bucket_sizes;
        tpcPtr->bkList[i] = b_ptr;
    }
    tpcPtr->count = xSemaphoreCreateCounting(numBuckets,numBuckets);
    tpcPtr->numBuckets = numBuckets;
    return tpcPtr;
}
// finds the first available bucket to read from and returns a pointer to it to the calling task
bucket* getReadBucket(topic* tpcPtr) {
    if(xSemaphoreTake(tpcPtr->count,(TickType_t)10)){   
        for (int i = 0; i < tpcPtr->numBuckets;i++) {
            if (!(tpcPtr->bkList[i]->inUse)) {

                tpcPtr->bkList[i]->inUse = true;
                return (tpcPtr->bkList[i]);
            }
        }
    }
    printf("getReadBucket failed");
    return NULL;
}

bucket* getWriteBucket(topic* tpcPtr) {
    if(xSemaphoreTake(tpcPtr->count,(TickType_t)10)){ 
        for (int i = 0; i < tpcPtr->numBuckets;i++) {
            if (!tpcPtr->bkList[i]->inUse) {
                tpcPtr->bkList[i]->inUse = true;
                return (tpcPtr->bkList[i]);
            }
        }
    }
    printf("getWriteBucket failed");
    return NULL;
}
// this is to give up control over a bucket
void giveBucket(bucket* bkPtr){
    xSemaphoreGive(bkPtr->tpc->count);
    bkPtr->inUse=false;
}
// reads num elements from the bucket pointed to by b_ptr
// ex: bMessageRecieve(receiver,bucket,num=4) on a bucket storing ints would read 4 ints from the bucket into the reciever pointer
// note the reciever pointer must be a void pointer that is then cast to the intended type and dereferenced 
void bMessageRecieve(void* rPtr, bucket* bPtr, int num) {
    //checks if the number of bytes in the bucket is enough for what is being asked for
    // then copies into the reciever pointer starting at the end of the bucket array - bytes
    if (bPtr->size >= num) {
        bPtr->buf -= num * bPtr->dataSize;
        memcpy(rPtr, (const*)(bPtr->buf), num * bPtr->dataSize);
        bPtr->size -= num;
    }
    else {
        // generic debug message such as bucket does not have enough data to read num bytes
        printf("message recieve failed \n");
    }

}
// writes num typed data points to the bucket pointed to by b_ptr
// ex: bMessageWrite(sender,bucket,num=4) on a bucket storing ints would copy 4 ints from the sender pointer into the bucket
void bMessageWrite(void* sPtr, bucket* bPtr, int num) {
    if (bPtr->capacity - bPtr->size >= num) {
        memcpy((const*)(bPtr->buf), sPtr, num * bPtr->dataSize);
        bPtr->size += num;
        //converted to char because the size of char is one byto so pointer arithmetic converts one to one byte
        // note that this will move the bPtr around instead of keeping its address the same and referencing that
        bPtr->buf += num * bPtr->dataSize;
    }
    else {
        //generic error message warning about a full bucket
        printf("message send failed \n");
    }
}
// this needs to be replaced with the serial.println stuff
void printBucket(bucket* bPtr) {
    printf("size: %d\n", bPtr->size);
    printf("capacity: %d\n", bPtr->capacity);
    printf("data size: %d\n", bPtr->dataSize);
    printf("in use: %d\n", bPtr->inUse);
    printf("buffer address %p\n", bPtr->buf);
}