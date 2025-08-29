#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASIZE 256 // Kích thước bảng ASCII

// Tiền xử lý bảng Bad Character cho Horspool
void preBmBc(char *x, int m, int bmBc[]) {
    printf("\n--- preBmBc (Horspool) ---\n");
    for (int i = 0; i < ASIZE; ++i)
        bmBc[i] = m;

    for (int i = 0; i < m - 1; ++i) {
        bmBc[(unsigned char)x[i]] = m - i - 1;
        printf("Char '%c' at pattern[%d] -> bmBc[%c] = %d\n", x[i], i, x[i], m - i - 1);
    }

    printf("\nBad Character Table (Horspool):\n");
    for (int i = 0; i < ASIZE; i++) {
        if (bmBc[i] != m) // Chỉ in những ký tự xuất hiện trong mẫu
            printf("bmBc[%c] = %d\n", i, bmBc[i]);
    }
}

// Thuật toán Horspool
void HORSPOOL(char *x, int m, char *y, int n) {
    int bmBc[ASIZE];
    preBmBc(x, m, bmBc);

    printf("\n--- Matching Process (Horspool) ---\n");

    int j = 0;
    while (j <= n - m) {
        printf("\nChecking window starting at j = %d\n", j);
        char c = y[j + m - 1];

        // So sánh từ phải sang trái, nhưng chỉ kiểm tra nếu ký tự cuối khớp
        if (x[m - 1] == c) {
            printf("  Last character matches (%c), checking prefix...\n", c);
            if (memcmp(x, y + j, m - 1) == 0) {
                printf(">>> Found match at position %d\n", j);
            } else {
                printf("  Prefix mismatch, not a full match.\n");
            }
        } else {
            printf("  Last character mismatch: pattern[%d]=%c vs text[%d]=%c\n",
                   m - 1, x[m - 1], j + m - 1, c);
        }

        printf("Shift by bmBc[%c] = %d\n", c, bmBc[(unsigned char)c]);
        j += bmBc[(unsigned char)c];
    }

    printf("\n===== End of Horspool Search =====\n");
}

int main() {
    char text[] = "abcababcababc";
    char pattern[] = "abcab";

    HORSPOOL(pattern, strlen(pattern), text, strlen(text));
    return 0;
}
