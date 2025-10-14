// Person.cc
#include "Person.h"
#include <iostream>

using namespace std;

const int MAX_CAPACITY = 6;

// Counters
int nmw = 0; // males waiting
int nfw = 0; // females waiting
int males_in_bath = 0;
int females_in_bath = 0;

// Semaphores
sem_t male_sem, female_sem, admin; 

void Person::EnterBathroom() {
    sem_wait(&admin);
    //check if i have to wait    
    if(sex == 'M' && (females_in_bath > 0 || males_in_bath == MAX_CAPACITY)){
        nmw++; 
        sem_post (&admin); 
        cout << Name+" (M) is waiting\n\n"; 
        sem_wait (&male_sem);
        sem_wait(&admin);

    }
    else if(sex == 'F' && (males_in_bath > 0 || females_in_bath == MAX_CAPACITY)){
        nfw++; 
        sem_post (&admin); 
        cout << Name+" (F) is waiting\n\n"; 
        sem_wait (&female_sem);
        sem_wait(&admin);
    }
    // enter bathroom
    int occupancy;
    
    if(sex == 'M') {
        occupancy = ++males_in_bath;
        // if more people of the same sex are waiting let the next one in. 
        if(occupancy < MAX_CAPACITY && nmw>0){
            nmw--;
            sem_post(&male_sem);
        }
    }
    else {
        occupancy = ++females_in_bath;
        if(occupancy < MAX_CAPACITY && nfw>0){
            nfw--;   
            sem_post(&female_sem);
        }
    }
    sem_post(&admin);
    cout << Name + " ("+sex+") is entering the bathroom.\n"+to_string(occupancy)+" people are in the bathroom\n\n";
}

void Person::ExitBathroom() {
    sem_wait(&admin);
    //exit bathrom
    if (sex == 'M') {
        males_in_bath--;
        cout << Name + " (M) exited.\n Occupancy: " + to_string(males_in_bath)+"\n";
       // if it is empty let the other sex in
        if(males_in_bath == 0 && nfw > 0){
            nfw--;
            sem_post(&female_sem);
        }else if (nmw > 0 && nfw == 0){ // if more males are waiting let them in only if no females are waiting
            nmw--;
            sem_post(&male_sem);
        }

    } else {
        females_in_bath--;
        cout << Name + " (F) exited.\n Occupancy: " + to_string(females_in_bath)+"\n";
        if(females_in_bath == 0 && nmw > 0){
            nmw--;
            sem_post(&male_sem);
        }else if (nfw > 0 && nmw == 0){// keeps males waiting for ever
            nfw--;
            sem_post(&female_sem);
        }
    }
    sem_post(&admin);
}
