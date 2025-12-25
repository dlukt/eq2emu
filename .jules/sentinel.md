## 2025-02-14 - Fix SQL Injection in SaveSignMark
**Vulnerability:** SQL Injection in `WorldDatabase::SaveSignMark` due to unescaped user input (`char_name`) being concatenated into a SQL query.
**Learning:** `DatabaseNew::Query` uses `vsnprintf` for formatting but does not automatically escape string parameters. Callers are responsible for escaping. This differs from Prepared Statements which handle escaping automatically.
**Prevention:** Always use `database_new.EscapeStr(...)` or `getSafeEscapeString(...)` when inserting strings into SQL queries constructed via `Query`. Prefer parameterized queries if the database abstraction layer supports them (which this legacy codebase does not seem to fully support in this context).
