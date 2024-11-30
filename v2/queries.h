#define INSERT_LINE_QUERY "INSERT INTO LINES (file_name,line_no,content) values (?,?,?)"
#define FILE_EXISTS_QUERY "SELECT EXISTS(SELECT 1 FROM LINES WHERE file_name = ?)"
#define LAST_LINE_NUMBER_QUERY "SELECT line_no from LINES WHERE file_name = ? ORDER BY line_no DESC LIMIT 1"
#define GET_LINES_IN_RANGE_QUERY "SELECT * FROM LINES WHERE file_name = ? AND line_no >= ? AND line_no <= ?"
#define GET_LINE_BY_NUMBER_QUERY "SELECT * FROM LINES WHERE file_name = ? AND line_no = ?"