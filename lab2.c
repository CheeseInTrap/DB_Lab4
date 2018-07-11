#ifdef _LCC_
#define _LCC_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <string.h>

int main() {


    MYSQL mysql_conn; /* Connection handle */
    MYSQL_RES *mysql_result;
    MYSQL_ROW mysql_row; /* Row data */
    int f1, f2, num_row, num_col;

    //query command
    char command[1000];
    //query number
    int number;
    int min = 1;
    int max = 9;



    if(mysql_init(&mysql_conn)!=NULL){

        if(mysql_real_connect(&mysql_conn,"localhost","root","root","company",MYSQL_PORT,NULL,0)!=NULL){

            //unify charset , command line -gbk
            mysql_query(&mysql_conn, "set names gbk");

            //output prompt and get query
            printf("=============================================================\n");
            printf("Connection success\nplease input your query\n");
            gets(command);

            //if get "exit",close db and stop
            while(strcmp(command,"exit")!=0){

                //extract query number after signal "-q"
                int i = 0;
                for(i=0;i<strlen(command);i++){
                    if(command[i]=='-' && command[i+1]=='q'){
                        number = command[i+3]-'0';
                        break;
                    }
                }

                //if the query number is out of range
                if( number < min || number > max){
                    printf("没有该查询参数");
                    break;
                }

                //extract arguments after "-p"
                int j;
                char p[20] = {""};
                for(j = i+3;j<strlen(command);j++){
                    if(command[j] == '-' && command[j+1] == 'p'){

                        strncpy(p,command+j+3,strlen(command)-j-3);
                        break;
                    }
                }

                //build the query sql
                char *sql;

                //classify
                if(number == 1 || number==2 || number == 3 || number == 5 || number== 6 || number==8){

                    //joint sql query
                    //single argument
                    if(number == 1){
                        sql = malloc(strlen("select essn from work_on where pno = \"")+strlen(p)+2);
                        strcpy(sql,"select essn from work_on where pno = \"");
                        strcat(sql,p);
                        strcat(sql,"\"");
                    }else if(number == 2){
                        sql = malloc(strlen("select ename from employee,work_on,project where employee.essn = work_on.essn and work_on.pno = project.pno and project.pname = \"")+strlen(p)+2);
                        strcpy(sql,"select ename from employee,work_on,project where employee.essn = work_on.essn and work_on.pno = project.pno and project.pname = \"");
                        strcat(sql,p);
                        strcat(sql,"\"");
                    }else if(number == 3){
                        sql = malloc(strlen("select ename,address from employee,department where employee.dno = department.dno and department.dname = \"")+strlen(p)+2);
                        strcpy(sql,"select ename,address from employee,department where employee.dno = department.dno and department.dname =  \"");
                        strcat(sql,p);
                        strcat(sql,"\"");
                    }else if(number == 5){
                        sql = malloc(strlen("select ename from employee where essn not in(select essn from work_on where pno = \"")+strlen(p)+3);
                        strcpy(sql,"select ename from employee where essn not in(select essn from work_on where pno = \"");
                        strcat(sql,p);
                        strcat(sql,"\")");
                    }else if(number == 6){
                        sql = malloc(strlen("select ename,dname from employee,department where employee.dno = department.dno and employee.superssn = (select essn from employee where ename = \"")+strlen(p)+3);
                        strcpy(sql,"select ename,dname from employee,department where employee.dno = department.dno and employee.superssn = (select essn from employee where ename = \"");
                        strcat(sql,p);
                        strcat(sql,"\")");
                    }else if(number == 8){
                        sql = malloc(strlen("select dname from employee,department where employee.dno = department.dno group by employee.dno having avg(salary)<")+strlen(p)+1);
                        strcpy(sql,"select dname from employee,department where employee.dno = department.dno group by employee.dno having avg(salary)<");
                        strcat(sql,p);
                    }
                }
                //two arguments
                else if(number == 4 || number == 7 || number ==9){

                    //split arguments
                    char *ps;
                    ps = strtok(p,",");
                    char *s1;
                    char *s2;
                    s1 = ps;
                    s2 = strtok(NULL,",");


                    if(number == 4){
                        sql = malloc(strlen("select ename,dname from employee,department where ename.dno = department.dno and dname = \"")+strlen(s1)+strlen("\" and salary<")+strlen(s2)+1);
                        strcpy(sql,"select ename,dname from employee,department where employee.dno = department.dno and dname = \"");
                        strcat(sql,s1);
                        strcat(sql,"\" and salary<");
                        strcat(sql,s2);
                    }else if(number == 7){
                        sql = malloc(strlen("select w1.essn from work_on w1,work_on w2 where w1.essn = w2.essn and w1.pno = \"")+strlen(s1)+strlen("\" and w2.pno = \"")+strlen(s2)+2);
                        strcpy(sql,"select w1.essn from work_on w1,work_on w2 where w1.essn = w2.essn and w1.pno = \"");
                        strcat(sql,s1);
                        strcat(sql,"\" and w2.pno = \"");
                        strcat(sql,s2);
                        strcat(sql,"\"");
                    }else if(number == 9){
                        sql = malloc(strlen("select ename from employee,work_on where employee.essn = work_on.essn group by work_on.essn having count(*)>")+strlen(s1)+strlen(" and sum(hours)<=")+strlen(s2)+1);
                        strcpy(sql,"select ename from employee,work_on where employee.essn = work_on.essn group by work_on.essn having count(*)>");
                        strcat(sql,s1);
                        strcat(sql," and sum(hours)<=");
                        strcat(sql,s2);
                    }
                }

                //out put sql
                printf(sql);
                printf("\n");

                //execute the query
                if(mysql_query(&mysql_conn,sql)==0){
                    mysql_result =  mysql_store_result(&mysql_conn);

                    //count row and column of query result
                    num_row = mysql_num_rows(mysql_result);
                    num_col = mysql_num_fields(mysql_result);

                    //output result
                    for (f1 = 0; f1 < num_row; f1++) {
                        mysql_row = mysql_fetch_row(mysql_result);

                        for (f2 = 0; f2 < num_col; f2++)
                            printf("[Row %d, Col %d] ==> [%s]\n", f1+1, f2+1, mysql_row[f2]);
                    }

                    //free
                    mysql_free_result(mysql_result);
                }else{
                    printf("Query fails\n");
                }


                free(sql);


                printf("=============================================================\n");
                printf("Connection success\nplease input your query\n");
                gets(command);
            }



        }else{
            printf("Connection fails.\n");
        }
    }else{
        printf("Initialization fails.\n");
        return -1;
    }

    mysql_close(&mysql_conn);
    return 0;

}
