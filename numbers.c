#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

#define LEN 40  // max length of the number list
#define SPEED 3 // between 1 and 10

#define INCREASE 'x'
#define DECREASE 'z'
#define KILL     ' '

char ch, line[LEN+2];
unsigned int score = 0;
unsigned short buffer[LEN], i, N=0, c=0;

void draw(void) {
    for(i=0; i<N; i++)
      line[i] = '0' + buffer[i];
    line[N] = '\0';
    
    clear(); 
    refresh();
    printw("Score: %d\n%c %40s\n", score, c+'0', line);
    printw("\n'%c' to increase, '%c' to decrease.\n",INCREASE,DECREASE);
    printw("Space to kill the number at the beginning of the list.\n");
    printw("'q' ends the game.\n");
    endwin();
}

void loop(void) {
  do {
    if(N == 0 || (rand()%10)<SPEED)
      buffer[N++] = rand()%10;
   
    draw();

    ch = getch();
    switch(ch) {
      case INCREASE: c = (c+1)%10; break;
      case DECREASE: if(c>0) c--; else c=9; break;
      case KILL    : if(buffer[0] == c && N > 0 ) {
			score += 1;
			for(i=0; i<N; i++) 
			   buffer[i] = buffer[i+1];
			buffer[N]=0;
			N--;
		     }; break;
      default      : break;
    }
  } while(ch!='q' && N < LEN);
}  


int main(void) {
  // init the game
  srand(time(NULL));
  initscr();
  curs_set(FALSE);
  
  // main loop
  loop();

  // finish the game
  clear(); 
  refresh();
  endwin();
  printf("Thanks for playing.\n");
  exit(0);
}
