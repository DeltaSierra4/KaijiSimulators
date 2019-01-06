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

#include <vector>
#include <iostream>
#include "OPContestant.h"
#include "PokerCards.h"

using namespace std;

OPContestant::OPContestant()
{
  //Default life count for a contestant is 10
  this->contestantLife = 10;
  initializeTable();
}

OPContestant::OPContestant(int lifeCount)
{
  this->contestantLife = lifeCount;
  initializeTable();
}

void OPContestant::initializeTable()
{
  //The table to keep track of scores will have the same size
  //as the number of cards (13 cards per suit, therefore 13x13 table)
  this->scoreA = new int**[PokerCards::KING];
  this->scoreB = new int**[PokerCards::KING];
  this->scoreC = new int**[PokerCards::KING];
  for (unsigned row = 0 ; row < PokerCards::KING ; row++)
  {
    this->scoreA[row] = new int*[PokerCards::KING];
    this->scoreB[row] = new int*[PokerCards::KING];
    this->scoreC[row] = new int*[PokerCards::KING];
    for (unsigned col = 0 ; col < PokerCards::KING ; col++)
    {
      this->scoreA[row][col] = new int[2];
      this->scoreB[row][col] = new int[2];
      this->scoreC[row][col] = new int[2];
      for (unsigned ind = 0 ; ind < 6 ; ind++)
      {
        //Index 0,1 - choosing to fold with a given card choice
        //Index 2,3 - choosing to check with a given card choice
        //Index 4,5 - choosing to raise with a given card choice
        this->scoreA[row][col][ind] = 0;
        this->scoreB[row][col][ind] = 0;
        this->scoreC[row][col][ind] = 0;
      }
    }
  }
}

int OPContestant::getLife()
{
  return this->contestantLife;
}

void OPContestant::setLife(int newLifeCount)
{
  this->contestantLife = newLifeCount;
}

int OPContestant::seeCardValue(int choice)
{
  return this->hand[choice]->getValue();
}

string OPContestant::cardToString(int choice)
{
  return this->hand[choice]->to_string();
}

bool OPContestant::addCard(PokerCards *& newCard)
{
  if (this->hand.size() > 2)
  {
    return false;
  }
  this->hand.push_back(newCard);
  if (this->hand.size() == 2)
  {
    this->arrangeHand();
  }
  return true;
}

void OPContestant::replaceCard(PokerCards *& newCard, int choice)
{
  delete this->hand[choice];
  this->hand[choice] = newCard;
  //delete deleteCard;
  this->arrangeHand();
}

int OPContestant::getScore(int scenario, int choice)
{
  //All indices will have a one-off error due to 0-based indexing
  switch(scenario)
  {
    case 0 : return scoreA[this->seeCardValue(0) - 1][this->seeCardValue(1) - 1][choice];
    case 1 : return scoreB[this->seeCardValue(0) - 1][this->seeCardValue(1) - 1][choice];
    case 2 : return scoreC[this->seeCardValue(0) - 1][this->seeCardValue(1) - 1][choice];
    default : cout << "Invalid case number detected." << endl;
              return 0;
  }
}

void OPContestant::setScore(int scenario, int choice, int changeScore)
{
  //Same with getScore(), all indices will have a one-off error
  switch(scenario)
  {
    case 0 : scoreA[this->seeCardValue(0) - 1][this->seeCardValue(1) - 1][choice] += changeScore;
             break;
    case 1 : scoreB[this->seeCardValue(0) - 1][this->seeCardValue(1) - 1][choice] += changeScore;
             break;
    case 2 : scoreC[this->seeCardValue(0) - 1][this->seeCardValue(1) - 1][choice] += changeScore;
             break;
    default : cout << "Invalid case number detected." << endl;
              break;
  }
}

void OPContestant::arrangeHand()
{
  if (this->seeCardValue(0) != this->seeCardValue(1))
  {
    //Only perform the swap if the two card values are not the same.
    if (this->seeCardValue(0) < this->seeCardValue(1) && this->seeCardValue(0) != PokerCards::ACE && this->seeCardValue(1) != PokerCards::ACE)
    {
      //Only swap the positions of both cards if the first card has a lower
      //value than the second card. This also applies only if both cards are not ace.
      PokerCards * temp = this->hand[0];
      this->hand[0] = this->hand[1];
      this->hand[1] = temp;
    }
    else if (this->seeCardValue(1) == PokerCards::ACE)
    {
      //The swap should also happen if the second card happens to be an ace,
      //regardless of the value of the first card, because ace is the highest value.
      PokerCards * temp = this->hand[0];
      this->hand[0] = this->hand[1];
      this->hand[1] = temp;
    }
  }
}

void OPContestant::resetHand(int lives=0)
{
  /*delete this->hand[1];
  this->hand[1] = NULL;
  delete this->hand[0];
  this->hand[0] = NULL;*/
  while (!this->hand.empty())
  {
    delete this->hand.back();
    this->hand.pop_back();
  }
  this->setLife(lives);
}

void OPContestant::resetComplete()
{
  this->resetHand();
  for (unsigned row = 0 ; row < PokerCards::KING ; row++)
  {
    for (unsigned col = 0 ; col < PokerCards::KING ; col++)
    {
      delete[] this->scoreA[row][col];
      delete[] this->scoreB[row][col];
      delete[] this->scoreC[row][col];
    }
    delete[] this->scoreA[row];
    delete[] this->scoreB[row];
    delete[] this->scoreC[row];
  }
  delete[] this->scoreA;
  delete[] this->scoreB;
  delete[] this->scoreC;
}

void OPContestant::combine(OPContestant * other, int newLifeCount)
{
  for (unsigned row = 0 ; row < PokerCards::KING ; row++)
  {
    for (unsigned col = 0 ; col < PokerCards::KING ; col++)
    {
      for (unsigned ind = 0 ; ind < 2 ; ind++)
      {
        this->scoreA[row][col][ind] += other->scoreA[row][col][ind];
        this->scoreB[row][col][ind] += other->scoreB[row][col][ind];
        this->scoreC[row][col][ind] += other->scoreC[row][col][ind];
      }
    }
  }
  if (newLifeCount == 0)
  {
    this->resetHand(this->getLife());
  }
  else
  {
    this->resetHand(newLifeCount);
  }
}

void OPContestant::printEverything()
{
  cout << "Results for scenario A:" << endl;
  cout << "Scores for Jack, 10: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreA[PokerCards::JACK][10][ind] << ", ";
  }
  cout << this->scoreA[PokerCards::JACK][10][5] << endl;

  cout << "Scores for Jack, 4: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreA[PokerCards::JACK][4][ind] << ", ";
  }
  cout << this->scoreA[PokerCards::JACK][4][5] << endl;

  cout << "Scores for 4, 2: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreA[4][2][ind] << ", ";
  }
  cout << this->scoreA[4][2][5] << endl;

  cout << "" << endl;

  cout << "Results for scenario B:" << endl;
  cout << "Scores for Jack, 10: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreB[PokerCards::JACK][10][ind] << ", ";
  }
  cout << this->scoreB[PokerCards::JACK][10][5] << endl;

  cout << "Scores for Jack, 4: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreB[PokerCards::JACK][4][ind] << ", ";
  }
  cout << this->scoreB[PokerCards::JACK][4][5] << endl;

  cout << "Scores for 4, 2: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreB[4][2][ind] << ", ";
  }
  cout << this->scoreB[4][2][5] << endl;

  cout << "" << endl;

  cout << "Results for scenario C:" << endl;
  cout << "Scores for Jack, 10: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreC[PokerCards::JACK][10][ind] << ", ";
  }
  cout << this->scoreC[PokerCards::JACK][10][5] << endl;

  cout << "Scores for Jack, 4: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreC[PokerCards::JACK][4][ind] << ", ";
  }
  cout << this->scoreC[PokerCards::JACK][4][5] << endl;

  cout << "Scores for 4, 2: ";
  for (unsigned ind = 0 ; ind < 5 ; ind++)
  {
    cout << this->scoreC[4][2][ind] << ", ";
  }
  cout << this->scoreC[4][2][5] << endl;

  cout << "" << endl;
  cout << "" << endl;
}
