#include<stdio.h>
#include "freertos/FreeRTOS.h" 
#include"freeRTOS/semphr.h"
#include "BucketManager.h"

// the user can recieve a pointer to a group now bc users of the header wont be able to access and change the details of a pointer to a group(opaque)
// one of the main issues is the the bucket itself is not allocated on the heap yet
struct bucket{
    topic* tpc;// pointer to the topic that this bucket is in
    bool in_use;// the bucket is not in use by default
    void* buf;
    int size;
    int capacity;
    int data_size;
};
struct topic{
    int num_buckets;
    SemaphoreHandle_t count;// this is the counting semaphore that determines if there are any available buckets
    bucket* bk_list;
};

// makes a group and fills it with the buckets of the sizes determined by bucket_sizes
// must be called like _declare_group<type>(args) where the type determines the type that the buckets are set up to store 

bucket* declare_topic( int bucket_sizes,int num_buckets,int data_size){
    topic* tpc_ptr =(topic *)malloc(sizeof(topic*)); 
    bucket lst[num_buckets];
    tpc_ptr->bk_list = &lst;
    // fill the topic with its buckets
    for(int i=0;i<num_buckets;i++){
        //stack allocated bucket struct that contains heap allocated buffer
        bucket b;
        void *buf_ptr = (void *)malloc(bucket_sizes*data_size);
        b.buf = buf_ptr;
        b.tpc = tpc_ptr;
        b.data_size = data_size;
        //puts the queue handle into a map that will map it to a pointer to the bucket that queue handle belongs to
        
        tpc_ptr->bk_list[i]=b;
    } 
    
    tpc_ptr->count = xSemaphoreCreateCounting(bucket_sizes,bucket_sizes);
    tpc_ptr->num_buckets = num_buckets;
    return tpc_ptr;
}
// finds the first available bucket to read from and returns a pointer to it to the calling task
// this would also give a very similar effect to subscribing to a topic in a pub sub model
bucket* get_read_bucket(topic *tpc_ptr){
    if(xSemaphoreTake(tpc_ptr->count, (TickType_t) 10)==pdTRUE){    
        for(int i=0; i<tpc_ptr->num_buckets;i++){
            if(!(tpc_ptr->bk_list[i].in_use)){
                
                tpc_ptr->bk_list[i].in_use = true;
                return &(tpc_ptr->bk_list[i]);
            }
        }
    }
    //return null pointer if it fails to be blocked and cannot find an open bucket
    return NULL;
}

bucket* get_write_bucket(topic* tpc_ptr){
    if(xSemaphoreTake(tpc_ptr->count,(TickType_t) 10)==pdTRUE){    
        for(int i=0; i<tpc_ptr->num_buckets;i++){
            if(!tpc_ptr->bk_list[i].in_use){
                tpc_ptr->bk_list[i].in_use = true;
                return &(tpc_ptr->bk_list[i]);
            }
        }
    }
    return NULL;
}

void give_bucket(bucket* bk_ptr){
    xSemaphoreGive(bk_ptr->tpc->count);
    bk_ptr->in_use=false;
}
// reads num elements from the bucket pointed to by b_ptr
// ex: bMessageRecieve(receiver,bucket,num=4) on a bucket storing ints would read 4 ints from the bucket into the reciever pointer
// note the reciever pointer must be a void pointer that is then cast to the intended type and dereferenced 
void b_message_recieve(void* r_ptr,bucket* b_ptr,int num){
    //checks if the number of bytes in the bucket is enough for what is being asked for
    // then copies into the reciever pointer starting at the end of the bucket array - bytes
    if(b_ptr->size >= num){
        memcpy(r_ptr,b_ptr+b_ptr->size*b_ptr->data_size-num*b_ptr->data_size,num*b_ptr->data_size);
    }else{
        // generic debug message such as bucket does not have enough data to read num bytes
    }

}
// writes num typed data points to the bucket pointed to by b_ptr
// ex: bMessageWrite(sender,bucket,num=4) on a bucket storing ints would copy 4 ints from the sender pointer into the bucket
void bMessageWrite(void* s_ptr,bucket* b_ptr,int num){
    if(b_ptr->capacity-b_ptr->size>=num){
        memcpy(b_ptr+b_ptr->size*b_ptr->data_size,s_ptr,num*b_ptr->data_size);
    }else{
        //generic error message warning about a full bucket
    }
}