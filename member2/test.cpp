
#include "conflicts.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {

    // Sample data: 6 courses
    vector<Course> courses = {
        {1, "Programming", {1, 2}},
        {2, "Database", {2, 3}},
        {3, "Mathematics", {4}},
        {4, "Algorithms", {1, 2, 4}},
        {5, "Networking", {5}},
        {6, "Web Development", {3, 6}}
    };

    // Build conflict matrix
    vector<vector<int>> matrix = buildConflictMatrix(courses);

    // Print conflict matrix
    printConflictMatrix(courses, matrix);

    // Build conflict graph
    vector<vector<int>> graph = buildConflictGraph(courses);

    // Print conflict graph
    printConflictGraph(courses, graph);

    // Test hasConflict()
    cout << "\nCONFLICT TESTS\n";

    cout << "Programming and Database: "
         << (hasConflict(courses[0], courses[1]) ? "Conflict" : "No Conflict")
         << endl;

    cout << "Programming and Algorithms: "
         << (hasConflict(courses[0], courses[3]) ? "Conflict" : "No Conflict")
         << endl;

    cout << "Programming and Mathematics: "
         << (hasConflict(courses[0], courses[2]) ? "Conflict" : "No Conflict")
         << endl;

    cout << "Networking and other courses: "
         << (graph[4].empty() ? "No Conflict (Isolated Course)" : "Conflict")
         << endl;

    return 0;
}
