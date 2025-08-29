#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASIZE 256  // số lượng ký tự ASCII

void preBmBc(char *x, int m, int bmBc[]) {
    for (int i = 0; i < ASIZE; ++i)
        bmBc[i] = m;
    for (int i = 0; i < m - 1; ++i)
        bmBc[(unsigned char)x[i]] = m - i - 1;
    // In ra bảng bmBc
    printf("\n--- preBmBc (Bad Character Rule) ---\n");
    for (int i = 0; i < ASIZE; i++) {
        if (bmBc[i] != m) // Chỉ in những ký tự xuất hiện trong mẫu
            printf("bmBc[%c] = %d\n", i, bmBc[i]);
    }
    
}

void TUNEDBM(char *x, int m, char *y, int n) {
    int j, k, shift, bmBc[ASIZE];

    // Preprocessing
    preBmBc(x, m, bmBc);
    shift = bmBc[(unsigned char)x[m - 1]];
    bmBc[(unsigned char)x[m - 1]] = 0;

    // Thêm guard để tránh kiểm tra biên khi đọc y[j+m-1]
    memset(y + n, x[m - 1], m);

    printf("Preprocessing completed.\nShift value: %d\n\n", shift);
    printf("Searching...\n");

    // Searching
    j = 0;
    while (j < n) {
        k = bmBc[(unsigned char)y[j + m - 1]];
        while (k != 0) {
            printf("Mismatch at j=%d, char='%c', shift by %d\n",
                   j, y[j + m - 1], k);
            j += k;
            k = bmBc[(unsigned char)y[j + m - 1]];
        }

        // Khi k == 0 -> ký tự cuối trùng -> kiểm tra pattern
        printf("Potential match at j=%d (last char matches)\n", j);
        if (memcmp(x, y + j, m - 1) == 0 && j < n) {
            printf("** MATCH found at position %d **\n", j);
        }
        j += shift;
        printf("Shift by precomputed shift=%d, next j=%d\n\n", shift, j);
    }
}

int main() {
    char text[] = "HERE IS A SIMPLE EXAMPLE";
    char pattern[] = "EXAMPLE";
    int n = strlen(text);
    int m = strlen(pattern);

    TUNEDBM(pattern, m, text, n);
    return 0;
}
