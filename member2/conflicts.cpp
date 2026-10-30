
#include "conflicts.h"
#include <iostream>

using namespace std;

// Check whether two courses share at least one student
bool hasConflict(const Course& courseA, const Course& courseB) {

    for (int studentA : courseA.studentIds) {

        for (int studentB : courseB.studentIds) {

            if (studentA == studentB) {
                return true;
            }
        }
    }

    return false;
}


// Build the conflict matrix
vector<vector<int>> buildConflictMatrix(
    const vector<Course>& courses
) {

    int n = courses.size();

    vector<vector<int>> matrix(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {

        for (int j = i + 1; j < n; j++) {

            if (hasConflict(courses[i], courses[j])) {

                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
        }
    }

    return matrix;
}


// Build the conflict graph using an adjacency list
vector<vector<int>> buildConflictGraph(
    const vector<Course>& courses
) {

    int n = courses.size();

    vector<vector<int>> graph(n);

    for (int i = 0; i < n; i++) {

        for (int j = i + 1; j < n; j++) {

            if (hasConflict(courses[i], courses[j])) {

                graph[i].push_back(j);
                graph[j].push_back(i);
            }
        }
    }

    return graph;
}


// Print the conflict matrix
void printConflictMatrix(
    const vector<Course>& courses,
    const vector<vector<int>>& matrix
) {

    cout << "\nCONFLICT MATRIX\n\n";

    cout << "     ";

    for (const Course& course : courses) {
        cout << course.id << "  ";
    }

    cout << endl;

    for (int i = 0; i < courses.size(); i++) {

        cout << courses[i].id << "    ";

        for (int j = 0; j < courses.size(); j++) {
            cout << matrix[i][j] << "  ";
        }

        cout << endl;
    }
}


// Print the conflict graph
void printConflictGraph(
    const vector<Course>& courses,
    const vector<vector<int>>& graph
) {

    cout << "\nCONFLICT GRAPH\n\n";

    for (int i = 0; i < courses.size(); i++) {

        cout << courses[i].name << " -> ";

        for (int neighbor : graph[i]) {
            cout << courses[neighbor].name << " ";
        }

        cout << endl;
    }
}
