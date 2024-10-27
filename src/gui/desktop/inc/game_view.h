/**
 * @file game_view.h
 * @brief Заголовочный файл для фронтенда, использующего gtk+ для BrickGame.
 */

#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <gtkmm.h>

#include "../../../brick_game/inc/defines.h"
#include "../../../brick_game/snake/controller/inc/game_controller.h"
#include "../../../brick_game/snake/model/inc/game_model.h"
#include "../../../brick_game/tetris/inc/backend.h"
#include "../../../brick_game/tetris/inc/fsm_t.h"

namespace s21 {

/**
 * @enum Game
 * @brief Перечисляет доступные игры.
 */
typedef enum { snake, tetris } Game;

/**
 * @class GameWindow
 * @brief Представляет главное игровое окно, обрабатывающее UI и взаимодействия.
 *
 * Наследуется от Gtk::Window и управляет компонентами интерфейса, обработкой
 * событий и состоянием игры как для Snake, так и для Tetris.
 */
class GameWindow : public Gtk::Window {
 public:
  /**
   * @brief Конструктор объекта GameWindow.
   */
  GameWindow();

  /**
   * @brief Деструктор объекта GameWindow и отключает сигналы.
   */
  ~GameWindow() override {
    connection_start_snake_.disconnect();
    connection_start_tetris_.disconnect();
    connection_key_controller_.disconnect();
    timeout_connection_.disconnect();
    connection_key_pressed_.disconnect();
    connection_key_released_.disconnect();
  };

  /**
   * @brief Обрабатывает события клавиш для игры Snake.
   *
   * @param keyval Значение клавиши.
   * @param keycode Код клавиши.
   * @param state Состояние модификаторов.
   * @return true, если событие было обработано.
   */
  bool key_handling_snake(guint keyval, guint keycode, Gdk::ModifierType state);

  bool key_release_handling_snake(guint keyval, guint keycode,
                                  Gdk::ModifierType state);

  /**
   * @brief Обрабатывает события клавиш для игры Tetris.
   *
   * @param keyval Значение клавиши.
   * @param keycode Код клавиши.
   * @param state Состояние модификаторов.
   * @return true, если событие было обработано.
   */
  bool key_handling_tetris(guint keyval, guint keycode,
                           Gdk::ModifierType state);

  /**
   * @brief Обработчик таймаута для игры Snake.
   *
   * @return true для продолжения таймаута.
   */
  bool on_timeout_snake();

  /**
   * @brief Обработчик таймаута для игры Tetris.
   *
   * @return true для продолжения таймаута.
   */
  bool on_timeout_tetris();

  /**
   * @brief Получает цвет, связанный со значением ячейки.
   *
   * @param cell_value Значение ячейки.
   * @return Цвет в виде строки.
   */
  std::string get_cell_color(int cell_value);

  /**
   * @brief Устанавливает цвет конкретной ячейки на игровом поле.
   *
   * @param row Индекс строки.
   * @param col Индекс столбца.
   * @param color Цвет для установки.
   */
  void set_cell_color(int row, int col, const std::string &color);

  /**
   * @brief Устанавливает цвет конкретной ячейки в превью следующей фигуры.
   *
   * @param row Индекс строки.
   * @param col Индекс столбца.
   * @param color Цвет для установки.
   */
  void set_next_figure_cell_color(int row, int col, const std::string &color);

  /**
   * @brief Инициализирует компоненты интерфейса главного меню.
   */
  void initialize_menu();

  /**
   * @brief Callback для запуска игры Snake.
   */
  void on_button_start_snake();

  /**
   * @brief Callback для запуска игры Tetris.
   */
  void on_button_start_tetris();

  /**
   * @brief Отображает игровой интерфейс после выбора.
   */
  void show_game_ui();

  /**
   * @brief Инициализирует компоненты игрового интерфейса.
   */
  void initialize_game_ui();

  /**
   * @brief Инициализирует сетку игрового поля.
   */
  void initialize_game_field_grid();

  /**
   * @brief Инициализирует сетку превью следующей фигуры.
   */
  void initialize_next_figure_grid();

  /**
   * @brief Инициализирует все метки, используемые в интерфейсе.
   */
  void initialize_labels();

  /**
   * @brief Инициализирует все сеточные макеты, используемые в интерфейсе.
   */
  void initialize_grids();

  /**
   * @brief Инициализирует контейнерные виджеты.
   */
  void initialize_containers();

  /**
   * @brief Применяет CSS-стилизацию к компонентам интерфейса.
   */
  void apply_css();

  /**
   * @brief Настраивает игровой контроллер.
   */
  void set_controller();

 private:
  Gtk::Box menu_box_{Gtk::Orientation::VERTICAL,
                     10};                       /**< Контейнер Box для меню */
  Gtk::Label menu_label_;                       /**< Метка для меню */
  Gtk::Button button_start_snake_{"S N A K E"}; /**< Кнопка для запуска Snake */
  Gtk::Button button_start_tetris_{
      "T E T R I S"}; /**< Кнопка для запуска Tetris */

  Gtk::Box main_box_{
      Gtk::Orientation::VERTICAL}; /**< Основной вертикальный контейнер Box */
  Gtk::Box game_info_box_{
      Gtk::Orientation::HORIZONTAL}; /**< Горизонтальный контейнер Box для
                                        информации об игре */
  Gtk::Box info_box_{
      Gtk::Orientation::VERTICAL}; /**< Вертикальный контейнер Box для
                                      дополнительной информации */

  Gtk::Grid game_field_grid_;  /**< Сетка для игрового поля */
  Gtk::Grid next_figure_grid_; /**< Сетка для превью следующей фигуры */
  Gtk::Grid info_grid_;        /**< Сетка для отображения информации об игре */

  Gtk::Label game_name_label_;     /**< Метка отображающая название игры */
  Gtk::Label pause_message_label_; /**< Метка отображающая сообщения о паузе */
  Gtk::Label win_label_;           /**< Метка, указывающая на победу */
  Gtk::Label game_over_label_;     /**< Метка, указывающая на конец игры */
  Gtk::Label level_label_;         /**< Метка отображающая текущий уровень */
  Gtk::Label score_label_;         /**< Метка отображающая текущий счёт */
  Gtk::Label max_score_label_;     /**< Метка отображающая максимальный счёт */
  Gtk::Label move_left_label_;     /**< Метка для инструкций движения влево */
  Gtk::Label move_right_label_;    /**< Метка для инструкций движения вправо */
  Gtk::Label move_down_label_;     /**< Метка для инструкций движения вниз */
  Gtk::Label move_up_label_;       /**< Метка для инструкций движения вверх */
  Gtk::Label action_label_;        /**< Метка для инструкций действий */
  Gtk::Label pause_label_;         /**< Метка для инструкций паузы */
  Gtk::Label exit_label_;          /**< Метка для инструкций выхода */

  Gtk::Overlay overlay_; /**< Контейнер Overlay для наложения виджетов */

  Glib::RefPtr<Gtk::EventControllerKey>
      key_controller_; /**< Контроллер событий клавиш */

  std::vector<std::vector<Gtk::Widget *>>
      game_field_cells_; /**< 2D вектор ячеек игрового поля */
  std::vector<std::vector<Gtk::Widget *>>
      next_figure_cells_; /**< 2D вектор ячеек для превью следующей фигуры */

  sigc::connection
      connection_start_snake_; /**< Соединение сигнала для запуска Snake */
  sigc::connection
      connection_start_tetris_; /**< Соединение сигнала для запуска Tetris */
  sigc::connection
      connection_key_controller_; /**< Соединение сигнала для событий клавиш */
  sigc::connection timeout_connection_;
  sigc::connection connection_key_pressed_;
  sigc::connection connection_key_released_;

  int level_;     /**< Текущий уровень игры */
  int score_;     /**< Текущий счёт */
  int max_score_; /**< Максимально достигнутый счёт */

  Game game_;            /**< Текущая игра */
  GameInfo_t game_info_; /**< Структура, содержащая информацию об игре */
  std::unique_ptr<GameModel> game_model_; /**< Указатель на модель игры */
  std::unique_ptr<GameController>
      game_controller_; /**< Указатель на контроллер игры */

  GameInfo_t *tetris_game_info_ =
      nullptr; /**< Указатель на информацию о игре Tetris */

  /**
   * @brief Инициализирует настройки, специфичные для игры Tetris.
   */
  void initialize_tetris_game();

  /**
   * @brief Освобождает ресурсы, выделенные для игры Tetris.
   */
  void free_tetris_game();
};
}  // namespace s21

#endif