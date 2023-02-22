#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

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
    // DELETE
	startCountdownTimer(5);  // this placeholder causes timeout after 5 seconds

    // Load the board data file in using Board Class
    ostringstream oss;
    oss << assetPath() << "board0" << getBoardNumber() << ".txt";
    string boardFile = oss.str();

    // Load positions into the board
    Board::LoadResult result = m_board.loadBoard(boardFile);

    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find " << boardFile << endl;
    else if (result == Board::load_fail_bad_format)
        cerr << "Board was improperly formatted" << endl;
    else if (result == Board::load_success) {
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
                    m_Peach = new Peach(x, y);
                    // m_Yoshi = new Yoshi(x, y);
                    a = new BlueCoinSquare(x, y); // Players start on a BlueCoinSquare
                    m_actors.push_back(a);
                    break;
                case Board::blue_coin_square:
                    a = new BlueCoinSquare(x, y); // Players start on a BlueCoinSquare
                    m_actors.push_back(a);
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

    // Convert grid locations to pixels (SPRITE_WIDTH * x1, SPRITE_HEIGHT * y1).



    return GWSTATUS_CONTINUE_GAME;
}

// GameWorld repeatedly calls this roughly 20 times per second. move() represents a single tick
int StudentWorld::move()
{
    // If game over
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);

        PlayerAvatar* winner = getWinner();

        // Call setFinalScore
        setFinalScore(winner->getStars(), winner->getCoins());

        // if Peach won
        return GWSTATUS_PEACH_WON;
        // else if Yoshi won
        // else randomly choose one to return
    }
    else {

        // Ask all active actors to doSomething
        m_Peach->doSomething();
        // m_Yoshi->doSomething();
        list<Actor*>::iterator it;
        for (it = m_actors.begin(); it != m_actors.end(); it++) {
            // If active TODO NOWW
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

        setGameStatText("TODO");



        // If game not over return
	    return GWSTATUS_CONTINUE_GAME;
    }


    // DELETE
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
}

void StudentWorld::cleanUp()
{
}


StudentWorld::~StudentWorld()
{
    if (m_Peach != nullptr)
        delete m_Peach;
    if (m_Yoshi != nullptr)
        delete m_Yoshi;

    // Delete other objects
    list<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        Actor* ptr = *it;
        if (ptr != nullptr)
            delete ptr;
    }
}


PlayerAvatar* StudentWorld::getWinner() {

    // TODO
    return m_Peach;
}