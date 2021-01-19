//
//  Structure.h
//  Project_Data_Structure
//
//  Created by Musa Dildar Ahmed Cheema on 11/28/20.
//

#ifndef Structure_h
#define Structure_h


#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;

template <class T>
struct csv_array {
    T data_of_column;
    string name_of_column;
    int index;
    csv_array <T>* down = NULL;
};




template <class T>
class csv_reading {
private:
    csv_array<string>** a = NULL;
    int inserted = 0;
    int number_of_labels = 1;
    string* column_name;
public:
    string getcolname(int index) {
        return column_name[index];
    }
    void findnumberofpossiblelabelsincsv(string file_name_to_open) {
        fstream file_read;
        char character_from_csv_file='a';
        string colnametemp = "";
        file_read.open(file_name_to_open, ios::in);
        if (!file_read.is_open()) {
            return;
        }
        while (character_from_csv_file != '\r' && character_from_csv_file != '\n' && character_from_csv_file != '\r\n') {
            if (character_from_csv_file == ',') {
                number_of_labels += 1;
            }
            file_read >> std::noskipws >> character_from_csv_file;
        }
        file_read.close();
        file_read.open(file_name_to_open, ios::in);
        column_name = new string[number_of_labels];
        int track_temp = 0;
        character_from_csv_file = '\0';
        while (character_from_csv_file != '\r' && character_from_csv_file != '\n' && character_from_csv_file != '\r\n') {
            file_read >> std::noskipws >> character_from_csv_file;
            if (character_from_csv_file == ',' || character_from_csv_file == '\r' || character_from_csv_file == '\n' || character_from_csv_file == '\r\n') {
                column_name[track_temp] = colnametemp;
                colnametemp = "";
                track_temp++;
            }
            else {
                colnametemp += character_from_csv_file;
            }
        }
    }

    void _init() {
        if (a != NULL) {
            return;
        }
        a = new csv_array<string> *[number_of_labels];
        for (int i = 0; i < number_of_labels; i++) {
            a[i] = new csv_array<string>;
        }
    }
    string _Extension_check(string filename, string type = ".csv") {
        for (int i = 0; i < filename.length(); i++) {
            if (filename[i] == '.') {
                return filename;
            }
        }
        filename = filename + type;
        return filename;
    }

    void read_csv(string filename) {
        fstream file_read;
        int track_temp = 0;
        string colnametemp = "";
        filename = _Extension_check(filename);
        if (a == NULL) {
            findnumberofpossiblelabelsincsv(filename);
            _init();
        }
        csv_array<string>** lastnode = new csv_array<string> *[number_of_labels];
        for (int i = 0; i < number_of_labels; i++) {
            lastnode[i] = a[i];
        }
        char character_from_csv_file = '\0';
        file_read.open(filename, ios::in);
        if (!file_read.is_open()) {
            cout << "File not Found"<<endl;
            return;
        }
        while (character_from_csv_file != '\r' && character_from_csv_file != '\n' && character_from_csv_file != '\r\n') {
            file_read >> std::noskipws >> character_from_csv_file;
        }// to skip first row
        bool quotation = false;
        while (!file_read.eof()) {
            if (character_from_csv_file == '\r' || character_from_csv_file == '\n') {
                csv_array<string>* b = new csv_array<string>;
                b->data_of_column = colnametemp;
                b->down = lastnode[track_temp];
                b->name_of_column = column_name[track_temp];
                lastnode[track_temp] = b;
                colnametemp = "";
                track_temp = 0;
                inserted++;
            }
            else if (character_from_csv_file == ',' && quotation != true) {
                csv_array<string>* b = new csv_array<string>;
                b->data_of_column = colnametemp;
                b->down = lastnode[track_temp];
                b->name_of_column = column_name[track_temp];
                lastnode[track_temp] = b;
                colnametemp = "";
                track_temp++;
            }
            else {
                if (character_from_csv_file != '\0') {
                    colnametemp += character_from_csv_file;
                }
            }
            file_read >> std::noskipws >> character_from_csv_file;
            if (character_from_csv_file == '"') {
                if (quotation == true) {
                    file_read >> std::noskipws >> character_from_csv_file;
                    if (character_from_csv_file == ',') {
                        quotation = false;
                    }
                }
                else {
                    quotation = true;
                    file_read >> std::noskipws >> character_from_csv_file;
                }
            }
        }
        a = lastnode;
    }
    T pop(int index) {
        if (a[index] == NULL) {
            return NULL;
        }
        else
        {
            csv_array <T>* temp = a[index];
            a[index] = a[index]->down;
            T re = temp->data_of_column;
            delete(temp);
            return re;
        }
    }

    T seek(int index) {
        if (a[index] == NULL) {
            return NULL;
        }
        else
        {
            csv_array <T>* temp = a[index];
            T re = temp->data_of_column;
            return re;
        }
    }

    int numlabels() {
        return number_of_labels;
    }
    int getinserted() {
        return inserted;
    }
};



#endif /* Structure_h */
