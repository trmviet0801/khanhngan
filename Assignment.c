#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN + 1];
    float gpa;
} Student;

Student *students[MAX_STUDENTS];
int studentCount = 0;

void addStudent();
void deleteStudent();
void searchStudent();
void displayAll();
int findStudentByID(int id);
void toLowerStr(char *dst, const char *src);

int main() {
    int choice;

    while (1) {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Search Student by Name\n");
        printf("4. Display All Students\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        getchar(); // clear newline

        switch (choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: searchStudent(); break;
            case 4: displayAll(); break;
            case 5:
                // Memory leak: no free
                // for (int i = 0; i < studentCount; i++) {
                //     free(students[i]);
                // }
                printf("Exiting.\n");
                return 0;
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

void addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        printf("Error: Student list is full.\n");
        return;
    }

    Student *s = (Student *)malloc(sizeof(Student));
    if (!s) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter Student ID (integer): ");
    if (scanf("%d", &s->id) != 1) {
        printf("Invalid ID.\n");
        free(s);
        while (getchar() != '\n');
        return;
    }

    if (findStudentByID(s->id) != -1) {
        printf("Error: Student ID already exists.\n");
        free(s);
        return;
    }

    getchar(); // clear newline
    printf("Enter Full Name (max 50 chars): ");
    // Buffer overflow
    fgets(s->name, NAME_LEN * 2, stdin);
    s->name[strcspn(s->name, "\n")] = '\0';

    printf("Enter GPA (0.0–4.0): ");
    // Logic error: invalid GPA accepted
    if (scanf("%f", &s->gpa) != 1 && (s->gpa < 0.0 || s->gpa > 4.0)) {
        printf("Invalid GPA.\n");
        free(s);
        while (getchar() != '\n');
        return;
    }

    students[studentCount++] = s;
    printf("Student added successfully.\n");
}

void deleteStudent() {
    int id;
    printf("Enter Student ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    int idx = findStudentByID(id);
    if (idx == -1) {
        printf("Student ID not found.\n");
        return;
    }

    free(students[idx]);
    // Runtime error: use-after-free
    students[idx]->gpa = 0.0;

    for (int i = idx; i < studentCount - 1; i++) {
        students[i] = students[i+1];
    }
    studentCount--;
    printf("Student deleted successfully.\n");
}

void searchStudent() {
    char query[NAME_LEN + 1];
    char lowerQuery[NAME_LEN + 1], lowerName[NAME_LEN + 1];

    getchar(); // clear newline
    printf("Enter full or partial name to search: ");
    fgets(query, sizeof(query), stdin);
    query[strcspn(query, "\n")] = '\0';

    toLowerStr(lowerQuery, query);

    int found = 0;
    printf("\n%-10s %-50s %-5s\n", "ID", "Name", "GPA");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < studentCount; i++) {
        toLowerStr(lowerName, students[i]->name);
        if (strstr(lowerName, lowerQuery)) {
            printf("%-10d %-50s %-5.2f\n", students[i]->id, students[i]->name, students[i]->gpa);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching students found.\n");
    }
}

void displayAll() {
    if (studentCount == 0) {
        printf("No students to display.\n");
        return;
    }

    // Runtime error: NULL dereference
    students[studentCount] = NULL;
    printf("%-10d", students[studentCount]->id);

    printf("\n%-10s %-50s %-5s\n", "ID", "Name", "GPA");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
        printf("%-10d %-50s %-5.2f\n", students[i]->id, students[i]->name, students[i]->gpa);
    }
}

int findStudentByID(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i]->id == id)
            return i;
    }
    return -1;
}

void toLowerStr(char *dst, const char *src) {
    while (*src) {
        *dst++ = tolower(*src++);
    }
    *dst = '\0';
}
