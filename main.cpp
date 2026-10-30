
#include <iostream>
#include <vector>

#include "models.h"
#include "input.h"
#include "conflicts.h"
#include "scheduler.h"

using namespace std;

int main() {

    // Get sample data from input.h
    vector<Student> students = getSampleStudents();
    vector<Course> courses = getSampleCourses();
    vector<Room> rooms = getSampleRooms();
    vector<TimeSlot> slots = getSampleTimeSlots();

    // Display project information
    cout << "========================================\n";
    cout << " COURSE TIMETABLE / EXAM SCHEDULING SYSTEM\n";
    cout << "========================================\n";

    cout << "Students: " << students.size() << '\n';
    cout << "Courses: " << courses.size() << '\n';
    cout << "Rooms: " << rooms.size() << '\n';
    cout << "Time Slots: " << slots.size() << '\n';

    // Display input information
    displayStudents(students);
    displayCourses(courses);
    displayRooms(rooms);
    displayTimeSlots(slots);
    cout << "\n========== INPUT VALIDATION ==========\n";

// Test Course ID
cout << "Course ID 101: "
     << (isValidCourseID(101, courses) ? "Valid" : "Invalid") << endl;

cout << "Course ID 999: "
     << (isValidCourseID(999, courses) ? "Valid" : "Invalid") << endl;

// Test Student ID
cout << "Student ID 1: "
     << (isValidStudentID(1, students) ? "Valid" : "Invalid") << endl;

cout << "Student ID 999: "
     << (isValidStudentID(999, students) ? "Valid" : "Invalid") << endl;

// Test Room ID
cout << "Room ID 1: "
     << (isValidRoomID(1, rooms) ? "Valid" : "Invalid") << endl;

cout << "Room ID 999: "
     << (isValidRoomID(999, rooms) ? "Valid" : "Invalid") << endl;

// Test Time Slot ID
cout << "Time Slot ID 1: "
     << (isValidTimeSlotID(1, slots) ? "Valid" : "Invalid") << endl;

cout << "Time Slot ID 999: "
     << (isValidTimeSlotID(999, slots) ? "Valid" : "Invalid") << endl;

    // Check course conflicts
    cout << "\n========== COURSE CONFLICTS ==========\n";

    for (size_t i = 0; i < courses.size(); ++i) {

        for (size_t j = i + 1; j < courses.size(); ++j) {

            if (haveConflict(courses[i], courses[j])) {

                cout << courses[i].name
                     << " conflicts with "
                     << courses[j].name << '\n';
            }
        }
    }

    // Run scheduling algorithm
    vector<Assignment> assignments;

    cout << "\n========== SCHEDULING ==========\n";

    if (scheduleCourses(courses, slots, rooms, assignments)) {

        cout << "All courses scheduled successfully!\n";

        printTimetable(courses, slots, rooms, assignments);

    } else {

        cout << "No valid timetable found with the available "
             << "rooms and time slots.\n";
    }

    return 0;
}
