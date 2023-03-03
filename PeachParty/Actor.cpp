#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

// Player Avatar
void Player::doSomething() {

	int nextX, nextY;
	if (getState() == WAITING) {
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

			setTicksToMove(die_roll * 8);

			setState(WALKING);
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

	if (getState() == WALKING) {

		// TODO Part 2

		// If Avatar directly on top of a square
		if (getStudentWorld()->isValidSquare(getX(), getY())) {

			// else if Avatar is directly on top of a square at a fork

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
		setTicksToMove(getTicksToMove() - 1);

		// Tell actors to activate next tick
		activate();

		// If done moving, change to waiting to roll state
		if (getTicksToMove() == 0)
			setState(WAITING);
	}
}

void Player::activate() {

	// Search for square Actor might be on
	Activatable* square = getStudentWorld()->getSquareAt(getX(), getY());

	bool landed = false;

	// If just stopped moving, landed is true
	if (getTicksToMove() == 0)
		landed = true;

	if (square != nullptr)
		square->setActorToActivateOn(this, landed);

	// Still walking
}

/*
Activatable::doSomething() {

}
*/

// Set Walk Direction and Sprite direction accordingly
void Character::setWalkDirection(int walkDirection) { 
	m_walkDirection = walkDirection; 
	if (walkDirection == left)
		setDirection(left);
	else
		setDirection(right);
}

// Prefer turning to left or right
void Character::turnPerpendicular() {
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

std::vector<int> Character::getLegalMoves() {
	vector<int> movesAllowed;
	int nextX, nextY;
	int direction = 0;
	for (int i = 0; i < 4; i++) {
		getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, nextX, nextY);
		if (getStudentWorld()->isValidSquare(nextX, nextY) && direction)
			movesAllowed.push_back(direction);
		direction += 90;
	}
	return movesAllowed;
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
	if (p != nullptr && p->getCoins() >= COINSFORASTAR) {
		// Replace 20 coins per stars
		p->setCoins(p->getCoins() - COINSFORASTAR);
		p->addStar();
		getStudentWorld()->playSound(SOUND_GIVE_STAR);
	}

}

void DirectionSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getActorToActivateOn();
	// If activated by a player
	if (p != nullptr) {
		p->setWalkDirection(getDirection());
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