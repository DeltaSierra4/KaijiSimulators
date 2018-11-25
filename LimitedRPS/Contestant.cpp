/*
 * Class Contestant
 * Based on the Anime 'Kaiji', each contestant is given
 * 12 cards, four of each rock, paper, and scissors.
 * They are also given three stars, each of which represent
 * a life. The main method of LimitedRPS.cpp will perform
 * the actual games
 *
 * V 1.0.0. Written by DeltaSierra4 (Vincent Yang) 11/24/2018
 */

#include "Contestant.h"

using namespace std;

/*
 * Default constructor of Contestant. Initializes number of
 * stars to 3 and sets the number of cards to 4 each.
 * Also assumes this contestant is not a repeater.
 */
Contestant::Contestant()
{
  this->stars = 3; //Initial number of lives is always 3.
  this->cards = vector<int>(3, 4); //Three types of cards - Rock, paper, scissors
  //Four per card type.
  this->repeater = false;
}

/*
 * Custom constructor of Contestant. Initializes number of
 * stars to 3 and sets the number of cards to 4 each.
 * Sets the repeater parameter to whatever the client sets it to.
 * @param  whether this contestant is a repeater or not
 */
Contestant::Contestant(bool isRepeater)
{
  this->stars = 3; //Initial number of lives is always 3.
  this->cards = vector<int>(3, 4); //Three types of cards - Rock, paper, scissors
  //Four per card type.
  this->repeater = isRepeater;
}

/*
 * Returns the number of stars kept by this contestant
 * @returns number of stars of this contestant
 */
int Contestant::getStars()
{
  return this->stars;
}

/*
 * Mutator to set the number of stars kept by this contestant
 * @param  new number of stars of this contestant
 */
void Contestant::setStars(int newStarCount)
{
  this->stars = newStarCount;
}

/*
 * Returns the number of cards of a specific type
 * kept by this contestant
 * @param  card type. 0 for number of rocks, 1 for papers, 2 for scissors
 * @returns number of stars of this contestant
 * PRE: cardIndex is 0, 1, or 2.
 */
int Contestant::getCard(int cardIndex)
{
  return this->cards[cardIndex];
}

/*
 * Decrements the number of cards of a specific type
 * kept by this contestant
 * @param  card type. 0 for number of rocks, 1 for papers, 2 for scissors
 * PRE: cardIndex is 0, 1, or 2.
 */
void Contestant::decreaseCard(int cardIndex)
{
  (this->cards[cardIndex])--;
}

/*
 * Accessor which determines if current contestant is a repeater
 * @returns  true iff this Contestant is a repeater.
 */
bool Contestant::isRepeater()
{
  return this->repeater;
}

/*
 * Checks if the contestant has completely consumed one type of card
 * @returns  the type of card that is consumed. If player still has all three types
 *           returns -1
 */
int Contestant::hasConsumedOneType()
{
  if ((this->cards[0] == 0) && (this->cards[1] != 0) && (this->cards[2] != 0)) {
    return 0;
  } else if ((this->cards[0] != 0) && (this->cards[1] == 0) && (this->cards[2] != 0)) {
    return 1;
  } else if ((this->cards[0] != 0) && (this->cards[1] != 0) && (this->cards[2] == 0)) {
    return 2;
  } else {
    return -1;
  }
}

/*
 * Checks if the contestant has only one type of card left
 * @returns  the type of card that is remaining. If player still has more than one type,
 *           returns -1
 */
int Contestant::hasOnlyOneType()
{
  if ((this->cards[0] == 0) && (this->cards[1] == 0) && (this->cards[2] != 0)) {
    return 2;
  } else if ((this->cards[0] == 0) && (this->cards[1] != 0) && (this->cards[2] == 0)) {
    return 1;
  } else if ((this->cards[0] != 0) && (this->cards[1] == 0) && (this->cards[2] == 0)) {
    return 0;
  } else {
    return -1;
  }
}

/*
 * Checks if the contestant has no cards left
 * @returns  true iff contestant has no cards left.
 */
bool Contestant::noCardsLeft()
{
  return ((this->cards[0] == 0) && (this->cards[1] == 0) && (this->cards[2] == 0));
}

/*
 * Method used for debugging. Checks if any card count is negative
 * @returns  true iff any card count is negative.
 */
bool Contestant::negativeCards()
{
  return ((this->cards[0] < 0) && (this->cards[1] < 0) && (this->cards[2] < 0));
}
