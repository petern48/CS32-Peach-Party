#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int WAITINGTOROLL = 0; // Make class constants???
const int WALKING = 1;
const bool IMPACTABLE = true;

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

	int getRoll() { return 1; }; // TODO
	void fireProjectile();

	virtual void doSomething();

private:
	int m_state = WAITINGTOROLL; // Waiting to Roll or Walking
	int m_stars = 0;
	int m_coins = 0;
	int ticks_to_move = 0;
	bool hasVortex = false;
	int m_playerNum;
};

class Peach : public PlayerAvatar {
public:
	Peach(int startX, int startY, StudentWorld *sw)
		:PlayerAvatar(IID_PEACH, sw, startX, startY, 1) { }
};

class Yoshi : public PlayerAvatar {
public:
	Yoshi::Yoshi(int startX, int startY, StudentWorld *sw)
		:PlayerAvatar(IID_YOSHI, sw, startX, startY, 2) { }
};

// Squares
class Square : public Actor {
public:
	Square(int imageID, StudentWorld* sw, int X, int Y, bool hasPlayer = false)
		:Actor(imageID, sw, X, Y, 1), m_hasPlayer(hasPlayer) { }

	// virtual void doSomething() = 0;
	bool getPlayerStatus();

private:
	bool m_hasPlayer;
};

class CoinSquare : public Square {
public:
	CoinSquare(int imageID, StudentWorld *sw, int X, int Y, bool hasPlayer = false)
		:Square(imageID, sw, X, Y, hasPlayer) { }
	virtual void doSomething();
	//virtual int changeCoins() = 0;
private:
	// May be disabled by bowser
};

class BlueCoinSquare : public CoinSquare {
public:
	BlueCoinSquare(int X, int Y, StudentWorld *sw)
		:CoinSquare(IID_BLUE_COIN_SQUARE, sw, X, Y) { }
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
