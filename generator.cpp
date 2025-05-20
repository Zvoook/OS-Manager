//#include "generator.h"
//
//void ResourceManagerGame::generateLevel(int level) {
//    currentLevel = level;
//    // ���������� �������� � ��������� ������������� � �������
//    int resourceCount = 2 + level;
//    int processCount = 3 + level;
//
//    // ������� ���������� ������
//    availableResources.clear();
//    processesNeeds.clear();
//    allocatedResources.clear();
//    completedProcesses.clear();
//
//    // ��������� ��������� �������� (�� 3 �� 10 ������� ����)
//    uniform_int_distribution<int> res_dist(3, 10);
//    for (int i = 0; i < resourceCount; ++i) {
//        availableResources.push_back(res_dist(rng));
//    }
//
//    // ������������� ������ ������������ � ���������� ��������
//    processesNeeds.resize(processCount, vector<int>(resourceCount));
//    allocatedResources.resize(processCount, vector<int>(resourceCount, 0));
//    completedProcesses.resize(processCount, false);
//
//    // ��������� ������������ ��������� (�� 1 �� 5 ������� �������)
//    uniform_int_distribution<int> need_dist(1, 5);
//    for (int i = 0; i < processCount; ++i) {
//        for (int j = 0; j < resourceCount; ++j) {
//            processesNeeds[i][j] = need_dist(rng);
//        }
//    }
//}
//
//    // ����� �������� ��������� �������
//void ResourceManagerGame::printSystemState() {
//    //cout << "\n=== Level is " << currentLevel << " ===\n";
//    //cout << "Available resources are:\n";
//    // ����� ���������� ������� �������
//    for (size_t i = 0; i < availableResources.size(); ++i) {
//        //cout << "Resource " << i + 1 << ": " << availableResources[i] << "\n";
//    }
//
//    //cout << "\nProcesses and needs:\n";
//    // ����� ��������� ������� ��������
//    for (size_t i = 0; i < processesNeeds.size(); ++i) {
//        if (completedProcesses[i]) {
//            //cout << "Process " << i + 1 << ": ENDED\n";
//            continue;
//        }
//
//        //cout << "Process " << i + 1 << ": ";
//        for (size_t j = 0; j < processesNeeds[i].size(); ++j) {
//            //cout << "Resource " << j + 1 << " - " << allocatedResources[i][j] << "/" << processesNeeds[i][j] << "  ";
//        }
//    }
//}
//
//    // ��������� ���������� ������� �� ��������
//void ResourceManagerGame::generateRandomRequest() {
//    // ���� �������� (�������������) ���������
//    vector<int> activeProcesses;
//    for (size_t i = 0; i < processesNeeds.size(); ++i) {
//        if (!completedProcesses[i]) {
//            activeProcesses.push_back(i);
//        }
//    }
//
//    // �������� �� ���������� ���� ���������
//    if (activeProcesses.empty()) {
//        //cout << "All processes are completed! The level is passed!!\n";
//        stats.levelsCompleted++;
//        if (currentLevel < maxLevels) {
//            currentLevel++;
//            generateLevel(currentLevel); // ��������� ���������� ������
//        }
//        else {
//            //cout << "Congrats! All levels have been passed!\n";
//        }
//        return;
//    }
//
//    // ��������� ����� ��������
//    uniform_int_distribution<int> proc_dist(0, activeProcesses.size() - 1);
//    int processIndex = activeProcesses[proc_dist(rng)];
//
//    // ���� ��������, ������� ��� ����� ��������
//    vector<int> neededResources;
//    for (size_t j = 0; j < processesNeeds[processIndex].size(); ++j) {
//        if (allocatedResources[processIndex][j] < processesNeeds[processIndex][j]) {
//            neededResources.push_back(j);
//        }
//    }
//
//    // ��������� ����� �������
//    uniform_int_distribution<int> res_dist(0, neededResources.size() - 1);
//    int resourceIndex = neededResources[res_dist(rng)];
//
//    // ����� ������� ������������
//    //cout << "\nRequest: Process " << processIndex + 1 
//         //<< " needs " << (processesNeeds[processIndex][resourceIndex] - allocatedResources[processIndex][resourceIndex])
//         //<< " items of resources " << resourceIndex + 1 << "\n";
//
//    handleRequest(processIndex, resourceIndex);
//}
//
//    // ��������� ������� �������
//void ResourceManagerGame::handleRequest(int processIndex, int resourceIndex) {
//    int needed = processesNeeds[processIndex][resourceIndex] - allocatedResources[processIndex][resourceIndex];
//
//    //cout << "Available: " << availableResources[resourceIndex] << "\n";
//    //cout << "Give out resources?? (y/n): ";
//    char choice;
//    //cin >> choice;
//
//    if (choice == 'y' || choice == 'Y') {
//        if (availableResources[resourceIndex] >= needed) {
//            // ��������� ��������
//            availableResources[resourceIndex] -= needed;
//            allocatedResources[processIndex][resourceIndex] += needed;
//            //cout << "Resources are given\n";
//            stats.correctDecisions++;
//
//            // �������� �� ���������� ��������
//            bool completed = true;
//            for (size_t j = 0; j < processesNeeds[processIndex].size(); ++j) {
//                if (allocatedResources[processIndex][j] < processesNeeds[processIndex][j]) {
//                    completed = false;
//                    break;
//                }
//            }
//
//            if (completed) {
//                //cout << "Process " << processIndex + 1 << " ended!\n";
//                completedProcesses[processIndex] = true;
//            }
//        }
//        else {
//            // ������������ �������� - �����
//            //cout << "Mistake: Not enough resources! The system is at a standstill!\n";
//            stats.wrongDecisions++;
//            stats.deadlocks++;
//            restartLevel();
//        }
//    }
//    else {
//        // ����� ������� � �������
//        //cout << "Request rejected.\n";
//        stats.correctDecisions++;
//    }
//
//    // �������� �� ����� ����� ������� ��������
//    if (isDeadlock()) {
//        //cout << "A dead end has been discovered! The system is rebooting...\n";
//        stats.deadlocks++;
//        restartLevel();
//    }
//}
//
//    // �������� �� ��������� ��������
//bool ResourceManagerGame::isDeadlock() {
//    for (size_t i = 0; i < processesNeeds.size(); ++i) {
//        if (completedProcesses[i]) continue;
//
//        bool canProceed = false;
//        // ��������, ����� �� ������� �������� ���� ���� ������ ������
//        for (size_t j = 0; j < processesNeeds[i].size(); ++j) {
//            if (allocatedResources[i][j] < processesNeeds[i][j] &&
//                availableResources[j] > 0) {
//                canProceed = true;
//                break;
//            }
//        }
//
//        if (canProceed) return false; // ���� �� ���� ������� ����� ����������
//    }
//    //return true; // ��� �������� �������������
//}
//
//    // ���������� �������� ������
//void ResourceManagerGame::restartLevel() {
//    //cout << "Restarting the level...\n";
//    generateLevel(currentLevel);
//}
//
//    // ���������� ���� � ����
//void ResourceManagerGame::saveGame() {
//    ofstream out(saveFile);
//    if (out) {
//        out << stats.name << "\n";
//        out << currentLevel << "\n";
//        out << stats.levelsCompleted << "\n";
//        out << stats.deadlocks << "\n";
//        out << stats.correctDecisions << "\n";
//        out << stats.wrongDecisions << "\n";
//        gameSaved = true;
//        //cout << "���� ���������.\n";
//    }
//}
//
//    // �������� ���� �� �����
////void ResourceManagerGame::loadGame() {
////    ifstream in(saveFile);
////    if (in) {
////        getline(in, stats.name);
////        in >> currentLevel;
////        in >> stats.levelsCompleted;
////        in >> stats.deadlocks;
////        in >> stats.correctDecisions;
////        in >> stats.wrongDecisions;
////        //cout << "Welcome, " << stats.name << "!\n";
////    }
////    else {
////        //cout << "Enter your name: ";
////        getline(cin, stats.name);
////    }
////}
//
//    // �������� ������� ����
//void ResourceManagerGame::play() {
//    generateLevel(currentLevel);
//
//    while (true) {
//        printSystemState();
//        showStats();
//
//        // ���� ������� ��������
//        //���� ��������
//
//        int choice;
//        //cin >> choice;
//        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
//
//        switch (choice) {
//        case 1:
//            generateRandomRequest(); // ��������� ������ �������
//            break;
//        case 2:
//            saveGame(); // ���������� ���������
//            break;
//        case 3:
//            // ����� �� ���� � �������������� ����������
//            if (!gameSaved) {
//                //cout << "Save the results? (y/n): ";
//                char saveChoice;
//                //cin >> saveChoice;
//                if (saveChoice == 'y' || saveChoice == 'Y') {
//                    saveGame();
//                }
//            }
//            return;
//        default:
//            //cout << "Try again, pls.\n";
//        }
//    }
//}
//
//int main() {
//    ResourceManagerGame game;
//    game.play();
//    return 0;
//}