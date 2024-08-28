/* Program name: main.cpp
*  Author: Justin Phillips
*  Date last updated: 7/28/2024
* Purpose: Final DB project
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "sqlite3.h"
#include <cstring>
#include <cstdio>
#include <ctime>

using namespace std;

//All of our functions to make main less messy
static int callback(void *data, int argc, char **argv, char **azColName);
int menu();
string add();
string update();
string deleteMenu();
string transaction(string purchID);
string catalog();
string Users();
string strQuoteSql(const string& s);
void callbackErr(int conn);
string getDate();

int main(){
    //Our main pointers to db file
    sqlite3 *db;
    int conn, choice = 0;
    string sql;
    const char* data = "Open";

    //We initially test our connection to the DB to make sure it opens correctly
    conn = sqlite3_open("bullseye.db", &db);

    //Exits program if cannot connect, else prints that we connected
    if(conn){
        fprintf(stderr, "Error in connection: unable to open database file", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Welcome to bullseye");
    }

    while(choice != -1){
        //Our standard menu function for when a query is accomplished
       choice = menu();

       switch(choice){
        case 1:
            //First we set our sql string to our functions return (they are all strings so return a string query directly to sql)
            sql = add();
            //We execute our current query. sql.c_str() converts to c string to accomplish this. We set our return value to conn
            conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);
            //Our return value is then tested to see if the database executed the query correctly or not
            callbackErr(conn);
            break;
        case 2:
            //Similar to our case 1
            sql = update();
            conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);
            callbackErr(conn);
            break;
        case 3:
            //Similar to case 1, however we check use input to see if we actually want to delete something or not
            sql = deleteMenu();
            if(sql == "n"){
                break;
            }else{
                conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);
                callbackErr(conn);
                break;
            }
        //Here we have our logic in the case itself
        case 4:{
            int id = 0;

            cout << "\nThis menu is used to process a refund. Please enter a customer ID to process a refund: ";
            cin >> id;
            string custID = to_string(id);

            //Selects our values so the customer can choose which purchase they want to refund
            string sql = "SELECT purchase_id, purchase_date, total_amount FROM purchase WHERE customer_id = " + custID;
            int conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);

            if(conn != SQLITE_OK){
                fprintf(stderr, "SQL error: %s\n", "Error in connection: unable to open database file");
                exit(0);
            }else{
                fprintf(stdout, "\nDatabase shown successfully");
            }

            //Once they choose the purchase id to refund, we compare that to our inventory id and update our revenue accordingly
            cout << "\nPlease select the purchase_id you wish to refund: ";
            cin.ignore(256, '\n');
            cin >> id;
            string purchID = to_string(id);

            sql = transaction(purchID);
            conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);
            callbackErr(conn);
            break;
        }
        case 5:
            sql = catalog();
            conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);
            callbackErr(conn);
            break;
        case 6:
            sql = Users();
            conn = sqlite3_exec(db, sql.c_str(), callback, (void*)data, 0);
            callbackErr(conn);
            break;
        case -1:
            sqlite3_close(db);
            exit(0);
        default:
            break;
       }


    }

    sqlite3_close(db);
}

//Callback function for executing our queries
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   
   //Prints out the column given all arguments from SQL syntax
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");
   return 0;
}

//Our main menu
int menu(){
    int choice = 0;

    cout << "\nPlease choose a menu option (press -1 to exit)" << endl;
    cout << "1. Add to table" << endl;
    cout << "2. Update table" << endl;
    cout << "3. Delete from table" << endl;
    cout << "4. Make a transaction" << endl;
    cout << "5. View catalog" << endl;
    cout << "6. Find a products popularity" << endl;

    cin >> choice;

    return choice;
}

//This adds a new customer or stock to our database
string add(){
    int choice = 0;

    cout << "\n1. Add stock" << endl;
    cout << "2. Add customer" << endl;

    def:
    cin >> choice;
    cin.ignore(256,'\n');

    switch(choice){
        case 1:{
            string brand;
            int quantity;
            int merchant;

            cout << "\nEnter the brand name: ";
            getline(cin, brand);

            cout << "Enter the quantity to stock: ";
            cin >> quantity;
            string number = to_string(quantity);

            cout << "Enter the merchant code for this product: ";
            cin >> merchant;
            string merch = to_string(merchant);

            string date = getDate();

            //Formated query
            return "INSERT INTO inventory (merchant_code, brand_name, quantity, date_last_received) VALUES (" 
            + strQuoteSql(merch) + ","
            + strQuoteSql(brand) + ","
            + strQuoteSql(number) + ","
            + strQuoteSql(date) + ");";
        }
        case 2:{
            string name;
            string email;
            string phone;
            string address;

            cout << "\nEnter customers name: ";
            getline(cin, name);

            cout << "\nEnter customers email: ";
            getline(cin, email);

            cout << "\nEnter customers phone number: ";
            getline(cin, phone);

            cout << "\nEnter customers address: ";
            getline(cin, address);

            //Our formatted query
            return "INSERT INTO customer (customer_name, email, phone_number, address) VALUES ("
            + strQuoteSql(name) + ","
            + strQuoteSql(email) + ","
            + strQuoteSql(phone) + ","
            + strQuoteSql(address) + ");";
        }

        default:
            cout << "\nPlease select choice 1 or 2";
            cin.ignore(256, '\n');
            cin.clear();
            goto def;  
    }    
}

//Our update function for our db for stock and customer, similarly setup to the add function
string update(){
    int choice = 0;

    cout << "\n1. Update stock" << endl;
    cout << "2. Update customer information" << endl;

    def:
    cin >> choice;
    cin.ignore(256,'\n');

    switch(choice){
        case 1:{
            string brand;
            int quantity;
            int merchant;

            cout << "\nEnter the merchant code to update stock: ";
            cin >> merchant;
            string merch = to_string(merchant);

            cout << "Enter the new quantity of stock: ";
            cin >> quantity;
            string number = to_string(quantity);

            string date = getDate();

            return "UPDATE inventory\nSET quantity = " + number + "," + "\ndate_last_received = " + strQuoteSql(date) + "\nWHERE merchant_code = " + merch;
        }
        case 2:{
            string name;
            string email;
            string phone;
            string address;
            int id;

            cout << "\nEnter customers ID: ";
            cin >> id;
            string customer = to_string(id);
            cin.ignore(256,'\n');

            cout << "\nEnter customers name: ";
            getline(cin, name);

            cout << "\nEnter customers email: ";
            getline(cin, email);

            cout << "\nEnter customers phone number: ";
            getline(cin, phone);

            cout << "\nEnter customers address: ";
            getline(cin, address);

            cout << "UPDATE customer\nSET customer_name = " + strQuoteSql(name) 
            + "," + "\nemail = " + strQuoteSql(email) + "," 
            + "\nphone_number = " + strQuoteSql(phone) + "," 
            + "\naddress = " + strQuoteSql(address) 
            + "\nWHERE customer_id = " + customer;

            return "UPDATE customer\nSET customer_name = " + strQuoteSql(name) 
            + "," + "\nemail = " + strQuoteSql(email) + "," 
            + "\nphone_number = " + strQuoteSql(phone) + "," 
            + "\naddress = " + strQuoteSql(address) 
            + "\nWHERE customer_id = " + customer;
        }

        default:
            cout << "\nPlease select choice 1 or 2";
            cin.ignore(256, '\n');
            cin.clear();
            goto def;  
    }  
}

//This function is used to delete a product from the database in case of it no longer being carried
string deleteMenu(){
    char choice;

    cout << "\nPlease use this menu to remove products from the database if they are no longer being stoocked." << endl;
    cout << "Would you like to remove a product from the database? (Y/N): ";

    cin >> choice;

    if(choice == 'y' || choice == 'Y'){
        cout << "Please enter the product code for deletion: ";
        int product;
        cin >> product;
        string prodCode = to_string(product);

        return "DELETE FROM department WHERE product_code = " + prodCode;

    }else{
        return "n";
    }
}

//Our transactional query for SQL, with logic in main. We are updating the hit to revenue given our refund and then removing that purchase from the db
//Updates revenue table, and deletes from purchase
string transaction(string purchID){
    return "UPDATE revenue\nSET inventory_cost = inventory_cost + (SELECT total_amount FROM purchase WHERE purchase_id = " + purchID + ")"
    + "\nWHERE item_number = (SELECT item_number FROM purchase WHERE purchase_id = " + purchID + ");"
    + "\nDELETE FROM purchase WHERE purchase_id = " + purchID;
}

//Displays extra department related information for our inventory (joins inventory + department)
string catalog(){
    int id;
    cout << "\nEnter the product code to see more information: ";

    cin >> id;
    string prodID = to_string(id);

    return "SELECT * FROM inventory INNER JOIN department ON inventory.product_code = department.product_code WHERE inventory.product_code = " + prodID;

}

//Function for showing us the amount of users the purchased a particular item from our revenue table (joins revenue + purchase)
string Users(){
    int id;
    cout << "\nEnter the inventory ID to display the amount of unique purchases: ";
    cin >> id;
    string invID = to_string(id);

    return "SELECT COUNT(*) FROM purchase INNER JOIN revenue ON purchase.item_number = revenue.item_number WHERE revenue.item_number = " + invID;
}

//This auto converts our string inputs to a proper query format
string strQuoteSql(const string& s){
    return string("'") + s + string("'");
}

//This function tells us if our query executed successfully. If not then we exit the program
void callbackErr(int conn){
    if(conn != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", "Error in connection: unable to open database file");
        exit(0);
    }else{
        fprintf(stdout, "\nDatabase updated successfully");
    }
}

//This function returns are date and time in the given format year-month-day hour:minute:second
string getDate(){
    time_t raw;
    tm* timeinfo;
    char buffer [80];

    time(&raw);
    timeinfo = localtime(&raw);
    strftime(buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);

    return buffer;
}