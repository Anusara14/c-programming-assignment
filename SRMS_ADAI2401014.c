#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define MAX_COURSE_LENGTH 30
#define MAX_GENDER_LENGTH 10
#define NUM_SUBJECTS 3
#define PASS_MARK 40.0f
#define DATA_FILE "student_records.dat"

typedef struct {
    int studentID;
    char name[MAX_NAME_LENGTH];
    int age;
    char gender[MAX_GENDER_LENGTH];
    char course[MAX_COURSE_LENGTH];
    int marks[NUM_SUBJECTS];
    float averageMarks;
    char passFailStatus[10];
} Student;

Student studentDatabase[MAX_STUDENTS];
int studentCount = 0;

void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void calculateAverages();
void saveToFile();
void loadFromFile();
void displayMenu();
int findStudentById(int id);
int findStudentByName(const char* name);
void clearInputBuffer();
void readString(char *str, int maxLen);

int main() {
    loadFromFile();
    int choice;

    do {
        system("cls || clear");
        displayMenu();
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a valid number.\n");
            clearInputBuffer();
            choice = -1;
        } else {
            clearInputBuffer();
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: calculateAverages(); break;
            case 7:
                saveToFile();
                printf("Data saved successfully!\n");
                break;
            case 0:
                saveToFile();
                printf("Goodbye!\n");
                break;
            default:
                if (choice != -1) printf("Invalid option. Try again.\n");
        }

        if (choice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
        }

    } while (choice != 0);

    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readString(char *str, int maxLen) {
    if (fgets(str, maxLen, stdin)) {
        str[strcspn(str, "\n")] = 0;
        
        int start = 0;
        while (isspace(str[start])) start++;
        if (start > 0) memmove(str, str + start, strlen(str) - start + 1);
        
        int end = strlen(str) - 1;
        while (end >= 0 && isspace(str[end])) str[end--] = '\0';
    } else {
        str[0] = '\0';
    }
}

void displayMenu() {
    printf("\n--- Student Record System ---\n");
    printf("1. Add New Student\n");
    printf("2. View All Students\n");
    printf("3. Search Student\n");
    printf("4. Update Student\n");
    printf("5. Remove Student\n");
    printf("6. View Averages\n");
    printf("7. Save Data\n");
    printf("0. Exit\n");
    printf("----------------------------\n");
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Database is full.\n");
        return;
    }

    Student newStudent;
    int id;
    int validId = 0;

    while (!validId) {
        printf("Enter Student ID: ");
        if (scanf("%d", &id) != 1) {
            printf("Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        if (findStudentById(id) == -1) {
            newStudent.studentID = id;
            validId = 1;
        } else {
            printf("This ID already exists.\n");
        }
    }

    printf("Enter Name: ");
    readString(newStudent.name, MAX_NAME_LENGTH);

    printf("Enter Age: ");
    while(scanf("%d", &newStudent.age) != 1 || newStudent.age <= 0) {
        printf("Please enter a valid age: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter Gender: ");
    readString(newStudent.gender, MAX_GENDER_LENGTH);

    printf("Enter Course: ");
    readString(newStudent.course, MAX_COURSE_LENGTH);

    printf("Enter Marks for %d subjects:\n", NUM_SUBJECTS);
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        printf("Subject %d: ", i + 1);
        while(scanf("%d", &newStudent.marks[i]) != 1 || newStudent.marks[i] < 0 || newStudent.marks[i] > 100) {
            printf("Marks must be 0-100: ");
            clearInputBuffer();
        }
        clearInputBuffer();
    }

    float sum = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++) sum += newStudent.marks[i];
    newStudent.averageMarks = sum / NUM_SUBJECTS;
    strcpy(newStudent.passFailStatus, (newStudent.averageMarks >= PASS_MARK) ? "Pass" : "Fail");

    studentDatabase[studentCount++] = newStudent;
    printf("Student added successfully.\n");
}

void displayAllStudents() {
    if (studentCount == 0) {
        printf("No students in database.\n");
        return;
    }

    printf("\n--- All Students ---\n");
    printf("%-10s %-20s %-5s %-10s %-15s %-10s %-5s\n",
           "ID", "Name", "Age", "Gender", "Course", "Avg", "Status");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-20s %-5d %-10s %-15s %-10.2f %-5s\n",
               studentDatabase[i].studentID,
               studentDatabase[i].name,
               studentDatabase[i].age,
               studentDatabase[i].gender,
               studentDatabase[i].course,
               studentDatabase[i].averageMarks,
               studentDatabase[i].passFailStatus);
    }
}

void searchStudent() {
    if (studentCount == 0) {
        printf("No students to search.\n");
        return;
    }

    int choice;
    printf("Search by:\n1. ID\n2. Name\nYour choice: ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid choice.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = -1;
    if (choice == 1) {
        int id;
        printf("Enter Student ID: ");
        if (scanf("%d", &id) != 1) {
            printf("Invalid ID.\n");
            clearInputBuffer();
            return;
        }
        clearInputBuffer();
        index = findStudentById(id);
    } else if (choice == 2) {
        char name[MAX_NAME_LENGTH];
        printf("Enter Name: ");
        readString(name, MAX_NAME_LENGTH);
        index = findStudentByName(name);
    } else {
        printf("Invalid option.\n");
        return;
    }

    if (index != -1) {
        printf("\n--- Student Details ---\n");
        printf("ID: %d\n", studentDatabase[index].studentID);
        printf("Name: %s\n", studentDatabase[index].name);
        printf("Age: %d\n", studentDatabase[index].age);
        printf("Gender: %s\n", studentDatabase[index].gender);
        printf("Course: %s\n", studentDatabase[index].course);
        printf("Marks: ");
        for(int i=0; i<NUM_SUBJECTS; i++) printf("%d ", studentDatabase[index].marks[i]);
        printf("\nAverage: %.2f\n", studentDatabase[index].averageMarks);
        printf("Status: %s\n", studentDatabase[index].passFailStatus);
    } else {
        printf("Student not found.\n");
    }
}

void updateStudent() {
    if (studentCount == 0) {
        printf("No students to update.\n");
        return;
    }

    int id;
    printf("Enter Student ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = findStudentById(id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Updating %s:\n", studentDatabase[index].name);
    char buffer[MAX_NAME_LENGTH];

    printf("New Name (%s): ", studentDatabase[index].name);
    readString(buffer, MAX_NAME_LENGTH);
    if (strlen(buffer) > 0) strcpy(studentDatabase[index].name, buffer);

    printf("New Age (%d): ", studentDatabase[index].age);
    int newAge;
    if (scanf("%d", &newAge) == 1 && newAge > 0) {
        studentDatabase[index].age = newAge;
    } else {
        printf("Keeping current age.\n");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("New Gender (%s): ", studentDatabase[index].gender);
    readString(buffer, MAX_GENDER_LENGTH);
    if (strlen(buffer) > 0) strcpy(studentDatabase[index].gender, buffer);

    printf("New Course (%s): ", studentDatabase[index].course);
    readString(buffer, MAX_COURSE_LENGTH);
    if (strlen(buffer) > 0) strcpy(studentDatabase[index].course, buffer);

    printf("Update marks? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    clearInputBuffer();
    
    if (choice == 'y' || choice == 'Y') {
        printf("Enter new marks:\n");
        for (int i = 0; i < NUM_SUBJECTS; i++) {
            printf("Subject %d (%d): ", i + 1, studentDatabase[index].marks[i]);
            int newMark;
            if(scanf("%d", &newMark) == 1 && newMark >= 0 && newMark <= 100) {
                studentDatabase[index].marks[i] = newMark;
            } else {
                printf("Keeping current mark.\n");
                clearInputBuffer();
            }
            clearInputBuffer();
        }
        
        float sum = 0;
        for (int i = 0; i < NUM_SUBJECTS; i++) sum += studentDatabase[index].marks[i];
        studentDatabase[index].averageMarks = sum / NUM_SUBJECTS;
        strcpy(studentDatabase[index].passFailStatus, 
              (studentDatabase[index].averageMarks >= PASS_MARK) ? "Pass" : "Fail");
    }

    printf("Update complete.\n");
}

void deleteStudent() {
    if (studentCount == 0) {
        printf("No students to remove.\n");
        return;
    }

    int id;
    printf("Enter Student ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    int index = findStudentById(id);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Removing %s (ID: %d)\n", studentDatabase[index].name, id);

    for (int i = index; i < studentCount - 1; i++) {
        studentDatabase[i] = studentDatabase[i + 1];
    }
    studentCount--;
    printf("Student removed.\n");
}

void calculateAverages() {
    if (studentCount == 0) {
        printf("No students to display.\n");
        return;
    }

    printf("\n--- Student Averages ---\n");
    printf("%-10s %-20s %-10s %-5s\n", "ID", "Name", "Average", "Status");
    printf("--------------------------------------\n");

    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-20s %-10.2f %-5s\n",
               studentDatabase[i].studentID,
               studentDatabase[i].name,
               studentDatabase[i].averageMarks,
               studentDatabase[i].passFailStatus);
    }
    printf("Passing mark: %.2f\n", PASS_MARK);
}

void saveToFile() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Error saving data.\n");
        return;
    }

    if (fwrite(&studentCount, sizeof(int), 1, fp) != 1) {
        printf("Error saving data.\n");
        fclose(fp);
        return;
    }

    if (studentCount > 0) {
        if (fwrite(studentDatabase, sizeof(Student), studentCount, fp) != studentCount) {
            printf("Error saving student records.\n");
        }
    }

    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        if (errno != ENOENT) printf("Error loading data.\n");
        studentCount = 0;
        return;
    }

    if (fread(&studentCount, sizeof(int), 1, fp) != 1) {
        studentCount = 0;
        fclose(fp);
        return;
    }

    if (studentCount < 0 || studentCount > MAX_STUDENTS) {
        printf("Data file corrupted. Starting fresh.\n");
        studentCount = 0;
        fclose(fp);
        return;
    }

    if (studentCount > 0) {
        size_t readCount = fread(studentDatabase, sizeof(Student), studentCount, fp);
        if (readCount != studentCount) {
            printf("Some data may be missing.\n");
            studentCount = readCount;
        }
    }

    fclose(fp);
    printf("Loaded %d student records.\n", studentCount);
}

int findStudentById(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (studentDatabase[i].studentID == id) return i;
    }
    return -1;
}

int findStudentByName(const char* name) {
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(studentDatabase[i].name, name) == 0) return i;
    }
    return -1;
}