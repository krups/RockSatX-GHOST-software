#include"BucketManager.h"
#include<iomanip>
#include<iostream>

BucketManager::BucketManager(){}
// makes a group and fills it with the buckets of the sizes determined by bucket_sizes
// must be called like _declare_group<type>(args) where the type determines the type that the buckets are set up to store 

void BucketManager::_declare_group(std::string group_id, std::vector<int> bucket_sizes,int var_size){
    struct group grp;
    for(int i=0;i<bucket_sizes.size();i++){
        
        struct bucket b;
        b.bucket_handle = xQueueCreate(bucket_sizes[i],var_size);
        //puts the queue handle into a map that will map it to a pointer to the bucket that queue handle belongs to
        _bucket_map.insert(std::make_pair(&b.bucket_handle,&b));

        grp.bk_list.push_back(b);

    } 
    grp.count = xSemaphoreCreateCounting(bucket_sizes.size(),bucket_sizes.size());
    _group_map.insert(std::pair <std::string,group> (group_id,grp));
}

QueueHandle_t BucketManager::_get_read_bucket(std::string group_id){
    group grp = _group_map[group_id];
    if(xSemaphoreTake(grp.count, (TickType_t) 10)==pdTRUE){    
        for(int i=0; i<grp.bk_list.size();i++){
            if(!grp.bk_list[i].in_use){
                std::string user_task = pcTaskGetName(NULL);
                grp.bk_list[i].user = user_task;
                grp.bk_list[i].in_use = true;
                return grp.bk_list[i].bucket_handle;
            }
        }
    }
    //return null pointer if it fails to be blocked and cannot find an open bucket
    return NULL;
}

QueueHandle_t* BucketManager::_get_write_bucket(std::string group_id){
    group grp = _group_map[group_id];
    if(xSemaphoreTake(grp.count,(TickType_t) 10)==pdTRUE){    
        for(int i=0; i<grp.bk_list.size();i++){
            if(!grp.bk_list[i].in_use){
                std::string user_task = pcTaskGetName(NULL);
                grp.bk_list[i].user = user_task;
                return &grp.bk_list[i].bucket_handle;
            }
        }
    }
    return NULL;
}

void BucketManager::_give_bucket(std::string grp_id,QueueHandle_t q){
    group grp = _group_map[grp_id];
    xSemaphoreGive(grp.count);
    _bucket_map[&q]->user = "";
    _bucket_map[&q]->in_use = false;
}

void BucketManager::_debug_print(){
    
    for (auto it=_group_map.begin();!(it==_group_map.end());++it){
        group grp = it->second;
        std::cout << "group" << std::endl;
        for(int i=0;i<grp.bk_list.size();i++){
            std::cout << std::left << std::setw(20) << i << std::endl; 
        }
        for(int i=0;i<grp.bk_list.size();i++){
            std::cout << std::left << std::setw(20) << grp.bk_list[i].user << std::endl; 
        }
    }
}