#include <stdio.h>
#include <string.h>

#define MAXN 1000

// Tiền xử lý để tính mảng mpNext (giống prefix function trong KMP)
void preMp(char *x, int m, int mpNext[]) {
    int i = 0, j = -1;
    mpNext[0] = -1;
    while (i < m) {
        while (j > -1 && x[i] != x[j]) // chừng nào mà mà trước không bằng -1 và ký tự hiện tại không bằng ký tự ở vị trí j thì lùi j về mpNext[j]
        j = mpNext[j];
        i++;
        j++;
        mpNext[i] = j;
    }
}

// Thuật toán Morris-Pratt
void MP(char *x, int m, char *y, int n) {
    int i = 0, j = 0;
    int mpNext[MAXN];

    // Bước 1: tiền xử lý pattern
    preMp(x, m, mpNext);

    // Bước 2: tìm kiếm
    while (j < n) {
        while (i > -1 && x[i] != y[j])
            i = mpNext[i];
        i++;
        j++;
        if (i >= m) {
            printf("Pattern found at index %d\n", j - i);
            i = mpNext[i];
        }
    }
}

int main() {
    char text[] = "ababcababcabc";
    char pattern[] = "ababc";

    int n = strlen(text);
    int m = strlen(pattern);

    MP(pattern, m, text, n);

    return 0;
}
