## 2024-05-24 - [Commands.cpp Buffer Overflow Mitigation]
**Vulnerability:** Extensive use of `sprintf` to format strings into fixed-size stack buffers (e.g., `char tmp[128]`, `char tmp[75]`) in `source/WorldServer/Commands/Commands.cpp`. This creates significant buffer overflow risks if inputs (like entity names or command arguments) exceed the buffer size.
**Learning:** Legacy C++ code often neglects bounds checking for string formatting. Even with fixed constraints in game logic (e.g., max name length), defensive coding dictates using bounds-checked functions to prevent crashes or exploitation from unexpected long inputs.
**Prevention:** Always use `snprintf` instead of `sprintf`. Ensure the size argument passed to `snprintf` matches the destination buffer size (`sizeof(buffer)`).

## 2024-05-24 - [Integer Overflow in Currency Calculation]
**Vulnerability:** In `Command_StoreSetPrice`, multiplying `int32` currency values (e.g., `plat * 1000000`) caused a 32-bit signed integer overflow when values exceeded ~2147 (since 2147 * 1M > INT_MAX). The overflow resulted in negative prices being set.
**Learning:** C++ performs arithmetic based on the operand types. Even if the result is assigned to `int64`, the intermediate calculation `int32 * int` is performed as `int` (32-bit).
**Prevention:** Explicitly cast operands to `int64` (e.g., `(int64)plat * 1000000`) to force 64-bit arithmetic for the entire expression.
