#include <stdio.h>
#include <string.h>

#define MAXN 1000

// Hàm tiền xử lý: xây dựng bảng kmpNext
void preKmp(char *x, int m, int kmpNext[]) {
    int i = 0, j = -1;
    kmpNext[0] = -1;

    while (i < m) {
        while (j > -1 && x[i] != x[j])
            j = kmpNext[j];
        i++;
        j++;
        if (i < m && x[i] == x[j])
            kmpNext[i] = kmpNext[j];
        else
            kmpNext[i] = j;
    }
}

// Hàm tìm kiếm KMP
void KMP(char *x, int m, char *y, int n) {
    int i = 0, j = 0;
    int kmpNext[MAXN];

    // Bước 1: tiền xử lý pattern
    preKmp(x, m, kmpNext);

    // Bước 2: tìm kiếm trong text
    while (j < n) {
        while (i > -1 && x[i] != y[j])
            i = kmpNext[i];
        i++;
        j++;
        if (i >= m) {
            printf("Pattern found at index %d\n", j - i);
            i = kmpNext[i];
        }
    }
}

int main() {
    char text[] = "ababcababcabc";
    char pattern[] = "ababc";

    int n = strlen(text);
    int m = strlen(pattern);

    KMP(pattern, m, text, n);

    return 0;
}
