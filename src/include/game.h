#ifndef __GAME_H__
#define __GAME_H__

#include <exception>
#include <string>

/*******************************************************************************
 * playGame() plays the game
 * filename indicates where default floor configuration is to be loaded from
 * exceptions: throws PCQuit, PCRestart, PCWon, GameError as appropriate
 * *****************************************************************************/
void playGame(const std::string filename);

/****************************** GAME EXCEPTIONS ********************************/
// base game exception
class GameException : public std::exception {};

// event exceptions
class InvalidFile : public GameException {};

class ReachedStairs : public GameException {};

class PCQuit : public GameException {};

class PCRestart : public GameException {};

class PCWon : public GameException {};

class PCSlain : public GameException {};

// invalid move/attack
class InvalidMove : public GameException {
  public:
    virtual const char * what () const noexcept = 0;  
};

class UnwalkableCell : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

class UnoccupiedCell : public InvalidMove {
  public : 
    const char * what () const noexcept;};

class PCNotInCell : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

class OccupiedCell : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

class CellContainsItem : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

class DragonAlive : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

class NonMerchant : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

class NotInterested : public InvalidMove {
  public : 
    const char * what () const noexcept;
};

// any other type of error (used for debugging purposes)
class GameError : public GameException {
  public : 
    const char * what () const noexcept;
};


#endif