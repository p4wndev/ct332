#include <stdio.h>

void thapHaNoi(int n, char source, char destination, char auxiliary) {
    // Trường hợp cơ bản: Nếu chỉ có 1 đĩa, di chuyển trực tiếp từ cột nguồn đến cột đích
    if (n == 1) {
        printf("Di chuyen dia 1 tu %c den %c\n", source, destination);
        return;
    }

    // Di chuyển n-1 đĩa từ cột nguồn sang cột trung gian
    thapHaNoi(n - 1, source, auxiliary, destination);

    // Di chuyển đĩa lớn nhất (n) từ cột nguồn sang cột đích
    printf("Di chuyen dia %d tu %c den %c\n", n, source, destination);

    // Di chuyển n-1 đĩa từ cột trung gian sang cột đích
    thapHaNoi(n - 1, auxiliary, destination, source);
}

int main() {
    int n;
    printf("Nhap so dia: ");
    scanf("%d", &n);
    thapHaNoi(n, 'A', 'C', 'B');

    return 0;
}
