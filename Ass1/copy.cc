#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

int main()
{
	
	int fd_in, fd_out, remove_out;
	off64_t len, ret;
	struct stat stat;
	pid_t child_val;
	int etest = 0;
	
	char buf[10];
	
	fd_in = open("Success", O_RDONLY);
	if (fd_in == -1)
	{
		perror("open Success");
		exit(EXIT_FAILURE);
		
	}
	
	len = stat.st_size;
	
	while (read(fd_in, &buf[1], 1) ==1)
	{
		cout << buf[1];
	}
	
	cout << endl;
	
	fd_out = open("Copy", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		perror("open Copy");
		exit(EXIT_FAILURE);
	}
	
	do
	{
		ret = copy_file_range(fd_in, NULL, fd_out, NULL, len, 0);
		if (ret == -1) 
		{ 
			perror ("copy_file_range");
			exit(EXIT_FAILURE);
		}
		
		len -= ret;
	} while (len > 0 && ret > 0);
	
	close(fd_in);
	close(fd_out);
	
	remove_out = remove("/home/nbryan/Copy");
	if (remove_out == -1)
	{
		perror("couldnt delete");
		exit(EXIT_FAILURE);
	}
	
		// Fork child process
		child_val = fork();
		
		if(child_val < 0)
		{
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		else if (child_val == 0) //child process
		{
			char *path = "firefox";
			char *args[] = {path, "www.fairmontstate.edu", NULL};
			
			etest = execv("firefox", args);
			if(etest < 0)
			{
				cout << "could not open browser\n";
			}
			
		}
		else //parent process
		{
			wait(NULL);
			
			printf("Firefox Open \n");
		}
		
	return 0;
}
