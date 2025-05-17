#pragma once
#include <string>

// ────────── размеры окна ──────────
constexpr float SCREEN_WIDTH = 1024.f;
constexpr float SCREEN_HEIGHT = 768.f;

// ────────── главный прямоугольник ──────────
constexpr float MAIN_X = 30.f;
constexpr float MAIN_Y = 30.f;
constexpr float MAIN_WIDTH = 964.f;
constexpr float MAIN_HEIGHT = 708.f;

// ────────── блок «RESOURCES» ──────────
constexpr float FRAME_WIDTH_RES = 300.f;
constexpr float FRAME_HEIGHT_RES = 300.f;
constexpr float FRAME_X_RES = 60.f;
constexpr float FRAME_Y_RES = 100.f;

// ────────── блок «REQUEST» ──────────
constexpr float FRAME_WIDTH_REQ = 560.f;
constexpr float FRAME_HEIGHT_REQ = 200.f;
constexpr float FRAME_X_REQ = 400.f;
constexpr float FRAME_Y_REQ = 100.f;

// ────────── кнопки ──────────
constexpr float BUTTON_WIDTH = 270.f;
constexpr float BUTTON_HEIGHT = 60.f;
constexpr float BUTTON_X_GRANT = 400.f;
constexpr float BUTTON_X_DENY = 690.f;
constexpr float BUTTON_Y = 320.f;

// ────────── блок «PROCESSES» ──────────
constexpr float FRAME_WIDTH_PROC = 900.f;
constexpr float FRAME_HEIGHT_PROC = 280.f;
constexpr float FRAME_X_PROC = 60.f;
constexpr float FRAME_Y_PROC = 420.f;

// ────────── отступы текста ──────────
constexpr float FRAME_TEXT_OFFSET_X = 160.f;
constexpr float FRAME_TEXT_OFFSET_Y = -1.f;
constexpr float BUTTON_TEXT_OFFSET_X = 130.f;
constexpr float BUTTON_TEXT_OFFSET_Y = 13.f;

// ────────── позиционирование строк ──────────
constexpr int RES_OFFSET = 45;
constexpr int RES_STEP = 30;
constexpr int PROC_OFFSET = 80;
constexpr int PROC_STEP = 25;

// ────────── стартовое число процессов ──────────
constexpr int START_PROC_COUNT = 3;

// ────────── «линейка» для табличек ──────────
constexpr double  RES_NAME_WIDTH = 7;   // «CPU», «DISK» …
constexpr double  PROC_NAME_WIDTH = 15;  // «Notepad», «Explorer» …
constexpr double  CELL_WIDTH = 7;   // «16/16», «0/12» …
inline const std::string SEP = "  "; // два пробела
