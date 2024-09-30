#include <stdio.h>
#include <queue>
#include <stack>
#include <vector>

#define MAXLENGTH 100

using namespace std;

// Trạng thái của các ly, 1 là ngửa (N), 0 là úp (U)
typedef struct {
    int cups[6];
} State;

// Node structure for the search tree
typedef struct Node {
    State state;
    struct Node* parent;
    int action;
} Node;

// Các hành động có thể
const char* actions[] = {"Initial state", "Flip cups from 1 to 3", "Flip cups from 2 to 4", "Flip cups from 3 to 5", "Flip cups from 4 to 6"};

// Hàm in trạng thái
void printState(State state) {
    for (int i = 0; i < 6; i++) {
        printf("%c ", state.cups[i] ? 'N' : 'U');
    }
    printf("\n");
}

// Hàm tạo trạng thái ban đầu (n,u,n,u,n,u)
void makeInitialState(State *state) {
    state->cups[0] = 1; // N: ngửa
    state->cups[1] = 0; // U: úp
    state->cups[2] = 1; // N: ngửa
    state->cups[3] = 0; // U: úp
    state->cups[4] = 1; // N: ngửa
    state->cups[5] = 0; // U: úp
}

// Hàm kiểm tra xem có đạt trạng thái mục tiêu không
int isGoalState(State state) {
    for (int i = 0; i < 6; i++) {
        if (state.cups[i] == 0) return 0; // Chưa đạt mục tiêu (vẫn còn ly úp)
    }
    return 1; // Đã đạt mục tiêu (tất cả đều ngửa)
}

// Hàm lật trạng thái của 3 ly từ vị trí start
void flipCups(State cur_state, State *result, int start) {
    *result = cur_state;
    for (int i = start; i < start + 3; i++) {
        result->cups[i] = !cur_state.cups[i]; // Lật trạng thái ly
    }
}

// Kiểm tra hai trạng thái có giống nhau không
int compareStates(State a, State b) {
    for (int i = 0; i < 6; i++) {
        if (a.cups[i] != b.cups[i]) return 0;
    }
    return 1;
}

// Tìm trạng thái trong queue
bool findState(State state, queue<Node*> q) {
    queue<Node*> tempQ = q;
    while (!tempQ.empty()) {
        if (compareStates(tempQ.front()->state, state)) return true;
        tempQ.pop();
    }
    return false;
}

// Hàm BFS tìm kiếm chuỗi hành động để đạt mục tiêu
Node* BFS(State initialState) {
    queue<Node*> Open;
    queue<Node*> Close;

    Node* root = new Node{initialState, NULL, 0};
    Open.push(root);

    while (!Open.empty()) {
        Node* node = Open.front();
        Open.pop();
        Close.push(node);

        if (isGoalState(node->state))
            return node;

        for (int opt = 1; opt <= 4; opt++) {
            State newState;
            flipCups(node->state, &newState, opt - 1);

            if (!findState(newState, Open) && !findState(newState, Close)) {
                Node* newNode = new Node{newState, node, opt};
                Open.push(newNode);
            }
        }
    }

    return NULL;
}

void printWayToGetGoal(Node* goalNode) {
    vector<Node*> path;
    Node* current = goalNode;

    while (current != NULL) {
        path.push_back(current);
        current = current->parent;
    }
    for (int i = path.size() - 1; i >= 0; i--) {
        printf("Action %ld: %s\n", path.size() - 1 - i, actions[path[i]->action]);
        printState(path[i]->state);
        printf("\n");
    }
}

int main() {
    State initialState;
    makeInitialState(&initialState);

    Node* goalNode = BFS(initialState);

    if (goalNode != NULL) {
        printf("Goal state reached!\n");
        printWayToGetGoal(goalNode);
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
