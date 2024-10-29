#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MVote.h"

//uses fgets to read text from stdin if flag is non-zero it removes the \n character from the end of the string
int read_text(char str[], int size, int flag);

//checks if string contains only numbers if yes converts it to in and returns it else return -1
int read_int(char *str);
//open file and initiate hash table
hash_table *init_table(char * filename, int bucket_entries);

int main(int argc, char *argv[])
{	
	//needs exactly 4 arguments to run
	if (argc != 5){
		printf("Mvote should be called by the following command: ./mvote -f registeredvoters -b bucketentries\n" );
		return 0;
	}


	int i;
	double j;
	hash_table *htable=NULL;
	list_node *list_head=NULL;
	void *NullPtr=NULL;
	//if was called as ./mvote -f filename -b bucketentries
	if ( ( !( strcmp(argv[1], "-f") ) && !( strcmp(argv[3], "-b" ) ) ) ){
		i=read_int(argv[4]);
		if ( i<=0){
			printf("Invalid bucket entries. Should be a integer > 0\n");
			return 0;
		}
		htable=init_table(argv[2], i);
		if( htable==NULL ){
			printf("Error: Couldnt initiate hashtable\n");
			return 0;
		}

	}
	//if was called as ./mvote -b bucketentries -f filename 
	else if ( (!( strcmp(argv[3], "-f") ) && !( strcmp(argv[1], "-b") ) ) ) {
		i=read_int(argv[2]);
		if ( i<=0){
			printf("Invalid bucket entries. Should be a integer > 0\n");
			return 0;
		}
		htable=init_table(argv[4], i);
		if( htable==NULL ){
			printf("Error: Couldnt initiate hashtable\n");
			return 0;
		}
	}
	else{
		printf("In order to run mvote you need to use the flag -b followed by < the number of entries each bucket of the hash table can hold without overlfowing > and -f followed by < a file that countains the info of voters in the form of \'<pin><space><fname><space><lname><space><zip><nl>\' >\n");
		return 0;
	}

	

	FILE *fp;
	int size=256,str_equal;
	char *command,*tmp;
	char str[size],tmp_arr[size];
	char *rest=str;
	int len;
	int pin,zip;
	char *fname,*lname;
	voter *voter_ptr;
	while(1)
	{
		printf("Mvote is running waiting for you to enter command: ");
		
		read_text(str, size, 1);
		if(strlen(str)==0)
			continue;
		//if the user just pressed enter ignore it and wait for next input

		// strok_r takes str and breaks it to 2 substrings separated by the first space. 1 goes to command and 1 to rest
		command=strtok_r(str," ", &rest);
		
		len=strlen(rest);
		str_equal=strcmp(command, "man");
		// MAN : PRINT AVAILABLE COMMANDS 
		if ((str_equal==0)&&(len==0))
		{
			printf("The available commands are the following.\n");
			printf("\t1. l <pin> \n\t2. i <pin> <lname> <fname> <zip> \n\t3. m <pin> \n\t4.bv <fileofkeys>\n\t5. v \n\t6. perc \n\t7. z <zipcode> \n\t8. o \n\t9. exit \n\t10. pt : prints the hashtable \n\t11. man\n");
		}
		//PT : print hash table
		else if( ( strcmp(command, "pt")==0 ) ){
			if((len=strlen(rest))!=0){
				printf("pt command shouldn't be followed by any flags\n");
				continue;
			}
			hashtable_print(htable);

		}
		//EXIT :STOP THE LOOP AND PRINT THE FREED MEMORY
		else if(((str_equal=strcmp(command, "exit"))==0)&&(len==0))
		{
			unsigned long int freesize=0;
			if(list_head!= NULL){
				freesize+=ll_destroy(list_head);
			}
			freesize+=hashtable_destroy(htable);
			printf("Exiting the mvote program: %ld bytes freed at the end (not counting realloc and frees during runtime).\n",freesize);
			return 0;
		}
		//L : SEARCH FOR <PIN> VOTER AND IF HE EXISTS PRINT HIS INFORMATION
		else if ((str_equal=strcmp(command,"l" ))==0) {
			//make sure that pin was entered
			if((len=strlen(rest))==0){
				printf("Error: You need to enter a pin number\n");
				continue;
			}
			//convert string to int and check if it's a valid number
			tmp=strtok_r(rest," ",&rest);
			pin=read_int(tmp);
			if(pin<=0)
			{
				printf("Error: Malformed Pin [%s]\n",tmp);
				continue;
			}
			if((len=strlen(rest))!=0){
				printf("Error: you entered too many arguments \n");
				printf("Expected form of command: l <pin>\n");
				continue;
			}
			
			printf("searching for voter with pin=%d\n",pin);
			voter_ptr=hashtable_search(htable, pin);
			if ( voter_ptr==NULL ){
				printf(" Participant %d not in cohort\n", pin);
			}
			else
			{
				printf(" %d %s %s %d %c\n", voter_ptr->pin, voter_ptr->lname, voter_ptr->fname, voter_ptr->zip, voter_ptr->voted);
			}
		}
		// I : ADD VOTER TO TABLE WITH VOTED=N . IF VOTER ALREADY EXISTS PRINT ERROR
		else if ((str_equal=strcmp(command,"i" ))==0){
			if((len=strlen(rest))==0){
				printf("Error: you need to enter <pin> <last_name> <first_name> and <zip> .\n");
				continue;
			}
			//PIN
			tmp=strtok_r(rest," ",&rest);
			pin=read_int(tmp);
			if(pin<=0){
				printf("Error: Malformed Input: Pin :[%s]\n",tmp);
				continue;
			}
			//LAST NAME
			if((len=strlen(rest))==0){
				printf("Error: you didn't enter: <last_name> <first_name> and <zip>.\n");
				continue;
			}
			lname=strtok_r(rest," ",&rest);
			//FIRST NAME
			if((len=strlen(rest))==0){
				printf("Error: you didn't enter <first_name> and <zip>.\n");
				continue;
			}
			fname=strtok_r(rest," ",&rest);
			//ZIP
			if((len=strlen(rest))==0){
				printf("Error: you didn't enter <zip>.\n");
				continue;
			}
			tmp=strtok_r(rest," ", &rest);
			zip=read_int(tmp);
			if(zip<=0){
				printf("Error: Malformed Input: Zip code\n");
				printf("In case of two names they should be a '-' between them and not a space (Giannis-Pavlos).\n");
				continue;
			}
			len=strlen(rest);
			if(len>0){
				printf("Error: too many arguments\n");
				continue;
			}

			//create the voter in the heap
			voter_ptr=malloc(sizeof(voter));
			voter_ptr->voted='N';
			voter_ptr->zip=zip;
			voter_ptr->pin=pin;
			strcpy( (voter_ptr->fname), fname );
			strcpy( (voter_ptr->lname), lname );

			//hashtable_insert returns 0 if pin already exists in the hashtable
			i=hashtable_insert(htable, voter_ptr);
			if (i == 0){
				printf(" %d already exists \n",pin);
				free(voter_ptr);
			}
			else {
				printf("Inserted %d %s %s %d %c\n", voter_ptr->pin, voter_ptr->lname, voter_ptr->fname, voter_ptr->zip, voter_ptr->voted);
			}

		}
		//M : NOTE THAT VOTER WITH PIN HAS VOTED
		else if ((str_equal=strcmp(command,"m"))==0)
		{
			if((len=strlen(rest))==0){
				printf("Error: you didnt enter a pin\n");
				continue;
			}
			pin=read_int(rest);
			if(pin<=0){
				printf("Error:Malformed Input\n");
				continue;
			}
			voter_ptr=hashtable_search(htable,pin);
			if ( voter_ptr == NULL ){
				printf(" %d does not exists \n", pin);
			}
			else if (voter_ptr->voted == 'Y') {
				printf(" %d has already voted.\n", pin);
			
			}
			else{
				voter_ptr->voted='Y';
				if( list_head==NULL){
					//first person that voted need to initiate the list
					list_head=ll_create_node(voter_ptr, NullPtr , NullPtr);
				}
				else{
					ll_add_voter_to_list(list_head, voter_ptr);
				}

				printf(" %d marked voted \n", pin);
			}
		}
		//BV: CHANGE ALL THE VOTERS THAT ARE IN FILEOFKEYS TO VOTED
		else if ((str_equal=strcmp(command, "bv"))==0)
		{
			if((len=strlen(rest))==0){
				printf("Error: you need to give a file to open\n");
				continue;
			}
			if((fp=fopen(rest,"r"))==NULL){
				perror("Error: ");
				continue;
			}
			while(1)
			{
				tmp=tmp_arr;
				if((fscanf(fp,"%s",tmp))!=1){
					break;}
				//printf("Scanned :%s\n",tmp);
				pin=read_int(tmp);
				if(pin<=0)
				{	
					printf("Malformed input: %s\n",tmp);
					break;
				}
				voter_ptr=hashtable_search(htable,pin);
				if ( voter_ptr == NULL ){
					printf(" %d does not exists \n", pin);
				}
				else{
					if (voter_ptr->voted == 'Y'){
						printf("%d has already voted.\n",voter_ptr->pin);
					}
					else{
						voter_ptr->voted='Y';
						printf(" %d marked voted \n", pin);
						if( list_head==NULL ){
							list_head=ll_create_node(voter_ptr, NULL, NULL);
						}
						else{
							ll_add_voter_to_list(list_head, voter_ptr);
						}
					}
				}

			}
			//check if error occured
			if(ferror(fp))
			{	
				printf("Error: Malformed Input\n");
				fclose(fp);
			}
			//reached end of file
			else if(feof(fp))
			{
				//printf("Reached end of file\n");
				fclose(fp);
			}
			else {
				printf("Error: Malformed Input : %s\n",tmp);
				fclose(fp);
			}	
		}
		// V: PRINT THE NUMBER OF PEOPLE THAT HAVE VOTED
		else if ((str_equal=strcmp(command, "v"))==0)
		{
			if((len=strlen(rest))!=0){
				printf("v command shouldn't be followed by any flags\n");
				continue;
			}
			if(list_head == NULL){
				printf("Voted So Far 0\n");
			}
			else{
				printf("Voted So Far %d\n",ll_total_voted(list_head));
			}

		}
		// PERC : PRINT THE VOTED/TOTAL_PEOPLE
		else if ((str_equal=strcmp(command, "perc"))==0)
		{
			if((len=strlen(rest))!=0){
				printf("perc command shouldn't be followed by any flags\n");
				continue;
			}
			if( list_head == NULL){
				printf(" Voters percent 0\n");
			}
			else{
				i=ll_total_voted(list_head);
				j=hashtable_total_voters(htable);
				j= (i/j) *100;
				
				printf("Voters percent  %lf %%\n",j);
			}

		}
		//Z: PRINT THE NUMBER AND PINS OF THOSE THAT HAVE VOTED WITH ZIP=<ZIPCODE>
		else if ((str_equal=strcmp(command,"z"))==0)
		{
			if((len=strlen(rest))==0){
				printf("Error: you didnt enter a zipcode\n");
				continue;
			}
			zip=read_int(rest);
			if(pin<=0){
				printf("Error:Malformed Input\n");
				continue;
			}
			//print number of voters and pins
			if( list_head == NULL ){
				printf("No one has voted yet.\n");
			}
			else{
				ll_print_tk_id_voters(list_head, zip);
			}
		}
		//O: "<zipcode> <num>" for every zipcode in descending order
		else if ((str_equal=strcmp(command,"o"))==0)
		{
			if((len=strlen(rest))!=0){
				printf("o command shouldn't be followed by any flags\n");
				continue;
			}
			if( list_head == NULL ){
				printf("No one has voted yet.\n");
			}
			else {
				ll_print_tk_num_voters(list_head);
			}
		}
		else {
			printf("Command not recognized write man for list of a list of the available commands and needed parameters\n");
		}
		

	}
	return 0;
}
int read_int(char *str)
{
	int ret,len,sub_len;
	len=strlen(str);
	//strspn returns the length of the substring of str that contains only '0123456789' 
	sub_len=strspn(str,"0123456789"); 
	// if the lenght of the substring is the same as the original string that means its the same.
	if(len==sub_len)
	{
		ret=atoi(str);
		return ret;
	}
	return -1;
}


int read_text(char str[], int size ,int flag)
{
	int len;
	if(fgets(str, size, stdin)==NULL)
	{
		perror("Fgets: ");
		exit(0);
	}
	len=strlen(str);
	if(len>0)
	{
		if(flag&&(str[len-1]=='\n'))
		{
			str[len-1]='\0';
			len--;
		}
	}
	else 
	{
		printf("Error: no input\n");
		exit(0);
	}
	return len;
}

//create a hashtable with 2 initial buckets that can hold buck_entries without overflowing. Then open filename and start copying voters from it to the hashtable.If at anypoint there is an error exit the program else return the adress of the table
hash_table * init_table(char * filename, int bucket_entries){
	
	FILE *fp;
	hash_table *table;

	// create a table with 2 buckets that can hold bucket_entries without overflowing
	table=hashtable_create(2, bucket_entries);
	if ( table==NULL ){
		printf("Error: couldn't create the hash table\n");
		exit(-1);
	}

	if ( (fp=fopen(filename, "r") )==NULL ){
		perror("Error: couldn't open the file. ");
		exit(-1);
	}
	voter *read_voter;
	int i,total=0;	
	//start scanning creating and adding to the table the voters
	while(1){
		read_voter=malloc(sizeof(voter));	
		read_voter->voted='N';
		if((fscanf(fp,"%d %s %s %d", &read_voter->pin, read_voter->fname, read_voter->lname, &read_voter->zip))!=4){
			free(read_voter);
			break;
		}
		i=hashtable_insert(table, read_voter);
		if (i==0){
			printf("Error %d already in cohort.\n", read_voter->pin);
			free(read_voter);
		}
		total++;
	}
	fclose(fp);
	if(total==0){
		printf("Error:Didn't read any voters from the file. The file with the voters should have the form of: PIN NAME SURNAME T.K. <nl>\n");
		exit(-1);
	}
	return table;
}



