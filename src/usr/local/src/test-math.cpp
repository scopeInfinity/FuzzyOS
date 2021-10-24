#include <math.h>
#include <stdio.h>
#include <testing.h>

double EPSILON = 1e-9;

double check_double_equal(double x, double y) {
    return std::fabs(x - y) < EPSILON;
}

double to_radian(double x) { return x * (M_PI / 180); }

void test_isnan() {
    ASSERT_TRUE(std::isnan(16.0) == 0);
    ASSERT_TRUE(std::isnan(-16.0) == 0);
    ASSERT_TRUE(std::isnan(0.0 / 0.0) != 0);
    ASSERT_TRUE(std::isnan(1.0 / 0.0) == 0);
}

void test_floor() {
    ASSERT_TRUE(check_double_equal(std::floor(16.0), 16.0));
    ASSERT_TRUE(check_double_equal(std::floor(16.6), 16.0));
    ASSERT_TRUE(check_double_equal(std::floor(-16.0), -16.0));
    ASSERT_TRUE(check_double_equal(std::floor(-16.9), -17));
}

void test_round() {
    ASSERT_TRUE(check_double_equal(std::round(16.3), 16.0));
    ASSERT_TRUE(check_double_equal(std::round(16.6), 17.0));
    ASSERT_TRUE(check_double_equal(std::round(-16.0), -16.0));
    ASSERT_TRUE(check_double_equal(std::round(-16.9), -17));
}

void test_fmod() {
    ASSERT_TRUE(check_double_equal(std::fmod(1.6, 1.2), 0.4));
    ASSERT_TRUE(check_double_equal(std::fmod(7.0, 5.0), 2.0));
}

void test_fabs() {
    ASSERT_TRUE(check_double_equal(std::fabs(1.6), 1.6));
    ASSERT_TRUE(check_double_equal(std::fabs(-7.7), 7.7));
    ASSERT_TRUE(check_double_equal(std::fabs(0.0), 0.0));
}

void test_sin() {
    ASSERT_TRUE(check_double_equal(std::sin(to_radian(45.0)), 0.7071067812));
    ASSERT_TRUE(check_double_equal(std::sin(to_radian(90.0)), 1.0));
}

void test_cos() {
    ASSERT_TRUE(check_double_equal(std::cos(to_radian(60.0)), 0.5));
    ASSERT_TRUE(check_double_equal(std::cos(to_radian(90.0)), 0.0));
}

int main(int argc, char *argv[]) {
    TEST_INIT();

    RUN_TEST(test_isnan);
    RUN_TEST(test_floor);
    RUN_TEST(test_round);
    RUN_TEST(test_fmod);
    RUN_TEST(test_fabs);
    RUN_TEST(test_sin);
    RUN_TEST(test_cos);

    TEST_SUMMARY();
    return 0;
}