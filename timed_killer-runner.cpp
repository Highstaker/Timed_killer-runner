#include "timed_killer-runner.h"

//////////////////////
//////GLOBALS
/////////////////

int life_time = 5;
bool keypress_detected = false;
string programPath = "";
int kill_name_specified = 0;
string *killName;



///////////////////////
/////////MAIN//////////
//////////////////////

int main(int argc, char **argv)
{

	option_reading(argc, argv);

	pid_t pid;
	pid = launch_process(pid);


	pthread_t keypress_detector;

	pthread_create(&keypress_detector,NULL,&keypress_detector_function,NULL);

	struct timeval start, end;

	gettimeofday(&start, NULL);
	gettimeofday(&end, NULL);

	while(true)
	{
		gettimeofday(&end, NULL);
		if(keypress_detected)
		{
			gettimeofday(&start, NULL);
			gettimeofday(&end, NULL);
			keypress_detected =false;
			cerr << "keypress processed" << endl;//debug
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

				pid = launch_process(pid);
			}

		}
		sleep(1);
	}

	return 0;


}


void* keypress_detector_function(void* a)
{
	while(true){
		cin.ignore();//wait for keypress
		keypress_detected = true;
		cerr << "keypress detected" << endl;//debug
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