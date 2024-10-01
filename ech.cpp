#include <stdio.h>
#include <stack>
#include <queue>
#include <malloc.h>

#define Maxlength 100

using namespace std;
const char* action[] = {"First State", "Move Yellow Frog", "Move Brown Frog", "Jump Yellow Frog", "Jump Brown Frog"};

typedef struct {
    int frogs[7]; // 7 vị trí trên phiến đá
} State;

typedef struct Node {
    State state;
    struct Node* Parent;
    int no_function;
} Node;

void makeNullState(State *state) {
    state->frogs[0] = 1;
    state->frogs[1] = 1;
    state->frogs[2] = 1;
    state->frogs[3] = 0;
    state->frogs[4] = -1;
    state->frogs[5] = -1;
    state->frogs[6] = -1;
}

void print_State(State state) {
    printf("\nState: ");
    for(int i = 0; i < 7; i++) {
        printf("%d ", state.frogs[i]);
    }
}

int goalcheck(State state) {
    int goal[7] = {-1, -1, -1, 0, 1, 1, 1};
    for (int i = 0; i < 7; i++) {
        if (state.frogs[i] != goal[i]) {
            return 0;
        }
    }
    return 1;
}

int moveYellowFrog(State cur_state, State *result, int pos) {
    *result = cur_state;
    if (pos < 6 && cur_state.frogs[pos] == 1 && cur_state.frogs[pos + 1] == 0) {
        result->frogs[pos] = 0;
        result->frogs[pos + 1] = 1;
        return 1;
    }
    return 0;
}

int moveBrownFrog(State cur_state, State *result, int pos) {
    *result = cur_state;
    if (pos > 0 && cur_state.frogs[pos] == -1 && cur_state.frogs[pos - 1] == 0) {
        result->frogs[pos] = 0;
        result->frogs[pos - 1] = -1;
        return 1;
    }
    return 0;
}

int jumpYellowFrog(State cur_state, State *result, int pos) {
    *result = cur_state;
    if (pos < 5 && cur_state.frogs[pos] == 1 && cur_state.frogs[pos + 1] == -1 && cur_state.frogs[pos + 2] == 0) {
        result->frogs[pos] = 0;
        result->frogs[pos + 2] = 1;
        return 1;
    }
    return 0;
}

int jumpBrownFrog(State cur_state, State *result, int pos) {
    *result = cur_state;
    if (pos > 1 && cur_state.frogs[pos] == -1 && cur_state.frogs[pos - 1] == 1 && cur_state.frogs[pos - 2] == 0) {
        result->frogs[pos] = 0;
        result->frogs[pos - 2] = -1;
        return 1;
    }
    return 0;
}

int call_operator(State cur_state, State *result, int option) {
    int pos = option / 4;  // Vị trí của ếch trên phiến đá
    switch (option % 4) {
        case 0: return moveYellowFrog(cur_state, result, pos);
        case 1: return moveBrownFrog(cur_state, result, pos);
        case 2: return jumpYellowFrog(cur_state, result, pos);
        case 3: return jumpBrownFrog(cur_state, result, pos);
        default: return 0;
    }
}

int compareStates(State a, State b) {
    for(int i = 0; i < 7; i++) {
        if (a.frogs[i] != b.frogs[i])
            return 0;
    }
    return 1;
}

int find_State(State state, queue<Node*> openQueue) {
    queue<Node*> tempQueue = openQueue;
    while(!tempQueue.empty()) {
        if(compareStates(tempQueue.front()->state, state))
            return 1;
        tempQueue.pop();
    }
    return 0;
}

Node* BFS_Algorithm(State state) {
    queue<Node*> Open_BFS;
    queue<Node*> Close_BFS;
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    Open_BFS.push(root);
    while(!Open_BFS.empty()) {
        Node* node = Open_BFS.front();
        Open_BFS.pop();
        Close_BFS.push(node);
        if(goalcheck(node->state))
            return node;
        int opt;
        for(opt = 0; opt < 28; opt++) {  // 7 positions * 4 actions
            State newstate;
            makeNullState(&newstate);
            if(call_operator(node->state, &newstate, opt)) {
                if(find_State(newstate, Close_BFS) || find_State(newstate, Open_BFS))
                    continue;
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = opt;
                Open_BFS.push(newNode);
            }
        }
    }
    return NULL;
}

void reverseQueue(queue<Node*>& Queue) {
    stack<Node*> Stack;
    while (!Queue.empty()) {
        Stack.push(Queue.front());
        Queue.pop();
    }
    while (!Stack.empty()) {
        Queue.push(Stack.top());
        Stack.pop();
    }
}

void print_WaysToGetGoal(Node* node) {
    queue<Node*> queuePrint;
    while(node->Parent != NULL) {
        queuePrint.push(node);
        node = node->Parent;
    }
    queuePrint.push(node);
    reverseQueue(queuePrint);
    int no_action = 0;
    while(!queuePrint.empty()) {
        if (no_action == 0) {
            printf("\nAction %d: %s", no_action, action[0]);  // First State
        } else {
            printf("\nAction %d: %s", no_action, action[(queuePrint.front()->no_function % 4) + 1]);
        }
        print_State(queuePrint.front()->state);
        queuePrint.pop();
        no_action++;
    }
}

int main() {
    State cur_state;
    makeNullState(&cur_state);
    Node* p = BFS_Algorithm(cur_state);
    if(p != NULL) {
        print_WaysToGetGoal(p);
    } else {
        printf("No solution found.\n");
    }
    return 0;
}
