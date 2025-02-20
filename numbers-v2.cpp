#include <iostream>
#include <vector>
#include <random>
#include <ncurses.h>
#include <ctime>
#include <algorithm> // For std::find

#define LEN 40   // Max length of the number list
#define SPEED 5  // Probability factor (1-10)

#define INCREASE 'x'
#define DECREASE 'z'
#define QUIT     'q'
#define KILL     ' '

class Game {
private:
    char userInput;
    unsigned int score;
    unsigned short currentNumber;
    std::vector<unsigned short> buffer;
    std::mt19937 rng;
    std::uniform_int_distribution<int> numberDist; // Random number (0-9)
    std::uniform_int_distribution<int> spawnChanceDist; // Probability check

public:
    Game() 
        : userInput(0), score(0), currentNumber(0), rng(std::random_device{}()),
          numberDist(0, 9), spawnChanceDist(0, 9) {
        srand(time(nullptr));
        initscr();
        curs_set(FALSE);
        noecho();
    }

    ~Game() {
        clear();
        refresh();
        endwin();
        std::cout << "Thanks for playing.\n";
    }

    void run() {
        gameLoop();
    }

private:
    void draw() const {
        clear();
        printw("Score: %d\n", score);
        printw(std::string(LEN + 1, '-').c_str());
        printw("\n");

        printw("%c ", currentNumber + '0');
        printw(std::string(LEN - buffer.size() - 1, ' ').c_str());

        for (unsigned short num : buffer)
            printw("%c", num + '0');
        
        printw("\n%s\n", std::string(LEN + 1, '-').c_str());
        printw("Space to remove the first occurrence of the selected number.\n");
        printw("'%c' to increase, '%c' to decrease, '%c' to quit.\n", INCREASE, DECREASE, QUIT);
        refresh();
    }

    void gameLoop() {
        while (userInput != QUIT && buffer.size() < LEN) {
            maybeAddNumber();
            draw();
            handleInput();
        }
    }

    void maybeAddNumber() {
        if (buffer.empty() || spawnChanceDist(rng) < SPEED) {
            buffer.push_back(numberDist(rng));
        }
    }

    void handleInput() {
        userInput = getch();
        switch (userInput) {
            case INCREASE:
                currentNumber = (currentNumber + 1) % 10;
                break;
            case DECREASE:
                currentNumber = (currentNumber == 0) ? 9 : currentNumber - 1;
                break;
            case KILL:
                removeFirstOccurrence();
                break;
            default:
                break;
        }
    }

    void removeFirstOccurrence() {
        auto it = std::find(buffer.begin(), buffer.end(), currentNumber);
        if (it != buffer.end()) {
            buffer.erase(it);
            score++;
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}

