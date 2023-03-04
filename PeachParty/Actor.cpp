#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

// Player Avatar
void Player::doSomething() {

	int nextX, nextY;
	if (m_state == WAITING) {
		// Maybe add part where only check if on square exactly (like below)

		// if Avatar has an invalid direction (due to being teleported) TODOOOOOO

		/*
		getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, nextX, nextY);
		if (!getStudentWorld()->isValidSquare(nextX, nextY)) {
			int newDirection;

			do {
			vector<int> legalMoves = getLegalMoves();
			newDirection = randInt(0, legalMoves.size() - 1);
			} while (newDirection != getWalkDirection() + 180 && newDirection != getWalkDirection() - 180);
			// Direction shouldn't be going backwards

			// Set new walk direction (and proper Sprite Direction)
			setWalkDirection(newDirection);
		}
		*/
		
		int action = getStudentWorld()->getAction(getPlayerNum());
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			setRoll(die_roll);

			m_ticks_to_move = die_roll * 8;

			m_state = WALKING;
		}
		if (action == ACTION_FIRE) {
			// TODO
			// m_vortex = new Vortex();

			getStudentWorld()->playSound(SOUND_PLAYER_FIRE);

			// delete
		}

		// User didn't press a key or any other key
		else {
			return;
		}
	}

	if (m_state == WALKING) {
		// If Avatar directly on top of a square
		if (getStudentWorld()->isValidSquare(getX(), getY())) {

			// If directly on direction Square TODO  (below during activate)

			// else if Avatar is directly on top of a square at a fork 
			if (atAFork(this) /* and not on a direction square*/) { // TODO
				int action = getStudentWorld()->getAction(getPlayerNum());
				int newDir;
				int directionIndicated = true;
				// if user pressed a key
				switch (action) {
				case ACTION_DOWN:
					newDir = down; break;
				case ACTION_LEFT:
					newDir = left; break;
				case ACTION_UP:
					newDir = up; break;
				case ACTION_RIGHT:
					newDir = right; break;
				default:
					return; // If no direction indicated
				}
				// If newDir is legal move
				vector<int> v;
				getLegalMoves(this, v);
				if (find(v.begin(), v.end(), newDir) != v.end())
					setWalkDirection(newDir);
			}



			getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, nextX, nextY);
			// If avatar can't continue moving forward
			if (!getStudentWorld()->isValidSquare(nextX, nextY)) {

				// Turn towards valid perpendicular direction, prefer moving up or right
				turnPerpendicular();
			}
		}
		
		// Move two pixels in the walk direction  (2 pixels per tick
		moveAtAngle(getWalkDirection(), 2);

		// Decrement ticks to move by 1
		m_ticks_to_move--;

		// Tell actors to activate next tick
		activate();

		// If done moving, change to waiting to roll state
		if (m_ticks_to_move == 0)
			m_state = WAITING;
	}
}

void Player::activate() {

	// Search for square Actor might be on
	Activatable* square = getStudentWorld()->getSquareAt(getX(), getY());

	bool landed = false;

	// If just stopped moving, landed is true
	if (m_ticks_to_move == 0)
		landed = true;

	if (square != nullptr)
		square->setActorToActivateOn(this, landed);

}



bool atAFork(Actor *a) {
	vector<int> validDir;
	getLegalMoves(a, validDir);
	// If can move at least two directions, return true, else false
	return validDir.size() >= 3 ? true : false; // Must have 3 legal directions (2 would just be a turn)
}

void getLegalMoves(Actor* a, std::vector<int> &movesAllowed) {
	int nextX, nextY;
	int direction = 0;
	for (int i = 0; i < 4; i++) {
		a->getPositionInThisDirection(direction, SPRITE_WIDTH, nextX, nextY);
		if (a->getStudentWorld()->isValidSquare(nextX, nextY))
			movesAllowed.push_back(direction);
		direction += 90;
	}
}


// Set Walk Direction and Sprite direction accordingly
void Actor::setWalkDirection(int dir) { 
	m_walkDirection = dir; 
	if (dir == left)
		setDirection(left);
	else
		setDirection(right);
}

// Prefer turning to left or right
void Player::turnPerpendicular() {
	switch (getWalkDirection()) {
	case right:
	case left:
		if (getStudentWorld()->isValidSquare(getX(), getY() + SPRITE_HEIGHT)) // up
			setWalkDirection(up);
		else if (getStudentWorld()->isValidSquare(getX(), getY() - SPRITE_HEIGHT)) // down
			setWalkDirection(down);
		break;
	case up:
	case down:
		if (getStudentWorld()->isValidSquare(getX() + SPRITE_WIDTH, getY())) // right
			setWalkDirection(right);
		else if (getStudentWorld()->isValidSquare(getX() - SPRITE_WIDTH, getY())) // left
			setWalkDirection(left);
		break;
	}
}


void Vortex::doSomething() {
	if (!isAlive())
		return;

	// Move forward two pixels
	moveAtAngle(getFiringDirection(), 2);

	// If vortex leaves boundaries of screen, set inactive
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_WIDTH)
		setDead();

	// Check if overlap
	// TODO

}


// SQUARES

void CoinSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getActorToActivateOn();

	// Player must have landed on square
	if (p != nullptr && didPlayerLand() == true) {
		updateCoins(p);
		unactivate();
	}
}

void CoinSquare::updateCoins(Player* p) {
	if (m_type == GRANTCOINS) {
		p->setCoins(p->getCoins() + GRANTCOINS);
		getStudentWorld()->playSound(SOUND_GIVE_COIN);
	}
	else {
		int newCount = p->getCoins() + DEDUCTCOINS;
		if (newCount <= 0)
			p->setCoins(0);
		else
			p->setCoins(newCount);
		getStudentWorld()->playSound(SOUND_TAKE_COIN);
	}
}

void StarSquare::doSomething() {
	if (!isAlive())
		return;

	Player* p = getActorToActivateOn();
	// If activated and has enough coins
	if (p != nullptr && p->getCoins() >= COINSFORASTAR) { // Doesn't need to land
		// Replace 20 coins per stars
		p->setCoins(p->getCoins() - COINSFORASTAR);
		p->addStar();
		getStudentWorld()->playSound(SOUND_GIVE_STAR);
		unactivate();
	}

}

void DirectionSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getActorToActivateOn();
	// If activated by a player
	if (p != nullptr) {  // Doesn't need to land
		p->setWalkDirection(getDirection());
		unactivate();
	}
}

void BankSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getActorToActivateOn();
	// If activated by a player
	if (p != nullptr) {

		if (didPlayerLand()) {
			// Empty bank and add coins to player
			p->setCoins(p->getCoins() + getStudentWorld()->getBankCoins());
			getStudentWorld()->resetBankCoins();
			getStudentWorld()->playSound(SOUND_WITHDRAW_BANK);
		}
		// if player passed onto square (didn't land)
		else {
			// Deduct 5 coins, can't go into negative
			int newCount = p->getCoins() - 5;
			if (newCount < 0)
				newCount = 0;
			p->setCoins(newCount);
			getStudentWorld()->depositBankCoins(newCount);
			getStudentWorld()->playSound(SOUND_DEPOSIT_BANK);
		}
		unactivate();
	}
}


void Bowser::doSomething() {
	/*
	if (getState() == WAITING) { // PAUSED
		
	}
	*/
}

void Boo::doSomething() {
	
}