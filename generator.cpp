#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <ctime>
#include <fstream>
#include <limits>

using namespace std;

// Структура для хранения статистики игрока
struct PlayerStats {
    string name;             // Имя игрока
    int levelsCompleted = 0; // Количество пройденных уровней
    int deadlocks = 0;       // Количество возникших тупиков
    int correctDecisions = 0;// Правильные решения
    int wrongDecisions = 0;  // Ошибочные решения
};

// Класс для управления игрой
class ResourceManagerGame {
private:
    // Состояние игры
    int currentLevel = 1;            // Текущий уровень
    int maxLevels = 5;               // Максимальное количество уровней
    vector<int> availableResources;  // Доступные ресурсы системы
    vector<vector<int>> processesNeeds; // Потребности процессов
    vector<vector<int>> allocatedResources; // Уже выделенные ресурсы
    vector<bool> completedProcesses;  // Завершенные процессы
    PlayerStats stats;               // Статистика игрока
    bool gameSaved = false;          // Флаг сохранения игры
    string saveFile = "blah-blah-blah.txt"; // Файл сохранения

    // Генератор случайных чисел (Mersenne Twister)
    mt19937 rng; //тута запускаем этот японский генератор

public:
    // Конструктор инициализирует генератор случайных чисел и загружает сохранение
    ResourceManagerGame() : rng(time(nullptr)) {
        loadGame();
    }

    // Генерация нового уровня
    void generateLevel(int level) {
        currentLevel = level;
        // Количество ресурсов и процессов увеличивается с уровнем
        int resourceCount = 2 + level; 
        int processCount = 3 + level;  

        // Очистка предыдущих данных
        availableResources.clear();
        processesNeeds.clear();
        allocatedResources.clear();
        completedProcesses.clear();

        // Генерация доступных ресурсов (от 3 до 10 каждого типа)
        uniform_int_distribution<int> res_dist(3, 10);
        for (int i = 0; i < resourceCount; ++i) {
            availableResources.push_back(res_dist(rng));
        }

        // Инициализация матриц потребностей и выделенных ресурсов
        processesNeeds.resize(processCount, vector<int>(resourceCount));
        allocatedResources.resize(processCount, vector<int>(resourceCount, 0));
        completedProcesses.resize(processCount, false);

        // Генерация потребностей процессов (от 1 до 5 каждого ресурса)
        uniform_int_distribution<int> need_dist(1, 5);
        for (int i = 0; i < processCount; ++i) {
            for (int j = 0; j < resourceCount; ++j) {
                processesNeeds[i][j] = need_dist(rng);
            }
        }
    }

    // Вывод текущего состояния системы
    void printSystemState() {
        //cout << "\n=== Level is " << currentLevel << " ===\n";
        //cout << "Available resources are:\n";
        // Вывод количества каждого ресурса
        for (size_t i = 0; i < availableResources.size(); ++i) {
            //cout << "Resource " << i + 1 << ": " << availableResources[i] << "\n";
        }

        //cout << "\nProcesses and needs:\n";
        // Вывод состояния каждого процесса
        for (size_t i = 0; i < processesNeeds.size(); ++i) {
            if (completedProcesses[i]) {
                //cout << "Process " << i + 1 << ": ENDED\n";
                continue;
            }

            //cout << "Process " << i + 1 << ": ";
            for (size_t j = 0; j < processesNeeds[i].size(); ++j) {
                //cout << "Resource " << j + 1 << " - " 
                     << allocatedResources[i][j] << "/" << processesNeeds[i][j] << "  ";
            }
            //cout << "\n";
        }
    }

    // Генерация случайного запроса от процесса
    void generateRandomRequest() {
        // Сбор активных (незавершенных) процессов
        vector<int> activeProcesses;
        for (size_t i = 0; i < processesNeeds.size(); ++i) {
            if (!completedProcesses[i]) {
                activeProcesses.push_back(i);
            }
        }

        // Проверка на завершение всех процессов
        if (activeProcesses.empty()) {
            //cout << "All processes are completed! The level is passed!!\n";
            stats.levelsCompleted++;
            if (currentLevel < maxLevels) {
                currentLevel++;
                generateLevel(currentLevel); // Генерация следующего уровня
            } else {
                //cout << "Congrats! All levels have been passed!\n";
            }
            return;
        }

        // Случайный выбор процесса
        uniform_int_distribution<int> proc_dist(0, activeProcesses.size() - 1);
        int processIndex = activeProcesses[proc_dist(rng)];
        
        // Сбор ресурсов, которые еще нужны процессу
        vector<int> neededResources;
        for (size_t j = 0; j < processesNeeds[processIndex].size(); ++j) {
            if (allocatedResources[processIndex][j] < processesNeeds[processIndex][j]) {
                neededResources.push_back(j);
            }
        }

        // Случайный выбор ресурса
        uniform_int_distribution<int> res_dist(0, neededResources.size() - 1);
        int resourceIndex = neededResources[res_dist(rng)];

        // Вывод запроса пользователю
        //cout << "\nRequest: Process " << processIndex + 1 
             //<< " needs " << (processesNeeds[processIndex][resourceIndex] - allocatedResources[processIndex][resourceIndex])
             //<< " items of resources " << resourceIndex + 1 << "\n";

        handleRequest(processIndex, resourceIndex);
    }

    // Обработка запроса игроком
    void handleRequest(int processIndex, int resourceIndex) {
        int needed = processesNeeds[processIndex][resourceIndex] - allocatedResources[processIndex][resourceIndex];
        
        //cout << "Available: " << availableResources[resourceIndex] << "\n";
        //cout << "Give out resources?? (y/n): ";
        char choice;
        //cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            if (availableResources[resourceIndex] >= needed) {
                // Выделение ресурсов
                availableResources[resourceIndex] -= needed;
                allocatedResources[processIndex][resourceIndex] += needed;
                //cout << "Resources are given\n";
                stats.correctDecisions++;

                // Проверка на завершение процесса
                bool completed = true;
                for (size_t j = 0; j < processesNeeds[processIndex].size(); ++j) {
                    if (allocatedResources[processIndex][j] < processesNeeds[processIndex][j]) {
                        completed = false;
                        break;
                    }
                }

                if (completed) {
                    //cout << "Process " << processIndex + 1 << " ended!\n";
                    completedProcesses[processIndex] = true;
                }
            } else {
                // Недостаточно ресурсов - тупик
                //cout << "Mistake: Not enough resources! The system is at a standstill!\n";
                stats.wrongDecisions++;
                stats.deadlocks++;
                restartLevel();
            }
        } else {
            // Игрок отказал в запросе
            //cout << "Request rejected.\n";
            stats.correctDecisions++;
        }

        // Проверка на тупик после каждого действия
        if (isDeadlock()) {
            //cout << "A dead end has been discovered! The system is rebooting...\n";
            stats.deadlocks++;
            restartLevel();
        }
    }

    // Проверка на тупиковую ситуацию
    bool isDeadlock() {
        for (size_t i = 0; i < processesNeeds.size(); ++i) {
            if (completedProcesses[i]) continue;

            bool canProceed = false;
            // Проверка, может ли процесс получить хоть один нужный ресурс
            for (size_t j = 0; j < processesNeeds[i].size(); ++j) {
                if (allocatedResources[i][j] < processesNeeds[i][j] && 
                    availableResources[j] > 0) {
                    canProceed = true;
                    break;
                }
            }

            if (canProceed) return false; // Хотя бы один процесс может продолжить
        }
        //return true; // Все процессы заблокированы
    }

    // Перезапуск текущего уровня
    void restartLevel() {
        //cout << "Restarting the level...\n";
        generateLevel(currentLevel);
    }

    // Отображение статистики игрока
    void showStats() {
        //cout << "\n=== Player's statistics ===\n";
        //cout << "Name: " << stats.name << "\n";
        //cout << "Levels passed: " << stats.levelsCompleted << "\n";
        //cout << "Deadlocks: " << stats.deadlocks << "\n";
        //cout << "Correct decisions: " << stats.correctDecisions << "\n";
        //cout << "Wrong decisions: " << stats.wrongDecisions << "\n";
    }

    // Сохранение игры в файл
    void saveGame() {
        ofstream out(saveFile);
        if (out) {
            out << stats.name << "\n";
            out << currentLevel << "\n";
            out << stats.levelsCompleted << "\n";
            out << stats.deadlocks << "\n";
            out << stats.correctDecisions << "\n";
            out << stats.wrongDecisions << "\n";
            gameSaved = true;
            //cout << "Игра сохранена.\n";
        }
    }

    // Загрузка игры из файла
    void loadGame() {
        ifstream in(saveFile);
        if (in) {
            getline(in, stats.name);
            in >> currentLevel;
            in >> stats.levelsCompleted;
            in >> stats.deadlocks;
            in >> stats.correctDecisions;
            in >> stats.wrongDecisions;
            //cout << "Welcome, " << stats.name << "!\n";
        } else {
            //cout << "Enter your name: ";
            getline(cin, stats.name);
        }
    }

    // Основной игровой цикл
    void play() {
        generateLevel(currentLevel);

        while (true) {
            printSystemState();
            showStats();

            // Меню игровых действий
            //Дима пропишет

            int choice;
            //cin >> choice;
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    generateRandomRequest(); // Обработка нового запроса
                    break;
                case 2:
                    saveGame(); // Сохранение прогресса
                    break;
                case 3:
                    // Выход из игры с подтверждением сохранения
                    if (!gameSaved) {
                        //cout << "Save the results? (y/n): ";
                        char saveChoice;
                        //cin >> saveChoice;
                        if (saveChoice == 'y' || saveChoice == 'Y') {
                            saveGame();
                        }
                    }
                    return;
                default:
                    //cout << "Try again, pls.\n";
            }
        }
    }
};

int main() {
    ResourceManagerGame game;
    game.play();
    return 0;
}
