#include "../include/finst.hpp"

#include <iostream>

struct thing
{

    thing() = default;

    enum class States {
        ONE,
        TWO
    } states = States::ONE;

    finst::State<decltype( [](int x, int y){ return x + y; })> holder;

    finst::State_machine<decltype(states), decltype(holder)> machine;
};

int main() {

    thing mine;

    mine.machine.set_state(thing::States::ONE);

    switch(static_cast<thing::States>(mine.machine.get_state())) {
        case thing::States::ONE : 
        std::cout << "ONE\n";
        break;
        case thing::States::TWO : 
        std::cout << "TWO\n";
        break;
        default :
        std::cout << "Whatever\n";
        break;
    }

    return 0;
}