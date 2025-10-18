#include "Person.h"

using namespace std;

const int Person::MAX_CAPACITY = 6;
// Counters
int Person::nmw = 0;
int Person::nfw = 0;
int Person::males_in_bath = 0;
int Person::females_in_bath = 0;
// Semaphores
sem_t Person::male_sem;
sem_t Person::female_sem;
sem_t Person::admin;

once_flag Person::sema_init_flag;

void Person::initSemaphores()
{
    sem_init(&male_sem, 0, 0);
    sem_init(&female_sem, 0, 0);
    sem_init(&admin, 0, 1);
}

void Person::destroySemaphores()
{
    sem_destroy(&male_sem);
    sem_destroy(&female_sem);
    sem_destroy(&admin);
}

bool Person::cleaner = [] // lambda function to register atexit cleanup
{
    // destroy semaphores at program exit
    std::atexit(Person::destroySemaphores);
    return true;
}();

Person::Person()
{
    Name = "";
    sex = ' ';
    call_once(sema_init_flag, initSemaphores);
}
Person::Person(string n, char s)
{
    Name = n;
    sex = s;
    call_once(sema_init_flag, initSemaphores);
}

void Person::EnterBathroom()
{
    sem_wait(&admin);
    // check if i have to wait
    if (sex == 'M' && (females_in_bath > 0 || males_in_bath == MAX_CAPACITY))
    {
        nmw++;
        sem_post(&admin);
        cout << Name + " (M) is waiting\n\n";
        sem_wait(&male_sem);
    }
    else if (sex == 'F' && (males_in_bath > 0 || females_in_bath == MAX_CAPACITY))
    {
        nfw++;
        sem_post(&admin);
        cout << Name + " (F) is waiting\n\n";
        sem_wait(&female_sem);
    }
    // enter bathroom
    int occupancy;

    if (sex == 'M')
    {
        occupancy = ++males_in_bath;
        // if more people of the same sex are waiting let the next one in.
        if (occupancy < MAX_CAPACITY && nmw > 0)
        {
            nmw--;
            sem_post(&male_sem);
        }
        else
        {
            sem_post(&admin);
        }
    }
    else
    {
        occupancy = ++females_in_bath;
        if (occupancy < MAX_CAPACITY && nfw > 0)
        {
            nfw--;
            sem_post(&female_sem);
        }
        else
        {
            sem_post(&admin);
        }
    }
    cout << Name + " (" + sex + ") is entering the bathroom.\n" + to_string(occupancy) + " people are in the bathroom\n\n";
}

void Person::ExitBathroom()
{
    sem_wait(&admin);
    // exit bathrom
    if (sex == 'M')
    {
        males_in_bath--;
        cout << Name + " (M) exited.\n Occupancy: " + to_string(males_in_bath) + "\n";
        // if it is empty let the other sex in
        if (males_in_bath == 0 && nfw > 0)
        {
            nfw--;
            sem_post(&female_sem);
        }
        else if (nmw > 0 && nfw == 0)
        { // if more males are waiting let them in only if no females are waiting
            nmw--;
            sem_post(&male_sem);
        }
        else
        {
            sem_post(&admin);
        }
    }
    else
    {
        females_in_bath--;
        cout << Name + " (F) exited.\n Occupancy: " + to_string(females_in_bath) + "\n";
        if (females_in_bath == 0 && nmw > 0)
        {
            nmw--;
            sem_post(&male_sem);
        }
        else if (nfw > 0 && nmw == 0)
        { // keeps males waiting for ever
            nfw--;
            sem_post(&female_sem);
        }
        else
        {
            sem_post(&admin);
        }
    }
}
