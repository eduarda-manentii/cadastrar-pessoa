#include "cadastro_window.h"

#include <iostream>
#include <string>

#include "pessoa.h"
#include "pessoas_storage.h"

CadastroWindow::CadastroWindow(PessoasStorage& storage)
    : storage(storage),
      modoEdicao(false),
      pessoaId(0),
      nome_box("Nome"),
      idade_box("Idade"),
      button_salvar("Salvar") {
    set_title("Cadastro");
    set_border_width(10);
    set_modal(true);

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

void CadastroWindow::on_button_salvar_clicked() {
    Pessoa pessoa;
    pessoa.nome = nome_box.entry.get_text();
    std::string nome = pessoa.nome;

    if (nome.empty()) {
        Gtk::MessageDialog msg(*this, "O campo de nome não pode estar vazio.");
        msg.run();
        return;
    }

    try {
        pessoa.idade = std::stoi(idade_box.entry.get_text());
        if (!modoEdicao) {
            storage.insere(pessoa);

            Gtk::MessageDialog msgInsert(*this,
                                         "Pessoa cadastrada com sucesso!");
            msgInsert.run();
        } else {
            pessoa.id = pessoaId;
            storage.atualiza(pessoa);
            Gtk::MessageDialog msgUpdate(*this, "Pessoa alterada com sucesso!");
            msgUpdate.run();
            hide();
        }
        nome_box.entry.set_text("");
        idade_box.entry.set_text("");
    } catch (...) {
        Gtk::MessageDialog ErrorMsg(*this, "A idade é inválida");
        ErrorMsg.run();
    }
}

void CadastroWindow::setModoEdicao(bool modoEdicao, int pessoaId) {
    this->modoEdicao = modoEdicao;
    this->pessoaId = pessoaId;

    if (modoEdicao) {
        const Pessoa& pessoaParaEditar = storage.busca(pessoaId);
        nome_box.entry.set_text(pessoaParaEditar.nome);
        idade_box.entry.set_text(std::to_string(pessoaParaEditar.idade));
    } else {
        nome_box.entry.set_text("");
        idade_box.entry.set_text("");
    }
}
