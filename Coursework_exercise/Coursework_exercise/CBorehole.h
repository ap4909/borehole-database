class CBorehole  //class for a borehole
{
public:
	char SID[100];	//ID for a particular sample
	char ID[100];	//ID for a borehole
	double lat;		//variable to store latitude of borehole
	double lon;		//variable to store longitude of borehole
	int day;		//day of month
	int month;		//month
	int year;		//year
	double sal;		//variable to store salinity of borehole
};

class CLBorehole:public CBorehole	//CLBorehole inherits CBorehole's variables
{
public:
	CLBorehole *next;		//Link to next borehole reading in list
};