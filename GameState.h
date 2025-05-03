#pragma once
#include "Resource.h"
#include "Process.h"
class GameState {
private:
    vector<Resource> resources;
    vector<Process> processes;
public:
    GameState() { initLevel1(); };
    void initLevel1();
    const vector<Resource>& get_vec_res() const { return resources; };
    const vector<Process>& get_vec_proc() const { return processes; };
    void reset_res();
    void reset_proc();
};
