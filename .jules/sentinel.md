## 2024-05-24 - [Commands.cpp Buffer Overflow Mitigation]
**Vulnerability:** Extensive use of `sprintf` to format strings into fixed-size stack buffers (e.g., `char tmp[128]`, `char tmp[75]`) in `source/WorldServer/Commands/Commands.cpp`. This creates significant buffer overflow risks if inputs (like entity names or command arguments) exceed the buffer size.
**Learning:** Legacy C++ code often neglects bounds checking for string formatting. Even with fixed constraints in game logic (e.g., max name length), defensive coding dictates using bounds-checked functions to prevent crashes or exploitation from unexpected long inputs.
**Prevention:** Always use `snprintf` instead of `sprintf`. Ensure the size argument passed to `snprintf` matches the destination buffer size (`sizeof(buffer)`).

## 2024-05-24 - [Integer Overflow in Currency Calculation]
**Vulnerability:** In `Command_StoreSetPrice`, multiplying `int32` currency values (e.g., `plat * 1000000`) caused a 32-bit signed integer overflow when values exceeded ~2147 (since 2147 * 1M > INT_MAX). The overflow resulted in negative prices being set.
**Learning:** C++ performs arithmetic based on the operand types. Even if the result is assigned to `int64`, the intermediate calculation `int32 * int` is performed as `int` (32-bit).
**Prevention:** Explicitly cast operands to `int64` (e.g., `(int64)plat * 1000000`) to force 64-bit arithmetic for the entire expression.

## 2024-05-27 - [Manual String Buffer Overflow]
**Vulnerability:** In `LoginDatabase::SetZoneInformation`, a buffer was allocated based on string length but failed to account for the null terminator when appending characters manually, leading to a heap buffer over-read/flow.
**Learning:** Manual string construction using `new char[]` and `strncpy` is error-prone. One-off errors in allocation size are common.
**Prevention:** Use `std::string` or ensure allocation size accounts for all characters plus the null terminator. Always null-terminate manually constructed C-strings.

## 2024-05-30 - [Buffer Overflow in ClaimItem]
**Vulnerability:** `sprintf` was used to format user-controlled or potentially large strings into fixed-size buffers in `source/WorldServer/WorldDatabase.cpp`, specifically in `ClaimItem`.
**Learning:** Even seemingly benign messages or internal logic can be vulnerable if they handle variable-length strings (like item names) without bounds checking.
**Prevention:** Replace `sprintf` with `snprintf(buffer, sizeof(buffer), ...)` to enforce buffer limits and prevent overflows.

## 2025-02-19 - [Buffer Overflow in ZoneServer]
**Vulnerability:** Found  usage with a fixed-size stack buffer () in  where the input string () length was not checked, leading to potential buffer overflow. Also found unsafe  usage in .
**Learning:** Legacy C++ code often uses  without bounds checking. Even if data comes from a database, it should be treated as untrusted to ensure defense in depth.
**Prevention:** Replaced  with , which allows specifying the buffer size to prevent overflow. Always prefer  or C++ string formatting over .

## 2025-02-19 - [Buffer Overflow in ZoneServer]
**Vulnerability:** Found `sprintf` usage with a fixed-size stack buffer (`tmp[200]`) in `ZoneServer::CheckLocationGrids` where the input string (`grid->name`) length was not checked, leading to potential buffer overflow. Also found unsafe `sprintf` usage in `ZoneServer::HandleEmote`.
**Learning:** Legacy C++ code often uses `sprintf` without bounds checking. Even if data comes from a database, it should be treated as untrusted to ensure defense in depth.
**Prevention:** Replaced `sprintf` with `snprintf`, which allows specifying the buffer size to prevent overflow. Always prefer `snprintf` or C++ string formatting over `sprintf`.
