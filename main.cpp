// ICS/CSE 46 Summer 2015
// Project 3 
//
// This file parses the employee id file.  You will edit this to
// add the employees with their ids to your AVL tree, perform a lookup
// of an employee, and print the employees with their ids in sorted order.
//
#include <fstream>
#include "AVLTree.hpp"
#include <iostream>

int main() 
{
    std::string file_name = "data/employees.txt";
    std::ifstream in_file(file_name);
    AVLTree<std::string, std::string> tree;

    //Parse the employee names and idss out of the text file
    std::string line;
    while(std::getline(in_file,line)) 
    {
        // Gets the employees name
        std::string employee_name = line.substr(0,line.find(","));
        // Gets the employees id number
        std::string employee_id = line.substr(line.find(",")+1);
        tree.insert(employee_id, employee_name);
    }
    tree.check();
    tree.print();


    std::cout << "\n Looking up employee's name with 4616748508 id number\n using [] overloading and lookup function" << std::endl;
    std::cout << tree["4616748508"] << std::endl;
    std::cout << tree.lookup("4616748508") << std::endl;
    std::cout << "\n Looking up employee's name with 0077389069 id number\n using [] overloading and lookup function" << std::endl;
    std::cout << tree["0077389069"] << std::endl;
    std::cout << tree.lookup("0077389069") << std::endl;

    std::cout << "\n Using remove method to remove employee with 0077389069 id from this tree" << std::endl;
    tree.remove("0077389069");
    std::cout <<"Now key 0077389069 returns an empty string -> " << tree["0077389069"] << std::endl;

    return 0;
}
