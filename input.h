
#ifndef INPUT_H
#define INPUT_H

#include "models.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// =====================================
// 1. CREATE 40 SAMPLE STUDENTS
// =====================================

inline vector<Student> getSampleStudents() {

    vector<Student> students;

    for (int i = 1; i <= 40; i++) {

        int course1 = 101 + ((i - 1) % 6);
        int course2 = 101 + ((i + 1) % 6);

        students.push_back({
            i,
            "Student " + to_string(i),
            {course1, course2}
        });
    }

    return students;
}


// =====================================
// 2. CREATE 6 SAMPLE COURSES
// =====================================

inline vector<Course> getSampleCourses() {

    vector<Course> courses = {
        {101, "Programming", {}},
        {102, "Data Structures", {}},
        {103, "Database Systems", {}},
        {104, "Computer Networks", {}},
        {105, "Software Engineering", {}},
        {106, "Artificial Intelligence", {}}
    };

    vector<Student> students = getSampleStudents();

    // Connect students with their courses
    for (const Student& student : students) {

        for (int courseId : student.courseIds) {

            for (Course& course : courses) {

                if (course.id == courseId) {
                    course.studentIds.push_back(student.id);
                }
            }
        }
    }

    return courses;
}


// =====================================
// 3. CREATE 2 SAMPLE ROOMS
// =====================================

inline vector<Room> getSampleRooms() {

    return {
        {1, "Room A", 50},
        {2, "Room B", 40}
    };
}


// =====================================
// 4. CREATE 4 SAMPLE TIME SLOTS
// =====================================

inline vector<TimeSlot> getSampleTimeSlots() {

    return {
        {1, "Monday", "08:00", "10:00"},
        {2, "Monday", "10:30", "12:30"},
        {3, "Tuesday", "08:00", "10:00"},
        {4, "Tuesday", "10:30", "12:30"}
    };
}


// =====================================
// 5. DISPLAY STUDENTS
// =====================================

inline void displayStudents(const vector<Student>& students) {

    cout << "\n========== STUDENT INFORMATION ==========\n";

    for (const Student& student : students) {

        cout << "Student ID: " << student.id << endl;
        cout << "Name: " << student.name << endl;

        cout << "Courses: ";

        for (int courseId : student.courseIds) {
            cout << courseId << " ";
        }

        cout << "\n-----------------------------------------\n";
    }
}


// =====================================
// 6. DISPLAY COURSES
// =====================================

inline void displayCourses(const vector<Course>& courses) {

    cout << "\n========== COURSE INFORMATION ==========\n";

    for (const Course& course : courses) {

        cout << "Course ID: " << course.id << endl;
        cout << "Course Name: " << course.name << endl;

        cout << "Enrolled Students: ";

        for (int studentId : course.studentIds) {
            cout << studentId << " ";
        }

        cout << "\nTotal Students: "
             << course.studentIds.size() << endl;

        cout << "-----------------------------------------\n";
    }
}


// =====================================
// 7. DISPLAY ROOMS
// =====================================

inline void displayRooms(const vector<Room>& rooms) {

    cout << "\n========== ROOM INFORMATION ==========\n";

    for (const Room& room : rooms) {

        cout << "Room ID: " << room.id << endl;
        cout << "Room Name: " << room.name << endl;
        cout << "Capacity: " << room.capacity << endl;

        cout << "-----------------------------------------\n";
    }
}


// =====================================
// 8. DISPLAY TIME SLOTS
// =====================================

inline void displayTimeSlots(const vector<TimeSlot>& slots) {

    cout << "\n========== TIME SLOT INFORMATION ==========\n";

    for (const TimeSlot& slot : slots) {

        cout << "Time Slot ID: " << slot.id << endl;
        cout << "Day: " << slot.day << endl;
        cout << "Start Time: " << slot.startTime << endl;
        cout << "End Time: " << slot.endTime << endl;

        cout << "-----------------------------------------\n";
    }
}


// =====================================
// 9. VALIDATE COURSE ID
// =====================================

inline bool isValidCourseID(
    int courseId,
    const vector<Course>& courses
) {

    for (const Course& course : courses) {

        if (course.id == courseId) {
            return true;
        }
    }

    return false;
}


// =====================================
// 10. VALIDATE STUDENT ID
// =====================================

inline bool isValidStudentID(
    int studentId,
    const vector<Student>& students
) {

    for (const Student& student : students) {

        if (student.id == studentId) {
            return true;
        }
    }

    return false;
}


// =====================================
// 11. VALIDATE ROOM ID
// =====================================

inline bool isValidRoomID(
    int roomId,
    const vector<Room>& rooms
) {

    for (const Room& room : rooms) {

        if (room.id == roomId) {
            return true;
        }
    }

    return false;
}


// =====================================
// 12. VALIDATE TIME SLOT ID
// =====================================

inline bool isValidTimeSlotID(
    int slotId,
    const vector<TimeSlot>& slots
) {

    for (const TimeSlot& slot : slots) {

        if (slot.id == slotId) {
            return true;
        }
    }

    return false;
}


// =====================================
// 13. VALIDATE ROOM CAPACITY
// =====================================

inline bool hasEnoughRoomCapacity(
    int courseId,
    int roomId,
    const vector<Course>& courses,
    const vector<Room>& rooms
) {

    for (const Course& course : courses) {

        if (course.id == courseId) {

            for (const Room& room : rooms) {

                if (room.id == roomId) {

                    return room.capacity >=
                           static_cast<int>(course.studentIds.size());
                }
            }
        }
    }

    return false;
}

#endif
