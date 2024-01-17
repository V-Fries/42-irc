#include "ft_String.hpp"
#include <iostream>

#define test1_split_call "ft::String(\"424242test2A424242test4B4242\").split(\"42\")"
#define test1_expected "{\"test2A\", \"test4B\"}"

#define test2_split_call "ft::String(\"424242test2A424242test4B4242\").split(\"42\", SPLIT_ON_CHARACTER_SET)"
#define test2_expected "{\"test\", \"A\", \"test\", \"B\"}"

#define test3_split_call "ft::String(\"\").split(\"42\")"
#define test3_expected "{}"

#define test4_split_call "ft::String(\"\").split(\"42\", SPLIT_ON_CHARACTER_SET)"
#define test4_expected "{}"

#define test5_split_call "ft::String(\"test\").split(\"\")"
#define test5_expected "{}"

#define test6_split_call "ft::String(\"test\").split(\"\", SPLIT_ON_CHARACTER_SET)"
#define test6_expected "{}"

#define test7_split_call "ft::String(\"2424242test2A424242test4B4242\").split(\"42\")"
#define test7_expected "{\"2\", \"test2A\", \"test4B\"}"

void    test_split() {
    std::vector<ft::String> test1 = ft::String("424242test2A424242test4B4242").split("42");
    if (test1.size() != 2
        || test1[0] != "test2A"
        || test1[1] != "test4B") {
        std::cerr << "split failed with " test1_split_call << "\n"
                     "expected " test1_expected << " got " << test1 << std::endl;
        }

    std::vector<ft::String> test2 = ft::String("424242test2A424242test4B4242")
                                        .split("42", SPLIT_ON_CHARACTER_SET);
    if (test2.size() != 4
        || test2[0] != "test"
        || test2[1] != "A"
        || test2[2] != "test"
        || test2[3] != "B") {
        std::cerr << "split failed with " test2_split_call << "\n"
                     "expected " test2_expected << " got " << test2 << std::endl;
        }

    std::vector<ft::String> test3 = ft::String("").split("42");
    if (!test3.empty()) {
        std::cerr << "split failed with " test3_split_call << "\n"
                     "expected " test3_expected << " got " << test3 << std::endl;
    }

    std::vector<ft::String> test4 = ft::String("").split("42", SPLIT_ON_CHARACTER_SET);
    if (!test4.empty()) {
        std::cerr << "split failed with " test4_split_call << "\n"
                     "expected " test4_expected << " got " << test4 << std::endl;
    }

    std::vector<ft::String> test5 = ft::String("test").split("");
    if (test5.size() != 1
        || test5[0] != "test") {
        std::cerr << "split failed with " test5_split_call << "\n"
                     "expected " test5_expected << " got " << test5 << std::endl;
    }

    std::vector<ft::String> test6 = ft::String("test").split("", SPLIT_ON_CHARACTER_SET);
    if (test6.size() != 1
        || test6[0] != "test") {
        std::cerr << "split failed with " test6_split_call << "\n"
                     "expected " test6_expected << " got " << test6 << std::endl;
    }

    std::vector<ft::String> test7 = ft::String("2424242test2A424242test4B4242").split("42");
    if (test7.size() != 3
        || test7[0] != "2"
        || test7[1] != "test2A"
        || test7[2] != "test4B") {
        std::cerr << "split failed with " test7_split_call << "\n"
                     "expected " test7_expected << " got " << test7 << std::endl;
    }
}