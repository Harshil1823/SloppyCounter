## sloppysim

##description
multi-threaded program that allows user to experiment with different parameters to ovserve the behavior of threads and how the local counter for the threads affect the global counter. 

You can play around with parameters such as 
<N_Threads> This simply is number of threads
<Sloppiness> Basically how much you want program to increment by
<work_time> time your allowing the threads to do work, if short time than can cause livelock
<work_iterations> thread will repeatedly perform work for the specified number of iterations
<cpu_bound> /CPU-bound threads, you should aim to keep the CPU cores.
// busy and actively executing code for as much of the work_time as possible. 
// This may involve using busy waiting loops or other CPU-intensive operations.
<Do_logging> /should print out information about the progress thread is making.
Including the settings and the global counter value at specified intervals


##compile instruction
has makefile in it. You can simply call make and executable will be creaated with the name of sloppySim. 
Then to run and experiment with the program you can run command as below. 

./sloppySim 2 1 10 20 false true

note it goes as <N_threads> <Sloppiness> <work_time> <work_iterations> <cpu_bound> <do_logging> 

Above is mentioned how the program takes in param.

If you don't enter any values it will have default values to run the progam. Default values are listed below. 

int N_Threads = 2;
int Sloppiness = 10;
int WorkTime = 10;
int WorkIterations = 100;
int CPU_Bound = 0; // false
int DoLogging = 0; // false
