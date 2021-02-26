#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define LEN 40  // max length of the number list
#define SPEED 3 // speed at which numbers generated
                // higher the number, slower the generation

#define INCREASE 'x'
#define DECREASE 'z'
#define KILL     ' '

int main(int argc, char *argv[]) {

  char ch, line[LEN+2];
  unsigned int score = 0;
  unsigned short buffer[LEN], i, N=0, c=0;

  srand(time(NULL));

  initscr();
  keypad(stdscr, TRUE);
  curs_set(FALSE);

  do {
    if(N == 0 || rand()%SPEED == 0) 
      buffer[N++] = rand()%10;

    for(i=0;i<N;i++)
       line[i] = '0' + buffer[i];
    line[N] = '\0';

    clear(); 
    refresh();
    printw("Score: %d\n%c %40s\n", score, c+'0', line);
    printw("\n'%c' to increase, '%c' to decrease.\n",INCREASE,DECREASE);
    printw("Space to kill the number at the beginning of the list.\n");
    printw("'q' ends the game.\n");
    endwin();

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

  clear(); 
  refresh();
  endwin();

  printf("Thanks for playing.\n");
  exit(0);
}
