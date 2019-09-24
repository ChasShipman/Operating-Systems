/* buffer.h */
typedef int buffer_item;
#define BUFFER_SIZE 5

//extern int counter;
//extern pthread_mutex_t mutex;
//extern pthread_attr_t attr;
//extern sem_t full;
//extern sem_t empty;

void initialize();
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
