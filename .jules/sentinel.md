## 2024-05-23 - [SQL Injection in WorldDatabase]
**Vulnerability:** Found `INSERT` statements in `source/WorldServer/Bots/BotDB.cpp` constructing queries using raw string concatenation with `name` and `type` parameters.
**Learning:** Even if callers currently sanitize inputs (like `CheckNameFilter`), public methods that construct SQL queries must defensively escape parameters to prevent future vulnerabilities (Defense in Depth).
**Prevention:** Always use `EscapeStr()` (or `getSafeEscapeString()`) on string parameters before embedding them into SQL query strings, regardless of current caller behavior. Ensure the escaping method matches the database connection object being used.

## 2024-05-24 - [SQL Injection in Commands.cpp]
**Vulnerability:** Found `UPDATE` statements in `source/WorldServer/Commands/Commands.cpp` constructing queries using raw string concatenation with `val` (user input) in `Command_SetLocationEntry`. The value was inserted as `%s` without quotes, allowing injection if the field was a string type (like `condition`) or potential manipulation even for numeric types if not strictly validated.
**Learning:** Whitelisting column names is good, but value parameters must still be escaped and quoted, especially when the column type varies (string vs number). Unquoted string interpolation is a major red flag.
**Prevention:** Always escape user input using `getSafeEscapeString()` and wrap string parameters in quotes in the SQL query format string. Use parameterized queries if available (though this codebase uses `sprintf` style formatting).
