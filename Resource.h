#pragma once
#include <string>
using namespace std;
class Resource {
private:
    string name_res;
    int total_res;
    int available_res;
public:
    Resource(const string& name, int n) : name_res(name), total_res(n), available_res(n) {}
    string get_name_res() const { return name_res; };
    int get_total() const { return total_res; };
    int get_available() const { return available_res; };
    void set_available(int value) {if (value >= 0 && value <= total_res) available_res = value;}
    void reset() { available_res = total_res; };
};
