#include <stddef.h>
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

void test_malloc_free() {
    // we don't want to test to be dependent on the internal
    // implementation of malloc and free. So, we are ensuring
    // no overlapping memory is being allocated.

    const int test_order[] = {
        64,   // [0] allocate 64 bytes
        128,  // [1] allocate 128 bytes
        -0,   // [2] test memory and deallocate 64 bytes
        1024, // [3] allocate 1024 bytes
        -1,   // [4] test memory and deallocate 128 bytes
        100,  // [5] allocate 100 bytes, it could get allocated in freed space
        -5,   // [6] test memory and deallocate 100 bytes
        -3    // [7] test memory and deallocate 1024 bytes
    };
    std::uint8_t *memory_track[sizeof(test_order) / sizeof(test_order[0])];

    for (std::uint8_t i = 0; i < sizeof(test_order) / sizeof(test_order[0]);
         i++) {
        int op = test_order[i];
        if (op > 0) {
            // allocate memory
            memory_track[i] = (std::uint8_t *)std::malloc(op);

            // put data bytes marker... in allocated memory
            const std::uint8_t marker = i;
            std::memset(memory_track[i], marker, test_order[i]);
        } else {
            int track_id = -op;

            // test memory
            const std::uint8_t marker = track_id;
            bool is_track_memory_good = true;
            for (int j = 0; j < test_order[track_id]; j++) {
                if (memory_track[track_id][j] != marker) {
                    is_track_memory_good = false;
                    break;
                }
            }
            ASSERT_TRUE(is_track_memory_good);

            // free memory
            std::free(memory_track[track_id]);
        }
    }
}

void test_realloc() {
    // allocate memory
    std::uint8_t *data =
        (std::uint8_t *)std::realloc(NULL, sizeof(std::uint8_t) * 2);
    ASSERT_TRUE(data);
    data[0] = 100;
    data[1] = 101;

    // upsize memory
    data = (std::uint8_t *)std::realloc(data, sizeof(std::uint8_t) * 4);
    ASSERT_TRUE(data);
    ASSERT_EQ(data[0], 100);
    ASSERT_EQ(data[1], 101);
    data[2] = 102;
    data[3] = 103;

    // downsize memory
    data = (std::uint8_t *)std::realloc(data, sizeof(std::uint8_t) * 3);
    ASSERT_TRUE(data);
    ASSERT_EQ(data[0], 100);
    ASSERT_EQ(data[1], 101);
    ASSERT_EQ(data[2], 102);

    // free memory
    data = (std::uint8_t *)std::realloc(data, 0);
    ASSERT_FALSE(data);
}

int main(int argc, char *argv[]) {
    TEST_INIT();

    RUN_TEST(test_min);
    RUN_TEST(test_max);
    RUN_TEST(test_abs);
    RUN_TEST(test_atoi);
    RUN_TEST(test_itoa);
    RUN_TEST(test_ftoa);
    RUN_TEST(test_malloc_free);
    RUN_TEST(test_realloc);

    TEST_SUMMARY();
    return 0;
}