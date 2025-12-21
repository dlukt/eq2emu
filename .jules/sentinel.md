## 2024-05-23 - Fix SQL Injection in Command_MoveCharacter
**Vulnerability:** SQL Injection in `Commands::Command_MoveCharacter`
**Learning:** `Command_MoveCharacter` took user input directly from `Seperator::arg` and inserted it into a SQL query using `snprintf` without any escaping. This allowed attackers to manipulate the query (e.g. via `/movechar ' OR 1=1 -- zone`).
**Prevention:** Always escape user input using `database.getSafeEscapeString()` before using it in SQL queries, especially when constructing queries manually with string concatenation or formatting. Using `std::string` for query construction avoids buffer overflows associated with fixed-size buffers like `char query[256]`.
