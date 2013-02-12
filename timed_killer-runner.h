#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

using namespace std;

/////////////////
////FUNCTIONS
//////

void option_reading(int, char**);
void* keypress_detector_function(void* a);
pid_t launch_process(pid_t);
void kill_including_children(pid_t pid);
