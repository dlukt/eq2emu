#include <iostream>
#include <string>
#include <cassert>
#include <cstring>
#include <cstdint>

// Dummy mock for DBcore to test DoEscapeString and getSafeEscapeString logic without a real DB connection
class DBcoreMock {
public:
    int32_t DoEscapeString(char* tobuf, const char* frombuf, int32_t fromlen) {
        // Dummy escape: just replace ' with \'
        int out_len = 0;
        for (int i = 0; i < fromlen; ++i) {
            if (frombuf[i] == '\'') {
                tobuf[out_len++] = '\\';
                tobuf[out_len++] = '\'';
            } else {
                tobuf[out_len++] = frombuf[i];
            }
        }
        tobuf[out_len] = '\0';
        return out_len;
    }

    std::string getSafeEscapeString(const char* from_string) {
        if (!from_string)
            return "";
        int orig_size = strlen(from_string);

        std::string ret;
        ret.resize((orig_size * 2) + 1);
        int32_t new_len = DoEscapeString(&ret[0], from_string, orig_size);
        ret.resize(new_len);
        return ret;
    }

    std::string getSafeEscapeString(std::string* from_string){
        if(!from_string)
            return "";
        int orig_size = from_string->length();

        std::string ret;
        ret.resize((orig_size * 2) + 1);
        int32_t new_len = DoEscapeString(&ret[0], from_string->c_str(), orig_size);
        ret.resize(new_len);
        return ret;
    }
};

void test_getSafeEscapeString() {
    DBcoreMock db;

    // Test with const char*
    std::string s1 = "hello'world";
    std::string s2 = db.getSafeEscapeString(s1.c_str());
    assert(s2 == "hello\\'world");

    // Test with std::string*
    std::string s3 = db.getSafeEscapeString(&s1);
    assert(s3 == "hello\\'world");

    // Test with nullptr
    std::string empty = db.getSafeEscapeString((const char*)nullptr);
    assert(empty == "");

    std::string empty2 = db.getSafeEscapeString((std::string*)nullptr);
    assert(empty2 == "");

    std::cout << "All getSafeEscapeString tests passed!" << std::endl;
}

#ifdef DBCORE_TEST_STANDALONE
int main() {
    test_getSafeEscapeString();
    return 0;
}
#endif // DBCORE_TEST_STANDALONE
