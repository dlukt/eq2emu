## 2024-05-24 - [Commands.cpp Buffer Overflow Mitigation]
**Vulnerability:** Extensive use of `sprintf` to format strings into fixed-size stack buffers (e.g., `char tmp[128]`, `char tmp[75]`) in `source/WorldServer/Commands/Commands.cpp`. This creates significant buffer overflow risks if inputs (like entity names or command arguments) exceed the buffer size.
**Learning:** Legacy C++ code often neglects bounds checking for string formatting. Even with fixed constraints in game logic (e.g., max name length), defensive coding dictates using bounds-checked functions to prevent crashes or exploitation from unexpected long inputs.
**Prevention:** Always use `snprintf` instead of `sprintf`. Ensure the size argument passed to `snprintf` matches the destination buffer size (`sizeof(buffer)`).
