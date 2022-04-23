#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
using namespace std;

struct Course {
    string courseID;
    string courseName;
    string courseNum;
    vector<string> prereqs;

    Course() { //initialized empty
        courseID = "";
        courseName = "";
        courseNum = "";
    }
}

const unsigned int DEFAULT_SIZE = 16; //hash table size is double the size of the course list

class AdvisingHashTable {
private:

    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        Node() { //constructor
            key = UINT_MAX;
            next = nullptr;
        }

        Node(Course aCourse) : Node() { //constructor
            course = aCourse;
        }

        Node(Course aCourse, unsigned int aKey) : Node(aCourse) { //constructor with key
            key = aKey;
        }
    }

    vector<Node> nodes;
    unsigned int tableSize = DEFAULT_SIZE;
    unsigned int hash(int key);
    void Menu();

public:
    AdvisingHashTable();
    AdvisingHashTable(unsigned int size);
    virtual ~AdvisingHashTable();
    void insertCourse(Course course);
    void displayMenu();
    void printAll();
    Course Search(AdvisingHashTable* hashTable, string courseID);
}

AdvisingHashTable::AdvisingHashTable() { //resizes to default table size
    nodes.resize(tableSize); 
}

AdvisingHashTable::AdvisingHashTable(unsigned int size) { //table size variable reset
    this->tableSize = size;
}

AdvisingHashTable::~AdvisingHashTable() { //destructor
    nodes.erase(nodes.begin());
}

unsigned int AdvisingHashTable::hash(int key) { //generates key using hash table
    return key % tableSize;
}



void AdvisingHashTable::insertCourse(Course course) {

    unsigned key = hash(atoi(course.courseNum.c_str())); //using the transformation line from previous assignmentts to get a key

    Node* oldNode = &(nodes.at(key)); //node empty at key check

    if (oldNode == nullptr) { //if node is empty, fill node, insert to vector
        Node* newNode = new Node(course, key);
        nodes.insert(nodes.begin() + key, (*newNode));
    }

    else { //else node is in use
        if (oldNode->key == UINT_MAX) { //so if key unassigned, oldNode's key and course are replaced
            oldNode->key = key;
            oldNode->course = course;
            oldNode->next = nullptr; //oldNOde is inserted at the end
        }
        else { //else key is assigned and unassigned key needs to be found
            while (oldNode->next != nullptr) { //iterates until end is reached
                oldNode = oldNode->next;
            }
            oldNode->next = new Node(course, key); //assign to new node when found
        }
    }
}

class CSVReader {
private:
    int numPrereqs;

public:
    void readAndParse(AdvisingHashTable* hashTable);
    vector<Course> courses;
}

void CSVReader::readAndParse(AdvisingHashTable* hashTable) {

    vector<string> fileLines; //row, stores file rows
    ifstream fileInput; //reads file
    string tempLine; //line, stores line temporarily

    fileInput.open("CourseList.csv");

    while (!fileInput.eof()) { //reads through until end of file
        fileLines.clear();

        getline(fileInput, tempLine);

        stringstream ss(tempLine);
        numPrereqs = 0;

        while (ss.good()) { //split at commas, no line will have more than 4, but just in case
            string substr;
            getline(ss, substr, ',');
            row.push_back(substr);

            if (fileLines.size() > 2) {
                numPrereqs++;
            }
            if (fileLines.size() > 3) {
                numPrereqs++;
            }
            if (fileLInes.size() > 4) {
                numPrereqs++;
            }
            else{
                return;
            }
        }

        Course newCourse;
        newCourse.courseID = row.at(0);
        newCourse.courseName = row.at(1);
        newCourse.courseNum = row.at(0).erase(0, 4);

        if (numPrereqs == 0) {
            return;
        }
        else if (numPrereqs == 1) {
            newCourse.prereqs.push_back(row.at(2));
        }
        else if (numPrereqs == 2) {
            newCourse.prereqs.push_back(row.at(2));
            newCourse.prereqs.push_back(row.at(3));
        }

        courses.push_back(newCourse);

        for (unsigned int i = 0; i < courses.size(); i++) {
            hashTable->insertCourse(courses.at(i));
        }
    }
    fileInput.close();
}

Course AdvisingHashTable::Search(AdvisingHashTable* hashTable, string courseID) {
    Course course;

    string courseNum = courseID.erase(0, 4);

    unsigned key = hash(atoi(courseNum.c_str()));

    Node* node = &(nodes.at(key));

    if (node != nullptr && node->key != UINT_MAX && node->course.courseNum.compare(courseNum) == 0) {
        cout << hashTable->nodes.at(key).course.courseID << ": " << hashTable->nodes.at(key).course.courseName << endl;
        cout << "Prerequisites: ";
        if (hashTable->nodes.at(key).course.prereqs.size() < 1) {
            cout << "None" << endl;
        }
        else if (hashTable->nodes.at(key).course.prereqs.size() == 1) {
            cout << hashTable->nodes.at(key).course.prereqs.at(0) << endl;
        }
        else if (hashTable->nodes.at(key).course.prereqs.size() == 2) {
            cout << hashTable->nodes.at(key).course.prereqs.at(0) << ", " << hashTable->nodes.at(key).course.prereqs.at(1) << endl;
        }
        cout << endl;
        return node->course;
    }

    if (node == nullptr || node->key == UINT_MAX) {
        cout << "Please input a valid course ID." << endl;
        cout << endl;
        return course;
    }

    while (node != nullptr) {
        if (node->key != UINT_MAX && node->course.courseNum.compare(courseNum) == 0) {
            cout << "Match found!";

            cout << hashTable->nodes.at(key).course.courseID << ": " << hashTable->nodes.at(key).course.courseName << endl;
            cout << "Prerequisites: ";

            if (hashTable->nodes.at(key).course.prereqs.size() < 1) {
                cout << "None" << endl;
            }
            else if (hashTable->nodes.at(key).course.prereqs.size() == 1) {
                cout << hashTable->nodes.at(key).course.prereqs.at(0) << endl;
            }
            else if (hashTable->nodes.at(key).course.prereqs.size() == 2) {
                cout << hashTable->nodes.at(key).course.prereqs.at(0) << ", " << hashTable->nodes.at(key).course.prereqs.at(1) << endl;
            }
            cout << endl;

            return node->course;
        }
        node = node->next;
    }
    cout << "No Match Found!" << endl;
    cout << endl;
    return course;
}

void displayCourse(Course course) {

    cout << course.courseID << ": " << course.courseName << endl;
    cout << "Prerequisites: ";
    if (course.prereqs.size() < 1) {
        cout << "None" << endl;
    }
    else if (course.prereqs.size() == 1) {
        cout << course.prereqs.at(0) << endl;
    }
    else if (course.prereqs.size() == 2) {
        cout << course.prereqs.at(0) << ", " << course.prereqs.at(1) << endl;
    }
    cout << endl;
}

void AdvisingHashTable::printAll() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        if (nodes.at(i).key != UINT_MAX) {
            displayCourse(nodes.at(i).course);
        }
    }
}

void AdvisingHashTable::Menu() {
    string userID;
    int userInput;
    string courseInput;

    cout << "What would you like to do ? ";
    cin >> userInput;
    cout << endl;
    
    while (userInput != 9) {
        AdvisingHashTable* hashTable;
        CSVReader reader;

        if (userInput == 1) {
            hashTable = new AdvisingHashTable;
            reader.readAndParse(hashTable);

            cout << "Load Courses Selected" << endl;
        }

        else if (userInput == 2) {
            cout << "Print List Selected" << endl;
            hashTable.printALL();
            cout << endl;
        }

        else if (userInput == 3) {
            cout << "Print Course Selected" << endl <<
            "Enter Course Name: ";
            cin >> courseInput;
            displayCourse(courseInput);
        }

        else if (userInput == 9) {
            break;
        }

        else {
            cout << "Invalid Input!!";
        }
    }

void AdvisingHashTable::displayMenu() {
    cout << endl;
    cout << "Main Menu:" << endl;
    cout << endl;
    cout << "1. Load data structure" << endl;;
    cout << "2. Print course list" << endl;
    cout << "3. Print course" << endl;
    cout << "9. Quit"
    cout << endl;
    Menu();
}



int main()
{
    AdvisingHashTable program;

    cout << endl;
    cout << "Welcome to the course planner." << endl;

    program.displayMenu();

    return 0;
}