#include<stdio.h>
#include<Windows.h>
#include<sql.h>
#include<sqlext.h>

bool DBConnect(); // Connect to the SQL Server
void DBDisconnect(); // Disconnect from the SQL Server

// SQL Environment Variables
SQLHENV hEnv = NULL;
SQLHDBC hDbc =NULL;

bool DBConnect()
{
	SQLRETURN Ret;

	// Allocate the Environment Handle (hEnv)
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
	{
		return false;
	}

	// Set attributes of the Environment Handle (hEnv)
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return false;
	}

	// Allocate the Connection Handle (hDbc)
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
	{
		return false;
	}

	// Connect to the SQL Sever with ODBC name, ID, and PW
	Ret = SQLConnect(hDbc, (SQLCHAR *)"DBKE", SQL_NTS, (SQLCHAR *)"DB02", SQL_NTS, (SQLCHAR *)"DB02", SQL_NTS);

	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
	{
		return false;
	}
	return true;
}

void DBDisconnect()
{
	SQLDisconnect(hDbc);					// Disconnect from the SQL Server
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);	// Free the Connection Handle
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);	// Free the Environment Handle
}

int main(int argc, char *argv[])
{
	if(DBConnect()==true)
	{
		printf("[Marine Mammal Laboratory]\n");
		printf("OCEAN(OCNAME, TEMPERATURE, AREA, SEAICE)\n"
			"MARINEMAMMAL(MAMNAME, CONSERVATION, FAMILY, GENUS)\n"
			"MAMFOOD(MNAME, FOOD)\n"
			"HABITAT(MNAME, ONAME)\n"
			"PROGRAM(PRONAME, BUDGET, MNAME)\n"
			"LOACTION(PNAME, ONAME)\n"
			"RESEARCHER(RESNUM, RESNAME, NATIONALITY, GENDER, HEADNUM, PNAME)\n"
			"EMERGENCY(RESNUM, ACNAME, PHONE, RELATION)\n");
		printf("\n");

		while(true)
		{
			//main code
			static SQLCHAR query[300];
			SQLHSTMT hStmt;

			SQLSMALLINT colCount=-1;//column 개수
			SQLCHAR data[100][100];//data 담을 2차원 char 배열
			SQLINTEGER nulldata[100];//null에 대한 data 담을 int 배열

			//Allocate the Statement Handle
			if( SQLAllocHandle( SQL_HANDLE_STMT, hDbc, &hStmt ) == SQL_SUCCESS )
			{
				int i;
				printf("1.SELECT 2.INSERT 3.UPDATE 4.DELETE 5.EXIT: ");
				int enter;
				scanf("%d",&enter);

				if(enter==1)//SELECT
				{
					printf(">>\n");
					printf("1 type in the query\n"
						"2 select the scenario\n");
					printf("--\n");
					printf("select number: ");
					int number;
					scanf("%d",&number);

					if(number==1)
					{
						while(getchar() != '\n');

						printf("query문 입력: ");
						char entq[100];
						gets_s(entq);
						printf(">>\n");
						// Execute a Query
						sprintf((char *) query,entq);
						SQLExecDirect( hStmt, query, SQL_NTS ); 
						SQLNumResultCols(hStmt, &colCount);
					}
					if(number==2)
					{
						printf(">>\n");
						printf("1 PROGRAM info\n"
							"2 RESEARCHER info\n"
							"3 HABITAT, SEAICE info using 2table join\n"
							"4 RESEARCHER, PROGRAM info using 2table join\n"
							"5 RESEARCHER, ACQUAINTANCE info using 2table join\n"
							"6 MARINEMAMMAL, HABITATS info using 3table join\n"
							"7 PROGRAM, MARINEMAMMAL info using 3table join\n");
						printf("--\n");
						printf("select number: ");
						int num;
						scanf("%d",&num);

						if(num==1)//SELECT PROGRAM
						{
							printf("input program name: ");
							char input[30];
							scanf("%s",input);
							printf("--PRONAME BUDGET MNAME--\n");
							// Execute a Query
							sprintf((char *) query,"SELECT * FROM PROGRAM WHERE PRONAME='%s';",input);
							SQLExecDirect( hStmt, query, SQL_NTS );
							SQLNumResultCols(hStmt, &colCount);
						}

						if(num==2)//SELECT RESEARCHER
						{
							printf("     1 researcher number\n     2 program name \n");
							printf("select number: ");
							int num;
							scanf("%d",&num);
							char input[30];

							if(num==1)//SELECT RESEARCHER-RESNUM
							{
								printf("input researcher number: ");
								scanf("%s",input);
								printf("--RESNUM RESNAME NATIONALITY GENDER HEADNUM PNAME--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT * FROM RESEARCHER WHERE RESNUM='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
							if(num==2)//SELECT RESEARCHER-PNAME
							{
								printf("input program name: ");
								scanf("%s",input);
								printf("--RESNUM RESNAME NATIONALITY GENDER HEADNUM PNAME--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT * FROM RESEARCHER WHERE PNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
						}

						if(num==3)//SELECT HABITAT OCEAN
						{
							printf("     1 mammal name \n     2 ocean name \n");
							printf("select number: ");
							int num;
							scanf("%d",&num);
							char input[30];

							if(num==1)//SELECT HABITAT OCEAN-MANME
							{
								printf("input mammal name: ");
								scanf("%s",input);
								printf("--MNAME OCNAME SEAICE--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT MNAME,OCNAME,SEAICE FROM HABITAT H,OCEAN O WHERE H.ONAME=O.OCNAME AND MNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
							if(num==2)//SELECT HABITAT OCEAN-ONAME
							{
								printf("input ocean name: ");
								scanf("%s",input);
								printf("--MNAME OCNAME SEAICE--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT MNAME,OCNAME,SEAICE FROM HABITAT H,OCEAN O WHERE H.ONAME=O.OCNAME AND OCNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
						}

						if(num==4)//SELECT RESEARCHER PROGRAM
						{
							printf("     1 researcher number \n     2 program name \n");
							printf("select number: ");
							int num;
							scanf("%d",&num);
							char input[30];

							if(num==1)//SELECT RESEARCHER PROGRAM-RESNUM
							{
								printf("input researcher number: ");
								scanf("%s",input);
								printf("--RESNUM RESNAME PRONAME BUDGET--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT RESNUM,RESNAME,PRONAME,BUDGET FROM RESEARCHER R,PROGRAM P WHERE R.PNAME=P.PRONAME AND RESNUM='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
							if(num==2)//SELECT RESEARCHER PROGRAM-PRONAME
							{
								printf("input program name : ");
								scanf("%s",input);
								printf("--RESNUM RESNAME PRONAME BUDGET--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT RESNUM,RESNAME,PRONAME,BUDGET FROM RESEARCHER R,PROGRAM P WHERE R.PNAME=P.PRONAME AND PRONAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
						}

						if(num==5)//SELECT RESEARCHER EMERGENCY-RESNUM
						{
							printf("input researcher number: ");
							char input[30];
							scanf("%s",input);
							printf("--RESNUM RESNAME PNAME ACNAME PHONE-\n");
							// Execute a Query
							sprintf((char *) query,"SELECT R.RESNUM,RESNAME,PNAME,ACNAME,PHONE FROM RESEARCHER R,EMERGENCY E WHERE R.RESNUM=E.RESNUM AND R.RESNUM='%s';",input);
							SQLExecDirect( hStmt, query, SQL_NTS );
							SQLNumResultCols(hStmt, &colCount);
						}

						if(num==6)//SELECT MARINEMAMMAL HABITAT OCEAN
						{
							printf("     1 mammal name \n     2 ocean name \n");
							printf("select number: ");
							int num;
							scanf("%d",&num);
							char input[30];

							if(num==1)//SELECT MARINEMAMMAL HABITAT OCEAN-MAMNAME
							{
								printf("input mammal name: ");
								scanf("%s",input);
								printf("--MAMNAME CONSERVATION OCNAME TEMPERATURE--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT MAMNAME,CONSERVATION,OCNAME,TEMPERATURE FROM MARINEMAMMAL MM,HABITAT H,OCEAN O WHERE (MM.MAMNAME=H.MNAME AND H.ONAME=O.OCNAME) AND MM.MAMNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
							if(num==2)//SELECT MARINEMAMMAL HABITAT OCEAN-OCNAME
							{
								printf("input ocean name : ");
								scanf("%s",input);
								printf("--MAMNAME CONSERVATION OCNAME TEMPERATURE--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT MAMNAME,CONSERVATION,OCNAME,TEMPERATURE FROM MARINEMAMMAL MM,HABITAT H,OCEAN O WHERE (MM.MAMNAME=H.MNAME AND H.ONAME=O.OCNAME) AND O.OCNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
						}

						if(num==7)//SELECT PROGRAM MARINEMAMMAL OCEAN
						{
							printf("     1 program name \n     2 mammal name \n     3 ocean name \n");
							printf("select number: ");
							int num;
							scanf("%d",&num);
							char input[30];

							if(num==1)//SELECT PROGRAM MARINEMAMMAL OCEAN-PRONAME
							{
								printf("input program name: ");
								scanf("%s",input);
								printf("--PRONAME MAMNAME OCNAME--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT PRONAME,H.MNAME,OCNAME FROM PROGRAM P,OCEAN O,HABITAT H WHERE (P.MNAME=H.MNAME AND H.ONAME=O.OCNAME) AND P.PRONAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
							if(num==2)//SELECT PROGRAM MARINEMAMMAL OCEAN-MAMNAME
							{
								printf("input mammal name : ");
								scanf("%s",input);
								printf("--PRONAME MAMNAME OCNAME--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT PRONAME,H.MNAME,OCNAME FROM PROGRAM P,OCEAN O,HABITAT H WHERE (P.MNAME=H.MNAME AND H.ONAME=O.OCNAME) AND H.MNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
							if(num==3)//SELECT PROGRAM MARINEMAMMAL OCEAN-ONAME
							{
								printf("input ocean name : ");
								scanf("%s",input);
								printf("--PRONAME MAMNAME OCNAME--\n");
								// Execute a Query
								sprintf((char *) query,"SELECT PRONAME,H.MNAME,OCNAME FROM PROGRAM P,OCEAN O,HABITAT H WHERE (P.MNAME=H.MNAME AND H.ONAME=O.OCNAME) AND O.OCNAME='%s';",input);
								SQLExecDirect( hStmt, query, SQL_NTS );
								SQLNumResultCols(hStmt, &colCount);
							}
						}
					}

					for(i=0; i<colCount; i++)
					{
						SQLBindCol(hStmt, i+1, SQL_C_CHAR, data[i], sizeof(data[i]), &nulldata[i]);
					}

					while( SQLFetch( hStmt) != SQL_NO_DATA)
					{
						for(i=0; i<colCount; i++)
						{
							if(nulldata[i]==SQL_NULL_DATA)
								printf("NULL     ");
							else
								printf(" %-8s",data[i]);
						}
						printf("\n");
					}
					printf("\n");
				}

				if(enter==2)//INSERT
				{
					printf(">>\n");
					printf("1 OCEAN \n"
						"2 MARINEMAMMAL, HABITAT \n"
						"3 RESEARCHER \n"
						"4 PROGRAM, LOCATION \n"
						"5 EMERGENCY \n");
					printf("--\n");
					printf("select number: ");
					int num;
					scanf("%d",&num);
					char input1[30],input2[30],input3[30],input4[30],input5[30],input6[30];

					if(num==1)//INSERT OCEAN
					{
						printf("--OCNAME TEMPERATURE AREA SEAICE--\n");
						printf("input : ");
						scanf("%s %s %s %s",input1,input2,input3,input4);
						// Execute a Query
						sprintf((char *) query,"INSERT INTO OCEAN VALUES('%s','%s','%s','%s')",input1,input2,input3,input4);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==2)//INSERT MARINEMAMMAL-HABITAT
					{
						printf("--MAMNAME CONSERVATION FAMILY GENUS--\n");
						printf("input : ");
						scanf("%s %s %s %s",input1,input2,input3,input4);
						printf("--ONAME--\n");
						printf("input : ");
						scanf("%s",input5);
						// Execute a Query
						sprintf((char *) query,"INSERT INTO MARINEMAMMAL VALUES('%s','%s','%s','%s')",input1,input2,input3,input4);
						SQLExecDirect( hStmt, query, SQL_NTS );
						static SQLCHAR query[300];
						sprintf((char *) query,"INSERT INTO HABITAT VALUES('%s','%s')",input1,input5);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==3)//INSERT RESEARCHER
					{
						printf("--RESNUM RESNAME NATIONALITY GENDER HEADNUM PNAME--\n");
						printf("input : ");
						scanf("%s %s %s %s %s %s",input1,input2,input3,input4,input5,input6);
						// Execute a Query
						if(strcmp(input5,"NULL")==0 && strcmp(input6,"NULL")==0)
						{
							sprintf((char *) query,"INSERT INTO RESEARCHER VALUES('%s','%s','%s','%s',NULL,NULL)",input1,input2,input3,input4);
							SQLExecDirect( hStmt, query, SQL_NTS );
						} 
						else if(strcmp(input5,"NULL")==0)
						{
							sprintf((char *) query,"INSERT INTO RESEARCHER VALUES('%s','%s','%s','%s',NULL,'%s')",input1,input2,input3,input4,input6);
							SQLExecDirect( hStmt, query, SQL_NTS );
						} 
						else if(strcmp(input6,"NULL")==0)
						{
							sprintf((char *) query,"INSERT INTO RESEARCHER VALUES('%s','%s','%s','%s','%s',NULL)",input1,input2,input3,input4,input5);
							SQLExecDirect( hStmt, query, SQL_NTS );
						} 
						else
						{
							sprintf((char *) query,"INSERT INTO RESEARCHER VALUES('%s','%s','%s','%s','%s','%s')",input1,input2,input3,input4,input5,input6);
							SQLExecDirect( hStmt, query, SQL_NTS );
						}
					}
					if(num==4)//INSERT PROGRAM-LOCATION
					{
						printf("--PRONAME BUDGET MNAME--\n");
						printf("input : ");
						scanf("%s %s %s",input1,input2,input3);
						printf("--ONAME--\n");
						printf("input : ");
						scanf("%s",input4);
						// Execute a Query
						sprintf((char *) query,"INSERT INTO PROGRAM VALUES('%s','%s','%s')",input1,input2,input3);
						SQLExecDirect( hStmt, query, SQL_NTS );
						static SQLCHAR query[300];
						sprintf((char *) query,"INSERT INTO LOCATION VALUES('%s','%s')",input1,input4);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==5)//INSERT EMERGENCY
					{
						printf("--RESNUM ACNAME PHONE RELATION--\n");
						printf("input : ");
						scanf("%s %s %s %s",input1,input2,input3,input4);
						// Execute a Query
						sprintf((char *) query,"INSERT INTO EMERGENCY VALUES('%s','%s','%s','%s')",input1,input2,input3,input4);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					printf("\n");
				}

				if(enter==3)//UPDATE
				{
					printf(">>\n");
					printf("1 OCEAN \n"
						"2 MARINEMAMMAL \n"
						"3 RESEARCHER \n"
						"4 PROGRAM\n"
						"5 EMERGENCY\n"
						"6 PROGRAM budget using join\n");
					printf("--\n");
					printf("select number: ");
					int num;
					scanf("%d",&num);
					char input1[30],input2[30],input3[30];

					if(num==1)//UPDATE OCEAN 
					{
						printf("input ocean name: ");
						scanf("%s",input1);
						printf("     1 temperature \n     2 seaice \n");
						printf("select number: ");
						int num;
						scanf("%d",&num);

						if(num==1)//UPDATE OCEAN-TEMPERATURE
						{
							printf("--TEMPERATURE--\n");
							printf("input : ");
							scanf("%s",input2);
							// Execute a Query
							sprintf((char *) query,"UPDATE OCEAN SET TEMPERATURE='%s' WHERE OCNAME='%s';",input2,input1);
							SQLExecDirect( hStmt, query, SQL_NTS );
						}
						if(num==2)//UPDATE OCEAN-SEAICE
						{
							printf("--SEAICEC--\n");
							printf("input : ");
							scanf("%s",input2);
							// Execute a Query
							sprintf((char *) query,"UPDATE OCEAN SET SEAICE='%s' WHERE OCNAME='%s';",input2,input1);
							SQLExecDirect( hStmt, query, SQL_NTS );
						}
					}
					if(num==2)//UPDATE MARINEMAMMAL-CONSERVATION
					{
						printf("input mammal name: ");
						scanf("%s",input1);
						printf("--CONSERVATION--\n");
						printf("input : ");
						scanf("%s",input2);
						// Execute a Query
						sprintf((char *) query,"UPDATE MARINEMAMMAL SET CONSERVATION='%s' WHERE MAMNAME='%s';",input2,input1);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==3)//UPDATE RESEARCHER
					{
						printf("input researcher number: ");
						scanf("%s",input1);
						printf("     1 headnum \n     2 pname \n");
						printf("select number: ");
						int num;
						scanf("%d",&num);

						if(num==1)//UPDATE RESEARCHER-HEADNUM
						{
							printf("--HEADNUM--\n");
							printf("input : ");
							scanf("%s",input2);
							// Execute a Query
							if(strcmp(input2,"NULL")==0)
							{
								sprintf((char *) query,"UPDATE RESEARCHER SET HEADNUM=NULL WHERE RESNUM='%s';",input1);
								SQLExecDirect( hStmt, query, SQL_NTS );
							} 
							else
							{
								sprintf((char *) query,"UPDATE RESEARCHER SET HEADNUM='%s' WHERE RESNUM='%s';",input2,input1);
								SQLExecDirect( hStmt, query, SQL_NTS );
							}
						}
						if(num==2)//UPDATE RESEARCHER-PNAME
						{
							printf("--PNAME--\n");
							printf("input : ");
							scanf("%s",input2);
							// Execute a Query
							if(strcmp(input2,"NULL")==0)
							{
								sprintf((char *) query,"UPDATE RESEARCHER SET PNAME=NULL WHERE RESNUM='%s';",input1);
								SQLExecDirect( hStmt, query, SQL_NTS );
							} 
							else
							{
								sprintf((char *) query,"UPDATE RESEARCHER SET PNAME='%s' WHERE RESNUM='%s';",input2,input1);
								SQLExecDirect( hStmt, query, SQL_NTS );
							}
						}
					}
					if(num==4)//UPDATE PROGRAM-BUDGET
					{
						printf("input program name: ");
						scanf("%s",input1);
						printf("--BUDGET--\n");
						printf("input : ");
						scanf("%s",input2);
						// Execute a Query
						sprintf((char *) query,"UPDATE PROGRAM SET BUDGET='%s' WHERE PRONAME='%s';",input2,input1);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==5)//UPDATE EMERGENCY-PHONE
					{
						printf("input researcher number and acquaintence name: ");
						scanf("%s %s",input1,input2);
						printf("--PHONE--\n");
						printf("input : ");
						scanf("%s",input3);
						// Execute a Query
						sprintf((char *) query,"UPDATE EMERGENCY SET PHONE='%s' WHERE RESNUM='%s' AND ACNAME='%s';",input3,input1,input2);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==6)//UPDATE PROGRAM AND RESEARCHER-BUDGET
					{
						printf("input researcher number: ");
						scanf("%s",input1);
						printf("--BUDGET INCREASE PERCENT--\n");
						float inputper;
						printf("input : ");
						scanf("%f",&inputper);
						// Execute a Query
						sprintf((char *) query,"UPDATE PROGRAM SET BUDGET=BUDGET*%f FROM RESEARCHER AS R WHERE R.PNAME=PROGRAM.PRONAME AND RESNUM='%s';",inputper,input1);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					printf("\n");
				} 

				if(enter==4)//DELETE
				{
					printf(">>\n");
					printf("1 MARINEMAMMAL \n"
						"2 RESEARCHER \n"
						"3 RPROGRAM \n"
						"4 EMERGENCY \n"
						"5 MAMFOOD \n");
					printf("--\n");
					printf("select number: ");
					int num;
					scanf("%d",&num);
					char input1[30],input2[30];

					if(num==1)//DELETE MARINEMAMMAL
					{
						printf("input mammal name \n");
						printf("input : ");
						scanf("%s",input1);
						// Execute a Query
						sprintf((char *) query,"DELETE FROM MARINEMAMMAL WHERE MAMNAME='%s';",input1);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==2)//DELETE RESEARCHER
					{
						printf("input researcher number \n");
						printf("input : ");
						scanf("%s",input1);
						// Execute a Query
						sprintf((char *) query,"DELETE FROM RESEARCHER WHERE RESNUM='%s';",input1);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==3)//DELETE PROGRAM
					{
						printf("input program name \n");
						printf("input : ");
						scanf("%s",input1);
						// Execute a Query
						sprintf((char *) query,"DELETE FROM PROGRAM WHERE PRONAME='%s';",input1);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==4)//DELETE EMERGENCY
					{
						printf("input researcher number and acquaintence name \n");
						printf("input : ");
						scanf("%s %s",input1,input2);
						// Execute a Query
						sprintf((char *) query,"DELETE FROM EMERGENCY WHERE RESNUM='%s' AND ACNAME='%s';",input1,input2);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					if(num==5)//DELETE MAMFOOD
					{
						printf("input mammal name and food name \n");
						printf("input : ");
						scanf("%s %s",input1,input2);
						// Execute a Query
						sprintf((char *) query,"DELETE FROM MAMFOOD WHERE MNAME='%s' AND FOOD='%s';",input1,input2);
						SQLExecDirect( hStmt, query, SQL_NTS );
					}
					printf("\n");
				}

				if(enter==5)//EXIT
					return 0;
			}
			// Codes for cleaning up
			SQLCloseCursor( hStmt );
			SQLFreeHandle( SQL_HANDLE_STMT, hStmt );
		}

		//Disconnect from the SQL Server
		DBDisconnect();
	}
	return 0;
}