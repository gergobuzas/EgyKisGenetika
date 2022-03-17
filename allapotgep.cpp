/**
 * \file allapotgep.cpp
 *
 * Ebben a fájlban kell megvalósítania az Allapotgep osztály
 * metódusait, valamint mindazon további osztályokat, melyek szükségesek
 * a feladat megvalósításához.
 *
 * Ezt a fájlt be kell adni (fel kell tölteni) a megoldással.
 */

#include <iostream>
#include <fstream>

#include "allapotgep.h"
#include "memtrace.h"

int Atoi(char* str){
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i){
        res = res * 10 + str[i] - '0';
    }
    return res;
}


char& States::getName(){
    return *this->name;
}

bool States::getAcceptable(){
    return this->acceptable;
}
void States::setName(char* newname){
    this->name = new char[21];
    for (int j = 0; j < 21; ++j) {
        this->name[j] = newname[j];
        if (newname[j] == '\n')
            break;
    }
}

void States::setAcceptable(bool setter){
    this->acceptable = setter;
}


/**
    * Konfig fájl beolvasása. Ha a fájl nem létezik/nem lehet megnyitni eldobja a NEPTUN-kódot.
    * Újra felépíti az állapotgépet, a korábbit törli ha volt. Kezdő állapotba áll.
    * @param  fajlnév - fájl neve
    * @return -
    * @exception fájl megnyitási hiba esetén NEPTUN_KÓD
   */
void Allapotgep::konfigural(const char* fajlnev){
    std::ifstream myFile(fajlnev);
    if (myFile.is_open()){
        //Reading the first line
        char* line = new char[1];
        myFile.getline(line, 1);
        int num = Atoi(line);
        delete[] line;


        //Reading every line of the config file, thus getting the states
        line = new char[21];
        States states[num];

        int i = 0;
        while(myFile.getline(line, 21)){
            if (i > 0){
                if (line[0] == 'I')
                    states[i-1].setAcceptable(true);
                else
                    states[i-1].setAcceptable(false);

                line = line + 2; //moving the char pointer two bits forward, to delete "I " from the string
                states[i-1].setName(line);
            }
            ++i;
        }
    }
    else
        throw("E0PWAX");
}
