## 2024-05-23 - [SQL Injection in WorldDatabase]
**Vulnerability:** Found `INSERT` statements in `source/WorldServer/Bots/BotDB.cpp` constructing queries using raw string concatenation with `name` and `type` parameters.
**Learning:** Even if callers currently sanitize inputs (like `CheckNameFilter`), public methods that construct SQL queries must defensively escape parameters to prevent future vulnerabilities (Defense in Depth).
**Prevention:** Always use `EscapeStr()` (or `getSafeEscapeString()`) on string parameters before embedding them into SQL query strings, regardless of current caller behavior. Ensure the escaping method matches the database connection object being used.
