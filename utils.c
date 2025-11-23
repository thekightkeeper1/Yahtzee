#include "utils.h"

int maxlen(wchar_t **arr, int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        int len = wcslen(arr[i]);
        if (len > max) {
            max = len;
        }
    }
    return max;
}
