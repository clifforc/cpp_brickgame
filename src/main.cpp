#include "./gui/desktop/inc/game_view.h"

/**
 * @brief Main function of the tetris game.
 *
 * This function initializes the game window, sets up the colors
 * starts the game loop, and cleans up the window before exiting.
 *
 * @return 0 indicating successful execution of the program.
 */

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("com.example.BrickGame");

  app->signal_activate().connect([&app]() {
    auto window = new s21::GameWindow();
    window->set_application(app);
    window->present();
  });

  return app->run(argc, argv);
}
