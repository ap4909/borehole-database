#include <iostream>
#include "CBorehole.h"	//classes CBorehole and CLBorehole are now accessible
#include <conio.h>
#include <fstream>

using namespace std;
int option;		//variable to store the choice from the user upon first entering the program in main
char f_name[100];	//name of newly created file
CLBorehole *start=NULL, *current, *temp;	//objects to store the start of the list, and the current position
int num;	//number used in save_file and load_list to store number of data entries
char file[100];	//used in main to store file name that the user wishes to load
void sepdate(char *d, CLBorehole *lcurrent)	//function to split date entered by user into day, month and year and store in relevant variables in lcurrent
{
	char daystr[3];
	strncpy(daystr,d,2);		//copies first two characters into another string
	lcurrent->day=(int)atof(daystr);	//converts daystr to a double, and then sets it equal to variable day in lcurrent 
	

	char monthstr[3];
	strncpy(monthstr,d+3,2);
	lcurrent->month=(int)atof(monthstr);

	char yearstr[5];
	strncpy(yearstr,d+6,4);
	lcurrent->year=(int)atof(yearstr);
}
void save_file(void)		//function to save entire list to file
{
	char retry;		//variable to store users answer if sfile.fail=true
	cout<<"Enter a name for the data file:" <<endl;	//allows user to enter a name for the file e.g. a particular area of study
	cin>>f_name;		//takes input and stores it in f_name
	fstream sfile;		//stream used to save the list as a file
	sfile.open(f_name,fstream::out|fstream::binary);		//open file for writing to in binary
	
	if (sfile.fail())		//if the file cannot be opened then the user is given the option to retry
	{	
		cout<<"Can't open the file for writing. Retry?(y/n)"<<endl;
		retry=_getch();		
		if(toupper(retry)!='N')		//if the user wishes to retry saving
		{
			save_file();
		}
		else exit(0);	//if the user does not wish to retry saving, exit
	}
	current=start;	//sets current to point at start of list
	num=0;
	do		
	{
		num=num+1;		//num counts the number of data entries in list
		current=current->next;	//current set to point to the next item in the list
	}
	while(current!=NULL);		//while not at the end of the list
	sfile.write((char*)&num, sizeof(int));		//firstly writes the number of entries to the file
	current=start; 
	while(current!=NULL)		//loops through list, writing each data entry to file until it reaches the end
	{
		sfile.write(current->SID, sizeof(char)*100);
		sfile.write(current->ID, sizeof(char)*100);
		sfile.write((char*)&current->lat, sizeof(double));
		sfile.write((char*)&current->lon, sizeof(double));
		sfile.write((char*)&current->day, sizeof(int));
		sfile.write((char*)&current->month, sizeof(int));
		sfile.write((char*)&current->year, sizeof(int));
		sfile.write((char*)&current->sal, sizeof(double));
		current=current->next;
		
	}

	sfile.close();

}
void add(CLBorehole*&lstart, CLBorehole*&lcurrent)		//function that adds data to a list
{	
	char ch,save;		//ch stores whether or not the user wishes to enter another borehole reading. Save stores the input of whether theuser wishes to save the data 
	char date[11];		//stores input for date of sample reading, which is then separated into day, month, year in CLBorehole 
	cout<<"Please enter data for a water sample taken at a particular borehole\n"<<endl;
	do
	{
		lcurrent=new CLBorehole;	//allocates memory to current of for an object of type CLBorehole
		cout<<"Enter sample I.D(Should be unique)"<<endl;
		cin>>lcurrent->SID;	//sets the sample ID in current to user input
		cout<<"Enter borehole I.D:"<<endl;
		cin>>lcurrent->ID;	 
		cout<<"Enter borehole latitude(deg):"<<endl;
		cin>>lcurrent->lat;
		cout<<"Enter borehole longitude(deg):"<<endl;
		cin>>lcurrent->lon;

		cout<<"Enter the date sample was taken in format dd/mm/yyyy:"<<endl;
		cin>>date;
		sepdate(date, lcurrent);		//runs sepdate
		cout<<"Enter salinity of sample(S/m):"<<endl;
		cin>>lcurrent->sal;
		if (lstart!=NULL) lcurrent->next=lstart;		//if there is a start item in the linked list set lcurrent->next to point to it
        else lcurrent->next=NULL;		//if this is the first item in the list then set lcurrent->next to point at nothing
		lstart=lcurrent;		//current is now at beginning of list so set it to start
		cout<<"Do you want to enter data for another borehole?(y/n)"<<endl;
		cin>>ch;
	}
	while(toupper(ch)!='N'); //loop runs until the user inputs 'n'
	cout<<"Save data?(y/n)"<<endl;		//asks the user if they want to save entered data to file
	save=_getch();
	if(toupper(save)!='N') save_file();	//if the user does not input 'n' then save_file called
}
void search_borehole(void)		//function to search for a particular borehole
{
	char BID[100];		//array to store borehole name wanted to search for by user
	cout<<"Enter borehole I.D:"<<endl;		//asks user for input
	cin>>BID;		//stores input in BID
	cout<<"SAMPLE I.D."<<"  "<<"BOREHOLE I.D."<<"  "<<"LATITUDE(deg)"<<"  "<<"LONGITUDE(deg)"<<"  "<<"DATE"<<"  "<<"SALINITY(S/m)"<<endl;	//header
	while(current!=NULL)		//searches through to end of list 
	{	
		if (strcmp(BID, current->ID)==0)	//checks if the current entry was from a particular borehole 
		{      
           cout<<current->SID<<"\t     "<<current->ID<<"\t\t    "<<current->lat<<"\t\t    "<<current->lon<<"\t\t   "<<current->day<<"/"<<current->month<<"/"<<current->year<<"\t"<<current->sal<<endl;	//prints out that entry
		}
		current=current->next;	//moves to the next item in the list
	}
}
void search_year(void)		//function to search for an entry from a particular year
{
	int y;		//array to store borehole name wanted to search for by user
	cout<<"Enter the year that you want to search for(yyyy):"<<endl;	//asks user for input
	cin>>y;		//stores input in y
	cout<<"SAMPLE I.D."<<"  "<<"BOREHOLE I.D."<<"  "<<"LATITUDE(deg)"<<"  "<<"LONGITUDE(deg)"<<"  "<<"DATE"<<"  "<<"SALINITY(S/m)"<<endl;	//header 
	while(current!=NULL)		//searches through list to the end of the list
	{
		if (current->year==y)	//if the current entry is from year y that entry is printed out	
		{    
           cout<<current->SID<<"\t     "<<current->ID<<"\t\t    "<<current->lat<<"\t\t    "<<current->lon<<"\t\t   "<<current->day<<"/"<<current->month<<"/"<<current->year<<"\t"<<current->sal<<endl;
		}
		current=current->next;	//moves to next item
	}
}


void delete_data(CLBorehole*& lstart, CLBorehole *lcurrent)	//function to delete a particular entry
{
	CLBorehole *temp, *prev=NULL;		//temp used to find lcurrent's position in list; prev used to store the item before temp
	temp=lstart;	//temp points to start of the list concerned
	
	if (lstart==lcurrent) //in the case that the entry to be deleted is at the start of the list
	{
		lstart=lstart->next;	//set start of the list to the next value
		delete lcurrent;		//delete the old start
		return; //exit function
	}
	do	//do..while not at the end of the list
	{
		
		if(temp==lcurrent)
		{
			break;		//value to be deleted has been found
		}
		prev=temp;						//keeps track of the previous value in the list
		temp=temp->next;		//moves temp to point at next item
	}
	while (temp!=NULL);
	prev->next=lcurrent->next;	//sets prev->next to point at item after lcurrent
	delete lcurrent;	//deletes entry

}
void print_all(CLBorehole *lstart)                         // function to print out data to screen, along with menu options
{
	   int func; 
	   current=lstart;                          // set current to point at the relevant start
       cout<<"SAMPLE I.D."<<"  "<<"BOREHOLE I.D."<<"  "<<"LATITUDE(deg)"<<"  "<<"LONGITUDE(deg)"<<"  "<<"DATE"<<"  "<<"SALINITY(S/m)"<<endl;//header for data
	   while (current!=NULL)            // go through list and print out each entry
              {
				  cout<<current->SID<<"\t     "<<current->ID<<"\t\t    "<<current->lat<<"\t\t    "<<current->lon<<"\t\t   "<<current->day<<"/"<<current->month<<"/"<<current->year<<"\t"<<current->sal<<endl;
				  current=current->next;
              }
	   cout<<endl;
	   cout<<"***MENU***"<<endl;		//header for menu
	   cout<<"1.Search data\n\n2.Delete entry\n\n3.Add entry\n\n4.Quit"<<endl;	//shows user options
	   cin>>func;
	   current=lstart;
	
	   if(func==1)		//if the user wants to search the data
		{
			
			cout<<"Search by:\n\n"<<"1.Borehole\n\n2.Year"<<endl;	//user given the option to search by borehole or year 
			cin>>func;
			if(func==1)		//user wishes to search by borehole
			{
				search_borehole();		//calls search_borehole
				cout<<endl;
				cout<<"Press enter to return to main menu"<<endl;
				getch();
				print_all(start);		//calls print_all
			}
			if(func==2)	//user wishes to search by year
			{
				search_year();		//calls search_year
				cout<<"Press enter to return to whole database"<<endl;
				getch();
				print_all(start);
			}
	   }
	   else if (func==2)	//user wishes to delete an entry
	   {
		   char ch;		//ch will store user input when user is asked whether they want to save data
		   char SID[100];	//stores input of sample ID to be deleted
		   cout<<"Enter the unique sample I.D number"<<endl;
		   cin>>SID;
		   while(current!=NULL)	//searches through list for the sample I.D number
		   {
			   if (strcmp(SID, current->SID)==0)
			   {
				   delete_data(start, current); //calls delete_data
				   break;		//once entry is deleted, loop is broken
			   }
			   current=current->next;
		   }
		   cout<<"Do you wish to save the changes?(y/n)"<<endl;		//asks user whether they want to save the data
		   cin>>ch;
		   current=lstart;
		   if(toupper(ch)!='N') save_file();//file is saved
		   print_all(start);	//calls print_all
	   }
	   else if (func==3)//user wishes to add data 
	   {
		   add(start, current);
		   print_all(start);
	   }
	   else if(func==4)	//user wishes to exit
	   {
		   exit(0);
	   }
}



void load_list(char *d_name)//function to load the items from a file into a list
{
	fstream load_file;
	int i;		//counter that is used in for loop below
	char retry;
	load_file.open(d_name,fstream::in|fstream::binary);		//opens a file for reading in binary
	if (load_file.fail())
	{
		cout<<"The file could not be opened. Please make sure the file name is correct. Retry?(y/n)"<<endl;
		cin>>retry;
		getch();
		exit(0);
	}
	load_file.read((char*)&num,sizeof(int));	//reads the number of data entries in the file. This is then used in the loop below to read through the rest of the file
	for(i=0;i<num;i++)	//runs num times 
	{
		current=new CLBorehole;		//memory allocated to current
		load_file.read(current->SID,sizeof(char)*100);
		load_file.read(current->ID, sizeof(char)*100);
		load_file.read((char*)&current->lat, sizeof(double));
		load_file.read((char*)&current->lon, sizeof(double));
		load_file.read((char*)&current->day, sizeof(int));
		load_file.read((char*)&current->month, sizeof(int));
		load_file.read((char*)&current->year, sizeof(int));
		load_file.read((char*)&current->sal, sizeof(double));
		if (start!=NULL) current->next=start;		//if there is a start item in the linked list set current->next to point to it
		else current->next=NULL;		//if this is the first item in the list then set current->next to point at nothing
		start=current;		//current is now at beginning of list so set it to start
	
	}
	
	print_all(start);	//prints all data to screen
}

void main(void)
{
	
	cout<<"Hello and welcome to the Borehole Hydrological Information Database"<<endl;
	getch();
	cout<<"\nWhat would you like to do?(Enter 1 or 2): "<<endl;		
	cout<<"1.Create a new data file\n"<<endl;	//Asks the user if they want to create an entirely new file or load an existing one
	cout<<"2.Load existing data\n"<<endl;
	cin>>option;	
	if(option==1)
	{
		add(start, current);		//calls add and creates the list 

		print_all(start);
	}
	if(option==2) 
	{
		cout<<"Enter file name e.g. data.bin:"<<endl;
		cin>>file;
		load_list(file);
	}
}
