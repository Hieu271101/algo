#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ASIZE 256  // Kích thước bảng ký tự (ASCII)
#define XSIZE 100  // Giới hạn pattern (có thể tăng nếu cần)

int matches[1000];
int matchCount = 0;
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
void OUTPUT(int pos) {
    matches[matchCount++] = pos;
}
void preBmBc(char *x, int m, int bmBc[]) {
    for (int i = 0; i < ASIZE; ++i)
        bmBc[i] = m;
    for (int i = 0; i < m - 1; ++i)
        bmBc[(unsigned char)x[i]] = m - i - 1;
    printf("\nBad Character Table:\n"); // in hết bảng bmBc
    for (int i = 0; i < ASIZE; i++) {
        if (bmBc[i] != m) // Chỉ in những ký tự xuất hiện trong mẫu
            printf("bmBc[%c] = %d\n", i, bmBc[i]);
    }
}

void suffixes(char *x, int m, int *suff) {
    int f = 0, g = m - 1;
    suff[m - 1] = m;
    for (int i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f])
                --g;
            suff[i] = f - g;
        }
    }
    printf("\nSuffixes Array:\n");
    for (int i = 0; i < m; i++)
        printf("suff[%d] = %d\n", i, suff[i]);
}

void preBmGs(char *x, int m, int bmGs[]) {
    int *suff = (int *)malloc(m * sizeof(int));
    suffixes(x, m, suff);
    for (int i = 0; i < m; ++i)
        bmGs[i] = m;
    int j = 0;
    for (int i = m - 1; i >= 0; --i) {
        if (suff[i] == i + 1) {
            for (; j < m - 1 - i; ++j) {
                if (bmGs[j] == m)
                    bmGs[j] = m - 1 - i;
            }
        }
    }
    for (int i = 0; i <= m - 2; ++i)
        bmGs[m - 1 - suff[i]] = m - 1 - i;
    // In ra bảng bmGs
    printf("\nGood Suffix Table:\n");
    for (int i = 0; i < m; i++)
        printf("bmGs[%d] = %d\n", i, bmGs[i]);
    free(suff);
}
void print_relevant_bmBc(char *x, int m, char *y, int n, int bmBc[]) {
    int seen[ASIZE] = {0};
    printf("bmBc (relevant chars):\n");
    printf("+------+-------+\n");
    printf("| Char | bmBc  |\n");
    printf("+------+-------+\n");
    for (int i = 0; i < m; ++i) seen[(unsigned char)x[i]] = 1;
    for (int i = 0; i < n; ++i) seen[(unsigned char)y[i]] = 1;
    for (int c = 0; c < 128; ++c) {
        if (seen[c]) {
            if (isprint(c))
                printf("|  '%c' | %5d |\n", c, bmBc[c]);
            else
                printf("| 0x%02X | %5d |\n", c, bmBc[c]);
        }
    }
    printf("+------+-------+\n\n");
}

void TBM_log(char *x, int m, char *y, int n) {
    int bmBc[ASIZE], bmGs[XSIZE];
    int j = 0, i, shift = m;
    int u = 0, v, turboShift, bcShift;
    int step = 0;

    // Preprocessing
    preBmBc(x, m, bmBc);
    preBmGs(x, m, bmGs);

    printf("Pattern: %s\n", x);
    printf("Text   : %s\n", y);
    printf("m = %d n = %d\n\n", m, n);

    // print bmGs
    printf("Precomputed bmGs: [");
    for (int k = 0; k < m; ++k) {
        printf("%d", bmGs[k]);
        if (k < m - 1) printf(", ");
    }
    printf("]\n");

    // print bmBc relevant
    print_relevant_bmBc(x, m, y, n, bmBc);

    printf("--- Search trace ---\n\n");

    while (j <= n - m) {
        step++;
        printf("Step %d: alignment j = %d, current u = %d\n", step, j, u);
        i = m - 1;
        shift = m; // default

        // comparison phase
        while (i >= 0 && x[i] == y[i + j]) {
            printf("  compare i=%d: pattern[%d]='%c' vs text[%d]='%c' -> match\n",
                   i, i, x[i], j + i, y[j + i]);
            --i;
            // turbo skip condition (uses last shift)
            if (u != 0 && i == m - 1 - shift) {
                printf("  turbo optimization: skip u=%d positions (i reduced by %d)\n", u, u);
                i -= u;
            }
        }

        if (i < 0) {
            printf("  -> FULL MATCH at j = %d\n", j);
            shift = bmGs[0];
            printf("  bmGs[0] = %d => shift = %d\n", bmGs[0], shift);
            u = m - shift;
            printf("  update u = m - shift = %d\n", u);
        } else {
            printf("  -> MISMATCH at i = %d, pattern[%d]='%c' vs text[%d]='%c'\n",
                   i, i, x[i], j + i, y[j + i]);
            v = m - 1 - i;
            turboShift = u - v;
            bcShift = bmBc[(unsigned char)y[j + i]] - m + 1 + i;
            printf("     v = m-1-i = %d\n", v);
            printf("     turboShift = u - v = %d - %d = %d\n", u, v, turboShift);
            printf("     bad-char '%c' -> bmBc = %d, bcShift = %d\n", y[j + i], bmBc[(unsigned char)y[j + i]], bcShift);

            int shift_choice = (turboShift > bcShift) ? turboShift : bcShift;
            printf("     max(turboShift, bcShift) = %d\n", shift_choice);

            if (shift_choice < bmGs[i]) {
                shift = bmGs[i];
                printf("     but bmGs[%d] = %d is larger -> shift = %d\n", i, bmGs[i], shift);
            } else {
                shift = shift_choice;
                printf("     bmGs[%d] = %d <= %d -> shift = %d\n", i, bmGs[i], shift_choice, shift);
            }

            if (shift == bmGs[i]) {
                u = (shift < v ? shift : v);
                printf("     shift == bmGs[i] so u = min(m-shift, v) = min(%d, %d) = %d\n", m - shift, v, u);
            } else {
                if (turboShift < bcShift) {
                    u = 0;
                    printf("     turboShift < bcShift -> reset u = 0\n");
                } else {
                    u = (shift < v ? shift : v);
                    printf("     else u = min(shift, v) = min(%d, %d) = %d\n", shift, v, u);
                }
            }
        }
        printf("  -> advance j by shift = %d -> new j = %d\n\n", shift, j + shift);
        j += shift;
    }

    printf("--- End ---\n");
}

void TBM(char *x, int m, char *y, int n) {
    int bcShift, i, j, shift, u, v, turboShift;
    int bmGs[XSIZE], bmBc[ASIZE];

    preBmGs(x, m, bmGs);
    preBmBc(x, m, bmBc);

    j = u = 0;
    shift = m;
    while (j <= n - m) {
        i = m - 1;
        while (i >= 0 && x[i] == y[i + j]) {
            --i;
            if (u != 0 && i == m - 1 - shift)
                i -= u;
        }

        if (i < 0) {
            OUTPUT(j);
            shift = bmGs[0];
            u = m - shift;
        } else {
            v = m - 1 - i;
            turboShift = u - v;
            bcShift = bmBc[(unsigned char)y[i + j]] - m + 1 + i;
            shift = MAX(turboShift, bcShift);
            shift = MAX(shift, bmGs[i]);
            if (shift == bmGs[i])
                u = MIN(m - shift, v);
            else {
                if (turboShift < bcShift)
                    shift = MAX(shift, u + 1);
                u = 0;
            }
        }
        j += shift;
    }
}


int main() {
    char text[] = "HERE IS A SIMPLE EXAMPLE";
    char pattern[] = "EXAMPLE";
    TBM(pattern, strlen(pattern), text, strlen(text));
    return 0;
}
