#ifndef CADASTRO_WINDOW_H
#define CADASTRO_WINDOW_H

#include <gtkmm.h>

#include <optional>

#include "entry_box.h"
#include "models/pessoa.h"

class CadastroWindow : public Gtk::Window {
   public:
    CadastroWindow();
    void setModoEdicao(const std::optional<Pessoa>& pessoa = {});

    virtual ~CadastroWindow();

    using signal_type = sigc::signal<void(int)>;
    signal_type signal_people_changed();

   protected:
    void on_button_salvar_clicked();

    std::optional<Pessoa> pessoa;

    Gtk::VBox box;
    EntryBox nome_box;
    EntryBox idade_box;
    Gtk::Button button_salvar;
    signal_type _signal_people_changed;
};

#endif  // CADASTRO_WINDOW_H
