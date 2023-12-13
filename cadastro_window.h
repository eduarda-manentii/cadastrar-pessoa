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

   protected:
    void on_button_salvar_clicked();

    PessoasStorage& storage;
    bool modoEdicao;
    int pessoaId;

    Gtk::VBox box;
    EntryBox nome_box;
    EntryBox idade_box;
    Gtk::Button button_salvar;
};

#endif  // CADASTRO_WINDOW_H
