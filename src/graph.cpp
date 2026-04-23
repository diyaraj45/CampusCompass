#include "graph.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <queue>
using namespace std;

void Graph::loadClasses(const string& filename) {
    ifstream file(filename);
    string line;
    getline(file, line); 
    while (getline(file, line)) {
         if (line.empty()) continue;
        stringstream ss(line);
        ClassInformation info;
        string locationStr;

        getline(ss, info.classCode, ',');    
        getline(ss, locationStr, ',');        
        getline(ss, info.starttime, ',');     
        getline(ss, info.endtime, ',');       

        info.locationID = stoi(locationStr);
        addClass(info);
    }
}

void Graph::addClass(ClassInformation info) {
    classes[info.classCode] = info;
}

void Graph::loadEdges(const string& filename) {
    ifstream file(filename);
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        int a, b, weight;
        string astring, bstring, name1, name2, weightstring;
        getline(ss, astring, ',');
        getline(ss, bstring, ',');
        getline(ss, name1, ',');
        getline(ss, name2, ',');
        getline(ss, weightstring);
        a = stoi(astring);
        b = stoi(bstring);
        weight = stoi(weightstring);
        addEdge(a, b, weight);
    }
}

void Graph::addEdge(int from, int to, int weight) {
    adjacencyList[from].push_back({to, weight});
    adjacencyList[to].push_back({from, weight});
};

bool Graph::validateStudentID(const string& studentID) const {
    if (studentID.empty()) {
        return false;
    }
    if (studentID.length() != 8) {
        return false;
    }
    for (char c : studentID) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool Graph::validateStudentName(const string& name) const {
    if (name.empty()) {
        return false;
    }
    for (char c : name) {
        if (!isalpha(c) && c != ' '){
            return false;
        }
    }
    return true;
}

bool Graph::validateClassCode(const string& classCode) const {
    if (classCode.length() != 7) {
        return false;
    }
    for (int i = 0; i < 3; i++) {
        if (!isupper(classCode[i])) {
            return false;
        }
    }
    for (int i = 3; i < 7; i++) {
        if (!isdigit(classCode[i])) {
            return false;
        }
    }
    return true;
}

string Graph::insert(string STUDENT_NAME, int STUDENT_ID, int RESIDENCE_LOCATION_ID, vector<string> CLASSCODE){
    if (!validateStudentName(STUDENT_NAME)) {
        return "unsuccessful";
    }
    if (!validateStudentID(to_string(STUDENT_ID))) {
        return "unsuccessful";
    }
    
    if (students.count(STUDENT_ID)) {
        return "unsuccessful";
    }

    if (adjacencyList.find(RESIDENCE_LOCATION_ID) == adjacencyList.end()) {
        return "unsuccessful";
    }

    if (CLASSCODE.empty() || CLASSCODE.size() > 6) {
        return "unsuccessful";
    }

    for (const string& code : CLASSCODE) {
        if (!validateClassCode(code) || classes.find(code) == classes.end()) {
            return "unsuccessful";
        }
    }

    Student student;
    student.name = STUDENT_NAME;
    student.studentID = STUDENT_ID;
    student.residenceLocationID = RESIDENCE_LOCATION_ID;
    student.classCodes = CLASSCODE;
    students[STUDENT_ID] = student;
    return "successful";
}

string Graph::remove(int STUDENT_ID) {
    if (students.find(STUDENT_ID) == students.end()) {
        return "unsuccessful";
    }
    students.erase(STUDENT_ID);
    return "successful";
}

string Graph::dropClass(int STUDENT_ID, const string& CLASSCODE){
    if (students.find(STUDENT_ID) == students.end()) {
        return "unsuccessful";
    }
    Student& student = students[STUDENT_ID];
    auto it = find(student.classCodes.begin(), student.classCodes.end(), CLASSCODE);
    if (it == student.classCodes.end()) {
        return "unsuccessful";
    }
    student.classCodes.erase(it);
    if (student.classCodes.empty()) {
        students.erase(STUDENT_ID);
    }
    return "successful";
}

string Graph::replaceClass(int STUDENT_ID, const string& CLASSCODE1, const string& CLASSCODE2) {
    if (students.find(STUDENT_ID) == students.end()) {
        return "unsuccessful";
    }
    Student& student = students[STUDENT_ID];
    auto it = find(student.classCodes.begin(), student.classCodes.end(), CLASSCODE1);
    if (it == student.classCodes.end()) {
        return "unsuccessful";
    }

    if (find(student.classCodes.begin(), student.classCodes.end(), CLASSCODE2) != student.classCodes.end()) {
        return "unsuccessful";
    }

    if (classes.find(CLASSCODE2) == classes.end()) {
        return "unsuccessful";
    }

    *it = CLASSCODE2;
    return "successful";
}

string Graph::removeClass(const string& CLASSCODE) {
    if (!validateClassCode(CLASSCODE)) {
        return "unsuccessful";
    }

    if (classes.find(CLASSCODE) == classes.end()) {
        return "unsuccessful";
    }

    int count = 0;
    vector<int> needtoremove;

    for (auto& [id, student] : students) {
        auto it = find(student.classCodes.begin(), student.classCodes.end(), CLASSCODE);
        if (it != student.classCodes.end()) {
            student.classCodes.erase(it);
            count++;
            if (student.classCodes.empty()) {
                needtoremove.push_back(id);
            }
        }
    }

    if (count ==0) {
        return "unsuccessful";
    }

    for (int id : needtoremove) {
        students.erase(id);
    }

    return to_string(count);
}

pair<int, int> Graph::makeEdge(int a, int b) const  {
    return {min(a,b), max(a,b)};
}

string Graph::toggleEdgesClosure(vector<pair<int, int>> edges) {
    for (auto& [a, b] : edges) {
        pair<int, int> edge = makeEdge(a, b);
        if (closedEdges.find(edge) != closedEdges.end()) {
            closedEdges.erase(edge);
        } else {
            closedEdges.insert(edge);
        }
    }
    return "successful";
}

string Graph::checkEdgeStatus(int LOCATION_IDX, int LOCATION_IDY) const {
    if (adjacencyList.find((LOCATION_IDX)) == adjacencyList.end() || adjacencyList.find((LOCATION_IDY)) == adjacencyList.end()) {
        return "DNE";
    }

    bool edgeExists = false;
    for (const auto& [neighbor, weight] : adjacencyList.at((LOCATION_IDX))) {
        if (neighbor == (LOCATION_IDY)) {
            edgeExists = true;
            break;
        }
    }

    if (!edgeExists) {
        return "DNE";
    }

    pair<int, int> edge = makeEdge((LOCATION_IDX), (LOCATION_IDY));
    if (closedEdges.find(edge) != closedEdges.end()) {
        return "closed";
    } else {
        return "open";
    }
}

string Graph::isConnected(int LOCATION_ID1, int LOCATION_ID2) const {
    if (LOCATION_ID1 == LOCATION_ID2) {
        return "successful";
    }

    if (adjacencyList.find(LOCATION_ID1) == adjacencyList.end() || adjacencyList.find(LOCATION_ID2) == adjacencyList.end()) {
        return "unsuccessful";
    }

    unordered_set<int> visited;
    queue<int> q;
    q.push(LOCATION_ID1);
    visited.insert(LOCATION_ID1);
    while(!q.empty()) {
        int curr = q.front();
        q.pop();
        for (const auto& [neighbor, weight] : adjacencyList.at(curr)) {
            if (visited.count(neighbor)){
                continue;
            }
            if (closedEdges.count(makeEdge(curr, neighbor))){
                continue;
            }
            if (neighbor == LOCATION_ID2) {
                return "successful";
            }
            visited.insert(neighbor);
            q.push(neighbor);
        }
    }
    return "unsuccessful";
}

unordered_map<int, int> Graph::dijkstraFunc(int start) const {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    unordered_map<int, int> distances;
    for (const auto& [node, _] : adjacencyList) {
        distances[node] = INT_MAX;
    }
    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [dist, node] = pq.top();
        pq.pop();
        if (dist > distances[node]) continue;
        for (const auto& [neighbor, weight] : adjacencyList.at(node)) {
            if (closedEdges.count(makeEdge(node, neighbor))) {
                continue;
            }
            int newDist = dist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                pq.push({newDist, neighbor});
            }
        }
    }
    return distances;
}

pair<int, vector<int>> Graph::dijkstraPath(int start, int dist) const {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    unordered_map<int, int> distances;
    unordered_map<int, int> previous;

    for (const auto& [node, _] : adjacencyList) {
        distances[node] = INT_MAX;
    }

    distances[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currDist, node] = pq.top();
        pq.pop();

        if (currDist > distances[node]) continue;
        
        for (const auto& [neighbor, weight] : adjacencyList.at(node)) {
            if (closedEdges.count(makeEdge(node, neighbor))) {
                continue;
            }
            int newDist = currDist + weight;
            if (newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                previous[neighbor] = node;
                pq.push({newDist, neighbor});
            }
        }
    }

    vector<int> path;
    if (distances[dist] == INT_MAX) {
        return {-1, {}};
    }
    for (int at = dist; at != start; at = previous[at]) {
        path.push_back(at);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());

    return {distances[dist], path};
}

void Graph::printShortestEdges(int STUDENT_ID) const {
    const Student& student = students.at(STUDENT_ID);
    cout << "Time For Shortest Edges: " << student.name << endl;

    auto distances = dijkstraFunc(student.residenceLocationID);

    vector<string> sorted = student.classCodes;
    sort(sorted.begin(), sorted.end());

    for (const string& classCode : sorted) {
        int locationID = classes.at(classCode).locationID;
        int dist = -1;
        if (distances.count(locationID)){
            dist = distances.at(locationID);
        }
        if (dist == INT_MAX) {
            cout << classCode << ": -1"<< endl;
        } else {
            cout << classCode << ": " << dist << endl;
        }
    }
}

void Graph::printStudentZone(int studentID) const {
    const Student& student = students.at(studentID);
    
    unordered_set<int> nodeset;
    nodeset.insert(student.residenceLocationID);

    for (const string& classCode : student.classCodes) {
        int classLocation = classes.at(classCode).locationID;
        auto [dist, path] = dijkstraPath(student.residenceLocationID, classLocation);
        if (dist == -1) {
            continue;
        }   
        for (int node : path) {
            nodeset.insert(node);
        }
    }

    unordered_map<int, vector<pair<int, int>>> subgraph;
    for (int node : nodeset) {
        if (adjacencyList.find(node) == adjacencyList.end()) {
            continue;
        }
        for (const auto& [neighbor, weight] : adjacencyList.at(node)) {
            if (!nodeset.count(neighbor)) continue;
            if (closedEdges.count(makeEdge(node, neighbor))) continue; 
            subgraph[node].push_back({neighbor, weight});
        }
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    unordered_set<int> visited;
    pq.push({0, student.residenceLocationID});
    int total = 0;

    while (!pq.empty()) {
        auto [dist, node] = pq.top();
        pq.pop();
        if (visited.count(node)) continue;
        visited.insert(node);
        total += dist;
        if (subgraph.find(node) == subgraph.end()) {
            continue;
        }
        for (const auto& [neighbor, weight] : subgraph.at(node)) {
            if (!visited.count(neighbor)) {
                pq.push({weight, neighbor});
            }
        }
    }
    cout << "Student Zone Cost For " << student.name << ": " << total << endl;  
}