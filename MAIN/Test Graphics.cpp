/*
	-lwinmm
	-lbgi 
	-lgdi32 
	-luser32
*/

#include<graphics.h>
#include<conio.h>
#include<dos.h>
#include<iostream>
#include<windows.h>
#include<mmsystem.h>
#include <pthread.h>

#define STANDING_BOY 2
#define MAIN_DELAY 100

using namespace std;

//*** Funcltion defination ***
//Bird Function
void * getBirdImage(int,int,int);
void printBird(int,int,int);

//Boy Function
void * getBoyImage(int,int,int);
void printBoy(int,int,int);

//Obstacle Function
void * getObstacleImage(int,int,int);
void printObstacle(int,int,int);

//GameOverBoard Function
void * getGameOverBoardImage(int,int);
void printGameOverBoard(int,int);

//White Image
void * getWhiteImage();

//Floor Image
void * getFloorImage(int);

//GameOverBoard Image
void * gameOverBoardImage;

//Geme Over Function
void gameOverFunction(int,int,int,int,int,int,int,int,int,bool,int,int,int,int);

//Last Page Score Function
void lastPageScore(int);

//Start Page Function
void startPage();	




// *** Globle Variables ***
//Score Variable
char scoreString[10] = "";

//Bird Image
void *bird[4];

//Boy Image
void *boy[3];

//Obstacle Image
void *obstacleImage;

//White Image
void * whiteImage;

//Floor Image
void * floorImage;

//Play again variable
bool playAgain;


void * marioDieSoundThreadFunction(void *data)
{
    PlaySound(TEXT("marioDieSound.wav"),NULL,SND_SYNC);  
	pthread_exit(NULL); 
}
pthread_t marioDieSoundThread;

void * marioFootSoundThreadFunction(void *data)
{
    PlaySound(TEXT("footSound.wav"),NULL,SND_SYNC);  
	pthread_exit(NULL); 
}
pthread_t marioFootSoundThread;

void * marioJumpSoundThreadFunction(void *data)
{
    PlaySound(TEXT("jumpSound.wav"),NULL,SND_SYNC);  
	pthread_exit(NULL); 
}
pthread_t marioJumpSoundThread;


	

//*** Main Function ***
int main()
{
	    
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");
	    
    //*** Getting Images *** 
	//Getting Bird Image	
	for(int i=0;i<3;i++){
		bird[i]=getBirdImage(1,1,i);
	}
	bird[3]=bird[1];
	
	//Getting Boy Image
	for(int i=0;i<3;i++){
		boy[i]=getBoyImage(1,1,i);
	}
	
	//Getting Obstacle Image
	obstacleImage=getObstacleImage(1,1,0);
	
	//Getting White Image
	whiteImage = getWhiteImage();
	
	//Getting floor Image
	floorImage = getFloorImage(350);
	
	//Getting Game Over Board Image
	gameOverBoardImage = getGameOverBoardImage(1,1);
		
	startPage(); // Calling start page	
		
	//Start From Initial Lable
	StartFromInitialLable:
		
	//playAgain Variable
	playAgain=false;	
		
	//*** Variables,Counters,Cordinates,Flags *** 
	//Cordinates
	int floorX = 0, floorY = 350;
	int boyX = floorX + 100 , boyY = floorY - 54; //Boy : [size 26 X 53]
	int birdX = 631 , birdY = floorY - 270;
	int obstacleOneX = getmaxx() , obstacleY = floorY - 23; 
	int obstacleTwoX = getmaxx() + 250 ; 
	int scoreBoardX = 158, scoreBoardY = 25;
	
	//Variables
	char inputCharacter;
	int jumpHeight = 60;
	int boyImageType=0;
	int boyUpDownIncrement = 10; //Speed For Boy Up - Down
	int delayIncrementor = 0;	
			
	//Counters
	int boyCounter = 0;
	int birdCounter = 0;
	int jumpCounter = 0;
	
	//Flags
	bool jumpFlag = false;
	bool whiteImageFlag = false;
	bool gameOverFlag = false;
		
	
	
		
	//*** Main Loop ***
	while(!gameOverFlag){
		
		if(!jumpFlag)	//Boy not Jumping		
		{			
			boyImageType = (boyCounter & 1);
			
			if(kbhit())
	        {
	            inputCharacter = getch();
	
	            if(inputCharacter==' ')
	            {
	        		jumpFlag = true; 
	        		//Jump Sound
	        		pthread_create(&marioJumpSoundThread,NULL,marioJumpSoundThreadFunction,(void *)0);
	            }
	        }	
	        
	        //FootSound
			//pthread_create(&marioFootSoundThread,NULL,marioFootSoundThreadFunction,(void *)0);
		
		}
		else	// Boy Jumping
		{
			       
			boyImageType=STANDING_BOY;
			
			jumpCounter+=boyUpDownIncrement;
			boyY-=boyUpDownIncrement;
									
			if(jumpCounter>=jumpHeight){
				boyUpDownIncrement=-boyUpDownIncrement;
			}
			
			if(jumpCounter<=0){
				boyUpDownIncrement=-boyUpDownIncrement;
				jumpCounter=0;
				jumpFlag=false;
			}	
			
			if(kbhit())
	            getch();		
		}
				
		
		//Bird Condition	
		if(true){
			
			birdX-=8;
			
			birdCounter++;
			if(birdCounter==4)
				birdCounter=0;
			
			if(birdX<=-110)
				birdX=631;
		}
		
		//Bird Condition	
		if(true){
			
			obstacleOneX-=7;
			
			if(obstacleOneX<=-25)
				obstacleOneX=631;
				
			obstacleTwoX-=7;
			
			if(obstacleTwoX<=-25)
				obstacleTwoX=631;
		}
		
		
		//*** Generating Screen ***
		//White Image
		if( (boyCounter % 300) == 0)
			whiteImageFlag=!whiteImageFlag;
			
		if(whiteImageFlag)
			putimage(0,0,whiteImage,OR_PUT);
		else
			cleardevice();
				
		//Floor
		putimage(floorX,floorY,floorImage,XOR_PUT);
		
		//boy 
		putimage(boyX,boyY,boy[boyImageType],XOR_PUT);
		
		//birds
		putimage(birdX+27,	birdY-23,	bird[birdCounter],			XOR_PUT);
		putimage(birdX,		birdY,		bird[((birdCounter+1)%3)],	XOR_PUT);
		putimage(birdX+30,	birdY+20,	bird[((birdCounter+2)%3)],	XOR_PUT);
		putimage(birdX+60,	birdY+45,	bird[birdCounter],			XOR_PUT);
		
		putimage((420+birdX+27)	%741-50,	birdY-13,	bird[birdCounter],			XOR_PUT);
		putimage((420+birdX)	%741-50,	birdY+10,	bird[((birdCounter+1)%3)],	XOR_PUT);
		putimage((420+birdX+35)	%741-50,	birdY+30,	bird[((birdCounter+2)%3)],	XOR_PUT);
		
		//Obstacle
		putimage(obstacleOneX,	obstacleY,	obstacleImage,	XOR_PUT);
		putimage(obstacleTwoX,	obstacleY,	obstacleImage,	XOR_PUT);
		
		//Showing Score
		itoa(boyCounter,scoreString,10);
		
		settextstyle(BOLD_FONT,0,3);
		outtextxy(floorX + scoreBoardX + 200, floorY + scoreBoardY+23,scoreString);
				
		//Game Over Logic
		if( ( obstacleOneX <= ( boyX + 19 ) && ( obstacleOneX + 25 ) >= ( boyX + 6 ) ) ||
				( obstacleTwoX <= ( boyX + 19 ) && ( obstacleTwoX + 25 ) >= ( boyX + 6 ) ) )				
				{
			
				if( (obstacleY <= ( boyY + 53 )) || ( obstacleY <= ( boyY + 53 )) ){
					 
					delay(400);
					//mario Die Sound
					pthread_create(&marioDieSoundThread,NULL,marioDieSoundThreadFunction,(void *)0);    	
										
					gameOverFlag = true;
					gameOverFunction(boyX,boyY,floorX,floorY,birdX,birdY,
										obstacleOneX,obstacleTwoX,obstacleY,whiteImageFlag,
										birdCounter,boyCounter,scoreBoardX,scoreBoardY);
				}
			
		}
		
		
		//*** Delay ***		
		if(boyCounter % 150 == 0)
			delayIncrementor+=2;
			
		delay(MAIN_DELAY - delayIncrementor);
		
		//*** Counter Increment ***		
		boyCounter++;
		
		//cout<<"\nBoy Counter : "<<boyCounter;
	}

	//Play again condition
	if(playAgain)
		goto StartFromInitialLable;

	closegraph();
	
	//Freeing Bird Image
	for(int i=0;i<3;i++){
		free(bird[i]);
	}
	
	//Freeing Boy Image
	for(int i=0;i<3;i++){
		free(boy[i]);
	}
	
	//Freeing White Image
	free(whiteImage);
	
	//Freeing fllor Image
	free(floorImage);
	
	//Freeing fllor Image
	free(floorImage);
	
	return 0;
}











// *** Function Implementation ***

void startPage(){
	
	cleardevice();
	
	//Welcome Message
	settextstyle(BOLD_FONT,0,5);
	outtextxy(190,70,"WELCOME TO");
	outtextxy(100,140,"*** JUM-MARIO ***");

	settextstyle(BOLD_FONT,0,4);
	outtextxy(0,210,"---------------------------------------------------------------------");
	
	//Author credits
	settextstyle(COMPLEX_FONT,0,4);
	outtextxy(210,260,"CREATED BY");
	outtextxy(130,320,"NISARG PETHANI - 45");
	outtextxy(160,360,"KUSHAL SHAH - 54");
	
	putimage(0,0,whiteImage,XOR_PUT);
	
	PlaySound(TEXT("startSound.wav"),NULL,SND_SYNC); 
	
	//Playing option
	settextstyle(BOLD_FONT,0,2);
	outtextxy(180,420,"PRESS ANY KEY TO START");
	
	delay(100);
	
	getch();
}


void gameOverFunction(int boyX,int boyY,int floorX,int floorY,
						int birdX,int birdY,int obstacleOneX,int obstacleTwoX,int obstacleY,
						bool whiteImageFlag,int birdCounter,int boyCounter,
						int scoreBoardX,int scoreBoardY){
	
	int jumpCounter = 0;
	int jumpHeight = 50;
	int boyUpDownIncrement = 7;
	int boyImageType = STANDING_BOY;
	
	int gameOverBoardX=200;
	int gameOverBoardY=170;
	
	int depth = getmaxy() - floorY + 100; 
	
	bool jumpFlag = true;
	
	while(jumpFlag){
		
		jumpCounter+=boyUpDownIncrement;
		boyY-=boyUpDownIncrement;
		
		//*** Generating Screen ***
		//White Image
		if(whiteImageFlag)
			putimage(0,0,whiteImage,OR_PUT);
		else
			cleardevice();		
		
		//Floor
		putimage(floorX,floorY,floorImage,XOR_PUT);
		
		//boy 
		putimage(boyX,boyY,boy[boyImageType],XOR_PUT);
		
		//birds
		putimage(birdX+27,	birdY-23,	bird[birdCounter],			XOR_PUT);
		putimage(birdX,		birdY,		bird[((birdCounter+1)%3)],	XOR_PUT);
		putimage(birdX+30,	birdY+20,	bird[((birdCounter+2)%3)],	XOR_PUT);
		putimage(birdX+60,	birdY+45,	bird[birdCounter],			XOR_PUT);
		
		putimage((420+birdX+27)	%741-50,	birdY-13,	bird[birdCounter],			XOR_PUT);
		putimage((420+birdX)	%741-50,	birdY+10,	bird[((birdCounter+1)%3)],	XOR_PUT);
		putimage((420+birdX+35)	%741-50,	birdY+30,	bird[((birdCounter+2)%3)],	XOR_PUT);
		
		//Obstacle
		putimage(obstacleOneX,	obstacleY,	obstacleImage,	XOR_PUT);
		putimage(obstacleTwoX,	obstacleY,	obstacleImage,	XOR_PUT);
		
		//GameOverBoardImage
		putimage(gameOverBoardX,	gameOverBoardY,	 gameOverBoardImage,	XOR_PUT);
				
		//Showing Score
		itoa(boyCounter,scoreString,10);
		
		settextstyle(BOLD_FONT,0,3);
		outtextxy(floorX + scoreBoardX + 200, floorY + scoreBoardY+23,scoreString);
				
		delay(50);
		
		if(jumpCounter>=jumpHeight){
			boyUpDownIncrement=-boyUpDownIncrement;
		}
		
		if(jumpCounter<=(-depth)){
			jumpFlag = false;
			
			lastPageScore(boyCounter);
		}
		
	}	
}


void lastPageScore(int boyCounter){
	
	cleardevice();
	
	//GAme over Message
	settextstyle(BOLD_FONT,0,5);
	outtextxy(88,80,"*** GAME  OVER ***");
	
	char scoreString[10] = "";
	
	//Score display
	itoa(boyCounter,scoreString,10);
	settextstyle(BOLD_FONT,0,4);
	outtextxy(190,150,"SCORE: ");
	outtextxy(370,150,scoreString);
	outtextxy(0,200,"-------------------------------------------------------------------");
	
	//Thank you message
	settextstyle(BOLD_FONT,0,6);
	outtextxy(170,270,"THANK YOU");
	
	putimage(0,0,whiteImage,XOR_PUT); // inverting color
	
	PlaySound(TEXT("gameOverSound.wav"),NULL,SND_SYNC);  
		
	//Play again option
	settextstyle(BOLD_FONT,0,3);
	outtextxy(145,360,"PRESS \'Y\' TO PLAY AGAIN");
	
	char inputCharacter = getch();
    if(inputCharacter=='Y' || inputCharacter=='y')
    {
		playAgain=true; 
    }
}


void * getFloorImage(int floorY){
	
	cleardevice();
	
	//Screen size : 631 X 467
	int x=getmaxx();
	int y=0;
	
	int scoreBoardX = 158;
	int scoreBoardY = 25;	
	
	int floorHeight = getmaxy() - floorY;
	
	line(0,y,x,y);
	
	//*** Setting Score Board ***
	//Outer rectangles
	rectangle(scoreBoardX+0,	scoreBoardY+0,	scoreBoardX+315,	scoreBoardY+70);
	rectangle(scoreBoardX+5,	scoreBoardY+5,	scoreBoardX+310,	scoreBoardY+65);
	
	//filling rectangles
	setfillstyle(1,15);
	floodfill(scoreBoardX+1,	scoreBoardY+1,	15);
	
	//Text in score board
	settextstyle(BOLD_FONT,0,3);
	outtextxy(scoreBoardX+70,	scoreBoardY+23,"SCORE: ");
	
	setfillstyle(3,15);		
	floodfill(5,5,15);
	
	void *floorImage = malloc(imagesize(0,y,x+1,y+1+floorHeight));
	getimage(0,y,x+1,y+1+floorHeight,floorImage);
	
	return floorImage;
}


void * getBirdImage(int x,int y,int wingType){
	
	cleardevice();
	
	printBird(x,y,wingType);
	
	//Bird size 50 X 39
	void *bird = malloc(imagesize(x-1,y-1,x+1+50,y+1+39));
	getimage(x-1,y-1,x+1+50,y+1+39,bird);
	
	return bird;
	
}

void printBird(int x,int y,int wingType){
	
	//***   Bird   ***
	//Head
	circle(x+10,y+18,5);
	
	//Body
	line(x+14,y+16,x+35,y+16);
	arc(x+24,y+16,205,360,11);
	
	//Tail
	line(x+34,y+18,x+50,y+18);
	line(x+33,y+22,x+50,y+19);
	
	//Beak
	line(x+0,y+18,x+6,y+16);
	line(x+0,y+18,x+6,y+20);
	
	//Eye
	putpixel(x+9,y+17,15);
	circle(x+9,y+17,1);

	//Wing
	line(x+18,y+19,x+30,y+19);
	
	switch(wingType){
		case 0:
			line(x+18,y+19,x+27,y+0);
			line(x+30,y+19,x+27,y+0);

			break;

		case 1:		
			
			line(x+18,y+19,x+36,y+23);
			line(x+30,y+19,x+36,y+23);

			break;
			
		case 2:
			line(x+18,y+19,x+27,y+39);
			line(x+30,y+19,x+27,y+39);

			break;

	}
}


void * getBoyImage(int x,int y,int legType){
	
	cleardevice();
	
	printBoy(x,y,legType);
	
	//Boy image size : 25 X 52
	void *boy = malloc(imagesize(x-1,y-1,x+1+25,y+1+52));
	getimage(x-1,y-1,x+1+25,y+1+52,boy);
	
	return boy;
	
}


void printBoy(int x,int y,int legType){
	
	//***   Boy   ***
	//Head
	circle(x+13,y+7,7);
	
	//Cap
	line(x+6,y+6,x+25,y+3);	
	line(x+6,y+7,x+25,y+4);
	floodfill(x+13,y+2,15);

	//Body 
    rectangle(x+5,y+15,x+20,y+35);
    line(x+12,y+15,x+12,y+22);
    line(x+5,y+33,x+20,y+33);
	
	//Hand
    rectangle(x+0,y+17,x+5,y+27);
    rectangle(x+20,y+17,x+25,y+27);
	
	//Leg
	switch(legType){
		
		//left leg up
		case 0: 
			rectangle(x+6,y+35,x+11,y+42);
		    rectangle(x+14,y+35,x+19,y+52);
		    
			//Foot
		    putpixel(x+12,y+42,15);
		    putpixel(x+20,y+52,15);
		    putpixel(x+21,y+52,15);

			break;

		//Right leg up		
		case 1:	
		    rectangle(x+6,y+35,x+11,y+52);
		    rectangle(x+14,y+35,x+19,y+42);

			//Foot
		    putpixel(x+12,y+52,15);
		    putpixel(x+20,y+42,15);
		    putpixel(x+21,y+42,15);
		    
			break;
			
		//Jumping leg / Standing leg
		case 2:
			rectangle(x+6,y+35,x+11,y+52);
		    rectangle(x+14,y+35,x+19,y+52);

			//Foot
		    putpixel(x+12,y+52,15);
		    putpixel(x+20,y+52,15);
		    putpixel(x+21,y+52,15);
		    
			break;

	}
}

void * getWhiteImage(){
	cleardevice();
	
	setcolor(15);
	floodfill(5,5,15);
	
	//Screen size : 631 X 467
	int x=getmaxx();
	int y=getmaxy();
	
	void *whiteImage = malloc(imagesize(0,0,x,y));
	getimage(0,0,x,y,whiteImage);
	
	return whiteImage;
}

void * getObstacleImage(int x,int y,int obstacleType){
	
	cleardevice();
	
	printObstacle(x,y,obstacleType);
	
	//Boy image size : 25 X 25
	void *obstacle = malloc(imagesize(x-1,y-1,x+1+25,y+1+25));
	getimage(x-1,y-1,x+1+25,y+1+25,obstacle);
	
	return obstacle;
	
}


void printObstacle(int x,int y,int obstacleType){
	
	//***   Obstacle   ***	
	//Head
	rectangle(x+0,y+0,x+25,y+5);
	
	//Body
    rectangle(x+3,y+5,x+21,y+21);
	
	//Fill Color
	setfillstyle(1,15);
	floodfill(x+1,y+1,15);
	floodfill(x+4,y+6,15);
	
		
}

void * getGameOverBoardImage(int x,int y){
	
	cleardevice();
	
	printGameOverBoard(x,y);
	
	//Boy image size : 225 X 35
	void *gameOverBoard = malloc(imagesize(x-1,y-1,x+1+225,y+1+35));
	getimage(x-1,y-1,x+1+225,y+1+35,gameOverBoard);
	
	return gameOverBoard;
	
}

void printGameOverBoard(int x,int y){
	
	//*** Setting GAME OVER Board ***
	
	//Changing to LIGHTRED[12] For White BackGround
	setcolor(15-(12));
			
	//Text in score board
	settextstyle(BOLD_FONT,0,5);
	outtextxy(x,	y,"GAME OVER");
	
	//Resetting color
	setcolor(15);
}
