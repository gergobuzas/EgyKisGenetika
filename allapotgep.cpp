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


char& States::getName(){
    return *this->name;
}

bool States::getAcceptable() const{
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
        //To get the number of states from the first line
        int numOfStates;
        myFile >> numOfStates;

        //Reading the next lines of the config file, thus getting the states of the state machine
        States states[numOfStates];

        for (int i = 0; i < numOfStates; ++i) {
            char* acceptState = new char[1];
            myFile >> acceptState;
            states[i].setAcceptable(acceptState);
            delete[] acceptState;

            char* nameState = new char[20];
            myFile >> nameState;
            states[i].setName(nameState);
            delete[] nameState;
        }

        //To get the events of the state machine
        for (int i = 0; i < numOfStates; ++i) {
            for (int j = 0; j < numOfStates; ++j) {
                char* eventGen = new char [4];
                myFile >> eventGen;
                switch(eventGen){

                }
            }
        }

    }
    else
        throw("E0PWAX");
}
