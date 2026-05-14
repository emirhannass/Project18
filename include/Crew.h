#ifndef CREW_H
#define CREW_H

#include "LinkedList.h"

struct Crew {
    MyString id;
    MyString role;
    MyString base;
    MyVector<MyString> qualifications;
    int seniority;

    Crew() : seniority(0) {}

    bool hasQualification(const MyString& aircraftType) const {
        for (int i = 0; i < qualifications.size(); i++) {
            if (qualifications[i] == aircraftType) {
                return true;
            }
        }
        return false;
    }
};
#endif