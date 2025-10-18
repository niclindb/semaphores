#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include "Person.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime> 


using namespace std;

void* PersonThread(void* arg) {
    int sleepTime = 1 + rand() % 10; 
    Person* p = (Person*)arg;
    sleep(sleepTime);
    p->EnterBathroom();
    sleep(1); // simulate time inside
    p->ExitBathroom();
    return nullptr;
}

int main() {

    // Create 15 people
    Person people[15] = {
        Person("Alex", 'M'), Person("Jamie", 'F'), Person("Chris", 'M'),
        Person("Taylor", 'F'), Person("Jordan", 'M'), Person("Riley", 'F'),
        Person("Cameron", 'M'), Person("Casey", 'F'), Person("Morgan", 'M'),
        Person("Drew", 'F'), Person("Shawn", 'M'), Person("Avery", 'F'),
        Person("Ryan", 'M'), Person("Quinn", 'F'), Person("Logan", 'M')
    };

    pthread_t threads[15];

    // Launch each person in a thread
    for (int i = 0; i < 15; ++i)
        pthread_create(&threads[i], nullptr, PersonThread, &people[i]);

    // Wait for all threads to finish
    for (int i = 0; i < 15; ++i)
        pthread_join(threads[i], nullptr);

    return 0;
}