#ifndef MEMBER2_CONFLICTS_H
#define MEMBER2_CONFLICTS_H

#include "../models.h"
#include <vector>

using namespace std;

bool hasConflict(const Course& courseA, const Course& courseB);

vector<vector<int>> buildConflictMatrix(
    const vector<Course>& courses
);

vector<vector<int>> buildConflictGraph(
    const vector<Course>& courses
);

void printConflictMatrix(
    const vector<Course>& courses,
    const vector<vector<int>>& matrix
);

void printConflictGraph(
    const vector<Course>& courses,
    const vector<vector<int>>& graph
);

#endif