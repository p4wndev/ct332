#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LENGTH 100

typedef struct {
    int soThayTu;
    int soConQuy;
    int boat; // 1 Trai - 0 Phai
} State;

typedef struct Node {
    State state;
    struct Node* parent;
    int action;
    int g;  // Chi phí thực tế để đến node này
    int h;  // Giá trị heuristic
    int f;  // Tổng chi phí f = g + h
} Node;

typedef struct {
    Node* elements[MAX_LENGTH];
    int front, rear;
} Queue;

const char* action[] = {
    "\nTrang thai bat dau",
    "\nDi chuyen 1 thay tu",
    "\nDi chuyen 2 thay tu",
    "\nDi chuyen 1 con quy",
    "\nDi chuyen 2 con quy",
    "\nDi chuyen 1 thay tu va 1 con quy"
};

// Hàm heuristic
int heuristic(State state) {
    int nguoiConLai = state.soThayTu + state.soConQuy;
    int soChuyenDi = ceil((float)nguoiConLai / 2);  // Mỗi chuyến có thể đưa tối đa 2 người
    if (state.boat == 1) soChuyenDi++;  // Nếu thuyền ở bờ trái, cần thêm một chuyến để quay lại
    return soChuyenDi;
}

bool goalCheck(State state) {
    return (state.soThayTu == 0 && state.soConQuy == 0 && state.boat == 0);
}

bool isValid(State state) {
    if (state.soThayTu < 0 || state.soThayTu > 3) return false;
    if (state.soConQuy < 0 || state.soConQuy > 3) return false;
    int soThayTuPhai = 3 - state.soThayTu;
    int soConQuyPhai = 3 - state.soConQuy;
    if ((state.soThayTu > 0 && state.soThayTu < state.soConQuy) ||
        (soThayTuPhai > 0 && soThayTuPhai < soConQuyPhai))
        return false;
    return true;
}

bool call_operator(State cur_state, State *result, int option) {
    *result = cur_state;
    switch(option) {
        case 1: // 1 thay tu
            if (cur_state.boat == 1) {
                result->soThayTu--;
                result->boat = 0;
            } else {
                result->soThayTu++;
                result->boat = 1;
            }
            break;
        case 2: // 2 thay tu
            if (cur_state.boat == 1) {
                result->soThayTu -= 2;
                result->boat = 0;
            } else {
                result->soThayTu += 2;
                result->boat = 1;
            }
            break;
        case 3: // 1 con quy
            if (cur_state.boat == 1) {
                result->soConQuy--;
                result->boat = 0;
            } else {
                result->soConQuy++;
                result->boat = 1;
            }
            break;
        case 4: // 2 con quy
            if (cur_state.boat == 1) {
                result->soConQuy -= 2;
                result->boat = 0;
            } else {
                result->soConQuy += 2;
                result->boat = 1;
            }
            break;
        case 5: // 1 thay tu 1 con quy
            if (cur_state.boat == 1) {
                result->soThayTu--;
                result->soConQuy--;
                result->boat = 0;
            } else {
                result->soThayTu++;
                result->soConQuy++;
                result->boat = 1;
            }
            break;
        default:
            return false;
    }
    return isValid(*result);
}

void printState(State state) {
    printf("(Thay tu:%d, Con quy:%d, Vi tri thuyen:%s)\n", state.soThayTu, state.soConQuy, state.boat ? "Trai" : "Phai");
}

void printSolution(Node* node) {
    if (node->parent != NULL) {
        printSolution(node->parent);
        printf("%s\n", action[node->action]);
        printState(node->state);
        printf("g = %d, h = %d, f = %d\n", node->g, node->h, node->f);
    }
}

bool compareStates(State s1, State s2) {
    return (s1.soThayTu == s2.soThayTu &&
            s1.soConQuy == s2.soConQuy &&
            s1.boat == s2.boat);
}

bool findState(State state, Node** explored, int numExplored) {
    for (int i = 0; i < numExplored; i++) {
        if (compareStates(state, explored[i]->state)) {
            return true;
        }
    }
    return false;
}

void makeNullQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

bool emptyQueue(Queue queue) {
    return queue.front == -1;
}

void pushQueue(Node* x, Queue *queue) {
    if (queue->rear == MAX_LENGTH - 1) {
        printf("Queue is full!\n");
        return;
    }
    if (queue->front == -1)
        queue->front = 0;
    queue->rear = queue->rear + 1;
    queue->elements[queue->rear] = x;
}

Node* popQueue(Queue *queue) {
    if (emptyQueue(*queue)) {
        printf("Queue is empty!\n");
        return NULL;
    }

    // Tìm node có f nhỏ nhất trong hàng đợi
    int min_f_idx = queue->front;
    for (int i = queue->front + 1; i <= queue->rear; i++) {
        if (queue->elements[i]->f < queue->elements[min_f_idx]->f) {
            min_f_idx = i;
        }
    }

    Node* x = queue->elements[min_f_idx];

    // Di chuyển các phần tử sau phần tử đã chọn
    for (int i = min_f_idx; i < queue->rear; i++) {
        queue->elements[i] = queue->elements[i + 1];
    }

    queue->rear--;
    if (queue->rear == -1) queue->front = -1; // Hàng đợi rỗng

    return x;
}

Node* A_Star_Algorithm(State start) {
    Queue open_queue;
    makeNullQueue(&open_queue);

    Node* root = (Node*)malloc(sizeof(Node));
    root->state = start;
    root->parent = NULL;
    root->action = 0;
    root->g = 0;
    root->h = heuristic(start);
    root->f = root->g + root->h;
    pushQueue(root, &open_queue);

    Node* explored[MAX_LENGTH];
    int numExplored = 0;

    while (!emptyQueue(open_queue)) {
        Node* node = popQueue(&open_queue);
        explored[numExplored++] = node;

        if (goalCheck(node->state)) {
            return node;
        }

        for (int option = 1; option <= 5; option++) {
            State newState;
            if (call_operator(node->state, &newState, option)) {
                if (!findState(newState, explored, numExplored)) {
                    Node* newNode = (Node*)malloc(sizeof(Node));
                    newNode->state = newState;
                    newNode->parent = node;
                    newNode->action = option;
                    newNode->g = node->g + 1;
                    newNode->h = heuristic(newState);
                    newNode->f = newNode->g + newNode->h;
                    pushQueue(newNode, &open_queue);
                }
            }
        }
    }

    return NULL;
}

int main() {
    State start = {3, 3, 1};  // 3 thay tu, 3 con quy, thuyen o bo trai
    Node* result = A_Star_Algorithm(start);

    if (result == NULL) {
        printf("Khong tim thay duong di.\n");
    } else {
        printf("Tim thay duong di:\n");
        printSolution(result);
    }

    return 0;
}
