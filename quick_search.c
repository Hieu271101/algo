#include <stdio.h>
#include <string.h>

#define ASIZE 256 // Kích thước bảng ASCII

void preQsBc(char *x, int m, int qsBc[]) {
    int i;
    for (i = 0; i < ASIZE; ++i)
        qsBc[i] = m + 1; // Mặc định dịch m + 1
    for (i = 0; i < m; ++i)
        qsBc[(unsigned char)x[i]] = m - i; // Cập nhật theo ký tự trong pattern
    // In ra bảng qsBc
    printf("\n--- preQsBc (Quick Search Bad Character Rule) ---\n");
    for (i = 0; i < ASIZE; i++) {
        if (qsBc[i] != m + 1) { // Chỉ in những ký tự xuất hiện trong mẫu
            if (i >= 32 && i <= 126) // ký tự in được
                printf("qsBc[%c] = %d\n", i, qsBc[i]);
            else
                printf("qsBc[0x%02X] = %d\n", i, qsBc[i]);
        }
    }
}

void QS(char *x, int m, char *y, int n) {
    int j;
    int qsBc[ASIZE];

    // Tiền xử lý
    preQsBc(x, m, qsBc);

    // In bảng dịch qsBc cho các ký tự xuất hiện trong pattern
    printf("\nQuick Search Bad Character Table (qsBc):\n");
    for (int c = 0; c < ASIZE; ++c) {
        if (qsBc[c] != m + 1) {
            if (c >= 32 && c <= 126) // ký tự in được
                printf("'%c': %d; ", c, qsBc[c]);
            else
                printf("0x%02X: %d; ", c, qsBc[c]);
        }
    }
    printf("\n\n");

    printf("Searching...\n");
    j = 0;
    while (j <= n - m) {
        printf("Checking position j = %d (window: \"", j);
        for (int k = 0; k < m; k++) printf("%c", y[j+k]);
        printf("\")\n");

        if (memcmp(x, y + j, m) == 0)
            printf("  ✅ Pattern found at index %d\n", j);
        else
            printf("  ❌ No match\n");

        unsigned char nextChar = (j + m < n) ? y[j + m] : '\0';
        int shift = qsBc[nextChar];
        printf("  Next char after window: '%c' (ASCII %d), shift = %d\n",
               (nextChar ? nextChar : '#'), nextChar, shift);

        j += shift;
        printf("  -> Move to j = %d\n\n", j);
    }
}

int main() {
    char text[] = "HERE IS A SIMPLE EXAMPLE";
    char pattern[] = "EXAMPLE";

    int n = strlen(text);
    int m = strlen(pattern);

    printf("Text: %s\n", text);
    printf("Pattern: %s\n\n", pattern);

    QS(pattern, m, text, n);

    return 0;
}
