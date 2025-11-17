#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 100
#define FILENAME "students.txt"

typedef struct {
    char name[NAME_LEN];
    int roll;
    float marks;
} Student;

typedef struct {
    Student *students;
    int count;
    int capacity;
} StudentList;

// Function Prototypes
void greetUser();
void initList(StudentList *list);
void freeList(StudentList *list);
void addStudent(StudentList *list);
void displayStudents(const StudentList *list);
void modifyStudent(StudentList *list);
void removeStudent(StudentList *list);
int searchStudent(const StudentList *list, int roll);
void displayStudent(const StudentList *list, int idx);
void saveToFile(const StudentList *list);
void loadFromFile(StudentList *list);
void sortStudents(StudentList *list, int ascending);
void averageMarks(const StudentList *list);

int main() {
    greetUser();

    StudentList list;
    initList(&list);

    int choice;
    do {
        printf("\n---- Student Record System ----\n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Modify student record\n");
        printf("4. Remove student record\n");
        printf("5. Search student by roll number\n");
        printf("6. Save records to file\n");
        printf("7. Load records from file\n");
        printf("8. Calculate average marks\n");
        printf("9. Sort records by marks\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                addStudent(&list);
                break;
            case 2:
                displayStudents(&list);
                break;
            case 3:
                modifyStudent(&list);
                break;
            case 4:
                removeStudent(&list);
                break;
            case 5: {
                int roll;
                printf("Enter roll number to search: ");
                scanf("%d", &roll);
                int idx = searchStudent(&list, roll);
                if (idx != -1) {
                    printf("Student found:\n");
                    displayStudent(&list, idx);
                } else {
                    printf("Student with roll number %d not found.\n", roll);
                }
                break;
            }
            case 6:
                saveToFile(&list);
                break;
            case 7:
                loadFromFile(&list);
                break;
            case 8:
                averageMarks(&list);
                break;
            case 9: {
                int order;
                printf("Enter 1 for ascending, 0 for descending: ");
                scanf("%d", &order);
                sortStudents(&list, order);
                break;
            }
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    freeList(&list);
    return 0;
}

void greetUser() {
    char userName[NAME_LEN];
    printf("Welcome to the Student Record System!\n");
    printf("Please enter your name: ");
    fgets(userName, NAME_LEN, stdin);
    userName[strcspn(userName, "\n")] = 0; // Remove trailing newline
    printf("Hello, %s! Let's manage some student records.\n", userName);
}

void initList(StudentList *list) {
    list->students = NULL;
    list->count = 0;
    list->capacity = 0;
}

void freeList(StudentList *list) {
    free(list->students);
    list->students = NULL;
    list->count = 0;
    list->capacity = 0;
}

void ensureCapacity(StudentList *list) {
    if (list->count >= list->capacity) {
        list->capacity = list->capacity == 0 ? 4 : list->capacity * 2;
        list->students = realloc(list->students, list->capacity * sizeof(Student));
        if (list->students == NULL) {
            printf("Memory allocation failed!\n");
            exit(EXIT_FAILURE);
        }
    }
}

void addStudent(StudentList *list) {
    ensureCapacity(list);
    Student s;
    printf("Enter student name: ");
    fgets(s.name, NAME_LEN, stdin);
    s.name[strcspn(s.name, "\n")] = 0;

    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    getchar();

    printf("Enter marks: ");
    scanf("%f", &s.marks);
    getchar();

    printf("%s has %s.\n", s.name, (s.marks > 40) ? "passed" : "failed");

    list->students[list->count++] = s;
    printf("Student record added successfully.\n");
}

void displayStudents(const StudentList *list) {
    if (list->count == 0) {
        printf("No student records to display.\n");
        return;
    }

    printf("\n%-20s %-10s %-10s %-10s\n", "Name", "Roll No", "Marks", "Status");
    for (int i = 0; i < list->count; i++) {
        printf("%-20s %-10d %-10.2f %-10s\n",
               list->students[i].name,
               list->students[i].roll,
               list->students[i].marks,
               (list->students[i].marks > 40) ? "Passed" : "Failed");
    }
}

void displayStudent(const StudentList *list, int idx) {
    printf("Name: %s\n", list->students[idx].name);
    printf("Roll Number: %d\n", list->students[idx].roll);
    printf("Marks: %.2f\n", list->students[idx].marks);
    printf("Status: %s\n", (list->students[idx].marks > 40) ? "Passed" : "Failed");
}

void modifyStudent(StudentList *list) {
    int roll;
    printf("Enter roll number to modify: ");
    scanf("%d", &roll);
    getchar();
    int idx = searchStudent(list, roll);
    if (idx == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("Modifying record for %s (Roll %d):\n", list->students[idx].name, list->students[idx].roll);
    printf("Enter new name (leave blank to keep current): ");
    char newName[NAME_LEN];
    fgets(newName, NAME_LEN, stdin);
    newName[strcspn(newName, "\n")] = 0;
    if (strlen(newName) > 0)
        strcpy(list->students[idx].name, newName);

    printf("Enter new marks (-1 to keep current): ");
    float newMarks;
    scanf("%f", &newMarks);
    getchar();
    if (newMarks >= 0)
        list->students[idx].marks = newMarks;

    printf("Record updated.\n");
}

void removeStudent(StudentList *list) {
    int roll;
    printf("Enter roll number to remove: ");
    scanf("%d", &roll);
    getchar();
    int idx = searchStudent(list, roll);
    if (idx == -1) {
        printf("Student not found.\n");
        return;
    }
    for (int i = idx; i < list->count - 1; i++) {
        list->students[i] = list->students[i + 1];
    }
    list->count--;
    printf("Student record removed.\n");
}

int searchStudent(const StudentList *list, int roll) {
    for (int i = 0; i < list->count; i++) {
        if (list->students[i].roll == roll)
            return i;
    }
    return -1;
}

void saveToFile(const StudentList *list) 
{
    FILE *fp = fopen("./records.txt", "w");  
    if (!fp) {
        printf("Error creating file.\n");
        return;
    }
    for (int i = 0; i < list->count; i++) {
        fprintf(fp, "%s,%d,%.2f\n", list->students[i].name, list->students[i].roll, list->students[i].marks);
    }
    fclose(fp);
    printf("Records saved to file.\n");
}

void loadFromFile(StudentList *list) {
    FILE *fp = fopen("./records.txt", "r");
    if (!fp) {
        printf("Error opening file for reading.\n");
        return;
    }
    freeList(list);
    initList(list);
    char line[NAME_LEN + 30];
    while (fgets(line, sizeof(line), fp)) {
        Student s;
        char *token = strtok(line, ",");
        if (token) strncpy(s.name, token, NAME_LEN);
        token = strtok(NULL, ",");
        if (token) s.roll = atoi(token);
        token = strtok(NULL, ",");
        if (token) s.marks = atof(token);
        ensureCapacity(list);
        list->students[list->count++] = s;
    }
    fclose(fp);
    printf("Records loaded from file.\n");
}

void sortStudents(StudentList *list, int ascending) {
    for (int i = 0; i < list->count - 1; i++) {
        for (int j = i + 1; j < list->count; j++) {
            int condition = ascending
                ? (list->students[i].marks > list->students[j].marks)
                : (list->students[i].marks < list->students[j].marks);
            if (condition) {
                Student temp = list->students[i];
                list->students[i] = list->students[j];
                list->students[j] = temp;
            }
        }
    }
    printf("Records sorted by marks %s.\n", ascending ? "ascending" : "descending");
}

void averageMarks(const StudentList *list) {
    if (list->count == 0) {
        printf("No student records.\n");
        return;
    }
    float sum = 0;
    for (int i = 0; i < list->count; i++) {
        sum += list->students[i].marks;
    }
    printf("Average marks: %.2f\n", sum / list->count);
}