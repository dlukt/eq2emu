## 2024-10-25 - strlen loop anti-pattern
**Learning:** Found multiple instances of `strlen(s)` called before a loop or inside a loop condition, causing unnecessary O(N) scans. For string parsing (like `hextoi`), iterating forward until null terminator is more efficient (O(N) vs O(2N)) and cache-friendly.
**Action:** Replace `strlen` dependent loops with forward iteration checking for `*p != '\0'` when safe.
## 2024-11-20 - [Optimizing IsNumber Anti-Pattern]
**Learning:** Legacy C++ codebases often iterate strings twice for simple validation (once for length via `strlen`, once for content). This is a textbook O(2N) -> O(N) optimization. In this specific repo, `MiscFunctions.cpp` and `seperator.h` both exhibited this pattern.
**Action:** When validating strings, prefer pointer iteration or single-pass checks over separate length calculation. Also, always verify if helper classes (like `Seperator`) reimplement standard logic inefficiently.
## 2024-11-20 - [Efficient String Arguments]
**Learning:** Found multiple utility functions in  passing  by value. In C++ server code, this triggers unnecessary copy constructors for every call. Changing to  is a standard optimization.
**Action:** Default to  for read-only string arguments in utility headers.

## 2024-11-20 - [Serialization Type Safety]
**Learning:** Discovered a buffer overread in  where  was reading 4 bytes from a 2-byte variable (). This happened because the variable holding the string length was declared with the wrong type.
**Action:** When working with raw memory serialization (), double-check that the source variable's type width matches the number of bytes being copied.
## 2024-11-20 - [Efficient String Arguments]
**Learning:** Found multiple utility functions in `MiscFunctions.cpp` passing `std::string` by value. In C++ server code, this triggers unnecessary copy constructors for every call. Changing to `const std::string&` is a standard optimization.
**Action:** Default to `const std::string&` for read-only string arguments in utility headers.

## 2024-11-20 - [Serialization Type Safety]
**Learning:** Discovered a buffer overread in `storeInt32String` where `memcpy` was reading 4 bytes from a 2-byte variable (`sint16`). This happened because the variable holding the string length was declared with the wrong type.
**Action:** When working with raw memory serialization (`memcpy`), double-check that the source variable's type width matches the number of bytes being copied.
