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

bool States::getActive(){
    return this->active;
}

void States::setName(char* newname){
    this->name = new char[21];
    for (int j = 0; j < 21; ++j) {
        this->name[j] = newname[j];
        if (newname[j] == '\n')
            break;
    }
}

void States::setAcceptable(char setter){
    if(setter == 'i' || setter == 'I')
        this->acceptable = true;
    if(setter == 'h' || setter == 'H')
        this->acceptable = false;
}

void States::setAcceptable(bool setter){
    this->acceptable = setter;
}

void States::setActive(bool setter) {
    this->active = setter;
}


void StateEvent::nextStateEvent(){

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
            char acceptState;
            myFile >> acceptState;
            states[i].setAcceptable(acceptState);

            char* nameState = new char[21];
            myFile >> nameState;
            states[i].setName(nameState);
            delete[] nameState;
        }

        //making the base state active
        states[0].setActive(true);

        //To get the events of the state machine
        for (int i = 0; i < numOfStates; ++i) {
            for (int j = 0; j < numOfStates; ++j) {
                char* eventGen = new char [5];
                myFile >> eventGen;
                for (int k = 0; k < 4; ++k) {
                    switch(eventGen[k]){
                        case '0':
                            break;
                        case 'A':
                        case 'a':

                            break;
                        case 'C':
                        case 'c':

                            break;
                        case 'G':
                        case 'g':

                            break;
                        case 'T':
                        case 't':

                            break;
                        /*default:
                            throw (std::invalid_argument("Invalid genetic type in config file"));*/
                    }
                }

            }
        }

    }
    else
        throw("E0PWAX");
}
