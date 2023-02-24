#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// Actor

// Player Avatar
void PlayerAvatar::doSomething() {

	// if Avatar has an invalid direction

	if (m_state == WAITINGTOROLL) {
		int action = getStudentWorld()->getAction(getPlayerNum());

		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);

			ticks_to_move = die_roll * 8;

			m_state = WALKING;
		}

	}
	// else if (action == ACTION_FIRE)

	// User didn't press a key or any other key
	else {
		return;
	}

	if (m_state == WALKING) {

		// TODO Part 2

		// If avatar can't continue moving forward
		int newX, newY;
		getPositionInThisDirection(getDirection(), 1, newX, newY);
		if (getStudentWorld()->isValidSquare(newX, newY)) {
			; // CONT
		}
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


void CoinSquare::doSomething() {

}

