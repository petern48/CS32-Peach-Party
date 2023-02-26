#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int WAITINGTOROLL = 0; // Make class constants???
const int WALKING = 1;
const bool IMPACTABLE = true;
const bool GRANTCOINS = true;
const bool DEDUCTCOINS = false;

class Actor : public GraphObject {
public:
	Actor(int imageID, StudentWorld* sw, int startX, int startY, bool impactable = false, int depth = 0, int startDir = right)
		:GraphObject(imageID, startX, startY, startDir, depth), m_impactable(impactable), m_studentWorld(sw) { }
	virtual void doSomething() = 0;
	//virtual ~Actor();

	void setDead() { m_isAlive = false; }
	bool isAlive() { return m_isAlive; }
	StudentWorld* getStudentWorld() { return m_studentWorld; }

private:
	StudentWorld* m_studentWorld;
	bool m_impactable;
	bool m_isAlive = true;
};

// PlayerAvatar
class PlayerAvatar : public Actor {
public:
	PlayerAvatar(int imageID, StudentWorld *sw, int startX, int startY, int playerNum) 
		:Actor(imageID, sw, startX, startY), m_playerNum(playerNum) { } // Default depth of 0, default startDirection of right
	void setState(int state) { m_state = state; }
	bool getState() const { return m_state; }
	void setStars(int stars) { m_stars = stars; }
	int getStars() const { return m_stars; }
	void setCoins(int coins) { m_coins = coins; }
	int getCoins() const { return m_coins; }
	int getPlayerNum() const { return m_playerNum; }
	int getWalkDirection() const { return m_walkDirection; }
	void setWalkDirection(int walkDirection) { m_walkDirection = walkDirection; }
	int getRoll() const { return m_dieRoll; };
	void setRoll(int roll) { m_dieRoll = roll; }
	void fireProjectile();

	virtual void doSomething();

private:

	void turnPerpendicular();

	int m_state = WAITINGTOROLL; // Waiting to Roll or Walking
	int m_stars = 0;
	int m_coins = 0;
	int m_walkDirection = right; // Starting Walking direction is right
	int ticks_to_move = 0;
	bool hasVortex = false;
	int m_playerNum;
	int m_dieRoll = 0;
};

class Peach : public PlayerAvatar {
public:
	Peach(int startX, int startY, StudentWorld *sw)
		:PlayerAvatar(IID_PEACH, sw, startX, startY, 1) { }
};

class Yoshi : public PlayerAvatar {
public:
	Yoshi(int startX, int startY, StudentWorld *sw)
		:PlayerAvatar(IID_YOSHI, sw, startX, startY, 2) { }
};

// Squares
class Square : public Actor {
public:
	Square(int imageID, StudentWorld* sw, int X, int Y, bool hasPlayer = false)
		:Actor(imageID, sw, X, Y, false, 1), m_hasPlayer(hasPlayer) { }

	// virtual void doSomething() = 0;
	bool hasPlayer();

private:
	bool m_hasPlayer;
};

class CoinSquare : public Square {
public:
	CoinSquare(int imageID, StudentWorld *sw, int X, int Y, bool AddSubtract, bool hasPlayer = false)
		:Square(imageID, sw, X, Y, hasPlayer), m_AddOrSubtract(AddSubtract) { }
	virtual void doSomething();
	//virtual int changeCoins() = 0;
private:
	bool m_AddOrSubtract;
};

class BlueCoinSquare : public CoinSquare {
public:
	BlueCoinSquare(int X, int Y, StudentWorld *sw)
		:CoinSquare(IID_BLUE_COIN_SQUARE, sw, X, Y, GRANTCOINS) { }
};

// Baddies
class Baddie : public Actor {
public:
	// Baddie(int imageID, int startX, int startY)
	// impactable
};

class Bowser : public Baddie {
public:
	Bowser();
};

class Boo : public Baddie {
public:
	Boo();
};



#endif // ACTOR_H_
