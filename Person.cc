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
    }
    else if(sex == 'F' && (males_in_bath > 0 || females_in_bath == MAX_CAPACITY)){
        nfw++; 
        sem_post (&admin); 
        cout << Name+" (F) is waiting\n\n"; 
        sem_wait (&female_sem);
    }
    // enter bathroom
    int occupancy;
    cout << Name<<" ("<<sex<<") is entering the bathroom\n"<<occupancy<<" people are in the bathroom\n\n";
    if(sex == 'M') {
        occupancy = ++males_in_bath;
        // if more people of the same sex are waiting let the next one in. 
        if(occupancy < MAX_CAPACITY && nmw>0){
            nmw--;
            sem_post(&male_sem);
        }else{
            sem_post(&admin);
        }
    }
    else {
        occupancy = ++females_in_bath;
        if(occupancy < MAX_CAPACITY && nfw>0){
            nfw--;
            sem_post(&female_sem);
        }else{
            // no one else to let in release admin lock
            sem_post(&admin);
        }
    }

}

void Person::ExitBathroom() {
    sem_wait(&admin);
    //exit bathrom
    if (sex == 'M') {
        males_in_bath--;
        cout << Name << " (M) exited.\n Occupancy: " << males_in_bath << endl;
       // if it is empty let the other sex in
        if(males_in_bath == 0 && nfw > 0){
            nfw--;
            sem_post(&female_sem);
        }else if (nmw > 0){ // if more males are waiting let them in
            nmw--;
            sem_post(&male_sem);
        }
        else{
            sem_post(&admin);
        }

    } else {
        females_in_bath--;
        cout << Name << " (F) exited.\n Occupancy: " << females_in_bath << endl;
        if(females_in_bath == 0 && nmw > 0){
            nmw--;
            sem_post(&male_sem);
        }else if (nfw > 0){
            nfw--;
            sem_post(&female_sem);
        }
        else{
            sem_post(&admin);
        }
    }
}
