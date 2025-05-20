//#pragma once
//#include <iostream>
//#include <vector>
//#include <map>
//#include <algorithm>
//#include <random>
//#include <ctime>
//#include <fstream>
//#include <limits>
//using namespace std;
//
//// ��������� ��� �������� ���������� ������
//struct PlayerStats {
//    string name;             // ��� ������
//    int levelsCompleted = 0; // ���������� ���������� �������
//    int deadlocks = 0;       // ���������� ��������� �������
//    int correctDecisions = 0;// ���������� �������
//    int wrongDecisions = 0;  // ��������� �������
//};
//
//// ����� ��� ���������� �����
//class ResourceManagerGame {
//private:
//    // ��������� ����
//    int currentLevel = 1;            // ������� �������
//    int maxLevels = 5;               // ������������ ���������� �������
//    vector<int> availableResources;  // ��������� ������� �������
//    vector<vector<int>> processesNeeds; // ����������� ���������
//    vector<vector<int>> allocatedResources; // ��� ���������� �������
//    vector<bool> completedProcesses;  // ����������� ��������
//    PlayerStats stats;               // ���������� ������
//    bool gameSaved = false;          // ���� ���������� ����
//    string saveFile = "Save.txt"; // ���� ����������
//
//    // ��������� ��������� ����� (Mersenne Twister)
//    mt19937 rng; //���� ��������� ���� �������� ���������
//
//public:
//    // ����������� �������������� ��������� ��������� ����� � ��������� ����������
//    ResourceManagerGame() : rng(time(nullptr)) { loadGame(); }
//
//    // ��������� ������ ������
//    void generateLevel(int level);
//
//    // ����� �������� ��������� �������
//    void printSystemState();
//
//    // ��������� ���������� ������� �� ��������
//    void generateRandomRequest();
//
//    // ��������� ������� �������
//    void handleRequest(int processIndex, int resourceIndex);
//
//    // �������� �� ��������� ��������
//    bool isDeadlock();
//
//    // ���������� �������� ������
//    void restartLevel();
//
//    // ����������� ���������� ������
//    void showStats();
//
//    // ���������� ���� � ����
//    void saveGame();
//
//    // �������� ���� �� �����
//    void loadGame();
//
//    // �������� ������� ����
//    void play();
//};