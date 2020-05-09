#include <AT89X51.H>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define RS P3_7  //set LCD RS
#define RW P3_6  //set LCD RW
#define E P3_5 //set LCD E
#define DB P2  //set LCD DB

unsigned char object1[]="               "
, object2[]="A              ";
unsigned char tree = 'T';
int score=0;


void delayms(unsigned int time)
{
  unsigned int n;
  while(time>0)
  {
    n=120;
    while(n>0) n--;
    time--;
  }
}

void wrins(char instruction)
{
  RS=0;
  RW=0;
  E=0;
  delayms(1);
  E=1;
  DB=instruction;
  delayms(1);
  E=0;
  delayms(8);
}

void init()
{
  delayms(30);
  wrins(0x38);
  wrins(0x38); //set LED module to display 2 line, 8-bit, 5*7 word
  wrins(0x38);
  wrins(0x38);
  wrins(0x08); //displayer off
  wrins(0x01); //clear displayer
  wrins(0x06); //get each 1bit data, AC+1
  wrins(0x0c); //displayer on, without display cursor
}

void wrdata(char d)
{
  RS=1;
  RW=0;
  E=0;
  delayms(1);
  E=1;
  DB=d; //deliver data
  delayms(1);
  E=0;
  delayms(1);
}

void position(char line, column)
{
  unsigned char instruction;
  line--;
  column--;
  instruction = 0x80+(0x40*line+column);  //compute the instruction code of the position
  wrins(instruction); //deliver instruction code into LCD module
}

void display(char *string)
{
  char k=0;
  while(string[k] != 0x00)
  {
    wrdata(string[k]);
    k++;
  }
}
void MSDelay(unsigned int itime)
  {
    unsigned int i,j;
    for (i=0;i<itime;i++)
	for (j=0;j<121;j++);
  }

void voice() //game over voice
{
  int i, j;
  for (i=0; i<10; i++)
  {
    for(j=0; j<10; j++)
    {
      P3=0x55;
      MSDelay(1);
      P3=0xAA;
      MSDelay(1);
    }
  }

}
void gameover()
{

  /*unsigned char scorec[16];
  scorec[8]= 0x30+score%0x10;
  score= score/0x10;
  scorec[7]= 0x30+score%0x10;
  score=score/0x10;
  scorec[6]= 0x30+score;*/
  position(2,1);
  display("       QAQ      ");
  //display(scorec);
  position(1,1);
  display("    Game Over   ");
  while(1);
}

void play()
{
  int a, b, x, y=0;
  for(;;)
  {
    for(x=0; x<15; x++)
    {
      object1[x]=object1[x+1];
      object2[x]=object2[x+1];
    }
    object2[0] = 0xEF;
    if(P0_0==1)
    {
      object1[0] = 0xEF;
      object2[0] = ' ';
    }
    else if(P0_0==0)
    {
      object1[0] = ' ';
      object2[0] = 0xEF;
    }
    object1[15] = ' ';
    object2[15] = ' ';
    if(object1[14]==tree||object2[14]==tree)
    goto RE; //to avoid tree appear sequential
    a = (rand()%5)+1;
    if(a==1)
      object2[15]=tree;
    else
    {
      object2[15]=' ';
      b = (rand()%11)+1;
      if(b==1)
        object1[15]=tree;
      else
        object1[15]=' ';
    }
    RE:
    position(1,1);
    display(object1);
    position(2,1);
    display(object2);
    delayms(1000);
    if((object1[0]==0xEF&&object1[1]==tree)||(object2[0]==0xEF&&object2[1]==tree))
      y++; //Too avoid the bug, so the game will fail when we meet the barrier second time.
    if(y==2)// if we meet the barrier 2 times, than game over.
    {
      voice();
      gameover();
    }
    if(object1[1]==tree||object2[1]==tree)
    {
      score=score+0x20;//count score(but we fail to display it)
    }
  }
}

void main()
{
  init();
  position(1,1);
  display(object1);
  position(2,1);
  display(object2);
  srand(0);
  play();
  while(1);

}
