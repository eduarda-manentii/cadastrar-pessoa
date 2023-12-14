#include "listagem_window.h"

#include <gtkmm.h>

#include "models/pessoa.h"

ListagemWindow::ListagemWindow()
    : listStore(Gtk::ListStore::create(modelColumns)),
      button_excluir("Excluir"),
      button_editar("Editar") {
    set_title("Listagem");
    set_border_width(10);
    set_modal(true);
    set_default_size(450, 200);

    treeview.set_model(listStore);

    treeview.append_column("Nome", modelColumns.col_nome);
    auto col = treeview.get_column(0);
    col->set_resizable();
    col->set_fixed_width(150);

    treeview.append_column("Idade", modelColumns.col_idade);
    treeview.get_column(1)->set_resizable();

    cadastro_window.signal_people_changed().connect([this](int n) {
        preencherTabela();
    });

    preencherTabela();

    Gtk::ScrolledWindow* scrolledWindow =
        Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolledWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    box.pack_start(*scrolledWindow, Gtk::PACK_EXPAND_WIDGET);

    scrolledWindow->add(treeview);

    button_excluir.signal_clicked().connect(
        sigc::mem_fun(*this, &ListagemWindow::on_button_excluir_clicked));

    button_editar.signal_clicked().connect(
        sigc::mem_fun(*this, &ListagemWindow::on_button_editar_clicked));

    buttons_box.add(button_editar);
    buttons_box.add(button_excluir);
    box.pack_end(buttons_box, Gtk::PACK_SHRINK);
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
    Pessoa::all().each([&](const Pessoa& p) {
        Gtk::TreeModel::Row row = *(listStore->append());
        row[modelColumns.col_pessoa] = p;
        row[modelColumns.col_nome] = p.nome;
        row[modelColumns.col_idade] = p.idade;
    });
}

void ListagemWindow::on_button_excluir_clicked() {
    auto iter = treeview.get_selection()->get_selected();

    if (iter) {
        Pessoa pessoa = (*iter)[modelColumns.col_pessoa];

        Gtk::MessageDialog confirmDialog(
            *this, "Tem certeza que deseja excluir?", false,
            Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);

        int result = confirmDialog.run();

        if (result == Gtk::RESPONSE_YES) {
            pessoa.delete_();
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
        Pessoa pessoa = (*iter)[modelColumns.col_pessoa];
        cadastro_window.setModoEdicao(pessoa);
        cadastro_window.show();

    } else {
        Gtk::MessageDialog errorMsg(*this, "Selecione uma linha para edição.");
        errorMsg.run();
    }
}
