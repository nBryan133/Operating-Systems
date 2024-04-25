#include <semaphore.h>
#include <fstream>
#include <string.h>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <cstring>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std;

sem_t* mutex;
sem_t* nxt;

void decrypt(char buffer[], int length);

int main()
{
	int i = 0;
	string buffer1;
	int strl;
	int loops;
	
	shm_unlink("Sem");
	
	ofstream newfile;
	newfile.open("output.txt");
	
	//needed for shared memory
	const int SIZE = 4096; 							//size of shared memory
	const char *name = "OS"; 						//name of shared memory
	int fd;											//file descriptor id
	char *ptr;										//points to shared memory segment
	char *loc;
	
	//accesses shared memory
	fd = shm_open(name, O_RDWR, 0666);										//accesses shared memory opened by parent
	ptr = (char*)mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);	//gets pointer location for shared memory opened by parent
	
	//testing shared memory
	//printf("%s", (char*)ptr);
	
	//setup semaphores
	sem_t *sem_prod = sem_open("/myproducer", 0);
	if(sem_prod == SEM_FAILED)
	{
		perror("sem_open/producer");
		exit(EXIT_FAILURE);
	}
	
	sem_t *sem_cons = sem_open("/myconsumer", 1);
	if(sem_cons == SEM_FAILED)
	{
		perror("sem_open/consumer");
		exit(EXIT_FAILURE);
	}

	cout << "How many passwords are being read at a time?: \n";
	cin >> loops;
	cout << endl;
	
	do
	{
		loc = ptr;
		sem_wait(sem_cons);
		
		for(int b = 0; b < loops; b++)
		{
			buffer1 = (string)loc;
			
			if(buffer1 == "*****")
			{
				break;
			}
			
			strl = (buffer1.find("\n"));
			loc += strl + 2;
			
			char* buffer2 = new char[strl + 1];
			strcpy(buffer2, buffer1.c_str());
			
			decrypt(buffer2, strl);
			
			cout << buffer2;
			
			newfile << buffer2;
		}
		
		sem_post(sem_prod);
		i++;
	}while(buffer1 != "*****");
	
	sem_close(sem_prod);
	sem_close(sem_cons);
	
	shm_unlink(name);							//unlinks the proccess with the shared memory segment
	return 0;
}



void decrypt(char buffer[], int length)
{
	char buffer2[length];

	for(int a = 0; a < length; a++)
	{
		buffer2[a] = buffer[a];
	}
	
	for(int a = 0; a < length; a++)
	{
		if( a > 0)
		{
			buffer[a] = buffer2[a - 1];
		}
		else
		{
			buffer[a] = buffer2[length - 1];
		}
	}
	
	for(int a = 0; a < length; a++)
	{
		buffer[a]--;
	}
	
}
