#include "my_window.h"

MyWindow::MyWindow()
    : button_cadastro("Cadastro"), button_listagem("Listagem") {
    set_title("Janela Principal");
    set_border_width(10);
    set_modal();

    button_cadastro.signal_clicked().connect(
        sigc::mem_fun(*this, &MyWindow::on_button_cadastro_clicked));
    button_listagem.signal_clicked().connect(
        sigc::mem_fun(*this, &MyWindow::on_button_listagem_clicked));

    box.add(button_cadastro);
    box.add(button_listagem);

    add(box);
    show_all_children();
}

void MyWindow::on_button_cadastro_clicked() {
    cadastro_window.setModoEdicao();
    cadastro_window.show();
}

void MyWindow::on_button_listagem_clicked() {
    listagem_window.mostra();
}
