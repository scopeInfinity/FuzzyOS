#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <testing.h>

void test_min() {
    ASSERT_TRUE(std::min(3, 6) == 3);
    ASSERT_TRUE(std::min(6, 4) == 4);
    ASSERT_TRUE(std::min(300, 6) == 6);
}

void test_max() {
    ASSERT_TRUE(std::max(3, 6) == 6);
    ASSERT_TRUE(std::max(8, 4) == 8);
    ASSERT_TRUE(std::max(3, 600) == 600);
}

void test_abs() {
    ASSERT_TRUE(std::abs(0) == 0);
    ASSERT_TRUE(std::abs(3) == 3);
    ASSERT_TRUE(std::abs(-3) == 3);
}

void test_atoi() { ASSERT_TRUE(std::atoi("36") == 36); }

void test_itoa() {
    char buffer[20];
    std::itoa(54325, buffer, 2);
    ASSERT_TRUE(std::strcmp(buffer, "1101010000110101") == 0);
    std::itoa(54325, buffer, 10);
    ASSERT_TRUE(std::strcmp(buffer, "54325") == 0);
    std::itoa(54325, buffer, 16);
    ASSERT_TRUE(std::strcmp(buffer, "D435") == 0);
}

void test_ftoa() {
    char buffer[20];
    std::ftoa(1.555, buffer, 1);
    ASSERT_TRUE(std::strcmp(buffer, "0.1e+1") == 0);
    std::ftoa(1.555, buffer, 2);
    ASSERT_TRUE(std::strcmp(buffer, "0.15e+1") == 0);
    std::ftoa(-1.555, buffer, 2);
    ASSERT_TRUE(std::strcmp(buffer, "-0.15e+1") == 0);
    std::ftoa(0.01555, buffer, 2);
    ASSERT_TRUE(std::strcmp(buffer, "0.15e-1") == 0);
    std::ftoa(0.1555, buffer, 2);
    ASSERT_TRUE(std::strcmp(buffer, "0.15") == 0);
}

int main(int argc, char *argv[]) {
    TEST_INIT();

    RUN_TEST(test_min);
    RUN_TEST(test_max);
    RUN_TEST(test_abs);
    RUN_TEST(test_atoi);
    RUN_TEST(test_itoa);
    RUN_TEST(test_ftoa);

    TEST_SUMMARY();
    return 0;
}