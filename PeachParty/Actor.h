#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const bool WAITING = false; // Make class constants???
const bool WALKING = true;
const bool IMPACTABLE = true;
const int GRANTCOINS = 3;
const int DEDUCTCOINS = -3;

class Actor : public GraphObject {
public:
	Actor(int imageID, StudentWorld* sw, int startX, int startY, bool impactable = false, int depth = 0, int startDir = right)
		:GraphObject(imageID, startX * SPRITE_WIDTH, startY * SPRITE_HEIGHT, startDir, depth),
		m_impactable(impactable), m_studentWorld(sw) { }
	virtual void doSomething() = 0;
	//virtual ~Actor();

	void setDead() { m_isAlive = false; }
	bool isAlive() const { return m_isAlive; }
	StudentWorld* getStudentWorld() const { return m_studentWorld; }
	virtual bool isSquare() const { return false; }
	virtual bool isPlayer() const { return false; }
	// virtual bool isImpactable() const { return m_impactable; }

private:
	StudentWorld* m_studentWorld;
	bool m_impactable;
	bool m_isAlive = true;
};

// Moveable character
class Character : public Actor {
public:
	Character(int imageID, StudentWorld* sw, int startX, int startY, bool impactable = false)
		:Actor(imageID, sw, startX, startY, impactable) { }
	int getWalkDirection() const { return m_walkDirection; }
	void setWalkDirection(int walkDirection) { m_walkDirection = walkDirection; }
	virtual void activate() = 0;

	void setState(int state) { m_state = state; }
	bool getState() const { return m_state; }

protected:
	int getTicksToMove() const { return m_ticks_to_move; }
	void setTicksToMove(int ticks) { m_ticks_to_move = ticks; }
	void turnPerpendicular();
	void setSpriteDirection(int dir) { dir == left ? setDirection(left) : setDirection(right); }
	void getLegalMoves(int moves[]);

private:
	bool m_state = WAITING; // Waiting to Roll or Paused for Baddies
	int m_walkDirection = right; // Starting Walking direction is right
	int m_ticks_to_move = 0;
};

// Player
class Player : public Character {
public:
	Player(int imageID, StudentWorld *sw, int startX, int startY, int playerNum) 
		:Character(imageID, sw, startX, startY), m_playerNum(playerNum) { } // Default depth of 0, default startDirection of right

	int getPlayerNum() const { return m_playerNum; }   // UNSURE!!!
	void setStars(int stars) { m_stars = stars; }
	int getStars() const { return m_stars; }
	void setCoins(int coins) { m_coins = coins; }
	int getCoins() const { return m_coins; }

	virtual bool isPlayer() const { return true; }
	virtual void activate(); // Activates squares or baddies it encounters

	int getRoll() const { return m_dieRoll; }
	void setRoll(int roll) { m_dieRoll = roll; }
	bool hasVortex() { return m_hasVortex; }
	void equipWithVortex() { m_hasVortex = true; }
	void swapPositions();
	void swapStars();
	void swapCoins();
	void teleportToRandomSq();

	virtual void doSomething();

private:
	int m_stars = 0;
	int m_coins = 0;
	bool m_hasVortex = false;
	int m_playerNum;
	int m_dieRoll = 0;
};

class Peach : public Player {
public:
	Peach(int startX, int startY, StudentWorld *sw)
		:Player(IID_PEACH, sw, startX, startY, 1) { }
};

class Yoshi : public Player {
public:
	Yoshi(int startX, int startY, StudentWorld *sw)
		:Player(IID_YOSHI, sw, startX, startY, 2) { }
};



class Activatable : public Actor {
public:
	Activatable(int imageID, StudentWorld* sw, int startX, int startY, bool impactable = false, int depth = 0, int startDir = right)
		: Actor(imageID, sw, startX, startY, impactable, depth, startDir) { }

	virtual void doSomething() { } // TODO

	// Overloaded Functions for Actor or Player
	virtual void setActorToActivateOn(Actor* a) { m_actorToActivateOn = a; }
	virtual void setActorToActivateOn(Player* p) = 0;

	virtual Actor* getActorToActivateOn() const { return m_actorToActivateOn; }
	virtual void unactivate() { m_actorToActivateOn = nullptr; }

private:
	Actor* m_actorToActivateOn = nullptr;
};


class ActivateOnPlayer : public Activatable {
public:
	ActivateOnPlayer(int imageID, StudentWorld* sw, int startX, int startY, bool impactable = false, int depth = 0, int startDir = right)
		: Activatable (imageID, sw, startX, startY, impactable, depth, startDir) { }

	//virtual void setActorToActivateOn(Player* p); /* { m_playerToActivateOn = p; }*/
	virtual Player* getActorToActivateOn() const { return m_playerToActivateOn; }
	virtual void setActorToActivateOn(Player* p) { m_playerToActivateOn = p; }
	virtual void unactivate() { m_playerToActivateOn = nullptr; }

private:
	Player* m_playerToActivateOn = nullptr;
};


class Vortex : public Activatable {
public:
	Vortex(StudentWorld *sw, int x, int y, int direction) 
		: Activatable(IID_VORTEX, sw, x, y), m_firingDirection(direction) { }
	virtual void doSomething();
	virtual void activate();

	int getFiringDirection() { return m_firingDirection; }

private:
	int m_firingDirection;
};



// SQUARES
class Square : public ActivateOnPlayer {
public:
	Square(int imageID, StudentWorld* sw, int X, int Y)
		:ActivateOnPlayer(imageID, sw, X, Y, false, 1) { }

	//bool hasPlayer() { return m_hasPlayer; }
	//void setHasPlayer();
	// virtual Character* hasNewPlayer(Character* c, bool landed) { return c; }
	virtual bool isSquare() { return true; }

private:
};

class CoinSquare : public Square {
public:
	CoinSquare(int imageID, StudentWorld *sw, int X, int Y, bool type)
		:Square(imageID, sw, X, Y), m_type(type) { }
	virtual void doSomething();

	void updateCoins(Player *p);

private:
	int m_type;
};

class BlueCoinSquare : public CoinSquare {
public:
	BlueCoinSquare(int x, int y, StudentWorld *sw)
		:CoinSquare(IID_BLUE_COIN_SQUARE, sw, x, y, GRANTCOINS) { }
};

class RedCoinSquare : public CoinSquare {
public:
	RedCoinSquare(int x, int y, StudentWorld* sw)
		: CoinSquare(IID_RED_COIN_SQUARE, sw, x, y, DEDUCTCOINS) { }
};

class StarSquare : public Square {
public:
	StarSquare(int x, int y, StudentWorld* sw)
		: Square(IID_STAR_SQUARE, sw, x, y) { }
	virtual void doSomething();
};



// BADDIES
class Baddie : public ActivateOnPlayer {
public:
	Baddie(int imageID, StudentWorld* sw, int startX, int startY)
		: ActivateOnPlayer(imageID, sw, startX, startY, IMPACTABLE){ }

	virtual void activate() { }

	void impactBaddie();

private:
	int m_pauseCounter = 180;
};

class Bowser : public Baddie {
public:
	Bowser(int startX, int startY, StudentWorld* sw)
	: Baddie(IID_BOWSER, sw, startX, startY) { }
	virtual void doSomething();
};

class Boo : public Baddie {
public:
	Boo(int startX, int startY, StudentWorld* sw)
		: Baddie(IID_BOO, sw, startX, startY) { }
	virtual void doSomething();
};


void vortexOverlapsWithABaddie();

#endif // ACTOR_H_