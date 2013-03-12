#include <X11/Xlib.h>
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
///////GLOBALS
///////////
Display *display;
Window frame_window; 
int rootx_buf, rooty_buf;char prevKeymap[32];


/////////////////
////FUNCTIONS
//////

void option_reading(int, char**);
void* keypress_detector_function(void* a);
pid_t launch_process(pid_t);
void kill_including_children(pid_t pid);
bool kbdActivity(Display* display);
int mouse_moved(Display *d, Window w);
void display_opener();
void screen_window_opener();