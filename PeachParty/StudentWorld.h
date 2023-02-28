#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

#include <list>
#include "Actor.h"

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld 
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  virtual ~StudentWorld();
  int getBankMoney() { return m_bankMoney; }
  bool isValidSquare(int x, int y);

private:
	PlayerAvatar* getWinner();
	std::string getStatsString();

	Board m_board;
	std::list<Actor*> m_actors;
	Yoshi* m_Yoshi = nullptr;
	Peach* m_Peach = nullptr;
	int m_bankMoney = 0;
};

#endif // STUDENTWORLD_H_
