#include "pessoas_storage.h"

void PessoasStorage::insere(Pessoa& pessoa) {
    int id = seq++;
    pessoa.id = id;
    pessoas[id] = pessoa;
}

Pessoa PessoasStorage::busca(int id) {
    return pessoas[id];
}

std::vector<Pessoa> PessoasStorage::lista() {
    std::vector<Pessoa> pessoas;
    for (const auto& [key, value] : this->pessoas) {
        pessoas.push_back(value);
    }
    return pessoas;
}

Pessoa PessoasStorage::apaga(int id) {
    auto it = pessoas.find(id);
    Pessoa p = it->second;
    pessoas.erase(it);
    return p;
}

void PessoasStorage::atualiza(Pessoa& pessoa) {
    pessoas[pessoa.id] = pessoa;
}
