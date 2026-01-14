## 2025-02-14 - Buffer Overflow Vulnerabilities in WorldServer
**Vulnerability:** Unsafe string manipulation using `strncpy` without explicit null termination and `sprintf` without bounds checking in `WorldServer`.
**Learning:** `strncpy` does not guarantee null termination if the source string length equals or exceeds the buffer size, leading to potential buffer overreads/overflows. `sprintf` is inherently unsafe as it lacks buffer size limits.
**Prevention:**
1. Replace `strncpy(dst, src, sizeof(dst))` with `strncpy(dst, src, sizeof(dst) - 1); dst[sizeof(dst) - 1] = '\0';` or use `strlcpy` if available.
2. Replace `sprintf` with `snprintf` to enforce buffer limits.
