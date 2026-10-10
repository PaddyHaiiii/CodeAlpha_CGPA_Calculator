# CodeAlpha CGPA Management System

A menu-driven C++ application for managing course grades, calculating semester GPA and overall CGPA, and generating academic reports.

## Features

- Create and manage multiple semesters
- Add, edit, delete, and search courses
- Validate grades and credit hours
- Calculate credit-weighted semester GPA and overall CGPA
- Display individual course grades and grade points
- Identify the highest- and lowest-grade courses in a semester
- Compare semester GPAs
- Save and load records using `cgpa_data.txt`
- Generate a readable `CGPA_Report.txt` report
- Object-oriented design using `Course`, `Semester`, and `CGPAManager`

## Grade scale

| Grade | Grade point |
|---|---:|
| A | 10 |
| B | 8 |
| C | 7 |
| D | 6 |
| E | 5 |
| F | 0 |

This is the grade scale used in this implementation. Change it if your institution follows a different grading scheme.

## Formula

`GPA = sum(credit hours × grade point) / sum(credit hours)`

Overall CGPA is calculated with the same credit-weighted formula across all recorded semesters.

## Requirements

- C++ compiler supporting C++11 or later (for example, g++)
- Terminal or IDE such as Visual Studio Code

## Compile

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o cgpa_calculator
```

## Run

Linux/macOS:

```bash
./cgpa_calculator
```

Windows (MinGW):

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o cgpa_calculator.exe
cgpa_calculator.exe
```

## How to use

1. Select **Create semester**.
2. Select **Add course** and enter the course code, name, credits, and grade.
3. Add additional courses and semesters.
4. Display semester results or the overall report.
5. Use **Save data** before exiting to preserve your records.
6. Use **Load data** in a later session to restore the saved records.
7. Use **Generate report file** to create `CGPA_Report.txt`.

The program does not automatically save changes; select **Save data** before exiting.

## Files

- `main.cpp` — source code
- `cgpa_data.txt` — saved records, created by the program
- `CGPA_Report.txt` — generated report, created by the program

The data files are created in the program's current working directory.

## Repository structure

```text
CodeAlpha_CGPA_Calculator/
├── main.cpp
└── README.md
```

## Limitations

- The grade scale may need adjustment to match a particular college.
- Course codes and names must not contain the `|` delimiter used in the save file.
- This is a console-based educational project; it is not connected to a college database.

## CodeAlpha task mapping

This project accepts courses, grades, and credit hours; calculates weighted semester GPA and overall CGPA; and displays individual course grades and final results.

## Author

Name : Parth Vairagade

