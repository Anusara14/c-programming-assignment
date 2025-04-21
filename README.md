# C Programming Assignment

## 1. Introduction

This report details the design, implementation, and testing of a Student Record Management System developed as part of the Programming in C module assessment. The primary problem addressed is the need for an efficient way to manage student information, replacing potentially cumbersome manual methods. The purpose of this project was to apply fundamental C programming concepts – including data structures, functions, file handling, loops, conditions, and arrays – to create a functional console-based application that meets the requirements outlined in the project specification [cite: 6]. The system allows users to add, display, search, update, and delete student records, calculate average marks, determine pass/fail status, and persist data between sessions using file storage [cite: 7, 8, 9, 10, 11, 12, 13].

---

## 2. Design

The Student Record Management System is designed as a modular, menu-driven console application to ensure ease of use and maintainability.

### 2.1 Program Structure

The program utilizes a top-down design approach, breaking down the overall task into smaller, manageable functions. The core logic resides within the `main` function, which presents a text-based menu to the user and calls other functions based on the user's choice. A `do-while` loop keeps the menu active until the user explicitly chooses to exit.

### 2.2 Data Structures

- **`Student` Struct:** A `struct` named `Student` is defined to encapsulate all information related to a single student. This includes `studentID` (int), `name` (char array), `age` (int), `gender` (char array), `course` (char array), `marks` (int array for `NUM_SUBJECTS`), `averageMarks` (float), and `passFailStatus` (char array) [cite: 7]. Using a structure provides a clear and organized way to manage related data.
- **Global Array:** A global array `studentDatabase[MAX_STUDENTS]` of type `Student` is used to store the records of all students. A global integer variable `studentCount` tracks the current number of students in the database. While global variables were used for simplicity in this context, alternative designs might pass the database array to functions.

### 2.3 Algorithms and Logic

- **Menu Navigation:** A `switch` statement within the main loop directs program flow to the appropriate function based on user input.
- **Search:** Linear search is implemented in the helper functions `findStudentById` and `findStudentByName`. These functions iterate through the `studentDatabase` array to locate a student based on their ID or name, returning the array index if found, or -1 otherwise [cite: 9]. While the assignment suggested recursion could be used [cite: 14], an iterative approach was chosen for simplicity in the search implementation.
- **Deletion:** Deletion involves finding the student's index and then shifting all subsequent elements in the `studentDatabase` array one position to the left to overwrite the deleted record, followed by decrementing `studentCount` [cite: 11].
- **Average Calculation:** The average mark is calculated when a student is added or updated by summing the marks in the `marks` array and dividing by `NUM_SUBJECTS`. The pass/fail status is determined by comparing the average mark against a predefined `PASS_MARK` [cite: 12].
- **File Handling:** Binary file I/O (`fopen` with "wb" and "rb" modes) is used for data persistence [cite: 13]. The `saveToFile` function writes the current `studentCount` followed by the entire `studentDatabase` array to `student_records.dat`. The `loadFromFile` function reads the count and then the records back into the array when the program starts. This approach is efficient for fixed-size structures. Error handling for file operations (e.g., file not found, read/write errors) is included.
- **Input Validation:** Basic input validation is implemented, such as checking for numeric input for IDs and marks, ensuring age is positive, checking for unique student IDs, and handling potential errors during file operations [cite: 23]. Helper functions like `clearInputBuffer` and `readString` are used to manage input more reliably.

---

## 3. Implementation

The system was implemented entirely in the C programming language using standard libraries.

### 3.1 Key Functions

- **`main()`:** Orchestrates the program flow, displays the menu, takes user input, and calls other functions via a `switch` statement. Also handles loading data on startup and saving data on exit.
- **`displayMenu()`:** Prints the text-based menu options to the console [cite: 15].
- **`addStudent()`:** Prompts the user for student details, performs basic validation (e.g., unique ID, valid age/marks), calculates the average and pass/fail status, stores the new record in the `studentDatabase` array, and increments `studentCount` [cite: 7].
- **`displayAllStudents()`:** Iterates through the `studentDatabase` array up to `studentCount` and prints the details of each student in a formatted table [cite: 8].
- **`searchStudent()`:** Asks the user whether to search by ID or name, takes the search term, calls the appropriate helper function (`findStudentById` or `findStudentByName`), and displays the full details if the student is found [cite: 9].
- **`updateStudent()`:** Finds a student by ID, then prompts the user for new details for each field. If new marks are entered, it recalculates the average and pass/fail status [cite: 10].
- **`deleteStudent()`:** Finds a student by ID, removes the record by shifting subsequent array elements, and decrements `studentCount` [cite: 11].
- **`calculateAverages()`:** Iterates through all students and displays their calculated average marks and pass/fail status [cite: 12]. (Note: Calculation now happens during add/update).
- **`saveToFile()` / `loadFromFile()`:** Implement data persistence using binary file I/O, writing/reading the `studentCount` and the `studentDatabase` array to/from `student_records.dat` [cite: 13]. Includes error handling for file operations.
- **`findStudentById()` / `findStudentByName()`:** Helper functions performing linear searches to locate a student's index in the array.
- **`clearInputBuffer()` / `readString()`:** Utility functions to handle potential issues with `scanf` and read string input more safely.

### 3.2 Data Handling

Data is primarily stored in the global `studentDatabase` array during runtime. File handling functions ensure this data persists across program executions by saving to and loading from a binary file (`student_records.dat`). Standard C libraries like `stdio.h` (for input/output), `stdlib.h` (for `exit`, `system`), `string.h` (for `strcpy`, `strcmp`, `strlen`), and `ctype.h` (for `isspace`) were used.

---

## 4. Testing

The program underwent testing to ensure functionality and robustness.

### 4.1 Testing Methods

- **Unit Testing:** Individual functions (like `addStudent`, `findStudentById`, `saveToFile`) were tested in isolation where possible, or by calling them through the main menu with specific inputs to check their outputs.
- **Integration Testing:** The system was tested as a whole by running through various scenarios using the menu interface, ensuring functions worked correctly together.
- **Input Validation Testing:** Tested with both valid and invalid inputs (e.g., non-numeric values for ID/age/marks, negative marks, duplicate IDs, overly long names) to check error handling.
- **File Handling Testing:** Tested saving and loading data, including scenarios like starting with no data file, saving data, exiting, restarting to ensure data loads correctly, and attempting to load corrupted or empty files (handled by starting fresh).
- **Edge Case Testing:** Included testing scenarios like adding students until the database is full, deleting from an empty database, searching when the database is empty, deleting the only student, deleting the last student in the array.

### 4.2 Sample Test Cases and Results

| Test Case ID | Description                                  | Input                                         | Expected Output                                        | Actual Output | Status |
| :----------- | :------------------------------------------- | :-------------------------------------------- | :----------------------------------------------------- | :------------ | :----- |
| TC01         | Add a new student                            | Menu: 1, ID: 101, Name: John Doe, Age: 20...  | Success message, student added to database             | As Expected   | Pass   |
| TC02         | Display all students (after TC01)            | Menu: 2                                       | Details of John Doe displayed                          | As Expected   | Pass   |
| TC03         | Search for existing student by ID            | Menu: 3, Choice: 1, ID: 101                   | Details of John Doe displayed                          | As Expected   | Pass   |
| TC04         | Search for non-existent student by Name      | Menu: 3, Choice: 2, Name: Jane Smith          | "Student not found" message                            | As Expected   | Pass   |
| TC05         | Update existing student's course             | Menu: 4, ID: 101, New Course: CS              | Success message, course updated when displayed         | As Expected   | Pass   |
| TC06         | Delete existing student                      | Menu: 5, ID: 101                              | Success message, student not found in later search     | As Expected   | Pass   |
| TC07         | Attempt to add student when database is full | Menu: 1 (when `studentCount == MAX_STUDENTS`) | "Database is full" message                             | As Expected   | Pass   |
| TC08         | Calculate Averages                           | Menu: 6                                       | Display list of students with average and status       | As Expected   | Pass   |
| TC09         | Save data to file                            | Menu: 7 or 0                                  | Success message, `student_records.dat` created/updated | As Expected   | Pass   |
| TC10         | Load data from file on startup               | Start program after saving data               | "Loaded X student record(s)" message                   | As Expected   | Pass   |
| TC11         | Invalid menu choice                          | Menu: 9                                       | "Invalid choice" message                               | As Expected   | Pass   |
| TC12         | Invalid input for age (text)                 | Menu: 1, ... Age: abc                         | Error message, prompt for age again                    | As Expected   | Pass   |

_(Note: Actual output confirmed via manual execution and inspection of console output and data file content.)_

---

## 5. Conclusion

The Student Record Management System project was successfully completed, meeting all the core and advanced functional requirements specified in the assignment brief [cite: 7, 8, 9, 10, 11, 12, 13, 14, 15]. The program provides a functional command-line interface for managing student records effectively. Key C programming concepts like structures, arrays, functions, file I/O, and menu-driven logic were implemented.

### 5.1 Challenges Faced

- **Input Handling:** Reliably handling user input in C, especially mixing `scanf` for numbers and `fgets` (or equivalent logic) for strings, required careful buffer management (using `clearInputBuffer`). Ensuring robust validation against various incorrect inputs was iterative.
- **File I/O:** Ensuring correct binary file reading and writing, especially handling potential errors like file corruption or incorrect formats, required careful implementation and testing.

### 5.2 Potential Improvements

- **Dynamic Memory Allocation:** Replace the fixed-size `studentDatabase` array with dynamic memory allocation (using `malloc`, `realloc`, `free`) to handle an arbitrary number of students.
- **Enhanced Error Handling:** Implement more comprehensive input validation (e.g., checking string formats, more specific numeric range checks).
- **Sorting:** Add functionality to sort student records (e.g., by ID or name) before displaying, potentially using recursion as suggested in the assignment brief [cite: 14].
- **User Interface:** Develop a graphical user interface (GUI) instead of the console menu for better usability (though outside the scope of this C assignment).
- **Search Algorithm:** For a very large number of students, implementing a more efficient search algorithm (like binary search, requiring sorted data) could be beneficial instead of linear search.
- **Modularization:** Further break down larger functions (like `addStudent` or `updateStudent`) into smaller, more focused helper functions.

---

## 6. References

- Kernighan, B. W., & Ritchie, D. M. (1988). _The C Programming Language_ (2nd ed.). Prentice Hall.
- Prata, S. (2013). _C Primer Plus_ (6th ed.). Addison-Wesley Professional.
- GeeksforGeeks. (n.d.). _C Programming Language_. Retrieved from [https://www.geeksforgeeks.org/c-programming-language/](https://www.geeksforgeeks.org/c-programming-language/)
- TutorialsPoint. (n.d.). _C Programming Tutorial_. Retrieved from [https://www.tutorialspoint.com/cprogramming/index.htm](https://www.tutorialspoint.com/cprogramming/index.htm)
- _Programming Assignment.pdf_ - The project specification document provided for this assignment.
