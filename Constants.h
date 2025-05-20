#pragma once
#include <string>

// ────────── размеры окна ──────────
const float SCREEN_WIDTH = 1024.f;
const float SCREEN_HEIGHT = 768.f;

// ────────── блок «MAIN» ──────────
const float MAIN_X = 30.f;
const float MAIN_Y = 30.f;
const float MAIN_WIDTH = 964.f;
const float MAIN_HEIGHT = 708.f;

// ────────── блок «RESOURCES» ──────────
const float FRAME_WIDTH_RES = 300.f;
const float FRAME_HEIGHT_RES = 300.f;
const float FRAME_X_RES = 60.f;
const float FRAME_Y_RES = 100.f;

// ────────── блок «REQUEST» ──────────
const float FRAME_WIDTH_REQ = 560.f;
const float FRAME_HEIGHT_REQ = 200.f;
const float FRAME_X_REQ = 400.f;
const float FRAME_Y_REQ = 100.f;

// ────────── кнопки на уровне ──────────
const float BUTTON_WIDTH = 270.f;
const float BUTTON_HEIGHT = 60.f;
const float BUTTON_X_GRANT = 400.f;
const float BUTTON_X_DENY = 690.f;
const float BUTTON_Y = 320.f;

// ────────── ввод ──────────
const float OFFSET_FOR_TEXT = SCREEN_HEIGHT / 7;
const float OK_FIELD_WIDTH = 400.f;
const float OK_BUTTION_WIDTH = 60.f;
const float OK_HEIGHT = 60;
const float OK_FIELD_X = SCREEN_WIDTH / 2 - 0.5 * OK_FIELD_WIDTH;
const float OK_Y = SCREEN_HEIGHT / 2 + OFFSET_FOR_TEXT;

// ────────── кнопки выбора уровня ──────────
const float BUT_OFFSET = 230;
const float LEVEL_WIDTH = 200;
const float LEVEL_HEIGHT = 100.f;
const float L1_X = 70;
const float L2_X = L1_X + BUT_OFFSET;
const float L3_X = L2_X + BUT_OFFSET;
const float L4_X = L3_X + BUT_OFFSET;
const float L_Y = 150;

// ────────── кнопка статистики ──────────
const float STATS_WIDTH = 1.2 * LEVEL_WIDTH;
const float STATS_HEIGHT = 1.2 * LEVEL_HEIGHT;
const float STATS_X = SCREEN_WIDTH / 2 - LEVEL_WIDTH / 2 * 1.2;
const float STATS_Y = SCREEN_HEIGHT * 2 / 3 + OFFSET_FOR_TEXT * 0.7;

// ────────── блок «ABOUT GAME» ──────────
const float INFO_WIDTH = 800;
const float INFO_HEIGHT = 150;
const float INFO_X = 110;
const float INFO_Y = SCREEN_HEIGHT / 2 * 0.8;

// ────────── блок «PROCESSES» ──────────
const float FRAME_WIDTH_PROC = 900.f;
const float FRAME_HEIGHT_PROC = 280.f;
const float FRAME_X_PROC = 60.f;
const float FRAME_Y_PROC = 420.f;

// ────────── отступы текста ──────────
const float FRAME_TEXT_OFFSET_X = 160.f;
const float FRAME_TEXT_OFFSET_Y = -1.f;
const float BUTTON_TEXT_OFFSET_X = 130.f;
const float BUTTON_TEXT_OFFSET_Y = 13.f;

// ────────── позиционирование строк ──────────
const int RES_OFFSET = 45;
const int RES_STEP = 30;
const int PROC_OFFSET = 80;
const int PROC_STEP = 28;

// ────────── стартовое число процессов ──────────
const int START_PROC_COUNT = 3;

// ────────── «линейка» для табличек ──────────
const double  RES_NAME_WIDTH = 7;   // «CPU», «DISK» …
const double  PROC_NAME_WIDTH = 15;  // «Notepad», «Explorer» …
const double  CELL_WIDTH = 7;   // «16/16», «0/12» …
inline const std::string SEP = "  "; // два пробела