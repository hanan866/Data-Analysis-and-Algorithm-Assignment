
#include <iostream>
#include <string>
#include <vector>
#include "models.h"
#include "conflicts.h"
#include "scheduler.h"

using namespace std;

int main() {
    vector<Student> students;

    for (int i = 1; i <= 10; ++i) {
        students.push_back({
            i, "Student " + to_string(i), {}
        });
    }

    vector<Course> courses = {
        {101, "Mathematics", {1, 2, 3, 4}},
        {102, "Programming", {3, 4, 5, 6}},
        {103, "Database", {1, 5, 7, 8}},
        {104, "Networking", {2, 6, 9, 10}},
        {105, "Artificial Intelligence", {1, 3, 9}},
        {106, "Software Engineering", {4, 7, 8, 10}}
    };

    vector<Room> rooms = {
    {1, "Room A", 2},
    {2, "Room B", 2}
};

    vector<TimeSlot> slots = {
    {1, "Monday", "08:00", "10:00"},
    {2, "Monday", "10:30", "12:30"},
    {3, "Tuesday", "08:00", "10:00"},
    {4, "Tuesday", "10:30", "12:30"}
};

    cout << "COURSE TIMETABLE / EXAM SCHEDULING SYSTEM\n";
    cout << "Students: " << students.size() << '\n';
    cout << "Courses: " << courses.size() << '\n';
    cout << "Rooms: " << rooms.size() << '\n';
    cout << "Time slots: " << slots.size() << '\n';

    cout << "\nChecking course conflicts...\n";

    for (size_t i = 0; i < courses.size(); ++i) {
        for (size_t j = i + 1; j < courses.size(); ++j) {
            if (haveConflict(courses[i], courses[j])) {
                cout << courses[i].name
                     << " conflicts with "
                     << courses[j].name << '\n';
            }
        }
    }

    vector<Assignment> assignments;

    cout << "\nRunning backtracking scheduler...\n";

    if (scheduleCourses(courses, slots, rooms, assignments)) {
        cout << "All courses scheduled successfully!\n";
        printTimetable(courses, slots, rooms, assignments);
    } else {
        cout << "No valid timetable found with the available "
             << "rooms and time slots.\n";
    }

    return 0;
}
