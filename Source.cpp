//
//  main.cpp
//  Project_Data_Structure
//
//  Created by Musa Dildar Ahmed Cheema on 11/28/20.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include "Structure.h"
#include "Btree.h"
#include <cstring>
#include<stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <ctime>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
struct arrs {
    string data;
    int id = 0;
    arrs* next;
};
class Array {
private:
    arrs* a = NULL;
    int inn = 0;
public:
    void insert(string ss) {
        if (a == NULL) {
            a = new arrs;
            a->data = ss;
            a->id = 0;
            a->next = NULL;
            inn++;
            return;
        }
        else {  
            arrs* b = new arrs;
            b->next = a;
            b->data = ss;
            b->id = a->id + 1;
            a = b;
            inn++;
        }
    }
    int getin() {

        return inn;
    }

    string getval(int i) {
        arrs* b = new arrs;
        b = a;
        while (b != NULL) {
            if (b->id == i) {
                break;
            }
            b = b->next;
        }
        if (b == NULL ) {
            return "";
        }
        return b->data;
    }

    void changeval(string ss , int i) {
        arrs* b = a;

        while ( b != NULL) {
            if (b->id == i) {
                break;
            }
            b = b->next;
        }
        if (b != NULL) {
            b->data = ss;
        }
    }
};
class Directory {
private:
    string currDir;
public:


    Directory(const char* argv[]) {
        std::string cur_dir(argv[0]);
        int pos = cur_dir.find_last_of("/\\");
        currDir = cur_dir.substr(0, pos);
    }
    Directory() {
        //
    }
    void setDir(const char* argv[]) {
        std::string cur_dir(argv[0]);
        int pos = cur_dir.find_last_of("/\\");
        currDir = cur_dir.substr(0, pos);
    }
    string getDir() {
        return currDir;
    }
};

class writesubfiles {
private:
    Directory a;
public:
    writesubfiles(const char* argv[]) {
        a.setDir(argv);
    }
    string write_file(string input, string store, string id) {
        string s = "mkdir Database\\" + id;
        //system(s.c_str());
        s =  "Database\\" + id + "\\" + input +".txt" ;
        
        const char* dirr = s.c_str();
        ofstream myfile;
        myfile.open(s,ios::app);
        myfile << store;
        myfile << "%";
        myfile.close();
        return s;
    }
    string getDir() {
        return a.getDir();
    }
};


int main(int argc, const char* argv[]) {
    srand(NULL);
    csv_reading<string> a;
    Bplustree<string> bpt;
    SetConsoleTextAttribute(console, 10);
    cout << "Input the Maximum Number of Order" << endl;
    cin >> max_allowed;
    ifstream openfile("_log.txt");
    string s;
    if (openfile) {
        system("cls");
        cout << "LogFile was Found this will save your time and don't need to Populate the DataBase Again" << endl;
        string csvv;
        getline(openfile, csvv);
        a.read_csv(csvv);
        Sleep(1500);
        while (!openfile.eof()) {
            try {
                getline(openfile, s);
                if (openfile.eof()) {
                    openfile.close();
                    break;
                }
                string id = "";
                string dir = "";
                int i = 0;
                while (s[i] != '-') {
                    id += s[i];
                    i++;
                }
                i++;
                while (s[i] != '\0') {
                    dir += s[i];
                    i++;
                }
                bpt.insert(id, dir);
            }
            catch (string e) {
                cout << e;
            }
        }
        cout << "B tree Populated" << endl;
    }
    else {
        SetConsoleTextAttribute(console, 12);
        system("cls");
        cout << "This is First Time Data is Beign loaded and Files / Folders are made this can take time" << endl;
        cout << "This will depend upon your computer HDD/ SDD" << endl;
        Sleep(1500);
        openfile.close();
        writesubfiles b(argv);
        FreeConsole();
        system("mkdir Database  > file.dat");
        
        remove("_log.txt");
        AttachConsole(ATTACH_PARENT_PROCESS);
        system("cls");
        string to_open;
        string lastfile = "";
        while (true) {
            SetConsoleTextAttribute(console, 15);
            cout << "Enter the csv file to read from" << endl;
            cout << "You have .csv named DeathRate_0* 1 - 10" << endl;
            cin >> to_open;
            if (to_open == "exit") {
                break;
            }
            lastfile = to_open;
            a.read_csv(to_open);
            cout << "If you want to add another file write name. or type (exit) to not add." << endl;
        }
        system("cls");
        int temp = a.numlabels();
        cout << "[ ";
        for (int i = 0; i < temp; i++) {
            cout << i << " ,";
        }
        cout << " ]" << endl;
        cout << "[ ";
        
        for (int i = 0; i < temp; i++) {
            cout << a.getcolname(i % temp) << " ,";
        }
        cout << " ]" << endl;
        cout << "Choose a Number from 1 to * to Populate the B tree on that Particular Index" << endl;
        int ni = 0;
        cin >> ni;
        string id = "";
        string popped = "";
        int numloop = a.getinserted() * a.numlabels();
        ofstream mylogfile("_log.txt", ios::app);
        mylogfile << lastfile << endl;
        system("cls");
        SetConsoleTextAttribute(console, 12);
        cout << "Please have Patience as Database is beign initialzed for first time" << endl;
        for (int i = 0; i < numloop - (1 * a.numlabels()); i++) {
            if (i % temp == ni) {
                id = a.pop(i % temp);
                string s = "mkdir Database\\" + id;
                system(s.c_str());
                popped = id;
            }
            else {
                popped = a.pop(i % temp);
            }
            string d = b.write_file(a.getcolname(i % temp), popped, id);
            if (i % temp == ni) {
                bpt.insert(id, "Database/" + id);
                mylogfile << id << "-" << "Database/" + id << endl;
            }
        }
        mylogfile.close();
    }
    // Menu goes at this place
    SetConsoleTextAttribute(console, 10);
    cout << "DataBase has been Loaded wait Loading Menu ..." << endl;
    SetConsoleTextAttribute(console, 15);
    int option;
    while (true) {
        system("cls");
        cout << "1) Search / Edit" << endl;
        cout << "2) Delete" << endl;
        cout << "4) Exit" << endl;
        cin >> option;
        switch (option){
        case 1: {
            system("cls");
            SetConsoleTextAttribute(console, 14);
            cout << "Search / Edit" << endl;
            cout << endl;
            cout << "Enter a Index which you want to Search" << endl;
            string idd = "";
            cin >> idd;
            idd = bpt.search(idd);
            if (idd == "") {
                cout << "Nothing found with this " << idd << endl;
            }
            else {
                system("cls");
                int temp = a.numlabels();
                cout << "[ ";
                for (int i = 0; i < temp; i++) {
                    cout << i << " ,";
                }
                cout << " ]" << endl;
                cout << "[ ";

                for (int i = 0; i < temp; i++) {
                    cout << a.getcolname(i % temp) << " ,";
                }
                cout << " ]" << endl;
                cout << "Choose a Number from 1 to * to Edit the Attribute of this" << endl;
                int ni = 0;
                cin >> ni;
                //cout << idd;
                string oldval = "";
                string newval = "";
                char ch = 'a';
                Array g;
                string openthis = idd+"/" + a.getcolname(ni)+".txt";
                ifstream opennew(openthis.c_str(),ios::in);
                while (!opennew.eof()) {
                    opennew >> ch;
                    if (ch == '%'){
                        opennew >> ch;
                        if (opennew.eof()) {
                            g.insert(newval);
                            break;
                        }
                        g.insert(newval);
                        newval = "";
                    }
                    newval += ch;
                }
                Array n;
                openthis = idd + "/" + a.getcolname(ni) + "old.txt";
                ifstream openold(openthis.c_str(), ios::in);
                if (openold.is_open()) {
                    while (!openold.eof()) {
                        openold >> ch;
                        if (ch == '%') {
                            openold >> ch;
                            if (openold.eof()) {
                                n.insert(oldval);
                                break;
                            }
                            n.insert(oldval);
                            oldval = "";
                        }
                        oldval += ch;
                    }
                }
                if (g.getin() <= 1) {
                    cout << "Value current: " << newval << endl;
                    cout << "Value old:" << oldval << endl;
                    Sleep(500);
                    cout << "Do you want to Edit the Value (y / n)" << endl;
                    cin >> ch;
                    if (ch == 'y') {
                        string newvala;
                        cout << "Enter a new value" << endl;
                        cin >> newvala;
                        openthis = idd + "/" + a.getcolname(ni) + ".txt";
                        ofstream opennew(openthis.c_str(), ios::out);
                        opennew << newvala;
                        opennew << "%";
                        opennew.close();
                        openthis = idd + "/" + a.getcolname(ni) + "old.txt";
                        ofstream openold(openthis.c_str(), ios::out);
                        openold << newval;
                        openold << "%";
                        openold.close();
                    }
                    else {

                    }
                }
                else {
                    cout << "Multiple Values where found against the Id" << endl;
                    temp = g.getin();
                    cout << "Choose = [ ";
                    for (int i = 0; i < temp; i++) {
                        cout << i << " ,";
                    }
                    cout << " ]" << endl;
                    cout << "Found  = [ ";

                    for (int i = 0; i < temp; i++) {
                        cout << g.getval(i) << " ,";
                    }
                    cout << " ]" << endl;
                    cout << "Choose a Number from 0 to * to edit one you wnat of this" << endl;
                    int l = 0;
                    cin >> l;
                    system("cls");
                    cout << "Selected: "<<g.getval(l) << endl;
                    cout << "Old Val : " << n.getval(l) << endl;
                    cout << "Do you want to Edit the Value (y / n)" << endl;
                    cin >> ch;
                    if (ch == 'y') {
                        string newvala;
                        cout << "Enter a new value" << endl;
                        cin >> newvala;
                        oldval = g.getval(l);
                        g.changeval(newvala, l);
                        openthis = idd + "/" + a.getcolname(ni) + ".txt";
                        ofstream opennew(openthis.c_str(), ios::out);
                        for (int i = 0; i < temp; i++)
                        {
                            opennew << g.getval(i);
                            opennew << "%";
                        }
                        opennew.close();
                        openthis = idd + "/" + a.getcolname(ni) + "old.txt";
                        ofstream openold(openthis.c_str(), ios::out);
                        for (int i = 0; i < temp; i++)
                        {
                            openold << n.getval(i);
                            openold << "%";
                            if (i == l-1) {
                                openold << oldval;
                                openold << "%";
                                i++;
                            }
                        }
                        openold.close();
                    }
                    else {

                    }
                }
            }
            break;
        }
        case 2: {
            system("cls");
            SetConsoleTextAttribute(console, 14);
            cout << "Delete" << endl;
            cout << endl;
            cout << "Enter a Index which you want to Delete" << endl;
            string idd = "";
            cin >> idd;
            bool removed = bpt.remove(idd);
            if (removed) {
                cout << "Succesfully Deleted " << idd << endl;
            }
            else {
                cout << "Not Found" << endl;
            }
            Sleep(1500);
            cout << endl;
            break;
        }
        case 3: {
            SetConsoleTextAttribute(console, 12);
            bpt.cleanUp(bpt.getRoot());
            return 0;
            break;
        }
        default: {
            break;
        }
        }
    }
    SetConsoleTextAttribute(console, 12);
    return 0;
}
