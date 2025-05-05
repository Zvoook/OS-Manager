#include "Resource.h"
bool Resource::take(int k) {
    if (k <= res_available) {
        res_available -= k;
        return true;
    }
    return false;
}

void Resource::back(int k) {
    res_available += k;
    if (res_available > res_total) {
        res_available = res_total;
    }
}
