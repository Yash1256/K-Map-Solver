/*
The following program solves a 4 varibale Karnaugh Map. Karnaugh Map was
developed by Maurice Karnaugh, at Bell labs in 1953.
A Karnaugh Map reduces boolean expression more quickly and easily, as compared
to algebraic reduction.
It uses following features to accomplish the task:
*/
#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<fstream.h>

char Ar[16][3]={" 0"," 1"," 3"," 2"," 4"," 5"," 7"," 6","12","13","15","14"," 8"," 9","11","10"};

int Kar[4][4]; //Stores The Filled K_Map!
int KarD[4][4]; //Duplicate Storage For Filled K_Map!
int Pair[4][4]; //Another Backup Array For Computation!
char Typ[4]="SOP"; //Type of K_Map (SOP/POS)!
int A; //Stores 1 for SOP and 0 for POS!

char Rexpr[100]; //Stores Reduced Expression After Extraction from Data File!

int oct8[8][8][2]; //A 3-D array that stores the address(s) of all possible Octet(s)!
int quad[24][4][2]; //A 3-D array that stores the address(s) of all possible Quad(s)!
int PairP[24][2][2]; //A 3-D array that stores the address(s) of all possible Pair(s)!
int block[16][2]; //A 2-D array to store the remaining independent block(s)!

int Qsel[10]={30,30,30,30,30,30,30,30,30,30}; //Store(s) the Quad(s) selected after computation!
int Psel[10]={30,30,30,30,30,30,30,30,30,30}; //Store(s) the Pair(s) selected after computation!
int QCount=0; //Counts total number of possible quad(s)!
int OCount=0; //Counts total number of possible octet(s)!
int PCount=0; //Counts total number of possible pair(s)!

void KarDC(); //Function declaration so that it can be used by the function(s) defined befor it.

/*
Function int Countr(int) returns the number of remaining terms after removal of quad(s) or
octet(s) or pair(s).
For different value(s) of arguement it returns the remaining terms for different duplicate array(s).
*/
int CountR(int b=1)
{
	int a=0;
	if(b==1)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				if(KarD[i][j]==A)
					a++;
			}
		}
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				if(Pair[i][j]==A)
					a++;
			}
		}
	}
	return a;
}

/*
Function Onull(int,int) removes the terms associated with the given octet passed as
an aruguement to the function from different duplicate array(s).
*/
void Onull(int a,int b=1)
{
	if(b==1)
	{
		for(int i=0;i<8;i++)
			KarD[oct8[a][i][0]][oct8[a][i][1]]=2;
			//Uses 3-D Array to Remove Required Terms!
	}
	else
	{
		for(int i=0;i<8;i++)
			Pair[oct8[a][i][0]][oct8[a][i][1]]=2;
			//Uses 3-D Array To Remove Required Terms!
	}
}

/*
Function Qnull(int,int) removes the terms associated with the given quad passed as
an aruguement to the function from different duplicate array(s).
*/
void Qnull(int a,int b=1)
{
	if(b==1)
	{
		for(int i=0;i<4;i++)
			KarD[quad[a][i][0]][quad[a][i][1]]=2;
			//Uses 3-D Array To Remove Required Terms!
	}
	else
	{
		for(int i=0;i<4;i++)
			Pair[quad[a][i][0]][quad[a][i][1]]=2;
			//Uses 3-D Array To Remove Required Terms!
	}
}

/*
Function Pnull(int,int) removes the terms associated with the given pair passed as
an aruguement to the function from different duplicate array(s).
*/
void Pnull(int a,int b=1)
{
	if(b==1)
	{
		for(int i=0;i<2;i++)
			Pair[PairP[a][i][0]][PairP[a][i][1]]=2;
	}
	else
	{
		for(int i=0;i<2;i++)
			KarD[PairP[a][i][0]][PairP[a][i][1]]=2;
	}
}

/*
Function CPair() copies the original K-Map postions and then removes the terms
associated from selected quad(s) and selected octet(s).
It Provides a way to select all possible pair(s).
*/
void CPair()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			Pair[i][j]=Kar[i][j];
	}
	//Removes The Terms Associated With Octet(s)!
	for(i=0;i<OCount;i++)
		Onull(i,0);

	//Removes The Terms Associated With Quad(s)!
	for(i=0;Qsel[i]!=30;i++)
		Qnull(Qsel[i],0);
}

/*
Function PselP() works on permutation(s) and combination(s) to select the
minimum number of pair(s) from all possible pair(s) that cover maximum number
of terms.
*/
void PselP()
{
	CPair();
	int a=0;
	if(CountR(0)!=0)
	{
		//Select One Best Pair(s)!
		for(int i=0;i<PCount;i++)
		{
			Pnull(i);
			if(i==0)
			{
				a=CountR(0);
				Psel[0]=i;
			}
			else
			{
				if(CountR(0)<a)
				{
					a=CountR(0);
					Psel[0]=i;
				}
			}
			CPair();
		}

		//Select Two Best Pair(s)!
		for(i=0;i<PCount;i++)
		{
			for(int j=i+1;j<PCount;j++)
			{
				Pnull(i);
				Pnull(j);
				if(CountR(0)<a)
				{
					a=CountR(0);
					Psel[0]=i;
					Psel[1]=j;
				}
				CPair();
			}
		}
		//Select Three Best Pair(s)!
		for(int k=0;k<PCount;k++)
		{
			for(int i=k+1;i<PCount;i++)
			{
				for(int j=i+1;j<PCount;j++)
				{
					Pnull(k);
					Pnull(i);
					Pnull(j);
					if(CountR(0)<a)
					{
						a=CountR(0);
						Psel[0]=k;
						Psel[1]=i;
						Psel[2]=j;
					}
					CPair();
				}
			}
		}
		//Select Four Best Pair(s)!
		for(int l=0;l<PCount;l++)
		{
			for(int k=l+1;k<PCount;k++)
			{
				for(int i=k+1;i<PCount;i++)
				{
					for(int j=i+1;j<PCount;j++)
					{
						Pnull(l);
						Pnull(k);
						Pnull(i);
						Pnull(j);
						if(CountR(0)<a)
						{
							a=CountR(0);
							Psel[0]=l;
							Psel[1]=k;
							Psel[2]=i;
							Psel[3]=j;
						}
						CPair();
					}
				}
			}
		}
		//Select Five Best Pair(s)!
		for(int m=0;m<PCount;m++)
		{
			for(int l=m+1;l<PCount;l++)
			{
				for(int k=l+1;k<PCount;k++)
				{
					for(int i=k+1;i<PCount;i++)
					{
						for(int j=i+1;j<PCount;j++)
						{
							Pnull(m);
							Pnull(l);
							Pnull(k);
							Pnull(i);
							Pnull(j);
							if(CountR(0)<a)
							{
								a=CountR(0);
								Psel[0]=m;
								Psel[1]=l;
								Psel[2]=k;
								Psel[3]=i;
								Psel[4]=j;
							}
							CPair();
						}
					}
				}
			}
		}
		//Select Six Best Pair(s)!
		for(int n=0;n<PCount;n++)
		{
			for(int m=n+1;m<PCount;m++)
			{
				for(int l=m+1;l<PCount;l++)
				{
					for(int k=l+1;k<PCount;k++)
					{
						for(int i=k+1;i<PCount;i++)
						{
							for(int j=i+1;j<PCount;j++)
							{
								Pnull(n);
								Pnull(m);
								Pnull(l);
								Pnull(k);
								Pnull(i);
								Pnull(j);
								if(CountR(0)<a)
								{
									a=CountR(0);
									Psel[0]=n;
									Psel[1]=m;
									Psel[2]=l;
									Psel[3]=k;
									Psel[4]=i;
									Psel[5]=j;
								}
								CPair();
							}
						}
					}
				}
			}
		}
	}
}

/*
Function SPair() selects all the possible pair(s) after removing the terms associated
with octet(s) and quad(s) and saves their respective addresses in a 3-D array PairP[24][2][2].
*/
void SPair()
{
	int a=0;
	//Fills Entire Array With -1 To Avoid Misinterpretation(s)!
	for(int i=0;i<24;i++)
	{
		for(int j=0;j<2;j++)
		{
			for(int k=0;k<2;k++)
				PairP[i][j][k]=-1;
		}
	}
	CPair();
	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(Pair[i][j]==A)
			{
				if(i==0&&Kar[3][j]==A)
				{
					PairP[a][0][0]=i;
					PairP[a][0][1]=j;
					PairP[a][1][0]=3;
					PairP[a][1][1]=j;
					a++;
				}
				else if(i==3&&Kar[0][j]==A)
				{
					PairP[a][0][0]=i;
					PairP[a][0][1]=j;
					PairP[a][1][0]=0;
					PairP[a][1][1]=j;
					a++;
				}

				else if(j==0&&Kar[i][3]==A)
				{
					PairP[a][0][0]=i;
					PairP[a][0][1]=j;
					PairP[a][1][0]=i;
					PairP[a][1][1]=3;
					a++;
				}

				else if(j==3&&Kar[i][0]==A)
				{
					PairP[a][0][0]=i;
					PairP[a][0][1]=j;
					PairP[a][1][0]=i;
					PairP[a][1][1]=0;
					a++;
				}
				else
				{
					if(Kar[i+1][j]==A&&i!=3)
					{
						PairP[a][0][0]=i;
						PairP[a][0][1]=j;
						PairP[a][1][0]=i+1;
						PairP[a][1][1]=j;
						a++;
					}
					if(Kar[i][j+1]==A&&j!=3)
					{
						PairP[a][0][0]=i;
						PairP[a][0][1]=j;
						PairP[a][1][0]=i;
						PairP[a][1][1]=j+1;
						a++;
					}
					if(Kar[i-1][j]==A&&i!=0)
					{
						PairP[a][0][0]=i;
						PairP[a][0][1]=j;
						PairP[a][1][0]=i-1;
						PairP[a][1][1]=j;
						a++;
					}
					if(Kar[i][j-1]==A&&j!=0)
					{
						PairP[a][0][0]=i;
						PairP[a][0][1]=j;
						PairP[a][1][0]=i;
						PairP[a][1][1]=j-1;
						a++;
					}
				}
			}
		}
	}
	PCount=a; //Counts Total Possible Pairs!
}

/*
Function QselP() works on permutation(s) and combination(s) to select the
minimum number of quad(s) from all possible quad(s) that cover maximum number
of terms.
*/
void QselP()
{
	KarDC();
	int a=0;
	if(CountR()!=0)
	{
		//Selecting One Best Quad(s)!
		for(int i=0;i<QCount;i++)
		{
			Qnull(i);
			if(i==0)
			{
				a=CountR();
				Qsel[0]=i;
			}
			else
			{
				if(CountR()<a)
				{
					a=CountR();
					Qsel[0]=i;
				}
			}
			KarDC();
		}
		//Selecting Two Best Quad(s)!
		for(i=0;i<QCount;i++)
		{
			for(int j=i+1;j<QCount;j++)
			{
				Qnull(i);
				Qnull(j);
				if(CountR()<a)
				{
					a=CountR();
					Qsel[0]=i;
					Qsel[1]=j;
				}
				KarDC();
			}
		}
		//Selecting Three Best Quad(s)!
		for(int k=0;k<QCount;k++)
		{
			for(int i=k+1;i<QCount;i++)
			{
				for(int j=i+1;j<QCount;j++)
				{
					Qnull(k);
					Qnull(i);
					Qnull(j);
					if(CountR()<a)
					{
						a=CountR();
						Qsel[0]=k;
						Qsel[1]=i;
						Qsel[2]=j;
					}
					KarDC();
				}
			}
		}
		//Selecting Four Best Quad(s)!
		for(int l=0;l<QCount;l++)
		{
			for(int k=l+1;k<QCount;k++)
			{
				for(int i=k+1;i<QCount;i++)
				{
					for(int j=i+1;j<QCount;j++)
					{
						Qnull(l);
						Qnull(k);
						Qnull(i);
						Qnull(j);
						if(CountR()<a)
						{
							a=CountR();
							Qsel[0]=l;
							Qsel[1]=k;
							Qsel[2]=i;
							Qsel[3]=j;
						}
						KarDC();
					}
				}
			}
		}
		//Selecting Five Best Quad(s)!
		for(int m=0;m<QCount;m++)
		{
			for(int l=m+1;l<QCount;l++)
			{
				for(int k=l+1;k<QCount;k++)
				{
					for(int i=k+1;i<QCount;i++)
					{
						for(int j=i+1;j<QCount;j++)
						{
							Qnull(m);
							Qnull(l);
							Qnull(k);
							Qnull(i);
							Qnull(j);
							if(CountR()<a)
							{
								a=CountR();
								Qsel[0]=m;
								Qsel[1]=l;
								Qsel[2]=k;
								Qsel[3]=i;
								Qsel[4]=j;
							}
							KarDC();
						}
					}
				}
			}
		}
		//Selecting Six Best Quad(s)!
		for(int n=0;n<QCount;n++)
		{
			for(int m=n+1;m<QCount;m++)
			{
				for(int l=m+1;l<QCount;l++)
				{
					for(int k=l+1;k<QCount;k++)
					{
						for(int i=k+1;i<QCount;i++)
						{
							for(int j=i+1;j<QCount;j++)
							{
								Qnull(n);
								Qnull(m);
								Qnull(l);
								Qnull(k);
								Qnull(i);
								Qnull(j);
								if(CountR()<a)
								{
									a=CountR();
									Qsel[0]=n;
									Qsel[1]=m;
									Qsel[2]=l;
									Qsel[3]=k;
									Qsel[4]=i;
									Qsel[5]=j;
								}
								KarDC();
							}
						}
					}
				}
			}
		}
	}
}

/*
Function KarDC() copies original K-Map array to duplicate KarD[4][4] array
and removes the terms associated with the octet(s).
After removal of octet terms, it becomes easier to select the best quad(s).
*/
void KarDC()
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			KarD[i][j]=Kar[i][j];
	}
	//Removes The Terms Associated With Octet(s)!
	for(i=0;i<OCount;i++)
		Onull(i);
}

/*
Function Layout() draws a basic layout required for solving a 4-variable K_Map
using function(s) of GRAPHICS.H
*/
void Layout()
{
	//Clearing Screen In Graphics Mode!
	cleardevice();
	settextstyle(SMALL_FONT,0,4);
	setcolor(15);
	int a=0;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<2;j++)
			line(218,138+50*i+j,418,138+50*i+j);

		for(j=0;j<2;j++)
			line(218+50*i+j,138,218+50*i+j,339);
	}
	for(i=0;i<3;i++)
		line(190+i,110,219+i,139);
	setcolor(2);
	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			outtextxy(256+50*j,176+50*i,Ar[a]);
			a++;
		}
	}
	setcolor(15);
	for(i=0;i<2;i++)
		rectangle(115+i,370+i,525-i,398-i);
	setcolor(2);
		rectangle(118,373,522,395);
	setcolor(12);
	outtextxy(236,355,"REDUCED EXPRESSION (SOP/POS)");
}

/*
Function line(int,int) removes/clears the previous position of fill pointer to
avoid misinterpretation between new and old fill position(s).
*/
void line(int x,int y)
{
	setcolor(0); //To cover previous position with black color!
	for(int i=0;i<3;i++)
	{
		line(224+50*x,143+50*y+i,263+50*x,143+50*y+i);
		line(224+50*x+i,143+50*y,224+50*x+i,184+50*y);
	}
}

/*
Function Selct() sets the value of Typ[4] as SOP/POS and A as 1/0 as per the user
selection and it makes visual changes to reflect the user choice (POS or SOP).
*/
void Selct()
{
	char ch='\0';
	setcolor(15);
	for(int i=0;i<3;i++)
		rectangle(260+i,40+i,380-i,80-i);
	settextstyle(SMALL_FONT,0,4);
	//To Depict The Control Key(s)!
	outtextxy(238,90,"* Press S [SOP] or P [POS] *");
	for(i=0;i<3;i++)
		line(319+i,40,319+i,80);
	settextstyle(COMPLEX_FONT,0,2);
	setcolor(4);
	outtextxy(273,44,"SOP");
	setcolor(2);
	outtextxy(331,44,"POS");
	p:
	ch=getch();
	if(toupper(ch)!='S'&&toupper(ch)!='P')
		goto p;

	setcolor(2);
	if(toupper(ch)=='S')
	{
		setcolor(0);
		setfillstyle(SOLID_FILL,0);
		bar(238,90,402,100);
		setcolor(4);
		//For Visual Change(s)!
		for(i=0;i<4;i++)
		{
			rectangle(262+i,42+i,318-i,78-i);
			if(i>1)
			{
				line(260,82+i,320,82+i);
				line(260,34+i,320,34+i);
			}
		}
		//Assigning Values!
		strcpy(Typ,"SOP");
		A=1;
	}
	if(toupper(ch)=='P')
	{
		setcolor(0);
		setfillstyle(SOLID_FILL,0);
		bar(238,90,402,100);
		setcolor(2);
		//For Visual Change(s)!
		for(i=0;i<4;i++)
		{
			rectangle(322+i,42+i,378-i,78-i);
			if(i>1)
			{
				line(320,82+i,380,82+i);
				line(320,34+i,380,34+i);
			}
		}
		//Assigning Values!
		strcpy(Typ,"POS");
		A=0;
	}

	//Depict(s) the variable(s) for each row and column for both POS and SOP!
	settextstyle(SANS_SERIF_FONT,0,1);
	//For SOP!
	if(A==1)
	{
		setcolor(4);
		for(int i=0;i<4;i++)
		{
			outtextxy(232+50*i,110,"C.D");
			outtextxy(231+50*i,100,"-");
			outtextxy(248+50*i,100,"-");
		}
		setcolor(0);
		setfillstyle(SOLID_FILL,0);
		bar(298,112,395,114);
		setcolor(4);
		for(i=0;i<4;i++)
			outtextxy(185,150+50*i,"A.B");
		outtextxy(185,140,"-");
		outtextxy(201,140,"-");
		outtextxy(185,190,"-");
		outtextxy(201,290,"-");
	}
	//For POS!
	else
	{
		setcolor(2);
		for(int i=0;i<4;i++)
			outtextxy(230+50*i,110,"C+D");
		outtextxy(304,100,"-");
		outtextxy(330,100,"-");
		outtextxy(354,100,"-");
		outtextxy(380,100,"-");
		for(i=0;i<4;i++)
			outtextxy(180,150+50*i,"A+B");
		outtextxy(204,190,"-");
		outtextxy(180,240,"-");
		outtextxy(204,240,"-");
		outtextxy(180,290,"-");
	}
}

/*
Function Instrct(int) displays the control key(s) for the current situation.
It accepts an integer parameter to represent the situation.
*/
void Instrct(int b=1)
{
	switch(b)
	{
		//While Filling The K-Map!
		case 1:
			settextstyle(SMALL_FONT,0,6);
			setcolor(6);
			outtextxy(440,140,"# Control Key(s)");
			setcolor(9);
			settextstyle(SMALL_FONT,0,5);
			outtextxy(440,170,"* ESC - Exit Program");
			outtextxy(440,190,"* ARROW(S) - Navigate");
			outtextxy(440,210,"* SPACE - Fill Position");
			outtextxy(440,230,"* DELETE - Delete Position");
			outtextxy(440,250,"* ENTER - Get Expression");
			break;

		//To Save The Calculated Expression!
		default:
			setcolor(4);
			outtextxy(440,270,"* F2 - Save Expression");
			outtextxy(440,290,"* Any Other Key To Cancel");
			break;
	}
}

/*
Function Brdr(int) draws the screen border as per the situation and fills the
content required.
It accepts an integer parameter to represent different situation(s).
*/
void Brdr(int b)
{
	delay(100);
	//To Clear Screen In Graphics Mode!
	cleardevice();
	int x=getmaxx()/8;
	int y=getmaxy()/8;
	setcolor(4);
	for(int l=0;l<3;l++)
		rectangle(x-l,y-l,7*x-l,7*y-l);
	for(l=0;l<2;l++)
	{
		for(int j=0;j<2;j++)
		{
			for(int i=0;i<5;i++)
			{
				setcolor(6);
				circle(x+(6*l*x),y+(6*j*y),22+i);
				setcolor(2);
				circle(x+(6*l*x),y+(6*j*y),14+i);
				if(i>2)
				{
					setcolor(15);
					circle(x+(6*l*x),y+(6*j*y),27+i);
					circle(x+(6*l*x),y+(6*j*y),32+i);
					circle(x+(6*l*x),y+(6*j*y),37+i);
				}
			}
		}
	}
	switch(b)
	{
		case 1:
			settextstyle(GOTHIC_FONT,0,6);
			setcolor(3);
			outtextxy(205,60,"| K-Map |");
			setcolor(15);
			settextstyle(SIMPLEX_FONT,0,2);
			outtextxy(155,350,"� Devised By :");
			setcolor(4);
			settextstyle(COMPLEX_FONT,0,2);
			outtextxy(288,350,"MANISH SHARMA");
			outtextxy(288,375,"XII Science [C.S.]");
			break;
		case 2:
			settextstyle(GOTHIC_FONT,0,6);
			setcolor(3);
			outtextxy(205,60,"| K-Map |");
			setcolor(15);
			settextstyle(SIMPLEX_FONT,0,2);
			outtextxy(155,350,"� Devised By :");
			setcolor(4);
			settextstyle(COMPLEX_FONT,0,2);
			outtextxy(288,350,"MANISH SHARMA");
			outtextxy(288,375,"XII Science [C.S.]");
			settextstyle(SIMPLEX_FONT,0,1);
			setcolor(15);
			outtextxy(155,140,"* Please Enter Your Choice !");
			outtextxy(155,170,"[�] Solve K-Map");
			outtextxy(155,195,"[�] View Saved Expression[s]");
			outtextxy(155,260,"� Press Esc To Exit");
			outtextxy(155,280,"� Press M To Know More");
			outtextxy(155,300,"� Press S To Solve");
			outtextxy(155,320,"� Press V To View");
			break;
		case 3:
			settextstyle(GOTHIC_FONT,0,6);
			setcolor(3);
			outtextxy(205,60,"| K-Map |");
			setcolor(15);
			settextstyle(SIMPLEX_FONT,0,2);
			outtextxy(155,350,"� Devised By :");
			setcolor(4);
			settextstyle(COMPLEX_FONT,0,2);
			outtextxy(288,350,"MANISH SHARMA");
			outtextxy(288,375,"XII Science [C.S.]");
			settextstyle(SANS_SERIF_FONT,0,1);
			setcolor(15);
			outtextxy(183,285,"[�] Thanks For Stopping By [�]");
			settextstyle(SMALL_FONT,0,5);
			outtextxy(100,320,"Whole-Heartedly Thanks To -");
			setcolor(14);
			outtextxy(320,320,"Mr. SHAKTIRISH KUMAR AGARWAL");
			setcolor(15);
			outtextxy(346,335,"[ PGT, Computer Science ]");
			settextstyle(SMALL_FONT,0,4);
			setcolor(2);
			outtextxy(127,418,"� For Feedback E-mail Me @");
			settextstyle(SMALL_FONT,0,5);
			setcolor(6);
			outtextxy(290,415,"theburningheart1999@gmail.com");
			setcolor(6);
			for(int i=0;i<4;i++)
			{
				setcolor(6);
				circle(getmaxx()/2,getmaxy()/2-40,55+i);
				if(i>1)
				{
					setcolor(13);
					circle(getmaxx()/2,getmaxy()/2-40,60+i);
				}
			}
			setcolor(14);
			for(i=0;i<4;i++)
				circle(getmaxx()/2-25,getmaxy()/2-55,12+i);
			for(i=0;i<4;i++)
				circle(getmaxx()/2+25,getmaxy()/2-55,12+i);
			delay(1000);
			for(i=0;i<=60;i++)
			{
				setcolor(4);
				for(int j=0;j<4;j++)
					arc(getmaxx()/2,getmaxy()/2-40,270-i,270+i,40-j);
				delay(10);
				if(i==60)
				{
					setcolor(6);
					for(j=0;j<3;j++)
						circle(getmaxx()/2-25,getmaxy()/2-55,5+j);
					for(j=0;j<3;j++)
						circle(getmaxx()/2+25,getmaxy()/2-55,5+j);
				}
			}
			delay(7000);
			break;
	}
}

/*
Function fill() accepts the position of terms(SOP/POS) and allows user to
navigate through the 16 block K-Map.
After accepting the position(s) of term(s), it fills remaining positions with
0(For SOP) and 1(For POS) and saves the entire map in 2-D array Kar[4][4].
*/
void fill()
{
	int x=0, y=0, i=0;
	char a, b;
	Instrct();
	settextstyle(GOTHIC_FONT,0,2);
	do
	{
		setcolor(4);
		for(i=0;i<3;i++)
		{
			line(224+50*x,143+50*y+i,263+50*x,143+50*y+i);
			line(224+50*x+i,143+50*y,224+50*x+i,184+50*y);
		}
	a=getch();
	// To detect arrow and delete key(s)!
	if(int(a)==0)
	{
		b=getch();
		//For deleting the filled value!
		if(int(b)==83)
		{
			setcolor(0);
			setfillstyle(SOLID_FILL,0);
			bar(238+50*x,150+50*y,248+50*x,180+50*y);
			Kar[y][x]=2;
		}
		//For Up Arrow!
		if(int(b)==72)
		{
			line(x,y);
			if(y>0)
				y--;
			for(i=0;i<3;i++)
			{
				line(224+50*x,143+50*y+i,263+50*x,143+50*y+i);
				line(224+50*x+i,143+50*y,224+50*x+i,184+50*y);
			}
		}
		//For Down Arrow!
		if(int(b)==80)
		{
			line(x,y);
			if(y<3)
				y++;
			for(i=0;i<3;i++)
			{
				line(224+50*x,143+50*y+i,263+50*x,143+50*y+i);
				line(224+50*x+i,143+50*y,224+50*x+i,184+50*y);
			}
		}
		//For Left Arrow!
		if(int(b)==75)
		{
			line(x,y);
			if(x>0)
				x--;
			for(i=0;i<3;i++)
			{
				line(224+50*x,143+50*y+i,263+50*x,143+50*y+i);
				line(224+50*x+i,143+50*y,224+50*x+i,184+50*y);
			}
		}
		//For Right Arrow!
		if(int(b)==77)
		{
			line(x,y);
			if(x<3)
				x++;
			for(i=0;i<3;i++)
			{
				line(224+50*x,143+50*y+i,263+50*x,143+50*y+i);
				line(224+50*x+i,143+50*y,224+50*x+i,184+50*y);
			}
		}
	}
	else
	{
		//To Exit Program!
		if(int(a)==27)
		{
			Brdr(3);
			cleardevice();
			closegraph();
			exit(0);
		}
		//Space Key To Fill Position!
		if(int(a)==32)
		{
			if(strcmpi(Typ,"SOP")==0)
			{
				setcolor(4);
				outtextxy(238+50*x,150+50*y,"1");
				Kar[y][x]=1;
			}
			if(strcmpi(Typ,"POS")==0)
			{
				setcolor(2);
				outtextxy(238+50*x,150+50*y,"0");
				Kar[y][x]=0;
			}
		}
	}
	}
	while(int(a)!=13);

	line(x,y);

	setcolor(9);
	//To Fill Blank Position(s)!
	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		if(Kar[i][j]==2)
		{
			if(strcmpi(Typ,"POS")==0)
			{
				outtextxy(238+50*j,150+50*i,"1");
				Kar[i][j]=1;
			}
			if(strcmpi(Typ,"SOP")==0)
			{
				outtextxy(238+50*j,150+50*i,"0");
				Kar[i][j]=0;
			}
		}
	}
}

/*
Function oct8S() checks for all possible octet(s) and stores the addresses of
the respective elements belonging to a given octet in a 3-D Array oct8[8][8][2].
*/
void oct8S()
{
	int a=0;
	if(Kar[0][0]==A&&Kar[0][1]==A&&Kar[0][2]==A&&Kar[0][3]==A&&Kar[3][0]==A&&Kar[3][1]==A&&Kar[3][2]==A&&Kar[3][3]==A)
	{
		oct8[a][0][0]=0;oct8[a][0][1]=0;
		oct8[a][1][0]=0;oct8[a][1][1]=1;
		oct8[a][2][0]=0;oct8[a][2][1]=2;
		oct8[a][3][0]=0;oct8[a][3][1]=3;
		oct8[a][4][0]=3;oct8[a][4][1]=0;
		oct8[a][5][0]=3;oct8[a][5][1]=1;
		oct8[a][6][0]=3;oct8[a][6][1]=2;
		oct8[a][7][0]=3;oct8[a][7][1]=3;
		a++;
	}
	if(Kar[0][0]==A&&Kar[1][0]==A&&Kar[2][0]==A&&Kar[3][0]==A&&Kar[0][3]==A&&Kar[1][3]==A&&Kar[2][3]==A&&Kar[3][3]==A)
	{
		oct8[a][0][0]=0;oct8[a][0][1]=0;
		oct8[a][1][0]=1;oct8[a][1][1]=0;
		oct8[a][2][0]=2;oct8[a][2][1]=0;
		oct8[a][3][0]=3;oct8[a][3][1]=0;
		oct8[a][4][0]=0;oct8[a][4][1]=3;
		oct8[a][5][0]=1;oct8[a][5][1]=3;
		oct8[a][6][0]=2;oct8[a][6][1]=3;
		oct8[a][7][0]=3;oct8[a][7][1]=3;
		a++;
	}
	for(int i=0;i<3;i++)
	{
		if(Kar[i][0]==A&&Kar[i][1]==A&&Kar[i][2]==A&&Kar[i][3]==A&&Kar[i+1][0]==A&&Kar[i+1][1]==A&&Kar[i+1][2]==A&&Kar[i+1][3]==A)
		{
			oct8[a][0][0]=i;oct8[a][0][1]=0;
			oct8[a][1][0]=i;oct8[a][1][1]=1;
			oct8[a][2][0]=i;oct8[a][2][1]=2;
			oct8[a][3][0]=i;oct8[a][3][1]=3;
			oct8[a][4][0]=i+1;oct8[a][4][1]=0;
			oct8[a][5][0]=i+1;oct8[a][5][1]=1;
			oct8[a][6][0]=i+1;oct8[a][6][1]=2;
			oct8[a][7][0]=i+1;oct8[a][7][1]=3;
			a++;
		}
	}
	for(i=0;i<3;i++)
	{
		if(Kar[0][i]==A&&Kar[1][i]==A&&Kar[2][i]==A&&Kar[3][i]==A&&Kar[0][i+1]==A&&Kar[1][i+1]==A&&Kar[2][i+1]==A&&Kar[3][i+1]==A)
		{
			oct8[a][0][0]=0;oct8[a][0][1]=i;
			oct8[a][1][0]=1;oct8[a][1][1]=i;
			oct8[a][2][0]=2;oct8[a][2][1]=i;
			oct8[a][3][0]=3;oct8[a][3][1]=i;
			oct8[a][4][0]=0;oct8[a][4][1]=i+1;
			oct8[a][5][0]=1;oct8[a][5][1]=i+1;
			oct8[a][6][0]=2;oct8[a][6][1]=i+1;
			oct8[a][7][0]=3;oct8[a][7][1]=i+1;
			a++;
		}
	}
	OCount=a;
}

/*
Function quadS() checks for all possible quad(s) and stores the addresses of
the respective elements belonging to a given quad in a 3-D Array quad[24][4][2].
*/
void quadS()
{
	int a=0;
	if(Kar[0][0]==A&&Kar[0][3]==A&&Kar[3][0]==A&&Kar[3][3]==A)
	{
		quad[a][0][0]=0;
		quad[a][0][1]=0;
		quad[a][1][0]=0;
		quad[a][1][1]=3;
		quad[a][2][0]=3;
		quad[a][2][1]=0;
		quad[a][3][0]=3;
		quad[a][3][1]=3;
		a++;
	}
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(Kar[i][j]==A&&Kar[i][j+1]==A&&Kar[i+1][j]==A&&Kar[i+1][j+1]==A)
			{
				quad[a][0][0]=i;
				quad[a][0][1]=j;
				quad[a][1][0]=i;
				quad[a][1][1]=j+1;
				quad[a][2][0]=i+1;
				quad[a][2][1]=j;
				quad[a][3][0]=i+1;
				quad[a][3][1]=j+1;
				a++;
			}
		}
	}
	for(i=0;i<4;i++)
	{
		if(Kar[0][i]==A&&Kar[1][i]==A&&Kar[2][i]==A&&Kar[3][i]==A)
		{
			quad[a][0][0]=0;
			quad[a][0][1]=i;
			quad[a][1][0]=1;
			quad[a][1][1]=i;
			quad[a][2][0]=2;
			quad[a][2][1]=i;
			quad[a][3][0]=3;
			quad[a][3][1]=i;
				a++;
		}
	}
	for(i=0;i<4;i++)
	{
		if(Kar[i][0]==A&&Kar[i][1]==A&&Kar[i][2]==A&&Kar[i][3]==A)
		{
			quad[a][0][0]=i;
			quad[a][0][1]=0;
			quad[a][1][0]=i;
			quad[a][1][1]=1;
			quad[a][2][0]=i;
			quad[a][2][1]=2;
			quad[a][3][0]=i;
			quad[a][3][1]=3;
				a++;
		}
	}
	for(i=0;i<3;i++)
	{
		if(Kar[0][i]==A&&Kar[0][i+1]==A&&Kar[3][i]==A&&Kar[3][i+1]==A)
		{
			quad[a][0][0]=0;
			quad[a][0][1]=i;
			quad[a][1][0]=0;
			quad[a][1][1]=i+1;
			quad[a][2][0]=3;
			quad[a][2][1]=i;
			quad[a][3][0]=3;
			quad[a][3][1]=i+1;
				a++;
		}
	}

	for(i=0;i<3;i++)
	{
		if(Kar[i][0]==A&&Kar[i+1][0]==A&&Kar[i][3]==A&&Kar[i+1][3]==A)
		{
			quad[a][0][0]=i;
			quad[a][0][1]=0;
			quad[a][1][0]=i+1;
			quad[a][1][1]=0;
			quad[a][2][0]=i;
			quad[a][2][1]=3;
			quad[a][3][0]=i+1;
			quad[a][3][1]=3;
				a++;
		}
	}
	QCount=a;
}

/*
Function BlockB() saves the addresses of all block(s) remaining after removal
of all the elements associated with octet(s), quad(s) and pair(s) in a 2-D
array block[16][2].
*/
void BlockB()
{
	//To Copy Original Array And Remove Octet(s), Quad(s) and Pair(s)!
	KarDC();
	int a=0;
	//Removing Quad(s)!
	for(int i=0;Qsel[i]!=30;i++)
		Qnull(Qsel[i]);
	//Removing Pair(s)!
	for(i=0;Psel[i]!=30;i++)
		Pnull(Psel[i],0);
	//Filling Entire Array With -1 To Avoid Miscue(s)!
	for(i=0;i<16;i++)
	{
		for(int j=0;j<2;j++)
			block[i][j]=-1;
	}
	//Checking For Block(s) And Saving Their Addresses!
	for(i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		if(KarD[i][j]==A)
		{
			block[a][0]=i;
			block[a][1]=j;
			a++;
		}
	}
}

/*
Function CH16() checks whether all the 16 blocks have same kind of value.
It returns the total number of blocks having same value.
If it returns 16 then the reduced expression will be 1 and if it returns 0
then the reduced expression will be 0.
*/
int CH16()
{
	int a=0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			if(Kar[i][j]==1)
				a++;
	}
	return a;
}

/*
Function Expr() generates the reduced boolean expression after marking out the
best octet(s), quad(s), pair(s) and block(s) from all possible ones.
It uses temporary data file for efficient and easy to handle procedure of expression generation.
After saving the entire expression in data file, it is extracted again in a string array and then
the temporary data file is deleted from the system.
*/
void Expr()
{
	int a=0;
	//Opening Temporary Data File In Output Mode!
	fstream F("temp.txt",ios::out);
	if(CH16()!=0&&CH16()!=16)
	{
		//All Possible Combination(s)!
		char Expr[4][4][5]=
		{
			{"0000","0001","0011","0010"},
			{"0100","0101","0111","0110"},
			{"1100","1101","1111","1110"},
			{"1000","1001","1011","1010"}
		};
		while(oct8[a][0][0]!=-1)
		{
			if(a>0&&A==1)
				F<<"+";
			//Generating The Octet Term!
			for(int i=0;i<4;i++)
			{
				if(Expr[oct8[a][0][0]][oct8[a][0][1]][i]==
				Expr[oct8[a][1][0]][oct8[a][1][1]][i]&&
				Expr[oct8[a][1][0]][oct8[a][1][1]][i]==
				Expr[oct8[a][2][0]][oct8[a][2][1]][i]&&
				Expr[oct8[a][2][0]][oct8[a][2][1]][i]==
				Expr[oct8[a][3][0]][oct8[a][3][1]][i]&&
				Expr[oct8[a][3][0]][oct8[a][3][1]][i]==
				Expr[oct8[a][4][0]][oct8[a][4][1]][i]&&
				Expr[oct8[a][4][0]][oct8[a][4][1]][i]==
				Expr[oct8[a][5][0]][oct8[a][5][1]][i]&&
				Expr[oct8[a][5][0]][oct8[a][5][1]][i]==
				Expr[oct8[a][6][0]][oct8[a][6][1]][i]&&
				Expr[oct8[a][6][0]][oct8[a][6][1]][i]==
				Expr[oct8[a][7][0]][oct8[a][7][1]][i])
				{
					if(Expr[oct8[a][0][0]][oct8[a][0][1]][i]=='1'&&A==1)
						F<<char(65+i);
					if(Expr[oct8[a][0][0]][oct8[a][0][1]][i]=='0'&&A==1)
						F<<char(65+i)<<"'";
					if(Expr[oct8[a][0][0]][oct8[a][0][1]][i]=='0'&&A==0)
						F<<char(65+i);
					if(Expr[oct8[a][0][0]][oct8[a][0][1]][i]=='1'&&A==0)
						F<<char(65+i)<<"'";
				};
			}
			a++;
		}
		a=0;
		while(Qsel[a]!=30)
		{
			if(oct8[0][0][0]!=-1&&a==0&&A==1)
				F<<"+";
			if(a>0&&A==1)
				F<<"+";
			if(A==0)
				F<<"(";
			int x=0;
			//Generating The Quad Term!
			for(int i=0;i<4;i++)
			{
				if(Expr[quad[Qsel[a]][0][0]][quad[Qsel[a]][0][1]][i]==
				Expr[quad[Qsel[a]][1][0]][quad[Qsel[a]][1][1]][i]&&
				Expr[quad[Qsel[a]][1][0]][quad[Qsel[a]][1][1]][i]==
				Expr[quad[Qsel[a]][2][0]][quad[Qsel[a]][2][1]][i]&&
				Expr[quad[Qsel[a]][2][0]][quad[Qsel[a]][2][1]][i]==
				Expr[quad[Qsel[a]][3][0]][quad[Qsel[a]][3][1]][i])
				{
					if(x>0&&A==0)
						F<<"+";
					if(Expr[quad[Qsel[a]][0][0]][quad[Qsel[a]][0][1]][i]=='1'&&A==1)
						F<<char(65+i);
					if(Expr[quad[Qsel[a]][0][0]][quad[Qsel[a]][0][1]][i]=='0'&&A==1)
						F<<char(65+i)<<"'";
					if(Expr[quad[Qsel[a]][0][0]][quad[Qsel[a]][0][1]][i]=='1'&&A==0)
						F<<char(65+i)<<"'";
					if(Expr[quad[Qsel[a]][0][0]][quad[Qsel[a]][0][1]][i]=='0'&&A==0)
						F<<char(65+i);
					x++;
				}
			}
			if(A==0)
				F<<")";
			a++;
		}
		a=0;
		while(Psel[a]!=30)
		{
			if(Qsel[0]!=30&&a==0&&A==1)
				F<<"+";
			if(a>0&&A==1)
				F<<"+";
			if(A==0)
				F<<"(";
			int x=0;
			//Generating The Pair Term!
			for(int i=0;i<4;i++)
			{
				if(Expr[PairP[Psel[a]][0][0]][PairP[Psel[a]][0][1]][i]==
				Expr[PairP[Psel[a]][1][0]][PairP[Psel[a]][1][1]][i])
				{
					if(x>0&&A==0)
						F<<"+";
					if(Expr[PairP[Psel[a]][0][0]][PairP[Psel[a]][0][1]][i]=='1'&&A==1)
						F<<char(65+i);
					if(Expr[PairP[Psel[a]][0][0]][PairP[Psel[a]][0][1]][i]=='0'&&A==1)
						F<<char(65+i)<<"'";
					if(Expr[PairP[Psel[a]][0][0]][PairP[Psel[a]][0][1]][i]=='1'&&A==0)
						F<<char(65+i)<<"'";
					if(Expr[PairP[Psel[a]][0][0]][PairP[Psel[a]][0][1]][i]=='0'&&A==0)
						F<<char(65+i);
					x++;
				}
			}
			if(A==0)
				F<<")";
			a++;
		}
		a=0;
		while(block[a][0]!=-1)
		{
			if(Psel[0]!=30&&a==0&&A==1)
				F<<"+";
			if(a>0&&A==1)
				F<<"+";
			if(A==0)
				F<<"(";
			//Generating The Block Term!
			for(int i=0;i<4;i++)
			{
				if(Expr[block[a][0]][block[a][1]][i]=='1'&&A==1)
					F<<char(65+i);
				if(Expr[block[a][0]][block[a][1]][i]=='0'&&A==1)
					F<<char(65+i)<<"'";
				if(Expr[block[a][0]][block[a][1]][i]=='1'&&A==0)
					F<<char(65+i)<<"'";
				if(Expr[block[a][0]][block[a][1]][i]=='0'&&A==0)
					F<<char(65+i);
				if(i<3&&A==0)
					F<<"+";
			}
			if(A==0)
				F<<")";
			a++;
		}
	}
	//Checks For Completely Filled Or Completely Null K-Map!
	else
	{
		if(CH16()==0)
			F<<"0";
		else
			F<<"1";
	}
	F.close();
	//Opening File Again In Input Mode To Extract Expression!
	F.open("temp.txt",ios::in);
	F.getline(Rexpr,100,' ');
	F.close();
	//Removing The Temporary Data File!
	remove("temp.txt");
	settextstyle(SMALL_FONT,0,5);
	outtextxy(126,376,Rexpr);
	Instrct(0);
}

/*
Function Essen() is an essential function that fills every element of every array
used to prevent the malfunctioning of the program and providing a reliable result.
*/
void Essen()
{
	A=2;
	strcpy(Rexpr,"\0");
	QCount=0;	PCount=0;	OCount=0;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<8;j++)
		{
			for(int k=0;k<2;k++)
				oct8[i][j][k]=-1;
		}
	}
	for(i=0;i<24;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<2;k++)
				quad[i][j][k]=-1;
		}
	}
	for(i=0;i<24;i++)
	{
		for(int j=0;j<2;j++)
		{
			for(int k=0;k<2;k++)
				PairP[i][j][k]=-1;
		}
	}
	for(i=0;i<16;i++)
	{
		for(int j=0;j<2;j++)
			block[i][j]=-1;
	}
	for(i=0;i<4;i++)
		for(int j=0;j<4;j++)
		{
			Kar[i][j]=2;
			KarD[i][j]=2;
			Pair[i][j]=2;
		}
	for(i=0;i<10;i++)
	{
		Qsel[i]=30;
		Psel[i]=30;
	}
}

//3-D Array To Store Block Numbers!
char Br[4][4][4]=
{
{"0","1","3","2"},
{"4","5","7","6"},
{"12","13","15","14"},
{"8","9","11","10"}
};

//A Class To Save And Then Retrieve The Block Of Data From The Data File!
class Kmap
{
	//Private Members Of The Class!
	private:
		char fill[100];
		char Expr[100];
	//Public Members Of The Class!
	public:
		Kmap()	//Default Constructor!
		{
			strcpy(fill,"NULL");
			strcpy(Expr,"NULL");
		}
		void Init();	//Function To Initialise The Data Members!
		void Disp(int);	//Function To Display The Values Stored!
};

//Function Definition Outside The Class!
void Kmap::Disp(int y)
{
	settextstyle(SMALL_FONT,0,5);
	setcolor(2);
	outtextxy(130,135+50*y,"F(A,B,C,D)=");
	outtextxy(130,150+50*y,"* Reduced Expression:");
	setcolor(6);
	outtextxy(215,135+50*y,fill);
	outtextxy(147,165+50*y,Expr);
}

//Function Definition Outside The Class!
void Kmap::Init()
{
	int a=0;
	strcpy(Expr,Rexpr);
	fstream F("temp1.txt",ios::out);
	F<<Typ<<'(';
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(Kar[i][j]==A)
			{
				if(a>0)
					F<<',';
				F<<Br[i][j];
				a++;
			}
		}
	}
	F<<')'<<' ';
	F.close();
	F.open("temp1.txt",ios::in);
	F.getline(fill,100,' ');
	F.close();
	remove("temp1.txt");
}

/*
Function Save() saves the generated reduced expression in a permanent data file so that
the results can be viewed later without being worried about filling the K-Map again.
It uses the object of class Kmap to save the data as a whole (block) and facilitating
the easy retrieval procedure.
*/
void Save()
{
	char a,b;
	a=getch();
	setcolor(10);
	settextstyle(SMALL_FONT,0,4);
	if(int(a)==0)
	{
		b=getch();
		if(int(b)==60)
		{
			Kmap K;
			K.Init();
			fstream F("SaveFile.dat",ios::out|ios::app|ios::binary);
			F.write((char *)&K, sizeof(K));
			F.close();
			outtextxy(260,405,"* |SAVE TRIUMPHANT| *");
			setcolor(15);
			outtextxy(204,417,"(ESC-Exit | Any Other Key For Main Menu)");
		}
	}
	else
	{
		outtextxy(275,405,"* |SAVE ABORT| *");
		setcolor(15);
		outtextxy(204,417,"(ESC-Exit | Any Other Key For Main Menu)");
	}
}

/*
Function Lscr() is used to generate the loading screen to make the program visually
appealing. It has no effect on the working of behind the screen processes.
*/
void Lscr()
{
	cleardevice();
	settextstyle(DEFAULT_FONT,0,1);
	setcolor(15);

	int x=getmaxx(), y=getmaxy(), Col=0;

	outtextxy((x/2)-27,(y/2-4),"LOADING");
	line((x/2)-27,(y/2)+6,(x/2),(y/2)+6);
	line((x/2)+27,(y/2)-8,(x/2),(y/2)-8);

	for(int i=0;i<=270;i++)
	{
		if(i%45==0)
			Col++;
		for(int j=0;j<4;j++)
		{
			setcolor(Col);
			arc(x/2,y/2,180-i,180+i,32+j);
			arc(x/2,y/2,0-i,0+i,32+j);

			setcolor(Col+1);
			arc(x/2,y/2,180-i,180+i,42+j);
			arc(x/2,y/2,0-i,0+i,42+j);

			setcolor(Col+2);
			arc(x/2,y/2,180-i,180+i,52+j);
			arc(x/2,y/2,0-i,0+i,52+j);
		}
		delay(15);
	}
	delay(100);
}

/*
Function view() facilitates viewing of saved reduced expression from the data file.
It also sets the co-ordinates and computes where to display and when to display the
data.
*/
void view()
{
	int y=0;
	char x='\0';
	Kmap K;
	//Reading Data From The Data File!
	fstream F("SaveFile.Dat",ios::in|ios::binary);
	F.seekg(0);
	cleardevice();
	Brdr(1);
	while(F.read((char*)&K,sizeof(K)))
	{
		if(y<4)
		{
			K.Disp(y);
			y++;
		}
		else
		{
			setcolor(15);
			settextstyle(SMALL_FONT,0,4);
			outtextxy(130,340,"* ESC-Exit | N-Next");
			x=getch();
			if(toupper(x)=='N')
			{
				cleardevice();
				Brdr(1);
				y=0;
				K.Disp(y);
				y++;
			}
			if(int(x)==27)
			{
				Brdr(3);
				cleardevice();
				closegraph();
				exit(0);
			}
		}
	}
	setcolor(15);
	settextstyle(SMALL_FONT,0,4);
	outtextxy(130,340,"* Press Any Key For Main Menu");
	F.close();
	getch();
}

/*
Function Prcs() is used to generate the initialising screen to make the program visually
appealing. It has no effect on the working of behind the screen processes.
*/
void Prcs()
{
	cleardevice();
	int x=getmaxx(), y=getmaxy();
	setcolor(15);
	settextstyle(SMALL_FONT,0,5);
	outtextxy(70,390,"Initialising...");

	settextstyle(SMALL_FONT,0,4);

	outtextxy(70,416,"0");
	outtextxy(77,417,"%");
	outtextxy(546,416,"100");
	outtextxy(565,417,"%");

	outtextxy(x/2-16,y/2-10,"PLEASE");
	outtextxy(x/2-10,y/2,"WAIT");

	setcolor(8);
	line(70,412,570,412);
	setcolor(15);
	delay(100);

	for(int i=1;i<=500;i++)
	{
		setcolor(2);
		for(int j=0;j<4;j++)
			line(70,409+j,70+i,409+j);

		for(j=0;j<4;j++)
		{
			setcolor(6);
			arc(x/2,y/2,10+i,80+i,30+j);
			arc(x/2,y/2,100+i,170+i,30+j);
			arc(x/2,y/2,190+i,260+i,30+j);
			arc(x/2,y/2,280	+i,350+i,30+j);

			setcolor(4);
			arc(x/2,y/2,190-i,260-i,40+j);
			arc(x/2,y/2,280-i,350-i,40+j);
			arc(x/2,y/2,100-i,170-i,40+j);
			arc(x/2,y/2,10-i,80-i,40+j);
		}

		setcolor(0);
		for(j=0;j<5;j++)
		{
			arc(x/2,y/2,-10+i,10+i,30+j);
			arc(x/2,y/2,80+i,100+i,30+j);
			arc(x/2,y/2,170+i,190+i,30+j);
			arc(x/2,y/2,260+i,280+i,30+j);
			arc(x/2,y/2,-10-i,10-i,40+j);
			arc(x/2,y/2,80-i,100-i,40+j);
			arc(x/2,y/2,170-i,190-i,40+j);
			arc(x/2,y/2,260-i,280-i,40+j);
		}
	}
}

/*
Function More() displays additional information about the program. It explains
what K-Map actually is and how it stores the position(s) of maxterm and minterm.
It also displays information about the author in a visually appealing way.
*/
void More()
{
	Brdr(1);
	settextstyle(SANS_SERIF_FONT,0,1);
	//Displaying All Necessary Information!
	setcolor(15);
	outtextxy(100,123,"K-Map,");
	settextstyle(SMALL_FONT,0,5);
	setcolor(7);
	outtextxy(163,130,"also known as Karnaugh Map is a highly efficient");
	outtextxy(100,145,"way of reducing a boolean expression. It is nothing more");
	outtextxy(100,160,"than a special form of truth table illustration. K-Map was");
	outtextxy(100,175,"developed by Maurice Karnaugh, @ Bell Labs in 1953. It is");
	outtextxy(100,190,"drawn in the form of diagram divided in rows and clomuns");
	outtextxy(100,205,"into squares and the number of square corresponds to a");
	outtextxy(100,220,"row number in the truth table.");
	outtextxy(100,235,"K-Map is composed of arrangement of cells each depicting");
	outtextxy(100,250,"one particular combination of variables in product or sum");
	outtextxy(100,265,"form. For a total number of [n] variables, K-Map consists");
	outtextxy(100,280,"of 2� cells. In K-Map a maxterm is represented by 1 while");
	outtextxy(100,295,"a minterm is represented by 0.");
	outtextxy(100,320,"Whole-Heartedly Thanks To -");
	setcolor(14);
	outtextxy(320,320,"Mr. SHAKTIRISH KUMAR AGARWAL");
	setcolor(15);
	outtextxy(346,335,"[ PGT, Computer Science ]");
	setcolor(3);
	settextstyle(SMALL_FONT,0,4);
	outtextxy(100,340,"[*] Press Any Key For Main Menu");
	getch();
}

/*
Function main() is the main function of the program and the execution
of all other functions begin from here.
It sets the steps of algorithm in a logical sequence through functions
to obtain desired results.
*/

void Gcnt(int a)
{
	char Oc[5];

	for(int i=0;oct8[i][0][0]!=-1;i++);

	if(i==20)	i=2;

	for(int j=0;Qsel[j]!=30;j++);
	for(int k=0;Psel[k]!=30;k++);
	for(int l=0;block[l][0]!=-1;l++);

	fstream F("Gtemp.txt",ios::out);
	F<<i<<" "<<j<<" "<<k<<" "<<l<<" ";
	F.close();

	settextstyle(SMALL_FONT,0,5);
	setcolor(15);

	outtextxy(35,200,"Octet(s) Count : ");
	outtextxy(39,220,"Quad(s) Count : ");
	outtextxy(41,240,"Pair(s) Count : ");
	outtextxy(33,260,"Block(s) Count : ");

	setcolor(3);
	if(a!=0)
	{
		F.open("Gtemp.txt",ios::in);

		F.getline(Oc,5,' ');
		outtextxy(155,201,Oc);

		F.getline(Oc,5,' ');
		outtextxy(155,221,Oc);

		F.getline(Oc,5,' ');
		outtextxy(155,241,Oc);

		F.getline(Oc,5,' ');
		outtextxy(155,261,Oc);

		F.close();
		remove("Gtemp.txt");
	}
}
void main()
{
	//Clearing Screen In Text Mode!
	clrscr();
	//Initialising Graphics!
	int Gdr=DETECT, Gmd;	//Detecting Graphic Driver And Graphic Mode!
	initgraph(&Gdr,&Gmd,"C:\\TurboC3\\BGI");
	char a='\0';
	Lscr();
	do
	{
		do
		{
			Brdr(2);
			a=getch();
			if(toupper(a)=='V')
			{
				Prcs();
				view();
			}
			if(toupper(a)=='M')
			{
				More();
			}
			if(int(a)==27)
			{
				Brdr(3);
				cleardevice();
				closegraph();
				exit(0);
			}
		}
		while(toupper(a)!='S');

		//Function(s) Acting As The Steps Of Problem Solving Algorithm!
		Prcs();		Essen();
		Layout();	Gcnt(0);
		Selct();
		fill();		quadS();
		oct8S();	QselP();
		SPair();	PselP();
		BlockB();	Expr();
		Gcnt(1);	Save();
		a=getch();
	}
	while(int(a)!=27);
	Brdr(3); cleardevice();	closegraph();
}