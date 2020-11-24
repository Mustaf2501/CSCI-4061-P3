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
#include <time.h>


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


static int number_of_request_in_queue = 0;
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
  //printf("token : %s | buf : %s \n", token, mybuf ); 

  if(strcmp(token,".html") == 0){
     final = "text/html";
  }
  else if(strcmp(token,".jpg") == 0){
    printf("jpg\n");
    final = "image/jpeg";
  }
  else if(strcmp(token,".gif") == 0){
    final = "image/gif";
  } else {
    final = "text/plain";
  }
  return final;

}

// Function to open and read the file from the disk into the memory
// Add necessary arguments as needed
long int readFromDisk(char * mybuf) {
  printf("read from disk entered\n");
  char path0[100] = "testing/testing";
    char *path1 = strcat(path0,mybuf);
    printf("Path : %s \n", path1); 
    FILE* fp = fopen(path1, "r"); 
    
    //char cwd[1024];
    //getcwd(cwd, sizeof(cwd));
    printf("After file open\n");
    //printf("mybuf: %s\n", mybuf);
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    fclose(fp);
    return res;
}
 
/**********************************************************************************/

// Function to receive the request from the client and add to the queue
void * dispatch(void *arg) {
  number_of_request_in_queue++;
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
  char *contentType;

   while (1) {
    struct request_queue req;
   // while(number_of_request_in_queue ==0){
        //pthread_cond_wait(cq->cond, cq->mutex);
  //  }
    number_of_request_in_queue--;
    memset((void *)req.request, '\0',1024); // blank out chunk
    //printf("1 \n");
    // Get the request from the queue
    //currRequest = requestQ->request;
    key_t key = ftok("./ftok.txt", 4061);
    //printf("2 \n");
    int mid = msgget(key,0666|IPC_CREAT);
    //printf("3 \n");
    msgrcv(mid,(void *)&req, BUFF_SIZE, 0, 0);
    //printf("4 \n");

    // Get the data from the disk or the cache (extra credit B)
    if (strcmp(req.request, "/") != 0){
      printf("Recieved Request\n");
      printf("Data : %s \n",req.request);

    // Log the request into the file and terminal
    contentType = getContentType(req.request);
    long int size = readFromDisk(req.request);
    printf("after readFromDisk %ld\n", size);
    printf("req.request: %s\n", req.request);
     char path0[100] = "testing/testing";
    char *path1 = strcat(path0,req.request);
    FILE* fp = fopen(path1, "r"); 
    void* memory = malloc(sizeof(long int)*size);
    //fread(memory, 1, size_t nmemb, fp);
    //read(); 
   fread(memory, 1,size, fp);
   fclose(fp);
   
    return_result(req.fd,contentType,memory, size);
    }

    // return the result
  }
  return NULL;
}

int logger(int id, int fd, char* request_string, int bytes){
    //FILE * f = fopen("web_server_log", "ab+"); 
    //char id[50];
    //char fd[50];
    //char bytes[50]; 
    return 0;
}

void handle(int signo) {
  exit(0);
}  
/**********************************************************************************/

int main(int argc, char **argv) {

  // Error check on number of arguments
  if(argc != 8){
    printf("usage: %s port path num_dispatcher num_workers dynamic_flag queue_length cache_size\n", argv[0]);
    return -1;
  }
  //printf("??????????????\n");
  // Get the input args
  int port =  strtol(argv[1], NULL, 10);
  printf("%d\n",port);
  char *path = argv[2];
  int num_dispatcher = strtol(argv[3], NULL, 10);
  int num_workers =  strtol(argv[4], NULL, 10);

  // Perform error checks on the input arguments

  // Change SIGINT action for grace termination
  struct sigaction act;
  act.sa_handler = handle;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);
  sigaction(SIGINT, &act, NULL);
  
  //initialize new sigset - sigset_t, sigemptyset
  //sigset_t set;
  //sigemptyset(&set);
	//add SIGINT to sigset - sigaddset
  //sigaddset(&set, SIGINT);
	//block SIGINT - sigprocmask
  //if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
    //return -1;

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
  //sleep(60); // wat
  // Create dynamic pool manager thread (extra credit A)

  // Terminate server gracefully
    // Print the number of pending requests in the request queue
    // close log file
    // Remove cache (extra credit B)
    //int a = 9;
    //int *a = malloc(sizeof(int));
    //pthread_exit((void*)&a);
    while(1){
      sleep(5);
    }
}
