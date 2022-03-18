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

bool States::getActive(){
    return this->active;
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
    this->active = setter;
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

/*StateEvent StateEvent::operator=(StateEvent& rhs){
    this->currentStateNum = rhs.currentStateNum;
    this->causingChar = rhs.causingChar;
    this->nextStateNum = rhs.nextStateNum;
    return *this;
}*/


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
    }
    std::ifstream myFile;
    myFile.open(fajlnev);
    if (myFile.fail()){
        throw("E0PWAX");
    }
    //To get the number of states from the first line
    myFile >> numOfStates;

    //Reading the next lines of the config file, thus getting the states of the state machine
    states = new States[numOfStates];

    for (int i = 0; i < numOfStates; ++i) {
        char acceptState;
        myFile >> acceptState;
        states[i].setAcceptable(acceptState);
        char* nameState = new char[21];
        myFile >> nameState;
        states[i].setName(nameState);
        states[i].id=i;
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
                            nextStateLogic[numOfStateEvents-1].setNextStateNum(j);
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
    current = &states[0];

    }

/** Visszaadja melyik állapot aktív.
     * @return pointer az aktív állapot nevére
     */
const char* Allapotgep::aktualisallapot(){
    return current->getName();
}

/**
  * Elfogadó állapotban van-e az állapotgép.
  * @return true, ha elfogadó állapotban van
  */
bool Allapotgep::elfogad(){
    return current->getAcceptable();
}


/**
 * Az aktuális állapotnak és inputnak megfelelő következő állapotba léptet.
 * @param b - beérkező bázis, mint input
 */
void Allapotgep::atmenet(Bazis b){
    char input = cast(b, true);
    for (int j = 0; j < numOfStateEvents; ++j) {
        if(input == nextStateLogic[j].getCausingChar()){
            if (current->id == states[nextStateLogic[j].getCurrentStateNum()].id){
                current->setActive(false);
                states[ nextStateLogic[j].getNextStateNum() ].setActive(true);
                current = &states[nextStateLogic[j].getNextStateNum()];
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
    current = &states[0];
    states[0].setActive(true);

}


