#pragma once

namespace s21 {

/**
 * @struct Position
 * @brief Структура, представляющая позицию на игровом поле.
 */
struct Position {
  int x;
  int y;

  /**
   * @brief Конструктор структуры Position.
   *
   * @param x_coord Координата X.
   * @param y_coord Координата Y.
   */
  Position(int x_coord, int y_coord) : x(x_coord), y(y_coord) {}

  /**
   * @brief Оператор равенства для сравнения двух позиций.
   *
   * @param other Другая позиция для сравнения.
   * @return true, если позиции совпадают по координатам, иначе false.
   */
  bool operator==(const Position &other) const {
    return x == other.x && y == other.y;
  }

  /**
   * @brief Оператор неравенства для сравнения двух позиций.
   *
   * @param other Другая позиция для сравнения.
   * @return true, если позиции не совпадают по координатам, иначе false.
   */
  bool operator!=(const Position &other) const { return !(*this == other); }
};
}  // namespace s21