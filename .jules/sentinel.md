## 2025-02-14 - SQL Injection in Table Identifiers
**Vulnerability:** Unsanitized user input (`char* name`) was passed directly to `RunQuery2` in `GetColumnNames` (Login/WorldDatabase), allowing SQL injection via table name interpolation (`show columns FROM %s`).
**Learning:** `RunQuery2` uses `vsnprintf` internally, which does not escape string arguments. Standard escaping functions (`mysql_real_escape_string`) are insufficient for SQL identifiers (table names) if the placeholder is not quoted.
**Prevention:**
1. Avoid dynamic table names from user input if possible.
2. If dynamic identifiers are necessary, strictly validate them against a whitelist (e.g., alphanumeric + underscore only).
3. Created `IsValidIdentifier(const char*)` in `MiscFunctions` to perform this validation.

## 2026-01-17 - Stack Buffer Overflow in PacketStruct
**Vulnerability:** `sprintf` was used to construct variable names with potentially large integer indices into small fixed-size stack buffers (e.g., `char tmp[15]`), leading to confirmed stack smashing.
**Learning:** Hardcoded buffer sizes for string formatting are risky even for integers, as max integer string length + delimiters can exceed expectations (e.g. 23 chars vs 15 chars).
**Prevention:** Use `std::to_string` and `std::string` concatenation in C++ to avoid manual buffer management and overflow risks completely.

## 2025-01-29 - Integer Overflow in Packet Parsing
**Vulnerability:** Packet parsing logic in `MiscFunctions.cpp` used 32-bit arithmetic to validate string lengths against the buffer size. A specially crafted packet with a very large string length could cause an integer overflow, bypassing the bounds check and leading to a buffer overread (Segfault/DoS).
**Learning:** `int32` is typedef'd as `unsigned int` in this codebase, making `if (size < 0)` checks useless and allowing 4GB values to wrap around in 32-bit addition.
**Prevention:** Use 64-bit arithmetic (casting to `int64` or `uint64_t`) when validating lengths, or check conditions like `size > remaining_buffer` directly rather than `current + size > total`.

## 2025-01-29 - Denial of Service in Database Escaping
**Vulnerability:** `DatabaseNew::EscapeStr` returned `NULL` on memory allocation failure, but the return type is `std::string`. Constructing a `std::string` from `NULL` causes a crash (Undefined Behavior).
**Learning:** Always ensure return values match the return type's contract. For `std::string`, return `""` on failure, not `NULL`.
**Prevention:** Review return types carefully, especially when converting legacy C-style pointer code to C++ classes.

## 2025-05-19 - Buffer Overflow in Commands and ZoneServer
**Vulnerability:** Unsafe `strcpy` calls were used to copy `item->name` (std::string) and `zone_name` (const char*) into fixed-size buffers (`spawn->appearance.name` [128] and `zone_name` [64]) without length checks.
**Learning:** Legacy C-style string handling (`strcpy`, `sprintf`) in C++ codebases often lacks bounds checking, leading to stack or heap buffer overflows when source data exceeds destination capacity.
**Prevention:** Use `snprintf` with `sizeof(dest)` or `std::string` (if applicable) to ensure null-termination and prevent buffer overflows.
## 2025-05-23 - [Memory Leak in GetCharacterName]
**Vulnerability:** WorldDatabase::GetCharacterName returned a heap-allocated char* which was frequently not freed by callers, leading to memory leaks and potential DoS.
**Learning:** Returning raw pointers ownership transfer is error-prone in C++.
**Prevention:** Use RAII types like std::string or smart pointers (std::unique_ptr) to manage memory automatically.
