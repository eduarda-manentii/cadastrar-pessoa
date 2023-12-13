#include <gtkmm.h>

#include <iostream>

#include "my_window.h"
#include "pessoas_storage.h"

void popula_pessoas(PessoasStorage& storage) {
    Pessoa p1;
    p1.nome = "Eduarda";
    p1.idade = 19;
    storage.insere(p1);

    Pessoa p2;
    p2.nome = "Philipe";
    p2.idade = 37;
    storage.insere(p2);
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

    PessoasStorage storage;
    // popula_pessoas(storage);
    MyWindow window(storage);

    return app->run(window);
}

int testes() {
    PessoasStorage ps;

    Pessoa p1;
    p1.nome = "Eduarda";
    p1.idade = 19;
    ps.insere(p1);

    Pessoa p2;
    p2.nome = "Philipe";
    p2.idade = 37;
    ps.insere(p2);

    auto print = [&](const Pessoa& p) {
        std::cout << '[' << p.id << "] = " << p.nome << ", " << p.idade << "; "
                  << std::endl;
    };

    Pessoa p3 = ps.busca(2);
    std::cout << p3.nome << std::endl;

    auto pp = ps.lista();
    for (Pessoa& p : pp) {
        print(p);
    }

    std::cout << "Para cada pessoa..." << std::endl;
    ps.each(print);

    std::cout << "apagando pessoa..." << std::endl;
    Pessoa apagada = ps.apaga(2);
    print(apagada);

    std::cout << "Para cada pessoa..." << std::endl;
    ps.each(print);
    return 0;
}
