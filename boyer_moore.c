// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #define ASIZE 256 // Kích thước bảng ASCII

// // Tiền xử lý Bad Character
// void preBmBc(char *x, int m, int bmBc[]) {
//     for (int i = 0; i < ASIZE; ++i)
//         bmBc[i] = m;
//     for (int i = 0; i < m - 1; ++i)
//         bmBc[(unsigned char)x[i]] = m - i - 1;
// }

// // Tiền xử lý Good Suffix
// void suffixes(char *x, int m, int *suff) {
//     int f = 0, g = m - 1;
//     suff[m - 1] = m;
//     for (int i = m - 2; i >= 0; --i) {
//         if (i > g && suff[i + m - 1 - f] < i - g)
//             suff[i] = suff[i + m - 1 - f];
//         else {
//             if (i < g) g = i;
//             f = i;
//             while (g >= 0 && x[g] == x[g + m - 1 - f]) --g;
//             suff[i] = f - g;
//         }
//     }
// }

// void preBmGs(char *x, int m, int bmGs[]) {
//     int i, j, *suff = (int *)malloc(m * sizeof(int));
//     suffixes(x, m, suff);
//     for (i = 0; i < m; ++i) bmGs[i] = m;
//     j = 0;
//     for (i = m - 1; i >= 0; --i)
//         if (suff[i] == i + 1)
//             for (; j < m - 1 - i; ++j)
//                 if (bmGs[j] == m) bmGs[j] = m - 1 - i;
//     for (i = 0; i <= m - 2; ++i)
//         bmGs[m - 1 - suff[i]] = m - 1 - i;
//     free(suff);
// }

// // Thuật toán Boyer–Moore
// void BM(char *x, int m, char *y, int n) {
//     int bmBc[ASIZE], *bmGs = (int *)malloc(m * sizeof(int));
//     preBmBc(x, m, bmBc);
//     preBmGs(x, m, bmGs);

//     int j = 0;
//     while (j <= n - m) {
//         int i = m - 1;
//         while (i >= 0 && x[i] == y[i + j]) i--;
//         if (i < 0) {
//             printf("Found at position %d\n", j);
//             j += bmGs[0];
//         } else {
//             j += (bmBc[(unsigned char)y[i + j]] > bmGs[i]) ? bmBc[(unsigned char)y[i + j]] : bmGs[i];
//         }
//     }
//     free(bmGs);
// }

// int main() {
//     char text[] = "HERE IS A SIMPLE EXAMPLE";
//     char pattern[] = "EXAMPLE";
//     BM(pattern, strlen(pattern), text, strlen(text));
//     return 0;
// }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ASIZE 256 // Kích thước bảng ASCII

// Tiền xử lý Bad Character
void preBmBc(char *x, int m, int bmBc[]) {
    printf("\n--- preBmBc (Bad Character Rule) ---\n");
    for (int i = 0; i < ASIZE; ++i)
        bmBc[i] = m;
// sau khi khởi tạo thì bmBc nó sẽ có giá trị m hết

    for (int i = 0; i < m - 1; ++i) {
        bmBc[(unsigned char)x[i]] = m - i - 1;
        printf("Char '%c' at pattern[%d] -> bmBc[%c] = %d\n", x[i], i, x[i], m - i - 1);
    }
    // in ra bảng bmBc
    printf("\nBad Character Table:\n");
    for (int i = 0; i < ASIZE; i++) {
        if (bmBc[i] != m) // Chỉ in những ký tự xuất hiện trong mẫu
            printf("bmBc[%c] = %d\n", i, bmBc[i]);
    }
}

// Tính mảng suff để phục vụ Good Suffix
void suffixes(char *x, int m, int *suff) {
    int f = 0, g = m - 1;
    suff[m - 1] = m;
    for (int i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g) g = i;
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f]) --g;
            suff[i] = f - g;
        }
    }
    printf("\n--- suffixes ---\n");
    for (int i = 0; i < m; i++)
        printf("suff[%d] = %d\n", i, suff[i]);
}

void preBmGs(char *x, int m, int bmGs[]) {
    int i, j, *suff = (int *)malloc(m * sizeof(int));
    suffixes(x, m, suff);

    printf("\n--- preBmGs (Good Suffix Rule) ---\n");
    for (i = 0; i < m; ++i) bmGs[i] = m;

    j = 0;
    for (i = m - 1; i >= 0; --i) {
        if (suff[i] == i + 1) {
            for (; j < m - 1 - i; ++j) {
                if (bmGs[j] == m) bmGs[j] = m - 1 - i;
            }
        }
    }

    for (i = 0; i <= m - 2; ++i)
        bmGs[m - 1 - suff[i]] = m - 1 - i;

    for (i = 0; i < m; i++)
        printf("bmGs[%d] = %d\n", i, bmGs[i]);

    free(suff);
}

// Thuật toán Boyer–Moore
void BM(char *x, int m, char *y, int n) {
    int bmBc[ASIZE], *bmGs = (int *)malloc(m * sizeof(int));
    preBmBc(x, m, bmBc);
    preBmGs(x, m, bmGs);

    printf("\n--- Matching Process ---\n");
    int j = 0;
    while (j <= n - m) {
        printf("\nChecking position j = %d\n", j);
        int i = m - 1;
        while (i >= 0 && x[i] == y[i + j]) {
            printf("  Match at x[%d] (%c) and y[%d] (%c)\n", i, x[i], i + j, y[i + j]);
            i--;
        }
        if (i < 0) {
            printf(">>> Found at position %d\n", j);
            j += bmGs[0];
        } else {
            printf("  Mismatch at x[%d] (%c) and y[%d] (%c)\n", i, x[i], i + j, y[i + j]);
            int shift_bc = bmBc[(unsigned char)y[i + j]];
            int shift_gs = bmGs[i];
            int shift = (shift_bc > shift_gs) ? shift_bc : shift_gs;
            printf("  Shift by max(bmBc[%c]=%d, bmGs[%d]=%d) = %d\n", y[i + j], shift_bc, i, shift_gs, shift);
            j += shift;
        }
    }
    free(bmGs);
}

int main() {
    char text[] = "abcababcababc";
    char pattern[] = "abcab";
    BM(pattern, strlen(pattern), text, strlen(text));
    return 0;
}
