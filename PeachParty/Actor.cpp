#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Player Avatar
void Player::doSomething() {

	if (getState() == WAITING) {
		// if Avatar has an invalid direction (due to being teleported)
		if (getWalkDirection() == INVALIDDIRECTION) {
			int newDirection, randomInt;
			vector<int> legalMoves = getLegalMoves();
			randomInt = randInt(0, legalMoves.size() - 1);
			newDirection = legalMoves[randomInt];
			setWalkDirection(newDirection); // Set new Walk and Sprite Direction
		}
		
		int action = getStudentWorld()->getAction(getPlayerNum());
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			setRoll(die_roll);

			setTicksToMove(die_roll * 8);

			setState(WALKING);
		}
		else if (action == ACTION_FIRE) {
			StudentWorld* sw = getStudentWorld();
			int newX, newY;
			getPositionInThisDirection(getWalkDirection(), SPRITE_HEIGHT, newX, newY);
			Actor* a = new Vortex(sw, newX / SPRITE_WIDTH, newY / SPRITE_HEIGHT, getWalkDirection());
			sw->addActor(a);
			sw->playSound(SOUND_PLAYER_FIRE);
			m_hasVortex = false;
		}
		// User didn't press a key or any other key
		else {
			return;
		}
	}

	if (getState() == WALKING) {
		// If Avatar directly on top of a square
		if (isOnASquare()) {

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

				// If avatar can't continue moving forward
				if (!squareInFrontExists())
					turnPerpendicular();
			}
		}
		
		// Move two pixels in the walk direction  (2 pixels per tick)
		moveAtAngle(getWalkDirection(), 2);
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
	Actor* square = getStudentWorld()->getSquareAt(getX(), getY());
	vector<Actor*> baddies = getStudentWorld()->getAllBaddies();
	bool landed = false;

	// If just stopped moving, landed is true
	if (getTicksToMove() == 0)
		landed = true;

	if (square != nullptr)
		square->setActorToActivateOn(this, landed);

	// TODO Activate Baddies

}



bool Character::atAFork() {
	vector<int> validDir = getLegalMoves();
	// If can move at least three directions, return true, else false
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
	Player* p = getPlayerToActivateOn();

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

	Player* p = getPlayerToActivateOn();
	// If activated and has enough coins
	if (p != nullptr && p->isPlayer() && p->getCoins() >= COINSFORASTAR) { // Doesn't need to land
		// Replace 20 coins per stars
		p->setCoins(p->getCoins() - COINSFORASTAR);
		p->setStars(p->getStars() + 1);
		getStudentWorld()->playSound(SOUND_GIVE_STAR);
		unactivate();
	}

}

void DirectionSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getPlayerToActivateOn();
	// If activated by a player
	if (p != nullptr) {  // Doesn't need to land
		p->setWalkDirection(getDirection());
		unactivate();
	}
}

void BankSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getPlayerToActivateOn();
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
	Player* p = getPlayerToActivateOn();
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

void DroppingSquare::doSomething() {
	if (!isAlive())
		return;
	Player* p = getPlayerToActivateOn();

	if (p != nullptr && didCharacterLand()) {
		int rand = randInt(1, 2);
		if (rand == 1) {
			int newCoins = p->getCoins() - 10; // Deduct 10
			if (newCoins < 0)
				newCoins = 0;
			p->setCoins(newCoins);
		}
		else {
			int newStars = p->getStars() - 1;
			if (newStars < 0)
				newStars = 0;
			p->setStars(newStars);
		}

		getStudentWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
	}
}

void Vortex::doSomething() {
	if (!isAlive())
		return;

	// Move forward two pixels
	moveAtAngle(getDirection(), 2);

	// If vortex leaves boundaries of screen, set inactive
	if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_WIDTH) {
		setDead();
		return;
	}

	// Check if overlap
	vector<Actor*> impactables = overlapsWithABaddie();
	if (impactables.size() != 0) {
		// Pick one
		Actor* target = impactables[0];
		target->hitByVortex();
		setDead();
		getStudentWorld()->playSound(SOUND_HIT_BY_VORTEX);
	}

}

vector<Actor*> Vortex::overlapsWithABaddie() const { // TODO
	vector<Actor*> v = getStudentWorld()->getAllBaddies();
	vector<Actor*>::iterator it;
	int lowerXBound = getX(); 
	int upperXBound = getX() + SPRITE_WIDTH - 1; // e.g pixels 0 to 15
	int lowerYBound = getY();
	int upperYBound = getY() + SPRITE_HEIGHT - 1;

	// Remove the Baddies not overlapping
	for (it = v.begin(); it != v.end(); it++) {
		Actor* curr = *it;
		// Check if not in bounds, remove from vector
		if (*it != nullptr && ((!isInBounds(lowerXBound, upperXBound, curr->getX()) &&
			!isInBounds(lowerXBound, upperXBound, curr->getX() + SPRITE_WIDTH - 1)) ||
			(!isInBounds(lowerYBound, upperYBound, curr->getY()) &&
				!isInBounds(lowerYBound, upperYBound, curr->getY() + SPRITE_WIDTH - 1)))) {
			v.erase(it);
			*it = nullptr;
			it = v.begin(); // Possibly invalidated
		}
	}
	return v;
}

bool Vortex::isInBounds(int lowerBound, int upperBound, int target) const {
	if (target < lowerBound || target > upperBound)
		return false;
	return true;
}


void Baddie::doSomething() {
	if (getState() == WAITING) {
		waitingAct(); // different for Boo or Bowser

		decreasePauseCounter();

		if (getPauseCounter() == 0) {
			squares_to_move = randInt(1, m_rangeToMove);

			setTicksToMove(squares_to_move * 8);
			
			setRandomLegalDirection();

			setState(WALKING);
		}
	}
	// if in WALKING state
	else {

		if (atAFork()) {
			setRandomLegalDirection();
		}
		// If Baddie on a square, but no square in front of it
		else if (isOnASquare() && !squareInFrontExists()) {
			turnPerpendicular();
		}

		moveAtAngle(getWalkDirection(), 2);

		setTicksToMove(getTicksToMove() - 1);;

		if (getTicksToMove() == 0)
			doneWalkingAct();
	}
}

void Boo::waitingAct() {
	Player* p = getPlayerToActivateOn();

	// Choose either to swap coins or stars
	if (p != nullptr) {
		int n = randInt(0, 1);
		if (n == 1)
			p->swapCoins();
		else
			p->swapStars();
		getStudentWorld()->playSound(SOUND_BOO_ACTIVATE);
	}
}

void Baddie::doneWalkingAct() {
	setState(WAITING);
	m_pauseCounter = 180;
}

void Bowser::waitingAct() {

}

void Bowser::doneWalkingAct() {

	Baddie::doneWalkingAct();

	// 25 percent chance
	int chance = randInt(1, 4);
	if (chance == 1) {
		// Remove current square
		Actor* sq = getStudentWorld()->getSquareAt(getX(), getY());
		sq->setDead();
		// Create Dropping Square
		Actor* a = new DroppingSquare(getX() / SPRITE_WIDTH, getY() / SPRITE_HEIGHT, getStudentWorld());
		getStudentWorld()->addActor(a);

		getStudentWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
	}
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
	temp1 = getTicksToMove();
	setTicksToMove(other->getTicksToMove());
	other->setTicksToMove(temp1);
	// Swap walking directions, (updating Sprite Directions)
	temp1 = getWalkDirection();
	setWalkDirection(other->getWalkDirection());
	other->setWalkDirection(temp1);
	// Swap States
	bool tempBool = getState();
	setState(other->getState());
	other->setState(tempBool);
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

void Baddie::hitByVortex() {
	if (isBaddie()) {
		teleportToRandomSq();
		setWalkDirection(right);
		setState(WAITING);
		m_pauseCounter = 180;
	}
}

void Actor::unactivate() { 
	m_characterToActivateOn = nullptr; 
	m_playerToActivateOn = nullptr;
	m_landed = false;
}

void Actor::setActorToActivateOn(Character* p, bool landed) { 
	m_characterToActivateOn = p;
	m_landed = landed; 
}

void Actor::setActorToActivateOn(Player* p, bool landed) { 
	m_playerToActivateOn = p; 
	m_landed = landed; 
}

void Baddie::setRandomLegalDirection() {
	vector<int> legalMoves = getLegalMoves();
	int randIdx = randInt(0, legalMoves.size() - 1);
	int newDir = legalMoves[randIdx];
	setWalkDirection(newDir);
}

bool Character::squareInFrontExists() const {
	int newX, newY;
	getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, newX, newY);
	if (getStudentWorld()->isValidSquare(newX, newY))
		return true;
	return false;
}

bool Character::isOnASquare() const { 
	return getStudentWorld()->isValidSquare(getX(), getY()); 
}