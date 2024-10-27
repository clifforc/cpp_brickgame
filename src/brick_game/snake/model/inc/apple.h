/**
 * @file apple.h
 * @brief Заголовочный файл, содержащий класс Apple для управления яблоками в
 * игре.
 */

#pragma once

#include <chrono>
#include <random>
#include <vector>

#include "../../../inc/defines.h"
#include "position.h"

namespace s21 {

/**
 * @class Apple
 * @brief Класс для управления яблоками в игре.
 *
 * Отвечает за создание и позиционирование яблок на игровом поле.
 */
class Apple {
 public:
  /**
   * @brief Конструктор класса Apple.
   *
   * Инициализирует генератор случайных чисел и создает первое яблоко.
   */
  Apple();

  /**
   * @brief Спавнит новое яблоко на игровом поле.
   *
   * Генерирует случайную позицию для яблока, избегая занятых позиций.
   *
   * @param occupied_position Вектор занятых позиций на игровом поле.
   */
  void SpawnApple(const std::vector<Position> &occupied_position);

  /**
   * @brief Получает текущую позицию яблока.
   *
   * @return Константная ссылка на объект Position, представляющий позицию
   * яблока.
   */
  const Position &GetPosition() const;

 private:
  Position position_;      /**< Текущая позиция яблока на игровом поле. */
  std::mt19937 generator_; /**< Генератор случайных чисел для спавна яблок. */
};

}  // namespace s21
