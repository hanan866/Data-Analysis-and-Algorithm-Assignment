#ifndef CONFLICTS_H
#define CONFLICTS_H

#include "models.h"
#include <vector>
#include <iostream>

using namespace std;

// ============================================================================
// MEMBER 2: CONFLICT DETECTION & GRAPH/MATRIX ALGORITHMS
// ============================================================================

// Check whether two courses share at least one student
inline bool hasConflict(const Course& courseA, const Course& courseB) {
    for (int studentA : courseA.studentIds) {
        for (int studentB : courseB.studentIds) {
            if (studentA == studentB) {
                return true;
            }
        }
    }
    return false;
}

// Backward-compatible alias for haveConflict
inline bool haveConflict(const Course& course1, const Course& course2) {
    return hasConflict(course1, course2);
}

// Build the conflict matrix (Adjacency Matrix: 1 for conflict, 0 otherwise)
inline vector<vector<int>> buildConflictMatrix(const vector<Course>& courses) {
    int n = static_cast<int>(courses.size());
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

// Build the conflict graph (Adjacency List: lists indices of conflicting courses)
inline vector<vector<int>> buildConflictGraph(const vector<Course>& courses) {
    int n = static_cast<int>(courses.size());
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
inline void printConflictMatrix(
    const vector<Course>& courses,
    const vector<vector<int>>& matrix
) {
    cout << "\n==================== CONFLICT MATRIX ====================\n";
    cout << "Course ID ";
    for (const Course& course : courses) {
        cout << course.id << "  ";
    }
    cout << "\n---------------------------------------------------------\n";

    for (size_t i = 0; i < courses.size(); i++) {
        cout << "  " << courses[i].id << "     ";
        for (size_t j = 0; j < courses.size(); j++) {
            cout << matrix[i][j] << "   ";
        }
        cout << '\n';
    }
    cout << "=========================================================\n";
}

// Print the conflict graph
inline void printConflictGraph(
    const vector<Course>& courses,
    const vector<vector<int>>& graph
) {
    cout << "\n==================== CONFLICT GRAPH =====================\n";
    for (size_t i = 0; i < courses.size(); i++) {
        cout << courses[i].name << " (" << courses[i].id << ") conflicts with -> ";
        if (graph[i].empty()) {
            cout << "[None - Isolated Course]";
        } else {
            for (size_t k = 0; k < graph[i].size(); ++k) {
                int neighbor = graph[i][k];
                cout << courses[neighbor].name;
                if (k + 1 < graph[i].size()) cout << ", ";
            }
        }
        cout << '\n';
    }
    cout << "=========================================================\n";
}

#endif
