#pragma once

#include <map>
#include <vector>

#include "pessoa.h"

struct PessoasStorage {
    std::map<int, Pessoa> pessoas;
    int seq = 1;

    void insere(Pessoa& pessoa);

    Pessoa busca(int id);

    std::vector<Pessoa> lista();

    Pessoa apaga(int id);

    template <typename F>
    void each(F f) {
        for (const auto& [key, value] : this->pessoas) {
            f(value);
        }
    }

    void atualiza(Pessoa& pessoa);
};
