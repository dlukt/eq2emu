## 2025-02-14 - Fix SQL Injection in SaveSignMark
**Vulnerability:** SQL Injection in `WorldDatabase::SaveSignMark` due to unescaped user input (`char_name`) being concatenated into a SQL query.
**Learning:** `DatabaseNew::Query` uses `vsnprintf` for formatting but does not automatically escape string parameters. Callers are responsible for escaping. This differs from Prepared Statements which handle escaping automatically.
**Prevention:** Always use `database_new.EscapeStr(...)` or `getSafeEscapeString(...)` when inserting strings into SQL queries constructed via `Query`. Prefer parameterized queries if the database abstraction layer supports them (which this legacy codebase does not seem to fully support in this context).
## 2024-05-23 - [CRITICAL] SQL Injection in WorldDatabase
**Vulnerability:** Found a SQL Injection vulnerability in `WorldDatabase::GetCharacterID` where user input `name` was concatenated directly into a SQL query string.
**Learning:** The `RunQuery2` function uses `printf`-style formatting but does NOT automatically escape string arguments. Developers must explicitly sanitize all string inputs before passing them to the query builder.
**Prevention:** Always use `getSafeEscapeString()` on untrusted string inputs before using them in SQL queries. Avoid passing raw pointers from user input directly to database functions.
