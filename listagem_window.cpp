#include "listagem_window.h"

#include <gtkmm.h>

ListagemWindow::ListagemWindow(PessoasStorage& storage)
    : listStore(Gtk::ListStore::create(modelColumns)),
      button_excluir("Excluir"),
      button_editar("Editar"),
      storage(storage),
      cadastro_window(storage) {
    set_title("Listagem");
    set_border_width(10);
    set_modal(true);

    treeview.set_model(listStore);

    treeview.append_column("Nome", modelColumns.col_nome);
    treeview.append_column("Idade", modelColumns.col_idade);

    preencherTabela();

    box.pack_start(treeview, Gtk::PACK_EXPAND_WIDGET);

    button_excluir.signal_clicked().connect(
        sigc::mem_fun(*this, &ListagemWindow::on_button_excluir_clicked));

    button_editar.signal_clicked().connect(
        sigc::mem_fun(*this, &ListagemWindow::on_button_editar_clicked));

    box.add(button_excluir);
    box.add(button_editar);
    add(box);
    show_all_children();
}

ListagemWindow::~ListagemWindow() {
}

void ListagemWindow::mostra() {
    preencherTabela();
    show();
}

void ListagemWindow::preencherTabela() {
    listStore->clear();
    storage.each([&](const Pessoa& p) {
        Gtk::TreeModel::Row row = *(listStore->append());
        row[modelColumns.col_id] = p.id;
        row[modelColumns.col_nome] = p.nome;
        row[modelColumns.col_idade] = p.idade;
    });
}

void ListagemWindow::on_button_excluir_clicked() {
    auto iter = treeview.get_selection()->get_selected();

    if (iter) {
        int id = (*iter)[modelColumns.col_id];
        Gtk::MessageDialog confirmDialog(*this,
                                         "Tem certeza que deseja excluir?");
        confirmDialog.add_button("Cancelar", Gtk::RESPONSE_CANCEL);

        int result = confirmDialog.run();

        if (result == Gtk::RESPONSE_OK) {
            storage.apaga(id);

            Gtk::MessageDialog successMsg(*this, "Removida com sucesso!");
            successMsg.run();

            preencherTabela();
        }
    } else {
        Gtk::MessageDialog errorMsg(*this, "Selecione uma linha para remoção.");
        errorMsg.run();
    }
}

void ListagemWindow::on_button_editar_clicked() {
    auto iter = treeview.get_selection()->get_selected();

    if (iter) {
        int id = (*iter)[modelColumns.col_id];
        cadastro_window.setModoEdicao(true, id);
        cadastro_window.show();
        cadastro_window.signal_hide().connect([this]() {
            preencherTabela();
        });
    } else {
        Gtk::MessageDialog errorMsg(*this, "Selecione uma linha para edição.");
        errorMsg.run();
    }
}
