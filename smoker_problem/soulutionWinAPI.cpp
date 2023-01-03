#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

void agent();
void smoker(int);


enum iState { EMPTY, FULL };
enum sState { GONE, PRESENT, CRAVING, SMOKING, DONE };
int item[3] = { EMPTY }; 
int smokerCount = 0;
int smokerState[3] = { GONE };
int numTimesSmoked[3] = { 0, 0, 0 };
const int NUM_TO_SMOKE = 100; 


HANDLE mutex = CreateMutex(NULL, FALSE, NULL);



void main()
{ 
	HANDLE sThread[4];                  
	DWORD  sThreadID[4];                
	DWORD WINAPI proc(LPVOID);          

	
	srand ( time(NULL) );   
	
	for (int smokerNbr = 0; smokerNbr < 4; smokerNbr++)
	{
		sThread[smokerNbr] = CreateThread(NULL, 0, proc, NULL, 0, &sThreadID[smokerNbr]);
	}

	WaitForMultipleObjects(4, sThread, true, INFINITE); 
	cout << "press CR to end."; while (_getch() != '\r');
	return;
}



DWORD WINAPI proc(LPVOID)    
{
	int myID;

	WaitForSingleObject(mutex, INFINITE);   
	myID = smokerCount++;
	ReleaseMutex(mutex);                    
	smokerState[myID] = PRESENT;

	if (myID == 3)
	{
		WaitForSingleObject(mutex, INFINITE);
		cout << "Calling Agent \n";
		ReleaseMutex(mutex);
		agent();
	}
	else
	{
		WaitForSingleObject(mutex, INFINITE);
		cout << "Calling Smoker " << myID << endl;
		ReleaseMutex(mutex);
		smokerState[myID] = PRESENT;
		smoker(myID);
	}
	return 0;
}


void agent()
{
	
	while (smokerCount < 3) Sleep(0);

	
	while (numTimesSmoked[0] < NUM_TO_SMOKE || numTimesSmoked[1] < NUM_TO_SMOKE || numTimesSmoked[2] < NUM_TO_SMOKE)
	{
		
		int smoker = rand() % 3;

		
		while (numTimesSmoked[smoker] >= NUM_TO_SMOKE)
			smoker = rand() % 3;

		
		WaitForSingleObject(mutex, INFINITE);
		item[(smoker + 1) % 3]++;
		item[(smoker + 2) % 3]++;
		
		ReleaseMutex(mutex);

		
		while (item[0] == FULL || item[1] == FULL || item[2] == FULL) Sleep(0);
	}
}

void smoker(int myID)
{	
	
	while (numTimesSmoked[myID] < NUM_TO_SMOKE)
	{
		smokerState[myID] = CRAVING;

		
		if (item[(myID + 1) % 3] == FULL && item[(myID + 2) % 3] == FULL)
		{
			
			WaitForSingleObject(mutex, INFINITE);
			item[(myID + 1) % 3]--;
			item[(myID + 2) % 3]--;
			numTimesSmoked[myID]++;
			ReleaseMutex(mutex);			

			
			smokerState[myID] = SMOKING;
			

			Sleep(rand() % 10);
		}
		else Sleep(0);
	}

	
	smokerState[myID] = DONE;
	WaitForSingleObject(mutex, INFINITE);
	cout << "Smoker " << myID << " done." << endl;
	ReleaseMutex(mutex);
}
