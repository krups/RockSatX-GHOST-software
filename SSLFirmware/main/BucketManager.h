#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include "freertos/FreeRTOS.h" 
#include"freeRTOS/semphr.h"
#ifndef BUCKET_MANAGER_H
#define BUCKET_MANAGER_H
struct bucket{ 
    std::string user = "";// the name of the task that is accessing the partition "" by default
    bool in_use = false;// the bucket is not in use by default
    QueueHandle_t bucket_handle;
};

struct group{
    SemaphoreHandle_t count;// this is the counting semaphore that determines if there are any available buckets
    std::vector<bucket> bk_list = {};
};
class BucketManager{
    public:
    //This is basically just a forward definition of the manager object and later methods will actually define the object.
    BucketManager();
    // will reserve a bucket to read from in the group passed to it as an argument
    QueueHandle_t _get_read_bucket(std::string grp_id);
    //preventing two tasks from trying to write to memory due to hardware limitations is the responsibility of the tasks 
    QueueHandle_t* _get_write_bucket(std::string grp_id);
    // puts the assigns the vector of buckets to the id in map
    
    void _declare_group(std::string grp_id,std::vector<int> bucket_sizes,int var_size);
    //Gives the control of the bucket specified with the handle back to the object similar to the way you give a semaphore
    void _give_bucket(std::string grp_id , QueueHandle_t q);
    //prints out a description of each groups buckets and which task is using them
    void _debug_print();
    private:
    std::unordered_map<std::string,group> _group_map;//map a string to a group using that groups id string
    std::unordered_map<QueueHandle_t*,bucket*> _bucket_map;// this is what is used to set the id of a bucket back to normal when a task is done using a bucket
};
#endif/* BUCKET_MANAGER_H */