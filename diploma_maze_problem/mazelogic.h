#ifndef MAZELOGIC_H
#define MAZELOGIC_H

#include <QString>
#include <vector>

enum class Direction { UP, RIGHT, DOWN, LEFT };

enum class Command {
    MOVE_FORWARD, TURN_LEFT, TURN_RIGHT,
    WAIT, SENSE_EXIT
};

struct Position {
    int row, col;
    bool operator==(const Position& o) const { return row==o.row && col==o.col; }
    bool operator!=(const Position& o) const { return !(*this==o); }
};

static const int N = 7;

// One individual moving wall segment
struct WallSegment {
    bool isHorizontal; // true աջ ու ձախ ա գնում,false վերև ներքև ա գնում
    int  line;
    int  pos;          // current col or row
    int  dir;          // +1 or -1 (current movement direction)
};

// միաժամանակ նույն բջջում հայտնված պատերը
struct OverlapInfo {
    bool isHorizontal;
    int  line;
    int  pos;
};

class MazeLogic {
public:
    bool hWall[N+1][N];
    bool vWall[N][N+1];

    std::vector<WallSegment> segments;
    std::vector<OverlapInfo> overlaps;

    int  movingHDir(int wallRow) const;
    int  movingVDir(int wallCol) const;
    int  segmentDir(bool isH, int line, int pos) const;
    bool isMovingHRow(int wallRow) const;
    bool isMovingVCol(int wallCol) const;
    bool isOverlap(bool isH, int line, int pos) const;

    MazeLogic();
    void reset();
    bool executeCommand(Command cmd, QString& message);
    void moveWalls();

    bool    canMove(const Position& from, Direction dir) const;
    QString wallTypeAt(const Position& from, Direction dir) const;

    bool    isExitReached()  const { return exitReached; }
    bool    isGameOver()     const { return gameOver; }
    QString getLastError()   const { return lastError; }
    int     getResetCount()  const { return resetCount; }
    void    incrementResetCount()  { resetCount++; }
    static const int MAX_RESETS = 3;

    int  victoryFrame = 0;
    static const int VICTORY_FRAMES = 18;
    void tickVictory() { if (victoryFrame < VICTORY_FRAMES) victoryFrame++; }

    Position  getRobotPos()  const { return robotPos;  }
    Position  getStartPos()  const { return startPos;  }
    Position  getExitPos()   const { return exitPos;   }
    Direction getRobotDir()  const { return robotDir;  }
    int       getStepCount() const { return stepCount; }
    int       getMazeSize()  const { return N; }

private:
    Position  robotPos, startPos, exitPos;
    Direction robotDir;
    int       stepCount;
    int       resetCount;
    bool      exitReached, gameOver;
    QString   lastError;

    bool hWallStatic[N+1][N];
    bool vWallStatic[N][N+1];

    void buildMaze();
    void rebuildWallArrays();
    void detectOverlaps();
    Position getNextPosition() const;
    void checkCrush();
};

#endif
