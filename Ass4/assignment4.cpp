#include <fstream>
#include <string.h>
#include <iomanip>
#include <ostream>
#include <cstring>
#include <iostream>

using namespace std;

struct Frames{
	int frame1;
	int frame2;
	int frame3;
};

void fifoAlg(Frames f, int string[100], int length);
void optimalAlg(Frames f, int string[100], int length);
void lruAlg(Frames f, int string[100], int length);

int main()
{
	Frames f;
    int length = 0;
    int refString[100];

    ifstream myfile;
    myfile.open("refString.txt");

    while(!myfile.eof())
    {
        myfile >> refString[length];

        length++;
    }
	length--;

	fifoAlg(f, refString, length);
	optimalAlg(f, refString, length);
	lruAlg(f, refString, length);

	myfile.close();

    return 0;
}

void fifoAlg(Frames f, int string[100], int length)
{
	int pFaults = 0;
	int lastFrame = 3;

	cout << "FIFO Algorithm starting...\n";

	for(int i = 0; i < length; i++)
	{
		cout << string[i] << "|";
		if(f.frame1 != string[i] && f.frame2 != string[i] && f.frame3 != string[i])
		{
			if(lastFrame == 3)
			{
				f.frame1 = string[i];
				pFaults++;
				lastFrame = 1;
			}
			else if(lastFrame == 1)
			{
				f.frame2 = string[i];
				pFaults++;
				lastFrame = 2;
			}
			else
			{
				f.frame3 = string[i];
				pFaults++;
				lastFrame = 3;
			}
		}
		else if(string[i] == 0 && i < 3)
		{
			pFaults++;
			lastFrame = i + 1;
		}

		if(i == 0)
		{
			cout << f.frame1 << endl;
		}
		else if(i == 1)
		{
			cout << f.frame1 << " " << f.frame2 << endl;
		}
		else
		{
			cout << f.frame1 << " " << f.frame2 << " " << f.frame3 << endl;
		}	
	}
	cout << endl;
	cout << pFaults << " Faults\n\n";
}

void optimalAlg(Frames f, int string[100], int length)
{
	int pFaults = 0;
	int lastFrame = 3;
	int f1 = 1;
	int f2 = 1;
	int f3 = 1;

	cout << "Optimal Algorithm starting...\n";

	for(int i = 0; i < length; i++)
	{
		cout << string[i] << "|";
		if(f.frame1 != string[i] && f.frame2 != string[i] && f.frame3 != string[i])
		{
			if(i >= 3)
			{
				for(int n = i; n < length;n++)
				{
					if(f.frame1 == string[n])
					{
						f1 = -1;

						if (f2 == -1)
						{
							f.frame3 = string[i];
							pFaults++;
							break;
						}
						else if(f3 == -1)
						{
							f.frame2 = string[i];
							pFaults++;
							break;
						}
						
					}
					else if(f.frame2 == string[n])
					{
						f2 = -1;

						if(f1 == -1)
						{
							f.frame3 = string[i];
							pFaults++;
							break;
						}
						else if(f3 == -1)
						{
							f.frame1 = string[i];
							pFaults++;
							break;
						}

					}
					else if(f.frame3 == string[n])
					{
						f3 = -1;

						if(f1 == -1)
						{
							f.frame2 = string[i];
							pFaults++;
							break;
						}
						else if(f2 == -1)
						{
							f.frame1 = string[i];
							pFaults++;
							break;
						}

					}
				}
			}
			else
			{
				if(i == 0)
				{
					f.frame1 = string[i];
					pFaults++;
				}
				else if(i == 1)
				{
					f.frame2 = string[i];
					pFaults++;
				}
				else if(i == 2)
				{
					f.frame3 = string[i];
					pFaults++;
				}
				
			}

			f1 = 1;
			f2 = 1;
			f3 = 1;

		}
		else if(string[i] == 0 && i < 3)
		{
			pFaults++;
		}

		if(i == 0)
		{
			cout << f.frame1 << endl;
		}
		else if(i == 1)
		{
			cout << f.frame1 << " " << f.frame2 << endl;
		}
		else
		{
			cout << f.frame1 << " " << f.frame2 << " " << f.frame3 << endl;
		}	
		
	}
	cout << endl;
	cout << pFaults << " Faults\n\n";
}

void lruAlg(Frames f, int string[100], int length)
{
	int pFaults = 0;
	int leastFrame = 3;
	int used1 = 1;
	int used2 = 2;
	int used3 = 3;

	cout << "LRU Algorithm starting...\n";

	for(int i = 0; i < length; i++)
	{
		cout << string[i] << "|";
		if(f.frame1 != string[i] && f.frame2 != string[i] && f.frame3 != string[i])
		{
			if(i < 3)
			{
				if(i == 0)
				{
					f.frame1 = string[i];
					pFaults++;
					used1 = 1;
				}
				else if(i == 1)
				{
					f.frame2 = string[i];
					pFaults++;
					used2 = 2;
				}
				else if(i == 2)
				{
					f.frame3 = string[i];
					pFaults++;
					used3 = 3;
				}
			}
			else
			{
				if(used1 < used2 && used1 < used3)
				{
					f.frame1 = string[i];
					pFaults++;
					used1 = 3;
					used2--;
					used3--;
				}
				else if(used2 < used1 && used2 < used3)
				{
					f.frame2 = string[i];
					pFaults++;
					used2 = 3;
					used1--;
					used3--;
				}
				else if(used3 < used1 && used3 < used2)
				{
					f.frame3 = string[i];
					pFaults++;
					used3 = 3;
					used1--;
					used2--;
				}
			}
		}
		else if(string[i] == 0 && i < 3)
		{
			pFaults++;
		}
		else
		{
			if(f.frame1 == string[i])
			{
				used1 = 3;
				used2--;
				used3--;
			}
			else if(f.frame2 == string[i])
			{
				used2 = 3;
				used1--;
				used3--;
			}
			else if(f.frame3 == string[i])
			{
				used3 = 3;
				used1--;
				used2--;
			}
		}
		if(i == 0)
		{
			cout << f.frame1 << endl;
		}
		else if(i == 1)
		{
			cout << f.frame1 << " " << f.frame2 << endl;
		}
		else
		{
			cout << f.frame1 << " " << f.frame2 << " " << f.frame3 << endl;
		}
	}
	cout << endl;
	cout << pFaults << " Faults\n\n";

}