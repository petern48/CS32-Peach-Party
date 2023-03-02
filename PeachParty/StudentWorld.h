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

  int getBankCoins() const { return m_bankMoney; }
  void depositBankCoins(int c) { m_bankMoney += c; }
  void resetBankCoins() { m_bankMoney = 0; }

  bool isValidSquare(int x, int y);
  Actor* getSquareAt(int x, int y);
  Actor* getRandomSquare();
  
  Player* getOtherPlayer(Player* p) const;


private:
	Player* getWinner();
	std::string getStatsString();
	void deleteActor(Actor* a);

	Board m_board;
	std::list<Actor*> m_actors;
	Yoshi* m_Yoshi = nullptr;
	Peach* m_Peach = nullptr;
	int m_bankMoney = 0;
};

#endif // STUDENTWORLD_H_
