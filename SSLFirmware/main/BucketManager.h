#pragma once
#ifndef BUCKET_MANAGER_H
#define BUCKET_MANAGER_H
typedef struct bucket bucket;
typedef struct topic topic;

// returns an opaque pointer to a bucket by looking for the most full bucket
bucket* get_read_bucket(topic* tpc);
// returns an opaque poiner to a bucket by looking for the most empty bucket
bucket* get_write_bucket(topic* tpc);
// declares a topic that can later be subscribed to
topic* declare_topic(int bucket_sizes,int num_buckets);
// tells the bucket manager that this bucket is no longer in use
void give_bucket(bucket* b);

#endif/* BUCKET_MANAGER_H */