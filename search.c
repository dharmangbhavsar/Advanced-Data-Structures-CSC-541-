#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#define fr freopen("input.in","r",stdin)
#define fw freopen("output.out","w",stdout)
struct timeval startTime, endTime;

void MemoryLinearSearch(char *keyFile, char*seekFile);
void startInMemoryLinearSearch(int *keyArray, int keyPos, int *seekArray, int seekPos, double exactTime);
void MemoryBinarySearch(char *keyFile, char *seekFile);
void startInMemoryBinarySearch(int* keyArray,int keyPos, int*seekArray, int seekPos, double exactTime);
int binSearch(int *keyArray, int seekValue, int min, int max);

void DiskLinearSearch(char *keyFile, char *seekFile);
void startDiskLinearSearch(FILE* keyFile, int keyPos, int* seekArray, int seekPos);
void DiskBinarySearch(char *keyFile, char*seekFile);
void startOnDiskBinarySearch(FILE* keyArray, int keyPos, int*seekArray, int seekPos);
int binSearchDisk(FILE *keyFile, int seekValue, int min, int max);
double getExactTime(struct timeval startTime, struct timeval endTime);






void MemoryLinearSearch( char *keyFile, char *seekFile)
{
	FILE *keys, *seeks;
	keys = fopen(keyFile, "rb");
	if(keys==NULL)
	{ printf("\n Not able to Open the File containing the key values. \n"); return;}
	seeks = fopen(seekFile, "rb");
	if(seeks==NULL)
	{ printf("\n Not able to Open the File containing the seek values. \n"); return;}
	fseek(keys, 0, SEEK_END);
	int keysLength = ftell(keys);
	fseek(keys, 0, SEEK_SET);
	fclose(keys);
	fseek(seeks, 0, SEEK_END);
	int seeksLength = ftell(seeks);
	fseek(seeks, 0, SEEK_SET);
	
	int *keyArray = (int*) malloc(keysLength);
	int *seekArray = (int*) malloc(seeksLength);
	int keyPos = 0, seekPos=0;
	gettimeofday(&startTime, NULL);
	keys = fopen(keyFile,"rb");

	while(!feof(keys))
	{
		fread((keyArray+keyPos), sizeof(int), 1, keys);
		keyPos++;
	}
	gettimeofday(&endTime, NULL);
	getExactTime(startTime, endTime);
	while(!feof(seeks))
	{
		fread((seekArray+seekPos), sizeof(int), 1, seeks);
		seekPos++;
	}
	gettimeofday(&endTime, NULL);
	double exactTime = (getExactTime(startTime, endTime));
	keyPos--;
	seekPos--;

	startInMemoryLinearSearch(keyArray, keyPos, seekArray, seekPos, exactTime);
	fclose(keys); fclose(seeks);
	return;
}
void startInMemoryLinearSearch(int *keyArray, int keyPos, int *seekArray, int seekPos, double exactTime)
{
	int found[seekPos], seekCounter, keyCounter; double timer;

	gettimeofday(&startTime, NULL);
	for(seekCounter = 0; seekCounter<seekPos; seekCounter++)
	{
		for(keyCounter =0; keyCounter<keyPos; keyCounter++)
		{
			if(seekArray[seekCounter] == keyArray[keyCounter]) {found[seekCounter] = 1;break; }
			else found[seekCounter] = 0;
		}
	}
	gettimeofday(&endTime, NULL);

	timer = getExactTime(startTime, endTime);
	timer+= exactTime;
	for(seekCounter = 0; seekCounter<seekPos; seekCounter++)
	{
		if(found[seekCounter] == 1) printf("%12d:Yes\n", seekArray[seekCounter]);
		else printf("%12d:No\n", seekArray[seekCounter]);
	}
	printf("Time Taken : %.8f\n", timer);
}

//Now Starts Binary Search Thingy

void MemoryBinarySearch(char *keyFile, char *seekFile)
{
	FILE *keys, *seeks;
	keys = fopen(keyFile, "rb");
	if(keys==NULL){printf("\n Cannot open the fle containing the values of keys \n"); return; }

	seeks = fopen(seekFile, "rb");
	if(seeks == NULL) {printf("\n Cannot open the file containing the values of seeks \n"); return;}

	fseek(keys, 0, SEEK_END);
	int keyLength = ftell(keys);
	fseek(keys, 0, SEEK_SET);
	fclose(keys);
	fseek(seeks, 0, SEEK_END);
	int seeksLength = ftell(seeks);
	fseek(seeks, 0, SEEK_SET);
	
	int *keyArray = (int*) malloc(keyLength);
	int *seekArray = (int*) malloc(seeksLength);
	int keyPos = 0, seekPos=0;
	gettimeofday(&startTime, NULL);
	keys = fopen(keyFile,"rb");

	while(!feof(keys))
	{
		fread((keyArray+keyPos), sizeof(int), 1, keys);
		keyPos++;
	}
	gettimeofday(&endTime, NULL);
	getExactTime(startTime, endTime);
	
	while(!feof(seeks))
	{
		fread((seekArray+seekPos), sizeof(int), 1, seeks);
		seekPos++;
	}
	gettimeofday(&endTime, NULL);
	double exactTime = (getExactTime(startTime, endTime));
	keyPos--;
	seekPos--;

	startInMemoryBinarySearch(keyArray, keyPos, seekArray, seekPos, exactTime);
	fclose(keys); fclose(seeks);
	return;
}

void startInMemoryBinarySearch(int* keyArray, int keyPos, int* seekArray, int seekPos, double exactTime)
{
	int seekCounter =0, found[seekPos]; double timer;
	gettimeofday(&startTime, NULL);
	for(seekCounter = 0; seekCounter<seekPos; seekCounter++)
	{
		int ans = binSearch(keyArray, seekArray[seekCounter], 0, (keyPos-1));
		if(ans==1) found[seekCounter]=1;
		else found[seekCounter] =0;
	}
	gettimeofday(&endTime, NULL);
	timer = getExactTime(startTime, endTime);
	timer+=exactTime;
	for(seekCounter=0; seekCounter<seekPos; seekCounter++)
	{
		if(found[seekCounter]==1) printf("%12d:Yes\n", seekArray[seekCounter]);
		else printf("%12d:No\n", seekArray[seekCounter]);
	}
	printf("Time: %0.8f\n", timer);
}

int binSearch(int *keyArray, int seekValue, int min, int max)
{
	int middleValue = 0;
	if(max<min) return 0;
	else
	{
		middleValue = ((max+min)/2);
		if(keyArray[middleValue]<seekValue)
		{return binSearch(keyArray, seekValue, (middleValue+1), max);}
		else if(keyArray[middleValue]>seekValue)
		{return binSearch(keyArray, seekValue, min, (middleValue-1));}
		else
		return 1;  
	}
	return 0;
}

//now we start the on disk algorithms

//Firstly On disk linear search
void DiskLinearSearch(char *keyFile, char *seekFile)
{
	FILE *keys, *seeks;
	keys = fopen(keyFile, "rb");
	if(keys==NULL){printf("\n Cannot open the fle containing the values of keys \n"); return; }

	seeks = fopen(seekFile, "rb");
	if(seeks == NULL) {printf("\n Cannot open the file containing the values of seeks \n"); return;}

	fseek(keys, 0, SEEK_END);
	int keyLength = ftell(keys);
	fseek(keys, 0, SEEK_SET);
	fclose(keys);
	fseek(seeks, 0, SEEK_END);
	int seeksLength = ftell(seeks);
	fseek(seeks, 0, SEEK_SET);
	
	int *keyArray = (int*) malloc(keyLength);
	int *seekArray = (int*) malloc(seeksLength);
	int keyPos = 0, seekPos=0;
	keys = fopen(keyFile,"rb");
	keyPos = (keyLength/sizeof(int));
	while(!feof(seeks))
	{
		fread((seekArray+seekPos), sizeof(int), 1, seeks);
		seekPos++;
	}
	seekPos--;

	startDiskLinearSearch(keys, keyPos, seekArray, seekPos);
	fclose(keys); fclose(seeks);
	return;
}
void startDiskLinearSearch(FILE* keyFile, int keyPos, int* seekArray, int seekPos)
{
	int seekCounter = 0, found[seekPos], readKeyValue, counter=0; double timer;
	gettimeofday(&startTime, NULL);
	for(seekCounter = 0; seekCounter<seekPos; seekCounter++)
	{
		fseek(keyFile, 0, SEEK_SET);
		while(!feof(keyFile))
		{
			fread(&readKeyValue, sizeof(int), 1, keyFile);
			if(seekArray[seekCounter]==readKeyValue)
			{
				found[seekCounter]=1; counter++; break;
			}
			else found[seekCounter]=0;
		}
	}
	gettimeofday(&endTime, NULL);
	timer = getExactTime(startTime, endTime);
	for(seekCounter=0; seekCounter<seekPos; seekCounter++)
	{
		if(found[seekCounter] ==1 )
			printf("%12d:Yes\n", seekArray[seekCounter]);
		else
			printf("%12d:No\n", seekArray[seekCounter]);
	}
	printf("Time: %0.8f\n", timer);
}


//starting the dis Binary Search space here
void DiskBinarySearch(char *keyFile, char* seekFile)
{
	FILE *keys, *seeks;
	keys = fopen(keyFile, "rb");
	if(keys==NULL){printf("\n Cannot open the fle containing the values of keys \n"); return; }

	seeks = fopen(seekFile, "rb");
	if(seeks == NULL) {printf("\n Cannot open the file containing the values of seeks \n"); return;}

	fseek(keys, 0, SEEK_END);
	int keyLength = ftell(keys);
	fseek(keys, 0, SEEK_SET);
	fclose(keys);
	fseek(seeks, 0, SEEK_END);
	int seeksLength = ftell(seeks);
	fseek(seeks, 0, SEEK_SET);
	
	int *keyArray = (int*) malloc(keyLength);
	int *seekArray = (int*) malloc(seeksLength);
	int keyPos = 0, seekPos=0;
	keys = fopen(keyFile,"rb");
	keyPos = (keyLength/sizeof(int));
	while(!feof(seeks))
	{
		fread((seekArray+seekPos), sizeof(int), 1, seeks);
		seekPos++;
	}
	seekPos--;

	startDiskLinearSearch(keys, keyPos, seekArray, seekPos);
	fclose(keys); fclose(seeks);
	return;
}

void startOnDiskBinarySearch(FILE* keyArray, int keyPos, int*seekArray, int seekPos)
{
	int seekCounter=0, found[seekPos]; double timer;
	gettimeofday(&startTime, NULL);
	for(seekCounter = 0; seekCounter<seekPos; seekCounter++)
	{
		int getValue = binSearchDisk(keyArray, seekArray[seekCounter],0, (seekPos-1));
		if(getValue == 1) found[seekCounter] = 1;
		else found[seekCounter] =0;
	}
	gettimeofday(&endTime, NULL);
	timer = getExactTime(startTime,endTime);

	for(seekCounter =0; seekCounter<seekPos; seekCounter++)
	{
		if(found[seekCounter] ==1) 
			printf("%12d:Yes\n", seekArray[seekCounter]);
		else
			printf("%12d:No\n", seekArray[seekCounter]);
	}
	printf("Time: %0.8f\n", timer);
}

int binSearchDisk(FILE *keyFile, int seekValue, int min, int max)
{
	int middleValue=0, readKeyValue=0;
	if(max<min) return 0;
	else
	{
		fseek(keyFile, 0 ,SEEK_SET);
		middleValue = (max+min)/2;
		fseek(keyFile, (middleValue*sizeof(int)), SEEK_SET);
		fread(&readKeyValue, sizeof(int), 1, keyFile);

		if(readKeyValue<seekValue)
		{return binSearchDisk(keyFile, seekValue, (middleValue+1), max);}
		else if(readKeyValue>seekValue)
			{return binSearchDisk(keyFile, seekValue, (min), (middleValue-1));}
		else return 1;
	}
	return 0;
}

double getExactTime(struct timeval startTime, struct timeval endTime)
{
	double timer = (((double)endTime.tv_sec+1.0e-6*endTime.tv_usec)-((double)startTime.tv_sec + 1.0e-6*startTime.tv_usec));
	return timer;
}

// Main Function
int main(int argc, char *argv[])
{
	//fw; fr;
	char *keyFileName, *seekFileName;
	//keyFileName = "keys.db"; seekFileName = "seeks.db";
	if(argc!=4) {printf("Please enter the right amount of arguments which is 3.\n"); return 0;}
	char* s; s=argv[1];
	keyFileName = argv[2];
	seekFileName = argv[3];
	if(!strcmp(s,"--mem-lin"))
		MemoryLinearSearch(keyFileName, seekFileName);
	else if(!strcmp(s,"--mem-bin"))
		MemoryBinarySearch(keyFileName, seekFileName);
	else if(!strcmp(s,"--disk-lin"))
		DiskLinearSearch(keyFileName, seekFileName);
	else if(!strcmp(s,"--disk-bin"))
		DiskBinarySearch(keyFileName, seekFileName);
	else 
		printf("\n Please specify a coorect code for the type of search \n");
	return 0;
}