#pragma once
#include <string>
using namespace std;
class Resource {
private:
    int res_id;
    string res_name;
    int res_available;
    int res_total;
public:
    Resource(int id, const string& name, int n) : res_id(id), res_name(name), res_total(n), res_available(n) {}
    int get_res_id() const { return res_id; }
    string get_res_name() const { return res_name; }
    int get_res_available() const { return res_available; }
    int get_res_total() const { return res_total; }
    void set_res_available(int k) { if (k >= 0 && k <= res_total) res_available = k; }
    bool take(int k);
    void back(int k);
};