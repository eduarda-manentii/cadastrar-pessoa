#include <gtkmm.h>

#include <iostream>

#include "my_window.h"

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    MyWindow window;

    return app->run(window);
}
