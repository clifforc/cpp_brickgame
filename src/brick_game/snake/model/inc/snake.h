/**
 * @file snake.h
 * @brief Заголовочный файл с классом Snake.
 */

#pragma once

#include <deque>
#include <vector>

#include "../../../inc/defines.h"
#include "position.h"

namespace s21 {
/**
 * @struct SnakeSegment
 * @brief Структура, представляющая сегмент змейки.
 *
 * Содержит позицию сегмента на игровом поле.
 */
struct SnakeSegment {
  Position position;

  /**
   * @brief Конструктор структуры SnakeSegment.
   *
   * @param x Координата X сегмента.
   * @param y Координата Y сегмента.
   */
  SnakeSegment(int x, int y) : position(x, y) {}
};

/**
 * @enum Direction
 * @brief Перечисление направлений движения змейки.
 */
enum class Direction { up, down, left, right };

/**
 * @class Snake
 * @brief Класс, представляющий змейку в игре.
 *
 * Управляет движением, ростом и состоянием змейки.
 */
class Snake {
 public:
  /**
   * @brief Конструктор класса Snake.
   *
   * Инициализирует начальное состояние змейки.
   */
  Snake();

  /**
   * @brief Перемещает змейку в текущем направлении.
   *
   * Обновляет позиции сегментов змейки на игровом поле.
   */
  void Move();

  /**
   * @brief Увеличивает длину змейки.
   *
   * Добавляет новый сегмент к телу змейки.
   */
  void Grow();

  /**
   * @brief Изменяет направление движения змейки.
   *
   * @param new_dir Новое направление движения.
   */
  void ChangeDirection(Direction new_dir);

  /**
   * @brief Проверяет, столкнулась ли змейка сама с собой.
   *
   * @return true, если произошло столкновение, иначе false.
   */
  bool CheckSelfCollision() const;

  /**
   * @brief Получает позицию головы змейки.
   *
   * @return Константная ссылка на объект Position, представляющий голову
   * змейки.
   */
  const Position &GetHeadPosition() const;

  /**
   * @brief Получает тело змейки.
   *
   * @return Константная ссылка на deque, содержащий сегменты тела змейки.
   */
  const std::deque<SnakeSegment> &GetBody() const;

  /**
   * @brief Получает все занятые позиции змейки на игровом поле.
   *
   * @return Вектор объектов Position, представляющих занятые позиции.
   */
  const std::vector<Position> GetOccupiedPositon() const;

 private:
  std::deque<SnakeSegment>
      body_; /**< Двусторонняя очередь, представляющая тело змейки. */
  Direction current_direction_; /**< Текущее направление движения змейки. */
  Direction next_direction_;    /**< Следующее направление движения змейки. */
};

}  // namespace s21
