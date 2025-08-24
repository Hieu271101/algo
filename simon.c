#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Định nghĩa cấu trúc danh sách liên kết cho các transition */
typedef struct _cell {
    int element;            // trạng thái đích (next state)
    struct _cell *next;     // con trỏ đến phần tử tiếp theo
} *List;

/* Báo lỗi khi cấp phát bộ nhớ thất bại */
void error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

/* Tạo một transition từ p đến q trong danh sách L */
void setTransition(int p, int q, List L[]) {
    List cell = (List)malloc(sizeof(struct _cell));
    if (cell == NULL) {
        error("SIMON/setTransition: Memory allocation failed");
    }
    cell->element = q;
    cell->next = L[p];
    L[p] = cell;
}

/* Lấy trạng thái tiếp theo khi ở trạng thái p đọc ký tự c */
int getTransition(char *x, int m, int p, List L[], char c) {
    List cell;
    if (p < m - 1 && x[p + 1] == c) {
        return (p + 1);
    } else if (p > -1) {
        cell = L[p];
        while (cell != NULL) {
            if (x[cell->element] == c)
                return (cell->element);
            else
                cell = cell->next;
        }
        return (-1);
    } else {
        return (-1);
    }
}

/* Tiền xử lý Simon: xây dựng các danh sách L cho lazy DFA */
int preSimon(char *x, int m, List L[]) {
    int i, k, ell;
    List cell;
    memset(L, 0, m * sizeof(List));  // khởi tạo L với NULL
    ell = -1;

    for (i = 1; i < m; ++i) {
        k = ell;
        cell = (ell == -1 ? NULL : L[k]);
        ell = -1;

        if (x[i] == x[k + 1]) {
            ell = k + 1;
        } else {
            setTransition(i - 1, k + 1, L);
        }

        while (cell != NULL) {
            k = cell->element;
            if (x[i] == x[k]) {
                ell = k;
            } else {
                setTransition(i - 1, k, L);
            }
            cell = cell->next;
        }
    }
    return ell;
}

/* Thuật toán Simon: tìm pattern x trong text y */
void SIMON(char *x, int m, char *y, int n) {
    int j, ell, state;
    List *L = (List *)malloc(m * sizeof(List));
    if (!L) error("Memory allocation failed");

    /* Preprocessing */
    ell = preSimon(x, m, L);

    /* Searching */
    for (state = -1, j = 0; j < n; ++j) {
        state = getTransition(x, m, state, L, y[j]);
        if (state >= m - 1) {
            printf("Match found at index %d\n", j - m + 1);
            state = ell;
        }
    }

    /* Giải phóng bộ nhớ */
    for (int i = 0; i < m; i++) {
        List tmp = L[i];
        while (tmp) {
            List next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }
    free(L);
}

/* Demo */
int main() {
    char text[] = "ababaabababa";
    char pattern[] = "ababa";

    SIMON(pattern, strlen(pattern), text, strlen(text));

    return 0;
}
