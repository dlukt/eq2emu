## 2024-05-24 - [Buffer Overflow in SendUpdateTitles]
**Vulnerability:** A critical stack-based buffer overflow was found in `Client::SendUpdateTitles` where `strcpy` was used to copy a `Title` name (256 bytes) into an `AppearanceData` field (128 bytes).
**Learning:** `memset` was using `strlen` on the destination buffer to determine the size to clear. This is dangerous as the buffer contents are undefined or untrusted.
**Prevention:** Always use `snprintf` for string copying into fixed-size buffers and ensure `memset` uses `sizeof(destination)` for clearing memory.
## 2025-02-18 - Legacy Buffer Overflows
**Vulnerability:** Extensive use of `sprintf` on fixed-size buffers (stack and heap) without size checks.
**Learning:** Legacy C++ patterns often rely on manual buffer management. `sprintf` blindly writes to the buffer, leading to overflow if the format string expansion exceeds the buffer size.
**Prevention:** Always use `snprintf` with explicit buffer size limits. Be careful with pointers vs arrays (use `sizeof` for arrays, explicit size for pointers). Remove redundant `memset` when `snprintf` guarantees null-termination and overwrite.
