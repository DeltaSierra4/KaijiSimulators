Kaiji Simulator Vol. 2 - One Poker
Written by DeltaSierra4 (Vincent Yang) 2/5/2019


I. Introduction

This simulator is based off of a gambling game featured in one of the latest arcs of the Japanese Manga 'Kaiji' - 'One Poker'. The rule of the game is as follows:

Each player draws 2 cards from a shuffled deck of poker cards. When they do, they state whether they have 'two up', 'two down', or 'one up one down', depending on what cards they have. The cards that belong to each category are defined as the following:

 * Down: All cards from 2 to 7
 * Up: All cards from 8 to K and Ace
 
Suits are ignored in this game. In each round, each player plays a card from their hand. Whoever has played the card with the higher value wins that round and claims a life from the other player. For instance, if player A plays 7 of hearts and player B plays 9 of spades, player B wins that round; if player A plays Ace of spades and player B plays Jack of clubs, then player A wins that round. This pattern wraps around the value 2: If player A plays Ace of spades and player B plays 2 of clubs, then player B wins that round. Only card 2 can beat the Ace cards (which are defined as the card with the highest value in this game). In case of a draw, no lives are exchanged. Cards that are already played are not returned to the original deck. If the remaining deck is completely consumed, the used cards are reshuffled and used again.

In addition to choosing what card to play, the player can also decide if they want to increase the bet by raising the bet by one more life. The opponent can decide to fold, accept the raise, or bet an even higher amount.

The number of starting lives for each player depends on how much money each player starts with. In this program, the number of lives can be set by the client by optional command line arguments. The game is over when one player runs out of lives.  
 


II. How to run the simulator

This program can be compiled and run from a LINUX shell with a command prompt.

i. Create the executable
Type 'make' on a Linux command line from the directory that contains the files to compile and create the executable for this program.

ii. Run the program
Type ./OnePokerSim to run the executable under default settings (10 lives to each player)

You can also pass in optional parameters to run the simulation under different settings. To pass in parameters, type ./OnePokerSim [-s <setting>] [-pl <player's life count>] [-ol <opponent's life count>]

Here, pass setting = 1 for the 'Kaiji setting', where the player starts with 2 lives and computer starts with 10 lives, or pass setting = 2 for custom settings where the client can choose life count for each player.

Ordering of the parameters does not matter. Typing in invalid parameters (e.g. any non-numeric characters for number of contestants, having more repeaters than contestants, or passing the same argument type twice) will not run the program.
In order to use optional parameters of player/opponent life count, the client must pass the optional parameter of -s 2. Attempting to set player/opponent life counts without passing -s 2 on the command line will not run the program.
Additionally, if the client passes optional arguments of -pl or -ol with -s 1, the program will ignore the optional parameters and proceed with 'Kaiji Settings'. Running ./OnePokerSim -s 2 without any -pl or -ol will make the program run in default settings.



III. Release Notes

1.1.0. Notes - 2/5/2019
-Updated argument processing: Now client can pass in optional arguments in any order as long as they provide which argument they are providing (e.g. "-pl" for player life count)
-Minor fix to the Makefile

1.0.0. Notes - 1/6/2019
- Implemented general simulator: Client can set number of contestants, number of repeaters (see Introduction), and turn limits if they wish.
- Computer can make rudimentary push/fold judgments depending on how many up/down cards are available to each player.


IV. Future plans

- Refine the A.I. to make more sophisticated judgments