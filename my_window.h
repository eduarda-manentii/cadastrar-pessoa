#ifndef MY_WINDOW_H
#define MY_WINDOW_H

#include <gtkmm.h>

#include "cadastro_window.h"
#include "listagem_window.h"

class MyWindow : public Gtk::Window {
   public:
    MyWindow();

   protected:
    void on_button_cadastro_clicked();
    void on_button_listagem_clicked();

    Gtk::VBox box;
    Gtk::Button button_cadastro;
    Gtk::Button button_listagem;
    ListagemWindow listagem_window;
    CadastroWindow cadastro_window;
};

#endif  // MY_WINDOW_H
