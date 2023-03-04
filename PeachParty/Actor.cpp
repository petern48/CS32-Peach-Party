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
		// if Avatar has an invalid direction (due to being teleported)
		getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, nextX, nextY);
		if (getWalkDirection() == INVALIDDIRECTION) {
			int newDirection, randomInt;
			do {
				vector<int> legalMoves = getLegalMoves();
				randomInt = randInt(0, legalMoves.size() - 1);
				newDirection = legalMoves[randomInt];
			} while (newDirection != getDirectionCameFrom());


			// Set new walk direction (and proper Sprite Direction)
			setWalkDirection(newDirection);
		}
		
		int action = getStudentWorld()->getAction(getPlayerNum());
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			setRoll(die_roll);

			m_ticks_to_move = die_roll * 8;

			m_state = WALKING;
		}
		else if (action == ACTION_FIRE) {
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

			Actor* sq = getStudentWorld()->getSquareAt(getX(), getY());

			// If directly on direction Square TODO  (below during activate)
			// If not on direction square
			if (!sq->isDirectionSquare()) {

				// else if Avatar is directly on top of a square at a fork and not direction square
				if (atAFork()) {
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
					vector<int> v = getLegalMoves();
					if (find(v.begin(), v.end(), newDir) != v.end() && newDir != getDirectionCameFrom())
						setWalkDirection(newDir);
					else
						return; // Invalid direction provided
				}

				getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, nextX, nextY);
				// If avatar can't continue moving forward
				if (!getStudentWorld()->isValidSquare(nextX, nextY))
					turnPerpendicular();
			}
		}
		
		// Move two pixels in the walk direction  (2 pixels per tick)
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
	Actor* square = getStudentWorld()->getSquareAt(getX(), getY());

	bool landed = false;

	// If just stopped moving, landed is true
	if (m_ticks_to_move == 0)
		landed = true;

	if (square != nullptr)
		square->setActorToActivateOn(this, landed);

	// TODO Activate Baddies

}



bool Character::atAFork() {
	vector<int> validDir = getLegalMoves();
	// If can move at least two directions, return true, else false
	return validDir.size() >= 3 ? true : false; // Must have 3 legal directions (2 would just be a turn)
}

vector<int> Character::getLegalMoves() {
	int nextX, nextY;
	int direction = 0;
	vector<int> movesAllowed;
	for (int i = 0; i < 4; i++) {
		getPositionInThisDirection(direction, SPRITE_WIDTH, nextX, nextY);
		if (getStudentWorld()->isValidSquare(nextX, nextY))
			movesAllowed.push_back(direction);
		direction += 90;
	}
	return movesAllowed;
}


// Set Walk Direction and Sprite direction accordingly
void Character::setWalkDirection(int dir) { 
	m_walkDirection = dir; 
	if (dir == left)
		setDirection(left);
	else if (dir == INVALIDDIRECTION) { } // Don't change Sprite Direction
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


// SQUARES

void CoinSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getActorToActivateOn();

	// Player must have landed on square
	if (p != nullptr && didCharacterLand()) {
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
	if (p != nullptr && p->isPlayer() && p->getCoins() >= COINSFORASTAR) { // Doesn't need to land
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
	Character* p = getActorToActivateOn();
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

		if (didCharacterLand()) {
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

void EventSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getActorToActivateOn();
	// If activated by a player
	if (p != nullptr && didCharacterLand()) {  // Needs to land
		int option = randInt(1, 3);
		switch (option) {
		case 1:
			p->teleportToRandomSq();
			getStudentWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 2:
			p->swapPositions();
			getStudentWorld()->playSound(SOUND_PLAYER_TELEPORT);
			break;
		case 3:
			p->equipWithVortex();
			getStudentWorld()->playSound(SOUND_GIVE_VORTEX);
			break;
		}
		unactivate();
	}
}


void Vortex::doSomething() {
	if (!isAlive())
		return;

	// Move forward two pixels
	moveAtAngle(getFiringDirection(), 2);

	// If vortex leaves boundaries of screen, set inactive
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_WIDTH) {
		setDead();
		return;
	}

	// Check if overlap
	// TODO
	vector<Actor*> impactables = overlapsWithABaddie();
	if (impactables.size() != 0) {


		setDead();
		getStudentWorld()->playSound(SOUND_HIT_BY_VORTEX);

	}

}

vector<Actor*> Vortex::overlapsWithABaddie() const {
	vector<Actor*> v = getStudentWorld()->getAllBaddies();
	// TODO
	// if (impactable)
	return v;
}


void Bowser::doSomething() {
	/*
	if (getState() == WAITING) { // PAUSED
		
	}
	*/
}

void Boo::doSomething() {
	
}



void Player::swapInts(int& x, int& y) {
	int temp = x;
	x = y;
	y = temp;
}

void Player::swapCoins() {
	Player* other = getStudentWorld()->getOtherPlayer(this);
	swapInts(m_coins, other->m_coins);
}

void Player::swapStars() {
	Player* other = getStudentWorld()->getOtherPlayer(this);
	swapInts(m_stars, other->m_stars);
}

void Player::swapPositions() {
	Player* other = getStudentWorld()->getOtherPlayer(this);
	int temp1 = getX();
	int temp2 = getY();
	// Swap Locations
	moveTo(other->getX(), other->getY());
	other->moveTo(temp1, temp2);
	// Swap number of ticks
	swapInts(m_ticks_to_move, other->m_ticks_to_move);
	// Swap walking directions, (updating Sprite Directions)
	temp1 = getWalkDirection();
	setWalkDirection(other->getWalkDirection());
	other->setWalkDirection(temp1);
	bool tempBool = m_state;
	m_state = other->m_state;
	other->m_state = tempBool;
}

// Teleport and set walk direction to invalid
void Character::teleportToRandomSq() {
	Actor* sq = getStudentWorld()->getRandomSquare();
	int newX = sq->getX();
	int newY = sq->getY();
	moveTo(newX, newY);
	setWalkDirection(INVALIDDIRECTION);
}

int Character::getDirectionCameFrom() const {
	switch (getWalkDirection()) {
	case right:
		return left;
	case left:
		return right;
	case up:
		return down;
	case down:
		return up;
	default:
		return -1; // Can't reach this
	}
}