#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <semaphore.h>

using namespace std;

extern const int MAX_CAPACITY;

// Counters
extern int nmw; // males waiting
extern int nfw; // females waiting
extern int males_in_bath;
extern int females_in_bath;

// Semaphores
extern sem_t male_sem, female_sem, admin; 

class Person {
public:
    string Name;
    char sex; // either 'M' or 'F'
    void EnterBathroom();
    void ExitBathroom();
    Person(string n, char s) : Name(n), sex(s) {};
};

#endif
