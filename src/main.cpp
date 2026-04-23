#include <iostream>

#include "graph.h"

using namespace std;

int main() {
    // initialize your main project object
    Graph graph;
    
    string edgesFile = "data/edges.csv";
    string classesFile = "data/classes.csv";
    graph.loadEdge(edgesFile);
    graph.loadClasses(classesFile);

    int numOfCommands;
    cin >> numOfCommands;
    cin.ignore();

    for (int i = 0; i < numOfCommands; i++) {
        string line;
        getline(cin, line);
        if (line.empty()){
            i--;
            continue;
        }

        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "insert") {
            size_t quote1 = line.find('"');
            size_t quote2 = line.find('"', quote1 + 1);
            if (quote1 == string::npos ! quote2 == string::npos) {
                cout << "unsuccessful" << endl;
                continue;
            }

            string name = line.substr(quote1 + 1, quote2 - quote1 - 1);

            stringstream rest(line.substr(quote2 + 1));
            int studID;
            int residenceID;
            int N;

            if (!(rest >> studID >> residenceID >> N)) {
                cout << "unsuccessful" << endl;
                continue;
            }

            vector<string> classCodes;
            string classCode;
            while(rest >> classCode) {
                classCodes.push_back(classCode);
            }
            if ((int)classCodes.size() != N) {
                cout << "unsuccessful" << endl;
                continue;
            }

            cout << graph.insert(name, studID, residenceID, classCodes) << endl;
        
        }
        else if (command == "remove") {
            int studID;
            if (!(ss >> studID)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.remove(studID) << endl;
        }
        else if (command == "dropClass"){
            int studID;
            string classCode;
            if (!(ss >> studID >> classCode)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.dropClass(studID, classCode) << endl;
        }
        else if (command == "replaceClass"){
            int studID;
            string oldClassCode, newClassCode;
            if (!(ss >> studID >> oldClassCode >> newClassCode)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.replaceClass(studID, oldClassCode, newClassCode) << endl;
        }
        else if (command == "removeClass"){
            string classCode;
            if (!(ss >> classCode)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.removeClass(classCode) << endl;
        }
        else if (command == "toggleEdgesClosure"){
            int N;
            if (!(ss >> N)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            vector<pair<int, int>> edges;
            bool valid = true;
            for (int j = 0; j < N; j++) {
                int id1, id2;
                if (!(ss >> id1 >> id2)) {
                    valid = false;
                    break;
                }
                edges.push_back(id1, id2);
            }
            if (!valid) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.toggleEdgesClosure(edges) << endl;
        }
        else if (command == "checkEdgeStatus"){
            int id1, id2;
            if (!(ss >> id1 >> id2)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.checkEdgeStatus(id1, id2) << endl;
        }
        else if (command == "isConnected"){
            int id1, id2;
            if (!(ss >> id1 >> id2)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            cout << graph.isConnected(id1, id2) << endl;
        }
        else if (command == "printShortEdges"){
            int studID;
            if (!(ss >> studID)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            graph.printShortEdges(studID);
        }
        else if (command == "printStudentZone"){
            int studID;
            if (!(ss >> studID)) {
                cout << "unsuccessful" << endl;
                continue;
            }
            graph.printStudentZone(studID);
        }
        else {
            cout << "unsuccessful" << endl;
        }
    }
    return 0;
}
