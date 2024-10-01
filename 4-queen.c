#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 8 // Kích thước bàn cờ (4x4)

// Cấu trúc lưu trạng thái của bàn cờ
typedef struct {
    int queens[N]; // queens[i] lưu vị trí hàng của quân hậu ở cột i
    int numQueens; // Số lượng quân hậu đã được đặt
} State;

// Cấu trúc Node dùng trong hàng đợi
typedef struct Node {
    State state;
    struct Node* parent;
} Node;

// Cấu trúc hàng đợi
typedef struct {
    Node* elements[1000000];
    int front, rear;
} Queue;

// Khởi tạo hàng đợi
void makeNullQueue(Queue* queue) {
    queue->front = queue->rear = -1;
}

// Kiểm tra hàng đợi rỗng
bool emptyQueue(Queue queue) {
    return queue.front == -1;
}

// Thêm phần tử vào hàng đợi
void pushQueue(Node* node, Queue* queue) {
    if (queue->rear == -1) {
        queue->front = 0;
    }
    queue->rear++;
    queue->elements[queue->rear] = node;
}

// Lấy phần tử ra khỏi hàng đợi
Node* popQueue(Queue* queue) {
    if (!emptyQueue(*queue)) {
        Node* node = queue->elements[queue->front];
        if (queue->front == queue->rear) {
            makeNullQueue(queue);
        } else {
            queue->front++;
        }
        return node;
    }
    return NULL;
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

// Thuật toán BFS để giải bài toán 4 quân hậu
void BFS_NQueens() {
    Queue queue;
    makeNullQueue(&queue);

    // Tạo trạng thái ban đầu (chưa có quân hậu nào)
    Node* root = (Node*)malloc(sizeof(Node));
    root->state.numQueens = 0;
    root->parent = NULL;
    pushQueue(root, &queue);

    while (!emptyQueue(queue)) {
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
                newNode->parent = node;
                pushQueue(newNode, &queue);
            }
        }
    }
}

int main() {
    printf("Cac giai phap cho bai toan 4 quan hau:\n");
    BFS_NQueens();
    return 0;
}
