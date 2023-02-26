#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

const int LENGTHOFGAME = 99;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    // Initialize data members

}

int StudentWorld::init()
{
    // Load the board data file in using Board Class
    ostringstream oss;
    oss << assetPath() << "board0" << getBoardNumber() << ".txt";
    string boardFile = oss.str();

    // Load positions into the board
    Board::LoadResult result = m_board.loadBoard(boardFile);

    if (result != Board::load_success) {
        if (result == Board::load_fail_file_not_found)
            cerr << "Could not find " << boardFile << endl;
        else if (result == Board::load_fail_bad_format)
            cerr << "Board was improperly formatted" << endl;

        return GWSTATUS_BOARD_ERROR;
    }

    // if result == load_success
    else {
        cerr << "Successfully loaded board" << endl;

        // Determine where each actor is supposed to be on board, save in single STL container of pointers
        for (int x = 0; x < SPRITE_WIDTH; x++)
            for (int y = 0; y < SPRITE_HEIGHT; y++) {
                Actor* a;
                //list<Actor*>::iterator it = m_actors.end();

                switch (m_board.getContentsOf(x,y)) {
                case Board::empty:
                    break;
                case Board::player:
                    m_Peach = new Peach(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this);
                    // m_Yoshi = new Yoshi(x, y);
                    a = new BlueCoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this); // Players start on a BlueCoinSquare
                    m_actors.push_back(a);
                    break;
                case Board::blue_coin_square:
                    a = new BlueCoinSquare(x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this); // Players start on a BlueCoinSquare
                    m_actors.push_back(a);
                    break;
                case Board::red_coin_square:
                case Board::up_dir_square:
                case Board::down_dir_square:
                case Board::left_dir_square:
                case Board::right_dir_square:
                case Board::event_square:
                case Board::bank_square:
                case Board::star_square:
                case Board::bowser:
                case Board::boo:
                    break;
                }
            }
    }


    // May keep pointers to the Peach and Yoshi separate. Everything else must be in above container

    // Initialize other member vars (like money in bank)
    m_bankMoney = 0;

    // Convert grid locations to pixels (SPRITE_WIDTH * x1, SPRITE_HEIGHT * y1).


    // Start countdown timer for 
	startCountdownTimer(LENGTHOFGAME);

    return GWSTATUS_CONTINUE_GAME;
}

// GameWorld repeatedly calls this roughly 20 times per second. move() represents a single tick
int StudentWorld::move()
{
    // Ask all active actors to doSomething
    m_Peach->doSomething();
    // m_Yoshi->doSomething();

    list<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->isAlive())
            (*it)->doSomething();
    }

    // Delete any actors that have become inactive/dead during this tick
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if (!(*it)->isAlive()) {
            delete* it;
            (*it) = nullptr;
        }
    }

    // Update the status text on top of the screen
    ostringstream oss;
    oss << "P1 Roll: " << m_Peach->getRoll() << " Stars: " << m_Peach->getStars() << " $$: " << m_Peach->getCoins();
    oss << " | Time: " << timeRemaining() << " | " << "Bank: " << getBankMoney() << " | ";
    // oss << "P2 Roll: " << m_Yoshi->getRoll() << " Stars: " << m_Yoshi->getStars() << " $$: " << m_Yoshi->getCoins();
    
    // if vor, oss << "VOR";
    string gameStats = oss.str();
    setGameStatText(gameStats);


    // If game over
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);

        PlayerAvatar* winner = getWinner();

        // Call setFinalScore
        setFinalScore(winner->getStars(), winner->getCoins());

        // if Peach won
        return GWSTATUS_PEACH_WON;
        // else if Yoshi won
        // 
        // else randomly choose one to return
    }

    // If game not over return
	return GWSTATUS_CONTINUE_GAME;
}


// Function may run twice, so set nullptrs
void StudentWorld::cleanUp()
{
    if (m_Peach != nullptr) {
        delete m_Peach;
        m_Peach = nullptr;
    }
    if (m_Yoshi != nullptr) {
        delete m_Yoshi;
        m_Yoshi = nullptr;
    }

    // Delete other objects
    list<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        Actor* ptr = *it;
        if (ptr != nullptr) {
            delete ptr;
            ptr = nullptr;
        }
    }
}


StudentWorld::~StudentWorld()
{
    cleanUp();
}

PlayerAvatar* StudentWorld::getWinner() {

    // TODO Part 2
    return m_Peach;
}

bool StudentWorld::isValidSquare(int x, int y) {
    // If not perfectly on a square
    if (x % SPRITE_WIDTH != 0 || y % SPRITE_HEIGHT != 0)
        return false;

    // Convert back to 16x16 Board coordinates
    int boardX = x / SPRITE_WIDTH; // Rounds down for int division
    int boardY = y / SPRITE_HEIGHT;
    if (m_board.getContentsOf(boardX, boardY) == Board::empty)
        return false;
    return true;
}