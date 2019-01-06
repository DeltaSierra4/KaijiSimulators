/*
 * Class PokerCards
 * Represents a Poker card.
 *
 * Written by Vincent Yang 12/30/2018
 */

#include <sstream>
#include <string>
#include "PokerCards.h"

typedef int suit;

using namespace std;

PokerCards::PokerCards(suit suitType, int cardValue)
{
  this->cardSuit = suitType;
  this->value = cardValue;
}

suit PokerCards::getSuit()
{
  return this->cardSuit;
}

int PokerCards::getValue()
{
  return this->value;
}

string PokerCards::to_string()
{
  string suitAndValue;
  if (this->value > PokerCards::KING || this->value < PokerCards::ACE)
  {
    return "ILLEGAL CARD";
  }
  switch(this->value)
  {
    case PokerCards::ACE : suitAndValue = "Ace";
                             break;
    case PokerCards::JACK : suitAndValue = "Jack";
                                break;
    case PokerCards::QUEEN : suitAndValue = "Queen";
                              break;
    case PokerCards::KING : suitAndValue = "King";
                              break;
    default : stringstream ss;
              ss << this->value;
              suitAndValue = ss.str();
              break;
  }
  switch(this->cardSuit)
  {
    case PokerCards::CLUBS : suitAndValue += " of Clubs";
                             break;
    case PokerCards::DIAMONDS : suitAndValue += " of Diamonds";
                                break;
    case PokerCards::SPADES : suitAndValue += " of Spades";
                              break;
    case PokerCards::HEARTS : suitAndValue += " of Hearts";
                              break;
    default : suitAndValue = "ILLEGAL CARD";
              break;
  }
  return suitAndValue;
}
