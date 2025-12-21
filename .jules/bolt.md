## 2024-10-25 - strlen loop anti-pattern
**Learning:** Found multiple instances of `strlen(s)` called before a loop or inside a loop condition, causing unnecessary O(N) scans. For string parsing (like `hextoi`), iterating forward until null terminator is more efficient (O(N) vs O(2N)) and cache-friendly.
**Action:** Replace `strlen` dependent loops with forward iteration checking for `*p != '\0'` when safe.
