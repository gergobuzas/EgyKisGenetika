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


/*void StateEvent::nextStateEvent(int currentStateNum, int nextStateNum){
    states[currentStateNum].setActive(false);
    states[nextStateNum].setActive(true);
}*/

void StateEvent::setCurrentStateNum(int num){
    currentStateNum = num;
}

void StateEvent::setNextStateNum(int num) {
    nextStateNum = num;
}

void StateEvent::setCausingChar(char setter){
    causingChar = setter;
}

int StateEvent::getCurrentStateNum() const{
    return currentStateNum;
}

int StateEvent::getNextStateNum() const{
    return nextStateNum;
}

char StateEvent::getCausingChar() const {
    return causingChar;
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
        numOfStateEvents = 0;
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
                                StateEvent* nextStateLogicTEMP = new StateEvent [numOfStateEvents]; //working with the memory management
                                for (int l = 0; l < numOfStateEvents - 1; ++l) {
                                    nextStateLogicTEMP[l] = nextStateLogic[l];
                                }
                                delete[] nextStateLogic;
                                nextStateLogic = new StateEvent [numOfStateEvents];
                                for (int l = 0; l < numOfStateEvents; ++l) {
                                    nextStateLogic[l] = nextStateLogicTEMP[l];
                                }
                                delete[] nextStateLogicTEMP;

                                nextStateLogic[numOfStateEvents-1].setCurrentStateNum(i); //saving the EVENTS to an array(currentstate = from    nextstate = to)
                                nextStateLogic[numOfStateEvents-1].setCausingChar(eventGen[k]);
                                nextStateLogic[numOfStateEvents-1].setNextStateNum(j);
                            }
                            else{
                                nextStateLogic = new StateEvent [numOfStateEvents];
                                nextStateLogic[numOfStateEvents-1].setCurrentStateNum(i);
                                nextStateLogic[numOfStateEvents-1].setCausingChar(eventGen[k]);
                            }

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
    for (int i = 0; i < numOfStates; ++i) {
        if (states[i].getActive()){
            return &states[i].getName();
        }
    }
}

/**
  * Elfogadó állapotban van-e az állapotgép.
  * @return true, ha elfogadó állapotban van
  */
bool Allapotgep::elfogad(){
    for (int i = 0; i < numOfStates; ++i) {
        if (states[i].getActive()){
            if(states[i].getAcceptable()){
                return true;
            }
        }
    }
    return false;
}


/**
 * Az aktuális állapotnak és inputnak megfelelő következő állapotba léptet.
 * @param b - beérkező bázis, mint input
 */
void Allapotgep::atmenet(Bazis b){
    char input = cast(b, false);

    for (int i = 0; i < numOfStates; ++i) {
        if (states[i].getActive()){
            for (int j = 0; j < numOfStateEvents; ++j) {
                if(i == nextStateLogic[j].getCurrentStateNum() && input == nextStateLogic[j].getCausingChar()){
                    states[i].setActive(false);
                    states[nextStateLogic[j].getNextStateNum()].setActive(true);
                }
            }
        }
    }
}


/**
 * Feldolgozza a paraméterként kapott bázissorozatot.
 * Visszaadja, hogy elfogadó állapotba került-e. (Nem áll vissza kezdő állapotba.)
 * @param b - input sorozatra mutató pointer
 * @param n - a sorozat hossza
 * @return tru, ha elfogadó állapotba került
*/
bool Allapotgep::feldolgoz(const Bazis *b, int n){
    /*char input[n];
    for (int i = 0; i < n; ++i) {
        input[i] = cast(b[i], false);
    }*/

    for (int i = 0; i < n; ++i) {
        atmenet(b[i]);
    }

    for (int i = 0; i < numOfStates; ++i) {
        if (states[i].getActive()){
            if(states[i].getAcceptable()){
                return true;
            }
        }
    }
    return false;
}

/**
     * Kezdő állapotba visz.
     */
void Allapotgep::alaphelyzet(){
    for (int i = 0; i < numOfStates; ++i) {
        states[i].setActive(false);
    }
    states[0].setActive(true);
}


