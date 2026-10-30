
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
#include "models.h"
#include "conflicts.h"

using namespace std;

inline bool isAssignmentValid(
    const Course& course,
    int slotId,
    int roomId,
    const vector<Course>& courses,
    const vector<Room>& rooms,
    const vector<Assignment>& assignments
) {
    const Room* selectedRoom = nullptr;

    for (const Room& room : rooms) {
        if (room.id == roomId) {
            selectedRoom = &room;
            break;
        }
    }

    if (selectedRoom == nullptr ||
        static_cast<int>(course.studentIds.size()) > selectedRoom->capacity) {
        return false;
    }

    for (const Assignment& a : assignments) {
        if (a.timeSlotId != slotId) {
            continue;
        }

        // A room cannot host two exams at the same time.
        if (a.roomId == roomId) {
            return false;
        }

        // Courses sharing students cannot run simultaneously.
        for (const Course& other : courses) {
            if (other.id == a.courseId && haveConflict(course, other)) {
                return false;
            }
        }
    }

    return true;
}

inline vector<int> getAvailableSlots(
    const Course& course,
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    const vector<Assignment>& assignments
) {
    vector<int> options;

    for (const TimeSlot& slot : slots) {
        for (const Room& room : rooms) {
            if (isAssignmentValid(
                    course, slot.id, room.id,
                    courses, rooms, assignments)) {
                options.push_back(slot.id * 100000 + room.id);
            }
        }
    }

    return options;
}

inline bool backtrack(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments,
    vector<bool>& scheduled
) {
    if (assignments.size() == courses.size()) {
        return true;
    }

    // Most-constrained-variable-first:
    // choose the unscheduled course with the fewest valid options.
    int bestIndex = -1;
    size_t fewestOptions = static_cast<size_t>(-1);

    for (size_t i = 0; i < courses.size(); ++i) {
        if (scheduled[i]) {
            continue;
        }

        vector<int> options = getAvailableSlots(
            courses[i], courses, slots, rooms, assignments);

        if (options.empty()) {
            return false; // Forward-checking failure.
        }

        if (options.size() < fewestOptions) {
            fewestOptions = options.size();
            bestIndex = static_cast<int>(i);
        }
    }

    if (bestIndex == -1) {
        return false;
    }

    const Course& course = courses[bestIndex];
    scheduled[bestIndex] = true;

    for (const TimeSlot& slot : slots) {
        for (const Room& room : rooms) {
            if (!isAssignmentValid(
                    course, slot.id, room.id,
                    courses, rooms, assignments)) {
                continue;
            }

            assignments.push_back({
                course.id, slot.id, room.id
            });

            // Forward-check: ensure every remaining course
            // still has at least one available assignment.
            bool possible = true;

            for (size_t i = 0; i < courses.size(); ++i) {
                if (!scheduled[i] &&
                    getAvailableSlots(
                        courses[i], courses, slots,
                        rooms, assignments).empty()) {
                    possible = false;
                    break;
                }
            }

            if (possible && backtrack(
                    courses, slots, rooms,
                    assignments, scheduled)) {
                return true;
            }

            // Undo the assignment and try another option.
            assignments.pop_back();
        }
    }

    scheduled[bestIndex] = false;
    return false;
}

inline bool scheduleCourses(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments
) {
    assignments.clear();

    if (courses.empty() || slots.empty() || rooms.empty()) {
        return false;
    }

    vector<bool> scheduled(courses.size(), false);

    return backtrack(
        courses, slots, rooms, assignments, scheduled);
}

inline void printTimetable(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    const vector<Assignment>& assignments
) {
    cout << "\n========== FINAL EXAM TIMETABLE ==========\n";

    for (const Assignment& a : assignments) {
        for (const Course& course : courses) {
            if (course.id != a.courseId) {
                continue;
            }

            for (const TimeSlot& slot : slots) {
                if (slot.id != a.timeSlotId) {
                    continue;
                }

                for (const Room& room : rooms) {
                    if (room.id == a.roomId) {
                        cout << course.name
                             << " | " << slot.day
                             << " | " << slot.startTime
                             << "-" << slot.endTime
                             << " | " << room.name << '\n';
                    }
                }
            }
        }
    }
}

#endif
