#ifndef CADASTRO_WINDOW_H
#define CADASTRO_WINDOW_H

#include <gtkmm.h>

#include "entry_box.h"
#include "pessoas_storage.h"

class CadastroWindow : public Gtk::Window {
   public:
    CadastroWindow(PessoasStorage& storage);
    void setModoEdicao(bool modoEdicao, int pessoaId);

    virtual ~CadastroWindow();

    using signal_type = sigc::signal<void(int)>;
    signal_type signal_people_changed();

   protected:
    void on_button_salvar_clicked();

    PessoasStorage& storage;
    bool modoEdicao;
    int pessoaId;

    Gtk::VBox box;
    EntryBox nome_box;
    EntryBox idade_box;
    Gtk::Button button_salvar;
    signal_type _signal_people_changed;
};

#endif  // CADASTRO_WINDOW_H
