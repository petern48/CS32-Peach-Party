# CS-32-Peach-Party

This class project was focused on implementing polymorphism, inheritance, and data structures to create a mini game modeled off of Nintendo's Mario Party.
Given a project skeleton, I implemented the Actor.h, Actor.cpp, StudentWorld.h, and StudentWorld.cpp files.
<br/>

### The Game

Peach Party is a two-player game that resembles a traditional board game.
The players must direct their avatars, Peach and Yoshi, along the squares on the game
board collecting coins and stars, while trying to avoid baddies like Bowser and Boo who cause all sorts of trouble.
 
When the game starts, the players may choose what board they want to play by pressing a key from 1 to 9

Players roll a (virtual) 10-sided die to decide how far they can move along the board during their next turn. After hitting a key to roll the die, the player that rolled then travels along the board's squares in their current direction until they've moved the appropriate number of squares, at which time they "land" on a square.

If players reach a fork, they may choose what direction to move next.

Many of the squares on the Peach Party game board have special powers - some will grant or deduct coins or stars from the players when they're landed on. Others will cause the player to teleport to another square on the board, give the player Vortex projectiles to shoot at the baddies, etc. In most cases, a square will only "activate" on Peach or Yoshi if they land upon it as their final move of the current die roll.

The winner of the game is the player who has collected the most stars and coins during 99 seconds of game play (a tiebreaker is used if needed).

### Implementation
Read more about the details of each of these classes in the `PeachPartySpec.pdf`

World: `StudentWorld`

Players: `Yoshi`, `Peach` 

Squares: `CoinSquare`, `StarSquare`, `DirectionSquare`, `BankSquare`, `EventSquare`, `DroppingSquare`

Baddies: `Bowser`, `Boo`

Projectiles: `Vortex`
