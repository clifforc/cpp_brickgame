#include "inc/game_view.h"

namespace s21 {

GameWindow::GameWindow()
    : button_start_snake_("S N A K E"), button_start_tetris_("T E T R I S") {
  set_title("BrickGame");
  set_default_size(500, 400);

  initialize_menu();
  initialize_game_ui();
  main_box_.set_visible(false);

  apply_css();
  set_child(menu_box_);
}

void GameWindow::initialize_menu() {
  menu_label_.set_text("Select Game:");
  menu_label_.get_style_context()->add_class("menu-label");
  menu_label_.set_halign(Gtk::Align::CENTER);
  menu_label_.set_margin_bottom(20);

  connection_start_snake_ = button_start_snake_.signal_clicked().connect(
      sigc::mem_fun(*this, &GameWindow::on_button_start_snake));
  connection_start_tetris_ = button_start_tetris_.signal_clicked().connect(
      sigc::mem_fun(*this, &GameWindow::on_button_start_tetris));

  button_start_snake_.get_style_context()->add_class("menu-button");
  button_start_tetris_.get_style_context()->add_class("menu-button");

  menu_box_.set_orientation(Gtk::Orientation::VERTICAL);
  menu_box_.set_spacing(10);
  menu_box_.set_margin_top(100);
  menu_box_.set_halign(Gtk::Align::CENTER);
  menu_box_.set_valign(Gtk::Align::CENTER);

  menu_box_.append(menu_label_);
  menu_box_.append(button_start_snake_);
  menu_box_.append(button_start_tetris_);
}

void GameWindow::on_button_start_snake() {
  game_ = Game::snake;
  menu_box_.set_visible(false);
  show_game_ui();
}

void GameWindow::on_button_start_tetris() {
  game_ = Game::tetris;
  menu_box_.set_visible(false);
  show_game_ui();
}

void GameWindow::show_game_ui() {
  if (game_ == Game::snake) {
    set_title("Snake Game");
    game_name_label_.set_text("S  N  A  K  E");
    set_controller();
  } else if (game_ == Game::tetris) {
    set_title("Tetris Game");
    game_name_label_.set_text("T  E  T  R  I  S");
    initialize_tetris_game();
    set_controller();
  }

  main_box_.set_visible(true);

  if (game_ == Game::snake) {
    game_model_ = std::make_unique<GameModel>();
    game_controller_ = std::make_unique<GameController>(game_model_.get());
  }

  set_child(overlay_);
}

void GameWindow::initialize_game_ui() {
  initialize_labels();
  initialize_grids();

  initialize_game_field_grid();
  initialize_next_figure_grid();

  initialize_containers();
}

std::string GameWindow::get_cell_color(int cell_value) {
  std::string color;
  switch (cell_value) {
  case ORANGE_COLOR:
    color = HEX_ORANGE;
    break;
  case RED_COLOR:
    color = HEX_RED;
    break;
  case GREEN_COLOR:
    color = HEX_GREEN;
    break;
  case YELLOW_COLOR:
    color = HEX_YELLOW;
    break;
  case BLUE_COLOR:
    color = HEX_BLUE;
    break;
  case MAGENTA_COLOR:
    color = HEX_MAGENTA;
    break;
  case CYAN_COLOR:
    color = HEX_CYAN;
    break;
  default:
    color = HEX_WHITE;
    break;
  }
  return color;
}

void GameWindow::set_cell_color(int row, int col, const std::string &color) {
  auto css_provider = Gtk::CssProvider::create();
  std::string css =
      "box { background-color: " + color + "; border: 1px solid #FFFFFF; }";
  css_provider->load_from_data(css);
  game_field_cells_[row][col]->get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void GameWindow::set_next_figure_cell_color(int row, int col,
                                            const std::string &color) {
  auto css_provider = Gtk::CssProvider::create();
  std::string css =
      "box { background-color: " + color + "; border: 1px solid #FFFFFF; }";
  css_provider->load_from_data(css);
  next_figure_cells_[row][col]->get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void GameWindow::initialize_game_field_grid() {
  game_field_cells_.resize(FIELD_HEIGHT,
                           std::vector<Gtk::Widget *>(FIELD_WIDTH, nullptr));

  for (int row = 0; row < FIELD_HEIGHT; ++row) {
    for (int col = 0; col < FIELD_WIDTH; ++col) {
      auto cell = Gtk::make_managed<Gtk::Box>();
      cell->set_size_request(20, 20);

      game_field_cells_[row][col] = cell;

      auto cell_css_provider = Gtk::CssProvider::create();
      cell_css_provider->load_from_data(
          "box { background-color: #FFFFFF; border: 1px solid #FFFFFF; }");
      cell->get_style_context()->add_provider(cell_css_provider,
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

      game_field_grid_.attach(*cell, col, row, 1, 1);
    }
  }
}

void GameWindow::initialize_next_figure_grid() {
  next_figure_cells_.resize(FIGURE_SIZE,
                            std::vector<Gtk::Widget *>(FIGURE_SIZE, nullptr));

  for (int row = 0; row < FIGURE_SIZE; ++row) {
    for (int col = 0; col < FIGURE_SIZE; ++col) {
      auto cell = Gtk::make_managed<Gtk::Box>();
      cell->set_size_request(20, 20);

      next_figure_cells_[row][col] = cell;

      auto cell_css_provider = Gtk::CssProvider::create();
      cell_css_provider->load_from_data(
          "box { background-color: #FFFFFF; border: 1px solid #FFFFFF; }");
      cell->get_style_context()->add_provider(cell_css_provider,
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

      next_figure_grid_.attach(*cell, col, row, 1, 1);
    }
  }
}

void GameWindow::initialize_labels() {
  score_label_.set_text("LEVEL: 0");
  level_label_.set_text("SCORE: 0");
  max_score_label_.set_text("MAX SCORE: 0");
  move_left_label_.set_text("LEFT ARROW         : MOVE LEFT");
  move_right_label_.set_text("RIGHT ARROW      : MOVE RIGHT");
  move_down_label_.set_text("DOWN ARROW      : MOVE DOWN");
  move_up_label_.set_text("UP ARROW             : MOVE UP/ROTATE");
  action_label_.set_text("X : ACTION");
  pause_label_.set_text("P : PAUSE");
  exit_label_.set_text("Q : EXIT");

  score_label_.get_style_context()->add_class("info-label");
  level_label_.get_style_context()->add_class("info-label");
  max_score_label_.get_style_context()->add_class("info-label");
  move_left_label_.get_style_context()->add_class("instruction-label");
  move_right_label_.get_style_context()->add_class("instruction-label");
  move_down_label_.get_style_context()->add_class("instruction-label");
  move_up_label_.get_style_context()->add_class("instruction-label");
  action_label_.get_style_context()->add_class("instruction-label");
  pause_label_.get_style_context()->add_class("instruction-label");
  exit_label_.get_style_context()->add_class("instruction-label");

  score_label_.set_halign(Gtk::Align::START);
  level_label_.set_halign(Gtk::Align::START);
  max_score_label_.set_halign(Gtk::Align::START);
  move_left_label_.set_halign(Gtk::Align::START);
  move_right_label_.set_halign(Gtk::Align::START);
  move_down_label_.set_halign(Gtk::Align::START);
  move_up_label_.set_halign(Gtk::Align::START);
  action_label_.set_halign(Gtk::Align::START);
  pause_label_.set_halign(Gtk::Align::START);
  exit_label_.set_halign(Gtk::Align::START);

  game_name_label_.set_text("S  N  A  K  E");
  game_name_label_.set_halign(Gtk::Align::CENTER);
  game_name_label_.set_margin(10);

  pause_message_label_.set_text("Press ENTER\nto Start");
  pause_message_label_.set_halign(Gtk::Align::CENTER);
  pause_message_label_.set_valign(Gtk::Align::CENTER);
  pause_message_label_.get_style_context()->add_class("overlay-message");
  pause_message_label_.set_visible(false);

  game_over_label_.set_text("GAME OVER\nPress ENTER\nto Restart");
  game_over_label_.set_halign(Gtk::Align::CENTER);
  game_over_label_.set_valign(Gtk::Align::CENTER);
  game_over_label_.get_style_context()->add_class("overlay-message");
  game_over_label_.set_visible(false);

  win_label_.set_text("YOU WIN!\nPress ENTER\nto Restart");
  win_label_.set_halign(Gtk::Align::CENTER);
  win_label_.set_valign(Gtk::Align::CENTER);
  win_label_.get_style_context()->add_class("overlay-message");
  win_label_.set_visible(true);
}
void GameWindow::initialize_grids() {
  info_grid_.set_margin(10);
  info_grid_.set_row_spacing(10);
  info_grid_.set_column_spacing(10);

  int row = 0;
  info_grid_.attach(score_label_, 0, row++, 1, 1);
  info_grid_.attach(level_label_, 0, row++, 1, 1);
  info_grid_.attach(max_score_label_, 0, row++, 1, 1);
  info_grid_.attach(move_left_label_, 0, row++, 1, 1);
  info_grid_.attach(move_right_label_, 0, row++, 1, 1);
  info_grid_.attach(move_down_label_, 0, row++, 1, 1);
  info_grid_.attach(move_up_label_, 0, row++, 1, 1);
  info_grid_.attach(action_label_, 0, row++, 1, 1);
  info_grid_.attach(pause_label_, 0, row++, 1, 1);
  info_grid_.attach(exit_label_, 0, row++, 1, 1);

  game_field_grid_.set_margin(10);
  game_field_grid_.set_row_spacing(1);
  game_field_grid_.set_column_spacing(1);

  next_figure_grid_.set_margin(10);
  next_figure_grid_.set_row_spacing(1);
  next_figure_grid_.set_column_spacing(1);
}
void GameWindow::initialize_containers() {
  overlay_.set_child(main_box_);

  overlay_.add_overlay(pause_message_label_);
  overlay_.add_overlay(game_over_label_);
  overlay_.add_overlay(win_label_);

  set_child(overlay_);

  main_box_.set_orientation(Gtk::Orientation::VERTICAL);
  game_info_box_.set_orientation(Gtk::Orientation::HORIZONTAL);
  info_box_.set_orientation(Gtk::Orientation::VERTICAL);

  main_box_.append(game_name_label_);
  main_box_.append(game_info_box_);

  info_box_.append(next_figure_grid_);
  info_box_.append(info_grid_);

  game_info_box_.append(game_field_grid_);
  game_info_box_.append(info_box_);
}

void GameWindow::apply_css() {
  auto css_provider = Gtk::CssProvider::create();
  css_provider->load_from_data(R"(
        label { color: black; font-weight: bold; font-size: 32px;}
        .info-label {color: green; font-size: 20px;}
        .instruction-label {color: black; font-size: 18px;}
        .overlay-message {
            font-size: 32px;
            color: white;
            background-color: #333333;
            padding: 10px;
            border-radius: 5px;
        }
        .menu-label {
            font-weight: bold;
            font-size: 24px;
            color: #333333;
        }
        .menu-button {
            font-size: 18px;
            padding: 10px 20px;
            background-color: #4CAF50;
            color: black;
            border: none;
            border-radius: 5px;
        }
        .menu-button:hover {
            background-color: #45a049;
        }
    )");

  game_name_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  score_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  level_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  max_score_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  move_left_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  move_right_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  move_down_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  move_up_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  action_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  pause_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  exit_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  pause_message_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  game_over_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  win_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);

  menu_label_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  button_start_snake_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
  button_start_tetris_.get_style_context()->add_provider(
      css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void GameWindow::set_controller() {
  set_focusable(true);
  grab_focus();
  key_controller_ = Gtk::EventControllerKey::create();

  if (game_ == Game::snake) {
    connection_key_pressed_ = key_controller_->signal_key_pressed().connect(
        sigc::mem_fun(*this, &GameWindow::key_handling_snake), false);
    connection_key_released_ = key_controller_->signal_key_released().connect(
        sigc::mem_fun(*this, &GameWindow::key_release_handling_snake), false);
    add_controller(key_controller_);
    timeout_connection_ = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &GameWindow::on_timeout_snake), 50);
  }
  if (game_ == Game::tetris) {
    connection_key_controller_ = key_controller_->signal_key_pressed().connect(
        sigc::mem_fun(*this, &GameWindow::key_handling_tetris), false);
    add_controller(key_controller_);
    timeout_connection_ = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &GameWindow::on_timeout_tetris), 50);
  }
}

} // namespace s21