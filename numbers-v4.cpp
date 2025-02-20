#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <ncurses.h>
#include <algorithm>

#define LEN 40   // Max length of the number list
#define SPEED 5  // Probability factor (1-10)

#define INITIAL_MIN_INTERVAL 1000  // Initial spawn rate range (in ms)
#define INITIAL_MAX_INTERVAL 3000
#define FINAL_MIN_INTERVAL 300     // Final (hardest) spawn rate range
#define FINAL_MAX_INTERVAL 1200

#define MAX_SCORE_FOR_HARD_MODE 20 // Score threshold for hardest difficulty

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
    std::uniform_int_distribution<int> numberDist;
    std::uniform_int_distribution<int> spawnIntervalDist;
    
    std::chrono::steady_clock::time_point nextSpawnTime;

public:
    Game() 
        : userInput(0), score(0), currentNumber(0), rng(std::random_device{}()),
          numberDist(0, 9) {
        srand(time(nullptr));
        initscr();
        curs_set(FALSE);
        noecho();
        timeout(10); // Non-blocking input
        updateSpawnRate(); // Set initial spawn rate
        resetSpawnTime();
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
            checkSpawnTime();
            draw();
            handleInput();
        }
    }

    void checkSpawnTime() {
        auto now = std::chrono::steady_clock::now();
        if (now >= nextSpawnTime) {
            buffer.push_back(numberDist(rng));
            resetSpawnTime();
        }
    }

    void resetSpawnTime() {
        updateSpawnRate(); // Adjust difficulty dynamically
        nextSpawnTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(spawnIntervalDist(rng));
    }

    void updateSpawnRate() {
        // Interpolate based on score: gradual shift from easy to hard
        float factor = std::min(1.0f, static_cast<float>(score) / MAX_SCORE_FOR_HARD_MODE);

        int minInterval = static_cast<int>(INITIAL_MIN_INTERVAL * (1 - factor) + FINAL_MIN_INTERVAL * factor);
        int maxInterval = static_cast<int>(INITIAL_MAX_INTERVAL * (1 - factor) + FINAL_MAX_INTERVAL * factor);

        spawnIntervalDist = std::uniform_int_distribution<int>(minInterval, maxInterval);
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
            updateSpawnRate(); // Update difficulty when scoring
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}

