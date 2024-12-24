#define FILE_SCHEMA \
    "BEGIN TRANSACTION;\n" \
    "CREATE TABLE `FILE_CONTENT` (`line_no` INTEGER PRIMARY KEY, `CONTENT` TEXT);\n" \
    "DELETE FROM sqlite_sequence;\n" \
    "COMMIT;\n"

