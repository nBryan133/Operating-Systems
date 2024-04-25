#include <iostream>
#include <stdio.h>
#include <fstream>
#include <pthread.h>
#include <sys/wait.h>
#include <cstring>
#include <string>

using namespace std;

int strl;
string buffer1;
char buffer3[20];

void encrypt(char buffer[], int length);
void decrypt(char buffer[], int length);

void *worker_thread(void *arg);

int main()
{
	string fileName;
	int threadNum;
	
	
	cout << "Please enter the name of the file: \n";
	cin >> fileName;
	cout << endl << endl;
	
	cout << "Please enter how many threads you would like to use: \n";
	cin >> threadNum;
	cout << endl << endl;
	
	ifstream myfile;
	myfile.open(fileName);
	
	ofstream newfile;
	newfile.open("output.txt");
	
	pthread_t my_thread[threadNum];
	
	do
	{
		for(long id = 1; id <= threadNum; id++)
		{
			getline(myfile, buffer1);
			
			if(!myfile.eof())
			{
				
				
				pthread_create(&my_thread[id], NULL, &worker_thread, (void*)id);
				int ret = pthread_join(my_thread[id], NULL);

				if(ret != 0)
				{
					printf("Error: pthread_join() failed\n");
					exit(EXIT_FAILURE);
				}
				
				newfile << buffer1;
				
				newfile << " : ";
				
				for(int a = 0; a < strl; a++)
				{
					newfile << buffer3[a];
				}
				
				newfile << endl;
					
			}
			else
			{
				cout << endl;
				
				myfile.close();
				newfile.close();
				
				pthread_exit(NULL);
			}
		}
	}while(!myfile.eof());	
	
	return 0;
}

void *worker_thread(void *arg)
{
	//printf("This is worker_thread #%Id\n\n", (long)arg);
	
	strl = buffer1.length();
	
	char* buffer2 = new char[strl + 1];
	
	strcpy(buffer2, buffer1.c_str());
		
	encrypt(buffer2, strl);
	
	cout << buffer1;
	
	cout << " : ";
	
	for (int a = 0; a < strl; a++)
	{
		cout << buffer2[a];
		
		buffer3[a] = buffer2[a];
	}
		
	cout << endl;
			
	pthread_exit((void*)911);
}

void encrypt(char buffer[], int length)
{
	for( int a = 0; a < length; a = a + 1)
	{
		buffer[a] += 1;
	}
}

void decrypt(char buffer[], int length)
{
	for( int a = 0; a < length; a = a + 1)
	{
		buffer[a] -= 1;
	}
}
