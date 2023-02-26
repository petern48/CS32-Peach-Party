#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

// Player Avatar
void PlayerAvatar::doSomething() {


	if (m_state == WAITINGTOROLL) {
		// if Avatar has an invalid direction TODO Part 2


		int action = getStudentWorld()->getAction(getPlayerNum());
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			setRoll(die_roll);

			ticks_to_move = die_roll * 8;

			m_state = WALKING;
		}
		// else if (action == ACTION_FIRE)

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

			int nextX, nextY;
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

	//if the player presses the left arrow key :
	//set the avatar's direction to left
	//	14
	//	move the avatar two pixels forward
	//	if the player presses the right arrow key :
	//set the avatar's direction to right
	//	move the avatar two pixels forward
	//	...
	//	if the player presses the space key :
	//add a new projectile directly in front of the player
	//	decrement the ammunition count by one
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


void CoinSquare::doSomething() {
	if (!isAlive())
		return;
}