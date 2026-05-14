#ifndef FLIGHT_H
#define FLIGHT_H

#include "LinkedList.h"

struct Flight {
    MyString id;     
    MyString from;    
    MyString to;      
    MyString depart;   
    MyString arrive;  
    MyString aircraft;

    Flight() {}

    Flight(MyString _id, MyString _from, MyString _to, MyString _dep, MyString _arr, MyString _air)
        : id(_id), from(_from), to(_to), depart(_dep), arrive(_arr), aircraft(_air) {}
};

#endif