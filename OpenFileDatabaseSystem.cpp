#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <iomanip>
using namespace std;

int make_database (char *get_dbname)
{	
	char create_db_name[100] = "mkdir ";
	strcat (create_db_name, get_dbname);
	int make_db = system (create_db_name);	
	if (make_db == -1)
		return 1;
	else
		return 0;
}

int del_database (char *get_dbname)
{
	char del_db_name[100] = "rm -rf ";
	strcat (del_db_name, get_dbname);
	int del_db = system (del_db_name);	
	if (del_db == -1)
		return 1;
	else
		return 0;
}

int list_database_table (char *get_dbname)
{
	char list_db_name[100] = "ls ";
	strcat (list_db_name, get_dbname);
	int list_db = system (list_db_name);	
	if (list_db == -1)
		return 1;
	else
		return 0;
}

int create_table (char *get_dbname)
{
	char make_db_name[100] = "touch ";
	strcat (make_db_name, get_dbname);
	char table_name[50];
	cout << "$db_ad Type Table Name : ";
	cin >> table_name;
	strcat (make_db_name, "/");
	strcat (make_db_name, table_name);
	int make_db = system (make_db_name);	
	if (make_db == -1)
		return 1;
	else
		return 0;
}

int create_multiple_table (char *get_dbname, char *data)
{
	char make_db_name[100];
	char **get_details = g_strsplit_set (data, "&", 2);
	char **split = g_strsplit_set (get_details[1], "+", -1);
	int length = g_strv_length (split);
	int i = 0;
	for (i=0;i<length;i++) {
		bzero (make_db_name, 100);
		strcat (make_db_name, "touch ");
		strcat (make_db_name, get_dbname);
		strcat (make_db_name, "/");
		strcat (make_db_name, split[i]);
		int make_db = system (make_db_name);	
		if (make_db == -1) {
			g_strfreev (split);
			g_strfreev (get_details);
			return 1;
		}
		else {
			continue;
		}
	}
	g_strfreev (split);
	g_strfreev (get_details);
	return 0;
}


int define_table_cols(char *get_dbtablename)
{
	int counter = 1;
	char ch, column_name[50], user_input[20];
	FILE *write_tuples = fopen (get_dbtablename, "w");
	if (NULL == write_tuples) {
		cout << "$db_ad Problem while opening the Table" << endl;
		fclose (write_tuples);
		return 0;
	}
	while (1) {
		bzero (column_name, 50);
		bzero (user_input, 20);
		cout << "$db_ad Adding " << counter << " column to the table" << endl;
		cout << "$db_ad ";
		cin >> user_input;
		strcat (column_name, "@");
		strcat (column_name, user_input); 
		fprintf (write_tuples, "%s\n", column_name);		
		counter++;
		cout << "$db_ad Do you want to exit -- 'y' to exit -- 'n' to add more columns" << endl;
		cout << "$db_ad ";
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			fprintf (write_tuples, "\n"); 
			break;
		}
	}
	fclose (write_tuples);
	return 0;	
}

//TODO ALTER TABLE START
int alter_table_cols(char *get_dbtablename)
{
	char existing_element[50], new_element[50];
	char element_to_find[50];
	char ch, schema_element[50];
	FILE *alter = fopen (get_dbtablename, "r");
	if (NULL == alter) {
		cout << "$db_ad Problem while opening the Table" << endl;
		fclose (alter);
		return 0;
	}
	cout << "$db_ad Enter Exisiting Element";
	cin >> existing_element;
	cout << endl;
	cout << "$db_ad Enter New Element";
	cin >> new_element;
	bzero (element_to_find, 50);
	strcat (element_to_find, "@");
	strcat (element_to_find, existing_element); 
	while (ch != EOF) {
		ch = fscanf (alter, "%s", schema_element);
		if (1 == ch) {
			gboolean value = g_str_has_prefix (schema_element, "@");
			if (TRUE == value) {
				cout << "$db_ad " << element_to_find << endl;
				if (strcmp(element_to_find, schema_element) == 0) {
					//int length = strlen (schema_element);
					FILE *update = fopen (get_dbtablename, "a");
					if (NULL == update) {
						cout << "$db_ad Problem while opening the Table" << endl;
						fclose (update);
						fclose (alter);
						return 0;																			   }
					bzero (element_to_find, 50);
					strcat (element_to_find, "@");
					strcat (element_to_find, new_element);
					cout << "$db_ad " << new_element << endl;
					fprintf (update, "%s\n", new_element);
					fclose (update);
				}
			}
			if (FALSE == value) {
				continue;
			}
		}		
	}
fclose (alter);
return 0;
}
//TODO END OF ALTER 

int check_fields(char *get_dbtablename)
{
	int counter = 1;
	char ch, field[50];
	FILE *check_field = fopen (get_dbtablename, "r");		
	if (NULL == check_field) {
		cout << "$db_ad Problem while opening the Table" << endl;
		fclose (check_field);
		return 0;
	}
	while (ch != EOF) {
		ch = fscanf (check_field, "%s", field);
		if (1 == ch) {
			gboolean value = g_str_has_prefix (field, "@");
			if (TRUE == value) {
				counter++;
			}
			if (FALSE == value) {
				return counter;
			}
		}
	}
	fclose (check_field);
	return counter;
}

int insert_data(char *get_dbtablename)
{
	int i, counter = 1, check_field = 0;
	char ch, insert_data[100];
	char data[20] = "-next";
	FILE *insert = fopen(get_dbtablename, "a");
	if (NULL == insert) {
		cout << "$db_ad Problem while opening the Table" << endl;
		fclose (insert);
		return 0;
	}
	while (1) {
		check_field = check_fields(get_dbtablename);
		if (check_field == 1) {
			cout << "$db_ad Table does not have any columns/fields defined" << endl; 
			fclose (insert);
			return 0;
		}
		while (i < check_field-1) {
			bzero (insert_data, 100);
			fflush (insert);
			cout << "$db_ad Inserting data at Column " << counter << " of the Table" << endl;
			cout << "$db_ad ";
			fgets (insert_data, 100, stdin);
			fprintf (insert, "%s", insert_data);
			bzero (insert_data, 100);
			fflush (insert);
			counter++;
			i++;
		}
		fprintf (insert, "%s\n", data);
		fflush (insert);
		cout << "$db_ad Do you want to exit -- 'y' to exit -- 'n' to add more data" << endl;
		cout << "$db_ad ";
		cin >> ch;
		if (ch == 'y' || ch == 'Y') {
			break;
		}
		else {
			i = 0;
			counter = 1;
			check_field = 0;
			continue;
		}
	}
	fclose (insert);
	return 0;
}

int insert_multiple_data(char *get_dbtablename, char *data)
{
	char next_data[20] = "-next";
	char **split_data = g_strsplit_set (data, "&", 2);
	FILE *insert_multiple_data = fopen (get_dbtablename, "r");
	if (NULL == insert_multiple_data) {
		cout << "$db_ad Problem while opening the Table" << endl;
		g_strfreev (split_data);
		fclose (insert_multiple_data);
		return 0;
	}
	FILE *insert_data = fopen (get_dbtablename, "a");
	if (NULL == insert_data) {
		cout << "$db_ad Problem while opening the Table" << endl;
		g_strfreev (split_data);
		fclose (insert_data);
		fclose (insert_multiple_data);
		return 0;
	}
	int value = check_fields (get_dbtablename);
	if (value == 1) {
		cout << "$db_ad Table does not have any columns/fields defined" << endl; 
		fclose (insert_data);
		fclose (insert_multiple_data);
		g_strfreev (split_data);	
		return 0;
	}
	int j = 0;
	if (strstr(split_data[1], "+") == NULL) {
		char **no_symbol_split = g_strsplit_set (split_data[1], ",", -1);
		fflush (insert_data);
		int length = g_strv_length (no_symbol_split);
		for (j=0;j<length;j++) {
			fprintf (insert_data, "%s\n", no_symbol_split[j]);
		}
		fprintf (insert_data, "%s\n", next_data);
		g_strfreev (no_symbol_split);
		fclose (insert_data);	
		fclose (insert_multiple_data);
		g_strfreev (split_data);
		return 0;
	}
	char **split = g_strsplit_set (split_data[1], "+", -1);
	char **str;
	int len = g_strv_length (split);
	int i = 0;	
	for (i=0;i<len;i++) {
		fflush (insert_data);
		str = g_strsplit_set (split[i], ",", -1);
		int length = g_strv_length (str);
		for (j=0;j<length;j++) {
			fprintf (insert_data, "%s\n", str[j]);
		}
		fprintf (insert_data, "%s\n", next_data);
		g_strfreev (str);
	}
	fclose (insert_data);
	g_strfreev (split);	
	return 0;
}

int insert_multiple_cols (char *get_dbtablename, char *data)
{
	char **split_data = g_strsplit_set (data, "&", 2);
	FILE *insert_multiple_cols = fopen (get_dbtablename, "r");
	if (NULL == insert_multiple_cols) {
		cout << "$db_ad Problem while opening the Table" << endl;
		g_strfreev (split_data);
		fclose (insert_multiple_cols);
		return 0;
	}
	FILE *insert_cols = fopen (get_dbtablename, "w");
	if (NULL == insert_cols) {
		cout << "$db_ad Problem while opening the Table" << endl;
		g_strfreev (split_data);
		fclose (insert_cols);
		fclose (insert_multiple_cols);
		return 0;
	}
	if (strstr(split_data[1], "+") == NULL) {
		fflush (insert_cols);
		fprintf (insert_cols, "@%s\n", split_data[1]);
		fclose (insert_cols);	
		fclose (insert_multiple_cols);
		g_strfreev (split_data);
		return 0;
	}
	char **split = g_strsplit_set (split_data[1], "+", -1);
	int len = g_strv_length (split);
	int i = 0;
	for (i=0;i<len;i++) {
		fflush (insert_cols);
		fprintf (insert_cols, "@%s\n", split[i]);
	}
	fclose (insert_cols);
	g_strfreev (split);	
	return 0;
}

int check_table (char *get_dbname)
{
	if (strstr(get_dbname, "/") != NULL) {
		FILE *check = fopen (get_dbname, "r");
		if (NULL == check) {
			cout << "$db_ad Table doesn't exist" << endl;	
			return 0;
		}
		else {
			fclose (check);
			return 2;
		}
	}
	char db_table_name[50];
	char **split = g_strsplit_set (get_dbname, ".", 2);
	bzero (db_table_name, 50);
	strcat (db_table_name, split[0]);
	strcat (db_table_name, "/");
	strcat (db_table_name, split[1]);
	FILE *check = fopen (db_table_name, "r");
	if (NULL == check) {
		cout << "$db_ad Table doesn't exist" << endl;	
		g_strfreev (split);
		return 0;
	}
	else { 
		g_strfreev (split);
		return 2;
	}
	fclose (check);
}

int get_all_table_value (char *get_dbtablename)
{
	char ch, value[100];
	FILE *get_value = fopen (get_dbtablename, "r");
	if (NULL == get_value) {
		cout << "$db_ad Problem while opening the Table" << endl;
		return 0;
	}

	while (ch != EOF) {
		ch = fscanf (get_value, "%s", value);
		if (TRUE == g_str_has_prefix (value, "@")) 
			continue;
		else if (strcmp (value, "-next") == 0) {
			cout << endl;
			continue;
		}
		else 
			cout << value << " ";
	}
	fclose (get_value); 
	return 0;

}
int get_table_value (char *get_dbtablename)
{
	int fields;
	char name[20];
	char bh, ch, value[100], new_value[100];
	FILE *get_value = fopen (get_dbtablename, "r");
	if (NULL == get_value) {
		cout << "$db_ad Problem while opening the Table" << endl;
		return 0;
	}
	cout << "$db_ad ------------------------------SEARCH OPTIONS----------------------------------------" << endl;
	cout << "$db_ad Type in the 'PRIMARY KEY STRING'  OR  type in 'all' or '*' to show complete database" << endl;
	cout << "$db_ad ------------------------------------------------------------------------------------" << endl;
	cout << "$db_ad ";
	cin >> name; 
	if (strcmp(name, "all") == 0 || strcmp(name, "All") == 0 || strcmp(name, "*") == 0) {
		while (ch != EOF) {
		ch = fscanf (get_value, "%s", value);
		if (TRUE == g_str_has_prefix (value, "@")) 
			continue;
		else if (strcmp (value, "-next") == 0) {
			cout << endl;
			continue;
		}
		else
			cout << value << " ";

		} 
		return 0;
	}
	fields = check_fields (get_dbtablename);
	while (ch != EOF) {
		ch = fscanf (get_value, "%s", value);
		if (TRUE == g_str_has_prefix (value, "@")) 
			continue;
		if (strcmp (value, "-next") == 0)
			continue;

		if (strcmp(name, value) == 0) {
			while (bh != EOF) {
				bh = fscanf (get_value, "%s", new_value);
				if (strcmp (new_value, "-next") == 0)
					return 0;
				cout << new_value << endl;		
			}	
		}	
	} 
	fclose (get_value);
	return 0;
}

char *make_path (char *get_dbname)
{
	char *db_table_name = g_new (char, 50);
	char **split = g_strsplit_set (get_dbname, ".", 2);
	bzero (db_table_name, 50);
	strcat (db_table_name, split[0]);
	strcat (db_table_name, "/");
	strcat (db_table_name, split[1]);
	g_strfreev (split);
	return db_table_name; 
}

char *get_path_with_data (char *get_dbname)
{
	char *db_table_name = g_new (char, 50);
	char **path = g_strsplit_set (get_dbname, "&", 2);
	char **split = g_strsplit_set (path[0], ".", 2);
	bzero (db_table_name, 50);
	strcat (db_table_name, split[0]);
	strcat (db_table_name, "/");
	strcat (db_table_name, split[1]);
	return g_strdup (db_table_name); 
}

char *get_db_path (char *get_db)
{
	char **get_db_new_name = g_strsplit_set (get_db, "&", 2);
	return g_strdup (get_db_new_name[0]);
} 

int handle_command (char *command)
{
	int value;
	char **command_split = g_strsplit_set (command, ":", 2);
	if (strcmp(command_split[0], "create") == 0) {
		value = make_database (command_split[1]);
		if (0 == value) 
		{
			g_strfreev (command_split);
			return 0;
		}
	}
	if (strcmp(command_split[0], "delete") == 0) {
		value = del_database (command_split[1]);
		if (0 == value)
		{
			g_strfreev (command_split); 
			return 0;
		}
	}
	if (strcmp(command_split[0], "list") == 0) {
		value = list_database_table (command_split[1]);
		if (0 == value)
		{
			g_strfreev (command_split);
			return 0;
		}
	}
	if (strcmp(command_split[0], "createtable") == 0) {
		if ((strstr(command_split[1], "&") != NULL && strstr(command_split[1], "+") !=NULL) || (strstr(command_split[1], "&") != NULL)) {
			char *get_db = get_db_path (command_split[1]);
			value = create_multiple_table (get_db, command_split[1]);
			if (0 == value) {
				g_free (get_db);
				g_strfreev (command_split);
				return 0;	
			}
		}
		else {
			value = create_table (command_split[1]);
			if (0 == value) {
				g_strfreev (command_split);
				return 0;
			}
		}
	}
	if (strcmp(command_split[0], "createfield") == 0) {
		if (strstr(command_split[1], ".") == NULL) {
			g_strfreev (command_split);
			return 1;
		}
		if ((strstr(command_split[1], "&") != NULL && strstr(command_split[1], "+") !=NULL) || (strstr(command_split[1], "&") != NULL)) {
			char *get_path = get_path_with_data (command_split[1]);
			value = check_table (get_path);	
			if (2 == value) {
				insert_multiple_cols (get_path, command_split[1]);
				g_free (get_path);
				g_strfreev (command_split);
				return 0;
			}
			else if (0 == value) {
				g_free (get_path);
				g_strfreev (command_split);
				return 0;	
			}

		}
		else {
			value = check_table (command_split[1]);
			if (2 == value) {
				char *path = make_path(command_split[1]);
				define_table_cols(path); 		
				return 0; 
			}
			else if (0 == value) {
				g_strfreev (command_split);
				return 0;
			}
		}
	}
	if (strcmp(command_split[0], "insert") == 0) {
		if (strstr(command_split[1], ".") == NULL) {
			g_strfreev (command_split);
			return 1;
		}
		if ((strstr(command_split[1], "&") != NULL && strstr(command_split[1], "+") !=NULL) || (strstr(command_split[1], "&") != NULL)) {
			char *get_path = get_path_with_data (command_split[1]);
			value = check_table (get_path);	
			if (2 == value) {
				insert_multiple_data (get_path, command_split[1]);
				g_free (get_path);
				g_strfreev (command_split);
				return 0;
			}
			else if (0 == value) {
				g_free (get_path);
				g_strfreev (command_split);
				return 0;	
			}
		}
		else 	{
			value = check_table (command_split[1]);			
			if (2 == value) {
				char *path = make_path(command_split[1]);
				insert_data(path); 		
				g_strfreev (command_split);
				return 0; 
			}
			else if (0 == value) {
				g_strfreev (command_split);
				return 0;
			}
		}
	}
	if (strcmp(command_split[0], "search") == 0) {
		if (strstr(command_split[1], ".") == NULL) {
			g_strfreev (command_split);
			return 1;
		}
		value = check_table (command_split[1]);
		if (2 == value) {
			char *path = make_path(command_split[1]);
			get_table_value(path); 		
			return 0; 
		}
		if (0 == value)
		{
			g_strfreev (command_split);
			return 0;
		}
	}
	if (strcmp(command_split[0], "searchall") == 0 || strcmp(command_split[0], "search*") == 0) {
		if (strstr(command_split[1], ".") == NULL) {
			g_strfreev (command_split);
			return 1;
		}
		value = check_table (command_split[1]);
		if (2 == value) {
			char *path = make_path(command_split[1]);
			get_all_table_value(path); 		
			return 0; 
		}
		else if (0 == value)
		{
			g_strfreev (command_split);
			return 0;
		}
	}
	//TODO Alter Start
	if (strcmp(command_split[0], "alter") == 0) {
		if (strstr(command_split[1], ".") == NULL) {
			g_strfreev (command_split);
			return 1;
		}
		value = check_table (command_split[1]);
		if (2 == value) {
			//char *path = make_path(command_split[1]);
			//alter_table_cols(path); 		
			cout << "$db_ad Currently this command is under construction" << endl;
			return 0; 
		}
		if (0 == value)
		{
			g_strfreev (command_split);
			return 0;
		}
	}
	//TODO Alter End
	g_strfreev (command_split);
	return 1;
}

void help()
{
	cout <<"-----------------------------------------HELP---------------------------------------" << endl;
	cout <<"1. Creating a Database" << endl;
	cout <<"	create:database_name" << endl;
	cout <<"2. Deleting a Database" << endl;
	cout <<"	delete:database_name" <<endl;
	cout <<"3. Creating a Table" << endl;
	cout <<"	3.1 createtable:database_name&database_table_1+database_table_2+..." << endl;
	cout <<"		'&' seperates the database name." << endl;
	cout <<"		'+' seperates the database tables. " << endl;
	cout <<"	3.2 createtable:database_name" << endl;
	cout <<"		It will prompt you to enter the Table" << endl;
	cout <<"		name to be created inside the database" << endl;
	cout <<"4. Viewing a Database table-wise" << endl;
	cout <<"	list:database_name" << endl;
	cout <<"5. Creating Fields in Tables"<< endl;
	cout <<"	5.1 createfield:database_name.table_name&field_1+field_2+... " << endl;
	cout <<"		'&' seperates the command from the data." << endl;
	cout <<"		'+' sepeartes the tuples in the database table. " << endl;
	cout <<"	5.2 createfield:database_name.table_name" << endl;
	cout <<" 		It will prompt to enter the Field Names Interactively." << endl;
	cout <<"6. Inserting data to a Table" << endl;
	cout <<"	6.1 insert:database_name.table_name&field_data_1+field_data_2+..." << endl;
	cout <<"		'&' seperates the command from the data." << endl;
	cout <<"		'+' sepeartes the tuples in the database table. " << endl;
	cout <<"	6.2 insert:database_name.table_name" << endl;
	cout <<"		It will prompt to enter the Tuples Interactively" << endl;
	cout <<"7. Searching the Table" << endl;
	cout <<"	7.1 search using a PRIMARY KEY" << endl;
	cout <<"		7.1.1 search:database_name.table_name" << endl;
	cout <<"			It will prompt to enter the PRIMARY KEY to search" << endl;
	cout <<"			the particular data." << endl;
	cout <<"	7.2 searching the complete table" << endl;
	cout <<"		7.1.2 searchall:database_name.table_name OR" << endl; 
	cout <<"			search*:database_name.table_name" << endl;
	cout <<"			It will print the complete database" << endl;
	cout <<"------------------------------------------------------------------------------------" << endl;
}

void author()
{
	cout <<"--------------------AUTHOR-----------------------" << endl;
	cout <<"	    	Abhijeet Das		 " << endl;
	cout <<"-------------------------------------------------" << endl;
}

int main (int argc, char *argv[])
{
	char command[1024];
	int val;
	cout << "Open File Database System 2016. 'help' for instruction. 'author' for author's details" << endl;
	while (1)
	{
		cout << "$db_ad ";
		fgets (command, 1024, stdin);
		int len = strlen (command);
		command[len-1] = '\0';
		if (strcmp(command, "help") == 0) {
			help();
			continue;
		}
		if (strcmp(command, "author") == 0) {
			author();
			continue;
		}
		if (strcmp(command, "clear") == 0) {
			system("clear");
			continue;
		}
		if (strcmp(command, "exit") == 0 || strcmp(command, "EXIT") == 0) {
			exit(0);
		}	
		if (strstr(command, ":") == NULL) {
			cout << "$db_ad Unknown command. 'help' for more instructions";
			cout << endl;
			continue;
		} 
		val = handle_command (command);
		if (0 == val)
			continue;
		if (1 == val)
			cout << "$db_ad Unknown command. 'help' for more instructions";
		cout << endl;
	}
	return 0;
}
