#include <string.h>	
#include <conio.h>
#include <stdbool.h>
#include <peekpoke.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <joystick.h>
#if defined __C64__
#include <c64.h>
#elif defined __C128__
#include <c128.h>
#endif
#define SCREENCOLS 40
#define SCREENROWS 25
#define UPPER_ROW 4
#define LOWER_ROW 22
#define SHIP_ROW 20
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define ALIENS 18
#define MAXALIENFIRE 5
#define MAXALIENPODS 4
#define ALIENROW 9
#define BEAM_COLOR 4
#define POWERBEAM_COLOR 10
#define SHIP_COLOR 1
#define ALIEN_COLOR 14
#define BONUS_COLOR 7
#define LEFT 65
#define RIGHT 68
#define FIRE 83
#define STRIFE 2
typedef unsigned char byte;
void drawSprite(byte code, byte len, int startx,int starty, byte color);
void delSprite(byte len, int startx,int starty);
void shipFire(void);
void alienFire(int pos);
void alienPodRelease(int pos);
void spawnAliens(int number);
void alienAdvancing(void);
void shipDestroyed(void);
void petsciiDraw(unsigned char *array, int arrSize, int width);
void removeElement(int pos, int arrayXY[], int count);
void removeAlien(int x);
void redefineChar(void);
void resetFlags(void);
void resetBonus(void);
void c64CopyChars(void);
void c64loadCustomChars(void);
void gameTitle(void);
void play(void);
void updateHud(void);
void shieldsUp(void);
void shieldBomb(void);
void bossAwaken(void);
void bossBeam(int pos);
void bossDestroy(void);
void endGame(void);
void showHiscore(void);
void playNote(int freq, int ad, int sr, int amp );
void silence(int voice);
#include "sounds.c"
#include "levelmap.c"
#include "spritedata.c"

const byte alienColor[] = {14,10,7,4};
const byte alienSound[] = {0x4FB,0x647,0x598,0x547};
char* diffLevel[4] = {" novice ", " medium ", "advanced", "extreme!"};
int alienSoundPos=0;
byte upperBorder = UPPER_ROW;
byte alienPodsXY[MAXALIENPODS*2] = {0};
byte alienLaserXY[MAXALIENFIRE*2] = {0};
byte bossBeamXY[2]={0};
byte destroyedAlien[2] = {0};
bool alienCycle = 0;
byte bonusShot[2] = {0};
int bonusType[]={80,68,70,83};
unsigned int shields = 3;
int shieldHP = 100;
byte shieldColor;
int bonusTimer;
int soundTimer;
int stateTimer = 0;
int bonus;
int direction=1;
byte level=0;
int lives;
byte difficulty = 1;
int alienData[ALIENS*4] = {0};
int alienSprite[] = {64,72,80,88};
int bossCoord[2] = {0};
int alienCount = ALIENS;
int alienActualShots=MAXALIENPODS;
unsigned int ship_x = 19;
unsigned int alien_y;
unsigned int firex,firey;
bool shotFired = false;
bool alienPodFired = false;
bool alienFired = false;
bool bonusFired = false;
bool powerShot = false;
bool doubleShot = false;
bool hyperShotMode = false;
bool fastShot = false;
bool bossLevel = false;
bool bossAwakened = false;
bool bossCritical = false;
bool shielded = false;
bool beamWeak = false;
bool bossBeamFired = false;
int strife = 0;
byte flickerShield = 0;
int bossBeamTimer = 0;
int y=1;
byte alienAdvance = 0, alienHP = 3;
byte bossHP=10;
int balance;
int	alienShotsTimer=0;
byte frontLine;
unsigned int shotCounter = 0, killCounter = 0;
byte hyperCounter = 0;
bool shipDown = false;
bool gameOver = false;
unsigned long score;
unsigned long hiscore = 50000u,bonusScore = 0;
unsigned long scoreTable[]={50000u,40000u,30000u,20000u,10000u};
char*scoreNames[]={"gui","gui","gui","gui","gui"};
unsigned char J,oldJ;
extern char c64_stdjoy;
//extern int setupAndStartPlayer();
#include "gfx.c"
void main(void){
	
	int c;
	bgcolor(0);
	bordercolor(0);
	textcolor(15);
	//POKE (53272u, 21); // set uppercase/gfx
	//POKE (650,128);
	//redefineChar();	

	joy_install(&c64_stdjoy);
	start:
	clrscr();
	c64CopyChars();
	gameOver = false;
	shipDown = false;
	resetFlags();
	resetBonus();
	//alienCount = sizeof((levelMap[level-1])/sizeof(byte))/3;
	alienCount = ALIENS;
	score = 0;
	lives = 5-difficulty;
	level  = 0;
	//setupAndStartPlayer();
	gameTitle();
	clrscr();
	c64loadCustomChars();
	shields = 4-difficulty;
	shieldHP = 0;
	//testChars();
	do{
		if (!shipDown)	++level;
		//alienCount=(sizeof(levelMap[level-1])/sizeof(byte))/3;
		alienCount = ALIENS;
		resetFlags();
		shipDown=false;	
		bossLevel = (level==10)? true:false;
		balance=(level/2)*difficulty;
		bossHP = balance+20;
		upperBorder = (level < 10) ? UPPER_ROW : 8;
		
		for (c=0; c<MAXALIENPODS; c++) alienPodsXY[c*2] = 0;
		spawnAliens(ALIENS);
		
		frontLine = wherey();
		if (level <= 10){
			clrscr();
			gotoxy(11,10);
			revers(1);
			if (!bossLevel) cprintf("level %d start",level);
			else cputs ("prepare yourself!");
			gotoxy(14,12);
			cprintf("ships %d",lives);
			revers(0);
			for (c=0; c<8500; c++){}
			while(1){
			J = joy_read(JOY_2);
			if (kbhit() || JOY_BTN_1(J)) break; 	
				}
			clrscr();
			if (!bossLevel && level%3 == 1) petsciiDraw(frame0001, ARRAY_SIZE(frame0001), 40);
			else if (level%3 == 2) petsciiDraw(frame0002, ARRAY_SIZE(frame0002), 40);
			else if (level%3 == 0) petsciiDraw(frame0003, ARRAY_SIZE(frame0003), 40);
			else if (bossLevel) petsciiDraw(frame0005,ARRAY_SIZE(frame0005), 40);
			
		}
		
		if (level <= 10) play();
		else endGame();
	
	if (gameOver) goto start;
	} while (!gameOver);
	joy_uninstall();
	
}
 

void updateHud(void){
	if (score > hiscore) hiscore = score;
	revers(1);
	gotoxy(2, 0); cprintf("%6lu hi %6lu ships %d shield %d %3d%%", score, hiscore, lives, shields, shieldHP);
	revers(0);
}


void play(void){
	
	int c=0, m=0;
	byte shotsTimer = 0;
	byte oldcolor;
	stateTimer = 0;
	alienShotsTimer=0;
	shipDown = false;
	bonusFired = false;
	bonusTimer = rand()%1500+200*difficulty;
	//alienCount = ALIENS;
	bossAwakened = false;
	bossCritical = false;
	updateHud();
	
	for (c=0; c< alienCount; c++){
			//drawSprite(68+(alienData[c*4+3]-1)*8+alienCycle*4,4,alienData[c*4],alienData[c*4+1], alienColor[alienData[c*4+2]-1]);
			drawSprite(alienSprite[alienData[c*4+3]-1]+alienCycle*4,4,alienData[c*4],alienData[c*4+1], alienColor[alienData[c*4+2]-1]);
			}
	//if (bossLevel) upperBorder = 8;
	bossHP = balance+20;
	//while( alienCount > 0 && !shipDown){
		while(1){
		
		if (shieldHP <=0) {shieldHP=0;shielded = false;}
		if (!bossLevel && alienCount == 0) break;
		if (bossLevel && bossHP == 0) break;
		if (shipDown) break;
		++stateTimer;
		++alienShotsTimer;
		++shotsTimer;
		//++soundTimer;
		if (bossAwakened) ++bossBeamTimer;
		if (bossBeamTimer==120-20*difficulty) {
			//bossBeamTimer = 0;
			bossBeamSound();
			if (ship_x < 14) bossBeam(10);
			else if (ship_x < 20)bossBeam(16);
			else if (ship_x < 26)bossBeam(22);
			else bossBeam(28);
		}
		
		if (bossBeamFired && bossBeamTimer%(4-difficulty)==0){
			oldcolor=textcolor(4);
			revers(1);
			
			gotoxy(bossBeamXY[0]-2,++bossBeamXY[1]);cputc(230);cputc(224);cputc(224);cputc(224);cputc(230);
			revers(0);
			textcolor(oldcolor);
			if (bossBeamXY[1] > 9) cputsxy(bossBeamXY[0]-2,bossBeamXY[1]-3,"     ");
			if ((bossBeamXY[0]>=ship_x-2 && bossBeamXY[0]<=ship_x+2) && (bossBeamXY[1] == SHIP_ROW || bossBeamXY[1] == SHIP_ROW+1))
				{
				SID.v1.ctrl  = 0x10;
				shipDestroyed();
				bossBeamFired = false;
				bossBeamXY[0]=0;
				bossBeamXY[1]=0;
				//goto winOrLose;
				
			}
			if (bossBeamXY[1] == SHIP_ROW+1) {
				SID.v1.ctrl  = 0x10;
				cputsxy(bossBeamXY[0]-2,SHIP_ROW-1,"     ");
				cputsxy(bossBeamXY[0]-2,SHIP_ROW,"     ");
				cputsxy(bossBeamXY[0]-2,SHIP_ROW+1,"     ");
				bossBeamFired = false;
				bossBeamXY[0]=0;
				bossBeamXY[1]=0;
				
				
			}
		}
		if (bossBeamTimer>=250-15*difficulty) bossBeamTimer=0;
		
		drawSprite((shielded&&flickerShield==1)? 41:33+alienCycle*4,4,ship_x,SHIP_ROW, (shielded) ? shieldColor :SHIP_COLOR);
		flickerShield = !flickerShield;
		
		if (alienShotsTimer % 10 - difficulty == 0){
			
			for (c=0; c<MAXALIENFIRE; c++){
			
				if (alienLaserXY[c*2] !=0){
					delSprite(1, alienLaserXY[c*2],alienLaserXY[c*2+1]);
					
					if (++alienLaserXY[c*2+1]<SCREENROWS-3) drawSprite(58,1, alienLaserXY[c*2],alienLaserXY[c*2+1], BEAM_COLOR);
					else {
						alienLaserXY[c*2] = 0;
						alienLaserXY[c*2+1] = 0;
						}
				}
				else if ((c<alienCount) || bossLevel&&bossAwakened) alienFire(c*2);		
				
			}
			
		}
		
		if (alienShotsTimer >= 20 - balance){
			alienShotsTimer=0;
			
			if (destroyedAlien[0] != 0){
				delSprite(4,destroyedAlien[0],destroyedAlien[1]);
				destroyedAlien[0] = 0;
				destroyedAlien[1] = 0;
				
			}
			
			for (c=0; c<difficulty+1; c++){
			
				if (alienPodsXY[c*2] !=0){
					delSprite(1, alienPodsXY[c*2],alienPodsXY[c*2+1]);
					if (balance>2 && alienPodsXY[c*2+1] >= SHIP_ROW-balance/3){
							if (alienPodsXY[c*2] > ship_x && alienPodsXY[c*2]-ship_x <= (balance)/3+1) --alienPodsXY[c*2];
							else if (alienPodsXY[c*2] < ship_x && ship_x - alienPodsXY[c*2] <= (balance)/3+1) ++alienPodsXY[c*2];	
						}
					++alienPodsXY[c*2+1];
					if (alienPodsXY[c*2+1]<SCREENROWS-3) drawSprite(59,1, alienPodsXY[c*2],alienPodsXY[c*2+1], BEAM_COLOR);
					else {
						alienPodsXY[c*2] = 0;
						alienPodsXY[c*2+1] = 0;
						}
				}
				else if ((c<alienCount) || bossLevel && bossAwakened) alienPodRelease(c*2);
				
			}
		
		
		}
		
		// Bonus
		
		if (bonusFired && stateTimer%(20-2*difficulty) == 0){
			delSprite(1,bonusShot[0],bonusShot[1]);
			if (bonusShot[1] <= SHIP_ROW){
			++bonusShot[1];
			oldcolor = textcolor(BONUS_COLOR);
			revers(1);
			drawSprite(bonusType[bonus],1,bonusShot[0],bonusShot[1],7);
			revers(0);
			textcolor(oldcolor);
			}
			else {
				bonusShot[0] = 0;
				bonusShot[1] = 0;
				bonusFired = false;
				bonusTimer = rand()%1500+200*difficulty;
			}

		}
		else {
			
			if (!bonusFired && stateTimer % bonusTimer == 0 && alienCount>0){
			c=rand()%alienCount;
			bonusShot[0] = alienData[c*4];
			bonusShot[1] = alienData[c*4+1]+1;
			bonus = rand()%ARRAY_SIZE(bonusType);
			bonusFired = true;
			drawSprite(bonusType[bonus],1, bonusShot[0],bonusShot[1], 7);
			
			}
		}
		
		
		if ((bonusShot[0] == ship_x || bonusShot[0] == ship_x+1 )&& bonusShot[1] == SHIP_ROW && bonusFired){
			delSprite(1,bonusShot[0],bonusShot[1]);
			if (bonus == 0) 	 powerShot = true;
			else if (bonus == 1) doubleShot = true;
			else if (bonus == 2) {shotsTimer=0; fastShot = true;}
			else if (bonus == 3) {++shields;}
			score+=500;
			updateHud();
			bonusSound();
			bonusFired = false;
			bonusTimer = rand()%1500+200*difficulty;
			
		}
		
		//Alien Shot VS Ship
		
		for (c=0; c<MAXALIENFIRE; c++){
			if ((alienLaserXY[c*2] == ship_x || alienLaserXY[c*2] == ship_x+1 )&& alienLaserXY[c*2+1] == 20){
					delSprite(1,alienLaserXY[c*2],alienLaserXY[c*2+1]);
					alienLaserXY[c*2] = 0;
					alienLaserXY[c*2+1] = 0;
					if (!shielded) shipDestroyed();
					else {
						pingSoundHigh();
						shieldHP-=(3*difficulty+1); 
						if (shieldHP<0) shieldHP=0;
						//gotoxy(38,23);
						//cprintf("%2d",shieldHP);
						updateHud();
					}
					break;			
				}
		
		}
		
		
		// Alien Pod VS Ship
		for (c=0; c<MAXALIENPODS; c++){
			if ((alienPodsXY[c*2] == ship_x || alienPodsXY[c*2] == ship_x+1 )&& (alienPodsXY[c*2+1] == 20 || alienPodsXY[c*2+1] == 21)){
					delSprite(1,alienPodsXY[c*2],alienPodsXY[c*2+1]);
					//delSprite(4,ship_x,20);
					alienPodsXY[c*2] = 0;
					alienPodsXY[c*2+1] = 0;
					if (!shielded) shipDestroyed();
					else {
						pingSoundHigh();
						shieldHP-=(4*difficulty+1); 
						if (shieldHP<0) shieldHP=0;
						//gotoxy(38,23);
						//cprintf("%2d",shieldHP);
						updateHud();
					}
					break;			
				}
		
		// Ship shot VS Pod
		if (shotFired && (firex == alienPodsXY[c*2] || (doubleShot && firex+1 == alienPodsXY[c*2] )) && firey == alienPodsXY[c*2+1]+1){
					delSprite(1,alienPodsXY[c*2],alienPodsXY[c*2+1]);
					score+=50;
					//alienPodsXY[c*2] = 0;
					//alienPodsXY[c*2+1] = 0;
					alienPodsXY[c*2] = 0;
					alienPodsXY[c*2+1] = 0;
					//--alienShotsCounter;
					if (!powerShot){
						delSprite(1,firex,firey);
						if (doubleShot) delSprite(1,firex+1,firey);
						firex=0;
						firey=0;
						shotFired = false;
						
					}
					else beamWeak = true;
					
					explosionSound();
					++killCounter;
					alienPodFired = false;
					break;
					}
		}

		
		// Shot VS Alien
		if ((!fastShot && shotsTimer == 3) || (fastShot && shotsTimer == 2)) {
			shotsTimer = 0;
			
			for (c=0; c< alienCount; c++){
				if 
						(((firex == alienData[c*4] 	||
						(firex == alienData[c*4]+1))	||
						(doubleShot && firex+1 == alienData[c*4]) 		||
						(doubleShot && firex+1 == alienData[c*4]+1))	&& 
						(firey == alienData[c*4+1]+1))	{
					
					delSprite(1, firex,firey);
					if (doubleShot) delSprite(1, firex+1,firey);
					
					if (powerShot && !beamWeak) --alienData[c*4+2];
					if (--alienData[c*4+2] <= 0){
					bangSound();
					drawSprite(60,1,alienData[c*4],alienData[c*4+1], BEAM_COLOR);
					drawSprite(60,1,alienData[c*4],alienData[c*4+1]+1, BEAM_COLOR);
					drawSprite(60,1,alienData[c*4]+1,alienData[c*4+1], BEAM_COLOR);
					drawSprite(60,1,alienData[c*4]+1,alienData[c*4+1]+1, BEAM_COLOR);
					destroyedAlien[0] = alienData[c*4];
					destroyedAlien[1] = alienData[c*4+1];
					removeAlien(c*4);
					++killCounter;
					
					}
					else pingSoundHigh();
					beamWeak=false;
					shotFired=false;
					firex=0;
					firey=0;
					
					score+=100+50*difficulty;
					updateHud();
					break;
				}
			} 
			
			
			if (firey < upperBorder ) {
					gotoxy(firex,firey);
					cputc(32);
					if (doubleShot) cputc(32);
					shotFired = false;
					firex=0;
					firey=0;
					hyperCounter = 0;
					hyperShotMode = false;
					}
			else{
			gotoxy(firex,firey);
			cputc(32);
			if (doubleShot) cputc(32);
			--firey;
			gotoxy(firex,firey);
			oldcolor = (powerShot)? textcolor(POWERBEAM_COLOR):textcolor(SHIP_COLOR);
			cputc((powerShot)? 61 : 58);
			if (doubleShot) cputc((powerShot)? 61 : 58);
			textcolor(oldcolor);
			}
			
			
			
		// Shot VS Boss
		
		if (bossLevel && alienCount == 0){
				if (!bossAwakened) bossAwaken();
				if (!bossCritical && bossHP < 10) {
					for (c=160; c<320; c++) POKE(c+55416u,2);
					bossCritical = true;
					balance+=difficulty;
				}
				if (shotFired && firey == upperBorder){
				delSprite(1,firex,firey);
				if (doubleShot) delSprite(1,firex+1,firey);
				shotFired=false;
				if	((firex >= 9 && firex <=11) ||
					(firex >= 15 && firex <=17) ||
					(firex >=21 && firex <= 23) ||
					(firex >=27 && firex <=29)){
						
					--bossHP;
					
					bangSound();
				
				if (firex >= 9 && firex <=11) {cputcxy(10,5,214); for(c=0; c<2000; c++){}; cputcxy(10,5,209);}
				else if (firex >= 15 && firex <=17) {cputcxy(16,5,214); for(c=0; c<3000; c++){}; cputcxy(16,5,209);}
				else if (firex >= 21 && firex <=23) {cputcxy(22,5,214); for(c=0; c<3000; c++){}; cputcxy(22,5,209);}
				else if (firex >= 27 && firex <=29) {cputcxy(28,5,214); for(c=0; c<3000; c++){}; cputcxy(28,5,209);}
				firex=0;
				firey=0;
					score+=300+100*difficulty;
					updateHud();
					//break;
						}
			}
		
		
		}
		}
		
		/*
		if (soundTimer == 3000) {
			soundTimer = 0;
			SID.v2.ctrl = 0x10;
			SID.v2.ad    = 0x39; 
			SID.v2.sr    = 0x6A; 
			SID.amp      = 0x1F; 
			SID.v2.ctrl  = 0x11; 
			SID.v2.freq  = alienSound[alienSoundPos]; 
			if (++alienSoundPos == ARRAY_SIZE(alienSound)) alienSoundPos= 0;
			
		}
		
		if (stateTimer %250 == 0){
			playNote(alienSound[alienSoundPos++],0x27,0xA6,0x1F);
			if (alienSoundPos > ARRAY_SIZE(alienSound)-1) alienSoundPos= 0;
			
		}
		*/
		if (stateTimer % 10 == 0 && alienCount>0) {
			//alienCycle = (alienCycle == 0) ? 1 : 0;
			alienCycle = !alienCycle;
			
			for (c=0; c< alienCount; c++){
			drawSprite(alienSprite[alienData[c*4+3]-1]+alienCycle*4,4,alienData[c*4],alienData[c*4+1], alienColor[alienData[c*4+2]-1]);
			}
		}
		

		
	
		if (stateTimer % (150-5*balance) == 0 && alienCount > 0) {
					if (shielded) shieldHP-=(difficulty+1);
					if (++strife > STRIFE) {strife = 0; direction = -direction; ++alienAdvance;}
					if (alienAdvance==2){alienAdvance=0; alienAdvancing();}
					for (c=alienCount;c>=0; --c) delSprite(4,alienData[c*4],alienData[c*4+1]);
					for (c=0; c<alienCount; c++){
						 
						alienData[c*4]+=direction*2;
						drawSprite(alienSprite[alienData[c*4+3]-1]+alienCycle*4,4,alienData[c*4],alienData[c*4+1], alienColor[alienData[c*4+2]-1]);
					}
			}		
	
	if (shieldHP >=90) shieldColor = 3;
		else if (shieldHP>=30) shieldColor = 7;
		else shieldColor = 10;
		
	if (stateTimer%2 == 0){
	J = joy_read(JOY_2);
	if (JOY_BTN_1(J)){if (!shotFired) shipFire(); }
	else if (JOY_LEFT(J)){if (ship_x>4) {delSprite(4,ship_x,SHIP_ROW);drawSprite(33+alienCycle*4,4,--ship_x,SHIP_ROW, (shielded) ? shieldColor :SHIP_COLOR);} }
	else if (JOY_RIGHT(J)) {if (ship_x<SCREENCOLS-4) {delSprite(4,ship_x,SHIP_ROW);drawSprite(33+alienCycle*4,4,++ship_x,SHIP_ROW, (shielded) ? shieldColor :SHIP_COLOR);} }
	
	else if (JOY_DOWN(J)){if ((shields > 0|| shieldHP>0) && !shielded) shieldsUp();}
	else if (JOY_UP(J)){if (shields > 0 && shielded) shieldBomb();}
	//else if (JOY_UP(J)){if (!shotFired) hyperShotMode=true;}
	}
	
	//cheat codes
	if (kbhit()){
		//key=cgetc();
		if (cgetc() =='q'){gameOver=true; break;}
		else if (cgetc() =='l'){alienCount = 0; break;}
		else if (cgetc() =='i'){++shields; shielded=true; }
		else if (cgetc() =='b'){level = 9; alienCount = 0; break;}
		else if (cgetc() =='m'){powerShot=true;fastShot=true;doubleShot=true;}
		else if (cgetc() =='x'){if (bossLevel){alienCount= 0; bossHP = 1;}}
		//if (key =='p'){while(1){if (kbhit() && cgetc() == 'p') break;}}
	}
		/*
		switch (key){
			case LEFT:
			if (ship_x>2) {delSprite(4,ship_x,20);--ship_x;drawSprite(800,4,ship_x,20, SHIP_COLOR);} 
			break;
			case FIRE:
			if (!shotFired) shipFire(); 
			break;
			case RIGHT:
			if (ship_x<SCREENCOLS-2) {delSprite(4,ship_x,20);++ship_x;drawSprite(800,4,ship_x,20, SHIP_COLOR);} 
			break;

			}

		}
	*/
	}
	//winOrLose:
	if ((!bossLevel && alienCount <= 0) || (bossLevel && bossHP <= 0)){
		if (bossLevel) bossDestroy();
		gotoxy(13,12);
		revers(1);
		cputs("level cleared");
		endLevelSound();
		gotoxy(9,14);
		cputs("accuracy: ");
		if (killCounter == 0) killCounter = 1;
		if (shotCounter == 0) shotCounter = 1;
		for (c=0; c<((killCounter*100)/shotCounter)/20; c++){pingSound();cputc(129); cputc(32);}
		gotoxy(9,16);
		bonusScore = ((killCounter*100)/shotCounter)*100;
		cprintf("bonus points: %lu", bonusScore);
		gotoxy(9,16);
		cprintf("shield recharge: +%d%%",c*15);
		shieldHP+=c*20; if (shieldHP>100) {++shields;shieldHP-=100;updateHud();}
		pingSound();
		score+=bonusScore;
		revers(0);
		for (c=0; c<150;c++){}
		while(1){J = joy_read(JOY_2);
		
			if ((kbhit() && cgetc() == FIRE) || JOY_BTN_1(J)) break;}	
	}
	else if (shipDown){
		gotoxy(13,12);
		revers(1);
		cputs("level failed");
		if (lives-- <= 0) {
			gotoxy(15,14);
			cputs("game over");
			gameOver = true;
		}
		for (c=0; c<550;c++){}
		while(1){J = joy_read(JOY_2);if ((kbhit() && cgetc() == FIRE) || JOY_BTN_1(J)) break;}	
		revers(0);
	}
	
	//if (bossLevel && stateTimer % 300 == 0 && alienCount == 0) spawnAliens(4);
	
	if (stateTimer >= 2000) stateTimer = 0;
}




void shieldsUp(void){
	bonusSound();
	if (shieldHP == 0) {
		--shields;
		shieldHP = 100;
	}
	shielded = true;
	updateHud();
	
}


void spawnAliens(int number){
	int c;
	for (c=0;c<number;c++){
			
			alienData[c*4]   = levelMap[level-1][c*3+1]; // x
			alienData[c*4+1] = levelMap[level-1][c*3+2]; // y
			alienData[c*4+2] = (levelMap[level-1][c*3] >= difficulty+1) ? difficulty+1 : levelMap[level-1][c*3]; // HP
			alienData[c*4+3] = levelMap[level-1][c*3]; // sprite
		}
		alienCount==number;
}


void drawSprite(byte code, byte len, int startx,int starty, byte color){
	byte n, oldcolor;
	oldcolor = textcolor(color);
	gotoxy(startx,starty);
	for (n=0; n<len; n++){
	if (n > 0 && n%(len/2) == 0) gotoxy(startx,starty+1);
	cputc(code+n);
	}
	textcolor(oldcolor);
} 


void shipFire(void){
	firex=ship_x;
	firey=20;
	shotFired = true;
	laserSound();
	//zraySound();
	++shotCounter;
}

void shieldBomb(void){
	int c,d;
	--shields;
	shielded = false;
	for (d=0; d<2; d++){
	bgcolor(shieldColor);
	for (c=0; c<200;c++){}
	bgcolor(0);
	}
	for (c=0; c<MAXALIENFIRE; c++){cputcxy(alienLaserXY[c*2],alienLaserXY[c*2+1],32);alienLaserXY[c*2]=0;alienLaserXY[c*2+1]=0;}
	for (c=0; c<MAXALIENPODS; c++){cputcxy(alienPodsXY[c*2],alienPodsXY[c*2+1],32);alienPodsXY[c*2]=0;alienPodsXY[c*2+1]=0;}
	alienShotsTimer=-40;
	
}

void alienPodRelease(int pos){
	int c;
	if (bossLevel && alienCount == 0) {
		c = rand()%(MAXALIENFIRE);
		//if (c <= alienCount) {
		//	alienPodsXY[pos] = alienData[c*4];
		//	alienPodsXY[pos+1] = alienData[c*4+1]+2;}
		//else {
			alienPodsXY[pos] = rand()%(21)+9;
			alienPodsXY[pos+1] = upperBorder+1;	//}
	}
	else {
		c = rand()%(alienCount);	
		alienPodsXY[pos] = alienData[c*4];
		alienPodsXY[pos+1] = alienData[c*4+1]+2;
		}
}

void alienAdvancing(void){
	int c;
	if (alienCount>0){
	for (c=alienCount; c>=0; --c) delSprite(4,alienData[c*4],alienData[c*4+1]);
		for (c=0; c< alienCount; c++){
				alienData[c*4+1] +=2;
				drawSprite(alienSprite[alienData[c*4+3]-1]+alienCycle*4,4,alienData[c*4],alienData[c*4+1], alienColor[alienData[c*4+2]-1]);

		}
		frontLine=wherey();
		updateHud();
		if (frontLine >= SHIP_ROW) {shipDestroyed();}
	}
	
}

void alienFire(int pos){
	int c;
	if (bossLevel && alienCount == 0) {
		c = rand()%(MAXALIENFIRE);
		//if (c < alienCount) {
		//	alienLaserXY[pos] = alienData[c*4];
		//	alienLaserXY[pos+1] = alienData[c*4+1]+2;}
		//else {
			alienLaserXY[pos] = rand()%((ship_x+4)-(ship_x-4)+1)+(ship_x-4);
			alienLaserXY[pos+1] = upperBorder+1;	//}
		}
	else {
		c=rand()%alienCount;
	alienLaserXY[pos] = alienData[c*4];
	alienLaserXY[pos+1] = alienData[c*4+1]+2; }
	
}

void shipDestroyed(void){
					
					drawSprite(60,1,ship_x,SHIP_ROW, BEAM_COLOR);
					drawSprite(60,1,ship_x,SHIP_ROW+1, BEAM_COLOR);
					drawSprite(60,1,ship_x+1,SHIP_ROW, BEAM_COLOR);
					drawSprite(60,1,ship_x+1,SHIP_ROW+1, BEAM_COLOR);
					failLevelSound();
					if (lives == 0) gameOverSound();
					resetFlags();
					resetBonus();
					shipDown = true;
	
}


void resetFlags(void){
		int c;
		for (c=0; c<MAXALIENFIRE*2; c++){alienLaserXY[c] = 0;}
		for (c=0; c<MAXALIENPODS*2; c++){alienPodsXY[c] = 0;}
		alienPodFired = false;
		alienFired = false;
		shotFired = false;
		firex=0;
		firey=0;
		strife = 0;
		direction = 1;
		shotCounter = 0;
		killCounter = 0;
		bonusScore = 0;
		ship_x = 19;
		
		//alienCount = ALIENS;
}

void resetBonus(void){	
		powerShot = false;
		doubleShot = false;
		fastShot = false;
		shielded = false;
}

void delSprite(byte len, int startx,int starty){
	byte n;
	gotoxy(startx,starty);
	for (n=0; n<len; n++){
	if (n > 0 && n%(len/2) == 0) gotoxy(startx,starty+1);
	cputc(32);
	} 
}

void removeElement(int pos, int arrayXY[], int count){
	byte c;
	for (c=pos; c<(count*2); c+=2){
		arrayXY[c] = arrayXY[c+2];
		arrayXY[c+1] = arrayXY[c+3];
	}
	--alienCount;
}

void removeAlien(int x){
	int c;
	for (c=x; c<(alienCount*4); c+=4){
		alienData[c] = alienData[c+4];
		alienData[c+1] = alienData[c+5];
		alienData[c+2] = alienData[c+6];
		alienData[c+3] = alienData[c+7];
		//levelMap[level-1][c*3] = 0;
	}
	--alienCount;
}

void bossAwaken(void){
	int a,c;
	for (a=0; a<3; a++){
		bgcolor(4);
		for (c=0; c<4500; c++){}
		bgcolor(0);
		for (c=0; c<4500; c++){}
	}
	for (a=0; a<5; a++){
		cputcxy(10,5,209);
		cputcxy(16,5,209);
		cputcxy(22,5,209);
		cputcxy(28,5,209);
		for (c=0; c<4500; c++){}
		cputcxy(10,5,224);
		cputcxy(16,5,224);
		cputcxy(22,5,224);
		cputcxy(28,5,224);
		for (c=0; c<4500; c++){}
	}
		textcolor(4);
		cputcxy(10,5,209);
		cputcxy(16,5,209);
		cputcxy(22,5,209);
		cputcxy(28,5,209);
		bossAwakened = true;
		textcolor(15);
}


void bossBeam(int pos){
	bossBeamFired = true;
	bossBeamXY[0]=pos; 
	bossBeamXY[1]=7;
}


void bossDestroy(void){
	int a,b,c;
	for (a=0; a<5; a++){
		bgcolor(2);
		for (c=0; c<4500; c++){}
		bgcolor(0);
		for (c=0; c<4500; c++){}
	}
	for (c=0; c<50; c++){
		a = rand()% (319-120+1) + 120;
		cputcxy(a%SCREENROWS,a/SCREENROWS,60);
		explosionSound();
	}
	for (c=0; c<220; c++) POKE(55416u+c,11);
	for (c=0; c<50; c++){
		a = rand() % (319-120+1) + 120;
		cputcxy(a%SCREENROWS,a/SCREENROWS,60);
		//for (b=0; b<3000; b++){}
		explosionSound();
	}
	for (c=0; c<50; c++){
		a = rand() % (319-120+1) + 120;
		cputcxy(a%SCREENROWS,a/SCREENROWS,32);
		for (b=0; b<3000; b++){}
	}
	for (c=0; c<220; c++) POKE(c+55416u,0);
	
}


void endGame(void){
	int c;
	clrscr();
	c64CopyChars();
	petsciiDraw(frame0004,ARRAY_SIZE(frame0004), 40);
	gotoxy(17,15);cprintf("%lu", score);
	gameOver = true;
	for (c=0; c<9000;c++){}
	
	do{
		J = joy_read(2);
	}while (!JOY_BTN_1(J));
}

void instructions(void){
	clrscr();
	revers(1);	
	cputsxy(1,0,"[fire]   [down]");
	cputsxy(1,1," shoot   raise");
	cputsxy(1,2,"enemies  shield");
	revers(0);
	
	revers(1);
	textcolor(8); cputsxy(19,1,"p   power beam");
	textcolor(13);cputsxy(19,3,"d   double beam");
	textcolor(14);cputsxy(19,5,"f   fast beam");
	textcolor(7); cputsxy(19,7,"s   shield");
	textcolor(10);
	cputsxy(0,10,"destroy all invaders to clear stage!");
	
	textcolor(1);
	cputsxy(3,15, "\"squid\"  \"yars\"  \"skaro\"  \"moth\"");
	
	
	textcolor(10);
	cputsxy(12,19,"alien weapons");
	textcolor(4);
	revers(0);
	cputcxy(2,21,58);
	revers(1);
	textcolor(15);
	cputsxy (4,21,"lasers (fast)");
	revers(0);
	textcolor(4);
	cputcxy(20,21,59);
	revers(1);
	textcolor(15);
	cputsxy (23,21,"pods (smart)");
	revers(0);
	do{
	if (stateTimer==0 || stateTimer==750){
	revers(0);
	drawSprite((alienCycle)?32:58,1,3,4,SHIP_COLOR);
	drawSprite(33+alienCycle*4,4,3,6,SHIP_COLOR);
	drawSprite((alienCycle)?41:33,4,12,6,3);
	drawSprite (alienSprite[0]+alienCycle*4,4,6,12, alienColor[0]);
	drawSprite (alienSprite[1]+alienCycle*4,4,14,12,alienColor[1]);
	drawSprite (alienSprite[2]+alienCycle*4,4,22,12,alienColor[2]);
	drawSprite (alienSprite[3]+alienCycle*4,4,30,12,alienColor[3]);
	revers(1);
	}
	if (++stateTimer>1499) {stateTimer=0; alienCycle=!alienCycle;}
	J=joy_read(2);
	if (kbhit() || JOY_BTN_1(J)) break;
	}while (1);
	stateTimer = 0;
}

void gameTitle(void){
	int c,diffchoice;
	unsigned int randToken = 1;
	clrscr();
	/*gotoxy(10, 8);
	cprintf("petscii invaders");
	gotoxy(7,12);
	cprintf("press fire to start");
	*/
	
		
	petsciiDraw(frame0000, ARRAY_SIZE(frame0000), 40);
	do{
	
		J = joy_read(JOY_2);
		if (kbhit() || JOY_BTN_1(J)){
			srand(randToken+clock());
			break;	
		} 	
		if (++randToken == 65535u) randToken = 1;
		
	}while(1);
	
	
	
	clrscr();
	c64loadCustomChars();
	instructions();
	clrscr();
	gotoxy(7,10);
	revers(1);
	cputs("select your skill level");
	diffchoice = 1;
	
	gotoxy(12,16);
	cprintf("  %s  ",diffLevel[diffchoice]);
	for (c=0; c<8800; c++){}
	do{
		J = joy_read(JOY_2);
		if (JOY_LEFT(J) && diffchoice > 0) {gotoxy(12,16);cprintf("  %s  ",diffLevel[--diffchoice]);pingSound();for (c=0; c<800; c++){}}
		if (JOY_RIGHT(J) && diffchoice < 3) {gotoxy(12,16);cprintf("  %s  ",diffLevel[++diffchoice]);pingSound();for (c=0; c<800; c++){}}
		if (JOY_BTN_1(J)) break;
		
	} while (!JOY_BTN_1(J));
		bonusSound();
		difficulty = diffchoice+1;
		revers(0);
}

void showHiscore(void){
	int c;
	clrscr();
	revers(1);
	cputsxy(15,4,"star pilots");
	for (c=0; c<ARRAY_SIZE(scoreTable); c++){
		cputsxy(12,c*2+8,scoreNames[c]); cputsxy(19,c*2+8,(char*)scoreTable[c]);
		
		
	}
	revers(0);
}



void c64CopyChars(void){
	*(byte*) 52 = 196;
	*(byte*) 55 = 0; 
	*(byte*) 56 = 196; // high basic mem limit 50176 (=256*196 + PEEK(55) ) and lower!
	//cprintf("%c",142);	cprintf("%c",8);
	
	POKE(56334u, PEEK(56334u) & 254);
	POKE(1,PEEK(1) & 251);

	//memcpy((byte*)51200u,(byte*)53248u,2048);
	//memcpy((byte*)63488u,(byte*)53248u,2048);
	
	memcpy((byte*)51200u,(byte*)53248u,2048);
	memcpy((byte*)52224u,(byte*)53248u,8*88);
	//memcpy((byte*)52224u,(byte*)53248u,1024);
	POKE(1,PEEK(1) | 4);
	POKE(56334u,PEEK(56334u) | 1);
	//POKE(53272u,PEEK(53272u) & 240 | 12);
	POKE(56578u,PEEK(56578u) | 3);
	POKE(53272u,PEEK(53272u) & 240 | 12);
	POKE(56576u,(PEEK(56576u) & 252 ) | 0);	// select VIC bank #3 (49152-65535)
	POKE(53272u,(PEEK(53272u) & 240) | 2);
	
	POKE(53272u,(PEEK(53272u) & 15) | 16);
	//POKE(53272u,(PEEK(53272u) & 15) | 240); //SCREEN MEM AT 64512 (49152+15360)
	
	//POKE(53272u,PEEK(53272u) & 240 | 14); // char memory at 63488 (49152 + 14336)
	POKE(648,196); // set video  hi-byte start address (196*256 = 50176) and higher!
	//POKE(648,252);// set video  hi-byte start address (252*256 = 64512) and higher!
	
}


void c64loadCustomChars(void){
	unsigned int h;
	//unsigned int b = 63488u;
	unsigned int b = 51200u;
	for (h=0; h<ARRAY_SIZE(udg); h++){
	POKE(h+b+(58*8), udg[h]); // from char #58
	}

for (h=0; h<ARRAY_SIZE(udg64); h++){
	POKE(h+b+(0*8), udg64[h]);// from char #64 (64-64 = 0)
	}

for (h=0; h<ARRAY_SIZE(udgB); h++){
	POKE(h+b+(33*8), udgB[h]);// 
	//POKE(h+51968u, udg64[h]); // from char #96
	}
}



void redefineChar(void){
	
	int h;
	*(byte*) 52 = 196;
	*(byte*) 55 = 0; 
	*(byte*) 56 = 196; // high basic mem limit 50176 (=256*196 + PEEK(55) ) and lower!
	POKE(56334u, PEEK(56334u) & 254);
	POKE(1,PEEK(1) & 251);

	memcpy((byte*)51200u,(byte*)53248u,2048);

	POKE(1,PEEK(1) | 4);
	POKE(56334u,PEEK(56334u) | 1);
	POKE(53272u,PEEK(53272u) & 240 | 12);
	POKE(56578u,PEEK(56578u) | 3);
	
	
	POKE(56576u,(PEEK(56576u) & 252 ) | 0);	// select VIC bank #3 (49152-65535)
	POKE(53272u,(PEEK(53272u) & 240) | 2);
	
	POKE(53272u,(PEEK(53272u) & 15) | 16);
	//POKE(53272u,(PEEK(53272u) & 15) | 240); //test
	
	POKE(648,196); // set video  hi-byte start address (196*256 = 50176) and higher!
	
for (h=0; h<ARRAY_SIZE(udg); h++){
	POKE(h+51416u, udg[h]);
	}
for (h=0; h<ARRAY_SIZE(udg64); h++){
	POKE(h+51464u, udg64[h]);// from char #64 to #95
	//POKE(h+51968u, udg64[h]); // from char #96
	}

for (h=0; h<ARRAY_SIZE(udgB); h++){
	POKE(h+52096u, udgB[h]);// from char #112
	//POKE(h+51968u, udg64[h]); // from char #96
	}

}


void petsciiDraw(unsigned char *array, int arrSize, int width){
int l,m,x,y;
int a = (arrSize-2)/2;
x =wherex();
y =wherey();
for (l=0; l < a; l++){
m = array[l+2];
textcolor(array[a+l+2]);
if (width < SCREENCOLS && l%width == 0 ) gotoxy(x,++y);
if (m==94) m=255;
else if (m==95) m=223;
else if (m==255) m=96;
else if (m<32)m+=64;
else if (m<64) m=(m);
else if (m<94) m+=128;
else if(m<128) m+=128;
else if(m<160) m-=128;
else if(m<192) m-=32; 
else if(m<224) m-=64;
else if(m<255) m-=64;

if (m>90 && m < 192) revers(1);
cputc(m);
if (m>90 && m < 192) revers(0);

}
textcolor(15);

}