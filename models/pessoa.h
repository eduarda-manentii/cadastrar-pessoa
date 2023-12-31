#pragma once

#include <persistent/Model.h>
#include <persistent/datetime.h>

#include <string>

struct Pessoa : public persistent::Model<Pessoa> {
    int id;
    std::string nome;
    int idade;
    std::string sexo;

    void before_save();
};
