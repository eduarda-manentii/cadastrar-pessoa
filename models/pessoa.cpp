#include "pessoa.h"

#include <iostream>

namespace persistent {

template <>
void Table<Pessoa>::schema() {
    table("pessoas");
    pk("id", &Pessoa::id);
    column("name", &Pessoa::nome);
    column("idade", &Pessoa::idade);
    column("sexo", &Pessoa::sexo);
}

}  // namespace persistent

void Pessoa::before_save() {
    std::cout << "salvando: " << to_string() << std::endl;
}
