## 2024-05-24 - [Buffer Overflow in SendUpdateTitles]
**Vulnerability:** A critical stack-based buffer overflow was found in `Client::SendUpdateTitles` where `strcpy` was used to copy a `Title` name (256 bytes) into an `AppearanceData` field (128 bytes).
**Learning:** `memset` was using `strlen` on the destination buffer to determine the size to clear. This is dangerous as the buffer contents are undefined or untrusted.
**Prevention:** Always use `snprintf` for string copying into fixed-size buffers and ensure `memset` uses `sizeof(destination)` for clearing memory.
