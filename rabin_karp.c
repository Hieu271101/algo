#include <stdio.h>
#include <string.h>

#define BASE 256       // Kích thước bảng ký tự (ASCII)
#define MOD  101       // Số nguyên tố để modulo (giảm collision)

void OUTPUT(int pos) {
    printf("Pattern found at index %d\n", pos);
}

// Hàm REHASH: cập nhật hash khi trượt cửa sổ
int REHASH(char oldChar, char newChar, int oldHash, int h) {
    int newHash = (BASE * (oldHash - oldChar * h) + newChar) % MOD;
    if (newHash < 0)
        newHash += MOD;
    return newHash;
}

// Thuật toán Karp–Rabin dựa theo pseudo-code
void KR(char *pattern, int m, char *text, int n) {
    int h = 1;     // BASE^(m-1) % MOD
    int p_hash = 0; // hash của pattern
    int t_hash = 0; // hash của cửa sổ hiện tại trong text
    int i, j;

    // Tính h = BASE^(m-1) % MOD
    for (i = 0; i < m - 1; i++)
        h = (h * BASE) % MOD;

    // Tính hash ban đầu cho pattern và text
    for (i = 0; i < m; i++) {
        p_hash = (BASE * p_hash + pattern[i]) % MOD;
        t_hash = (BASE * t_hash + text[i]) % MOD;
    }

    // Tìm kiếm
    for (j = 0; j <= n - m; j++) {
        // Nếu hash trùng, kiểm tra ký tự
        if (p_hash == t_hash && memcmp(pattern, text + j, m) == 0) {
            OUTPUT(j);
        }

        // Cập nhật hash cho cửa sổ tiếp theo
        if (j < n - m) {
            t_hash = REHASH(text[j], text[j + m], t_hash, h);
        }
    }
}

int main() {
    char text[] = "ABCCDDAEFG";
    char pattern[] = "CDD";
    KR(pattern, strlen(pattern), text, strlen(text));
    return 0;
}
