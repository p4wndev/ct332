#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 4 // Kích thước bàn cờ (4x4)

// Cấu trúc lưu trạng thái của bàn cờ
typedef struct {
    int queens[N]; // queens[i] lưu vị trí hàng của quân hậu ở cột i
    int numQueens; // Số lượng quân hậu đã được đặt
    int heuristic; // Heuristic đánh giá trạng thái này
} State;

// Cấu trúc Node dùng trong hàng đợi ưu tiên
typedef struct Node {
    State state;
    struct Node* parent;
} Node;

// Cấu trúc hàng đợi ưu tiên
typedef struct {
    Node* elements[1000];
    int size;
} PriorityQueue;

// Khởi tạo hàng đợi ưu tiên
void makeNullQueue(PriorityQueue* queue) {
    queue->size = 0;
}

// Thêm phần tử vào hàng đợi ưu tiên (sắp xếp theo heuristic)
void pushQueue(Node* node, PriorityQueue* queue) {
    int i = queue->size;
    queue->elements[queue->size++] = node;

    // Sắp xếp dựa vào giá trị heuristic (ưu tiên giá trị nhỏ hơn)
    while (i > 0 && queue->elements[i]->state.heuristic < queue->elements[(i - 1) / 2]->state.heuristic) {
        Node* temp = queue->elements[i];
        queue->elements[i] = queue->elements[(i - 1) / 2];
        queue->elements[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// Lấy phần tử ra khỏi hàng đợi ưu tiên
Node* popQueue(PriorityQueue* queue) {
    if (queue->size == 0) return NULL;

    Node* node = queue->elements[0];
    queue->elements[0] = queue->elements[--queue->size];

    int i = 0;
    while (i * 2 + 1 < queue->size) {
        int child = i * 2 + 1;
        if (child + 1 < queue->size && queue->elements[child + 1]->state.heuristic < queue->elements[child]->state.heuristic) {
            child++;
        }
        if (queue->elements[i]->state.heuristic <= queue->elements[child]->state.heuristic) break;

        Node* temp = queue->elements[i];
        queue->elements[i] = queue->elements[child];
        queue->elements[child] = temp;
        i = child;
    }
    return node;
}

// Hàm tính số xung đột giữa các quân hậu
int calculateConflicts(State state) {
    int conflicts = 0;
    for (int i = 0; i < state.numQueens; i++) {
        for (int j = i + 1; j < state.numQueens; j++) {
            if (state.queens[i] == state.queens[j] || abs(state.queens[i] - state.queens[j]) == abs(i - j)) {
                conflicts++;
            }
        }
    }
    return conflicts;
}

// Hàm kiểm tra xem có an toàn để đặt quân hậu tại cột col và hàng row hay không
bool isSafe(State state, int row, int col) {
    for (int i = 0; i < col; i++) {
        int queenRow = state.queens[i];
        if (queenRow == row || abs(queenRow - row) == abs(i - col)) {
            return false; // Cùng hàng hoặc cùng đường chéo
        }
    }
    return true;
}

// Hàm in ra trạng thái bàn cờ
void printSolution(State state) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state.queens[i] == j) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Thuật toán A* với heuristic để giải bài toán 4 quân hậu
void AStar_NQueens() {
    PriorityQueue queue;
    makeNullQueue(&queue);

    // Tạo trạng thái ban đầu (chưa có quân hậu nào)
    Node* root = (Node*)malloc(sizeof(Node));
    root->state.numQueens = 0;
    root->state.heuristic = 0;
    root->parent = NULL;
    pushQueue(root, &queue);

    while (queue.size > 0) {
        Node* node = popQueue(&queue);

        // Nếu đã đặt đủ 4 quân hậu, in ra giải pháp
        if (node->state.numQueens == N) {
            printSolution(node->state);
            continue;
        }

        int col = node->state.numQueens;
        for (int row = 0; row < N; row++) {
            if (isSafe(node->state, row, col)) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = node->state;
                newNode->state.queens[col] = row;
                newNode->state.numQueens++;
                newNode->state.heuristic = calculateConflicts(newNode->state); // Tính heuristic dựa trên số xung đột
                newNode->parent = node;
                pushQueue(newNode, &queue);
            }
        }
    }
}

int main() {
    printf("Cac giai phap cho bai toan 4 quan hau (voi heuristic):\n");
    AStar_NQueens();
    return 0;
}
