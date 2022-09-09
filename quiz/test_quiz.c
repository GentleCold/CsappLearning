//
// Created by GentleCold on 2022/9/7.
//

#include "test.h"


void test_all() {
}

int main() {
    test_all();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}