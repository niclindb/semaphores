#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
#include <semaphore.h>
#include <mutex>

using namespace std;

class Person
{
public:
    string Name;
    char sex; // either 'M' or 'F'
    void EnterBathroom();
    void ExitBathroom();
    Person(string n, char s);
    Person();

private:
    // Shared counters
    static const int MAX_CAPACITY;
    static int nmw; // males waiting
    static int nfw; // females waiting
    static int males_in_bath;
    static int females_in_bath;

    // Shared semaphores
    static sem_t male_sem;
    static sem_t female_sem;
    static sem_t admin;

    // Synchronization flag for one-time initialization
    // the once flag garantees that the initialization is done only once
    static once_flag sema_init_flag;

    // initialize and clean up semaphores
    static void initSemaphores();
    static void destroySemaphores();

    // Static cleaner variable
    static bool cleaner; // will call destroySemaphores once
};

#endif
