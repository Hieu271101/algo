#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_CHAR 256

// Shift-Or algorithm
void shift_or_search(const char *text, const char *pattern) {
    int m = strlen(pattern);
    int n = strlen(text);

    if (m == 0 || m > 64) { // Bitset giới hạn 64 bit
        printf("Pattern length must be between 1 and 64\n");
        return;
    }

    uint64_t mask[MAX_CHAR];
    for (int i = 0; i < MAX_CHAR; i++) {
        mask[i] = ~0ULL; // all bits set
    }

    for (int i = 0; i < m; i++) {
        mask[(unsigned char)pattern[i]] &= ~(1ULL << i);
    }

    uint64_t state = ~0ULL;
    for (int i = 0; i < n; i++) {
        state = (state << 1) | mask[(unsigned char)text[i]];
        if ((~state & (1ULL << (m - 1))) != 0) {
            printf("Match found at index %d\n", i - m + 1);
        }
    }
}

int main() {
    const char *text = "ababcabcabababd";
    const char *pattern = "ababd";

    shift_or_search(text, pattern);

    return 0;
}
