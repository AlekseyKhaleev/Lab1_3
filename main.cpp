#include <iostream>
#include "func.h"

const std::size_t step = 64;

int main() {
    std::string txt;
    std::string n_txt;
    int start = next_rand(5);
    std::cin>>txt;
    for(auto &i:txt){
        n_txt += i^ next_rand();
    }
    std::cout << n_txt << std::endl;
    start = next_rand(5);
    std::string a_txt;
    for(auto &i:n_txt){
        a_txt += i^ next_rand();
    }
    std::cout << a_txt << std::endl;
}
