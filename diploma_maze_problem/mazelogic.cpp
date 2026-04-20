#include "mazelogic.h"
#include <cstring>

MazeLogic::MazeLogic() {
    startPos = {3, 0};
    exitPos  = {3, 6};
    resetCount = 0;
    reset();
}

void MazeLogic::reset() {
    robotPos    = startPos;
    robotDir    = Direction::RIGHT;
    stepCount   = 0;
    exitReached = false;
    gameOver    = false;
    lastError   = "";
    victoryFrame = 0;
    buildMaze();
}

void MazeLogic::buildMaze() {
    memset(hWallStatic, 0, sizeof(hWallStatic));
    memset(vWallStatic, 0, sizeof(vWallStatic));

    for (int c = 0; c < N; c++) { hWallStatic[0][c] = true; hWallStatic[N][c] = true; }
    for (int r = 0; r < N; r++) { vWallStatic[r][0] = true; vWallStatic[r][N] = true; }
    vWallStatic[3][0] = false;  // entrance
    vWallStatic[3][N] = false;  // exit

    // ── Static interior walls ─────────────────────────────────────────────────
    hWallStatic[3][1]=true; hWallStatic[3][2]=true;
    hWallStatic[3][6]=true;

    vWallStatic[0][1]=true; vWallStatic[1][1]=true; vWallStatic[2][1]=true;
    vWallStatic[4][1]=true; vWallStatic[5][1]=true; vWallStatic[6][1]=true;

    vWallStatic[0][6]=true; vWallStatic[1][6]=true; vWallStatic[2][6]=true;
    vWallStatic[4][6]=true; vWallStatic[5][6]=true; vWallStatic[6][6]=true;

    vWallStatic[0][3]=true;
    vWallStatic[6][4]=true;

    hWallStatic[5][0]=true; hWallStatic[5][1]=true; hWallStatic[5][3]=true;
    hWallStatic[5][4]=true; hWallStatic[5][5]=true; hWallStatic[5][6]=true;

    vWallStatic[1][3]=true; vWallStatic[3][3]=true;
    vWallStatic[5][3]=true; vWallStatic[6][3]=true;

    vWallStatic[0][4]=true;
    vWallStatic[4][4]=true; vWallStatic[5][4]=true;

    vWallStatic[0][5]=true; vWallStatic[2][5]=true; vWallStatic[3][5]=true;
    vWallStatic[4][5]=true; vWallStatic[5][5]=true; vWallStatic[6][5]=true;

    segments.clear();
    for (int c : {0,1,2,3,5})
        segments.push_back({true, 1, c, +1});

    // hWall row 2: segments at col 0,1,4,5 (gap at 2,3,6), all go LEFT (-1)
    for (int c : {0,1,4,5})
        segments.push_back({true, 2, c, -1});

    // hWall row 4: segments at col 0,2,3,5 (gap at 1,4,6), all go RIGHT (+1)
    // NOTE: col=6 excluded to keep path open to exit cell (3,6) from below
    for (int c : {0,2,3,5})
        segments.push_back({true, 4, c, +1});

    // hWall row 6: segments at col 0,1,2,3,4,6 (gap at 5), all go RIGHT (+1)
    for (int c : {0,1,2,3,4,6})
        segments.push_back({true, 6, c, +1});

    // vWall col 2: segments at row 0,1,2,4,5,6 (gap at 3), all go DOWN (+1)
    for (int r : {0,1,2,4,5,6})
        segments.push_back({false, 2, r, +1});

    // vWall col 5: segments at row 2,4,5,6 (gap at 0,1,3), all go UP (-1)
    for (int r : {2,4,5,6})
        segments.push_back({false, 5, r, -1});

    rebuildWallArrays();
    detectOverlaps();
}

//  rebuildWallArrays — recompute hWall/vWall from static + moving segments

void MazeLogic::rebuildWallArrays() {
    memcpy(hWall, hWallStatic, sizeof(hWall));
    memcpy(vWall, vWallStatic, sizeof(vWall));

    for (auto& s : segments) {
        if (s.isHorizontal)
            hWall[s.line][s.pos] = true;
        else
            vWall[s.pos][s.line] = true;
    }

    vWall[exitPos.row][exitPos.col] = false;
    vWall[exitPos.row][N]           = false;
}

void MazeLogic::detectOverlaps() {
    overlaps.clear();
    int hCount[N+1][N] = {};
    int vCount[N][N+1] = {};

    for (auto& s : segments) {
        if (s.isHorizontal)
            hCount[s.line][s.pos]++;
        else
            vCount[s.pos][s.line]++;
    }
    for (int r = 0; r <= N; r++)
        for (int c = 0; c < N; c++)
            if (hCount[r][c] >= 2)
                overlaps.push_back({true, r, c});
    for (int r = 0; r < N; r++)
        for (int c = 0; c <= N; c++)
            if (vCount[r][c] >= 2)
                overlaps.push_back({false, c, r});
}

bool MazeLogic::isMovingHRow(int wallRow) const {
    for (auto& s : segments)
        if (s.isHorizontal && s.line == wallRow) return true;
    return false;
}
bool MazeLogic::isMovingVCol(int wallCol) const {
    for (auto& s : segments)
        if (!s.isHorizontal && s.line == wallCol) return true;
    return false;
}
int MazeLogic::movingHDir(int wallRow) const {
    for (auto& s : segments)
        if (s.isHorizontal && s.line == wallRow) return s.dir;
    return 0;
}
int MazeLogic::movingVDir(int wallCol) const {
    for (auto& s : segments)
        if (!s.isHorizontal && s.line == wallCol) return s.dir;
    return 0;
}
int MazeLogic::segmentDir(bool isH, int line, int pos) const {
    for (auto& s : segments)
        if (s.isHorizontal == isH && s.line == line && s.pos == pos) return s.dir;
    return 0;
}
bool MazeLogic::isOverlap(bool isH, int line, int pos) const {
    for (auto& o : overlaps)
        if (o.isHorizontal == isH && o.line == line && o.pos == pos) return true;
    return false;
}

bool MazeLogic::canMove(const Position& from, Direction dir) const {
    int r = from.row, c = from.col;
    switch (dir) {
    case Direction::UP:    return !hWall[r][c];
    case Direction::DOWN:  return !hWall[r+1][c];
    case Direction::LEFT:  return !vWall[r][c];
    case Direction::RIGHT: return !vWall[r][c+1];
    }
    return false;
}

QString MazeLogic::wallTypeAt(const Position& from, Direction dir) const {
    int r = from.row, c = from.col;
    if (dir == Direction::UP    && r == 0)   return "border";
    if (dir == Direction::DOWN  && r == N-1) return "border";
    if (dir == Direction::LEFT  && c == 0)   return "border";
    if (dir == Direction::RIGHT && c == N-1) return "border";

    bool isH; int wallIdx; int cellIdx;
    switch (dir) {
    case Direction::UP:    isH=true;  wallIdx=r;   cellIdx=c; break;
    case Direction::DOWN:  isH=true;  wallIdx=r+1; cellIdx=c; break;
    case Direction::LEFT:  isH=false; wallIdx=c;   cellIdx=r; break;
    case Direction::RIGHT: isH=false; wallIdx=c+1; cellIdx=r; break;
    default: return "none";
    }

    for (auto& s : segments)
        if (s.isHorizontal == isH && s.line == wallIdx && s.pos == cellIdx)
            return "moving";

    return "static";
}

Position MazeLogic::getNextPosition() const {
    Position p = robotPos;
    switch (robotDir) {
    case Direction::UP:    p.row--; break;
    case Direction::DOWN:  p.row++; break;
    case Direction::LEFT:  p.col--; break;
    case Direction::RIGHT: p.col++; break;
    }
    return p;
}

bool MazeLogic::executeCommand(Command cmd, QString& message) {
    if (gameOver || exitReached) {
        message = " Խաղն ավարտված է";
        return false;
    }

    switch (cmd) {
    case Command::MOVE_FORWARD: {
        if (!canMove(robotPos, robotDir)) {
            QString dirStr = robotDir==Direction::UP?"↑ Վերև":
                                 robotDir==Direction::DOWN?"↓ Ներքև":
                                 robotDir==Direction::LEFT?"← Ձախ":"→ Աջ";
            QString posStr = QString("(%1,%2)").arg(robotPos.row+1).arg(robotPos.col+1);
            QString wtype  = wallTypeAt(robotPos, robotDir);

            if (wtype == "border")
                message = QString("Խաղն ավարտվեց։ Շրջմոլիկը փորձել է դուրս գալ լաբիրինթից! Ուղղ. %1, Դիրք %2").arg(dirStr, posStr);
            else if (wtype == "moving")
                message = QString("Խաղն ավարտվեց։ Բախում շարժվող պատի հետ! Ուղղ. %1, Դիրք %2").arg(dirStr, posStr);
            else
                message = QString("Խաղն ավարտվեց։ Բախում անշարժ պատի հետ! Ուղղ. %1, Դիրք %2").arg(dirStr, posStr);

            lastError = message;
            gameOver = true;
            return false;
        }
        robotPos = getNextPosition();
        stepCount++;
        message = QString(" Տեղաշարժ → (%1,%2)").arg(robotPos.row+1).arg(robotPos.col+1);
        if (robotPos == exitPos) { exitReached = true; message += " ԵԼՔ!"; }
        return true;
    }
    case Command::TURN_LEFT:
        robotDir = static_cast<Direction>((static_cast<int>(robotDir)+3)%4);
        message = "↺ Ձախ";
        return true;
    case Command::TURN_RIGHT:
        robotDir = static_cast<Direction>((static_cast<int>(robotDir)+1)%4);
        message = "↻ Աջ";
        return true;
    case Command::WAIT:
        stepCount++;
        message = " Սպասում...";
        return true;
    case Command::SENSE_EXIT: {
        Position next = getNextPosition();
        bool yes = (next == exitPos);
        message = yes ? " Ելքը առջևում Է!" : " Ելք չկա";
        return true;
    }
    }
    return false;
}

void MazeLogic::moveWalls() {
    const int maxPos = N - 1;

    for (auto& s : segments) {
        s.pos += s.dir;

        if (s.pos < 0) {
            // Overshot left: reflect.  e.g. pos=-1 → pos=1, then dir=+1
            s.pos = -s.pos;
            s.dir = +1;
        } else if (s.pos > maxPos) {
            // Overshot right: reflect.  e.g. pos=7 → pos=5 (for maxPos=6)
            s.pos = 2 * maxPos - s.pos;
            s.dir = -1;
        }
    }

    rebuildWallArrays();
    detectOverlaps();
    checkCrush();
}

void MazeLogic::checkCrush() {
    if (exitReached || gameOver) return;
    int r = robotPos.row, c = robotPos.col;

    bool movTopSet=false, movBottomSet=false;
    bool movLeftSet=false, movRightSet=false;

    for (auto& s : segments) {
        if (s.isHorizontal) {
            if (s.line == r   && s.pos == c) movTopSet    = true;
            if (s.line == r+1 && s.pos == c) movBottomSet = true;
        } else {
            if (s.line == c   && s.pos == r) movLeftSet   = true;
            if (s.line == c+1 && s.pos == r) movRightSet  = true;
        }
    }

    if ((movTopSet && movBottomSet) || (movLeftSet && movRightSet)) {
        gameOver = true;
        QString side = (movTopSet && movBottomSet) ? "վերևից և ներքևից" : "ձախից և աջից";
        lastError = QString(" Ջախջախվեց շարժվող պատերի կողմից (%1)! Դիրք (%2,%3)")
                        .arg(side).arg(r+1).arg(c+1);
    }
}
