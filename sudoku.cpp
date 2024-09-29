#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

#define NB_ROWS 9
#define NB_COLS 9
#define MAX_VALUE 10
#define EMPTY 0
#define AREA_SQUARE_SIZE 3
#define INF 9999999

using namespace std;

struct Coord {
    int x, y;
};

typedef vector<Coord> ListCoord;

struct Constrains {
    vector<vector<int>> data;
    int n;

    Constrains() : data(NB_ROWS*NB_COLS, vector<int>(NB_ROWS*NB_COLS, 0)), n(NB_ROWS*NB_COLS) {}
};

struct Sudoku {
    int cells[NB_ROWS][NB_COLS];
    Constrains constrains;
};

void initListCoord(ListCoord &list) {
    list.clear();
}

void appendListCoord(ListCoord &list, Coord c) {
    list.push_back(c);
}

int indexOf(Coord c) {
    return c.x*NB_ROWS + c.y;
}

Coord positionOfVertex(int v) {
    Coord c;
    c.x = v / NB_ROWS;
    c.y = v % NB_COLS;
    return c;
}

int addConstrains(Constrains &c, Coord source, Coord target) {
    int u = indexOf(source);
    int v = indexOf(target);
    if (c.data[u][v] == 0) {
        c.data[u][v] = 1;
        c.data[v][u] = 1;
        return 1;
    }
    return 0;
}

ListCoord getConstrains(Constrains constrains, Coord coord) {
    int v = indexOf(coord);
    ListCoord result;
    for (int i = 0; i < constrains.n; i++) {
        if (constrains.data[v][i] == 1) {
            result.push_back(positionOfVertex(i));
        }
    }
    return result;
}

void initSudoku(Sudoku &s) {
    for (int i = 0; i < NB_ROWS; i++) {
        for (int j = 0; j < NB_COLS; j++) {
            s.cells[i][j] = EMPTY;
        }
    }
}

void initSudokuWithValues(Sudoku &s, int values[NB_ROWS][NB_COLS]) {
    for (int i = 0; i < NB_ROWS; i++) {
        for (int j = 0; j < NB_COLS; j++) {
            s.cells[i][j] = values[i][j];
        }
    }
}

void printSudoku(Sudoku s) {
    cout << "Sudoku:\n";
    for (int i = 0; i < NB_ROWS; i++) {
        if (i % AREA_SQUARE_SIZE == 0) {
            cout << "-------------------------\n";
        }
        for (int j = 0; j < NB_COLS; j++) {
            if (j % AREA_SQUARE_SIZE == 0) {
                cout << "| ";
            }
            cout << s.cells[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "-------------------------\n";
}

bool isFilledSudoku(Sudoku s) {
    for (int i = 0; i < NB_ROWS; i++) {
        for (int j = 0; j < NB_COLS; j++) {
            if (s.cells[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

void spreadConstraintsFrom(Coord position, Constrains &c, ListCoord &changeds) {
    int row = position.x;
    int col = position.y;
    //Tạo ràng buộc cột
    for (int i = 0; i < NB_COLS; i++) {
        if (i != col) {
            Coord pos = {row, i};
            if (addConstrains(c, position, pos)) {
                changeds.push_back(pos);
            }
        }
    }
    //Tạo ràng buộc khối 3x3
    for (int i = 0; i < AREA_SQUARE_SIZE; i++) {
        for (int j = 0; j < AREA_SQUARE_SIZE; j++) {
            int areaX = (row / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            int areaY = (col / AREA_SQUARE_SIZE) * AREA_SQUARE_SIZE;
            if (areaX != row && areaY != col) {
                Coord pos = {areaX + i, areaY + j};
                if (addConstrains(c, position, pos)) {
                    changeds.push_back(pos);
                }
            }
        }
    }
}

vector<int> getAvailableValues(Coord position, Sudoku s) {
    ListCoord posList = getConstrains(s.constrains, position);
    vector<bool> availables(MAX_VALUE, true);

    for (const auto &pos : posList) {
        if (s.cells[pos.x][pos.y] != EMPTY) {
            availables[s.cells[pos.x][pos.y]] = false;
        }
    }

    vector<int> result;
    for (int i = 1; i < MAX_VALUE; i++) {
        if (availables[i]) {
            result.push_back(i);
        }
    }
    return result;
}

Coord getNextMinDomainCell(Sudoku s) {
    int minLength = INF;
    Coord result = {-1, -1};
    for (int i = 0; i < NB_ROWS; i++) {
        for (int j = 0; j < NB_COLS; j++) {
            if (s.cells[i][j] == EMPTY) {
                Coord pos = {i, j};
                int availableLength = getAvailableValues(pos, s).size();
                if (availableLength < minLength) {
                    minLength = availableLength;
                    result = pos;
                }
            }
        }
    }
    return result;
}

int exploredCounter = 0;

bool sudokuBacktracking(Sudoku &s) {
    if (isFilledSudoku(s)) {
        return true;
    }
    Coord nextPos = getNextMinDomainCell(s);
    vector<int> availableValues = getAvailableValues(nextPos, s);
    for (int value : availableValues) {
        s.cells[nextPos.x][nextPos.y] = value;
        exploredCounter++;
        if (sudokuBacktracking(s)) return true;
        s.cells[nextPos.x][nextPos.y] = EMPTY;
    }
    return false;
}

Sudoku solveSudoku(Sudoku s) {
    s.constrains = Constrains();
    for (int i = 0; i < NB_ROWS; i++) {
        for (int j = 0; j < NB_COLS; j++) {
            Coord pos = {i, j};
            ListCoord history;
            spreadConstraintsFrom(pos, s.constrains, history);
        }
    }
    exploredCounter = 0;
    if (sudokuBacktracking(s)) {
        cout << "Sudoku solved!\n";
    } else {
        cout << "Sudoku cannot be solved!\n";
    }
    cout << "Explored: " << exploredCounter << "\n";
    return s;
}

int main() {
    int inputs[NB_ROWS][NB_COLS] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    Sudoku s;
    initSudokuWithValues(s, inputs);
    printSudoku(s);
    Sudoku result = solveSudoku(s);
    printSudoku(result);
    return 0;
}
