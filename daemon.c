#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>

// Fork off parent
// Change File mode mask
// Create Unique Session ID
// Change Current Working Dir to Safe Place
// open files for logging
// Close Standard File Descriptors

void become_daemon()
{
	pid_t pid;
	// fork off the parent proccess.
	pid = fork();
	switch(pid)
	{
		case -1:
			// failure - log appropriate failure message and exit
			exit(-1);
		case 0:
			// change the file mode mask
			umask(0);		

			// Open any logs if needed

			// get new SID for Child
			if (setsid() == -1){
				// failure - log appropriate failure message and exit
				exit(-1);
			}				
			// close standard file descriptors.
			close(STDIN_FILENO); close(STDOUT_FILENO); close(STDERR_FILENO);
			// actual daemon code
			while(1){
				// Daemon at Work !
			}

		default:
			// parent will exit.
			exit(0);
	}
	return;
}

int main()
{
	// some stuff you want to do before becoming a daemon.

	become_daemon();

	return 0;
}
