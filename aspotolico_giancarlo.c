#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASIZE 256 // Kích thước bảng ASCII

// ===== Bad Character =====
void preBmBc(char *x, int m, int bmBc[]) {
    printf("\n--- preBmBc (Bad Character Rule) ---\n");
    for (int i = 0; i < ASIZE; ++i)
        bmBc[i] = m;

    for (int i = 0; i < m - 1; ++i) {
        bmBc[(unsigned char)x[i]] = m - i - 1;
        printf("Char '%c' at pattern[%d] -> bmBc[%c] = %d\n", x[i], i, x[i], m - i - 1);
    }
    printf("\nBad Character Table (only seen chars):\n");
    for (int i = 0; i < ASIZE; i++) {
        if (bmBc[i] != m)
            printf("bmBc[%c] = %d\n", i, bmBc[i]);
    }
}

// ===== suff (prefix-suffix giống trong BM) =====
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

// ===== Good Suffix =====
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
    printf("\nGood Suffix Table after first pass:\n");
    for (i = 0; i < m; i++)
        printf("bmGs[%d] = %d\n", i, bmGs[i]);

    for (i = 0; i <= m - 2; ++i)
        bmGs[m - 1 - suff[i]] = m - 1 - i;

    printf("\nGood Suffix Table after second pass:\n");
    for (i = 0; i < m; i++)
        printf("bmGs[%d] = %d\n", i, bmGs[i]);

    free(suff);
}

/*
 * ===== Apostolico–Giancarlo (AG) =====
 * Ý tưởng: dùng mảng skip[] để tái sử dụng các so khớp suffix đã khớp ở cửa sổ trước,
 * giảm số lần so sánh lặp lại (tăng tốc so với BM cổ điển).
 *
 * Ghi chú:
 * - skip[i] = độ dài suffix (tính từ cuối mẫu) đã biết là khớp ở vị trí i trong lần trước.
 * - Khi mismatch, dịch cửa sổ theo max(bmGs[i], bcShift) như BM,
 *   đồng thời trượt (memmove) vùng skip sang phải để đúng vị trí cửa sổ mới.
 */
void AG(char *x, int m, char *y, int n) {
    int *bmGs = (int *)malloc(m * sizeof(int));
    int bmBc[ASIZE];
    int *suff = (int *)malloc(m * sizeof(int)); // chỉ để log/giải thích, bmGs đã tính từ suff nội bộ trong preBmGs

    // Tính trước
    preBmBc(x, m, bmBc);

    // Lấy lại suff để dùng trong logic AG (preBmGs ở trên tự cấp phát suff nội bộ, nên ta tự gọi lại suffixes)
    suffixes(x, m, suff);
    preBmGs(x, m, bmGs);

    // skip khởi tạo 0
    int *skip = (int *)calloc(m, sizeof(int));

    printf("\n--- Matching Process (Apostolico–Giancarlo) ---\n");

    int j = 0;
    while (j <= n - m) {
        int i = m - 1;

        printf("\nChecking window at j = %d  (y[%d..%d])\n", j, j, j + m - 1);

        while (i >= 0) {
            int k = skip[i];     // suffix đã biết khớp ở vị trí i
            int s = suff[i];     // độ dài suffix “tốt” tại i

            if (k > 0) {
                // Đã biết trước có k ký tự cuối khớp ở vị trí i
                if (k < s) {
                    // Ta chỉ cần nhảy qua k ký tự đã biết là khớp và dừng để tính shift
                    printf("  Reuse skip at i=%d: k=%d < suff[%d]=%d -> reuse and break to shift\n", i, k, i, s);
                    i -= k;
                    break;
                } else if (k == s) {
                    // Trường hợp trùng khít với suff: bỏ nguyên khúc s và tiếp tục so khớp phần còn lại
                    printf("  Reuse skip at i=%d: k==s==%d -> i -= s\n", i, s);
                    i -= s;
                    if (i < 0) break;
                    // tiếp tục vòng while để so sánh/xử lý tiếp
                } else { // k > s
                    // Dịch cửa sổ sớm bằng (k - s) mà không so sánh thêm
                    int early = k - s;
                    printf("  Reuse skip at i=%d: k=%d > s=%d -> early shift window by %d\n", i, k, s, early);
                    j += early;

                    // Trượt skip tương ứng cho cửa sổ mới
                    if (early >= m) {
                        memset(skip, 0, m * sizeof(int));
                    } else {
                        memmove(skip, skip + early, (m - early) * sizeof(int));
                        memset(skip + m - early, 0, early * sizeof(int));
                    }
                    goto next_window; // chuyển sang window mới
                }
            } else {
                // Chưa có thông tin reuse -> so sánh bình thường
                if (x[i] == y[i + j]) {
                    printf("  Match at x[%d] (%c) and y[%d] (%c)\n", i, x[i], i + j, y[i + j]);
                    --i;
                } else {
                    printf("  Mismatch at x[%d] (%c) and y[%d] (%c)\n", i, x[i], i + j, y[i + j]);
                    break;
                }
            }
        }

        if (i < 0) {
            // Tìm thấy
            printf(">>> Found at position %d\n", j);
            // Sau khi tìm thấy: theo AG, đánh dấu skip[m-1] = m (đã khớp toàn bộ)
            skip[m - 1] = m;

            int shift = bmGs[0];
            printf("  Shift (full match) by bmGs[0]=%d\n", shift);

            j += shift;

            // Trượt skip
            if (shift >= m) {
                memset(skip, 0, m * sizeof(int));
            } else {
                memmove(skip, skip + shift, (m - shift) * sizeof(int));
                memset(skip + m - shift, 0, shift * sizeof(int));
            }
        } else {
            // Mismatch tại i
            // Bad-character shift (chuẩn): bc = bmBc[ch] - (m - 1 - i); tối thiểu 1
            int bc = bmBc[(unsigned char)y[i + j]] - (m - 1 - i);
            if (bc < 1) bc = 1;

            int gs = bmGs[i];
            int shift = (bc > gs) ? bc : gs;

            // AG: ghi nhớ đã khớp phần đuôi dài m-1-i ký tự tại cửa sổ này
            skip[m - 1] = m - 1 - i;

            printf("  Shift by max(bc=%d, gs=%d) = %d; set skip[m-1]=%d\n",
                   bc, gs, shift, m - 1 - i);

            j += shift;

            // Trượt skip theo lượng dịch
            if (shift >= m) {
                memset(skip, 0, m * sizeof(int));
            } else {
                memmove(skip, skip + shift, (m - shift) * sizeof(int));
                memset(skip + m - shift, 0, shift * sizeof(int));
            }
        }

    next_window:
        // tiếp tục vòng while lớn
        ;
    }

    free(skip);
    free(bmGs);
    free(suff);
}

int main() {
    char text[] = "abcababcababc";
    char pattern[] = "abcab";

    // Gọi AG thay vì BM
    AG(pattern, (int)strlen(pattern), text, (int)strlen(text));
    return 0;
}
