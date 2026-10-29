
#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

using namespace std;

struct Course {
    int id;
    string name;
    vector<int> studentIds;
};

struct Student {
    int id;
    string name;
    vector<int> courseIds;
};

struct Room {
    int id;
    string name;
    int capacity;
};

struct TimeSlot {
    int id;
    string day;
    string startTime;
    string endTime;
};

struct Assignment {
    int courseId;
    int timeSlotId;
    int roomId;
};

#endif
