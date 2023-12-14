#include "cadastro_window.h"

#include <iostream>
#include <optional>
#include <string>

CadastroWindow::CadastroWindow()
    : nome_box("Nome"), idade_box("Idade"), button_salvar("Salvar") {
    set_title("Cadastro");
    set_border_width(10);
    set_modal(true);

    idade_box.entry.set_max_length(3);
    sexo_combobox.append("Feminino");
    sexo_combobox.append("Masculino");

    button_salvar.signal_clicked().connect(
        sigc::mem_fun(*this, &CadastroWindow::on_button_salvar_clicked));

    box.add(nome_box);
    box.add(idade_box);
    box.add(sexo_combobox);
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
    pessoa.sexo = sexo_combobox.get_active_text();

    if (pessoa.nome.empty() || pessoa.sexo.empty()) {
        Gtk::MessageDialog msg(*this, "Todos os campos devem ser preenchidos.");
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
        sexo_combobox.set_active(-1);
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
        sexo_combobox.set_active_text(pessoa->sexo);
    } else {
        this->pessoa = {};
        nome_box.entry.set_text("");
        idade_box.entry.set_text("");
    }
}
