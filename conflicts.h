
#ifndef CONFLICTS_H
#define CONFLICTS_H

#include "models.h"

// Returns true if two courses share at least one student.
inline bool haveConflict(const Course& course1, const Course& course2) {
    for (int student1 : course1.studentIds) {
        for (int student2 : course2.studentIds) {
            if (student1 == student2) {
                return true;
            }
        }
    }

    return false;
}

#endif
