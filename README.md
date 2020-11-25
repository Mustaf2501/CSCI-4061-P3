test machine : CSELAB_machine_name date : 11 / 02 / 20 name : Mustaf Ahmed , Ellie Hollenberger, John Schmitz x500 : ahmed719 , holle299, schm4722
.
How to Compile: 

Navigate to the directory of the project and run make clean, make and then the following:

./web_server 9000 /testing/testing 1 1 0 0 0 

It is important to note our specific path "/testing/testing"



What the program does: Our program is a multithreaded webserver. Upon running, the server waits for requests. When one is recieved, a dispatcher will put the request information into our queue, implemented using message queues that we learned in P2. From there, requests will be handled by the created worker threads, which will return information for viewing our return and error. As well, the worker thread is updating our log file "web_server_log.txt". 

Assumptions: 
Path variable is :  "/testing/testing"


Team:17 John Schmitz schm4722@umn.edu Mustaf Ahmed ahmed719@umn.edu Ellie Hollenberger holle299@umn.edu

Contributions: Our team chose to meet on Zoom for the project, and work through the code togther using Repl.