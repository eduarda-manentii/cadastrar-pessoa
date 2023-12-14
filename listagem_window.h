#ifndef LISTAGEM_WINDOW_H
#define LISTAGEM_WINDOW_H

#include <gtkmm.h>

#include "cadastro_window.h"

class ListagemWindow : public Gtk::Window {
   public:
    ListagemWindow();
    virtual ~ListagemWindow();

    void mostra();

   protected:
    void on_button_excluir_clicked();
    void on_button_editar_clicked();

    class ModelColumns : public Gtk::TreeModel::ColumnRecord {
       public:
        ModelColumns() {
            add(col_pessoa);
            add(col_nome);
            add(col_idade);
        }
        Gtk::TreeModelColumn<Pessoa> col_pessoa;
        Gtk::TreeModelColumn<Glib::ustring> col_nome;
        Gtk::TreeModelColumn<int> col_idade;
    };

    ModelColumns modelColumns;

    Gtk::VBox box;
    Gtk::HBox buttons_box;
    Gtk::TreeView treeview;
    Glib::RefPtr<Gtk::ListStore> listStore;
    Gtk::Button button_excluir;
    Gtk::Button button_editar;

    CadastroWindow cadastro_window;

    void preencherTabela();
};

#endif  // LISTAGEM_WINDOW_H
