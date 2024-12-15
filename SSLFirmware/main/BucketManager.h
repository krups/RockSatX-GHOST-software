#pragma once
#ifndef BUCKET_MANAGER_H
#define BUCKET_MANAGER_H
typedef struct bucket bucket;
typedef struct topic topic;

// returns an opaque pointer to a bucket by looking for the most full bucket
bucket* getReadBucket(topic* tpc);
// returns an opaque poiner to a bucket by looking for the most empty bucket
bucket* getWriteBucket(topic* tpc);
// declares a topic that can later be subscribed to
topic* declareTopic(int bucket_sizes, int num_buckets,int data_size);
// tells the bucket manager that this bucket is no longer in use
void giveBucket(bucket* b);

void bMessageWrite(void* s_ptr, bucket* bk_ptr, int num);

void bMessageRecieve(void* rPtr, bucket* bkPtr, int num);

void printBucket(bucket* bPtr);

#endif/* BUCKET_MANAGER_H */