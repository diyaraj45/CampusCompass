#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
using namespace std;

struct Student{
    string name;
    int studentID;
    int residenceLocationID;
    vector<string> classCodes;
};


struct ClassInformation{
    string classCode;
    int locationID;
    string starttime;
    string endtime;
};


class Graph {
private:
    unordered_map<int, vector<pair<int, int>>> adjacencyList;
    set<pair<int, int>> closedEdges; //imma use this to track the closededges
    unordered_map<int, Student> students;
    //load the classes into this right heeyuh
    unordered_map<string, ClassInformation> classes;

    //these will be the helpers
    pair<int, int> makeEdge(int a, int b) const;
    unordered_map<int, int> dijkstraFunc(int start) const;
    pair<int, vector<int>> dijkstraPath(int start, int dist) const;

    //these will validate studentID, name, and classcode
    bool validateStudentID(const string& studentID) const;
    bool validateStudentName(const string& name) const;
    bool validateClassCode(const string& classCode) const;

public:
    //gotta get your data from somewhere
    void addEdge(int a, int b, int weight);
    void addClass(ClassInformation info);
    void loadEdges(string& filename);
    void loadClasses(string& filename);

    //fix the parameters bruh
    string insert(string STUDENT_NAME, int STUDENT_ID, int RESIDENCE_LOCATION_ID, vector<string> CLASSCODE);
    // may need a helper function for this one

    string remove(int STUDENT_ID);
    //also need a helper for this

    string dropClass(int STUDENT_ID, const string& CLASSCODE);
    //prolly need a helper here too duh

    string replaceClass(int STUDENT_ID, const string& CLASSCODE1, const string& CLASSCODE2);
    //also need a helper for this one too

    string removeClass(const string& CLASSCODE);
    //you know the drill

    //how tf do i do this one
    string toggleEdgesClosure(vector<pair<int, int>> edges);

    string checkEdgeStatus(int LOCATION_IDX, int LOCATION_IDY) const;
    //so this one only prints out open, closed or DNE

    string isConnected(int LOCATION_ID1, int LOCATION_ID2) const;

    //oh my fureaking god with this one
    void printShortestEdges(int STUDENT_ID) const;

    void printStudentZone(int studentID) const;
};

