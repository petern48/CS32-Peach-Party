#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

// Player Avatar
void PlayerAvatar::doSomething() {

	int nextX, nextY;
	if (m_state == WAITINGTOROLL) {
		// Maybe add part where only check if on square exactly (like below)

		// if Avatar has an invalid direction (due to being teleported)
		getPositionInThisDirection(getWalkDirection(), SPRITE_WIDTH, nextX, nextY);
		if (!getStudentWorld()->isValidSquare(nextX, nextY)) {
			int newDirection;
			do {
				// Pick random valid direction (facing a valid square)
				newDirection = randInt(0, 3) * 90; // Returns direction
				getPositionInThisDirection(newDirection, SPRITE_WIDTH, nextX, nextY);
			} while (getStudentWorld()->isValidSquare(nextX, nextY));

			// Set new Sprite Direction
			if (newDirection == left)
				setDirection(left);
			else
				setDirection(right);
		}
		
		int action = getStudentWorld()->getAction(getPlayerNum());
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			setRoll(die_roll);

			ticks_to_move = die_roll * 8;

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

		// TODO Part 2

		// Only check if Avatar directly on top of directional square
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
		ticks_to_move--;

		// If done moving, change to waiting to roll state
		if (ticks_to_move == 0)
			m_state = WAITINGTOROLL;
	}
}



void PlayerAvatar::turnPerpendicular() {
	// Default to sprite direction right (will change if left)
	setDirection(right);

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
		else if (getStudentWorld()->isValidSquare(getX() - SPRITE_WIDTH, getY())) { // left
			setWalkDirection(left);
			setDirection(left); // set Sprite direction left
		}
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


void CoinSquare::doSomething() {
	if (!isAlive())
		return;

	
}