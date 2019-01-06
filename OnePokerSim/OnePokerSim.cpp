/*
 * class OnePokerSim
 *
 * Main executable of the One Poker Simulator program. Simulates a game of
 * 'One Poker' that is featured in Season 4 of the Japanese Manga 'Kaiji'.
 * Rules of the game: Each player draws 2 cards from a shuffled deck of
 * poker cards. When they do, they state whether they have 'two up', 'two down',
 * or 'one up one down', depending on what cards they have. The cards that
 * belong to each category are defined as the following:
 *
 * Down: All cards from 2 to 7
 * Up: All cards from 8 to K and Ace
 *
 * Suits are ignored in this game. In each round, each player plays a card
 * from their hand. Whoever has played the card with the higher value wins
 * that round and claims a life from the other player. For instance, if
 * player A plays 7 of hearts and player B plays 9 of spades, player B wins
 * that round; if player A plays Ace of spades and player B plays Jack of clubs,
 * then player A wins that round. This pattern wraps around the value 2:
 * If player A plays Ace of spades and player B plays 2 of clubs, then player B
 * wins that round. Only card 2 can beat the Ace cards (which are defined as
 * the card with the highest value in this game). In case of a draw, no lives
 * are exchanged. Cards that are already played are not returned to the original
 * deck. If the remaining deck is completely consumed, the used cards are
 * reshuffled and used again.
 *
 * The number of starting lives for each player depends on how much money
 * each player starts with. In this program, the number of lives can be set
 * by the client by optional command line arguments. The game is over when one
 * player runs out of lives.
 *
 * How to run it:
 *    ./OnePokerSim <setting> <player's life count> <opponent's life count>
 * setting = 1 for 'Kaiji setting': player starts with 2 lives and computer
 * starts with 10 lives
 * setting = 2 for custom settings: Client can choose life count for each
 * player
 * If no arguments are used, game will proceed with default settings
 * of 6:6 life count
 *
 * Written by Vincent Yang 1/5/2019
 * This is a work in progress!
 */

#include "OPContestant.h"
#include "PokerCards.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>

#define DEFAULT_LIFE_COUNT 10
#define KAIJI_LIFE_COUNT_PLAYER 2
#define KAIJI_LIFE_COUNT_COMPUTER 10

using namespace std;

/*
 * Helper method that checks if user input a valid integer.
 * No floating points or alphabets are allowed. also
 * no negative numbers allowed either.
 * @param  input from the command line argument
 * @returns true iff input was a non-negative integer
 */
bool isValidInput(const string & s)
{
  string::const_iterator it = s.begin();
  while (it != s.end() && isdigit(*it)) ++it;

  return !s.empty() && it == s.end();
}


 /*
  * Produces 52 cards and shuffles them in a random manner.
  * @param   vector that will contain the deck of cards
  */

void generateShuffledDeck(vector<PokerCards*> & deck)
{
  for (int suit = PokerCards::CLUBS ; suit <= PokerCards::HEARTS ; suit++)
  {
    for (int value = PokerCards::ACE ; value <= PokerCards::KING ; value++)
    {
      deck.push_back(new PokerCards(suit, value));
    }
  }
  random_shuffle(deck.begin(), deck.end());
}

/*
 * Checks the number of up cards and down cards for two players engaged in
 * a game.
 * @params  The two instances of OPContestants that will engage in the game
 * @param   The vector of booleans indicating the status of each player.
 * index 0 - player 1 has two up cards
 * index 1 - player 1 has one up and one down cards
 * index 2 - player 1 has two down cards
 * index 3 - player 2 has two up cards
 * index 4 - player 2 has one up and one down cards
 * index 5 - player 2 has two down cards
 */
void checkUpDown(OPContestant *& player1, OPContestant *& player2, vector<bool> & updown)
{
  int player1card1 = player1->seeCardValue(0);
  int player1card2 = player1->seeCardValue(1);
  int player2card1 = player2->seeCardValue(0);
  int player2card2 = player2->seeCardValue(1);
  if (player1card1 >= 2 && player1card1 <= 7 && player1card2 >= 2 && player1card2 <= 7)
  {
    updown[2] = true;
  }
  else if ((player1card1 >= 8 || player1card1 == PokerCards::ACE) && player1card2 >= 2 && player1card2 <= 7)
  {
    updown[1] = true;
  }
  else if ((player1card1 >= 8 || player1card1 == PokerCards::ACE) && (player1card2 >= 8 || player1card2 == PokerCards::ACE))
  {
    updown[0] = true;
  }
  if (player2card1 >= 2 && player2card1 <= 7 && player2card2 >= 2 && player2card2 <= 7)
  {
    updown[5] = true;
  }
  else if ((player2card1 >= 8 || player2card1 == PokerCards::ACE) && player2card2 >= 2 && player2card2 <= 7)
  {
    updown[4] = true;
  }
  else if ((player2card1 >= 8 || player2card1 == PokerCards::ACE) && (player2card2 >= 8 || player2card2 == PokerCards::ACE))
  {
    updown[3] = true;
  }
}

/*
 * Plays a round of One Poker. This is used for the training data!
 * @params  The two instances of OPContestants that will engage in the game
 * @param   The deck of cards used for this game
 */
void playRoundTraining(OPContestant *& player1, OPContestant *& player2, vector<PokerCards*> & deck)
{
  //cout << "Prior to creating updown vector." << endl;//DEBUG
  //Check for the number of ups and downs for each player
  vector<bool> updown(6);
  fill(updown.begin(), updown.end(), false);
  //cout << "After creating updown array and prior to checking updowns." << endl;//DEBUG
  checkUpDown(player1, player2, updown);
  //cout << "After checking updowns and prior to check/comparing card values." << endl;//DEBUG

  //In the training mode, the computer randomly picks a card to play.
  //If the choice was good, 1 point is added to the corresponding index
  //in one of the arrays (scoreA, scoreB, or scoreC) - see OPContestant.h for
  //details.
  int player1Choice = rand() % 2;
  int player2Choice = rand() % 2;
  int player1Value = player1->seeCardValue(player1Choice);
  int player2Value = player2->seeCardValue(player2Choice);

  //cout << "player1 card choices: " << player1->seeCardValue(0) << " and " << player1->seeCardValue(1) << " | player1 picked " << player1->seeCardValue(player1Choice) << endl; //DEBUG
  //cout << "player2 card choices: " << player2->seeCardValue(0) << " and " << player2->seeCardValue(1) << " | player2 picked " << player2->seeCardValue(player2Choice) << endl; //DEBUG

  //In each game, each player bets at least 1 life. This amount can be raised
  //if both players agree to do so. If one player chooses to raise and the other
  //folds, they forfeit all lives they have bet until that point.
  int player1Bet = 1;
  int player2Bet = 1;

  int player1Raise;
  int player2Raise;
  //In the training mode, the computer will randomly decide to raise or not,
  //regardless of the number of up/down cards the opponent has.
  //2 for raise, 1 for check, 0 for fold
  //If both sides pick 0, it counts as both sides agreeing not to raise.
  //If both sides pick 2, it counts as both sides agreeing to raise.
  player1Raise = rand() % 3;
  player2Raise = rand() % 3;

  //The following two if statements are placed to prevent a degenerate case,
  //where the A.I. attempts to 'fold before the round even begins'.
  //(i.e. you are not allowed to fold before placing a minimum bet of 1 life.)
  //Folding is only an option with the initial bet iff the opponent chooses to
  //raise the bet (i.e. if either raise flags result in a roll of 2).
  if ((player1Raise == 0 && player2Raise == 1) && player1Bet == 1)
  {
    player1Raise = 1;
  }
  if ((player2Raise == 0 && player1Raise == 1) && player2Bet == 1)
  {
    player2Raise = 1;
  }

  while ((player1Raise == 2 || player2Raise == 2) && (player1Raise != 0 && player2Raise != 0) && (player1Bet < player1->getLife() && player2Bet < player2->getLife()))
  {
    //This while loop allows the players to engage in a game of chicken,
    //where both players raise until one side folds or they reach all the life
    //they can bet. The loop continues as long as both players agree to raise
    //(i.e. player1Raise == 2 or player2Raise == 2 and neither side folds).
    player1Bet++;
    player2Bet++;
    player1Raise = rand() % 3;
    player2Raise = rand() % 3;
  }
  if (player1Raise == player2Raise && player1Raise == 0)
  {
    //This prevents the case where both players chose to fold.
    //If the betting amount is 1 (default value), set raise flag to 1
    //Otherwise, set to 2 (this is for the training data switch later)
    switch(player1Bet)
    {
      case 1: player1Raise = 1; break;
      default: player1Raise = 2; break;
    }
    switch(player2Bet)
    {
      case 1: player2Raise = 1; break;
      default: player2Raise = 2; break;
    }
  }


  //There are two conditions that a card wins the other:
  //Condition 1 - One player plays the card with the higher value. Neither side plays Ace.
  //Condition 2 - One player plays the Ace and the other player plays any card that is neither Ace nor 2.
  //Condition 3 - One player plays the Ace and the other player plays 2. Only 2 Beats Ace.
  //Condition 4 - One player chose to fold.
  //The variables below have been named as such. Any one of the three should be
  //true for a player to beat the opponent.
  bool player1cond1 = player1Value != PokerCards::ACE && player2Value != PokerCards::ACE && player1Value > player2Value;
  bool player1cond2 = player1Value == PokerCards::ACE && player2Value > 2;
  bool player1cond3 = player1Value == 2 && player2Value == PokerCards::ACE;
  bool player1cond4 = player2Raise == 0;
  bool player2cond1 = player1Value != PokerCards::ACE && player2Value != PokerCards::ACE && player2Value > player1Value;
  bool player2cond2 = player2Value == PokerCards::ACE && player1Value > 2;
  bool player2cond3 = player2Value == 2 && player1Value == PokerCards::ACE;
  bool player2cond4 = player1Raise == 0;

  //cout << "player1 cond1: " << player1cond1 << ", cond2: " << player1cond2 << ", cond3: " << player1cond3 << endl; //DEBUG
  //cout << "player2 cond1: " << player2cond1 << ", cond2: " << player2cond2 << ", cond3: " << player2cond3 << endl; //DEBUG

  //Whoever wins claims one life from the opponent. Draws do not affect anything.
  //For the switch cases of setScore() calls, punish the A.I. for losing lives
  //with one exception: Iff the A.I. chose to fold correctly (i.e. choosing to
  //fold when the opponent has a higher card), reward the A.I. even if folding
  //causes loss of life.
  //If any of player2cond1,2,3 is true and player 2 folded, it should still count
  //as player 1 win and vice versa. In this case, it is considered an incorrect
  //folding, and the player will be punished for the incorrect folding.
  //Reckless pushing is also punished. When the A.I. raises with two down cards
  //when the opponent has two up cards, the raise is suicidal. Hence, if the
  //opponent had a better card yet the A.I. raised, the A.I. is punished.
  if ((player1cond1 || player1cond2 || player1cond3 || player1cond4) && !player2cond4) //Player 1 wins
  {
    player1->setLife(player1->getLife() + player1Bet);
    player2->setLife(player2->getLife() - player2Bet);
    if (updown[3]) //Player 2 at 2 up
    {
      switch(player1Raise)
      {
        case 1: player1->setScore(0, player1Choice + 2, player1Bet); break;
        case 0: player1->setScore(0, player1Choice, player1Bet); break;
      }
      if (player2cond1 || player2cond2 || player2cond3)
      {
        player1->setScore(0, player1Choice + 4, -1 * player1Bet);
      }
      else
      {
        player1->setScore(0, player1Choice + 4, player1Bet);
      }
    }
    else if (updown[4]) //Player 2 at 1 up 1 down
    {
      switch(player1Raise)
      {
        case 1: player1->setScore(1, player1Choice + 2, player1Bet); break;
        case 0: player1->setScore(1, player1Choice, player1Bet); break;
      }
      if (player2cond1 || player2cond2 || player2cond3)
      {
        player1->setScore(1, player1Choice + 4, -1 * player1Bet);
      }
      else
      {
        player1->setScore(1, player1Choice + 4, player1Bet);
      }
    }
    else if (updown[5]) //Player 2 at 2 down
    {
      switch(player1Raise)
      {
        case 1: player1->setScore(2, player1Choice + 2, player1Bet); break;
        case 0: player1->setScore(2, player1Choice, player1Bet); break;
      }
      if (player2cond1 || player2cond2 || player2cond3)
      {
        player1->setScore(2, player1Choice + 4, -1 * player1Bet);
      }
      else
      {
        player1->setScore(2, player1Choice + 4, player1Bet);
      }
    }
    if (updown[0]) //Player 1 at 2 up
    {
      switch(player2Raise)
      {
        case 1: player2->setScore(0, player2Choice + 2, -1 * player2Bet); break;
        case 2: player2->setScore(0, player2Choice + 4, -1 * player2Bet); break;
      }
      if (player2cond1 || player2cond2 || player2cond3)
      {
        player2->setScore(0, player2Choice, -1 * player2Bet);
      }
      else
      {
        player2->setScore(0, player2Choice, player2Bet);
      }
    }
    else if (updown[1]) //Player 1 at 1 up 1 down
    {
      switch(player2Raise)
      {
        case 1: player2->setScore(1, player2Choice + 2, -1 * player2Bet); break;
        case 2: player2->setScore(1, player2Choice + 4, -1 * player2Bet); break;
      }
      if (player2cond1 || player2cond2 || player2cond3)
      {
        player2->setScore(1, player2Choice, -1 * player2Bet);
      }
      else
      {
        player2->setScore(1, player2Choice, player2Bet);
      }
    }
    else if (updown[2]) //Player 1 at 2 down
    {
      switch(player2Raise)
      {
        case 1: player2->setScore(2, player2Choice + 2, -1 * player2Bet); break;
        case 2: player2->setScore(2, player2Choice + 4, -1 * player2Bet); break;
      }
      if (player2cond1 || player2cond2 || player2cond3)
      {
        player2->setScore(2, player2Choice, -1 * player2Bet);
      }
      else
      {
        player2->setScore(2, player2Choice, player2Bet);
      }
    }
  }
  else if ((player2cond1 || player2cond2 || player2cond3 || player2cond4) && !player1cond4) //Player 2 wins
  {
    player1->setLife(player1->getLife() - player1Bet);
    player2->setLife(player2->getLife() + player2Bet);
    if (updown[3]) //Player 2 at 2 up
    {
      switch(player1Raise)
      {
        case 1: player1->setScore(0, player1Choice + 2, -1 * player1Bet); break;
        case 2: player1->setScore(0, player1Choice + 4, -1 * player1Bet); break;
      }
      if (player1cond1 || player1cond2 || player1cond3)
      {
        player1->setScore(0, player1Choice, -1 * player1Bet);
      }
      else
      {
        player1->setScore(0, player1Choice, player1Bet);
      }
    }
    else if (updown[4]) //Player 2 at 1 up 1 down
    {
      switch(player1Raise)
      {
        case 1: player1->setScore(1, player1Choice + 2, -1 * player1Bet); break;
        case 2: player1->setScore(1, player1Choice + 4, -1 * player1Bet); break;
      }
      if (player1cond1 || player1cond2 || player1cond3)
      {
        player1->setScore(1, player1Choice, -1 * player1Bet);
      }
      else
      {
        player1->setScore(1, player1Choice, player1Bet);
      }
    }
    else if (updown[5]) //Player 2 at 2 down
    {
      switch(player1Raise)
      {
        case 1: player1->setScore(2, player1Choice + 2, -1 * player1Bet); break;
        case 2: player1->setScore(2, player1Choice + 4, -1 * player1Bet); break;
      }
      if (player1cond1 || player1cond2 || player1cond3)
      {
        player1->setScore(2, player1Choice, -1 * player1Bet);
      }
      else
      {
        player1->setScore(2, player1Choice, player1Bet);
      }
    }
    if (updown[0]) //Player 1 at 2 up
    {
      switch(player2Raise)
      {
        case 1: player2->setScore(0, player2Choice + 2, player2Bet); break;
        case 0: player2->setScore(0, player2Choice, player2Bet); break;
      }
      if (player1cond1 || player1cond2 || player1cond3)
      {
        player2->setScore(0, player2Choice + 4, -1 * player2Bet);
      }
      else
      {
        player2->setScore(0, player2Choice + 4, player2Bet);
      }
    }
    else if (updown[1]) //Player 1 at 1 up 1 down
    {
      switch(player2Raise)
      {
        case 1: player2->setScore(1, player2Choice + 2, player2Bet); break;
        case 0: player2->setScore(1, player2Choice, player2Bet); break;
      }
      if (player1cond1 || player1cond2 || player1cond3)
      {
        player2->setScore(1, player2Choice + 4, -1 * player2Bet);
      }
      else
      {
        player2->setScore(1, player2Choice + 4, player2Bet);
      }
    }
    else if (updown[2]) //Player 1 at 2 down
    {
      switch(player2Raise)
      {
        case 1: player2->setScore(2, player2Choice + 2, player2Bet); break;
        case 0: player2->setScore(2, player2Choice, player2Bet); break;
      }
      if (player1cond1 || player1cond2 || player1cond3)
      {
        player2->setScore(2, player2Choice + 4, -1 * player2Bet);
      }
      else
      {
        player2->setScore(2, player2Choice + 4, player2Bet);
      }
    }
  }

  //cout << "Player 1 life: " << player1->getLife() << endl; //DEBUG
  //cout << "Player 2 life: " << player2->getLife() << endl; //DEBUG

  //cout << "After check/comparing card values." << endl;//DEBUG
  //Now that the round is over, each player draws a new card.
  player1->replaceCard(deck.back(), player1Choice);
  deck.pop_back();
  player2->replaceCard(deck.back(), player2Choice);
  deck.pop_back();
}


/*
 * Plays a round of One Poker. This is used for the game between player and
 * computer. The second OPContestant object is always the computer.
 * @params  The two instances of OPContestants that will engage in the game
 * @param   The deck of cards used for this game
 */
void playRound(OPContestant *& player1, OPContestant *& player2, vector<PokerCards*> & deck)
{
  cout << "Your life count: " << player1->getLife() << ", Computer life count: " << player2->getLife() << endl;
  //cout << "Prior to creating updown vector." << endl;//DEBUG
  //Check for the number of ups and downs for each player
  vector<bool> updown(6);
  fill(updown.begin(), updown.end(), false);
  //cout << "After creating updown array and prior to checking updowns." << endl;//DEBUG
  checkUpDown(player1, player2, updown);
  //cout << "After checking updowns and prior to check/comparing card values." << endl;//DEBUG

  if (updown[3])
  {
    cout << "Computer has two up cards, ";
  }
  else if (updown[4])
  {
    cout << "Computer has one up card and one down card, ";
  }
  else if (updown[5])
  {
    cout << "Computer has two down cards, ";
  }
  if (updown[0])
  {
    cout << "and you have two up cards." << endl;
  }
  else if (updown[1])
  {
    cout << "and you have one up card and one down card." << endl;
  }
  else if (updown[2])
  {
    cout << "and you have two down cards." << endl;
  }

  string player1Choice;
  int player1ChoiceInt;
  bool validInput = false; //Player provided a valid input of an integer.
  bool validPick = false; //Player picked 1 or 2.
  while(!validPick)
  {
    cout << "Your cards: " << player1->cardToString(0) << " and " << player1->cardToString(1) << endl;
    cout << "Pick a card by typing 1 for the first card and 2 for the second card. ";
    getline(cin, player1Choice);
    cout << endl;
    validInput = isValidInput(player1Choice);
    while(!validInput)
    {
      cout << "You have provided an invalid input." << endl;
      cout << "Your cards: " << player1->cardToString(0) << " and " << player1->cardToString(1) << endl;
      cout << "Pick a card by typing 1 for the first card and 2 for the second card. ";
      getline(cin, player1Choice);
      validInput = isValidInput(player1Choice);
      cout << endl;
    }
    player1ChoiceInt = atoi(player1Choice.c_str()) - 1;
    validPick = player1ChoiceInt == 0 || player1ChoiceInt == 1;
    if (!validPick)
    {
      cout << "Please choose between card 1 or card 2." << endl;
    }
  }

  int player1bet = 1;
  int player2bet = 1;

  string player1betInquire;
  int player1betInquireInt;
  bool player1raise = false;
  bool player1fold = false;
  validPick = false;
  while(!validPick)
  {
    cout << "Would you like to raise your bet?" << endl;
    cout << "Type 1 and press enter to check or 2 to raise the bet. ";
    getline(cin, player1betInquire);
    cout << endl;
    validInput = isValidInput(player1betInquire);
    while(!validInput)
    {
      cout << "You have provided an invalid input." << endl;
      cout << "Type 1 and press enter to check or 2 to raise the bet. ";
      getline(cin, player1betInquire);
      validInput = isValidInput(player1betInquire);
      cout << endl;
    }
    player1betInquireInt = atoi(player1betInquire.c_str());

    validPick = player1betInquireInt == 1 || player1betInquireInt == 2;
    if (!validPick)
    {
      cout << "Please choose between check (type 1) or raise (type 2)." << endl;
    }
    else if (player1betInquireInt == 2)
    {
      player1raise = true;
      player1bet++;
    }
  }

  //THIS IS THE A.I. PART.
  //Computer decision process:
  //1.1 IF player chose to raise, decide whether to additionally raise or check/fold.
  //1.2 Otherwise, decide whether to raise or check.
  //2. Pick the card to play.
  int player2choice, maximumChoice;
  bool player2raise = false;
  bool player2fold = false;
  if (updown[0])
  {
    //Player has two up cards.
    //Pick the index with the maximum score.
    //0 = play 1st card and fold
    //1 = play 2nd card and fold
    //2 = play 1st card and check
    //3 = play 2nd card and check
    //4 = play 1st card and raise
    //5 = play 2nd card and raise
    maximumChoice = player2->getMaxIndex(0, player1raise);
  }
  else if (updown[1])
  {
    //Player has one up card and one down card.
    maximumChoice = player2->getMaxIndex(1, player1raise);
  }
  else //if (updown[2])
  {
    //Player has two down cards.
    maximumChoice = player2->getMaxIndex(2, player1raise);
  }

  player2choice = maximumChoice % 2;
  if ((maximumChoice / 2) == 2)
  {
    player2raise = true;
  }
  else if ((maximumChoice / 2) == 0)
  {
    player2fold = true;
  }
  else if ((maximumChoice / 2) == 1 && player1raise)
  {
    player2bet++;
  }


  if (player2raise)
  {
    if (!player1raise)
    {
      player2bet++;
    }
    else
    {
      player2bet += 2;
    }
    cout << "Computer decided to raise. Would you like to raise the bet even higher, check, or fold?" << endl;
    validPick = false;
    while(!validPick)
    {
      cout << "Type 0 and press enter to fold, 1 to check, or 2 to raise the bet. ";
      getline(cin, player1betInquire);
      cout << endl;
      validInput = isValidInput(player1betInquire);
      while(!validInput)
      {
        cout << "You have provided an invalid input." << endl;
        cout << "Type 0 and press enter to fold, 1 to check, or 2 to raise the bet. ";
        getline(cin, player1betInquire);
        validInput = isValidInput(player1betInquire);
        cout << endl;
      }
      player1betInquireInt = atoi(player1betInquire.c_str());
      validPick = player1betInquireInt == 1 || player1betInquireInt == 2 || player1betInquireInt == 0;
      if (!validPick)
      {
        cout << "Please choose between fold (type 0), check (type 1), or raise (type 2)." << endl;
      }
      else if (player1betInquireInt == 2)
      {
        player1raise = true;
        player1bet += 2;
        player2bet++;
        cout << "You challenged the computer by upping the ante!" << endl;
      }
      else if (player1betInquireInt == 1)
      {
        player1bet++;
        cout << "You accepted the raise." << endl;
      }
      else if (player1betInquireInt == 0)
      {
        player1fold = true;
      }
    }
  }


  if (player2fold)
  {
    cout << "Computer folded." << endl;
  }
  else if (player1fold)
  {
    cout << "You decided to fold." << endl;
  }

  cout << "Computer picked " << player2->cardToString(player2choice) << ". ";

  int player1Value = player1->seeCardValue(player1ChoiceInt);
  int player2Value = player2->seeCardValue(player2choice);

  //cout << "player1 card choices: " << player1->seeCardValue(0) << " and " << player1->seeCardValue(1) << " | player1 picked " << player1->seeCardValue(player1ChoiceInt) << endl; //DEBUG
  //cout << "player2 card choices: " << player2->seeCardValue(0) << " and " << player2->seeCardValue(1) << " | player2 picked " << player2->seeCardValue(player2Choice) << endl; //DEBUG

  //There are two conditions that a card wins the other:
  //Condition 1 - One player plays the card with the higher value. Neither side plays Ace.
  //Condition 2 - One player plays the Ace and the other player plays any card that is neither Ace nor 2.
  //Condition 3 - One player plays the Ace and the other player plays 2. Only 2 Beats Ace.
  //The variables below have been named as such. Any one of the three should be
  //true for a player to beat the opponent.
  bool player1cond1 = player1Value != PokerCards::ACE && player2Value != PokerCards::ACE && player1Value > player2Value;
  bool player1cond2 = player1Value == PokerCards::ACE && player2Value > 2;
  bool player1cond3 = player1Value == 2 && player2Value == PokerCards::ACE;
  bool player2cond1 = player1Value != PokerCards::ACE && player2Value != PokerCards::ACE && player2Value > player1Value;
  bool player2cond2 = player2Value == PokerCards::ACE && player1Value > 2;
  bool player2cond3 = player2Value == 2 && player1Value == PokerCards::ACE;

  //cout << "player1 cond1: " << player1cond1 << ", cond2: " << player1cond2 << ", cond3: " << player1cond3 << endl; //DEBUG
  //cout << "player2 cond1: " << player2cond1 << ", cond2: " << player2cond2 << ", cond3: " << player2cond3 << endl; //DEBUG

  //Whoever wins claims one life from the opponent. Draws do not affect anything.
  if ((player1cond1 || player1cond2 || player1cond3 || player2fold) && !player1fold) //Player 1 wins
  {
    player1->setLife(player1->getLife() + player2bet);
    player2->setLife(player2->getLife() - player2bet);
    switch(player2bet)
    {
      case 1: cout << "You win one life." << endl; break;
      default: cout << "You win " << player2bet << " lives." << endl; break;
    }
  }
  else if ((player2cond1 || player2cond2 || player2cond3 || player1fold) && !player2fold) //Player 2 wins
  {
    player1->setLife(player1->getLife() - player1bet);
    player2->setLife(player2->getLife() + player1bet);
    switch(player1bet)
    {
      case 1: cout << "You lose one life." << endl; break;
      default: cout << "You lose " << player1bet << " lives." << endl; break;
    }
  }
  else
  {
    cout << "The round ends in a draw." << endl;
  }
  //cout << "Player 1 life: " << player1->getLife() << endl; //DEBUG
  //cout << "Player 2 life: " << player2->getLife() << endl; //DEBUG

  //cout << "After check/comparing card values." << endl;//DEBUG
  //Now that the round is over, each player draws a new card.
  player1->replaceCard(deck.back(), player1ChoiceInt);
  deck.pop_back();
  player2->replaceCard(deck.back(), player2choice);
  deck.pop_back();

  //cout << "After the draws." << endl; //DEBUG
}

/*
 * Helper method to print error message when invalid command line arguments
 * are used.
 * @param  errorType
 *         1 if input was faulty (e.g. alphabet characters used, or negative
             numbers for life count)
           2 if more arguments than required were typed in
           3 if fewer arguments than required were typed in
           4 if incorrect setting number was typed in.
 * @param  argCount number of arguments client entered in command line.
 */
void printErrorMsg(int errorType, int argCount)
{
  if (errorType == 1)
  {
    cout << "Invalid input detected. Type in valid integers to pass in as parameters." << endl;
    cout << "Command line optional argument format: " << endl;
  }
  else if (errorType == 2)
  {
    cout << "You have entered too many arguments. This program requires 3 arguments for custom settings." << endl;
    cout << "Command line optional argument format: " << endl;
  }
  else if (errorType == 3)
  {
    cout << "You have entered " << (argCount - 1) << " arguments. This program requires 3 arguments for custom settings." << endl;
    cout << "Command line optional argument format: " << endl;
  }
  else if (errorType == 4)
  {
    cout << "You have typed in an invalid setting number. Type 1 or 2 for custom settings." << endl;
    cout << "Command line optional argument format: " << endl;
  }
  cout << "./OnePokerSim <setting> <player's life count> <opponent's life count>" << endl;
  cout << "setting = 1 for 'Kaiji setting': player starts with 2 lives and computer starts with 10 lives" << endl;
  cout << "setting = 2 for custom settings: Client can choose life count for each player" << endl;
  cout << "If no arguments are used, game will proceed with default settings of 6:6 life count." << endl;
}

/*
 * Main method. See class comments for instructions on
 * how to use optional command line arguments.
 * @params  Optional setting, life count for player and computer
 */
int main(int argc, char * argv[])
{
  srand(time(NULL)); //Used for random number generation for the games.

  //Use default constructors for initial simulation that will be used for
  //the reinforced machine learning
  OPContestant * com1 = new OPContestant();
  OPContestant * com2 = new OPContestant();

  //The player's instance will be created after we check for the optional
  //command line arguments.
  OPContestant * player;

  // variable that will be used to store the opponent's life count.
  int opponentlife;

  for (int argi = 1 ; argi < argc ; argi++)
  {
    if (!isValidInput(argv[argi]))
    {
      printErrorMsg(1, 0);
      return 0;
    }
  }

  //At this point, all input is guaranteed to be numerical values
  if (argc == 4 || (argc == 2 && atoi(argv[1]) == 1))
  {
    int setting = atoi(argv[1]);
    if (setting == 1) //Kaiji settings
    {
      player = new OPContestant(KAIJI_LIFE_COUNT_PLAYER);
      opponentlife = KAIJI_LIFE_COUNT_COMPUTER;
    }
    else if (setting == 2) //Custom settings
    {
      player = new OPContestant(atoi(argv[2]));
      opponentlife = atoi(argv[3]);
    }
    else //Invalid input
    {
      printErrorMsg(4, 0);
      return 0;
    }
  }
  else if (argc == 1) //No optional arguments used. Proceed with default settings.
  {
    player = new OPContestant();
    opponentlife = DEFAULT_LIFE_COUNT;
  }
  else if (argc == 2 || argc == 3) //fewer arguments detected. Display warning and close program
  {
    printErrorMsg(3, argc);
    return 0;
  }
  else //too many arguments detected. Display warning and close program
  {
    printErrorMsg(2, argc);
    return 0;
  }



  int trainingCount = 100000; //Number of training runs for the reinforced machine learning

  vector<PokerCards*> deck;

  while (trainingCount > 0)
  {
    generateShuffledDeck(deck);
    com1->addCard(deck.back());
    deck.pop_back();
    com2->addCard(deck.back());
    deck.pop_back();
    com1->addCard(deck.back());
    deck.pop_back();
    com2->addCard(deck.back());
    deck.pop_back();

    while (com1->getLife() != 0 && com2->getLife() != 0)
    {
      playRoundTraining(com1, com2, deck);

      //If all cards have been consumed, regenerate a randomly shuffled deck
      if (deck.empty())
      {
        generateShuffledDeck(deck);
      }
    }
    com1->resetHand(DEFAULT_LIFE_COUNT);
    com2->resetHand(DEFAULT_LIFE_COUNT);
    trainingCount--;
  }
  while (!deck.empty())
  {
    delete deck.back();
    deck.pop_back();
  }

  //DEBUG BLOCK
  /*cout << "com1 results:" << endl;
  com1->printEverything();
  cout << "com2 results:" << endl;
  com2->printEverything();*/
  //cout << "" << endl;
  com1->combine(com2, opponentlife);
  com2->resetComplete();
  //cout << "com1 results after combination:" << endl; //DEBUG
  //com1->printEverything();

  generateShuffledDeck(deck);
  com1->addCard(deck.back());
  deck.pop_back();
  player->addCard(deck.back());
  deck.pop_back();
  com1->addCard(deck.back());
  deck.pop_back();
  player->addCard(deck.back());
  deck.pop_back();

  string blank; //Dummy variable used for the "press enter key to continue."
  while (com1->getLife() > 0 && player->getLife() > 0)
  {
    playRound(player, com1, deck);

    cout << "Press the enter key to continue.";
    getline(cin, blank);
    cout << endl;

    //If all cards have been consumed, regenerate a randomly shuffled deck
    if (deck.empty())
    {
      generateShuffledDeck(deck);
    }
  }

  if (com1->getLife() <= 0)
  {
    cout << "You win!" << endl;
  }
  else
  {
    cout << "You lose..." << endl;
  }

  com1->resetComplete();
  player->resetComplete();
  delete com1;
  delete com2;
  delete player;
  return 0;
}
