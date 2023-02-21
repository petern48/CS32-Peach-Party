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

private:
	Board m_board;
	std::list<Actor*> m_actors;
	Yoshi* m_Yoshi = nullptr;
	Peach* m_Peach = nullptr;
};

#endif // STUDENTWORLD_H_
