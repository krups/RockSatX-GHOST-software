#include"BucketManager.h"


BucketManager::BucketManager(){

}
// makes a group and fills it with the buckets of the sizes determined by bucket_sizes
template <typename type>
void BucketManager::_declare_group(std::string group_id, std::vector<int> bucket_sizes){
    struct group grp;
    for(int i=0;i<bucket_sizes.size();i++){
        
        struct bucket b;
        b.bucket_handle = (bucket_sizes[i],sizeof(type));
        //puts the queue handle into a map that will map it to a pointer to the bucket that queue handle belongs to
        bucket* ptr_b = &b;
        _bucket_map.insert(b.bucket_handle,ptr_b);

        grp.group_bucket_list.push_back(b)

    } 
    g.count = xSemaphoreCreateCounting(bucket_sizes.size(),bucket_sizes.size());
    _group_map.insert(std::pair <std::string,group> (group_id,g));
}

QueueHandle_t BucketManager::_get_read_bucket(std::string group_id){
    group grp = _group_map[group_id];
    if(xSemaphoreTake(grp.count, (TickType_t) 10)==pdTRUE){    
        for(int i=0; i<grp.group_bucket_list.size();i++){
            if(grp.group_bucket_list[i].user == ""){
                std::string user_task = pcTaskGetName(NULL);
                grp.group_bucket_list[i].user = user_task;
                return grp.group_bucket_list[i].bucket_handle;
            }
        }
    }
}

QueueHandle_t BucketManager::_get_write_bucket(std::string group_id){
    group grp = _group_map[group_id];
    if(xSemaphoreTake(grp.count,(TickType_t) 10)==pdTRUE){    
        for(int i=0; i<grp.group_bucket_list.size();i++){
            if(grp.group_bucket_list[i].user == ""){
                std::string user_task = pcTaskGetName(NULL);
                grp.group_bucket_list[i].user = user_task;
                return grp.group_bucket_list[i].bucket_handle;
            }
        }
    }
}

void BucketManager::give_bucket(std::string grp_id,QueueHandle_t q){
    group grp = _group_map[grp_id];
    for (int i = 0;i<grp.group_bucket_list.size();i++){
        if(grp.group_bucket_list[i].bucket_handle==q){
            grp.group_bucket_list[i].user = "";
            xSemaphoreGive(grp.count);
            return;
        }
    }
}
