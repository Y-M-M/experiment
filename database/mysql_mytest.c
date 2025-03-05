#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    mysql_library_init(0, NULL, NULL); // 初始化mysql库
    MYSQL *mysql = mysql_init(NULL); // 初始化一个连接句柄

    const char *host_name = "127.0.0.1";
    const char *user_name = "root";
    const char *password = ""; // 密码为空
    const char *db_name = "tpc";
    int port_num = 2881; // 端口为2881
    char *socket_name = NULL; // 采用TCP/IP连接
    MYSQL_RES *result;
    MYSQL_FIELD *fields;
    MYSQL_ROW row;
    int status = 0;
    // 建立连接
    if (mysql_real_connect(mysql, host_name, user_name, password, db_name,
                            port_num, socket_name,
                            CLIENT_MULTI_STATEMENTS) == NULL) 
    { 
        printf("mysql_real_connect() failed\n");
        mysql_close(mysql);
        exit(1); 
    }

    status = mysql_query(mysql, "DROP TABLE IF EXISTS NATIONcopy;"); // 向observer节点发送sql语句

    if (status)  // status为0时，执行语句失败
    { 
        printf("Could not execute drop statement(s)"); 
        mysql_close(mysql);
        exit(0);
    }

    // sql语句-创建表
    status = mysql_query(mysql, "CREATE TABLE NATIONcopy ( N_NATIONKEY INTEGER NOT NULL, N_NAME CHAR(25) NOT NULL,"
                                   " N_REGIONKEY INTEGER NOT NULL,"
                                    "N_COMMENT VARCHAR(152));");
    if (status)
    {
        printf("Could not execute create statement");
        mysql_close(mysql);
        exit(0);
    }

    // 向表中插入数据
    status = mysql_query(mysql, "INSERT INTO NATIONcopy"
                                " SELECT * FROM NATION;");

    status = mysql_query(mysql, "COMMIT;");

    if (status)
    {
        printf("Could not execute create status");
        mysql_close(mysql);
        exit(0);
    }

    // 查看表中数据
    status = mysql_query(mysql, "SELECT * FROM NATIONcopy;");

    result = mysql_store_result(mysql); // 获取返回值

    if (result)
    {
        int num_fields = mysql_num_fields(result);
        int num_rows = mysql_num_rows(result);
        printf("result: %d rows %d fields\n", num_rows, num_fields); printf("---------------------\n");
        fields = mysql_fetch_fields(result);
        for (int i = 0; i < num_fields; ++i) 
        { 
            printf("%s\t", fields[i].name);
        }
        printf("\n---------------------\n");
        while ((row = mysql_fetch_row(result))) { for (int i = 0; i < num_fields; ++i) {
        printf("%s\t", row[i] ? row[i] : "NULL"); }
        printf("\n"); }
        printf("---------------------\n");

        mysql_free_result(result);
    }
    else /* no result set or error */ {
        if (mysql_field_count(mysql) == 0) 
        {
            printf("%lld rows affected\n", mysql_affected_rows(mysql));
        } 
        else /* some error occurred */ 
        {
            printf("Could not retrieve result set\n"); 
        }
    }

    status = mysql_query(mysql, "DROP TABLE NATION_copy;");

    mysql_close(mysql); // 关闭连接
    return 0;
}