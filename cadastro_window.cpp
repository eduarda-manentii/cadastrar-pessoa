#include "cadastro_window.h"

#include <iostream>
#include <string>

CadastroWindow::CadastroWindow()
    : nome_box("Nome"), idade_box("Idade"), button_salvar("Salvar") {
    set_title("Cadastro");
    set_border_width(10);
    set_modal(true);

    idade_box.entry.set_max_length(3);

    button_salvar.signal_clicked().connect(
        sigc::mem_fun(*this, &CadastroWindow::on_button_salvar_clicked));

    box.add(nome_box);
    box.add(idade_box);
    box.add(button_salvar);

    add(box);
    show_all_children();
}

CadastroWindow::~CadastroWindow() {
}

CadastroWindow::signal_type CadastroWindow::signal_people_changed() {
    return _signal_people_changed;
}

void CadastroWindow::on_button_salvar_clicked() {
    Pessoa pessoa;

    if (this->pessoa) {
        pessoa = *this->pessoa;
    }
    pessoa.nome = nome_box.entry.get_text();

    if (pessoa.nome.empty()) {
        Gtk::MessageDialog msg(*this, "O campo de nome não pode estar vazio.");
        msg.run();
        return;
    }

    try {
        pessoa.idade = std::stoi(idade_box.entry.get_text());
        if (this->pessoa) {
            pessoa.save();
            Gtk::MessageDialog msgUpdate(*this, "Pessoa alterada com sucesso!");
            msgUpdate.run();
            hide();

        } else {
            pessoa.save();
            Gtk::MessageDialog msgInsert(*this,
                                         "Pessoa cadastrada com sucesso!");
            msgInsert.run();
        }
        signal_people_changed().emit(5);
        nome_box.entry.set_text("");
        idade_box.entry.set_text("");
    } catch (...) {
        Gtk::MessageDialog ErrorMsg(*this, "A idade é inválida");
        ErrorMsg.run();
    }
}

void CadastroWindow::setModoEdicao(const std::optional<Pessoa>& pessoa) {
    if (pessoa) {
        this->pessoa = pessoa;
        nome_box.entry.set_text(pessoa->nome);
        idade_box.entry.set_text(std::to_string(pessoa->idade));
    } else {
        this->pessoa = {};
        nome_box.entry.set_text("");
        idade_box.entry.set_text("");
    }
}
