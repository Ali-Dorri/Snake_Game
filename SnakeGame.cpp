#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <ctime>

using namespace std;

struct position {
    int x;
    int y;
};


void gotoxy(int x, int y);
void sleep(unsigned int mseconds);
int MenuSelect(int OptionNum,position MenuPos,int borderSize,bool enterExist,bool &firstTime,short &newSelect, short &previousSelect,short &changeLeftRight);
void SnakeCharController(char SnakeChar[],short &changeLeftRight,short LeftRightNum[],int n,position MenuPos,bool Bot[],bool isPlayer);
void MenuManager(char SnakeChar[],bool Player[],bool Bot[]);
int MainMenu();
int NewGameMenu(int snakeLength[],int firstLength[]);
int OptionMenu(int &sceneSize,float &speed,bool &poiApplePermision,bool poiAppleExist[],int &poiAppleNum
				,bool &chargedAppleExist,bool &increasingSpeed,int &increaseSpeedRate,int firstLength[],short LeftRightNum[]);
int OptionLeftRightController(short &changeLeftRight, short LeftRightNum[],char optionString[][40],int &whatOption
								,char LeftRightString[][5][30],int Number,position MenuPos,int borderSize);
void WrittingController(position MenuPos,char optionString[][40],int &sceneSize,int &poiAppleNum,
						int &increaseSpeedRate,int firstLength[],int getEnter,int min,int max);
int ControlsMenu(char PDirection[][5],char &pauseChar,bool eachArrowKeyExist[][5]);
void DirectionCharController(char PDirection[][5],char optionString[][40],int whichPlayer, int whichDirection
								,bool eachArrowKeyExist[][5],position MenuPos, int borderSize);
int SinglePlayerMenu(char &character,bool Player[],bool Bot[]);
int SinglePlayer_vs_ComputerMenu(char SnakeChar[],bool Player[],bool Bot[]);
int MultiPlayerMenu(char SnakeChar[],bool Player[],bool Bot[]);
void FakeSnake(int firstPosX /*snake's head*/, int firstPosY /*snake's head*/, const char &character, int &currentPos /*1,2,3 or 4*/
				, int &insideCurrentPos /*0 to 6, or 0 to 4  (depends on currentPos)*/, position fakeSnake[]);
int GameScene(int sceneSize,int poiAppleNum,int increaseSpeedRate,int firstLength[],float speed,bool poiApplePermision,
				bool poiAppleExist[],bool chargedAppleExist,bool increasingSpeed,char PDirection[][5],char pauseChar,
				bool eachArrowKeyExist[][5],char SnakeChar[],bool Player[],int speedCount[],bool GameRunning,bool specialKey,
				int i,int j,int countPoiApple,int snakeLength[],char DefaultMove[],char InputP[][10],unsigned char inputKey,
				position snake[][500],position apple,position PoisonousApple[],position chargedApple,short CountP[]);


void UpdateSnake(position snake[][500],short &thisPlayer,int &snakeLength);
void RandomApple(position &apple,position PoisonousApple[],bool &poiApplePermision,int &poiAppleNum,position &chargedApple,
					bool &chargedAppleExist, const position snake[][500], int snakeLength[],bool Player[],int sceneSize);
void RandomPoisonousApple(position &apple,position PoisonousApple[],int &poiAppleNum,position &chargedApple,
					bool &chargedAppleExist, const position snake[][500], int snakeLength[],bool Player[],int sceneSize);
void ChangeHead(const char PDirection[][5],char DefaultMoveP[],char InputP[][10],short CountP[],position &SnakeHead,int thisPlayer);
void Condition(position snake[][500],short thisPlayer,int snakeLength[],bool Player[],position &apple,position PoisonousApple[],
				bool &poiApplePermision,bool poiAppleExist[],int &poiAppleNum,position &chargedApple,bool chargedAppleExist,int &sceneSize,char SnakeChar[]);



void gotoxy(int x, int y)
{
HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorCoord;
cursorCoord.X = x;
cursorCoord.Y = y;
SetConsoleCursorPosition(consoleHandle, cursorCoord);
}

void sleep(unsigned int mseconds)
{
clock_t goal = mseconds + clock();
while (goal > clock());
}

//This Initiates the fullscreen function
void fullscreen()
{
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}

void MenuManager(char SnakeChar[],bool Player[],bool Bot[]){

	int whichMenu=1,menuChoice,i,j;
	bool managerRunning=true;

	//Option neccessary variables
	static int sceneSize=100,increaseSpeedRate=0 /*1 speed per apple    for different speeds for snakes we need a count variable for each one*/;
	static float speed=0.005;
	static int firstLength[7]={5,5,5,5,5,5,5},speedCount[7]={1,1,1,1,1,1,1},poiAppleNum=0;
	static bool poiApplePermision=false,poiAppleExist[10]={false,false,false,false,false,false,false},chargedAppleExist=false,increasingSpeed=false;
	static short LeftRightNum[11]={1,1,1,1,1,1,1,1,1,1,1};

	//ControlsMenu neccessary variables
	static char PDirection[5][5],pauseChar;
	static bool eachArrowKeyExist[6][5]={{false,false,false,false,false},{false,false,false,false,false},{true,true,true,true,true},
													{false,false,false,false,false},{false,false,false,false,false}};
	eachArrowKeyExist[5][0]=false;//this is pauseArrowExist

	//game scene own variables
	static bool GameRunning=true,specialKey;
	static int countPoiApple=0,snakeLength[7]={5,5,5,5,5,5,5},gameWorth;
	static char DefaultMove[7]={'d',75,'t','k','d','d','d'},InputP[7][10];
	static unsigned char inputKey;
	static position snake[7][500],apple,PoisonousApple[9],chargedApple;
	static short CountP[7];

	//initializing
	InputP[0][0]='d';
	InputP[1][0]=75;
	InputP[2][0]='t';
	InputP[3][0]='k';
	CountP[0]=0;
	CountP[1]=0;
	CountP[2]=0;
	CountP[3]=0;

	//initializing ControlsMenu variables
	PDirection[1][1]='a'/*P1Left*/; PDirection[1][2]='d'/*P1Right*/; PDirection[1][3]='w'/*P1Up*/; PDirection[1][4]='s'/*P1Down*/;
	PDirection[2][1]=75 /*P2Left*/; PDirection[2][2]=77 /*P2Right*/; PDirection[2][3]=72 /*P2Up*/; PDirection[2][4]=80 /*P2Down*/;
	PDirection[3][1]='f'/*P3Left*/; PDirection[3][2]='h'/*P3Right*/; PDirection[3][3]='t'/*P3Up*/; PDirection[3][4]='g'/*P3Down*/;
	PDirection[4][1]='j'/*P4Left*/; PDirection[4][2]='l'/*P4Right*/; PDirection[4][3]='i'/*P4Up*/; PDirection[4][4]='k'/*P4Down*/;
	pauseChar='p';

	while(managerRunning){
		switch (whichMenu){
			case 1:
				//main menu
				menuChoice=MainMenu();
				if(menuChoice==1)   	 {whichMenu=2;}
				else if(menuChoice==2)   {whichMenu=3;}
				else if(menuChoice==3)   {whichMenu=4;}
				else if(menuChoice==4)   {whichMenu=5;}
				else if(menuChoice==5)   {whichMenu=12;}
				else if(menuChoice==-1)   {whichMenu=12;}
				break;
			case 2:
				//new game menu
				menuChoice=NewGameMenu(snakeLength,firstLength);
				if(menuChoice==1)   	 {whichMenu=6;}
				else if(menuChoice==2)   {whichMenu=7;}
				else if(menuChoice==3)   {whichMenu=8;}
				else if(menuChoice==4)   {whichMenu=1;}
				else if(menuChoice==-1)   {whichMenu=1;}
				break;
			case 3:
				//load game menu
				system("cls");
				gotoxy(70,30);
				printf("Not ready yet!");
				gotoxy(68,32);
				printf("Press any key to go back");
				getch();
				whichMenu=1;
				break;
			case 4:
				//scores archive menu
				system("cls");
				gotoxy(70,30);
				printf("Not ready yet!");
				gotoxy(68,32);
				printf("Press any key to go back");
				getch();
				whichMenu=1;
				break;
			case 5:
				//option menu
				menuChoice=OptionMenu(sceneSize,speed,poiApplePermision,poiAppleExist,poiAppleNum,chargedAppleExist,
										increasingSpeed,increaseSpeedRate,firstLength,LeftRightNum);
				if(menuChoice==1)   	 {whichMenu=10;}	//controls menu
				else if(menuChoice==2)   {whichMenu=1;}		//back to main menu
				break;

			//lower menus
			case 6:
				//single player menu
				menuChoice=SinglePlayerMenu(SnakeChar[0],Player,Bot);
				if(menuChoice==1)   	 {whichMenu=11;}	//start game
				else if(menuChoice==2)   {whichMenu=2;}		//back button to new game menu
				else if(menuChoice==-1)   {whichMenu=2;}	//back with escape to new game menu
				break;
			case 7:
				//single player vs computer menu
				menuChoice=SinglePlayer_vs_ComputerMenu(SnakeChar,Player,Bot);
				if(menuChoice==1)   	 {whichMenu=11;}	//start game
				else if(menuChoice==2)   {whichMenu=2;}		//back to new game menu
				break;
			case 8:
				//multiplayer menu
				menuChoice=MultiPlayerMenu(SnakeChar, Player, Bot);
				if(menuChoice==1)   	 {whichMenu=11;}	//start game
				else if(menuChoice==2)   {whichMenu=2;}		//back to new game menu
				break;
			case 9:
				//person's score details
				break;
			case 10:
				//controls menu
				menuChoice=ControlsMenu(PDirection,pauseChar,eachArrowKeyExist);
				if(menuChoice==2)   	 {whichMenu=5;}
				break;
			case 11:
				//game scene
				menuChoice=GameScene(sceneSize,poiAppleNum,increaseSpeedRate,firstLength,speed,poiApplePermision,
								poiAppleExist,chargedAppleExist,increasingSpeed,PDirection,pauseChar,
								eachArrowKeyExist,SnakeChar,Player,speedCount,GameRunning,specialKey,
								 i,j,countPoiApple,snakeLength,DefaultMove,InputP,inputKey,
								snake,apple,PoisonousApple,chargedApple,CountP);

				if(menuChoice==2)   	 {whichMenu=1;}
				break;
			case 12:
				//Exit
				system("cls");
				//printing the big END
				for(i=70;i<76;i++){
					gotoxy(i,30);
					printf("*");
				}
				for(i=70;i<76;i++){
					gotoxy(i,34);
					printf("*");
				}
				for(i=70;i<76;i++){
					gotoxy(i,38);
					printf("*");
				}
				for(i=30;i<39;i++){
					gotoxy(70,i);
					printf("*");
				}

				for(i=30;i<39;i++){
					gotoxy(80,i);
					printf("*");
				}
				for(i=30;i<39;i++){
					gotoxy(80+i-30,i);
					printf("*");
				}
				for(i=30;i<39;i++){
					gotoxy(88,i);
					printf("*");
				}

				for(i=30;i<39;i++){
					gotoxy(93,i);
					printf("*");
				}
				gotoxy(93+1,30);
					printf("**");
				for(i=30;i<33;i++){
					gotoxy(93+2+i-30+1,i);
					printf("*");
				}
				for(i=33;i<36;i++){
					gotoxy(93+5,i);
					printf("*");
				}
				gotoxy(93+1,38);
					printf("**");
				for(i=36;i<39;i++){
					gotoxy(93+2+3-(i-36),i);
					printf("*");
				}

				gotoxy(0,0);
				exit(0);
				break;

		}
	}
}	//End of MenuManager function

void FakeSnake(int firstPosX /*snake's head*/, int firstPosY /*snake's head*/, const char &character, int &currentPos /*1,2,3 or 4*/
				, int &insideCurrentPos /*0 to 6, or 0 to 4  (depends on currentPos)*/, position fakeSnake[]){
	int i;
	//initialize the fake snake
	if(currentPos==0){
		for(i=0;i<6;i++){
			fakeSnake[i].x=firstPosX-i;
			fakeSnake[i].y=firstPosY;
			gotoxy(fakeSnake[i].x,fakeSnake[i].y);
			printf("%c",character);
		}
		currentPos=1;
	}

	//moving the fake snake
	gotoxy(fakeSnake[5].x,fakeSnake[5].y);
	printf(" ");
	for(i=5;i>0;i--)
		fakeSnake[i]=fakeSnake[i-1];
	switch(currentPos){
		case 1:
			//fake snake in top position (insideCurrentPos between 0 to 6)
			fakeSnake[0].x+=1;
			insideCurrentPos++;
			if(insideCurrentPos>=6){
				insideCurrentPos=0;
				currentPos=2;
			}
			break;
		case 2:
			//fake snake in right position (insideCurrentPos between 0 to 4)
			fakeSnake[0].y+=1;
			insideCurrentPos++;
			if(insideCurrentPos>=4){
				insideCurrentPos=0;
				currentPos=3;
			}
			break;
		case 3:
			//fake snake in bottom position (insideCurrentPos between 0 to 6)
			fakeSnake[0].x-=1;
			insideCurrentPos++;
			if(insideCurrentPos>=6){
				insideCurrentPos=0;
				currentPos=4;
			}
			break;
		case 4:
			//fake snake in left position (insideCurrentPos between 0 to 4)
			fakeSnake[0].y-=1;
			insideCurrentPos++;
			if(insideCurrentPos>=4){
				insideCurrentPos=0;
				currentPos=1;
			}
			break;
	};
	gotoxy(fakeSnake[0].x,fakeSnake[0].y);
	printf("%c",character);
}

int MenuSelect(int OptionNum,position MenuPos,int borderSize,bool enterExist,bool &firstTime,short &newSelect, short &previousSelect,short &changeLeftRight){
	int i,j=0,k;
	unsigned char decide=0;
	const char Escape=27;
	const char ReturnEnter=13;

	//the first border(just for one time)
	if(firstTime==true){
		for(i=MenuPos.x-borderSize/2;i<MenuPos.x+borderSize/2+1;i++){
			gotoxy(i,MenuPos.y-1+j);
			printf("-");
		}
		for(i=MenuPos.x-borderSize/2;i<MenuPos.x+borderSize/2+1;i++){
			gotoxy(i,MenuPos.y+1+j);
			printf("-");
		}
		gotoxy(MenuPos.x-borderSize/2,MenuPos.y+j);
		printf("|");
		gotoxy(MenuPos.x+borderSize/2,MenuPos.y+j);
		printf("|");
		gotoxy(0,MenuPos.y+j);
		firstTime=false;
	}

	while(decide!=Escape){
	  if(kbhit()){

		//get the player choice
		decide=getch();
		//special key is pressed
		if(decide==224){
   			decide=getch();
   			if(decide==72){
   				//Up arrow was pressed!
   				if(newSelect!=1)  {newSelect--;}
   				else           {newSelect=OptionNum;}

   			}
   			if(decide==80){
   				//Down arrow was pressed!
   				if(newSelect!=OptionNum)  {newSelect++;}
   				else           {newSelect=1;}
   			}
   			if(decide==75){
   				//left arrow was pressed!
   				changeLeftRight=-1;
   			}
   			if(decide==77){
   				//right arrow was pressed!
   				changeLeftRight=1;
   			}

		 //change border
		 for(j=0;j<OptionNum*2-1;j+=2){
			if(newSelect==j/2+1){
				//erase previous border
				for(k=0;k<OptionNum*2-1;k+=2){
					if(previousSelect==k/2+1){
						for(i=MenuPos.x-borderSize/2;i<MenuPos.x+borderSize/2+1;i++){
							gotoxy(i,MenuPos.y-1+k);
							printf(" ");
						}
						for(i=MenuPos.x-borderSize/2;i<MenuPos.x+borderSize/2+1;i++){
							gotoxy(i,MenuPos.y+1+k);
							printf(" ");
						}
						gotoxy(MenuPos.x-borderSize/2,MenuPos.y+k);
						printf(" ");
						gotoxy(MenuPos.x+borderSize/2,MenuPos.y+k);
						printf(" ");
					}
				}
				//print new border
				for(i=MenuPos.x-borderSize/2;i<MenuPos.x+borderSize/2+1;i++){
					gotoxy(i,MenuPos.y-1+j);
					printf("-");
				}
				for(i=MenuPos.x-borderSize/2;i<MenuPos.x+borderSize/2+1;i++){
					gotoxy(i,MenuPos.y+1+j);
					printf("-");
				}
				gotoxy(MenuPos.x-borderSize/2,MenuPos.y+j);
				printf("|");
				gotoxy(MenuPos.x+borderSize/2,MenuPos.y+j);
				printf("|");
				//set previous border the new border (new-->previous)
				previousSelect=newSelect;
				gotoxy(0,MenuPos.y+j);
			}
		 }

   		}
   		else if(decide==ReturnEnter && enterExist==true){
   			return (newSelect);
		}
		else if(decide==ReturnEnter && enterExist==false){
			return (-2);
	    }
	  }

	  //this is for when enterExist is false
	  if(decide==Escape){
	  	return(-1);
	  }


	  if(enterExist==false){
			return(newSelect);
		}
	}
	return(-1);
}   //end of MenuSelect

void SnakeCharController(char SnakeChar[],short &changeLeftRight,short LeftRightNum[],int n,position MenuPos,bool Bot[],bool isPlayer){
	//whichSnakeChar==n
	//m for LeftRightNum[m], MenuPos.y+2*m
	short permanentLeftRightNum,m,snakeCharNum[4];
	char permanentCharacter;

	if(n==0 /*Player1*/)							{m=1;}
	else if(n==4 /*Bot1*/ || n==1 /*Player2*/)		{m=2;}
	else if(n==5 /*Bot2*/ || n==2 /*Player3*/)		{m=3;}
	else if(n==6 /*Bot3*/ || n==3 /*Player4*/)		{m=4;}

	if(isPlayer){
		snakeCharNum[0]=0;
		snakeCharNum[1]=1;
		snakeCharNum[2]=2;
		snakeCharNum[3]=3;
	}
	else{
		snakeCharNum[0]=0;
		snakeCharNum[1]=4;
		snakeCharNum[2]=5;
		snakeCharNum[3]=6;
	}

	permanentCharacter=SnakeChar[n];
	permanentLeftRightNum=LeftRightNum[m];

	if(changeLeftRight==1){
		if(LeftRightNum[m]==4)		{LeftRightNum[m]=1;}
		else						{LeftRightNum[m]++;}
	}
	if(changeLeftRight==-1){
		if(LeftRightNum[m]==1)		{LeftRightNum[m]=4;}
		else						{LeftRightNum[m]--;}
	}


	if(changeLeftRight==1 || changeLeftRight==-1){
		if(LeftRightNum[m]==1)			{SnakeChar[n]='*'; gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2*m); printf("*");}
		else if(LeftRightNum[m]==2)		{SnakeChar[n]='#'; gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2*m); printf("#");}
		else if(LeftRightNum[m]==3)		{SnakeChar[n]='0'; gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2*m); printf("0");}
		else if(LeftRightNum[m]==4)		{SnakeChar[n]='O'; gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2*m); printf("O");}

		if(SnakeChar[n]==SnakeChar[snakeCharNum[0]] && n!=snakeCharNum[0]){
			SnakeChar[snakeCharNum[0]]=permanentCharacter;
			LeftRightNum[1]=permanentLeftRightNum;
				gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2); printf("%c",SnakeChar[snakeCharNum[0]]);
		}
		else if(SnakeChar[n]==SnakeChar[snakeCharNum[1]] && n!=snakeCharNum[1]){
			SnakeChar[snakeCharNum[1]]=permanentCharacter;
			LeftRightNum[2]=permanentLeftRightNum;
			if(Bot[1]){
				gotoxy(MenuPos.x-16+strlen("Bot1 character: "),MenuPos.y+4); printf("%c",SnakeChar[snakeCharNum[1]]);
			}
		}
		else if(SnakeChar[n]==SnakeChar[snakeCharNum[2]] && n!=snakeCharNum[2]){
			SnakeChar[snakeCharNum[2]]=permanentCharacter;
			LeftRightNum[3]=permanentLeftRightNum;
			if(Bot[2]){
				gotoxy(MenuPos.x-16+strlen("Bot2 character: "),MenuPos.y+6); printf("%c",SnakeChar[snakeCharNum[2]]);
			}
		}
		else if(SnakeChar[n]==SnakeChar[snakeCharNum[3]] && n!=snakeCharNum[3]){
			SnakeChar[snakeCharNum[3]]=permanentCharacter;
			LeftRightNum[4]=permanentLeftRightNum;
			if(Bot[3]){
				gotoxy(MenuPos.x-16+strlen("Bot2 character: "),MenuPos.y+8); printf("%c",SnakeChar[snakeCharNum[3]]);
			}
		}
		changeLeftRight=0;
		gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2*m);
	}
}  //end of SnakeCharController function

int MainMenu(){
	const position mainMenuPos={75,25};
	const int mainMenuBorderSize=60, OptionNum=5;
	short newSelect=1,previousSelect=1,changeLeftRight=0;
	bool firstTime=true;

	system("cls");
	gotoxy(mainMenuPos.x-strlen("New Game")/2,mainMenuPos.y);
	printf("New Game");
	gotoxy(mainMenuPos.x-strlen("Load Game")/2,mainMenuPos.y+2);
	printf("Load Game");
	gotoxy(mainMenuPos.x-strlen("Scores Archive")/2,mainMenuPos.y+4);
	printf("Scores Archive");
	gotoxy(mainMenuPos.x-strlen("Options")/2,mainMenuPos.y+6);
	printf("Options");
	gotoxy(mainMenuPos.x-strlen("Exit")/2,mainMenuPos.y+8);
	printf("Exit");
    return (MenuSelect(OptionNum,mainMenuPos,mainMenuBorderSize,true,firstTime,newSelect,previousSelect,changeLeftRight));
}

int NewGameMenu(int snakeLength[],int firstLength[]){
	const position mainMenuPos={75,25};
	const int mainMenuBorderSize=60, OptionNum=4;
	short newSelect=1,previousSelect=1,changeLeftRight=0;
	bool firstTime=true;
	int i;

	//initializing game requirements
	for(i=0;i<7;i++){
		snakeLength[i]=firstLength[i];
	}


	system("cls");
	gotoxy(mainMenuPos.x-strlen("Single Player")/2,mainMenuPos.y);
	printf("Single Player");
	gotoxy(mainMenuPos.x-strlen("Single Player vs Computer")/2,mainMenuPos.y+2);
	printf("Single Player vs Computer");
	gotoxy(mainMenuPos.x-strlen("MultiPlayer")/2,mainMenuPos.y+4);
	printf("MultiPlayer");
	gotoxy(mainMenuPos.x-strlen("Back")/2,mainMenuPos.y+6);
	printf("Back");
    return (MenuSelect(OptionNum,mainMenuPos,mainMenuBorderSize,true,firstTime,newSelect,previousSelect,changeLeftRight));
}

int SinglePlayerMenu(char &character, bool Player[],bool Bot[]){

	character='*';
	unsigned char choice;
	const char Escape=27, ReturnEnter=13;
	const int back=2, startGame=1;
	const position MenuPos={75,25};
	int whatCharacter=1,i,firstPosX=MenuPos.x+3,firstPosY=MenuPos.y+20,currentPos=0,insideCurrentPos=6;
	position fakeSnake[6];

	system("cls");
	gotoxy(MenuPos.x-strlen("Your character: ")/2,MenuPos.y);
	printf("Your character: ");
	gotoxy(MenuPos.x-strlen("Press enter to start the game!")/2,MenuPos.y+10);
	printf("Press enter to start the game!");
	gotoxy(MenuPos.x+strlen("Your character: ")/2,MenuPos.y);
	printf("*");


	FakeSnake(firstPosX,firstPosY,character,currentPos,insideCurrentPos,fakeSnake);

	Player[0]=true; Player[1]=false; Player[2]=false; Player[3]=false; Bot[1]=false; Bot[2]=false; Bot[3]=false;
	do{
		FakeSnake(firstPosX,firstPosY,character,currentPos,insideCurrentPos,fakeSnake);
		sleep(100);
		if(kbhit()){

		choice=getch();
		if(choice==224){
			choice=getch();
			if(choice==77){
				//right arrow was pressed
				if(whatCharacter==4)	{whatCharacter=1;}
				else					{whatCharacter++;}
			}
			else if(choice==75){
				//left arrow was pressed
				if(whatCharacter==1)	{whatCharacter=4;}
				else					{whatCharacter--;}
			}
			if(whatCharacter==1)		{gotoxy(MenuPos.x+strlen("Your character: ")/2,MenuPos.y);character='*'; printf("%c",character);}
			else if(whatCharacter==2)	{gotoxy(MenuPos.x+strlen("Your character: ")/2,MenuPos.y);character='#'; printf("%c",character);}
			else if(whatCharacter==3)	{gotoxy(MenuPos.x+strlen("Your character: ")/2,MenuPos.y);character='0'; printf("%c",character);}
			else if(whatCharacter==4)	{gotoxy(MenuPos.x+strlen("Your character: ")/2,MenuPos.y);character='O'; printf("%c",character);}
		}
		else if(choice==ReturnEnter){
			return (startGame);
		}

		}
	}while(choice!=Escape);
	//back to new game menu
	return (back);
}

int SinglePlayer_vs_ComputerMenu(char SnakeChar[],bool Player[],bool Bot[]){

	SnakeChar[0]='*';	SnakeChar[4]='#';	SnakeChar[5]='0';	SnakeChar[6]='O';
	const int back=2, startGame=1;

	//MenuSelect neccessary variables
	const position MenuPos={60,25};
	int borderSize=40,optionNum=5,whatOption=1;
	short newSelect=1,previousSelect=1,changeLeftRight=0;
	bool firstTime=true;

	//FakeSnake neccessary variables
	int firstPosX=MenuPos.x+3,firstPosY=MenuPos.y+20,currentPos[4]={0},insideCurrentPos[4]={6,6,6,6};
	position fakeSnake[4][6];
	bool fakeSnakeExist[4]={true,true,false,false};

	int i;
	short getEnter=0,caseNum[8],LeftRightNum[5]={1,1,2,3,4};
	char permanentCharacter;

	system("cls");
	gotoxy(MenuPos.x-16,MenuPos.y);
	printf("Number of Bots: ");
	gotoxy(MenuPos.x-16+strlen("Number of Bots: "),MenuPos.y);
	printf("1");
	gotoxy(MenuPos.x-16,MenuPos.y+2);
	printf("Your character: ");
	gotoxy(MenuPos.x-16+strlen("Your character: "),MenuPos.y+2);
	printf("*");
	gotoxy(MenuPos.x-16,MenuPos.y+4);
	printf("Bot1 character: ");
	gotoxy(MenuPos.x-16+strlen("Bot1 character: "),MenuPos.y+4);
	printf("#");
	gotoxy(MenuPos.x-1,MenuPos.y+6);
	printf("Start");
	gotoxy(MenuPos.x-1,MenuPos.y+8);
	printf("Back");

	//initializing LeftRightNums
	LeftRightNum[0]=1;
	for(i=1;i<5;i++){
		LeftRightNum[i]=i;
	}

	Bot[1]=true;	Bot[2]=false;	Bot[3]=false;	Player[0]=true;	Player[1]=false;	Player[2]=false;	Player[3]=false;
	caseNum[0]=0;caseNum[4]=0;caseNum[5]=0;caseNum[6]=4;caseNum[7]=5;

	while(whatOption!=-1){
		whatOption=MenuSelect(optionNum,MenuPos,borderSize,false,firstTime,newSelect,previousSelect,changeLeftRight);

		if(whatOption==1){
			//number of bots
			getEnter=0;
			if(changeLeftRight==1){
				if(LeftRightNum[0]==3)		{LeftRightNum[0]=1;}
				else						{LeftRightNum[0]++;}
			}
			else if(changeLeftRight==-1){
				if(LeftRightNum[0]==1)		{LeftRightNum[0]=3;}
				else						{LeftRightNum[0]--;}
			}
			if(changeLeftRight==1 || changeLeftRight==-1){
				if(LeftRightNum[0]==1){
					Bot[1]=true;Bot[2]=false;Bot[3]=false;optionNum=5;gotoxy(MenuPos.x-16+strlen("Number of Bots: "),MenuPos.y);printf("1");
					gotoxy(MenuPos.x-16,MenuPos.y+6);printf("                 ");gotoxy(MenuPos.x-16,MenuPos.y+8);printf("                 ");
					caseNum[4]=0;caseNum[5]=0;caseNum[6]=4;caseNum[7]=5;
					fakeSnakeExist[2]=false;fakeSnakeExist[3]=false;

					//fakeSnake appearance controlling
					for(i=0;i<6;i++){
						//erase  fakeSnake[2] and fakeSnake[3]
						gotoxy(fakeSnake[2][i].x,fakeSnake[2][i].y);
						printf(" ");
						gotoxy(fakeSnake[3][i].x,fakeSnake[3][i].y);
						printf(" ");
					}

					//start and back button appearance controlling
					gotoxy(MenuPos.x-1+4,MenuPos.y+8);
					printf(" ");
					gotoxy(MenuPos.x-1,MenuPos.y+10);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+12);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+6);
					printf("Start");
					gotoxy(MenuPos.x-1,MenuPos.y+8);
					printf("Back");
				}
				else if(LeftRightNum[0]==2){
					Bot[1]=true;Bot[2]=true;Bot[3]=false;optionNum=6;gotoxy(MenuPos.x-16+strlen("Number of Bots: "),MenuPos.y);printf("2");
					gotoxy(MenuPos.x-16,MenuPos.y+6);printf("Bot2 character: %c",SnakeChar[5]);gotoxy(MenuPos.x-16,MenuPos.y+8);printf("                 ");
					caseNum[4]=4;caseNum[5]=0;caseNum[6]=5;caseNum[7]=6;
					fakeSnakeExist[2]=true;fakeSnakeExist[3]=false;
					for(i=0;i<6;i++){
						//erase  fakeSnake[3]
						gotoxy(fakeSnake[3][i].x,fakeSnake[3][i].y);
						printf(" ");
					}

					//start and back button appearance controlling
					gotoxy(MenuPos.x-1,MenuPos.y+6);
					printf("        ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+8);
					printf(" ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+10);
					printf(" ");
					gotoxy(MenuPos.x-1,MenuPos.y+12);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+8);
					printf("Start");
					gotoxy(MenuPos.x-1,MenuPos.y+10);
					printf("Back");
					gotoxy(MenuPos.x,MenuPos.y+6);
					printf("%c",SnakeChar[5]);

				}
				else if(LeftRightNum[0]==3){
					Bot[1]=true;Bot[2]=true;Bot[3]=true;optionNum=7;gotoxy(MenuPos.x-16+strlen("Number of Bots: "),MenuPos.y);printf("3");
					gotoxy(MenuPos.x-16,MenuPos.y+6);printf("Bot2 character: %c",SnakeChar[5]);gotoxy(MenuPos.x-16,MenuPos.y+8);printf("Bot3 character: %c",SnakeChar[6]);
					caseNum[4]=4;caseNum[5]=5;caseNum[6]=6;caseNum[7]=7;
					fakeSnakeExist[2]=true;fakeSnakeExist[3]=true;

					//start and back button appearance controlling
					gotoxy(MenuPos.x-1,MenuPos.y+6);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+8);
					printf("        ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+10);
					printf(" ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+12);
					printf(" ");
					gotoxy(MenuPos.x-1,MenuPos.y+10);
					printf("Start");
					gotoxy(MenuPos.x-1,MenuPos.y+12);
					printf("Back");
					gotoxy(MenuPos.x,MenuPos.y+6);
					printf("%c",SnakeChar[5]);
					gotoxy(MenuPos.x,MenuPos.y+8);
					printf("%c",SnakeChar[6]);
				}

				changeLeftRight=0;
			}
		}
		else if(whatOption==2){
			//your character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,0 /*SnakeChar[0]*/, MenuPos, Bot,false);
		}
		else if(whatOption==3){
			//bot1 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,4 /*SnakeChar[4]*/, MenuPos, Bot,false);
		}
		else if(whatOption==caseNum[4]){
			//bot2 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,5 /*SnakeChar[5]*/, MenuPos, Bot,false);
		}
		else if(whatOption==caseNum[5]){
			//bot3 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,6 /*SnakeChar[6]*/, MenuPos, Bot,false);
		}
		else if(whatOption==caseNum[6]){
			//start game
			getEnter=1;
		}
		else if(whatOption==caseNum[7]){
			//back
			getEnter=2;
		}
		else if(whatOption==-2){
			if(getEnter==1){
				return(startGame);
			}
			else if(getEnter==2){
				return(back);
			}
		}

		FakeSnake(MenuPos.x+60, MenuPos.y-5, SnakeChar[0], currentPos[0], insideCurrentPos[0], fakeSnake[0]);
		FakeSnake(MenuPos.x+60, MenuPos.y+1, SnakeChar[4], currentPos[1], insideCurrentPos[1], fakeSnake[1]);
		if(fakeSnakeExist[2])
			FakeSnake(MenuPos.x+60, MenuPos.y+7, SnakeChar[5], currentPos[2], insideCurrentPos[2], fakeSnake[2]);
		if(fakeSnakeExist[3])
			FakeSnake(MenuPos.x+60, MenuPos.y+13, SnakeChar[6], currentPos[3], insideCurrentPos[3], fakeSnake[3]);

		sleep(100);
	}
	return(back);
}  //end of SinglePlayer_vs_ComputerMenu function

int MultiPlayerMenu(char SnakeChar[],bool Player[],bool Bot[]){

	SnakeChar[0]='*';	SnakeChar[1]='#';	SnakeChar[2]='0';	SnakeChar[3]='O';
	const int back=2, startGame=1;

	//MenuSelect neccessary variables
	const position MenuPos={60,25};
	int borderSize=40,optionNum=5,whatOption=1;
	short newSelect=1,previousSelect=1,changeLeftRight=0;
	bool firstTime=true;

	//FakeSnake neccessary variables
	int firstPosX=MenuPos.x+3,firstPosY=MenuPos.y+20,currentPos[4]={0},insideCurrentPos[4]={6,6,6,6};
	position fakeSnake[4][6];
	bool fakeSnakeExist[4]={true,true,false,false};

	int i;
	short getEnter=0,caseNum[8],LeftRightNum[5]={1,1,2,3,4};
	char permanentCharacter;

	system("cls");
	gotoxy(MenuPos.x-19,MenuPos.y);
	printf("Number of Players: ");
	gotoxy(MenuPos.x,MenuPos.y);
	printf("1");
	gotoxy(MenuPos.x-19,MenuPos.y+2);
	printf("Player1 character: ");
	gotoxy(MenuPos.x,MenuPos.y+2);
	printf("*");
	gotoxy(MenuPos.x-19,MenuPos.y+4);
	printf("Player2 character: ");
	gotoxy(MenuPos.x,MenuPos.y+4);
	printf("#");
	gotoxy(MenuPos.x-1,MenuPos.y+6);
	printf("Start");
	gotoxy(MenuPos.x-1,MenuPos.y+8);
	printf("Back");

	//initializing LeftRightNums
	LeftRightNum[0]=1;
	for(i=1;i<5;i++){
		LeftRightNum[i]=i;
	}

	Bot[1]=false;	Bot[2]=false;	Bot[3]=false;	Player[0]=true;	Player[1]=true;	Player[2]=false;	Player[3]=false;
	caseNum[0]=0;caseNum[4]=0;caseNum[5]=0;caseNum[6]=4;caseNum[7]=5;

	while(whatOption!=-1){
		whatOption=MenuSelect(optionNum,MenuPos,borderSize,false,firstTime,newSelect,previousSelect,changeLeftRight);

		if(whatOption==1){
			//number of Players
			getEnter=0;
			if(changeLeftRight==1){
				if(LeftRightNum[0]==3)		{LeftRightNum[0]=1;}
				else						{LeftRightNum[0]++;}
			}
			else if(changeLeftRight==-1){
				if(LeftRightNum[0]==1)		{LeftRightNum[0]=3;}
				else						{LeftRightNum[0]--;}
			}
			if(changeLeftRight==1 || changeLeftRight==-1){
				if(LeftRightNum[0]==1){
					Player[1]=true;Player[2]=false;Player[3]=false;optionNum=5;gotoxy(MenuPos.x,MenuPos.y);printf("1");
					gotoxy(MenuPos.x-19,MenuPos.y+6);printf("                 ");gotoxy(MenuPos.x-19,MenuPos.y+8);printf("                 ");
					caseNum[4]=0;caseNum[5]=0;caseNum[6]=4;caseNum[7]=5;
					fakeSnakeExist[2]=false;fakeSnakeExist[3]=false;

					//fakeSnake appearance controlling
					for(i=0;i<6;i++){
						//erase  fakeSnake[2] and fakeSnake[3]
						gotoxy(fakeSnake[2][i].x,fakeSnake[2][i].y);
						printf(" ");
						gotoxy(fakeSnake[3][i].x,fakeSnake[3][i].y);
						printf(" ");
					}

					//start and back button appearance controlling
					gotoxy(MenuPos.x-1+4,MenuPos.y+8);
					printf(" ");
					gotoxy(MenuPos.x-1,MenuPos.y+10);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+12);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+6);
					printf("Start");
					gotoxy(MenuPos.x-1,MenuPos.y+8);
					printf("Back");
				}
				else if(LeftRightNum[0]==2){
					Player[1]=true;Player[2]=true;Player[3]=false;optionNum=6;gotoxy(MenuPos.x,MenuPos.y);printf("2");
					gotoxy(MenuPos.x-19,MenuPos.y+6);printf("Player3 character: %c",SnakeChar[2]);gotoxy(MenuPos.x-19,MenuPos.y+8);printf("                 ");
					caseNum[4]=4;caseNum[5]=0;caseNum[6]=5;caseNum[7]=6;
					fakeSnakeExist[2]=true;fakeSnakeExist[3]=false;
					for(i=0;i<6;i++){
						//erase  fakeSnake[3]
						gotoxy(fakeSnake[3][i].x,fakeSnake[3][i].y);
						printf(" ");
					}

					//start and back button appearance controlling
					gotoxy(MenuPos.x-1,MenuPos.y+6);
					printf("        ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+8);
					printf(" ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+10);
					printf(" ");
					gotoxy(MenuPos.x-1,MenuPos.y+12);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+8);
					printf("Start");
					gotoxy(MenuPos.x-1,MenuPos.y+10);
					printf("Back");
					gotoxy(MenuPos.x,MenuPos.y+6);
					printf("%c",SnakeChar[2]);

				}
				else if(LeftRightNum[0]==3){
					Player[1]=true;Player[2]=true;Player[3]=true;optionNum=7;gotoxy(MenuPos.x,MenuPos.y);printf("3");
					gotoxy(MenuPos.x-19,MenuPos.y+6);printf("Player3 character: %c",SnakeChar[2]);gotoxy(MenuPos.x-19,MenuPos.y+8);printf("Player4 character: %c",SnakeChar[3]);
					caseNum[4]=4;caseNum[5]=5;caseNum[6]=6;caseNum[7]=7;
					fakeSnakeExist[2]=true;fakeSnakeExist[3]=true;

					//start and back button appearance controlling
					gotoxy(MenuPos.x-1,MenuPos.y+6);
					printf("        ");
					gotoxy(MenuPos.x-1,MenuPos.y+8);
					printf("        ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+10);
					printf(" ");
					gotoxy(MenuPos.x-1+4,MenuPos.y+12);
					printf(" ");
					gotoxy(MenuPos.x-1,MenuPos.y+10);
					printf("Start");
					gotoxy(MenuPos.x-1,MenuPos.y+12);
					printf("Back");
					gotoxy(MenuPos.x,MenuPos.y+6);
					printf("%c",SnakeChar[2]);
					gotoxy(MenuPos.x,MenuPos.y+8);
					printf("%c",SnakeChar[3]);
				}

				changeLeftRight=0;
			}
		}
		else if(whatOption==2){
			//Player1 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,0 /*SnakeChar[0]*/, MenuPos, Player,true);
		}
		else if(whatOption==3){
			//Player2 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,1 /*SnakeChar[4]*/, MenuPos, Player,true);
		}
		else if(whatOption==caseNum[4]){
			//Player3 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,2 /*SnakeChar[5]*/, MenuPos, Player,true);
		}
		else if(whatOption==caseNum[5]){
			//Player4 character
			getEnter=0;
			SnakeCharController(SnakeChar, changeLeftRight, LeftRightNum,3 /*SnakeChar[6]*/, MenuPos, Player,true);
		}
		else if(whatOption==caseNum[6]){
			//start game
			getEnter=1;
		}
		else if(whatOption==caseNum[7]){
			//back
			getEnter=2;
		}
		else if(whatOption==-2){
			if(getEnter==1){
				return(startGame);
			}
			else if(getEnter==2){
				return(back);
			}
		}

		FakeSnake(MenuPos.x+60, MenuPos.y-5, SnakeChar[0], currentPos[0], insideCurrentPos[0], fakeSnake[0]);
		FakeSnake(MenuPos.x+60, MenuPos.y+1, SnakeChar[1], currentPos[1], insideCurrentPos[1], fakeSnake[1]);
		if(fakeSnakeExist[2])
			FakeSnake(MenuPos.x+60, MenuPos.y+7, SnakeChar[2], currentPos[2], insideCurrentPos[2], fakeSnake[2]);
		if(fakeSnakeExist[3])
			FakeSnake(MenuPos.x+60, MenuPos.y+13, SnakeChar[3], currentPos[3], insideCurrentPos[3], fakeSnake[3]);

		sleep(100);
	}
	return(back);
}	//end of MultiPlayerMenu function

int OptionMenu(int &sceneSize,float &speed,bool &poiApplePermision,bool poiAppleExist[],int &poiAppleNum
				,bool &chargedAppleExist,bool &increasingSpeed,int &increaseSpeedRate,int firstLength[],short LeftRightNum[]){

	const int back=2, controls=1;

	//MenuSelect neccessary variables
	const position MenuPos={75,25};
	int borderSize=60,optionNum=10,whatOption=1;
	short newSelect=1,previousSelect=1,changeLeftRight=0;
	bool firstTime=true;



	int i;
	short getEnter=0;
	char optionString[11][40],LeftRightString[11][5][30];

	//initializing optionStrings
	strcpy(optionString[1],"Size of game scene: ");
	strcpy(optionString[2],"Difficulty: ");
	strcpy(optionString[3],"Poisonous apple: ");
	strcpy(optionString[4],"Number of poisonous apples: ");
	strcpy(optionString[5],"Charged apple: ");
	strcpy(optionString[6],"Increasing speed: ");
	strcpy(optionString[7],"Rate of increasing speed: "); //should be between 1 and 5
	strcpy(optionString[8],"First length of snake: ");
	strcpy(optionString[9],"Controls");
	strcpy(optionString[10],"Back");

	//initializing LeftRightStrings
	strcpy(LeftRightString[2][1],"Easy  (low speed)");
	strcpy(LeftRightString[2][2],"Normal  (medium speed)");
	strcpy(LeftRightString[2][3],"Hard  (high speed)");
	strcpy(LeftRightString[2][4],"Expert  (too fast!)");
	strcpy(LeftRightString[3][1],"OFF"); strcpy(LeftRightString[3][2],"ON");
	strcpy(LeftRightString[5][1],"OFF"); strcpy(LeftRightString[5][2],"ON");
	strcpy(LeftRightString[6][1],"OFF"); strcpy(LeftRightString[6][2],"ON");

	//printing options
	system("cls");
	gotoxy(MenuPos.x-strlen(optionString[1])/2,MenuPos.y);		//Size of game scene
	printf("%s",optionString[1]);
	gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y);
	printf("%d",sceneSize);
	gotoxy(MenuPos.x-strlen(optionString[2])/2,MenuPos.y+2);	//Difficulty
	printf("%s",optionString[2]);
	gotoxy(MenuPos.x+strlen(optionString[2])/2,MenuPos.y+2);
	printf("%s",LeftRightString[2][LeftRightNum[2]]);//Easy at first
	gotoxy(MenuPos.x-strlen(optionString[3])/2,MenuPos.y+4);	//Poisonous apple
	printf("%s",optionString[3]);
	gotoxy(MenuPos.x+strlen(optionString[3])/2,MenuPos.y+4);
	printf("%s",LeftRightString[3][LeftRightNum[3]]);//OFF at first
	gotoxy(MenuPos.x-strlen(optionString[4])/2,MenuPos.y+6);	//Number of poisonous apples
	printf("%s",optionString[4]);
	gotoxy(MenuPos.x+strlen(optionString[4])/2,MenuPos.y+6);
	if(poiApplePermision==true)
		printf("%d",poiAppleNum);
	if(poiApplePermision==false)
		printf("None");
	gotoxy(MenuPos.x-strlen(optionString[5])/2,MenuPos.y+8);	//Charged apple
	printf("%s",optionString[5]);
	gotoxy(MenuPos.x+strlen(optionString[5])/2,MenuPos.y+8);
	printf("%s",LeftRightString[5][LeftRightNum[5]]);//OFF at first
	gotoxy(MenuPos.x-strlen(optionString[6])/2,MenuPos.y+10);	//Increasing speed
	printf("%s",optionString[6]);
	gotoxy(MenuPos.x+strlen(optionString[6])/2,MenuPos.y+10);
	printf("%s",LeftRightString[6][LeftRightNum[6]]);//OFF at first
	gotoxy(MenuPos.x-strlen(optionString[7])/2,MenuPos.y+12);	//Rate of increasing speed
	printf("%s",optionString[7]);
	gotoxy(MenuPos.x+strlen(optionString[7])/2,MenuPos.y+12);
	if(increasingSpeed==true)
		printf("%d",increaseSpeedRate);
	if(increasingSpeed==false)
		printf("None");
	gotoxy(MenuPos.x-strlen(optionString[8])/2,MenuPos.y+14);	//First length of snake
	printf("%s",optionString[8]);
	gotoxy(MenuPos.x+strlen(optionString[8])/2,MenuPos.y+14);
	printf("%d",firstLength[0]);
	gotoxy(MenuPos.x-strlen(optionString[9])/2,MenuPos.y+16);	//Controls
	printf("%s",optionString[9]);
	gotoxy(MenuPos.x-strlen(optionString[10])/2,MenuPos.y+18);	//Back
	printf("%s",optionString[10]);

	while(whatOption!=-1){
		whatOption=MenuSelect(optionNum,MenuPos,borderSize,false,firstTime,newSelect,previousSelect,changeLeftRight);

		if(whatOption==1){
			//size of game scene
			getEnter=1;
			changeLeftRight=0;	//this is for avoiding left and right affect other options here
		}
		else if(whatOption==2){
			//Difficulty
			int a;
			a=OptionLeftRightController(changeLeftRight,LeftRightNum,optionString,whatOption,LeftRightString,4 /*number of difficulties*/,MenuPos,borderSize);
			if(a==4)	{speed=(0.005)*a*2;}	//expert speed
			else 		{speed=(0.005)*a;}		//easy,normal and hard speed
			changeLeftRight=0;
			getEnter=0;

		}
		else if(whatOption==3){
			//Poisonous apple
			int a;
			a=OptionLeftRightController(changeLeftRight,LeftRightNum,optionString,whatOption,LeftRightString,2 /*ON and OFF*/,MenuPos,borderSize);
			if(a==1){
				poiApplePermision=false;
				//appearance of Number of poisonous apples option
				if(changeLeftRight==1 || changeLeftRight==-1){
					for(i=MenuPos.x+strlen(optionString[whatOption+1])/2;i<MenuPos.x+borderSize/2;i++){
						gotoxy(i,MenuPos.y+(whatOption+1-1)*2);
						printf(" ");
					}
					gotoxy(MenuPos.x+strlen(optionString[whatOption+1])/2,MenuPos.y+(whatOption+1-1)*2);
					printf("None");
					gotoxy(0,MenuPos.y+(whatOption-1)*2);
				}
			}
			else if(a==2){
				poiApplePermision=true;
				//appearance of Number of poisonous apples option
				if(changeLeftRight==1 || changeLeftRight==-1){
					for(i=MenuPos.x+strlen(optionString[whatOption+1])/2;i<MenuPos.x+borderSize/2;i++){
						gotoxy(i,MenuPos.y+(whatOption+1-1)*2);
						printf(" ");
					}
					gotoxy(MenuPos.x+strlen(optionString[whatOption+1])/2,MenuPos.y+(whatOption+1-1)*2);
					printf("%d",poiAppleNum);
					gotoxy(0,MenuPos.y+(whatOption-1)*2);
				}
			}
			changeLeftRight=0;
			getEnter=0;
		}
		else if(whatOption==4){
			//Number of poisonous apples
			getEnter=4;
			changeLeftRight=0;	//this is for avoiding left and right affect other options here

		}
		else if(whatOption==5){
			//Charged apple
			int a;
			a=OptionLeftRightController(changeLeftRight,LeftRightNum,optionString,whatOption,LeftRightString,2 /*ON and OFF*/,MenuPos,borderSize);
			if(a==1){
				chargedAppleExist=false;
			}
			else if(a==2){
				chargedAppleExist=true;
			}
			getEnter=0;
			changeLeftRight=0;

		}
		else if(whatOption==6){
			//Increasing speed
			int a;
			a=OptionLeftRightController(changeLeftRight,LeftRightNum,optionString,whatOption,LeftRightString,2 /*ON and OFF*/,MenuPos,borderSize);
			if(a==1){
				increasingSpeed=false;
				//appearance of Rate of increasing speed option
				if(changeLeftRight==1 || changeLeftRight==-1){
					for(i=MenuPos.x+strlen(optionString[whatOption+1])/2;i<MenuPos.x+borderSize/2;i++){
						gotoxy(i,MenuPos.y+(whatOption+1-1)*2);
						printf(" ");
					}
					gotoxy(MenuPos.x+strlen(optionString[whatOption+1])/2,MenuPos.y+(whatOption+1-1)*2);
					printf("None");
					gotoxy(0,MenuPos.y+(whatOption-1)*2);
				}
				increaseSpeedRate=0;
			}
			else if(a==2){
				increasingSpeed=true;
				//appearance of Rate of increasing speed option
				if(changeLeftRight==1 || changeLeftRight==-1){
					for(i=MenuPos.x+strlen(optionString[whatOption+1])/2;i<MenuPos.x+borderSize/2;i++){
						gotoxy(i,MenuPos.y+(whatOption+1-1)*2);
						printf(" ");
					}
					gotoxy(MenuPos.x+strlen(optionString[whatOption+1])/2,MenuPos.y+(whatOption+1-1)*2);
					printf("%d",increaseSpeedRate);
					gotoxy(0,MenuPos.y+(whatOption-1)*2);
				}
			}
			getEnter=0;
			changeLeftRight=0;

		}
		else if(whatOption==7){
			//Rate of Increasing Speed
			getEnter=7;
			changeLeftRight=0;	//this is for avoiding left and right affect other options here

		}
		else if(whatOption==8){
			//First length of snake
			getEnter=8;
			changeLeftRight=0;	//this is for avoiding left and right affect other options here

		}
		else if(whatOption==9){
			//Controls
			getEnter=9;
			changeLeftRight=0;	//this is for avoiding left and right affect other options here

		}
		else if(whatOption==10){
			//Back
			getEnter=10;
			changeLeftRight=0;	//this is for avoiding left and right affect other options here

		}
		else if(whatOption==-2){
			//enter is pressed!!!!!!!!!!!!!!!!!
			changeLeftRight=0;

			if(getEnter==1){
				//size of game scene

				//appearance
					int min=50,max=100;
					//printing hint
						gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
						printf("(between 50 and 100)");
					//controlling writting appearance
						WrittingController(MenuPos,optionString,sceneSize,poiAppleNum,increaseSpeedRate,firstLength,getEnter,min,max);
					//erasing hint
						gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
						printf("                    ");
					//putting back the end of menu border
						gotoxy(MenuPos.x+borderSize/2,MenuPos.y+(getEnter-1)*2);
						printf("|");
						gotoxy(0,MenuPos.y+(getEnter-1)*2);
			}
			else if(getEnter==4){
				//Number of poisonous apples
				if(poiApplePermision==true){

					//appearance
						int min=0,max=9;
						//printing hint
							gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
							printf("(between 0 and 9)");
						//controlling writting appearance
							WrittingController(MenuPos,optionString,sceneSize,poiAppleNum,increaseSpeedRate,firstLength,getEnter,min,max);
						//erasing hint
							gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
							printf("                    ");
						//putting back the end of menu border
							gotoxy(MenuPos.x+borderSize/2,MenuPos.y+(getEnter-1)*2);
							printf("|");
							gotoxy(0,MenuPos.y+(getEnter-1)*2);

					//logic
					for(i=0;i<10;i++){
						poiAppleExist[i]=false;
					}
					for(i=0;i<poiAppleNum;i++){
						poiAppleExist[i]=true;
					}

				}
				else if(poiApplePermision==false){
					poiAppleNum=0;
					//logic
					for(i=0;i<10;i++){
						poiAppleExist[i]=false;
					}
				}
			}
			else if(getEnter==7){
				//Rate of Increasing Speed
				if(increasingSpeed==true){

					//appearance
						int min=0,max=5;
						//printing hint
							gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
							printf("(between 0 and 5)");
						//controlling writting
							WrittingController(MenuPos,optionString,sceneSize,poiAppleNum,increaseSpeedRate,firstLength,getEnter,min,max);
						//erasing hint
							gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
							printf("                    ");
						//putting back the end of menu border
							gotoxy(MenuPos.x+borderSize/2,MenuPos.y+(getEnter-1)*2);
							printf("|");
							gotoxy(0,MenuPos.y+(getEnter-1)*2);

				}
			}
			else if(getEnter==8){
				//First length of snake

				//appearance
					int min=1,max=5;
					//printing hint
						gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
						printf("(between 1 and 5)");
					//controlling writting
						WrittingController(MenuPos,optionString,sceneSize,poiAppleNum,increaseSpeedRate,firstLength,getEnter,min,max);
						for(i=1;i<7;i++){
							firstLength[i]=firstLength[0];
						}
					//erasing hint
						gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+10,MenuPos.y+(getEnter-1)*2);
						printf("                    ");
					//putting back the end of menu border
						gotoxy(MenuPos.x+borderSize/2,MenuPos.y+(getEnter-1)*2);
						printf("|");
						gotoxy(0,MenuPos.y+(getEnter-1)*2);

			}
			else if(getEnter==9){
				//Controls
				return(controls);
			}
			else if(getEnter==10){
				//Back
				return(back);
			}
			getEnter=0;
		}

	}
	return(back);

}	//end of OptionMenu function

int OptionLeftRightController(short &changeLeftRight, short LeftRightNum[],char optionString[][40],int &whatOption
								,char LeftRightString[][5][30],int Number,position MenuPos,int borderSize){
	int i,j;

	if(changeLeftRight==1){
		if(LeftRightNum[whatOption]==Number)		{LeftRightNum[whatOption]=1;}
		else										{LeftRightNum[whatOption]++;}
	}
	else if(changeLeftRight==-1){
		if(LeftRightNum[whatOption]==1)		{LeftRightNum[whatOption]=Number;}
		else								{LeftRightNum[whatOption]--;}
	}

	for(i=1;i<=Number;i++){
		if(LeftRightNum[whatOption]==i){
			if(changeLeftRight==1 || changeLeftRight==-1){
				for(j=MenuPos.x+strlen(optionString[whatOption])/2;j<MenuPos.x+borderSize/2;j++){
					gotoxy(j,MenuPos.y+(whatOption-1)*2);
					printf(" ");
				}
				gotoxy(MenuPos.x+strlen(optionString[whatOption])/2,MenuPos.y+(whatOption-1)*2);
				printf("%s",LeftRightString[whatOption][i]);
				gotoxy(MenuPos.x-borderSize/2-1,MenuPos.y+(whatOption-1)*2);
			}
			return(i);
		}
	}

}

void WrittingController(position MenuPos,char optionString[][40],int &sceneSize,int &poiAppleNum,int &increaseSpeedRate
							,int firstLength[],int getEnter,int min,int max){

	int i=0,number=0,j=0,k=1,permanentNum[3]={0};
	short digitNumber;
	char getNumber;
	bool writting=true;

	if(getEnter==1){
		digitNumber=3;
	}
	else{
		digitNumber=1;
	}

	gotoxy(MenuPos.x+strlen(optionString[getEnter])/2,MenuPos.y+(getEnter-1)*2);
	printf("    ");

	do{
		gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+j,MenuPos.y+(getEnter-1)*2);
		getNumber=getch();
		if(48<=getNumber && getNumber<=57 && j<digitNumber /*getNumber ascii code should be 0,1,...,8 or 9 && we can only put certain numbers(j<digitNumber)*/){
			gotoxy(MenuPos.x+strlen(optionString[getEnter])/2+j,MenuPos.y+(getEnter-1)*2);
			printf("%c",getNumber);
			permanentNum[j]=(getNumber-48);
			if(getEnter==1 || j==0)
				j++;
		}
		else if(getNumber==27){
			gotoxy(MenuPos.x+strlen(optionString[getEnter])/2,MenuPos.y+(getEnter-1)*2);
			printf("    ");
			gotoxy(MenuPos.x+strlen(optionString[getEnter])/2,MenuPos.y+(getEnter-1)*2);
			if(getEnter==1){
				printf("%d",sceneSize);
			}
			if(getEnter==4){
				printf("%d",poiAppleNum);
			}
			if(getEnter==7){
				printf("%d",increaseSpeedRate);
			}
			if(getEnter==8){
				printf("%d",firstLength[0]);
			}
			gotoxy(0,MenuPos.y+(getEnter-1)*2);
			writting=false;
		}
		else if(getNumber==13){
			for(i=0;i<j-1;i++){
				k*=10;
			}
			for(i=0;i<j;i++){
				number+=permanentNum[i]*k;
				k/=10;
			}
			if(min<=number && number<=max){
				gotoxy(MenuPos.x+strlen(optionString[getEnter])/2,MenuPos.y+(getEnter-1)*2);
				if(getEnter==1){
					sceneSize=number;
				}
				if(getEnter==4){
					poiAppleNum=number;
				}
				if(getEnter==7){
					increaseSpeedRate=number;
				}
				if(getEnter==8){
					firstLength[0]=number;
				}
				printf("%d",number);
				gotoxy(0,MenuPos.y+(getEnter-1)*2);
				writting=false;
			}
			else{
				gotoxy(MenuPos.x+strlen(optionString[getEnter])/2,MenuPos.y+(getEnter-1)*2);
				printf("    ");
				number=0;	i=0;	j=0;	k=1;	permanentNum[0]=0;	permanentNum[1]=0;	permanentNum[2]=0;
			}
		}
	}while(writting);

}	//end of WrittingController function

int ControlsMenu(char PDirection[][5],char &pauseChar,bool eachArrowKeyExist[][5]){

	const int back=2;

	//MenuSelect neccessary variables
	const position MenuPos={75,15};
	int borderSize=60,optionNum=22,whatOption=1;
	short newSelect=1,previousSelect=1,changeLeftRight=0;
	bool firstTime=true;

	int i,j;
	short getEnter=0;
	char optionString[22][40];

	//initializing optionStrings
		//Player1 controls:
		strcpy(optionString[1],"Player1 controls:");
		//Player2 controls:
		strcpy(optionString[2],"Player2 controls:");
		//Player3 controls:
		strcpy(optionString[3],"Player3 controls:");
		//Player4 controls:
		strcpy(optionString[4],"Player4 controls:");
		//Direction
		strcpy(optionString[5],"Left: ");
		strcpy(optionString[6],"Right: ");
		strcpy(optionString[7],"Up: ");
		strcpy(optionString[8],"Down: ");
		//Pause:
		strcpy(optionString[9],"Pause: ");
		//Back
		strcpy(optionString[10],"Back");

	//printing options
	system("cls");
	for(i=0;i<4;i++){
		gotoxy(MenuPos.x-strlen(optionString[i+1])/2,MenuPos.y+i*5*2);		//Player controls:
		printf("%s",optionString[i+1]);
		for(j=0;j<4;j++){
			gotoxy(MenuPos.x-strlen(optionString[i+1])/2,MenuPos.y+(i*5*2)+(j+1)*2);		//Direction:
			printf("%s",optionString[j+5]);
			gotoxy(MenuPos.x+strlen(optionString[i+1])/2,MenuPos.y+(i*5*2)+(j+1)*2);		//Direction character
			// a condition to find out is the PDirection[i+1][j+1] an arrow key
			if(eachArrowKeyExist[i+1][j+1]==true){
				if(PDirection[i+1][j+1]==75)		{printf("%c",17);}
				else if(PDirection[i+1][j+1]==77)	{printf("%c",16);}
				else if(PDirection[i+1][j+1]==72)	{printf("%c",30);}
				else if(PDirection[i+1][j+1]==80)	{printf("%c",31);}
			}
			else if(eachArrowKeyExist[i+1][j+1]==false){
				printf("%c",PDirection[i+1][j+1]);
			}
			// end of this condition
		}
	}
	//Print Pause:
	gotoxy(MenuPos.x-strlen(optionString[1])/2,MenuPos.y+40);
	printf("%s",optionString[9]);
	gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+40);
	if(eachArrowKeyExist[5][0]/*pauseArrowKeyExist*/==true){
		if(pauseChar==75)		{printf("%c",17);}
		else if(pauseChar==77)	{printf("%c",16);}
		else if(pauseChar==72)	{printf("%c",30);}
		else if(pauseChar==80)	{printf("%c",31);}
	}
	else if(eachArrowKeyExist[5][0]/*pauseArrowKeyExist*/==false){
		printf("%c",pauseChar);
	}
	//Print Back
	gotoxy(MenuPos.x-strlen(optionString[10])/2,MenuPos.y+42);
	printf("%s",optionString[10]);

	while(whatOption!=-1){
		whatOption=MenuSelect(optionNum,MenuPos,borderSize,true,firstTime,newSelect,previousSelect,changeLeftRight);

		//Player1
		if(whatOption==2){
			//Player1 Left
			int whichPlayer=1 ,whichDirection=1 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==3){
			//Player1 Right
			int whichPlayer=1 ,whichDirection=2 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==4){
			//Player1 Up
			int whichPlayer=1 ,whichDirection=3 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==5){
			//Player1 Down
			int whichPlayer=1 ,whichDirection=4 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		//Player2
		else if(whatOption==7){
			//Player2 Left
			int whichPlayer=2 ,whichDirection=1 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==8){
			//Player2 Right
			int whichPlayer=2 ,whichDirection=2 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==9){
			//Player2 Up
			int whichPlayer=2 ,whichDirection=3 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==10){
			//Player2 Down
			int whichPlayer=2 ,whichDirection=4 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		//Player3
		else if(whatOption==12){
			//Player3 Left
			int whichPlayer=3 ,whichDirection=1 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==13){
			//Player3 Right
			int whichPlayer=3 ,whichDirection=2 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==14){
			//Player3 Up
			int whichPlayer=3 ,whichDirection=3 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==15){
			//Player3 Down
			int whichPlayer=3 ,whichDirection=4 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		//Player4
		else if(whatOption==17){
			//Player4 Left
			int whichPlayer=4 ,whichDirection=1 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==18){
			//Player4 Right
			int whichPlayer=4 ,whichDirection=2 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==19){
			//Player4 Up
			int whichPlayer=4 ,whichDirection=3 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==20){
			//Player4 Down
			int whichPlayer=4 ,whichDirection=4 /*left==1,right==2,up==3,down==4*/;
			DirectionCharController(PDirection,optionString,whichPlayer,whichDirection,eachArrowKeyExist,MenuPos,borderSize);
		}
		else if(whatOption==21){
			//Pause:
			unsigned char permanentChar=0;

			while(permanentChar!=27){
				gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+40);
				printf("                                             ");
				gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+40);
				permanentChar=getch();

				if(permanentChar==224){
					//arrow key is pressed!
					permanentChar=getch();

					if(permanentChar==75){
						//Left arrow
						printf("%c",17);
					}
					else if(permanentChar==77){
						//Right arrow
						printf("%c",16);
					}
					else if(permanentChar==72){
						//Up arrow
						printf("%c",30);
					}
					else if(permanentChar==80){
						//Down arrow
						printf("%c",31);
					}

					//you can only press Return or Escape now
					while(1){
						char a=getch();
						if(a==13){
							pauseChar=permanentChar;
							eachArrowKeyExist[5][0]/*pauseArrowKeyExist*/=true;
							permanentChar=27;
							gotoxy(MenuPos.x+borderSize/2,MenuPos.y+40);
							printf("|");
							gotoxy(0,MenuPos.y+40);
							break;
						}
						else if(a==27){
							permanentChar=27;
							break;
						}
					}
				}
				else if(permanentChar!=27){	//an arrow key isn't pressed

					gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+40);
					printf("%c",permanentChar);

					//you can only press Return or Escape now
					while(1){
						char a=getch();
						if(a==13){
							pauseChar=permanentChar;
							eachArrowKeyExist[5][0]/*pauseArrowKeyExist*/=false;
							permanentChar=27;
							gotoxy(MenuPos.x+borderSize/2,MenuPos.y+40);
							printf("|");
							gotoxy(0,MenuPos.y+40);
							break;
						}
						else if(a==27){
							permanentChar=27;
							break;
						}
					}

				}

			}
			//permanentChar is now 27
			gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+40);
			if(eachArrowKeyExist[5][0]/*pauseArrowKeyExist*/==true){
				if(pauseChar==75)			{printf("%c",17);}
				else if(pauseChar==77)	{printf("%c",16);}
				else if(pauseChar==72)	{printf("%c",30);}
				else if(pauseChar==80)	{printf("%c",31);}
			}
			else if(eachArrowKeyExist[5][0]/*pauseArrowKeyExist*/==false){
				printf("%c",pauseChar);
			}
			gotoxy(MenuPos.x+borderSize/2,MenuPos.y+40);
			printf("|");
			gotoxy(0,MenuPos.y+40);
		}
		else if(whatOption==22){
			//Back
			return(back);
		}

	}
	//Escape is pressed
	return(back);

}	//end of ControlsMenu function

void DirectionCharController(char PDirection[][5],char optionString[][40],int whichPlayer, int whichDirection
								,bool eachArrowKeyExist[][5],position MenuPos, int borderSize){

	unsigned char permanentChar=0;

	while(permanentChar!=27){
		gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
		printf("                                             ");
		gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
		permanentChar=getch();

		if(permanentChar==224){
			//arrow key is pressed!
			permanentChar=getch();

			if(permanentChar==75){
				//Left arrow
				printf("%c",17);
			}
			else if(permanentChar==77){
				//Right arrow
				printf("%c",16);
			}
			else if(permanentChar==72){
				//Up arrow
				printf("%c",30);
			}
			else if(permanentChar==80){
				//Down arrow
				printf("%c",31);
			}

			//you can only press Return or Escape now
			while(1){
				char a=getch();
				if(a==13){
					eachArrowKeyExist[whichPlayer][whichDirection]=true;
					PDirection[whichPlayer][whichDirection]=permanentChar;
					gotoxy(MenuPos.x+borderSize/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
					printf("|");
					gotoxy(0,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
					return;
				}
				else if(a==27){
					permanentChar=27;
					break;
				}
			}
		}
		else if(permanentChar!=27){	//an arrow key isn't pressed

			gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
			printf("%c",permanentChar);

			//you can only press Return or Escape now
			while(1){
				char a=getch();
				if(a==13){
					eachArrowKeyExist[whichPlayer][whichDirection]=false;
					PDirection[whichPlayer][whichDirection]=permanentChar;
					gotoxy(MenuPos.x+borderSize/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
					printf("|");
					gotoxy(0,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
					return;
				}
				else if(a==27){
					permanentChar=27;
					break;
				}
			}

		}

	}
	//permanentChar is now 27
	gotoxy(MenuPos.x+strlen(optionString[1])/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
	if(eachArrowKeyExist[whichPlayer][whichDirection]==true){
		if(PDirection[whichPlayer][whichDirection]==75)			{printf("%c",17);}
		else if(PDirection[whichPlayer][whichDirection]==77)	{printf("%c",16);}
		else if(PDirection[whichPlayer][whichDirection]==72)	{printf("%c",30);}
		else if(PDirection[whichPlayer][whichDirection]==80)	{printf("%c",31);}
	}
	else if(eachArrowKeyExist[whichPlayer][whichDirection]==false){
		printf("%c",PDirection[whichPlayer][whichDirection]);
	}
	gotoxy(MenuPos.x+borderSize/2,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
	printf("|");
	gotoxy(0,MenuPos.y+((whichPlayer-1)*5*2)+(whichDirection)*2);
	return;

}	//end of DirectionCharController function

int GameScene(int sceneSize,int poiAppleNum,int increaseSpeedRate,int firstLength[],float speed,bool poiApplePermision,
				bool poiAppleExist[],bool chargedAppleExist,bool increasingSpeed,char PDirection[][5],char pauseChar,
				bool eachArrowKeyExist[][5],char SnakeChar[],bool Player[],int speedCount[],bool GameRunning,bool specialKey,
				int i,int j,int countPoiApple,int snakeLength[],char DefaultMove[],char InputP[][10],unsigned char inputKey,
				position snake[][500],position apple,position PoisonousApple[],position chargedApple,short CountP[]){






	system("cls");

	//printing the walls
	for(i=75-sceneSize/2;i<=75+sceneSize/2;i++){
    	gotoxy(i,25-sceneSize/4);
    	printf("$");
    	gotoxy(i,25+sceneSize/4);
    	printf("$");
	}
	for(i=25-sceneSize/4;i<=25+sceneSize/4;i++){
    	gotoxy(75-sceneSize/2,i);
    	printf("$");
    	gotoxy(75+sceneSize/2,i);
    	printf("$");
	}

    //needed for rand();
    srand(time(NULL));

    //initializing snakes
    	//Player1	left position
    	for(j=0;j<snakeLength[0];j++){
    		snake[0][j].x=75-sceneSize/2+snakeLength[0]-j;
    		snake[0][j].y=25;
		}
		//Player2	right position
    	for(j=0;j<snakeLength[1];j++){
    		snake[1][j].x=75+sceneSize/2-snakeLength[1]+j;
    		snake[1][j].y=25;
		}
		//Player3	down position
    	for(j=0;j<snakeLength[2];j++){
    		snake[2][j].x=75;
    		snake[2][j].y=25+sceneSize/4-snakeLength[2]+j;
		}
		//Player4	up position
    	for(j=0;j<snakeLength[3];j++){
    		snake[3][j].x=75;
    		snake[3][j].y=25-sceneSize/4+snakeLength[3]-j;
		}


    //printing snakes at the begining
    for(i=0;i<4;i++){
    	if(Player[i]){
    		for(j=0;j<snakeLength[i];j++){
    			gotoxy(snake[i][j].x,snake[i][j].y);
        		printf("%c",SnakeChar[i]);
			}
		}
	}

	//n is now 5
    RandomApple(apple,PoisonousApple,poiApplePermision,poiAppleNum,chargedApple,
					chargedAppleExist,snake,snakeLength,Player,sceneSize);
    //set coordinates of poisonous apple out of the game scene at first
    if(poiApplePermision){
    	for(i=0;i<poiAppleNum;i++){
    		PoisonousApple[i].x=75+sceneSize/2+5;
			PoisonousApple[i].y=25;
		}
	}

	//the game starts here
    do{

        if(Player[0]) {Condition(snake,0,snakeLength,Player,apple,PoisonousApple,poiApplePermision,
									poiAppleExist,poiAppleNum,chargedApple,chargedAppleExist,sceneSize,SnakeChar);}
		if(Player[1]) {Condition(snake,1,snakeLength,Player,apple,PoisonousApple,poiApplePermision,
									poiAppleExist,poiAppleNum,chargedApple,chargedAppleExist,sceneSize,SnakeChar);}
		if(Player[2]) {Condition(snake,2,snakeLength,Player,apple,PoisonousApple,poiApplePermision,
									poiAppleExist,poiAppleNum,chargedApple,chargedAppleExist,sceneSize,SnakeChar);}
		if(Player[3]) {Condition(snake,3,snakeLength,Player,apple,PoisonousApple,poiApplePermision,
									poiAppleExist,poiAppleNum,chargedApple,chargedAppleExist,sceneSize,SnakeChar);}

        if(poiApplePermision){
        	countPoiApple++;
			if(countPoiApple==200){
				//erasing visible previous poisonous apples
				for(i=0;i<poiAppleNum;i++){
					if(poiAppleExist[i]==true){
						gotoxy(PoisonousApple[i].x,PoisonousApple[i].y);
						printf(" ");
					}
				}

				RandomPoisonousApple(apple,PoisonousApple,poiAppleNum,chargedApple,chargedAppleExist,snake,snakeLength,Player,sceneSize);
				for(i=0;i<poiAppleNum;i++){
					poiAppleExist[i]=true;
					gotoxy(PoisonousApple[i].x,PoisonousApple[i].y);
					printf("!");
				}
				countPoiApple=0;
			}
		}

		//countp1=0 at first of the main()
		while(kbhit()){
			//getting the key
			inputKey=getch();
			if(inputKey==224){
				//especial key is pressed!
				specialKey=true;
				inputKey=getch();	//up==72 down==80 left==75 right==77
			}
			else	{specialKey=false;}

			//analysing the key
			for(i=1;i<5;i++){
				if(Player[i-1]==true){
					for(j=1;j<5;j++){
						if(eachArrowKeyExist[i][j]==true){
							if(inputKey==PDirection[i][j]  && specialKey){
								InputP[i-1][CountP[i-1]]=inputKey;
								CountP[i-1]++;
							}
						}
						else{
							if(inputKey==PDirection[i][j]  && !specialKey){
								InputP[i-1][CountP[i-1]]=inputKey;
								CountP[i-1]++;
							}
						}
					}
				}
			}

			while(inputKey==pauseChar){
				getch();
				if(inputKey==pauseChar)
					break;
			}
		}

		if(Player[0])	{ChangeHead(PDirection,DefaultMove,InputP,CountP,snake[0][0],0);}
		if(Player[1])	{ChangeHead(PDirection,DefaultMove,InputP,CountP,snake[1][0],1);}
		if(Player[2])	{ChangeHead(PDirection,DefaultMove,InputP,CountP,snake[2][0],2);}
		if(Player[3])	{ChangeHead(PDirection,DefaultMove,InputP,CountP,snake[3][0],3);}

        //print'*' in the coordinates of (snake.x[0],snake.y[0]);
        if(Player[0])	{gotoxy(snake[0][0].x,snake[0][0].y);printf("%c",SnakeChar[0]);}
        if(Player[1])	{gotoxy(snake[1][0].x,snake[1][0].y);printf("%c",SnakeChar[1]);}
        if(Player[2])	{gotoxy(snake[2][0].x,snake[2][0].y);printf("%c",SnakeChar[2]);}
        if(Player[3])	{gotoxy(snake[3][0].x,snake[3][0].y);printf("%c",SnakeChar[3]);}

        //game ending
        if(Player[0]==false && Player[1]==false && Player[2]==false && Player[3]==false){
        	GameRunning=false;
		}

		sleep(1/(speed+increaseSpeedRate*0.0005));


    }while(GameRunning);



    system("cls");
    for(i=65;i<=92;i++){
    	gotoxy(i,20);
    	printf("$");
    	gotoxy(i,40);
    	printf("$");
	}
	for(i=20;i<=40;i++){
    	gotoxy(65,i);
    	printf("$");
    	gotoxy(92,i);
    	printf("$");
	}

    gotoxy(70,24);
    printf("Player1 score is: %d",snakeLength[0]-firstLength[0]);
    gotoxy(70,28);
    printf("Player2 score is: %d",snakeLength[1]-firstLength[1]);
    gotoxy(70,32);
    printf("Player3 score is: %d",snakeLength[2]-firstLength[2]);
    gotoxy(70,36);
    printf("Player4 score is: %d",snakeLength[3]-firstLength[3]);

    gotoxy(65,50);
    printf("Press any key to continue");

    getch();

    system("cls");

	return(2);

}	//end of GameScene function



void RandomApple(position &apple,position PoisonousApple[],bool &poiApplePermision,int &poiAppleNum,position &chargedApple,
					bool &chargedAppleExist, const position snake[][500], int snakeLength[],bool Player[],int sceneSize){
    int i,j;
    bool sw;
    //srand(time(NULL));
    do{
        //apple.x= random();
        //apple.y= random();
        apple.x=rand()%(sceneSize/2+sceneSize/2+1-2)+(75-sceneSize/2+1);
        apple.y=rand()%(sceneSize/4+sceneSize/4+1-2)+(25-sceneSize/4+1);

        sw=true;
        for(i=0;i<4 && sw;i++){
        	if(Player[i]){
        		for(j=0;j<snakeLength[i] && sw ;j++){
            		if(apple.x==snake[i][j].x && apple.y==snake[i][j].y)
               	 		sw=false;
        		}
			}
		}

        if(poiApplePermision){
        	for(i=0;i<poiAppleNum;i++){
        		if(PoisonousApple[i].x==apple.x && PoisonousApple[i].y==apple.y)
        			sw=false;
			}
		}

		if(chargedAppleExist){
			//charged apple is 4 character
			if(apple.x==chargedApple.x && apple.y==chargedApple.x)
        		sw=false;
        	if(apple.x==chargedApple.x+1 && apple.y==chargedApple.y+1)
        		sw=false;
        	if(apple.x==chargedApple.x+1 && apple.y==chargedApple.y)
        		sw=false;
        	if(apple.x==chargedApple.x && apple.y==chargedApple.y+1)
        		sw=false;
		}

    }while(!sw);
    //print'+' in the coordinates of (apple.x,apple.y);
    gotoxy(apple.x,apple.y);
    printf("+");
}
//**********************************************************************
void RandomPoisonousApple(position &apple,position PoisonousApple[],int &poiAppleNum,position &chargedApple,
					bool &chargedAppleExist, const position snake[][500], int snakeLength[],bool Player[],int sceneSize){
    int i,j,k;
    bool sw;
    //srand(time(NULL));
    for(k=0;k<poiAppleNum;k++){
    	do{
        	//PoisonousApple.x= random();
        	//PoisonousApple.y= random();
        	PoisonousApple[k].x=rand()%(sceneSize/2+sceneSize/2+1-2)+(75-sceneSize/2+1);
        	PoisonousApple[k].y=rand()%(sceneSize/4+sceneSize/4+1-2)+(25-sceneSize/4+1);

       		sw=true;

        	for(i=0;i<4 && sw;i++){
        		if(Player[i]){
        			for(j=0;j<snakeLength[i] && sw ;j++){
          	  		if(PoisonousApple[k].x==snake[i][j].x && PoisonousApple[k].y==snake[i][j].y)
         	      	 		sw=false;
        			}
				}
			}

        	if(PoisonousApple[k].x==apple.x && PoisonousApple[k].y==apple.y)
        		sw=false;

        	if(chargedAppleExist){
        		//charged apple is 4 character
        		if(PoisonousApple[k].x==chargedApple.x && PoisonousApple[k].y==chargedApple.y)
        			sw=false;
        		if(PoisonousApple[k].x==chargedApple.x+1 && PoisonousApple[k].y==chargedApple.y+1)
        			sw=false;
        		if(PoisonousApple[k].x==chargedApple.x+1 && PoisonousApple[k].y==chargedApple.y)
        			sw=false;
        		if(PoisonousApple[k].x==chargedApple.x && PoisonousApple[k].y==chargedApple.y+1)
        			sw=false;
			}

    	}while(!sw);
	}

}

void UpdateSnake(position snake[][500],short &thisPlayer,int &snakeLength){
	int i;
	for(i=snakeLength-1;i>0;i--){
        snake[thisPlayer][i].x=snake[thisPlayer][i-1].x;
        snake[thisPlayer][i].y=snake[thisPlayer][i-1].y;
    }
}

void ChangeHead(const char PDirection[][5],char DefaultMoveP[],char InputP[][10],short CountP[],position &SnakeHead,int thisPlayer){
	short i,j,k;
			//to understand which direction
	  		for(j=1;j<5;j++){
	  			//analysing the input[i][0] condition
	  			if(DefaultMoveP[thisPlayer]==PDirection[thisPlayer+1][j]){
					if(j%2==0){
						if(InputP[thisPlayer][0]!=PDirection[thisPlayer+1][j-1])
							DefaultMoveP[thisPlayer]=InputP[thisPlayer][0];
					}
					else{
						if(InputP[thisPlayer][0]!=PDirection[thisPlayer+1][j+1])
							DefaultMoveP[thisPlayer]=InputP[thisPlayer][0];
					}
		 	 	}
			}
			for(k=0;k<CountP[thisPlayer]-1;k++)
				InputP[thisPlayer][k]=InputP[thisPlayer][k+1];
			if(CountP[thisPlayer]>0) {CountP[thisPlayer]--;}

			//changing head
			if(DefaultMoveP[thisPlayer]==PDirection[thisPlayer+1][1])//left
				SnakeHead.x -= 1;
			if(DefaultMoveP[thisPlayer]==PDirection[thisPlayer+1][2])//right
				SnakeHead.x += 1;
			if(DefaultMoveP[thisPlayer]==PDirection[thisPlayer+1][3])//up
				SnakeHead.y -= 1;
			if(DefaultMoveP[thisPlayer]==PDirection[thisPlayer+1][4])//down
				SnakeHead.y += 1;
}

void Condition(position snake[][500],short thisPlayer,int snakeLength[],bool Player[],position &apple,position PoisonousApple[],
				bool &poiApplePermision,bool poiAppleExist[],int &poiAppleNum,position &chargedApple,bool chargedAppleExist,int &sceneSize,char SnakeChar[]){
	int i,j,k;
	bool sw=true;
	bool jafar=true;//this is for making the "if"s  "else if" with each other

		//checking colliding other snakes
		for(i=0;i<4 && sw;i++){
			if(Player[i] && i!=thisPlayer){
				for(j=0;j<snakeLength[i] && sw;j++){
        			if(snake[thisPlayer][0].x==snake[i][j].x && snake[thisPlayer][0].y==snake[i][j].y){
        				jafar=false;
        				Player[thisPlayer]=false;
        				//erasing the hitter snake body
						for(k=1;k<snakeLength[thisPlayer];k++){
							gotoxy(snake[thisPlayer][k].x,snake[thisPlayer][k].y);
							printf(" ");
						}
						//both snakes crashed with head
        				if(snake[thisPlayer][0].x==snake[i][0].x && snake[thisPlayer][0].y==snake[i][0].y){
        					Player[i]=false;
        					//erasing the hitted snake body
							for(k=0;k<snakeLength[i];k++){
								gotoxy(snake[i][k].x,snake[i][k].y);
								printf(" ");
							}
						}
						//only this player crashed with head
						else{
							//printing back the hitted snake body (one character)
        					gotoxy(snake[i][j].x,snake[i][j].y);
							printf("%c",SnakeChar[i]);
						}
						sw=false;
					}
				}
			}
		}

		//checking colliding apple
        if(snake[thisPlayer][0].x==apple.x && snake[thisPlayer][0].y==apple.y){
        	jafar=false;
        	snakeLength[thisPlayer]++;
        	UpdateSnake(snake,thisPlayer,snakeLength[thisPlayer]);
			RandomApple(apple,PoisonousApple,poiApplePermision,poiAppleNum,chargedApple,
						chargedAppleExist,snake,snakeLength,Player,sceneSize);
        }
        //checking colliding PoisonousAapple
        if(poiApplePermision){
        	for(i=0;i<poiAppleNum;i++){
        		if(poiAppleExist[i]){
        			if(snake[thisPlayer][0].x==PoisonousApple[i].x && snake[thisPlayer][0].y==PoisonousApple[i].y){
        				jafar=false;
        				poiAppleExist[i]=false;
        				if(snakeLength[thisPlayer]==1){
        					Player[thisPlayer]=false;
        					gotoxy(snake[thisPlayer][0].x,snake[thisPlayer][0].y);
        					printf(" ");
        					snakeLength[thisPlayer]--;
						}
						else{
        					gotoxy(snake[thisPlayer][snakeLength[thisPlayer]-1].x,snake[thisPlayer][snakeLength[thisPlayer]-1].y);
        					printf(" ");
        					gotoxy(snake[thisPlayer][snakeLength[thisPlayer]-2].x,snake[thisPlayer][snakeLength[thisPlayer]-2].y);
        					printf(" ");
        					snakeLength[thisPlayer]--;
        					UpdateSnake(snake,thisPlayer,snakeLength[thisPlayer]);
        				}
					}
				}
			}
		}
		//checking colliding walls
        if(snake[thisPlayer][0].x==75+sceneSize/2 || snake[thisPlayer][0].y==25+sceneSize/4 || snake[thisPlayer][0].x==75-sceneSize/2 || snake[thisPlayer][0].y==25-sceneSize/4){
        	jafar=false;
			Player[thisPlayer]=false;
        	for(i=1;i<snakeLength[thisPlayer];i++){
				gotoxy(snake[thisPlayer][i].x,snake[thisPlayer][i].y);
				printf(" ");
			}
			gotoxy(snake[thisPlayer][0].x,snake[thisPlayer][0].y);
			printf("$");
		}
		//nothing special heppened
        if(jafar==true) {
        	//erase the character in coordinates of (snake.x[n],snake.y[n]);
            gotoxy(snake[thisPlayer][snakeLength[thisPlayer]-1].x,snake[thisPlayer][snakeLength[thisPlayer]-1].y);
            printf(" ");
            UpdateSnake(snake,thisPlayer,snakeLength[thisPlayer]);
            for(i=3;i<snakeLength[thisPlayer];i++){
                if(snake[thisPlayer][0].x==snake[thisPlayer][i].x && snake[thisPlayer][0].y==snake[thisPlayer][i].y){
                    Player[thisPlayer]=false;
					for(i=0;i<snakeLength[thisPlayer];i++){
						gotoxy(snake[thisPlayer][i].x,snake[thisPlayer][i].y);
						printf(" ");
					}

                }
            }

        }
}  //end of Condition function




int main(){
	int i,j,k;
	char SnakeChar[7]={'*','#','0','O','*','*','*'};
	// SnakeChar[0]==SnakeCharP1
	// SnakeChar[1]==SnakeCharP2
	// SnakeChar[2]==SnakeCharP3
	// SnakeChar[3]==SnakeCharP4
	// SnakeChar[4]==SnakeCharBot1
	// SnakeChar[5]==SnakeCharBot2
	// SnakeChar[6]==SnakeCharBot3
	bool Bot[4]={false,false,false,false};
	bool Player[4]={true,true,false,false};



	fullscreen();

	MenuManager(SnakeChar, Player, Bot);


	return 0;
}











