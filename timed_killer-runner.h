#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string>

using namespace std;

/////////////////
////FUNCTIONS
//////

void option_reading(int, char**);
void* keypress_detector_function(void* a);
pid_t launch_process(pid_t);
