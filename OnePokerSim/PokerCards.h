/*
 * Class PokerCards
 * Represents a Poker card.
 *
 * Written by Vincent Yang 12/30/2018
 */

#ifndef POKERCARDS_H
#define POKERCARDS_H

#include <string>

typedef int suit;

class PokerCards
{
  public:
    /*
     * Custom constructor; takes the suit type and value of card as parameters
     */
    PokerCards(suit suitType, int cardValue);

    /*
     * Check the suit of this card
     */
    suit getSuit();


    /*
    * Check the value of this card
    */
    int getValue();

    /*
     * obtain the suit and value of the card in string format
     */
    std::string to_string();

    /*
     * Suits and letter cards represented as static final variables
     */
    static const int CLUBS = 1;
    static const int DIAMONDS = 2;
    static const int SPADES = 3;
    static const int HEARTS = 4;
    static const int ACE = 1;
    static const int JACK = 11;
    static const int QUEEN = 12;
    static const int KING = 13;
    static const int JOKER = 0;
    static const int TOTAL_CARD_COUNT = 52;

  private:
    /*
     * The suit of the card
     */
    suit cardSuit;

    /*
     * The value of the card
     */
    int value;
};

#endif
