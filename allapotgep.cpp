 /*\\file allapotgep.cpp
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


char* States::getName(){
    return this->name;
}

bool States::getAcceptable() const{
    return this->acceptable;
}

int States::getActive(){
    return active;
}

void States::setName(char* newname){
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

void States::setActive(bool setter) {
    active = setter;
}

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
    if(numOfStates > 0) {
        delete[] states;
        delete[] nextStateLogic;
        alaphelyzet();
    }

    std::ifstream input;
    input.open(fajlnev);
    if (input.fail()){
        throw("E0PWAX");
    }
    //To get the number of states from the first line
    input >> numOfStates;

    //Reading the next lines of the config file, thus getting the states of the state machine
    states = new States[numOfStates];

    for (int i = 0; i < numOfStates; ++i) {
        char acceptState;
        input >> acceptState;
        states[i].setAcceptable(acceptState);
        char* nameState = new char[21];
        input >> nameState;
        states[i].setName(nameState);
        delete[] nameState;
    }

    //To get the events of the state machine
    numOfStateEvents = numOfStates * 4;
    nextStateLogic = new StateEvent [numOfStateEvents];
    int idx = 0;
    char* eventGen = new char [5];
    for (int i = 0; i < numOfStates; ++i) {
        for (int j = 0; j < numOfStates; ++j) {
            input >> eventGen;
                for (int k = 0; eventGen[k] != '\0'; ++k) {
                    if (eventGen[k] == 'A' || eventGen[k] == 'a' || eventGen[k] == 'C' || eventGen[k] == 'c' ||
                        eventGen[k] == 'g' || eventGen[k] == 'G' || \
                        eventGen[k] == 'T' || eventGen[k] == 't') {
                            nextStateLogic[idx].setCurrentStateNum(i);
                            nextStateLogic[idx].setCausingChar(eventGen[k]);
                            nextStateLogic[idx++].setNextStateNum(j);
                    }

                }
        }
    }
     delete[] eventGen;

    //making the BASE state active
    states[0].setActive(true);
    input.close();
}

/** Visszaadja melyik állapot aktív.
     * @return pointer az aktív állapot nevére
     */
const char* Allapotgep::aktualisallapot(){
     for (int i = 0; i < numOfStates; ++i) {
         if (states[i].getActive())
             return states[i].getName();
     }
     return "Error";
}

/**
  * Elfogadó állapotban van-e az állapotgép.
  * @return true, ha elfogadó állapotban van
  */
bool Allapotgep::elfogad(){
     for (int i = 0; i < numOfStates; ++i) {
         if (states[i].getActive())
             return states[i].getAcceptable();
     }
     return "Error";
}


/**
 * Az aktuális állapotnak és inputnak megfelelő következő állapotba léptet.
 * @param b - beérkező bázis, mint input
 */
 void Allapotgep::atmenet(Bazis b){
     char input = cast(b, true);
     bool done = false;
     for (int i = 0; i < numOfStates; ++i) {
         if (states[i].getActive()){
             for (int j = 0; j < numOfStateEvents; ++j) {
                 if(i == nextStateLogic[j].getCurrentStateNum() && input == nextStateLogic[j].getCausingChar()){
                     states[i].setActive(false);
                     states[nextStateLogic[j].getNextStateNum()].setActive(true);
                     done = true;
                     break;
                 }
             }
         }
         if (done)
             break;
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
    for (int i = 0; i < n; ++i) {
        atmenet(b[i]);
    }
    return elfogad();
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


