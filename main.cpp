#include <iostream>
#include <string>
#include <vector>
#include "models.h"
#include "input.h"
#include "conflicts.h"
#include "scheduler.h"

using namespace std;

// ============================================================================
// TEST CASE 1: FULL INTEGRATION WITH MEMBER 1 DATA & MEMBER 2 CONFLICT GRAPH
// Demonstrates Member 3's Pure Backtracking vs Member 4's Heuristic Backtracking
// ============================================================================
void runMainIntegrationDemo() {
    cout << "\n====================================================================\n";
    cout << "TEST CASE 1: INTEGRATED DATASET (Member 1 Input + Member 2 Conflicts)\n";
    cout << "====================================================================\n";

    // 1. Member 1 Contribution: Load realistic sample dataset
    vector<Student> students = getSampleStudents();
    vector<Course> courses = getSampleCourses();
    vector<Room> rooms = getSampleRooms();
    vector<TimeSlot> slots = getSampleTimeSlots();

    cout << ">>> MEMBER 1: DATA LOADED FROM input.h <<<\n";
    cout << "- Total Students  : " << students.size() << '\n';
    cout << "- Total Courses   : " << courses.size() << '\n';
    cout << "- Total Rooms     : " << rooms.size() << " (Room A: 50, Room B: 40)\n";
    cout << "- Total Time Slots: " << slots.size() << '\n';

    // Display summary of Member 1 input
    displayCourses(courses);
    displayRooms(rooms);
    displayTimeSlots(slots);

    // Test Member 1 input validation functions
    cout << "\n>>> MEMBER 1: INPUT VALIDATION TEST <<<\n";
    cout << "Course ID 101 valid? " << (isValidCourseID(101, courses) ? "YES" : "NO") << '\n';
    cout << "Course ID 999 valid? " << (isValidCourseID(999, courses) ? "YES" : "NO") << '\n';
    cout << "Room ID 1 valid?     " << (isValidRoomID(1, rooms) ? "YES" : "NO") << '\n';
    cout << "Slot ID 1 valid?     " << (isValidTimeSlotID(1, slots) ? "YES" : "NO") << '\n';

    // 2. Member 2 Contribution: Build and display Conflict Matrix & Graph
    cout << "\n>>> MEMBER 2: CONFLICT DETECTION & GRAPH/MATRIX <<<\n";
    vector<vector<int>> conflictMatrix = buildConflictMatrix(courses);
    printConflictMatrix(courses, conflictMatrix);

    vector<vector<int>> conflictGraph = buildConflictGraph(courses);
    printConflictGraph(courses, conflictGraph);

    // 3. Member 3 Contribution: Core Recursive Backtracking Algorithm
    cout << "\n>>> MEMBER 3: PURE RECURSIVE BACKTRACKING ALGORITHM <<<\n";
    vector<Assignment> standardAssignments;
    SchedulingStats standardStats;

    bool stdSuccess = scheduleCourses(courses, slots, rooms, standardAssignments, standardStats, false);

    if (stdSuccess) {
        cout << "[SUCCESS] Member 3's pure backtracking successfully scheduled all courses!\n";
        printTimetable(courses, slots, rooms, standardAssignments);
        printSchedulingStats(standardStats, "Member 3: Pure Backtracking (Baseline)");
    } else {
        cout << "[FAILED] Member 3's pure backtracking could not find a valid schedule.\n";
        printSchedulingStats(standardStats, "Member 3: Pure Backtracking (Baseline)");
    }

    // 4. Member 4 Heuristic Comparison: MCV + Forward Checking
    cout << "\n>>> MEMBER 4: HEURISTIC-GUIDED BACKTRACKING (MCV + FC) <<<\n";
    vector<Assignment> heuristicAssignments;
    SchedulingStats heuristicStats;

    bool heurSuccess = scheduleCourses(courses, slots, rooms, heuristicAssignments, heuristicStats, true);

    if (heurSuccess) {
        cout << "[SUCCESS] Member 4's heuristic scheduler found a valid schedule!\n";
        printSchedulingStats(heuristicStats, "Member 4: Heuristic-Guided (MCV + FC)");
    } else {
        cout << "[FAILED] Member 4's heuristic scheduler could not find a valid schedule.\n";
        printSchedulingStats(heuristicStats, "Member 4: Heuristic-Guided (MCV + FC)");
    }

    // 5. DAA Performance Telemetry Comparison
    cout << "\n====================================================================\n";
    cout << "           DAA ALGORITHMIC COMPARISON SUMMARY                       \n";
    cout << "====================================================================\n";
    cout << "Algorithm Variant            | Status  | Nodes Explored | Backtracks\n";
    cout << "-----------------------------+---------+----------------+-----------\n";
    cout << "Pure Backtracking (Member 3) | " << (stdSuccess ? "SUCCESS" : "FAILED ")
         << " | " << standardStats.nodesExplored
         << string(15 - to_string(standardStats.nodesExplored).length(), ' ')
         << "| " << standardStats.backtracks << '\n';
    cout << "Heuristic MCV+FC (Member 4)  | " << (heurSuccess ? "SUCCESS" : "FAILED ")
         << " | " << heuristicStats.nodesExplored
         << string(15 - to_string(heuristicStats.nodesExplored).length(), ' ')
         << "| " << heuristicStats.backtracks << '\n';
    cout << "====================================================================\n";
}

// ============================================================================
// TEST CASE 2: CONSTRAINT ENFORCEMENT - ROOM CAPACITY VIOLATION
// ============================================================================
void testCapacityViolationCase() {
    cout << "\n====================================================================\n";
    cout << "TEST CASE 2: HARD CONSTRAINT TEST (Room Capacity Violation)\n";
    cout << "====================================================================\n";

    vector<Course> courses = {
        {201, "Algorithms Lab", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}} // 10 students
    };

    // Rooms can only fit 5 students (< 10 enrolled)
    vector<Room> tinyRooms = {
        {1, "Tiny Room 1", 5},
        {2, "Tiny Room 2", 5}
    };

    vector<TimeSlot> slots = {
        {1, "Monday", "08:00", "10:00"},
        {2, "Monday", "10:30", "12:30"}
    };

    cout << "Testing: Enrolling 10 students into rooms with capacity 5...\n";
    vector<Assignment> assignments;
    SchedulingStats stats;

    bool result = scheduleCourses(courses, slots, tinyRooms, assignments, stats, false);
    cout << "Capacity Constraint Status: "
         << (!result ? "CORRECTLY REJECTED (NO VALID SCHEDULE)" : "UNEXPECTED SUCCESS") << '\n';
    printSchedulingStats(stats, "Capacity Constraint Test");
}

// ============================================================================
// TEST CASE 3: CONSTRAINT ENFORCEMENT - OVER-CONSTRAINED STUDENT CONFLICTS
// ============================================================================
void testOverConstrainedConflictCase() {
    cout << "\n====================================================================\n";
    cout << "TEST CASE 3: HARD CONSTRAINT TEST (Over-Constrained Student Conflicts)\n";
    cout << "====================================================================\n";

    // 3 courses sharing student #1, but only 1 timeslot available in the schedule
    vector<Course> conflictingCourses = {
        {301, "Course A", {1}},
        {302, "Course B", {1}},
        {303, "Course C", {1}}
    };

    vector<Room> rooms = {
        {1, "Main Hall A", 100},
        {2, "Main Hall B", 100}
    };

    vector<TimeSlot> singleSlot = {
        {1, "Monday", "08:00", "10:00"}
    };

    cout << "Testing: 3 mutually clashing courses competing for 1 time slot...\n";
    vector<Assignment> assignments;
    SchedulingStats stats;

    bool result = scheduleCourses(conflictingCourses, singleSlot, rooms, assignments, stats, false);
    cout << "Conflict Constraint Status: "
         << (!result ? "CORRECTLY REJECTED (NO VALID SCHEDULE)" : "UNEXPECTED SUCCESS") << '\n';
    printSchedulingStats(stats, "Conflict Constraint Test");
}

// ============================================================================
// TEST CASE 4: EDGE CASES (Empty Inputs)
// ============================================================================
void testEdgeCases() {
    cout << "\n====================================================================\n";
    cout << "TEST CASE 4: EDGE CASES (Empty Inputs)\n";
    cout << "====================================================================\n";

    vector<Course> emptyCourses;
    vector<Room> rooms = {{1, "Room A", 50}};
    vector<TimeSlot> slots = {{1, "Monday", "08:00", "10:00"}};
    vector<Assignment> assignments;
    SchedulingStats stats;

    // Subcase 4A: 0 courses to schedule
    bool emptySuccess = scheduleCourses(emptyCourses, slots, rooms, assignments, stats, false);
    cout << "Subcase 4A (0 Courses): " << (emptySuccess ? "SUCCESS (Empty Schedule)" : "FAILED") << '\n';

    // Subcase 4B: Courses exist but 0 rooms available
    vector<Course> someCourses = {{401, "Discrete Mathematics", {1, 2}}};
    vector<Room> noRooms;
    bool noRoomSuccess = scheduleCourses(someCourses, slots, noRooms, assignments, stats, false);
    cout << "Subcase 4B (0 Rooms)  : " << (!noRoomSuccess ? "CORRECTLY REJECTED" : "UNEXPECTED SUCCESS") << '\n';

    // Subcase 4C: Courses exist but 0 slots available
    vector<TimeSlot> noSlots;
    bool noSlotSuccess = scheduleCourses(someCourses, noSlots, rooms, assignments, stats, false);
    cout << "Subcase 4C (0 Slots)  : " << (!noSlotSuccess ? "CORRECTLY REJECTED" : "UNEXPECTED SUCCESS") << '\n';
    cout << "====================================================================\n";
}

int main() {
    cout << "====================================================================\n";
    cout << "       EXAM TIMETABLE SCHEDULING SYSTEM (DAA GROUP PROJECT)        \n";
    cout << "       Member 1: Input Management                                 \n";
    cout << "       Member 2: Conflict Graph & Adjacency Matrix                \n";
    cout << "       Member 3: Core Recursive Backtracking Algorithm            \n";
    cout << "====================================================================\n";

    runMainIntegrationDemo();
    testCapacityViolationCase();
    testOverConstrainedConflictCase();
    testEdgeCases();

    return 0;
}
