#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MaxLength 500

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

typedef struct{
    Node* element[MaxLength];
    int size;
}List;

void makeNullList(List *list){
    list->size = 0;
}

int emptyList(List list){
    return list.size == 0;
}

int fullList(List list){
    return list.size == MaxLength;
}

Node* elementAt(int position, List list){
    return list.element[position - 1];
}

void pushList(Node *node, int position, List *list){
    if(!fullList(*list)){
        int q;
        for(q = list->size; q >= position; q--){
            list->element[q] = list->element[q - 1];
        }
        list->element[position - 1] = node;
        list->size++;
    }
    else
        printf("List is full!");
}

void deleteList(int position, List *list){
    if(emptyList(*list)){
        printf("List is empty!\n");
    }
    else if((position < 1) || (position > list->size)){
        printf("Position is not possible to delete!\n");
    }
    else{
        int i;
        for(i = position - 1; i < list->size - 1; i++){
            list->element[i] = list->element[i + 1];
        }
        list->size--;
    }
}
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

Node* findState(State state, List list, int *position){
    int i;
    for(i = 1; i <= list.size; i++){
        if(compareState(elementAt(i, list)->state, state)){
            *position = i;
            return elementAt(i, list);
        }
    }
    return NULL;
}

void sortList(List *list){
    int i, j;
    for(i = 0; i < list->size - 1; i++){
        for(j = i + 1; j < list->size; j++){
            if(elementAt(i, *list)->heuristic > elementAt(j, *list)->heuristic){
                Node* temp = list->element[i];
                list->element[i] = list->element[j];
                list->element[j] = temp;
            }
        }
    }
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

void printPath(Node* node){
    List listPrint;
    makeNullList(&listPrint);
    while(node->parent != NULL){
        pushList(node, listPrint.size + 1, &listPrint);
        node = node->parent;
    }
    pushList(node, listPrint.size + 1, &listPrint);
    int no_action = 0,i;
    for(i = listPrint.size; i >= 1; i--){
        printf("\nAction %d: %s", no_action, action[elementAt(i, listPrint)->no_function]);
        printState(elementAt(i, listPrint)->state);
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
Node* bestFirstSearch(State state, State goal){
    List open;
    List close;
    makeNullList(&open);
    makeNullList(&close);
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->heuristic = heuristic2(root->state, goal);
    pushList(root, open.size + 1, &open);
    while(!emptyList(open)){
        Node* node = elementAt(1, open);
        deleteList(1, &open);
        pushList(node, close.size + 1, &close);
        if(goalcheck(node->state, goal)){
            return node;
        }
        int opt;
        for(opt = 1; opt <= MAX_OPERATOR; opt++){
            State newState;
            if(callOperator(node->state, &newState, opt)){
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->heuristic = heuristic2(newState, goal);
                //Kiem tra newState co trong open hay khong
                int posOpen, posClose;
                Node* nodeOpen = findState(newState, open, &posOpen);
                Node* nodeClose = findState(newState, close, &posClose);
                if(nodeOpen == NULL && nodeClose == NULL){
                    pushList(newNode, open.size + 1, &open);
                }
                else if(nodeOpen != NULL && nodeOpen->heuristic > newNode->heuristic){
                    deleteList(posOpen, &open);
                    pushList(newNode, posOpen, &open);
                }
                else if(nodeClose != NULL && nodeClose->heuristic > newNode->heuristic){
                    deleteList(posClose, &close);
                    pushList(newNode, open.size + 1, &open);
                }
                sortList(&open);
            }
        }
    }
    return NULL;
}


int main(){
    //Bai 1
    // State state, result;
    // state.emptyRow = 1;
    // state.emptyCol = 1;
    // state.eightPuzzle[0][0] = 3;
    // state.eightPuzzle[0][1] = 4;
    // state.eightPuzzle[0][2] = 5;
    // state.eightPuzzle[1][0] = 1;
    // state.eightPuzzle[1][1] = 0;
    // state.eightPuzzle[1][2] = 2;
    // state.eightPuzzle[2][0] = 6;
    // state.eightPuzzle[2][1] = 7;
    // state.eightPuzzle[2][2] = 8;
    // printf("Trang thai bat dau: \n");
    // printState(state);
    // int opt;
    // for(opt = 1; opt <= MAX_OPERATOR; opt++){
    //     callOperator(state, &result, opt);
    //     if(!compareState(state, result)){
    //         printf("Hanh dong %s thanh cong: \n", action[opt]);
    //         printState(result);
    //     }
    //     else
    //         printf("Hanh dong %s khong thanh cong\n", action[opt]);
    // }
    // return 0;


    //Bai 2
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
