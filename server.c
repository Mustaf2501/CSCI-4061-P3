#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include "util.h"
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MAX_THREADS 100
#define MAX_queue_len 100
#define MAX_CE 100
#define INVALID -1
#define BUFF_SIZE 1024

 // global key
//int mid = msgget(key,0666|IPC_CREAT); // global queue id

/*
  THE CODE STRUCTURE GIVEN BELOW IS JUST A SUGGESTION. FEEL FREE TO MODIFY AS NEEDED
*/

// structs:
typedef struct request_queue {
   int fd;
   char request[BUFF_SIZE]; // change from char *request, might be a problem
   //char* request
} request_t;

typedef struct cache_entry {
    int len;
    char *request;
    char *content;
} cache_entry_t;

/* ******************** Dynamic Pool Code  [Extra Credit A] **********************/
// Extra Credit: This function implements the policy to change the worker thread pool dynamically
// depending on the number of requests
void * dynamic_pool_size_update(void *arg) {
  while(1) {
    // Run at regular intervals
    // Increase / decrease dynamically based on your policy
  }
}
/**********************************************************************************/

/* ************************ Cache Code [Extra Credit B] **************************/

// Function to check whether the given request is present in cache
int getCacheIndex(char *request){
  /// return the index if the request is present in the cache
  return 0;
}

// Function to add the request and its file content into the cache
void addIntoCache(char *mybuf, char *memory , int memory_size){
  // It should add the request at an index according to the cache replacement policy
  // Make sure to allocate/free memory when adding or replacing cache entries
}

// clear the memory allocated to the cache
void deleteCache(){
  // De-allocate/free the cache memory
}

// Function to initialize the cache
void initCache(){
  // Allocating memory and initializing the cache array
}

/**********************************************************************************/

/* ************************************ Utilities ********************************/
// Function to get the content type from the request
char* getContentType(char * mybuf) {
  // Should return the content type based on the file type in the request
  // (See Section 5 in Project description for more details)
char * final = "";
  char *token = strchr(mybuf, '.');

  if(strcmp(token,".html")){
     final = "text/html";
  }
  else if(strcmp(token,".jpg")){
    final = "image/jpeg";
  }
  else if(strcmp(token,".gif")){
    final = "image/gif";
  } else {
    final = "text/plain";
  }
  return final;

}

// Function to open and read the file from the disk into the memory
// Add necessary arguments as needed
int readFromDisk(/*necessary arguments*/) {
    // Open and read the contents of file given the request
    return 0;
}

/**********************************************************************************/

// Function to receive the request from the client and add to the queue
void * dispatch(void *arg) {

  int fd;
  char buf[1024];


  while (1) {
    struct request_queue req;
    memset((void *)req.request, '\0',1024); // blank out chunk

    // Accept client connection
    fd = accept_connection();

    // Get request from the client
    if (get_request(fd, buf) == 0){
      /*returns 0 on success, nonzero on failure. You must account
     for failures because some connections might send faulty
     requests. This is a recoverable error - you must not exit
     inside the thread that called get_request. After an error, you
     must NOT use a return_request or return_error function for that
     specific 'connection'. */
    }

    // Add the request into the queue
    strcpy(req.request, buf);
    req.fd = fd;
   //requestQ->request = buf;
   // requestQ->fd = fd;
   key_t key = ftok("./ftok.txt", 4061);
   int mid = msgget(key,0666|IPC_CREAT);
   msgsnd(mid, (void *)&req,BUFF_SIZE,0);

   }
   return NULL;
}

/**********************************************************************************/

// Function to retrieve the request from the queue, process it and then return a result to the client
void * worker(void *arg) {

  //WDchar *currRequest;
  //char *contentType;
  //char buffer[1000];
   // return_error(accept_connection(),buffer);
   while (1) {

    char* contentType;
    struct request_queue req;
    memset((void *)req.request, '\0',1024); // blank out chunk

    // Get the request from the queue
    //currRequest = requestQ->request;
    key_t key = ftok("./ftok.txt", 4061);
    int mid = msgget(key,0666|IPC_CREAT);
    msgrcv(mid,(void *)&req, BUFF_SIZE, 0, 0);

    // Get the data from the disk or the cache (extra credit B)

    // Log the request into the file and terminal
    contentType = getContentType(req.request);
    return_result(req.fd,contentType,req.request, 1024);

    // return the result
  }
  return NULL;
}

/**********************************************************************************/

int main(int argc, char **argv) {

  // Error check on number of arguments
  if(argc != 8){
    printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
    return -1;
  }

  // Get the input args
  int port =  strtol(argv[1], NULL, 10);
  printf("%d\n",port);
  char *path = argv[2];
  int num_dispatcher = strtol(argv[3], NULL, 10);
  int num_workers =  strtol(argv[4], NULL, 10);

  // Perform error checks on the input arguments

  // Change SIGINT action for grace termination

  // initialize new sigset - sigset_t, sigemptyset
  sigset_t set;

  sigemptyset(&set);

	// add SIGINT to sigset - sigaddset
  sigaddset(&set, SIGINT);
	// block SIGINT - sigprocmask
  if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    return -1;

  // Open log file (skip for now)

  // Change the current working directory to server root directory
  chdir(path);

  // Initialize cache (extra credit B)

  // Start the server
  init(port);

  // Create dispatcher and worker threads (all threads should be detachable)

  //pthread_t disparr[num_dispatcher];
  //pthread_t worparr[num_workers];

  for(int i=0; i<num_dispatcher; i++){
    pthread_t t;

    pthread_attr_t attr; //added
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    //pthread_create (&t1,NULL,thread_fn, (void* )&x);
    //pthread_create(tid + i, NULL, processfd, (fd + i)))
    pthread_create(&t, &attr, dispatch, NULL);
  }

   for(int i=0; i<num_workers; i++){
    pthread_t t;
    //pthread_create (&t1,NULL,thread_fn, (void* )&x);
    //pthread_create(tid + i, NULL, processfd, (fd + i)))

    pthread_attr_t attr; //added
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&t, &attr, worker, NULL);
  }

  // Create dynamic pool manager thread (extra credit A)

  // Terminate server gracefully
    // Print the number of pending requests in the request queue
    // close log file
    // Remove cache (extra credit B)

  return 0;
}
