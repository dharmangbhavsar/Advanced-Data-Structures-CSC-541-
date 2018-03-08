#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<unistd.h>
#include<limits.h>
#include<stdbool.h>
#define fr freopen("input.in","r",stdin)
#define fw freopen("output.out","w",stdout)
#define debug printf("here\n")
#define debugin cout<<"inhere\n" 
#define debugname cout<<"dharmang\n";
//struct for the primary key array
typedef struct index_S {
	int key;  /* Record's key */
	long off; /* Record's offset in file */
	}index_S;
index_S primary[10000];
//struct for the availability list array
typedef struct avail_S{ 
	int siz; /* Hole's size */ 
	long off; /* Hole's offset in file */ 
	} avail_S;
avail_S available[10000];
//to compare when best fit
int best( const void *a, const void *b)
{
	int s1 = (*(avail_S*)a).siz;
	int s2 = (*(avail_S*)b).siz;
	if(s1<s2) return -1;
	else if(s1>s2) return 1;
	else return 0;
}
//to compare when worst fit
int worst( const void *a, const void *b)
{
	int s1 = (*(avail_S*)a).siz;
	int s2 = (*(avail_S*)b).siz;
	if(s1<s2) return 1;
	else if(s1>s2) return -1;
	else return 0;
}
// to sort the primary keys
int primary_sort(const void *a, const void *b)
{
	int s1 = (*(index_S*)a).key;
	int s2 = (*(index_S*)b).key;
	if(s1<s2) return -1;
	if(s1>s2) return 1;
	else return 0;
}

//getting offsets of location in best fit and first fit
long getting_offset( int size, char* avail_list)
{
	//total size 
	size+=sizeof(int);
	//checking for a large enough hole in the array
	int new_offset = -1;
	int new_size = -1, i=0, hole_num=-1;
	for( i=0;i<10000;i++)
	{
		if(available[i].siz == 0)
		{
			break;
		}
		if(available[i].siz >=size)
		{
			new_offset = available[i].off;
			new_size = available[i].siz;
			hole_num = i;
			break;
		}
	}
	//If the hole exists
	if(hole_num!=-1)
	{
		for(i=0;i<10000;i++)
		{
			if(available[i].siz==0)
			{
				break;
			}
		}

	int ending = i-1;
	//shifting the holes as we remove one hole.
	for(i = hole_num; i<ending; i++)
	{
		available[i].siz = available[i+1].siz;
		available[i].off = available[i+1].off;
	}
	available[ending].siz = 0;
	available[ending].off = 0;

	// if hole is bigger than record, then adding the smaller hole left into the availability list.
	if(new_size > size)
	{
		available[ending].siz = new_size- size;
		available[ending].off = new_offset+size;
	}
	//sorting increasingly if the list is to be added as best fit
	if(strcmp(avail_list, "--best-fit")==0)
	{
		qsort(available, ending+1, sizeof(avail_S), best);
	}
	return new_offset;
	}
	return -1;
}

//when it is needed to print the things to the end of file
long toEOF(char * record, FILE *f)
{
	//we go to the end of the file to paste the record.
	//printf("\n %s record in is EOF \n\n", record);
	int rec_size;
	long rec_off = 0;
	fseek(f, 0, SEEK_SET);
	while(fread( &rec_size, sizeof(int),1,f)==1)
	{
		rec_off = rec_off + sizeof(int) + (rec_size)*sizeof(char);
		fseek (f, (rec_size)*sizeof(char), SEEK_CUR);
	}
	rec_size = strlen(record);
	record[strlen(record)] = '\0';
	fwrite( &rec_size, sizeof(int),1, f);
	fwrite(record, sizeof(char), rec_size, f);
	return rec_off;
}
//when add command is used
void adding(char *key, char *record, char *avail_list, FILE *f);
//when del command is used
void deleting(char *key, char *avail_list, FILE *f);
//when find command is used
void finding(char *key, FILE *f);
//when the file ends, weprint the contents of availability list and primary key array
void ending( FILE *f);

void adding(char *key, char *record, char *avail_list, FILE *f)
{
	//I have copied some code for the adding function for running accuracy
	//first-sit and best fit part only.
	//0debug;
	//printf("Inside the add thingy \n");
	//printf("%s ", key);
	//printf("%s", record);
	int siz; 
	//if the last char were not set as \0 it was causing problems
	key[strlen(key)] = '\0';
	record[strlen(record)] = '\0';
	//getting the size of the primary key array
	for(siz =0; siz<10000; siz++)
	{
		if(primary[siz].key ==0)
		{
			break;
		}
	}
	//doing a binary search on the primary array to check if the record already exists.
	int end = siz;
	int low = 0, high = siz-1;
	int mid = (low+high) / 2;
	bool flag = false;
	while(low<= high)
	{
		//debugin;
		mid = (low+high) /2;
		if(atoi(key)<primary[mid].key)
		{
			high = mid -1;
		}
		else if(atoi(key) > primary[mid].key)
		{
			low = mid +1;
		}
		else 
		{
			flag =true; break;
		}
	}
	//if it exists then we ignore the record
	if(flag == true)
	{
		//debugname;
		printf("Record with SID=%s exists\n", key);
	}
	//else we check if the type is woest or best or first fit and according to it add to the primary key and remove the hole
	//when removing the hole, worst fit will sort in descending order
	//
	else
	{
		long off = -1;
		//checking if it is worst fit
		//if worst fit we have to sort descendingly and check only the first entry of availability list thats why
		if(strcmp(avail_list, "--worst-fit") == 0)
		{
			//debugin;
			//off = worst_off(strlen(record));
			off = -1;
			int siz = strlen(record);
			siz += sizeof(int);
			int i=0;
			if(available[0].siz>=siz)
			{
				//printf("BITCHINNNNN\n");
				int new_size = available[0].siz;
				int new_off = available[0].off;
				for( i=0;i<9999;i++)
				{
					if(available[i].siz==0)
						break;
					available[i].siz = available[i+1].siz;
					available[i].off = available[i+1].off;
				}
				int end_hole = i;
				available[end_hole].off = 0;
				available[end_hole].siz = 0;
				//debugname;
				if(new_size >siz)
				{
					//debugin;
					available[end_hole].siz = new_size - siz;
					available[end_hole].off = new_off + siz;
				}
				qsort(available, end_hole+1, sizeof(avail_S), worst);
				off = new_off;
			}
			else 
				off = -1;
			//debugname;

			if(off!=-1)
			{
				//debugin; 
				siz = strlen(record);
				//record[siz] = '\0';
				fseek(f, off, SEEK_SET);
				fwrite(&siz, sizeof(int),1,f);
				fwrite(record, sizeof(char), (int)strlen(record), f);
			}
			else
				off = toEOF(record, f);
		}
		//checking for if not worst fit, so basically first fit and best fit
		else{
			//debugname;
		off = INT_MIN;
		int size = 0;
		int new_size = 0;
		int hole_num = getting_offset(strlen(record), avail_list);
		//printf("Reached Here, reached here");
		if(hole_num!=-1)
		{
			char * rec = record;
			int rec_size = strlen(record);
			rec[rec_size] = '\0';
			off = hole_num;
			fseek(f, off, SEEK_SET);
			fwrite(&rec_size, sizeof(int), 1, f);
			fwrite(rec, sizeof(char), rec_size, f);
		}
		else 
			off = toEOF(record, f);//----------
		}
		//printf("Ending ending");
		primary[end].key = atoi(key);
		primary[end].off = off;
		//sorting the primary keys according to the key
		qsort(primary, end+1, sizeof(index_S), primary_sort);
		/*if(strcmp(avail_list, "--first-fit") == 0 || strcmp(avail_list, "--best-fit"))
		{
			size = strlen(record)+sizeof(int);
			int i=0;
			for(i=0;i<10000;i++)
			{
				if(available[i].siz ==0 )
					break;
				if(available[i].siz >= size)
				{
					off = available[i].off;
					new_size = available[i].siz;
					hole_num = i;
					break;
				}
			}

			if(hole_num != INT_MIN)
			{
				for(i=0;i<10000;i++)
				{
					if(available[i].siz==0)
						break;
				}
				int end_hole = i-1;

				for(i=hole_num; i<end_hole; i++)
				{
					available[i].siz = available[i+1].siz;
					available[i].off = available[i+1].off;
				}
				available[end_hole].siz = 0;
				available[end_hole].off = 0;

				if(new_size>size)
				{
					available[end_hole-1].siz = new_size - size;
					available[end_hole-1].off = off + size;
				}

				if(strcmp(avail_list, "--best-fit") ==0)
					qsort(available, end_hole, sizeof(avail_S), best);
			}


		}
		else
		{
			long hole_num = INT_MIN;

		}*/
	}
	return;
}


//making the fuction to delete a certain record from the student.db file
//runs when student uses "del" command.

void deleting(char *key, char *avail_list, FILE *f)
{
	//debugin;
	//printf("Now in the deleting phase")
	key[strlen(key)] = '\0';
	//record[strlen(record)] = '\0';-
	//printf("%s\n", key);
	//getting the size of the primary key array
	int prim_siz;
	for(prim_siz=0;prim_siz<10000;prim_siz++)
	{
		if(primary[prim_siz].key==0)
		{
			break;
		}
	}
	int end = prim_siz;
	int low=0, high=prim_siz-1, mid=(low+high)/2;
	bool flag =false;
	//binary search to find the element in the primary key
	while(low<=high)
	{
		//printf("In binary search\t");
		mid = (low+high)/2;
		if(atoi(key) < primary[mid].key)
		{
			high = mid-1;
		}
		else if(atoi(key) > primary[mid].key)
		{
			low = mid +1;
		}
		else 
		{
			flag =true;
			break;
		}
	}
	//if it is present deleteing it
	if(flag == true)
	{
		//printf("if present\t");
		int siz;
		fseek(f, primary[mid].off, SEEK_SET);
		fread(&siz, sizeof(int), 1, f);
		//This is the total size of the hole made by record
		int total_siz = siz*sizeof(char) + sizeof(int);
		//This size hole is set as the last element of availability list
		int i=0;
		for(i=0;i<10000;i++)
		{
			if(available[i].siz==0)
			{
				break;
			}
		}
		available[i].siz = total_siz; //setting size of hole
		available[i].off = primary[mid].off; //setting offset of hole
		//sorting the availability list according to the type of 
		//availability list needed. We need to sort only if best
		//or worst fir are asked.
		if(strcmp(avail_list,"--best-fit")==0)
		{
			qsort(available, i+1, sizeof(avail_S), best);
		}
		else if(strcmp(avail_list, "--worst-fit")==0)
		{
			qsort(available, i+1, sizeof(avail_S), worst);
		}
		//As we remove the record from the file, we need to remove the record
		//from the primary array as well.
		//To do this, we shift the whole primary key from deleted part
		i = mid;
		//debugname;
		for(i = mid; i<end-1; i++)
		{
			primary[i].key = primary[i+1].key;
			primary[i].off = primary[i+1].off;
		}
		//debugin;
		primary[i].key = 0;
		primary[i].off = 0;
	}
	else
	{
		key[strlen(key)-1] = '\0';
		printf("No record with SID=%s exists\n", key);
	}
	return;
}

//-------------------------------------------------------
//Making the fuction for when the user passes "find" command
//used to find a certain key in the student database

void finding(char *key, FILE *f)
{
	//printf("Inside the finding thingy----------\n");
	key[strlen(key)] = '\0';
	//record[strlen(record)] = '\0';
	//getting the size of 
	int prim_siz;
	for(prim_siz=0;prim_siz<10000;prim_siz++)
	{
		if(primary[prim_siz].key==0)
		{
			break;
		}
	}
	//printf("Dharmang is shit\n");
	int low=0, high=prim_siz-1; int mid=(low+high)/2;
	bool flag =false;
	while(low<=high)
	{
		//debugin;
		mid = (low+high)/2;
		if(atoi(key) < primary[mid].key)
		{
			high = mid-1;
		}
		else if(atoi(key) > primary[mid].key)
		{
			low = mid +1;
		}
		else 
		{
			flag =true;
			break;
		}
	}
	//printf("DEAD DEAD DEAD DEAD\n\n\n");
	if(flag == true)
	{
		//debugname;
		int siz =0;
		fseek(f, primary[mid].off, SEEK_SET);
		fread(&siz, sizeof(int), 1, f);
		char *rec;
		rec = malloc(siz);
		fread(rec, sizeof(char), siz, f);
		rec[siz] = '\0';
		printf("%s\n",rec);
		//free(rec);
	}
	else 
	{
		//debug;
		key[strlen(key)-1] = '\0';
		printf("No record with SID=%s exists\n", key);
	}
	//printf("here we go\n");
	return;
}
//---------------------------------------------------------------------
//Making the function of when we encounter "end" in user input

void ending(FILE *f)
{
	//for ending we only have to print out the availability lists and primary keys and save them on the files
	//printf("Inside ending now\n");
	printf("Index:\n");
	int j;
	//printing the primary keys to console output
	for(j=0;j<10000;j++)
	{
		if(primary[j].key ==0)
		{
			break;
		}
		printf("key=%d: offset=%d\n",primary[j].key, primary[j].off);
	}
	//printing the primary keys in the binary file
	int i=0;
	if(j>0)
	{
		//debugname;
		FILE *ind = fopen ("index.bin", "wb+");
		for(i=0;i<j;i++)
		{
			fwrite(&primary[i].key, sizeof(int), 1, ind);
			fwrite(&primary[i].off, sizeof(long), 1, ind);
		}
		fclose(ind);
	}
	//Printing the availability list to the console output
	printf("Availability:\n");
	int total=0;
	//debugname;
	//printf("SAVE MEEEEEEEEEEEE!!!!!!!");
	for(i=0;i<10000;i++)
	{
		if(available[i].siz==0)
			break;
		else
		{
			printf("size=%d: offset=%ld\n", available[i].siz, available[i].off);
			total+=available[i].siz;
		}
	}
	//Saving the availability list in thebinary file
	printf("Number of Holes: %d\n", i);
	printf("Hole space: %d\n", total);
	if(i>0)
	{
		FILE *avai = fopen("available.bin", "wb+");
		for(j=0;j<i;j++)
		{
			fwrite(&available[j].siz, sizeof(int), 1, avai);
			fwrite(&available[j].off, sizeof(long), 1, avai);
		}
		fclose(avai);
	}
	fclose(f);
	return;
}

//---------------------------------------------------------------------
//Making the main function
int main(int argc, char *argv[])
{
	//printf("Dharlkanfsiagsshajikfnjak");
	if(argc!=3) 
	{
		printf("\nEnter the correct amount of arguments\n");
		return 0;
	}
	//debug;
	char *filename = argv[2];
	char *avail_type = argv[1];
	FILE *student_file;
	FILE *prim_file;
	FILE *avail_file;
	
	if ((student_file=fopen(filename,"r+b"))==NULL)
	{
		//printf("1st\n"); 
		student_file=fopen(filename,"w+b");
		prim_file=fopen("index.bin","w+b");
		avail_file=fopen("available.bin","w+b");

		long off;
		int key, siz_prim=0;
		//taking input from file and populating the data structure
		//debug;
		while(fread(&key, sizeof(int), 1, prim_file))
		{
			primary[siz_prim].key = key;
			fread(&off, sizeof(long), 1, prim_file);
			primary[siz_prim].off = off;
			siz_prim++;
		}
		
		//performing the same procedure for availability
		//list file

		//debug;
		//printf("Here I am\n");
		//FILE *avail_file = fopen("available.bin", "w+b");
		int siz_avail=0, siz;
		while(fread(&siz,sizeof(int),1,avail_file))
		{
			available[siz_avail].siz = siz;
			fread( &off, sizeof(long), 1, avail_file);
			available[siz_avail].off = off;
			siz_avail++;
		}
	}
	else
	{
		//printf("2nd\n");
		prim_file=fopen("index.bin","rb+");
		avail_file=fopen("available.bin","rb+");
	
		//FILE *student_file = fopen(filename, "w+b");
		//if files does not exist, create file or 
		//open the file for reading and writing
		//FILE *prim_file = fopen("index.bin", "w+b");
		long off;
		int key, siz_prim;
		int i=0; 
		siz_prim = i;
		//taking input from file and populating the data structure
		//debug;
		while(fread(&key, sizeof(int), 1, prim_file))
		{
			primary[siz_prim].key = key;
			fread(&off, sizeof(long), 1, prim_file);
			primary[siz_prim].off = off;
			siz_prim++;
		}
		
		//performing the same procedure for availability
		//list file

		//debug;
		//printf("Here I am\n");
		//FILE *avail_file = fopen("available.bin", "w+b");
		int siz_avail=0, siz;
		siz_avail = i;
		while(fread(&siz,sizeof(int),1,avail_file))
		{
			available[siz_avail].siz = siz;
			fread( &off, sizeof(long), 1, avail_file);
			available[siz_avail].off = off;
			siz_avail++;
		}
		//printf("Reached Ahead\n");
	}
	int i=0;
	//for(i=0;i<100;i++)
	//{
	//	printf("%d \t %ld",primary[i].key, primary[i].off);
	//}
	fclose(prim_file);
	fclose(avail_file);
	//Getting the input for student file
	char c[256];
	char *s, *tokens;
	while(1)
	{
		//debug;
		//printf("Now in while\n");
		//debug;
		fgets(c, sizeof(c), stdin);
		//printf("%s\n", c);
		//debug;
		//printf("%s\n", c);
		int j=0;
		//char *cop;
		//strcpy(c, cop);
		char *key, *record;
		//s = strtok(c, " \n");
		//printf("%s\n",s);
		/*while(s !=NULL)
		{
			tokens[j++] = s;
			s = strtok(NULL, " ");
		}
		char *key = tokens[1];
		char *record = tokens[2];*/
		//debug;
		//debug;
		//getting the tokens from the input
		//tokens is the command
		//key is the primary key
		//record is the record
		if(strcmp(c, "end\n")!=0)
		{
			tokens = strtok(c, " ");
			key = strtok(NULL," ");
			if(strcmp(tokens, "add")==0){
			record = strtok (NULL," ");
			record[strlen(record)-1] = '\0';}
			key[strlen(key)] = '\0';
		}
		//printf("%s\n", record);}
		//printf("%s\n", key);
		//printf("%s\n", record);
		//int i=0;
		//for( i=0;i<3;i++) 
		//	printf("%s\n", s[i]);
		//debug;
		//debug;
		if(strcmp(tokens,"add")==0)
		{
			//debug;
			adding(key, record, avail_type, student_file);
		}
		else if(strcmp(tokens,"del")==0)
		{
			//debug;
			deleting(key, avail_type, student_file);
		}
		else if(strcmp(tokens, "find")==0)
		{
			//debug;
			finding(key, student_file);
		}
		else if(strcmp(tokens, "end\n")==0)
		{
			//debug;
			ending( student_file);
			return 0;
		}
		else
		{
			//debug;
			printf("\nPlease enter a valid command\n");
			break;
		}
		//break;
		//printf("Getting control till here\n");
	}
	
	return 0;
}
