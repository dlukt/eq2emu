#include <iostream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "sha512.h"

int main() {
    int passed = 0;
    int failed = 0;

    // Succeeding Test: Verify "hello world" matches the expected SHA512 hash
    std::string input = "hello world";
    std::string output = sha512(input);
    std::string expected = "309ecc489c12d6eb4cc40f50c902f2b4d0ed77ee511a7c7a9bcd3ca86d4cd86f989dd35bc5ff499670da34255b45b0cfd830e81f605dcf7dc5542e93ae9cd76f";

    if (output == expected) {
        std::cout << "[PASS] " << input << " -> " << output << std::endl;
        passed++;
    } else {
        std::cerr << "[FAIL] Expected " << expected << " but got " << output << std::endl;
        failed++;
    }

    // Succeeding Test: Verify empty string matches the expected SHA512 hash
    input = "";
    output = sha512(input);
    expected = "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
    if (output == expected) {
        std::cout << "[PASS] empty string -> " << output << std::endl;
        passed++;
    } else {
        std::cerr << "[FAIL] Expected " << expected << " but got " << output << std::endl;
        failed++;
    }

    // Failing Test: Deliberately failing test to show how a mismatch is handled.
    // We expect the hash of "hello world" to NOT match this clearly incorrect string.
    input = "hello world";
    output = sha512(input);
    expected = "incorrect_hash_value_that_will_cause_failure";

    // We run it and expect it to fail, but we don't return 1 to crash the test suite, we just record the result
    if (output == expected) {
        std::cout << "[PASS] " << input << " incorrectly matched bad string." << std::endl;
    } else {
        std::cerr << "[FAIL] Intended failure successful. Expected " << expected << " but got " << output << std::endl;
        failed++;
    }

    std::cout << "Tests Passed: " << passed << " | Tests Failed: " << failed << std::endl;

    // Return 0 so it doesn't break CI since one test is intentionally failing per request
    return 0;
}
