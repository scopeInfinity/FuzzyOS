#include <stdio.h>
#include <testing.h>
#include <vector.h>

class TestElement {
    int id;
    static int _global_instance_active_count;

  public:
    TestElement() { id = _global_instance_active_count++; }

    TestElement(int id) : id(id) { _global_instance_active_count++; }

    TestElement(const TestElement &o) : id(o.id) {
        _global_instance_active_count++;
    }

    ~TestElement() { _global_instance_active_count--; }

    static int get_global_instance_active_count() {
        return _global_instance_active_count;
    }

    static void clear_global_instance_active_count() {
        _global_instance_active_count = 0;
    }

    int get_id() { return id; }
};

int TestElement::_global_instance_active_count;

void test_constructor_default() {
    std::vector<int> v1;
    ASSERT_EQ(v1.size(), 0);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2;
    ASSERT_EQ(v2.size(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 0);
}

void test_constructor_atsize_withdefault() {
    std::vector<int> v1(2);
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 0);
    ASSERT_EQ(v1[1], 0);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2(2);
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0].get_id(), 0);
    ASSERT_EQ(v2[1].get_id(), 1); // id increment by 1
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);
}

void test_constructor_atsize() {
    std::vector<int> v1(2, 100);
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 100);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2(2, TestElement(100));
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0].get_id(), 100);
    ASSERT_EQ(v2[1].get_id(), 100); // same id
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);
}

void test_destructor() {
    TestElement::clear_global_instance_active_count();
    {
        std::vector<TestElement> v(1);
        ASSERT_EQ(v.size(), 1);
        ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);
        // going out of scope
    }
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 0);
}

void test_constructor_copy() {
    std::vector<int> v1(2);
    ASSERT_EQ(v1.size(), 2)
    v1[0] = 100;
    v1[1] = 101;

    std::vector<int> v2 = v1;
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0], 100);
    ASSERT_EQ(v2[1], 101);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v3(1);
    ASSERT_EQ(v3.size(), 1);
    ASSERT_EQ(v3[0].get_id(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);

    std::vector<TestElement> v4 = v3;
    ASSERT_EQ(v4.size(), 1);
    ASSERT_EQ(v4[0].get_id(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);
}

void test_assign() {
    std::vector<int> v1(2);
    ASSERT_EQ(v1.size(), 2)
    v1[0] = 100;
    v1[1] = 101;

    std::vector<int> v2(1);
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0], 0);
    v2 = v1;
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0], 100);
    ASSERT_EQ(v2[1], 101);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v3(1);
    ASSERT_EQ(v3.size(), 1);
    ASSERT_EQ(v3[0].get_id(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);

    std::vector<TestElement> v4(2);
    ASSERT_EQ(v4.size(), 2);
    ASSERT_EQ(v4[0].get_id(), 1);
    ASSERT_EQ(v4[1].get_id(), 2);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 3);
    v4 = v3;
    ASSERT_EQ(v4.size(), 1);
    ASSERT_EQ(v4[0].get_id(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);
}

void test_reserve() {
    std::vector<int> v1(2);
    ASSERT_EQ(v1.size(), 2);
    v1[0] = 100;
    v1[1] = 101;
    const int *v1_base_a = &v1[0];
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);
    v1.reserve(64);
    const int *v1_base_b = &v1[0];
    ASSERT_NE(v1_base_a, v1_base_b); // reallocation
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);
    v1.reserve(2);
    const int *v1_base_c = &v1[0];
    ASSERT_NE(v1_base_b, v1_base_c); // reallocation
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2(1, TestElement(201));
    const TestElement *v2_base_a = &v2[0];
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0].get_id(), 201);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);
    v2.reserve(16);
    const TestElement *v2_base_b = &v2[0];
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0].get_id(), 201);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);
    ASSERT_NE(v2_base_a, v2_base_b); // reallocation
}

void test_resize_default() {
    std::vector<int> v1;
    ASSERT_EQ(v1.size(), 0);

    v1.resize(2); // upsize
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 0);
    ASSERT_EQ(v1[1], 0);
    v1[0] = 100;
    v1[1] = 101;
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);

    v1.resize(1); // downsize
    ASSERT_EQ(v1.size(), 1);
    ASSERT_EQ(v1[0], 100);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2;
    ASSERT_EQ(v2.size(), 0);
    v2.resize(2); // upsize
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0].get_id(), 0);
    ASSERT_EQ(v2[1].get_id(), 1);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);

    v2.resize(1); // downsize
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0].get_id(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);
}

void test_resize_nodefault() {
    std::vector<int> v1;
    ASSERT_EQ(v1.size(), 0);

    v1.resize(2, 100); // upsize
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 100);

    v1.resize(1, 200); // downsize
    ASSERT_EQ(v1.size(), 1);
    ASSERT_EQ(v1[0], 100);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2;
    ASSERT_EQ(v2.size(), 0);
    v2.resize(2, TestElement(100)); // upsize
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0].get_id(), 100);
    ASSERT_EQ(v2[1].get_id(), 100);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);

    v2.resize(1, TestElement(200)); // downsize
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0].get_id(), 100);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);
}

void test_push_back() {
    std::vector<int> v1;
    ASSERT_EQ(v1.size(), 0);

    v1.push_back(100);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1.size(), 1);

    v1.push_back(101);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);
    ASSERT_EQ(v1.size(), 2);

    v1.push_back(102);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);
    ASSERT_EQ(v1[2], 102);
    ASSERT_EQ(v1.size(), 3);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2;
    ASSERT_EQ(v2.size(), 0);
    v2.push_back(TestElement(100));
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0].get_id(), 100);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);

    v2.push_back(TestElement(101));
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0].get_id(), 100);
    ASSERT_EQ(v2[1].get_id(), 101);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);

    v2.push_back(TestElement(102));
    ASSERT_EQ(v2.size(), 3);
    ASSERT_EQ(v2[0].get_id(), 100);
    ASSERT_EQ(v2[1].get_id(), 101);
    ASSERT_EQ(v2[2].get_id(), 102);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 3);
}

void test_pop_back() {
    std::vector<int> v1(3);
    ASSERT_EQ(v1.size(), 3);
    v1[0] = 100;
    v1[1] = 101;
    v1[2] = 102;
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);
    ASSERT_EQ(v1[2], 102);

    v1.pop_back();
    ASSERT_EQ(v1.size(), 2);
    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);

    v1.pop_back();
    ASSERT_EQ(v1.size(), 1);
    ASSERT_EQ(v1[0], 100);

    v1.pop_back();
    ASSERT_EQ(v1.size(), 0);

    TestElement::clear_global_instance_active_count();
    std::vector<TestElement> v2(3);
    ASSERT_EQ(v2.size(), 3);
    ASSERT_EQ(v2[0].get_id(), 0);
    ASSERT_EQ(v2[1].get_id(), 1);
    ASSERT_EQ(v2[2].get_id(), 2);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 3);

    v2.pop_back();
    ASSERT_EQ(v2.size(), 2);
    ASSERT_EQ(v2[0].get_id(), 0);
    ASSERT_EQ(v2[1].get_id(), 1);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 2);

    v2.pop_back();
    ASSERT_EQ(v2.size(), 1);
    ASSERT_EQ(v2[0].get_id(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 1);

    v2.pop_back();
    ASSERT_EQ(v2.size(), 0);
    ASSERT_EQ(TestElement::get_global_instance_active_count(), 0);
}

void test_iterator() {
    std::vector<int> v1(3);
    ASSERT_EQ(v1.size(), 3);
    v1[0] = 100;
    v1[1] = 101;
    v1[2] = 102;

    std::vector<int>::iterator it = v1.begin();
    ASSERT_EQ(it++, &v1[0]);
    ASSERT_EQ(it++, &v1[1]);
    ASSERT_EQ(it++, &v1[2]);
    ASSERT_EQ(it--, v1.end());
    ASSERT_EQ(it--, &v1[2]);
    ASSERT_EQ(it--, &v1[1]);
    ASSERT_EQ(it, &v1[0]);
    ASSERT_EQ(it, v1.begin());
}

void test_accessor() {
    std::vector<int> v1(3);
    ASSERT_EQ(v1.size(), 3);
    v1[0] = 100;
    v1[1] = 101;
    v1[2] = 102;

    std::vector<int>::iterator it = v1.begin();
    ASSERT_EQ(&v1.front(), &v1[0]);
    ASSERT_EQ(&v1.back(), &v1[2]);
    ASSERT_EQ(v1.front(), 100);
    ASSERT_EQ(v1.back(), 102);
    v1.front()--;
    v1.back()++;
    ASSERT_EQ(v1.front(), 99);
    ASSERT_EQ(v1.back(), 103);

    ASSERT_EQ(v1.at(0), 99);
    ASSERT_EQ(v1.at(1), 101);
    ASSERT_EQ(v1.at(2), 103);
    v1.at(0)++;
    v1.at(2)--;

    ASSERT_EQ(v1[0], 100);
    ASSERT_EQ(v1[1], 101);
    ASSERT_EQ(v1[2], 102);
    v1.at(0)++;
    v1.at(2)--;
    ASSERT_EQ(v1[0], 101);
    ASSERT_EQ(v1[1], 101);
    ASSERT_EQ(v1[2], 101);
}

void test_op_compare() {
    std::vector<int> v1(3);
    ASSERT_EQ(v1.size(), 3);
    v1[0] = 100;
    v1[1] = 101;
    v1[2] = 102;

    std::vector<int> v2;
    ASSERT_NE(v1, v2);
    v2.push_back(100);
    ASSERT_NE(v1, v2);
    v2.push_back(101);
    ASSERT_NE(v1, v2);
    v2.push_back(102);
    ASSERT_EQ(v1, v2);
    v2.back()++;
    ASSERT_NE(v1, v2);
    v2.back()--;
    ASSERT_EQ(v1, v2);
    v2.push_back(103);
    ASSERT_NE(v1, v2);
}

int main(int argc, char *argv[]) {
    TEST_INIT();

    RUN_TEST(test_constructor_default);
    RUN_TEST(test_constructor_atsize_withdefault);
    RUN_TEST(test_constructor_atsize);
    RUN_TEST(test_constructor_copy);
    RUN_TEST(test_destructor);

    RUN_TEST(test_assign);
    RUN_TEST(test_reserve);
    RUN_TEST(test_resize_default);
    RUN_TEST(test_resize_nodefault);
    RUN_TEST(test_push_back);
    RUN_TEST(test_pop_back);

    RUN_TEST(test_iterator);
    RUN_TEST(test_accessor);

    RUN_TEST(test_op_compare);

    TEST_SUMMARY();
    return 0;
}