#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// Represents one course and its grade information.
class Course {
private:
    string code, name;
    double credits;
    char grade;

public:
    Course() : credits(0.0), grade('F') {}
    Course(string c, string n, double cr, char g)
        : code(c), name(n), credits(cr), grade(toupper(g)) {}

    string getCode() const { return code; }
    string getName() const { return name; }
    double getCredits() const { return credits; }
    char getGrade() const { return grade; }

    void setName(const string& value) { name = value; }
    void setCredits(double value) { credits = value; }
    void setGrade(char value) { grade = toupper(value); }

    double gradePoint() const {
        switch (grade) {
            case 'A': return 10.0;
            case 'B': return 8.0;
            case 'C': return 7.0;
            case 'D': return 6.0;
            case 'E': return 5.0;
            case 'F': return 0.0;
            default: return 0.0;
        }
    }

    string gradeDescription() const {
        switch (grade) {
            case 'A': return "Excellent";
            case 'B': return "Very Good";
            case 'C': return "Good";
            case 'D': return "Average";
            case 'E': return "Pass";
            case 'F': return "Fail";
            default: return "Unknown";
        }
    }

    void display() const {
        cout << left << setw(13) << code
             << setw(25) << name
             << right << setw(9) << fixed << setprecision(2) << credits
             << setw(8) << grade
             << setw(12) << gradePoint()
             << "  " << gradeDescription() << '\n';
    }

    string toFileLine() const {
        ostringstream out;
        out << code << '|' << name << '|'
            << setprecision(10) << credits << '|' << grade;
        return out.str();
    }
};

class Semester {
private:
    int number;
    vector<Course> courses;

    bool validGrade(char g) const {
        g = toupper(g);
        return g == 'A' || g == 'B' || g == 'C' ||
               g == 'D' || g == 'E' || g == 'F';
    }

public:
    explicit Semester(int n = 1) : number(n) {}

    int getNumber() const { return number; }
    const vector<Course>& getCourses() const { return courses; }

    int findCourse(const string& code) const {
        for (size_t i = 0; i < courses.size(); ++i)
            if (courses[i].getCode() == code)
                return static_cast<int>(i);
        return -1;
    }

    void addCourse() {
        string code, name;
        double credits;
        char grade;

        cout << "Course code (unique; do not use |): ";
        cin >> code;

        if (code.find('|') != string::npos || findCourse(code) != -1) {
            cout << "Invalid or duplicate course code.\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Course name: ";
        getline(cin, name);

        if (name.empty() || name.find('|') != string::npos) {
            cout << "Course name cannot be empty or contain |.\n";
            return;
        }

        cout << "Credit hours: ";
        if (!(cin >> credits) || credits <= 0) {
            cout << "Credits must be a positive number.\n";
            recoverInput();
            return;
        }

        cout << "Grade (A/B/C/D/E/F): ";
        cin >> grade;
        grade = toupper(grade);

        if (!validGrade(grade)) {
            cout << "Invalid grade.\n";
            return;
        }

        courses.emplace_back(code, name, credits, grade);
        cout << "Course added successfully.\n";
    }

    void editCourse() {
        string code;
        cout << "Course code to edit: ";
        cin >> code;
        int index = findCourse(code);

        if (index == -1) {
            cout << "Course not found.\n";
            return;
        }

        string name;
        double credits;
        char grade;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "New course name: ";
        getline(cin, name);
        if (name.empty() || name.find('|') != string::npos) {
            cout << "Invalid course name.\n";
            return;
        }

        cout << "New credit hours: ";
        if (!(cin >> credits) || credits <= 0) {
            cout << "Credits must be positive.\n";
            recoverInput();
            return;
        }

        cout << "New grade (A/B/C/D/E/F): ";
        cin >> grade;
        grade = toupper(grade);
        if (!validGrade(grade)) {
            cout << "Invalid grade.\n";
            return;
        }

        courses[index].setName(name);
        courses[index].setCredits(credits);
        courses[index].setGrade(grade);
        cout << "Course updated.\n";
    }

    void deleteCourse() {
        string code;
        cout << "Course code to delete: ";
        cin >> code;
        int index = findCourse(code);

        if (index == -1) {
            cout << "Course not found.\n";
            return;
        }

        courses.erase(courses.begin() + index);
        cout << "Course deleted.\n";
    }

    void searchCourse() const {
        string key;
        cout << "Search course code or name: ";
        cin >> ws;
        getline(cin, key);

        bool found = false;
        for (const Course& c : courses) {
            if (c.getCode().find(key) != string::npos ||
                c.getName().find(key) != string::npos) {
                if (!found) printHeader();
                c.display();
                found = true;
            }
        }
        if (!found) cout << "No matching course found.\n";
    }

    double totalCredits() const {
        double total = 0;
        for (const Course& c : courses) total += c.getCredits();
        return total;
    }

    double weightedPoints() const {
        double total = 0;
        for (const Course& c : courses)
            total += c.getCredits() * c.gradePoint();
        return total;
    }

    double gpa() const {
        return totalCredits() == 0 ? 0 : weightedPoints() / totalCredits();
    }

    void displayResults() const {
        cout << "\n========== SEMESTER " << number << " ==========\n";
        if (courses.empty()) {
            cout << "No courses recorded.\n";
            return;
        }

        printHeader();
        for (const Course& c : courses) c.display();

        cout << string(83, '-') << '\n';
        cout << "Total credits: " << fixed << setprecision(2)
             << totalCredits() << '\n';
        cout << "Semester GPA:  " << fixed << setprecision(2) << gpa() << '\n';

        if (gpa() >= 9) cout << "Performance: Outstanding\n";
        else if (gpa() >= 8) cout << "Performance: Excellent\n";
        else if (gpa() >= 7) cout << "Performance: Very Good\n";
        else if (gpa() >= 6) cout << "Performance: Good\n";
        else if (gpa() >= 5) cout << "Performance: Satisfactory\n";
        else cout << "Performance: Needs Improvement\n";

        const Course* high = &courses[0];
        const Course* low = &courses[0];
        for (const Course& c : courses) {
            if (c.gradePoint() > high->gradePoint()) high = &c;
            if (c.gradePoint() < low->gradePoint()) low = &c;
        }
        cout << "Highest-grade course: " << high->getName()
             << " (" << high->getGrade() << ")\n";
        cout << "Lowest-grade course:  " << low->getName()
             << " (" << low->getGrade() << ")\n";
    }

    void save(ofstream& file) const {
        file << "SEMESTER|" << number << '\n';
        for (const Course& c : courses)
            file << "COURSE|" << c.toFileLine() << '\n';
        file << "ENDSEMESTER\n";
    }

    bool loadCourse(const string& line) {
        istringstream in(line);
        string code, name, creditText, gradeText;
        if (!getline(in, code, '|') || !getline(in, name, '|') ||
            !getline(in, creditText, '|') || !getline(in, gradeText, '|') ||
            gradeText.empty())
            return false;

        try {
            double cr = stod(creditText);
            char g = toupper(gradeText[0]);
            if (code.empty() || name.empty() || cr <= 0 ||
                !validGrade(g) || findCourse(code) != -1)
                return false;
            courses.emplace_back(code, name, cr, g);
            return true;
        } catch (...) {
            return false;
        }
    }

    static void printHeader() {
        cout << left << setw(13) << "Course Code"
             << setw(25) << "Course Name"
             << right << setw(9) << "Credits"
             << setw(8) << "Grade"
             << setw(12) << "Points"
             << "  Description\n";
        cout << string(83, '-') << '\n';
    }

    static void recoverInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
};

class CGPAManager {
private:
    vector<Semester> semesters;
    const string dataFile = "cgpa_data.txt";

    int semesterIndex(int number) const {
        for (size_t i = 0; i < semesters.size(); ++i)
            if (semesters[i].getNumber() == number)
                return static_cast<int>(i);
        return -1;
    }

    int chooseSemester() const {
        if (semesters.empty()) {
            cout << "Create or load a semester first.\n";
            return -1;
        }

        int number;
        cout << "Semester number (";
        for (size_t i = 0; i < semesters.size(); ++i) {
            if (i) cout << ", ";
            cout << semesters[i].getNumber();
        }
        cout << "): ";

        if (!(cin >> number)) {
            cout << "Enter a valid number.\n";
            Semester::recoverInput();
            return -1;
        }

        int index = semesterIndex(number);
        if (index == -1) cout << "Semester not found.\n";
        return index;
    }

public:
    void createSemester() {
        int number = 1;
        while (semesterIndex(number) != -1) ++number;
        semesters.emplace_back(number);
        cout << "Semester " << number << " created.\n";
    }

    void addCourse() {
        int i = chooseSemester();
        if (i != -1) semesters[i].addCourse();
    }

    void editCourse() {
        int i = chooseSemester();
        if (i != -1) semesters[i].editCourse();
    }

    void deleteCourse() {
        int i = chooseSemester();
        if (i != -1) semesters[i].deleteCourse();
    }

    void searchCourse() {
        int i = chooseSemester();
        if (i != -1) semesters[i].searchCourse();
    }

    void displaySemester() {
        int i = chooseSemester();
        if (i != -1) semesters[i].displayResults();
    }

    double totalCredits() const {
        double total = 0;
        for (const Semester& s : semesters) total += s.totalCredits();
        return total;
    }

    double totalWeightedPoints() const {
        double total = 0;
        for (const Semester& s : semesters) total += s.weightedPoints();
        return total;
    }

    double overallCGPA() const {
        return totalCredits() == 0 ? 0 : totalWeightedPoints() / totalCredits();
    }

    void overallReport() const {
        if (semesters.empty()) {
            cout << "No academic data available.\n";
            return;
        }

        cout << "\n========== OVERALL ACADEMIC REPORT ==========\n";
        for (const Semester& s : semesters) {
            cout << "Semester " << s.getNumber()
                 << " | Credits: " << fixed << setprecision(2)
                 << s.totalCredits()
                 << " | GPA: " << s.gpa() << '\n';
        }

        cout << "Total credits: " << totalCredits() << '\n';
        cout << "Overall CGPA:  " << overallCGPA() << '\n';
    }

    void compareSemesters() const {
        if (semesters.size() < 2) {
            cout << "Create at least two semesters to compare them.\n";
            return;
        }

        const Semester* best = &semesters[0];
        const Semester* lowest = &semesters[0];

        cout << "\n========== SEMESTER COMPARISON ==========\n";
        for (const Semester& s : semesters) {
            cout << "Semester " << s.getNumber()
                 << " GPA: " << fixed << setprecision(2) << s.gpa() << '\n';
            if (s.gpa() > best->gpa()) best = &s;
            if (s.gpa() < lowest->gpa()) lowest = &s;
        }

        cout << "Highest GPA: Semester " << best->getNumber() << '\n';
        cout << "Lowest GPA:  Semester " << lowest->getNumber() << '\n';
    }

    bool saveData() const {
        ofstream file(dataFile);
        if (!file) {
            cout << "Could not open data file for writing.\n";
            return false;
        }

        file << "CGPA_DATA_V1\n";
        for (const Semester& s : semesters) s.save(file);
        file.close();
        cout << "Data saved to " << dataFile << ".\n";
        return true;
    }

    bool loadData() {
        ifstream file(dataFile);
        if (!file) {
            cout << "No saved data file found.\n";
            return false;
        }

        string line;
        if (!getline(file, line) || line != "CGPA_DATA_V1") {
            cout << "Unrecognized data file format.\n";
            return false;
        }

        vector<Semester> loaded;
        Semester* current = nullptr;
        bool valid = true;

        while (getline(file, line)) {
            if (line.empty()) continue;

            if (line.rfind("SEMESTER|", 0) == 0) {
                try {
                    int number = stoi(line.substr(9));
                    if (number <= 0) { valid = false; break; }
                    for (const Semester& s : loaded) {
                        if (s.getNumber() == number) valid = false;
                    }
                    if (!valid) break;
                    loaded.emplace_back(number);
                    current = &loaded.back();
                } catch (...) {
                    valid = false;
                    break;
                }
            } else if (line.rfind("COURSE|", 0) == 0) {
                if (current == nullptr || !current->loadCourse(line.substr(7))) {
                    valid = false;
                    break;
                }
            } else if (line == "ENDSEMESTER") {
                current = nullptr;
            } else {
                valid = false;
                break;
            }
        }

        if (!valid) {
            cout << "Saved data is invalid. Current records were not changed.\n";
            return false;
        }

        semesters = loaded;
        cout << "Data loaded successfully.\n";
        return true;
    }

    void generateReport() const {
        ofstream file("CGPA_Report.txt");
        if (!file) {
            cout << "Unable to create report file.\n";
            return;
        }

        file << "========================================\n";
        file << "           CGPA ACADEMIC REPORT\n";
        file << "========================================\n\n";

        for (const Semester& s : semesters) {
            file << "Semester " << s.getNumber() << '\n';
            file << "GPA: " << fixed << setprecision(2) << s.gpa() << '\n';
            file << "Credits: " << s.totalCredits() << '\n';

            for (const Course& c : s.getCourses()) {
                file << "  " << c.getCode() << " | " << c.getName()
                     << " | " << c.getCredits() << " credits | Grade "
                     << c.getGrade() << " | Grade point "
                     << c.gradePoint() << '\n';
            }
            file << '\n';
        }

        file << "Total credits: " << totalCredits() << '\n';
        file << "Overall CGPA: " << overallCGPA() << '\n';
        file.close();
        cout << "Report saved as CGPA_Report.txt.\n";
    }
};

void showMenu() {
    cout << "\n============================================\n";
    cout << "          CGPA MANAGEMENT SYSTEM\n";
    cout << "============================================\n";
    cout << "1. Create semester\n";
    cout << "2. Add course\n";
    cout << "3. Display semester results\n";
    cout << "4. Edit course\n";
    cout << "5. Delete course\n";
    cout << "6. Search course\n";
    cout << "7. Overall academic report\n";
    cout << "8. Compare semesters\n";
    cout << "9. Save data\n";
    cout << "10. Load data\n";
    cout << "11. Generate report file\n";
    cout << "12. Exit\n";
    cout << "============================================\n";
}

int main() {
    CGPAManager manager;
    int choice;

    do {
        showMenu();
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Please enter a number from 1 to 12.\n";
            Semester::recoverInput();
            continue;
        }

        switch (choice) {
            case 1: manager.createSemester(); break;
            case 2: manager.addCourse(); break;
            case 3: manager.displaySemester(); break;
            case 4: manager.editCourse(); break;
            case 5: manager.deleteCourse(); break;
            case 6: manager.searchCourse(); break;
            case 7: manager.overallReport(); break;
            case 8: manager.compareSemesters(); break;
            case 9: manager.saveData(); break;
            case 10: manager.loadData(); break;
            case 11: manager.generateReport(); break;
            case 12: cout << "Thank you for using the CGPA Management System!\n"; break;
            default: cout << "Invalid choice. Select 1-12.\n";
        }
    } while (choice != 12);

    return 0;
}
