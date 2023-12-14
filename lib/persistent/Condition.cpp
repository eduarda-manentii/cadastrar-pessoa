#include "Condition.h"

namespace persistent{

    Conditions order_by(const std::string& sql){
        Conditions cond;
        return cond.order(sql);
    }

    Conditions all(){
        return Conditions();
    }

}
