#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include "models.h"
#include "conflicts.h"

using namespace std;

// Performance metrics for DAA analysis
struct SchedulingStats {
    long long nodesExplored = 0;
    long long backtracks = 0;
    bool success = false;
};

// ============================================================================
// CONSTRAINT CHECKING (Hard Constraints)
// 1. Room exists and has sufficient capacity for enrolled students.
// 2. Room is not double-booked in the same time slot.
// 3. No two courses sharing students are scheduled in the same time slot.
// ============================================================================
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

    // Constraint 1: Room capacity
    if (selectedRoom == nullptr ||
        static_cast<int>(course.studentIds.size()) > selectedRoom->capacity) {
        return false;
    }

    for (const Assignment& a : assignments) {
        if (a.timeSlotId != slotId) {
            continue;
        }

        // Constraint 2: Room cannot host two exams at the same time
        if (a.roomId == roomId) {
            return false;
        }

        // Constraint 3: Courses sharing students cannot run simultaneously
        for (const Course& other : courses) {
            if (other.id == a.courseId && haveConflict(course, other)) {
                return false;
            }
        }
    }

    return true;
}

// Helper to count available (slot, room) options for a course
inline int countAvailableOptions(
    const Course& course,
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    const vector<Assignment>& assignments
) {
    int count = 0;
    for (const TimeSlot& slot : slots) {
        for (const Room& room : rooms) {
            if (isAssignmentValid(course, slot.id, room.id, courses, rooms, assignments)) {
                count++;
            }
        }
    }
    return count;
}

// Backward-compatible slot querying for existing tests
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
            if (isAssignmentValid(course, slot.id, room.id, courses, rooms, assignments)) {
                options.push_back(slot.id * 100000 + room.id);
            }
        }
    }
    return options;
}

// ============================================================================
// MEMBER 3: PURE RECURSIVE BACKTRACKING ALGORITHM (BASELINE)
// Explores the state space course-by-course in sequential order.
// ============================================================================
inline bool backtrackStandard(
    size_t courseIndex,
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments,
    SchedulingStats& stats
) {
    stats.nodesExplored++;

    // Base Case: All courses have been successfully assigned
    if (courseIndex == courses.size()) {
        stats.success = true;
        return true;
    }

    const Course& currentCourse = courses[courseIndex];

    // Try all combinations of time slots and rooms
    for (const TimeSlot& slot : slots) {
        for (const Room& room : rooms) {
            if (isAssignmentValid(currentCourse, slot.id, room.id, courses, rooms, assignments)) {
                // 1. Choose: Tentative assignment
                assignments.push_back({currentCourse.id, slot.id, room.id});

                // 2. Recurse: Move to next course
                if (backtrackStandard(courseIndex + 1, courses, slots, rooms, assignments, stats)) {
                    return true;
                }

                // 3. Backtrack: Undo assignment and explore next possibility
                assignments.pop_back();
                stats.backtracks++;
            }
        }
    }

    return false;
}

// ============================================================================
// HEURISTIC-GUIDED BACKTRACKING (MEMBER 4: MCV / MRV + FORWARD-CHECKING)
// Preserved cleanly to enable DAA algorithmic comparison.
// ============================================================================
inline bool backtrackHeuristic(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments,
    vector<bool>& scheduled,
    SchedulingStats& stats
) {
    stats.nodesExplored++;

    if (assignments.size() == courses.size()) {
        stats.success = true;
        return true;
    }

    // Heuristic 1: Most Constrained Variable (Minimum Remaining Values)
    int bestIndex = -1;
    int fewestOptions = INT_MAX;

    for (size_t i = 0; i < courses.size(); ++i) {
        if (scheduled[i]) continue;

        int optionsCount = countAvailableOptions(courses[i], courses, slots, rooms, assignments);

        // Forward-checking pruning: If any remaining course has 0 options, prune immediately
        if (optionsCount == 0) {
            return false;
        }

        if (optionsCount < fewestOptions) {
            fewestOptions = optionsCount;
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
            if (!isAssignmentValid(course, slot.id, room.id, courses, rooms, assignments)) {
                continue;
            }

            assignments.push_back({course.id, slot.id, room.id});

            // Forward-check lookahead
            bool possible = true;
            for (size_t i = 0; i < courses.size(); ++i) {
                if (!scheduled[i] && countAvailableOptions(courses[i], courses, slots, rooms, assignments) == 0) {
                    possible = false;
                    break;
                }
            }

            if (possible && backtrackHeuristic(courses, slots, rooms, assignments, scheduled, stats)) {
                return true;
            }

            // Undo assignment
            assignments.pop_back();
            stats.backtracks++;
        }
    }

    scheduled[bestIndex] = false;
    return false;
}

// ============================================================================
// SCHEDULER ENTRY POINTS
// ============================================================================

// Overload with stats tracking
inline bool scheduleCourses(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments,
    SchedulingStats& stats,
    bool useHeuristic = false
) {
    assignments.clear();
    stats = SchedulingStats();

    // Edge Case: 0 courses to schedule trivially succeeds with an empty schedule
    if (courses.empty()) {
        stats.success = true;
        return true;
    }

    // Edge Case: Courses exist but no available rooms or slots
    if (slots.empty() || rooms.empty()) {
        stats.success = false;
        return false;
    }

    if (useHeuristic) {
        vector<bool> scheduled(courses.size(), false);
        return backtrackHeuristic(courses, slots, rooms, assignments, scheduled, stats);
    } else {
        return backtrackStandard(0, courses, slots, rooms, assignments, stats);
    }
}

// Original signature for full backward-compatibility
inline bool scheduleCourses(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments
) {
    SchedulingStats unusedStats;
    return scheduleCourses(courses, slots, rooms, assignments, unusedStats, false);
}

// Backward-compatible backtrack function signature
inline bool backtrack(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    vector<Assignment>& assignments,
    vector<bool>& scheduled
) {
    SchedulingStats stats;
    return backtrackHeuristic(courses, slots, rooms, assignments, scheduled, stats);
}

// ============================================================================
// DISPLAY UTILITIES
// ============================================================================
inline void printTimetable(
    const vector<Course>& courses,
    const vector<TimeSlot>& slots,
    const vector<Room>& rooms,
    const vector<Assignment>& assignments
) {
    cout << "\n==================== FINAL EXAM TIMETABLE ====================\n";
    cout << "Course (Enrolled)         | Day       | Time          | Room (Cap)\n";
    cout << "--------------------------------------------------------------\n";

    for (const Assignment& a : assignments) {
        string courseInfo = "";
        for (const Course& c : courses) {
            if (c.id == a.courseId) {
                courseInfo = c.name + " (" + to_string(c.studentIds.size()) + ")";
                break;
            }
        }

        string slotInfo = "";
        for (const TimeSlot& s : slots) {
            if (s.id == a.timeSlotId) {
                slotInfo = s.day + " | " + s.startTime + "-" + s.endTime;
                break;
            }
        }

        string roomInfo = "";
        for (const Room& r : rooms) {
            if (r.id == a.roomId) {
                roomInfo = r.name + " (" + to_string(r.capacity) + ")";
                break;
            }
        }

        cout << courseInfo;
        if (courseInfo.length() < 26) cout << string(26 - courseInfo.length(), ' ');
        cout << "| " << slotInfo << " | " << roomInfo << '\n';
    }
    cout << "==============================================================\n";
}

inline void printSchedulingStats(const SchedulingStats& stats, const string& methodLabel = "Backtracking") {
    cout << "\n--- Performance Telemetry (" << methodLabel << ") ---\n";
    cout << "Search Status   : " << (stats.success ? "SUCCESS" : "FAILED / NO SOLUTION") << '\n';
    cout << "Nodes Explored  : " << stats.nodesExplored << '\n';
    cout << "Backtracks Made : " << stats.backtracks << '\n';
    cout << "---------------------------------------------------\n";
}

#endif
