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
	
void encrypt(char buffer[], int length);

int main()
{	
	ifstream myfile;
	myfile.open("passwords");
	
	string buffer1;
	
	int strl;
	int loops;
	
	//variables needed to make shared memory segment
	const int SIZE = 4096; 							//size of shared memory
	const char *name = "OS"; 						//name of shared memory
	int fd;									//file descriptor id
	char *ptr;
	char *loc;								//points to shared memory segment

	//creating shared memory segment
	fd = shm_open(name, O_CREAT|O_RDWR, 0666);  				//opens the shared memory and gets the dile descriptor id
		
	ftruncate(fd, SIZE);							//makes sure we get the amount of memory requested for the segment
	ptr = (char*)mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); 	//puts the memory location into the pointer
	
	int id;
	
	memset(ptr,'\0', strlen(ptr) + 1);
	
	//setup semaphores
	sem_unlink("/myproducer");
	sem_unlink("/myconsumer");
	
	sem_t *sem_prod = sem_open("/myproducer", O_CREAT, 0660, 1);
	if(sem_prod == SEM_FAILED)
	{
		perror("sem_open/producer");
		exit(EXIT_FAILURE);
	}
	
	sem_t *sem_cons = sem_open("/myconsumer", O_CREAT, 0660, 0);
	if(sem_cons == SEM_FAILED)
	{
		perror("sem_open/consumer");
		exit(EXIT_FAILURE);
	}
	
	cout << "How many passwords would you like to manage at a time?: \n";
	cin >> loops;
	cout << endl;
		
	while(!myfile.eof())
	{
		
		loc = ptr;
		
		sem_wait(sem_prod);
		for(int i = 0; i < loops; i++)
		{
			getline(myfile, buffer1);
				
			if(!myfile.eof())
			{
					
				strl = buffer1.length();
						
				char* buffer2 = new char[strl + 1];
						
				strcpy(buffer2, buffer1.c_str());
						
				encrypt(buffer2, strl);
						
				sprintf(loc, "%s", buffer2);
				loc += strl;
				sprintf(loc, "\n");
				loc++;
				loc++;
				
			}
			else
			{
				myfile.close();
				sprintf(loc, "*****");
				break;
			}
		}	
		printf("\nSending to Consumer\n");
		sem_post(sem_cons);
	
	}	
	
	sem_close(sem_prod);
	sem_close(sem_cons);
	
	
	return 0;
}


void encrypt(char buffer[], int length)
{
	char buffer2[length];

	for(int a = 0; a < length; a++)
	{
		buffer2[a] = buffer[a];
	}
	
	for(int a = 0; a < length; a++)
	{
		if( a < length - 1)
		{
			buffer[a] = buffer2[a + 1];
		}
		else
		{
			buffer[a] = buffer2[0];
		}
	}
	
	for(int a = 0; a < length; a++)
	{
		buffer[a] += 1;
	}
}

