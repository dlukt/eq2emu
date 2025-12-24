## 2024-10-25 - strlen loop anti-pattern
**Learning:** Found multiple instances of `strlen(s)` called before a loop or inside a loop condition, causing unnecessary O(N) scans. For string parsing (like `hextoi`), iterating forward until null terminator is more efficient (O(N) vs O(2N)) and cache-friendly.
**Action:** Replace `strlen` dependent loops with forward iteration checking for `*p != '\0'` when safe.
## 2024-11-20 - [Optimizing IsNumber Anti-Pattern]
**Learning:** Legacy C++ codebases often iterate strings twice for simple validation (once for length via `strlen`, once for content). This is a textbook O(2N) -> O(N) optimization. In this specific repo, `MiscFunctions.cpp` and `seperator.h` both exhibited this pattern.
**Action:** When validating strings, prefer pointer iteration or single-pass checks over separate length calculation. Also, always verify if helper classes (like `Seperator`) reimplement standard logic inefficiently.
