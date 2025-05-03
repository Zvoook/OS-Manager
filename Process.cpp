#include "Process.h"
vector<int> Process::get_rest() const {
    vector<int> rest(max_res_in_proc.size());
    for (size_t i = 0; i < max_res_in_proc.size(); ++i) {
        rest[i] = max_res_in_proc[i] - allocated_res_for_proc[i];
    }
    return rest;
}

void Process::allocate_res(int ind, int k) {
    if (ind >= 0 && ind < static_cast<int>(allocated_res_for_proc.size())) {
        allocated_res_for_proc[ind] += k;
    }
}
void Process::reset() {
    allocated_res_for_proc.assign(allocated_res_for_proc.size(), 0);
}
