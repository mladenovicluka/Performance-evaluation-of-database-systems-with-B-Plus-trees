# Performance-evaluation-of-database-systems-with-B-Plus-trees
Let the table CUSTOMER_ACCOUNT be given, representing the table from one standard benchmark for testing database performance. The structure of this table is as follows: (CA_ID, CA_B_ID, CA_C_ID, CA_NAME, CA_TAX_ST, CA_BAL). CA_ID is the primary key of the table (integer data), and an index is created on it. Parameters CA_B_ID, CA_C_ID, and CA_TAX_ST are administrative data related to the account (of integer type), CA_NAME represents the account name (string), and CA_BAL represents the account balance (real number).
The table is given by a text file that has the following structure (| represents the delimiter of the file):
CA_ID|CA_B_ID|CA_C_ID|CA_NAME|CA_TAX_ST|CA_BAL
