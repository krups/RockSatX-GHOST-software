#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
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
    SemaphoreHandle_t write;// number of buckets that you could write to 
    SemaphoreHandle_t read;// number of buckets that you could read from
    bucket** bkList;
};

// creates a topic with a given number of buckets to read from and write to 
topic* declareTopic(int bucket_sizes, int numBuckets, int dataSize) {
    topic* tpcPtr = (topic*)pvPortMalloc(sizeof(topic*));
    bucket** lst= (bucket**)pvPortMalloc(sizeof(bucket)*numBuckets);
    // remember that you havent handled the case that malloc fails and returns a null pointer
    tpcPtr->bkList = lst;
    // fill the topic with its buckets
    for (int i = 0;i < numBuckets;i++) {
        
        //stack allocated bucket struct that contains heap allocated buffer
        bucket* b_ptr = (bucket*)pvPortMalloc(sizeof(bucket));
        void* buf_ptr = (void*)pvPortMalloc(bucket_sizes * dataSize);
        b_ptr->buf = buf_ptr;
        b_ptr->tpc = tpcPtr;
        b_ptr->dataSize = dataSize;
        b_ptr->inUse = false;
        b_ptr->size = 0;
        b_ptr->capacity = bucket_sizes;
        tpcPtr->bkList[i] = b_ptr;
    }

    // start with all buckets available to be written to
    tpcPtr->write = xSemaphoreCreateCounting(numBuckets,numBuckets);
    // and no buckets available to be read from
    tpcPtr->read = xSemaphoreCreateCounting(numBuckets,0);
    tpcPtr->numBuckets = numBuckets;
    return tpcPtr;
}
// finds the first available bucket to read from and returns a pointer to it to the calling task
bkHandle getReadBucket(topic* tpcPtr) {
    if(xSemaphoreTake(tpcPtr->read,(TickType_t)10)){

        for (int i = 0; i < tpcPtr->numBuckets;i++){

            if (!(tpcPtr->bkList[i]->inUse)){

                // if its size is less than max also take from the possible write buckets
                if(tpcPtr->bkList[i]->size < tpcPtr->bkList[i]->capacity){
                    xSemaphoreTake(tpcPtr->write,(TickType_t)10);
                }

                tpcPtr->bkList[i]->inUse = true;
                return &(tpcPtr->bkList[i]);
            }
        }
    }
    //printf("getReadBucket failed");
    return NULL;
}
// gets a bucket specifically for writing to will later be changed to check for the fullness of the bucket
bkHandle getWriteBucket(topic* tpcPtr) {
    if(xSemaphoreTake(tpcPtr->write,(TickType_t)10)){ 

        for (int i = 0; i < tpcPtr->numBuckets;i++) {

            if (!tpcPtr->bkList[i]->inUse) {

                if(tpcPtr->bkList[i]->size>0){
                    xSemaphoreTake(tpcPtr->read,(TickType_t)10);
                }

                tpcPtr->bkList[i]->inUse = true;

                return &(tpcPtr->bkList[i]);
            }
        }
    }
    //printf("getWriteBucket failed");
    return NULL;
}
// this is to give up control over a bucket
void giveBucket(bkHandle bHandle){
    bucket* bkPtr = *bHandle;
    // if there is still data in the bucket list it as being able to be read
    if(bkPtr->size>0){
        xSemaphoreGive(bkPtr->tpc->read);
    }
    
    // if there is still room in the bucket label it as available to write to 
    if(bkPtr->size<bkPtr->capacity){
        xSemaphoreGive(bkPtr->tpc->write);
    }

    bkPtr->inUse=false;
    *bHandle = NULL;
}
// reads numItems elements from the bucket pointed to by b_ptr
// ex: bMessageRecieve(receiver,bucket,numItems=4) on a bucket storing ints would read 4 ints from the bucket into the reciever pointer
// note the reciever pointer must be a void pointer that is then cast to the intended type and dereferenced 
void bMessageRecieve(void* rPtr, const bkHandle bHandle, int numItems) {
    //checks if the number of bytes in the bucket is enough for what is being asked for
    // then copies into the reciever pointer starting at the end of the bucket array - bytes
    bucket* bPtr = *bHandle;
    
    if (bPtr->size >= numItems) {

        bPtr->buf -= numItems * bPtr->dataSize;
        memcpy(rPtr, bPtr->buf, (size_t)(numItems * bPtr->dataSize));
        bPtr->size -= numItems;

    }else {
        //printf("attempting to read more data than the bucket contains error")
    }

}
// writes numItems typed data points to the bucket pointed to by b_ptr
// ex: bMessageWrite(sender,bucket,numItems=4) on a bucket storing ints would copy 4 ints from the sender pointer into the bucket
void bMessageWrite(void* sPtr, const bkHandle bkHandle, int numItems) {
    bucket* bPtr = *bkHandle;
    if (bPtr->capacity - bPtr->size >= numItems) {
        memcpy(bPtr->buf, sPtr, (size_t)(numItems * bPtr->dataSize));
        bPtr->size += numItems;
        //converted to char because the size of char is one byto so pointer arithmetic converts one to one byte
        // note that this will move the bPtr around instead of keeping its address the same and referencing that
        bPtr->buf += numItems * bPtr->dataSize;
    }
    else {
        //generic error message warning about a full bucket
        //printf("message send failed \n");
    }
}
// this needs to be replaced with the serial.println stuff
void printBucket(bucket* bPtr) {
    //printf("size: %d\n", bPtr->size);
    //printf("capacity: %d\n", bPtr->capacity);
    //printf("data size: %d\n", bPtr->dataSize);
    //printf("in use: %d\n", bPtr->inUse);
    //printf("buffer address %p\n", bPtr->buf);
}

int itemsIn(bkHandle bHandle){
    return bHandle[0]->size;
}

int capacityOf(bkHandle bHandle){
    return bHandle[0]->capacity;
}

int dataSize(bkHandle bHandle){
    return bHandle[0]->dataSize;
}

