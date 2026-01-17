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
