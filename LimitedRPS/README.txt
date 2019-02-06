Kaiji Simulator Vol. 1 - Limited Rock-Paper-Scissors
Written by DeltaSierra4 (Vincent Yang) 2/5/2018


I. Introduction

This simulator is based off of a gambling game featured in the very first arc of the Japanese Anime 'Kaiji' - 'Limited Rock-Paper-Scissors'. It follows the same rules as rock-paper-scissors, with a few additional rules: Each contestant of the game is given three lives, represented by the number of stars they have. Each contestant is also given twelve cards: Four rocks, four papers, and four scissors, one of which they have to use to play a game of rock paper scissors against another contestant. Once they use a card, the card is consumed; if the result is a draw, no stars are exchanged; otherwise, the winning contestant takes a star from the losing contestant.

Overall, a contestant is sent to a losing pit when any of the following conditions are met:
i. Contestant runs out of stars.
ii. Contestant has less than 3 stars (less than 4 stars for repeaters) but has no cards left.
iii. The time limit (or turn limit) is over and the contestant has any number of cards left.

A contestant is sent to a lounge for winners when they have 3 or more stars (4 or more stars for repeaters) and they have no cards left.

Repeaters are returning players who have played the game once and are playing again. 


II. How to run the simulator

This program can be compiled and run from a LINUX shell with a command prompt.

i. Create the executable
Type 'make' on a Linux command line from the directory that contains the files to compile and create the executable for this program.

ii. Run the program
Type ./LimitedRPS to run the executable under default settings (300 contestants, no repeaters, no turn limit)

You can also pass in optional parameters to run the simulation under different settings. To pass in parameters, type ./LimitedRPS <number of contestants> <number of repeaters> <turn limit>

Typing in invalid parameters (e.g. any non-numeric characters or having more repeaters than contestants) will not run the program.


III. Release Notes

1.1.0. Notes - 2/5/2018
-Updated argument processing: Now client can pass in optional arguments in any order as long as they provide which argument they are providing (e.g. "-c" for contestant count, "-t" for turn limit)
-Minor fix to the Makefile

1.0.0. Notes - 11/25/2018
- Implemented general simulator: Client can set number of contestants, number of repeaters (see Introduction), and turn limits if they wish.
- Can set number of repeaters participating in the game.


IV. Future plans

- Implement betting mode - Client can bet on specific contestants to see if they win.
- Allow the client to play in the game? This would require implementations of other features (such as buying or trading cards etc)
