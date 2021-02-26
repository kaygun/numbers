Once upon a time, when I was in secondary school, a class-mate brought his new cool gadget 
watch which was a [Casio calculator watch][1].  It was a very cool watch, and cooler still,
it had a very simple game on it.  This is my *approximation* of the game. I think it is
short and simple enough to make any modifications to make it more playable. Do whatever you
may want to it, as long as give credit to the original game and this version. Enjoy!

I wrote this on my linux box. You will need a C compiler and the [ncurses][2] library to
compile.  Use

    gcc -o numbers numbers.c -lncurses

to compile. The defaul configuration is that 'z' decreases, 'x' increases and space cancels.
Use 'q' to quit the game.

[1]: https://www.reddit.com/r/casio/comments/8zem5h/casio_ca_901/
[2]: https://en.wikipedia.org/wiki/Ncurses
