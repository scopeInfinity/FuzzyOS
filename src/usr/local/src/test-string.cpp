#include <stdio.h>
#include <string.h>
#include <testing.h>

void test_strcmp() {
    ASSERT_TRUE(std::strcmp("hello", "hello") == 0);
    ASSERT_TRUE(std::strcmp("hell", "hello") < 0);
    ASSERT_TRUE(std::strcmp("hello", "hell") > 0);

    ASSERT_TRUE(std::strcmp("hello", "Hello") != 0);

    ASSERT_TRUE(std::strcmp("abc", "def") < 0);
    ASSERT_TRUE(std::strcmp("def", "abc") > 0);
    ASSERT_TRUE(std::strcmp("abcdef", "abcdef") == 0);
}

void test_strcmpi() {
    ASSERT_TRUE(std::strcmpi("hello", "hello") == 0);
    ASSERT_TRUE(std::strcmpi("hell", "hello") < 0);
    ASSERT_TRUE(std::strcmpi("hello", "hell") > 0);

    ASSERT_TRUE(std::strcmpi("hellO", "Hello") == 0);

    ASSERT_TRUE(std::strcmpi("ABC", "def") < 0);
    ASSERT_TRUE(std::strcmpi("DEF", "abc") > 0);
    ASSERT_TRUE(std::strcmpi("ABCDEF", "abcdef") == 0);
}

void test_strlen() {
    ASSERT_EQ(std::strlen(""), 0);
    ASSERT_EQ(std::strlen("hi"), 2);
    ASSERT_EQ(std::strlen("hello"), 5);
}

void test_strcpy() {
    char buffer[64];

    ASSERT_EQ(std::strcmp(std::strcpy(buffer, "hi"), "hi"), 0);
    ASSERT_NE(std::strcmp(std::strcpy(buffer, "hello"), "hi"), 0);
    ASSERT_EQ(std::strcmp(buffer, "hello"), 0);
}

void test_strncpy() {
    char buffer[64];

    std::strncpy(buffer, "hello", 2);
    ASSERT_TRUE(buffer[0] == 'h' && buffer[1] == 'e');
}

void test_memcpy() {
    char buffer[64];

    std::memcpy(buffer, "hello", 6);
    ASSERT_EQ(std::strcmp(buffer, "hello"), 0);
}

void test_memset() {
    char buffer[64];

    buffer[5] = '\0';
    std::memset(buffer, 'a', 5);
    ASSERT_EQ(std::strcmp(buffer, "aaaaa"), 0);
}

void test_strchr() {
    char buffer[] = "hello";
    char *occ;
    occ = std::strchr(buffer, 'l');
    ASSERT_EQ(occ, buffer + 2);
    occ = std::strchr(occ + 1, 'l');
    ASSERT_EQ(occ, buffer + 3);
    occ = std::strchr(occ + 1, 'l');
    ASSERT_EQ(occ, NULL);
}

void test_strtok() {
    const char delim[] = " ,";
    char buffer[] = "This world is amazing,is it not?";
    char *tok;

    ASSERT_EQ(std::strcmp((tok = std::strtok(buffer, delim)), "This"), 0);
    ASSERT_EQ(std::strcmp((tok = std::strtok(NULL, delim)), "world"), 0);
    ASSERT_EQ(std::strcmp((tok = std::strtok(NULL, delim)), "is"), 0);
    ASSERT_EQ(std::strcmp((tok = std::strtok(NULL, delim)), "amazing"), 0);
    ASSERT_EQ(std::strcmp((tok = std::strtok(NULL, delim)), "is"), 0);
    ASSERT_EQ(std::strcmp((tok = std::strtok(NULL, delim)), "it"), 0);
    ASSERT_EQ(std::strcmp((tok = std::strtok(NULL, delim)), "not?"), 0);
    ASSERT_EQ((tok = std::strtok(NULL, delim)), NULL);
}

int main(int argc, char *argv[]) {
    TEST_INIT();

    RUN_TEST(test_strcmp);
    RUN_TEST(test_strcmpi);
    RUN_TEST(test_strlen);
    RUN_TEST(test_strcpy);
    RUN_TEST(test_strncpy);
    RUN_TEST(test_memcpy);
    RUN_TEST(test_memset);
    RUN_TEST(test_strchr);
    RUN_TEST(test_strtok);

    TEST_SUMMARY();
    return 0;
}