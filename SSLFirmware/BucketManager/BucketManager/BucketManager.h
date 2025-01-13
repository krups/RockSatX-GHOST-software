#pragma once
#ifndef BUCKET_MANAGER_H
#define BUCKET_MANAGER_H

#include <string.h>

typedef struct bucket bucket;
typedef struct topic topic;
typedef bucket** bkHandle;

// returns an opaque pointer to a bucket by looking for the most full bucket
bkHandle getReadBucket(topic* tpc);
// returns an opaque poiner to a bucket by looking for the most empty bucket
bkHandle getWriteBucket(topic* tpc);
// declares a topic that can later be subscribed to
topic* declareTopic(int bucket_sizes, int num_buckets,int data_size);
// tells the bucket manager that this bucket is no longer in use
void giveBucket(bkHandle b);
// writes data from sPtr into the bucket
void bMessageWrite(void* s_ptr, bkHandle bkHandle, int num);
// reads data from the bucket into rPtr
void bMessageRecieve(void* rPtr, bkHandle bkHandle, int num);
// sends data bout the bucket pointed to by bHandle to the serial port
void printBucket(bucket* bPtr);
// returns the number of items of size DataSize in the bucket 
int itemsIn(bkHandle bHandle);

int dataSize(bkHandle bHandle);

int capacityOf(bkHandle bHandle);

#endif/* BUCKET_MANAGER_H */