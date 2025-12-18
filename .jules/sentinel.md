## 2024-05-23 - Fix SQL Injection in WorldDatabase::AddMasterTitle
**Vulnerability:** SQL Injection in `WorldDatabase::AddMasterTitle`
**Learning:** The `RunQuery2` method does not automatically sanitize inputs when using formatted strings. It acts like `printf`, so user input passed directly to `%s` is vulnerable.
**Prevention:** Always use `getSafeEscapeString` or `getEscapeString` on any user-controlled string before passing it to a SQL query format string.
