#include "timed_killer-runner.h"

//////////////////////
//////GLOBALS
/////////////////

int life_time = 5;
bool keypress_detected = false;
string programPath = "";
int kill_name_specified = 0;
string *killName;
bool input_detection = true;

///////////////////////
/////////MAIN//////////
//////////////////////

int main(int argc, char **argv)
{

	option_reading(argc, argv);

	display_opener();
	screen_window_opener();


	pid_t pid;
	pid = launch_process(pid);


	pthread_t keypress_detector;

	pthread_create(&keypress_detector,NULL,&keypress_detector_function,NULL);

	struct timeval start, end;

	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);
	cout << endl;

	while(true)
	{
		gettimeofday(&end, NULL);
		printf("\033[A");
		printf("seconds left: %d \n", life_time - (int)(end.tv_sec  - start.tv_sec) );
		if(keypress_detected)
		{
			gettimeofday(&start, NULL);
			gettimeofday(&end, NULL);
			keypress_detected =false;
			//cerr << "keypress processed" << endl;//debug
		}

		if((end.tv_sec  - start.tv_sec) > life_time)
		{
			gettimeofday(&start, NULL);
			gettimeofday(&end, NULL);
			cerr << "time up. Killing process " << pid << endl;//debug

			//killing
			if(!kill_name_specified)
				kill_including_children(pid);
			else
			{
				for(int i=0;i<kill_name_specified;i++)
				{
					string command = "killall ";
					command += killName[i];

					system(command.c_str());
				}				
			}

			pid = launch_process(pid);

		}
		sleep(1);
	}

	return 0;


}


void* keypress_detector_function(void* a)
{
	if(input_detection)
	{
		while(true)
		{		
			if(kbdActivity(display) || mouse_moved(display,frame_window))
			{
				keypress_detected = true;
		//cerr << "keypress detected" << endl;//debug
			}
			usleep(100000);
		}
	}
	else
	{
		while(true)
		{
		cin.ignore();//wait for keypress
		keypress_detected = true;
		cerr << "keypress detected" << endl;//debug
		}
	}
}

pid_t launch_process(pid_t pid)
{

	pid = fork(); // Create a child process 

	switch (pid) 
	{
		case -1: // Error
			std::cerr << "fork() failed.\n";
		exit(1);
		case 0: // Child process - on sucess fork() returns PID in PARENT and 0 in child
			execl(programPath.c_str(), "", (const char*)NULL); // Execute the program with no argumets in CHILD
		std::cerr << "execl() failed!"; // execl doesn't return unless there's an error 
		exit(1);
		default: // Parent process 
			std::cout << "Process created with pid " << pid << "\n";
	}

	return pid;
}

void timer_set_exit()
{
	cerr<<"timer already set. exiting" << endl;
	exit(1);
}

void option_reading(int argc, char **argv)
{

	bool timer_set = false;

	for(int i = 0; i<argc; i++)
	{
		if((string)argv[i] == "--no-input-detection")
		{
			input_detection = false;
		}

		if((string)argv[i] == "-stimer")
		{
			if(timer_set){timer_set_exit();}
			i++;
			life_time = atoi(argv[i]);
			timer_set = true;
		}

		if((string)argv[i] == "-mtimer")
		{
			if(timer_set){timer_set_exit();}
			i++;
			life_time = atoi(argv[i])*60;
			timer_set = true;
		}

		if((string)argv[i] == "-htimer")
		{
			if(timer_set){timer_set_exit();}
			i++;
			life_time = atoi(argv[i])*3600;
			timer_set = true;
		}

		if((string)argv[i] == "-prog")
		{
			i++;
			programPath = argv[i];
			cerr << "path " << programPath << endl;//debug
		}

		if((string)argv[i] == "-killname")
		{

			//kill_name_specified = true;
			i++; int j = 0;string killNametemp[1000];
			while((string)argv[i] != "0")
			{
				cerr << argv[i] << endl;
				killNametemp[j] = argv[i];
				i++; j++;kill_name_specified++;
			}

			killName = new string[j];
			for(int k =0; k<j; k++)
			{
				killName[k] =  killNametemp[k];
			}

		}

	}

}

void kill_including_children(pid_t pid)//not including children yet
{
	kill(pid,SIGTERM);
}

void display_opener()
{
	display = XOpenDisplay(NULL); if(display == NULL){printf("Can't open display. Keyboard and mouse movement detection will not be available.",0);}
}

void screen_window_opener()
{	
	XSetWindowAttributes frame_attributes;
	XWindowAttributes window_attributes;

	frame_attributes.override_redirect = true;
	frame_attributes.background_pixel = XWhitePixel(display, 0);

	XGetWindowAttributes(display, XRootWindow(display, 0), &window_attributes);	

	frame_window = XCreateWindow(display, XRootWindow(display, 0),
                                 0,0, window_attributes.width, window_attributes.height, 0,  DefaultDepth(display, 0),
                                 InputOutput, DefaultVisual(display, 0), CWBackPixel|CWOverrideRedirect,
                                 &frame_attributes);	
}


int mouse_moved(Display *d, Window w)
{
bool query;

Window root, child;
int rootx, rooty, childx, childy;
unsigned int mask;

query = XQueryPointer(d, w , &root, &child, &rootx, &rooty, &childx, &childy, &mask);

if ((rootx == rootx_buf) && (rooty == rooty_buf))
{return false;}
else{rootx_buf = rootx; rooty_buf = rooty; return true;}

}//end mouse_moved

bool kbdActivity(Display* display)  // checks for key presses
{char keymap[32]; 
    XQueryKeymap(display, keymap);  // asks x server for current keymap
    for (int i=0; i<32; i++)        // for 0 to 32 (keymap size)
    {
      if (prevKeymap[i] != keymap[i])   // if previous keymap does not
       {                              // equal current keymap 
            XQueryKeymap(display, prevKeymap);  // ask for new keymap
            return true;                // exit with true
        }
    }
    return false;                   // no change == no activity
}//KbdActivity