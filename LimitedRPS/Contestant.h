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

#ifndef CONTESTANT_H
#define CONTESTANT_H

#include <vector>

class Contestant
{
  public:
    //Default constructor
    Contestant();

    //Custom constructor only for repeaters
    //More on repeaters on LimitedRPS.cpp. Also see
    //commens on the boolean variable repeater below.
    Contestant(bool isRepeater);

    //Obtains number of stars (lives) held by current contestant
    int getStars();

    //Changes the number of stars held by current contestant
    void setStars(int newStarCount);

    //Obtains number of cards (rocks, papers, or scissors)
    //held by current contestant
    int getCard(int cardIndex);

    //Decreases number of cards (rocks, papers, or scissors)
    //held by current contestant. There is no increment because
    //in this version, the card count can only go down.
    void decreaseCard(int cardIndex);

    //Accessor which determines if current contestant is a repeater
    bool isRepeater();

    //Checks if the contestant has completely consumed one type of card
    int hasConsumedOneType();

    //Checks if the contestant has only one type of card remaining
    int hasOnlyOneType();

    //Checks if the contestant has no cards remaining
    bool noCardsLeft();

    //DEBUG methods listed below
    bool negativeCards();

  private:
    //Number of lives for each player
    int stars;
    //Index 0 contains number of rocks
    //Index 1 contains number of papers
    //Index 2 contains number of scissors
    std::vector<int> cards;
    //Sets the endgame requirement for this player to be higher
    //If set to true, this player needs to have 4 or more stars to win
    bool repeater;

    //Prevent the copying of contestant objects
    Contestant &operator =(const Contestant &);
    Contestant(const Contestant &);
};

#endif
