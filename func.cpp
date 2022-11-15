#include "func.h"

int next_rand(int x){
    static int z{};
    int answer{};
    if (x) {
        answer = (421 * x + 1663) % 7875;
    }else
        answer = (421 * z + 1663) % 7875;
    z = answer;
    return answer;
}

