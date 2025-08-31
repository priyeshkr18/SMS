#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Course {
public:
    string courseID;
    string courseName;
    double *grades;
    int capacity;
    int numEnrolled;

    Course(string id, string name) {
        courseID = id;
        courseName = name;
        capacity = 100;
        grades = new double[capacity];
        numEnrolled = 0;
    }

    ~Course() {
        delete[] grades;
    }

    void addGrade(double grade) {
        if (numEnrolled < capacity) {
            grades[numEnrolled++] = grade;
        }
    }

    double averageGrade() {
        if (numEnrolled == 0) return 0.0;
        double sum = 0;
        for (int i = 0; i < numEnrolled; i++) {
            sum += grades[i];
        }
        return sum / numEnrolled;
    }
};

class Department {
public:
    string deptName;
    Department(string name): deptName(name) {}
};

class Student {
protected:
    string name, id, contact;
    int numCourses;
    Course **courses;
    double *grades;
public:
    Student *prev, *next;
    Student(string n, string i, string c) {
        name = n; id = i; contact = c;
        numCourses = 0;
        courses = new Course*[10];
        grades = new double[10];
        prev = next = nullptr;
    }
    virtual ~Student() {
        delete[] courses;
        delete[] grades;
    }
    virtual string studentType() { return "Student"; }
    string getID() { return id; }
    string getName() { return name; }
    string getContact() { return contact; }
    int getNumCourses() { return numCourses; }
    Course* getCourse(int idx) { return courses[idx]; }
    double getGrade(int idx) { return grades[idx]; }

    void enrollInCourse(Course* c, double grade) {
        if (numCourses < 10) {
            courses[numCourses] = c;
            grades[numCourses] = grade;
            c->addGrade(grade);
            numCourses++;
        }
    }
    void displayProfile() {
        cout << "ID: " << id << endl
            << "Name: " << name << endl
          << "Contact: " << contact << endl
            << "Type: " << studentType() << endl
            << "Courses Enrolled: " << numCourses << endl;
        for (int i=0;i<numCourses;i++) {
            cout << "  " << courses[i]->courseID << " - " << courses[i]->courseName
                << ", Grade: " << grades[i] << endl;
        }
    }
    void printGradeReport() {
        cout << "Grade Report for " << name << " (" << id << "):" << endl;
        for (int i=0; i<numCourses; i++) {
            cout << courses[i]->courseName << ": " << grades[i] << endl;
        }
    }
};

class UndergraduateStudent : public Student {
public:
    UndergraduateStudent(string n, string i, string c): Student(n, i, c) {}
    string studentType() override { return "Undergraduate"; }
};

class GraduateStudent : public Student {
public:
    GraduateStudent(string n, string i, string c): Student(n, i, c) {}
    string studentType() override { return "Graduate"; }
};

class StudentList {
    Student *head, *tail;
public:
    StudentList() { head = tail = nullptr; }
    ~StudentList() {
        Student *current = head, *next = nullptr;
        while (current) {
            next = current->next;
            delete current;
            current = next;
        }
    }

    void addStudent(Student* s) {
        if (!head) {
            head = tail = s;
        } else {
            tail->next = s;
            s->prev = tail;
            tail = s;
        }
    }

    Student* findStudentByID(string id) {
        Student *cur = head;
        while (cur) {
            if (cur->getID() == id) return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void displayAllStudents() {
        Student *cur = head;
        while (cur) {
            cur->displayProfile();
            cout << "------------------------" << endl;
            cur = cur->next;
        }
    }
};

class CourseList {
    Course **courses;
    int numCourses, capacity;
public:
    CourseList() {
        capacity = 20; numCourses = 0;
        courses = new Course*[capacity];
    }
    ~CourseList() {
        for (int i = 0; i < numCourses; i++) delete courses[i];
        delete[] courses;
    }
    void addCourse(string id, string name) {
        if (numCourses < capacity) courses[numCourses++] = new Course(id, name);
    }
    Course* findCourseByID(string id) {
        for (int i=0;i<numCourses;i++)
            if (courses[i]->courseID == id) return courses[i];
        return nullptr;
    }
    void listCourses() {
        for (int i=0;i<numCourses;i++)
            cout << courses[i]->courseID << ": " << courses[i]->courseName << endl;
    }
};

int main() {
    StudentList students;
    CourseList courses;

    courses.addCourse("CSE101", "Data Structures");
    courses.addCourse("MAT100", "Calculus");
    courses.addCourse("PHY130", "Physics");

    while (true) {
        cout << "\n1. Add Student\n2. Enroll and Assign Grade\n3. Search Student by ID\n4. Display All Students\n5. Grade Report\n6. Average Grade for Course\n7. Exit\nChoose option: ";
        int opt;
        cin >> opt;
        if (opt == 1) {
            int stype;
            string name, id, contact;
            cout << "Type (1-Undergrad,2-Grad): "; cin >> stype;
            cout << "Name: "; cin.ignore(); getline(cin, name);
            cout << "ID: "; getline(cin, id);
            cout << "Contact: "; getline(cin, contact);
            Student *stu = nullptr;
            if (stype == 1)
                stu = new UndergraduateStudent(name, id, contact);
            else
                stu = new GraduateStudent(name, id, contact);
            students.addStudent(stu);
            cout << "Student added.\n";
        } else if (opt == 2) {
            string sid, cid;
            double grade;
            cout << "Student ID: "; cin.ignore(); getline(cin, sid);
            Student* stu = students.findStudentByID(sid);
            if (!stu) { cout << "Not found\n"; continue; }
            courses.listCourses();
            cout << "Course ID: "; getline(cin, cid);
            Course* c = courses.findCourseByID(cid);
            if (!c) { cout << "Invalid course\n"; continue; }
            cout << "Grade: "; cin >> grade;
            stu->enrollInCourse(c, grade);
            cout << "Enrolled and grade assigned.\n";
        } else if (opt == 3) {
            string sid;
            cout << "Student ID: "; cin.ignore(); getline(cin, sid);
            Student* stu = students.findStudentByID(sid);
            if (!stu) cout << "Not found\n";
            else stu->displayProfile();
        } else if (opt == 4) {
            students.displayAllStudents();
        } else if (opt == 5) {
            string sid;
            cout << "Student ID: "; cin.ignore(); getline(cin, sid);
            Student* stu = students.findStudentByID(sid);
            if (!stu) cout << "Not found\n";
            else stu->printGradeReport();
        } else if (opt == 6) {
            string cid;
            courses.listCourses();
            cout << "Course ID: "; cin.ignore(); getline(cin, cid);
            Course* c = courses.findCourseByID(cid);
            if (c) {
                cout << fixed << setprecision(2);
                cout << "Average Grade for " << c->courseName << ": " << c->averageGrade() << endl;
            }
            else cout << "Invalid course\n";
        } else if (opt == 7) {
            break;
        }
    }
    return 0;
}
