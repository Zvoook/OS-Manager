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
//// Структура для хранения статистики игрока
//struct PlayerStats {
//    string name;             // Имя игрока
//    int levelsCompleted = 0; // Количество пройденных уровней
//    int deadlocks = 0;       // Количество возникших тупиков
//    int correctDecisions = 0;// Правильные решения
//    int wrongDecisions = 0;  // Ошибочные решения
//};
//
//// Класс для управления игрой
//class ResourceManagerGame {
//private:
//    // Состояние игры
//    int currentLevel = 1;            // Текущий уровень
//    int maxLevels = 5;               // Максимальное количество уровней
//    vector<int> availableResources;  // Доступные ресурсы системы
//    vector<vector<int>> processesNeeds; // Потребности процессов
//    vector<vector<int>> allocatedResources; // Уже выделенные ресурсы
//    vector<bool> completedProcesses;  // Завершенные процессы
//    PlayerStats stats;               // Статистика игрока
//    bool gameSaved = false;          // Флаг сохранения игры
//    string saveFile = "Save.txt"; // Файл сохранения
//
//    // Генератор случайных чисел (Mersenne Twister)
//    mt19937 rng; //тута запускаем этот японский генератор
//
//public:
//    // Конструктор инициализирует генератор случайных чисел и загружает сохранение
//    ResourceManagerGame() : rng(time(nullptr)) { loadGame(); }
//
//    // Генерация нового уровня
//    void generateLevel(int level);
//
//    // Вывод текущего состояния системы
//    void printSystemState();
//
//    // Генерация случайного запроса от процесса
//    void generateRandomRequest();
//
//    // Обработка запроса игроком
//    void handleRequest(int processIndex, int resourceIndex);
//
//    // Проверка на тупиковую ситуацию
//    bool isDeadlock();
//
//    // Перезапуск текущего уровня
//    void restartLevel();
//
//    // Отображение статистики игрока
//    void showStats();
//
//    // Сохранение игры в файл
//    void saveGame();
//
//    // Загрузка игры из файла
//    void loadGame();
//
//    // Основной игровой цикл
//    void play();
//};