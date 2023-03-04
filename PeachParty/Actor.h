#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <vector>

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const bool WAITING = false; // Make class constants???
const bool WALKING = true;
const int GRANTCOINS = 3;
const int DEDUCTCOINS = -3;
const int COINSFORASTAR = 20;
const int INVALIDDIRECTION = -1;

class Actor : public GraphObject {
public:
	Actor(int imageID, StudentWorld* sw, int startX, int startY, int depth = 0, int startDir = right)
		:GraphObject(imageID, startX * SPRITE_WIDTH, startY * SPRITE_HEIGHT, startDir, depth),
		m_studentWorld(sw), m_walkDirection(startDir) { }
	virtual void doSomething() = 0;

	void setDead() { m_isAlive = false; }
	bool isAlive() const { return m_isAlive; }
	StudentWorld* getStudentWorld() const { return m_studentWorld; }
	virtual bool isSquare() const { return false; }
	virtual bool isPlayer() const { return false; }
	virtual bool isImpactable() const { return false; }
	virtual bool isDirectionSquare() const { return false; }
	
	// Included here so both Baddies and Players can access them
	int getWalkDirection() const { return m_walkDirection; }
	void setWalkDirection(int dir);

private:
	StudentWorld* m_studentWorld;
	bool m_isAlive = true;
	int m_walkDirection;
};

	/*
// Moveable character
class Character : public Actor {
public:
	Character(int imageID, StudentWorld* sw, int startX, int startY, bool impactable = false)
		:Actor(imageID, sw, startX, startY, impactable) { }

	int getWalkDirection() const { return m_walkDirection; }
	void setWalkDirection(int walkDirection);
	void setState(int state) { m_state = state; }
	bool getState() const { return m_state; }
	int getTicksToMove() const { return m_ticks_to_move; }
	void setTicksToMove(int ticks) { m_ticks_to_move = ticks; }

private:

};
	*/



// Player
class Player : public Actor {
public:
	Player(int imageID, StudentWorld *sw, int startX, int startY, int playerNum) 
		:Actor(imageID, sw, startX, startY), m_playerNum(playerNum) { } // Default depth of 0, default startDirection of right

	int getPlayerNum() const { return m_playerNum; }
	void addStar() { m_stars++; }
	int getStars() const { return m_stars; }
	void setCoins(int coins) { m_coins = coins; }
	int getCoins() const { return m_coins; }

	virtual bool isPlayer() const { return true; }
	void activate(); // Activates squares or baddies it encounters

	int getRoll() const { return m_dieRoll; }
	void setRoll(int roll) { m_dieRoll = roll; }
	bool hasVortex() const { return m_hasVortex; }
	void equipWithVortex() { m_hasVortex = true; }
	void swapPositions();
	void swapStars();
	void swapCoins();
	void teleportToRandomSq();

	/*
	int getWalkDirection() const { return m_walkDirection; }
	void setWalkDirection(int walkDirection);
	*/
	virtual void doSomething();
	void turnPerpendicular();

private:
	int getDirectionCameFrom() const;
	void swapInts(int &x, int &y);

	int m_stars = 0;
	int m_coins = 0;
	bool m_hasVortex = false;
	int m_playerNum;
	int m_dieRoll = 0;

	bool m_state = WAITING; // Waiting to Roll
	int m_ticks_to_move = 0;
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

bool atAFork(Actor* a);
void getLegalMoves(Actor* a, std::vector<int>& movesAllowed);


class Activatable : public Actor {
public:
	Activatable(int imageID, StudentWorld* sw, int startX, int startY, int depth = 0, int startDir = right)
		: Actor(imageID, sw, startX, startY, depth, startDir) { }

	virtual void doSomething() { } // TODO

	// Overloaded Functions for Actor or Player
	virtual void setActorToActivateOn(Actor* a) { m_actorToActivateOn = a; }
	virtual void setActorToActivateOn(Player* p, bool landed) = 0;

	virtual Actor* getActorToActivateOn() const { return m_actorToActivateOn; }
	virtual void unactivate() { m_actorToActivateOn = nullptr; }

private:
	Actor* m_actorToActivateOn = nullptr;
};


class ActivateOnPlayer : public Activatable {
public:
	ActivateOnPlayer(int imageID, StudentWorld* sw, int startX, int startY, int depth = 0, int startDir = right)
		: Activatable (imageID, sw, startX, startY, depth, startDir) { }

	virtual Player* getActorToActivateOn() const { return m_playerToActivateOn; }
	virtual void setActorToActivateOn(Player* p, bool landed) { m_playerToActivateOn = p; m_landed = landed; }
	virtual void unactivate() { m_playerToActivateOn = nullptr; m_landed = false; }
	bool didPlayerLand() { return m_landed; }

private:
	Player* m_playerToActivateOn = nullptr;
	bool m_landed = false;
};




// SQUARES
class Square : public ActivateOnPlayer {
public:
	Square(int imageID, StudentWorld* sw, int X, int Y)
		:ActivateOnPlayer(imageID, sw, X, Y, 1) { }

	virtual bool isSquare() const { return true; }

private:
};

class CoinSquare : public Square {
public:
	CoinSquare(int imageID, StudentWorld *sw, int X, int Y, int type)
		:Square(imageID, sw, X, Y), m_type(type) { }
	virtual void doSomething();

	void updateCoins(Player *p);

private:
	int m_type;
};

class BlueCoinSquare : public CoinSquare {
public:
	BlueCoinSquare(int x, int y, StudentWorld *sw)
		: CoinSquare(IID_BLUE_COIN_SQUARE, sw, x, y, GRANTCOINS) { }
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

class DirectionSquare : public Square {
public:
	DirectionSquare(int x, int y, StudentWorld* sw, int dir)
		: Square(IID_DIR_SQUARE, sw, x, y) {
		setDirection(dir);
	}
	virtual void doSomething();
	virtual bool isDirectionSquare() const { return true; }
private:
};

class BankSquare : public Square {
public:
	BankSquare(int x, int y, StudentWorld* sw)
		: Square(IID_BANK_SQUARE, sw, x, y) { }
	virtual void doSomething();
};

class EventSquare : public Square {
public:
	EventSquare(int x, int y, StudentWorld* sw)
		: Square(IID_EVENT_SQUARE, sw, x, y) { }
	virtual void doSomething();
};


// BADDIES
class Baddie : public ActivateOnPlayer {
public:
	Baddie(int imageID, StudentWorld* sw, int startX, int startY, int pauseCounter = 180)
		: ActivateOnPlayer(imageID, sw, startX, startY), m_pauseCounter(pauseCounter) { }

	virtual void activate() { }
	virtual bool isImpactable() const { return true; }

	void impactBaddie();

private:
	int m_pauseCounter;
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


class Vortex : public Activatable {
public:
	Vortex(StudentWorld *sw, int x, int y, int direction) 
		: Activatable(IID_VORTEX, sw, x, y), m_firingDirection(direction) { }
	virtual void doSomething();

	int getFiringDirection() const { return m_firingDirection; }
	std::vector<Activatable*> overlapsWithABaddie() const;

private:
	int m_firingDirection;
};

#endif // ACTOR_H_