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


/* void StateEvent::nextStateEvent(States* states, int currentStateNum, int nextStateNum){
    states[currentStateNum].setActive(false);
    states[nextStateNum].setActive(true);
} */



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
        myFile >> numOfStates;

        //Reading the next lines of the config file, thus getting the states of the state machine
        states[numOfStates];

        for (int i = 0; i < numOfStates; ++i) {
            char acceptState;
            myFile >> acceptState;
            states[i].setAcceptable(acceptState);

            char* nameState = new char[21];
            myFile >> nameState;
            states[i].setName(nameState);
            delete[] nameState;
        }

        //To get the events of the state machine
        int numOfStateEvents = 0;
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
                        case 'C':
                        case 'c':
                        case 'G':
                        case 'g':
                        case 'T':
                        case 't':
                            numOfStateEvents++;
                            if (numOfStateEvents > 1){
                                StateEvent* nextStateLogicTEMP = new StateEvent [numOfStateEvents];
                                for (int l = 0; l < numOfStateEvents - 1; ++l) {
                                    nextStateLogicTEMP[l] = nextStateLogic[l];
                                }
                                delete[] nextStateLogic;
                                nextStateLogic = new StateEvent [numOfStateEvents];
                                for (int l = 0; l < numOfStateEvents; ++l) {
                                    nextStateLogic[l] = nextStateLogicTEMP[l];
                                }
                                delete[] nextStateLogicTEMP;
                            }
                            nextStateLogic = new StateEvent [numOfStateEvents];
                            break;
                        /*default:
                            throw (std::invalid_argument("Invalid genetic type in config file"));*/
                    }
                }
                delete[] eventGen;
            }
        }

        //making the BASE state active
        states[0].setActive(true);

    }
    else
        throw("E0PWAX");
}

/** Visszaadja melyik állapot aktív.
     * @return pointer az aktív állapot nevére
     */
const char* Allapotgep::aktualisallapot(){

}

