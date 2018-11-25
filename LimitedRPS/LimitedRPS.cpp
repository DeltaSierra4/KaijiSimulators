/*
 * class LimitedRPS
 *
 * Main executable of the LimitedRPS program. Simulates a game of
 * 'Limited Rock-Paper-Scissors' that is featured in the Japanese Anime 'Kaiji'.
 * Rules of the game: Each player is given 3 lives, represented by number of stars,
 * 12 cards, 4 of each are rocks, papers, and scissors. Each player can only use
 * each card once. Two players are randomly placed in a game and play the game.
 * If it's a draw, they lose the card used and neither side lose their stars.
 * Otherwise, the winner takes a star from the loser.
 * Winning conditions: Player must have 3 or more stars and use up all their cards
 *                     before time limit (represented by optional turn limit) is up.
 * Losing conditions: Player has any number of cards left by the end of the game.
 *                    Player has run out of cards but has less than 3 stars.
 * If the player is a repeater, the number of stars required to win goes up to 4.
 * If the turn limit is set to 0, there is no turn limit and the game continues
 * until everyone has used up their cards.
 *
 * How to run it:
 *    ./LimitedRPS [number of contestants] [number of repeaters] [turn limit]
 * If no optional arguments are given, the number of contestants is set to 300,
 * there are no repeaters, and the turn limit is set to 0 (i.e. unlimited turns
 * until there are no contestants remaining in the general pool).
 *
 * V 1.0.0. Written by DeltaSierra4 (Vincent Yang) 11/24/2018
 */

#include "Contestant.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string.h>
#include <vector>

#define DEFAULT_CONTESTANT_COUNT 300
#define DEFAULT_TURN_LIMIT 0
#define DEFAULT_REPEATERS 0

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
 * Helper method that initializes all contestants, including repeaters,
 * if there are any.
 * @params  number of contestants for this game as well as any repeaters
 * @returns vector of all Contestant instances
 */
vector<Contestant*> initializer(vector<Contestant*> & vectorOfContestants, int contestantCount, int repeaters)
{
  for (int contestantNum = 0 ; contestantNum < (contestantCount - repeaters) ; contestantNum++)
  {
    //Add regular contestants using the default constructor.
    vectorOfContestants.push_back(new Contestant());
  }
  for (int repeaterNum = 0 ; repeaterNum < repeaters ; repeaterNum++)
  {
    //Add repeaters into this vector by using the custom constructor.
    vectorOfContestants.push_back(new Contestant(true));
  }
  return vectorOfContestants;
}

/*
 * Helper method to pick the player's card
 * @param  pointers to the Contestants
 * @returns card to play
 */
 int pick(Contestant *& player)
 {
   if (player->hasOnlyOneType() >= 0) {
     //Player has only one type of card. Return that type.
     return player->hasOnlyOneType();
   } else if (player->hasConsumedOneType() >= 0) {
     //Player has used up one pile. Pick from the remaining two
     if (player->hasConsumedOneType() == 2) {
       //Player has used up scissors. Pick from rock or paper
       return rand() % 2;
     } else if (player->hasConsumedOneType() == 0) {
       //Player has used up rock. Pick from paper or scissors
       return (1 + (rand() % 2));
     } else {
       //Player has used up paper. Pick from rock or scissors
       int randomPick = rand() % 2;
       if (randomPick == 1) {
         //Cannot use paper. add 1 to turn it to scissors
         randomPick++;
       }
       return randomPick;
     }
   } else {
     //Player has all three types. Pick from any.
     return rand() % 3;
   }
 }

/*
 * The actual rock-paper-scissor game.
 * @params  pointers to the two Contestants
 */
void game(Contestant *& first, Contestant *& second)
{
  // Each contestant gets to pick their card choice
  int firstPick = pick(first);
  int secondPick = pick(second);

  // Decrease card count of card that was picked
  first->decreaseCard(firstPick);
  second->decreaseCard(secondPick);
  if (firstPick == (secondPick + 1) % 3) {
    //Shift second player's pick by 1 up (e.g. scissors to rock, rock to paper, etc.)
    //First player wins if up-shift matches first player's pick.
    first->setStars(first->getStars() + 1);
    second->setStars(second->getStars() - 1);
  } else if (secondPick == (firstPick + 1) % 3) {
    //Shift first player's pick by 1 up
    //Second player wins if up-shift matches first player's pick.
    first->setStars(first->getStars() - 1);
    second->setStars(second->getStars() + 1);
  }
}

/*
 * Helper method that swaps a given element with the last element in the
 * vector, then pop/delete a given element.
 * @params  Vector representing general pool and index of player in the pool.
 */
void swapper(vector<Contestant*> & pool, int index)
{
  Contestant* swap;
  Contestant* player = pool[index];
  swap = pool[pool.size() - 1];
  pool[pool.size() - 1] = player;
  pool.pop_back();
  pool[index] = swap;
  delete player;
}

/*
 * After each game, removes any winners and losers from the pool, then
 * delete the objects.
 * @params  vector representing general pool, count of winners and losers
 * @params  Count of players and indices of players in the pool.
 * @params  Number of contestants in this game and player with most stars
 */
void processLoserWinner(vector<Contestant*> & pool, int & contestantCount, int & loserCount, int & winnerCount, int & mostStar)
{
  int index = 0;
  while (index < contestantCount)
  {
    Contestant * player = pool[index];
    if (!player->isRepeater()) {
      // Process regular players first
      // Winning condition: Have 3 or more stars and use up all cards
      if (player->getStars() == 0 || ((player->getStars() < 3) && (player->noCardsLeft()))) {
        swapper(pool, index);
        loserCount++;
        contestantCount--;
      } else if ((player->getStars() >= 3) && (player->noCardsLeft())) {
        if (player->getStars() > mostStar)
        {
          mostStar = player->getStars();
        }
        swapper(pool, index);
        winnerCount++;
        contestantCount--;
      } else {
        index++;
      }
    } else {
      // Process repeater players next
      // Winning condition: Have 4 or more stars and use up all cards
      if (player->getStars() == 0 || ((player->getStars() < 4) && (player->noCardsLeft()))) {
        swapper(pool, index);
        loserCount++;
        contestantCount--;
      } else if ((player->getStars() >= 4) && (player->noCardsLeft())) {
        if (player->getStars() > mostStar)
        {
          mostStar = player->getStars();
        }
        swapper(pool, index);
        winnerCount++;
        contestantCount--;
      } else {
        index++;
      }
    }
  }
}

/*
 * Wrapper method which simulates the game of rock-paper-scissors among contestants
 * after shuffling through the general pool. If anyone runs out of stars,
 * that contestant is removed from the general pool and placed in the prison.
 * If anyone runs out of cards but has 3 or more stars, they are placed in
 * the lounge.
 * @params  vector representing general pool, count of winners and losers.
 * @params  Number of contestants in this game and player with most stars
 */
void rpsSim(vector<Contestant*> & pool, int & loserCount, int & winnerCount, int & contestantCount, int & mostStar)
{
  //Shuffle the general pool
  random_shuffle(pool.begin(), pool.end());

  for (int contPair = 0 ; contPair < (int)(pool.size()) ; contPair += 2)
  {
    if (contPair == (int)(pool.size() - 1))
    {
      break;
    }
    Contestant* first = pool[contPair];
    Contestant* second = pool[contPair+1];
    game(first, second);
  }
  //Check for any winners or losers.
  processLoserWinner(pool, contestantCount, loserCount, winnerCount, mostStar);
}

/*
 * Print out end results, including number of contestants in the
 * winning pool and the losing pool. Also gets the most number of
 * stars held by any contestant.
 * @params  count of losers, winners, turns, and most stars held by any player.
 */
void printResult(int & loserCount, int & winnerCount, int & turn, int mostStar)
{
  if (turn < 0)
  {
    cout << "Endless mode game ended after " << (turn * -1) << " turns." << endl;
  }
  cout << "Number of people in prison: " << loserCount << endl;
  cout << "Number of people in lounge: " << winnerCount << endl;
  cout << "Most number of stars held by contestant: " << mostStar << endl;
}


/*
 * Main method. See class comments for instructions on
 * how to use optional command line arguments.
 * @params  optional number of contestants, any repeaters, and turn limit
 */
int main(int argc, char * argv[])
{
  srand(time(NULL)); //Used for random number generation for the games.

  //Default number of contestant is set to 5000
  //Default turn limit is 0 (i.e. no turn limit)
  //Default repeater count is 0
  int contestantCount = DEFAULT_CONTESTANT_COUNT;
  int turnLimit = DEFAULT_TURN_LIMIT;
  int repeaters = DEFAULT_REPEATERS;

  if (argc > 1)
  {
    for (int argi = 1 ; argi < argc ; argi++)
    {
      if (!isValidInput(argv[argi]))
      {
        cout << "Invalid input detected. Type in valid integers to pass in as parameters." << endl;
        cout << "Command line optional argument format: " << endl;
        cout << "./LimitedRPS <number of contestants> <number of repeaters> <turn limit>" << endl;
        return 0;
      }
    }
    contestantCount = atoi(argv[1]);
    if (argc > 2)
    {
      repeaters = atoi(argv[2]);
      if (repeaters > contestantCount)
      {
        cout << "You cannot have more repeaters than contestants." << endl;
        return 0;
      }
      if (argc > 2)
      {
        turnLimit = atoi(argv[3]);
      }
    }
  }

  if (turnLimit == 0)
  {
    //The while loop below ends when turnLimit hits 0.
    //If the client chooses to run the endless mode, the turnLimit variable
    //is forcibly dropped to -1 to allow the loop to run endless
    //until there are no contestants left.
    turnLimit--;
  }

  vector<Contestant*> generalPool;
  int loserCount = 0;
  int winnerCount = 0;
  int highestStars = 3; // At the start, everyone has 3 stars and no more no less.

  generalPool = initializer(generalPool, contestantCount, repeaters);

  while (turnLimit != 0 && (generalPool.size() > 1))
  {
    rpsSim(generalPool, loserCount, winnerCount, contestantCount, highestStars);
    turnLimit--;
  }

  if (!(generalPool.empty()))
  {
    // If the general pool is still not empty, that means there
    // are contestants who still have cards left. Any contestant
    // with remaining cards after the turn limit is up go into
    // the losing pool.
    loserCount += generalPool.size();
    for (int index = 0 ; index < (int)(generalPool.size()) ; index++)
    {
      delete generalPool[index];
    }
  }

  //Print results of the game after exiting the loop.
  printResult(loserCount, winnerCount, turnLimit, highestStars);

  return 0;
}
