/*
 * Class OPContestant
 * Based on the Anime 'Kaiji', each contestant is given
 * 6 lives and 2 cards from a regular deck of poker cards.
 * The main method of OnePokerSim.cpp will perform
 * the actual games.
 *
 * Written by Vincent Yang 1/4/2019
 * This is a work in progress!
 */

#ifndef OPCONTESTANT_H
#define OPCONTESTANT_H

#include <vector>
#include "PokerCards.h"

class OPContestant
{
  public:
    /*
     * Default constructor of a contestant instance
     */
    OPContestant();

    /*
     * Custom constructor; takes number of lives in as parameter
     */
    OPContestant(int lifeCount);

    /*
     * Check the number of lives the contestant holds
     */
    int getLife();

    /*
     * Modify the number of lives the contestant holds
     */
    void setLife(int newLifeCount);

    /*
     * Checks the value of the card picked by the player.
     * @param  index of card in the hand. 0 for higher value,
     *         and 1 for lower value.
     */
    int seeCardValue(int choice);

    /*
     * Returns a given card choice in string form.
     * @param  index of card in the hand. 0 for higher value,
     *         and 1 for lower value.
     */
    std::string cardToString(int choice);

    /*
     * Add a new card to the player's hand. Returns true iff the card was
     * successfully added to the player's hand. Returns false if the player
     * already has two cards in their hand.
     */
    bool addCard(PokerCards *& newCard);

    /*
     * Choose a card from the player's hand and replace it with a new card.
     * Takes in the new card as well as the player's choice represented by
     * numbers 1 or 2 as parameters (see OnePokerSim.cpp for details).
     * This method will be called after one round of One Poker
     */
    void replaceCard(PokerCards *& newCard, int choice);

    /*
     * View the score for current hand and determine which card to play.
     * @param  scenario = 0 for score table A, 1 for table B, 2 for table C
     * @param  choice = 0 for higher value card, 1 for lower value card
     */
    int getScore(int scenario, int choice);

    /*
     * Change the score for current hand and card that has been played.
     * @param  scenario = 0 for score table A, 1 for table B, 2 for table C
     * @param  choice = 0 for higher value card, 1 for lower value card
     * @param  increment/decrement to the score
     */
    void setScore(int scenario, int choice, int changeScore);

    /*
     * Deletes all unused PokerCard objects and sets life back to default.
     * @param  number of lives to reset to
     */
    void resetHand(int lives);

    /*
     * Method preceding all delete calls to OPContestant instances.
     * deletes all arrays in use by the score tables as well as unused
     * PokerCard objects.
     */
    void resetComplete();

    /*
     * Method that combines the score tables of two instances of OPContestant.
     * It can also change the life count of the resulting OPContestant instance.
     * It will always reset the current object's hand.
     * @param  Pointer to the OPContestant to combine with the current instance.
     * @param  the life count of the resulting combined instance. Set to 0 to
     *         leave the life count unchanged.
     */
    void combine(OPContestant * other, int newLifeCount);

    /*
     * DEBUG method! DELETE AFTER PROGRAM IS COMPLETE
     */
    void printEverything();

  private:
    /*
     * Number of lives held by contestant
     */
    int contestantLife;

    /*
     * For the reinforced machine learning to take place, this class will
     * maintain several tables depending on which scenario the contestant
     * is facing. There are three possible scenarios the contestant will be
     * facing (see file comments in OnePokerSim.cpp for terminology):
     *
     * Scenario A: Opponent has two up
     * Scenario B: Opponent has one up one down
     * Scenario C: Opponent has two down
     *
     * The index of the row will indicate the value of the card with the
     * greater value. The index of the column will indicate the value of the
     * lower card.
     */

    /*
     * Table of "scores" for scenario A
     */
    int*** scoreA;

    /*
     * Table of "scores" for scenario B
     */
    int*** scoreB;

    /*
     * Table of "scores" for scenario C
     */
    int*** scoreC;

    /*
     * Player's hand
     */
    std::vector<PokerCards*> hand;

    /*
     * Helper method to initialize the three tables above
     */
    void initializeTable();

    /*
     * Arranges cards in hand such that the first card is higher in value
     * than the second card
     */
    void arrangeHand();
};

#endif
