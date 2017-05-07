# OpenFileDatabaseSystem

#Build with glib.h

#Now Supports basic commands:

1. Create Databases
2. Create Tables
3. Insert Data
4. Search Data
5. Remove Tables from particular database.
6. Remove Databases


Command List:

1. Creating a Database 
	create:database_name 
2. Deleting a Database 
	delete:database_name
3. Creating a Table 
	3.1 createtable:database_name&database_table_1+database_table_2+... 
		'&' seperates the database name. 
		'+' seperates the database tables.  
	3.2 createtable:database_name 
		It will prompt you to enter the Table 
		name to be created inside the database 
4. Viewing a Database table-wise 
	list:database_name 
5. Creating Fields in Tables
	5.1 createfield:database_name.table_name&field_1+field_2+...  
		'&' seperates the command from the data. 
		'+' sepeartes the tuples in the database table.  
	5.2 createfield:database_name.table_name 
	 	It will prompt to enter the Field Names Interactively. 
6. Inserting data to a Table 
	6.1 insert:database_name.table_name&field_data_1+field_data_2+... 
		'&' seperates the command from the data. 
		'+' sepeartes the tuples in the database table.  
	6.2 insert:database_name.table_name 
		It will prompt to enter the Tuples Interactively 
7. Searching the Table 
	7.1 search using a PRIMARY KEY 
		7.1.1 search:database_name.table_name 
			It will prompt to enter the PRIMARY KEY to search 
			the particular data. 
	7.2 searching the complete table 
		7.1.2 searchall:database_name.table_name OR  
			search*:database_name.table_name 
			It will print the complete database 
        
        
        
