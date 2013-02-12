#include "timed_killer-runner.h"

//////////////////////
//////GLOBALS
/////////////////

int life_time = 5;
bool keypress_detected = false;
string programPath = "/usr/bin/leafpad";



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
    kill(pid,SIGKILL);

    pid = launch_process(pid);
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
            life_time = atoi(argv[i+1]);
            timer_set = true;
        }

        if((string)argv[i] == "-mtimer")
        {
          if(timer_set){timer_set_exit();}
          life_time = atoi(argv[i+1])*60;
          timer_set = true;
      }

      if((string)argv[i] == "-htimer")
      {
        if(timer_set){timer_set_exit();}
        life_time = atoi(argv[i+1])*3600;
        timer_set = true;
    }

    if((string)argv[i] == "-prog")
    {
        programPath = argv[i+1];
    cerr << "path " << programPath << endl;//debug
}
}

}