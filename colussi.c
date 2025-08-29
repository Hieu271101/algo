#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define XSIZE 1000
#define ASIZE 256

int preColussi(char *x, int m, int h[], int next[], int shift[]) {
    int i, k, nd, q, r, s;
    int hmax[XSIZE], kmin[XSIZE], nhd0[XSIZE], rmin[XSIZE];

    printf("\n===== Preprocessing Colussi =====\n");

    /* ===== Computation of hmax ===== */
    printf("\n--- Computing hmax[] ---\n");
    i = k = 1;
    hmax[0] = 0;
    do {
        while (i < m && x[i] == x[i - k])
            i++;
        hmax[k] = i;
        printf("hmax[%d] = %d\n", k, hmax[k]);
        q = k + 1;
        while (q <= m && hmax[q - k] + k < i) {
            hmax[q] = hmax[q - k] + k;
            printf("hmax[%d] = %d (propagated)\n", q, hmax[q]);
            q++;
        }
        k = q;
        if (k == i + 1)
            i = k;
    } while (k <= m);

    /* ===== Computation of kmin ===== */
    printf("\n--- Computing kmin[] ---\n");
    memset(kmin, 0, m * sizeof(int));
    for (i = m; i >= 1; --i)
        if (hmax[i] < m)
            kmin[hmax[i]] = i;

    for (i = 0; i < m; i++)
        printf("kmin[%d] = %d\n", i, kmin[i]);

    /* ===== Computation of rmin ===== */
    printf("\n--- Computing rmin[] ---\n");
    r = m;
    for (i = m - 1; i >= 0; --i) {
        if (hmax[i + 1] == m)
            r = i + 1;
        if (kmin[i] == 0)
            rmin[i] = r;
        else
            rmin[i] = 0;
    }
    for (i = 0; i < m; i++)
        printf("rmin[%d] = %d\n", i, rmin[i]);

    /* ===== Computation of h[] ===== */
    printf("\n--- Computing h[] ---\n");
    s = -1;
    r = m;
    for (i = 0; i < m; ++i) {
        if (kmin[i] == 0)
            h[--r] = i;
        else
            h[++s] = i;
    }
    nd = s;

    printf("Order of comparisons (h[]):\n");
    for (i = 0; i < m; i++)
        printf("h[%d] = %d\n", i, h[i]);
    printf("nd = %d\n", nd);

    /* ===== Computation of shift[] ===== */
    printf("\n--- Computing shift[] ---\n");
    for (i = 0; i <= nd; ++i)
        shift[i] = kmin[h[i]];
    for (i = nd + 1; i < m; ++i)
        shift[i] = rmin[h[i]];
    shift[m] = rmin[0];

    for (i = 0; i <= m; i++)
        printf("shift[%d] = %d\n", i, shift[i]);

    /* ===== Computation of nhd0[] ===== */
    s = 0;
    for (i = 0; i < m; ++i) {
        nhd0[i] = s;
        if (kmin[i] > 0)
            ++s;
    }

    /* ===== Computation of next[] ===== */
    printf("\n--- Computing next[] ---\n");
    for (i = 0; i <= nd; ++i)
        next[i] = nhd0[h[i] - kmin[h[i]]];
    for (i = nd + 1; i < m; ++i)
        next[i] = nhd0[m - rmin[h[i]]];
    next[m] = nhd0[m - rmin[h[m - 1]]];

    for (i = 0; i <= m; i++)
        printf("next[%d] = %d\n", i, next[i]);

    printf("\n===== End of Preprocessing =====\n");

    return nd;
}

void COLUSSI(char *x, int m, char *y, int n) {
    int i, j, last, nd;
    int h[XSIZE], next[XSIZE], shift[XSIZE];

    /* Preprocessing */
    nd = preColussi(x, m, h, next, shift);

    /* Searching */
    printf("\n===== Searching (Colussi) =====\n");
    i = j = 0;
    last = -1;

    while (j <= n - m) {
        printf("\nWindow at j = %d:\n", j);
        while (i < m && last < j + h[i] && x[h[i]] == y[j + h[i]]) {
            printf("  Match at x[%d] (%c) and y[%d] (%c)\n", h[i], x[h[i]], j + h[i], y[j + h[i]]);
            i++;
        }
        if (i >= m || last >= j + h[i]) {
            printf(">>> Match found at position %d\n", j);
            last = j + m;
        }
        if (i > nd)
            last = j + m - 1;

        printf("Mismatch or end: i = %d, shift by shift[%d] = %d\n", i, i, shift[i]);
        j += shift[i];
        i = next[i];
        printf("Next i = %d\n", i);
    }
    printf("\n===== End of Searching =====\n");
}

int main() {
    char text[] = "abcababcababc";
    char pattern[] = "abcab";

    COLUSSI(pattern, strlen(pattern), text, strlen(text));
    return 0;
}
