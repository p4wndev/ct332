#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MaxLength 500

using namespace std;

const char* action[] = {"First state", "Move cell EMPTY to UP", "Move cell EMPTY to DOWN", "Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};

typedef struct{
    int eightPuzzle[ROWS][COLS];
    int emptyRow;
    int emptyCol;
}State;

typedef struct Node{
    State state;
    struct Node* parent;
    int no_function;
    int heuristic;
}Node;

typedef vector<Node*> List;

int compareState(State state1, State state2){
    if(state1.emptyRow != state2.emptyRow || state1.emptyCol != state2.emptyCol)
        return 0;
    int row, col;
    for(row = 0; row < ROWS; row++)
        for(col = 0; col < COLS; col++)
            if(state1.eightPuzzle[row][col] != state2.eightPuzzle[row][col])
                return 0;
    return 1;
}

Node* findState(State state, const List& list, int *position) {
    for (int i = 0; i < list.size(); i++) {
        if (compareState(list[i]->state, state)) {
            *position = i;
            return list[i];
        }
    }
    return NULL;
}

void sortList(List& list) {
    sort(list.begin(), list.end(), [](const Node* a, const Node* b) {
        return a->heuristic < b->heuristic;
    });
}

void printState(State state){
    int row, col;
    printf("----------\n");
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLS; col++)
            printf("|%d ", state.eightPuzzle[row][col]);
        printf("|\n");
    }
    printf("----------\n");
}

void printPath(Node* node) {
    vector<Node*> listPrint;
    while (node->parent != NULL) {
        listPrint.push_back(node);
        node = node->parent;
    }
    listPrint.push_back(node);

    int no_action = 0;
    for (auto it = listPrint.rbegin(); it != listPrint.rend(); ++it) {
        printf("\nAction %d: %s", no_action, action[(*it)->no_function]);
        printState((*it)->state);
        no_action++;
    }
}

int goalcheck(State state, State goal){
    return compareState(state, goal);
}

int upOperator(State current, State *result){
    *result = current;
    int emptyRowCurrent = current.emptyRow, emptyColCurrent = current.emptyCol;
    if(emptyRowCurrent > 0){
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent - 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent - 1][emptyColCurrent] = EMPTY;
        result->emptyRow = emptyRowCurrent - 1;
        result->emptyCol = emptyColCurrent;
        return 1;
    }
    return 0;
}

int downOperator(State current, State *result){
    *result = current;
    int emptyRowCurrent = current.emptyRow, emptyColCurrent = current.emptyCol;
    if(emptyRowCurrent < ROWS - 1){
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent + 1][emptyColCurrent];
        result->eightPuzzle[emptyRowCurrent + 1][emptyColCurrent] = EMPTY;
        result->emptyRow = emptyRowCurrent + 1;
        result->emptyCol = emptyColCurrent;
        return 1;
    }
    return 0;
}

int leftOperator(State current, State *result){
    *result = current;
    int emptyRowCurrent = current.emptyRow, emptyColCurrent = current.emptyCol;
    if(emptyColCurrent > 0){
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent][emptyColCurrent - 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent - 1] = EMPTY;
        result->emptyRow = emptyRowCurrent;
        result->emptyCol = emptyColCurrent - 1;
        return 1;
    }
    return 0;
}

int rightOperator(State current, State *result){
    *result = current;
    int emptyRowCurrent = current.emptyRow, emptyColCurrent = current.emptyCol;
    if(emptyColCurrent < COLS - 1){
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent] = result->eightPuzzle[emptyRowCurrent][emptyColCurrent + 1];
        result->eightPuzzle[emptyRowCurrent][emptyColCurrent + 1] = EMPTY;
        result->emptyRow = emptyRowCurrent;
        result->emptyCol = emptyColCurrent + 1;
        return 1;
    }
    return 0;
}

int callOperator(State current, State *result, int opt){
    switch(opt){
        case 1: return upOperator(current, result);
        case 2: return downOperator(current, result);
        case 3: return leftOperator(current, result);
        case 4: return rightOperator(current, result);
        default:
            printf("Cannot call operator");
                return 0;
    }
}

//Ham heuristic 1
//Dem so o sai khac so voi vi tri muc tieu
int heuristic1(State state, State goal){
    int row, col;
    int count = 0;
    for(row = 0; row < ROWS; row++)
        for(col = 0; col < COLS; col++)
            if(state.eightPuzzle[row][col] != goal.eightPuzzle[row][col])
                count++;
    return count;
}

//Ham heuristic 2 - Manhattan
// Dem so buoc chuyen o sai ve o dung cua trang thai muc tieu
int heuristic2(State state, State goal){
    int row, col;
    int count = 0;
    for(row = 0; row < ROWS; row++){
        for(col = 0; col < COLS; col++){
            if(state.eightPuzzle[row][col] != EMPTY){
                for(int row_goal = 0; row_goal < ROWS; row_goal++){
                    for(int col_goal = 0; col_goal < COLS; col_goal++){
                        if(state.eightPuzzle[row][col] == goal.eightPuzzle[row_goal][col_goal]){
                            count += abs(row - row_goal) + abs(col - col_goal);
                            col_goal = COLS;
                            row_goal = ROWS;
                        }
                    }
                }
            }
        }
    }
    return count;
}

//Thuat toan tim kiem tot nhat dau tien
//Ham f = h
Node* bestFirstSearch(State state, State goal) {
    List open, close;
    Node* root = new Node{state, NULL, 0, heuristic2(state, goal)};
    open.push_back(root);

    while (!open.empty()) {
        Node* node = open.front();
        open.erase(open.begin());
        close.push_back(node);

        if (goalcheck(node->state, goal)) {
            return node;
        }

        for (int opt = 1; opt <= MAX_OPERATOR; opt++) {
            State newState;
            if (callOperator(node->state, &newState, opt)) {
                Node* newNode = new Node{newState, node, opt, heuristic2(newState, goal)};

                int posOpen, posClose;
                Node* nodeOpen = findState(newState, open, &posOpen);
                Node* nodeClose = findState(newState, close, &posClose);

                if (nodeOpen == NULL && nodeClose == NULL) {
                    open.push_back(newNode);
                }
                else if (nodeOpen != NULL && nodeOpen->heuristic > newNode->heuristic) {
                    open[posOpen] = newNode;
                }
                else if (nodeClose != NULL && nodeClose->heuristic > newNode->heuristic) {
                    close.erase(close.begin() + posClose);
                    open.push_back(newNode);
                }
                sortList(open);
            }
        }
    }
    return NULL;
}

int main(){
    //Bai 3
    State state;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzle[0][0] = 3;
    state.eightPuzzle[0][1] = 4;
    state.eightPuzzle[0][2] = 5;
    state.eightPuzzle[1][0] = 1;
    state.eightPuzzle[1][1] = 0;
    state.eightPuzzle[1][2] = 2;
    state.eightPuzzle[2][0] = 6;
    state.eightPuzzle[2][1] = 7;
    state.eightPuzzle[2][2] = 8;

    State goal;
    goal.emptyRow = 0;
    goal.emptyCol = 0;
    goal.eightPuzzle[0][0] = 0;
    goal.eightPuzzle[0][1] = 1;
    goal.eightPuzzle[0][2] = 2;
    goal.eightPuzzle[1][0] = 3;
    goal.eightPuzzle[1][1] = 4;
    goal.eightPuzzle[1][2] = 5;
    goal.eightPuzzle[2][0] = 6;
    goal.eightPuzzle[2][1] = 7;
    goal.eightPuzzle[2][2] = 8;
    Node* node = bestFirstSearch(state, goal);
    printPath(node);
    return 0;



}
