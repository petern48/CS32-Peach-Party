#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>

#include <list>
#include <vector>
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

  void addActor(Actor* a) { m_actors.push_back(a); }
  int getBankCoins() const { return m_bankMoney; }
  void depositBankCoins(int c) { m_bankMoney += c; }
  void resetBankCoins() { m_bankMoney = 0; }

  bool isValidSquare(int x, int y);
  Actor* getSquareAt(int x, int y) const;
  Actor* getRandomSquare() const;
  
  Player* getOtherPlayer(Player* p) const;
  std::vector<Actor*> getAllBaddies() const;
  std::vector<Player*> getAllPlayers() const;


private:
	Player* getWinner() const;
	std::string getStatsString() const;

	Board m_board;
	std::list<Actor*> m_actors;
	Yoshi* m_Yoshi = nullptr;
	Peach* m_Peach = nullptr;
	int m_bankMoney = 0;
};

#endif // STUDENTWORLD_H_
