#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
int main() {
  MYSQL *conn;
  MYSQL_RES *result;
  MYSQL_ROW row;

  conn = mysql_init(NULL);
  if (!mysql_real_connect(conn, "hostname", "username", "password", "database_name", 0, NULL, 0)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }

mysql_query(conn, "SELECT * FROM users INNER JOIN radius_users ON users.username = radius_users.username");

result = mysql_store_result(conn);

while ((row = mysql_fetch_row(result)) != NULL) {
   printf("Username: %s\n", row[0]);
}

mysql_query(conn, "SELECT * FROM users LEFT JOIN radius_users ON users.username = radius_users.username WHERE radius_users.username IS NULL");

result = mysql_store_result(conn);


while ((row = mysql_fetch_row(result)) != NULL) {
  char *username = row[0];
  char *query = (char*) malloc(strlen("UPDATE users SET online = 'false' WHERE username = ''") + strlen(username) + 1);
  sprintf(query, "UPDATE users SET online = 'false' WHERE username = '%s'", username);
  if (mysql_query(conn, query)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  mysql_free_result(result);
  mysql_close(conn);

  return 0;
}