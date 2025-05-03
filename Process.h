#pragma once
#include <string>
#include <vector>
using namespace std;
class Process {
private:
    string name_proc;
    vector<int> max_res_in_proc;
    vector<int> allocated_res_for_proc;
public:
    Process(const string& name, const vector<int>& max) : name_proc(name), max_res_in_proc(max), allocated_res_for_proc(max.size(), 0) {};
    string get_name_proc() const { return name_proc; };
    const vector<int>& get_max_proc() const { return max_res_in_proc; };
    const vector<int>& get_allocated_proc() const { return allocated_res_for_proc; };
    vector<int> get_rest() const;
    void allocate_res(int ind, int k);
    void reset();
};
