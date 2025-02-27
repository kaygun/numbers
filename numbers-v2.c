#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define LEN 40  // max length of the number list
#define SPEED 5 // between 1 and 10

#define INCREASE 'x'
#define DECREASE 'z'
#define QUIT     'q'
#define KILL     ' '

char ch;
unsigned int score = 0;
unsigned short buffer[LEN], N=0, c='s';

void draw(void) {
    int i;
    
    move(0,0);
    printw("Score: %d\n", score);
    for(i=0;i<LEN+1;i++) printw("-"); printw("\n");
    printw("%c ", c+'0');
    for(i=0;i<LEN-N-1;i++) printw(" "); 
    for(i=0;i<N;i++) printw("%c",buffer[i]+'0'); printw("\n");
    for(i=0;i<LEN+1;i++) printw("-"); printw("\n");
    printw("Space to kill the 1st instance of the number in the list.\n");
    printw("'%c' to increase, ", INCREASE);
    printw("'%c' to decrease, ", DECREASE);
    printw("'%c' ends the game.\n", QUIT);
    refresh();
}

void loop(void) {
  int i,j;
  struct timespec delay = {0, 200000000L};

  do {
    if(N == 0 || (rand()%10) < SPEED)
      buffer[N++] = rand()%10;
   
    draw();

    ch = getch();

    switch(ch) {
      case INCREASE: c = (c+1)%10; 
		     break;
      case DECREASE: if(c>0) 
                       c--; 
		     else 
		       c=9; 
		     break;
      case KILL    : for(j=0; j<N && buffer[j]!=c; j++);
		     if(j<N) { 
		       score++;
		       for(i=j; i<N; i++) 
		          buffer[i] = buffer[i+1];
		       buffer[N--]=0;
		     }; 
		     break;
      default      : break;
    }
    nanosleep(&delay, NULL);
  } while(ch!='q' && N < LEN);
}  


int main(void) {
  // init the game
  srand(time(NULL));
  initscr();
  curs_set(FALSE);
  nodelay(stdscr, TRUE);
  noecho();

  // main loop
  loop();

  // finish the game
  erase(); 
  refresh();
  endwin();
  printf("Thanks for playing.\n");
  exit(0);
}
