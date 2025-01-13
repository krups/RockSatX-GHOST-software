# BucketManager
## Data Structures
### Buckets
buckets in the bucket manager objects are structs that store a queue a user id and an in use variable.
- the queue is the variable responsible for storing and passing data to tasks via passing the queue handle to the tasks
- the user id is obtained when the bucket is given to a task by calling pcGetTaskName(NULL) which will get the name of the task that has recieved the queue from this bucket
- when a task is given one of the buckets the buckets in use variable is set to true

notes:
- the user id variable is only there for debugging purposes and the actual functional bucketmanager wont include it
- tasks will need to "give" their bucket back by calling give on the object and passing back the queue that they were given

This is the c++ bucket struct

~~~
struct bucket{
    QueueHandle_t q;
    bool in_use = false
    string user_id = ""
}
~~~
### Groups
groups are how buckets are organized each group has
- a counting semaphore that is decremented each time a bucket is given to a task 
- a list of buckets that are part of that group

notes:
- the tasks are intended to be a way of selecting a specific bucket in the same way that topics in a pub/sub model works
- all groups in the object are stored in a map that maps the name of the group to the actual group struct
~~~
struct group{
    SemaphoreHandle_t count;// this is the counting semaphore for the bucket list
    vector<bucket> bucket_list = {};
}
~~~


## Member Functions
### QueueHandle_t getReadBucket(string group_id)
This is the function that the tasks will call when they want to get a bucket. They will pass a group id that will map to that groups struct. Then the BucketManager will access the list of buckets and iterate over that list. Since this is getting a read bucket it will look for the highest size(most full) queue that isnt being used by any other task. Then it will set in use to true set the user id to the name of the task that called get read bucket and return the handle for that queue.
### QueueHandle_t getWriteBucket(string group_id)
This function is almost completely the same as getReadBucket with the main difference being that it will get the lowest size(least full) queue and return it to the task that called it.
### void give_queue(QueueHandle_t q)
When a task receives a queue from the bucket manager that queues bucket is marked as being used this allows the task to give the usage of that bucket up by letting go of the QueueHandle. When this function is called the object will map the queue handle to the bucket that the queue is in and set the in use variable to false and the user_id to "".
### void declare_group(string group_id,vector<int> bucket_sizes, int size_of_type)
This will add a group with a list of buckets to the map of strings to groups. Each bucket is a queue with a size determined by the sizes in bucket sizes and each entry in the queue has a size determined by the size of type variable.  

## Purpose/Function
### Thread Safety
When two threads/tasks are allowed to access the same resource it puts both tasks at risk of difficult to debug issues. By getting the data through the bucket manager, the data given to each task is guarded from other tasks accessing it maintaining thread and memory safety.
### Easy Parallelization 
By allowing each group to have multiple buckets there could be two tasks running the same computations accessing different memory which allows those two tasks to run in parallel.
### Smooth Transition to Pub/Sub model
The end goal for this is to become a pub/sub model and by having buckets organized in groups the groups can be used similar to how topics are used in a pub/sub model. Then when a task subscribes to a topic the group can have a new bucket appended to the groups bucket list. When the task wants data on the topic it subscribed to it would take data from the queue that was passed to it when it subscribed to that topic.