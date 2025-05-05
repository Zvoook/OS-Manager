#pragma once
#include <string>
#include <vector>
using namespace std;
class Process {
private:
    int proc_id;
    string proc_name;
    bool done;            
    vector<int> max_require; 
    vector<int> alloc; 

public:
    Process(int id, const string& name, const vector<int>& r) : proc_id(id), proc_name(name), max_require(r), done(false) {alloc.resize(max_require.size(), 0);}
    int get_proc_id() const { return proc_id; }
    string get_proc_name() const { return proc_name; }
    bool is_done() const { return done; }
    const vector<int>& get_max_require() const { return max_require; }
    const vector<int>& get_alloc() const { return alloc; }
    vector<int> get_rest() const;
    void set_done(bool status) { done = status; };
    void set_alloc(const vector<int>& res);
    void add_res(int res_id, int amount);
};
