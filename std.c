#include <stdio.h>
#include <stdbool.h>

// Define the maximum length for various fields
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_EMAIL_LENGTH 100
#define MAX_SEMESTER_LENGTH 50
#define MAX_SECTION_LENGTH 20

// Define the structure for a student
struct Student {
    int rollNumber;
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char semester[MAX_SEMESTER_LENGTH];
    char section[MAX_SECTION_LENGTH];
    float marks;
};

// Function to check if a roll number already exists in the file
bool isRollNumberExist(FILE *file, int rollNumber) {
    struct Student student;
    
    rewind(file); // Go to the beginning of the file
    
    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.rollNumber == rollNumber) {
            return true; // Roll number already exists
        }
    }
    
    return false; // Roll number does not exist
}

// Function to add a student to the file
void addStudent(FILE *file) {
    struct Student student;
    printf("Enter Roll Number: ");
    scanf("%d", &student.rollNumber);
    
    // Check if the roll number already exists
    if (isRollNumberExist(file, student.rollNumber)) {
        printf("Student with Roll Number %d already exists. Student not added.\n", student.rollNumber);
        return;
    }
    
    printf("Enter Name: ");
    scanf(" %[^\n]", student.name);
    printf("Enter Phone Number: ");
    scanf(" %[^\n]", student.phone);
    printf("Enter Email: ");
    scanf(" %[^\n]", student.email);
    printf("Enter Semester: ");
    scanf(" %[^\n]", student.semester);
    printf("Enter Section: ");
    scanf(" %[^\n]", student.section);
    printf("Enter Marks: ");
    scanf("%f", &student.marks);

    fseek(file, 0, SEEK_END); // Move to the end of the file
    fwrite(&student, sizeof(struct Student), 1, file);
    printf("Student added successfully!\n");
}

// Function to display a student's information by roll number
void displayStudentByRoll(FILE *file) {
    struct Student student;
    int rollToSearch;

    printf("Enter Roll Number to search: ");
    scanf("%d", &rollToSearch);

    rewind(file); // Go to the beginning of the file

    // Create a visual border for the student data
    printf("\n+--------+-----------+----------------------+----------------------+----------------------+----------------------+----------------------+----------+\n");

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.rollNumber == rollToSearch) {
            // Student found, display their information
            printf("| %6s | %9s | %-20s | %-20s | %-20s | %-20s | %-20s | %8s |\n", "Serial", "Roll No.", "Name", "Phone Number", "Email", "Semester", "Section", "Marks");
            printf("+--------+-----------+----------------------+----------------------+----------------------+----------------------+----------------------+----------+\n");
            printf("| %6d | %9d | %-20s | %-20s | %-20s | %-20s | %-20s | %8.2f |\n",
                   1, student.rollNumber, student.name, student.phone, student.email, student.semester, student.section, student.marks);
            printf("+--------+-----------+----------------------+----------------------+----------------------+----------------------+----------------------+----------+\n");
            return; // Exit the function after finding the student
        }
    }

    // If the loop completes without finding the student
    printf("Student with Roll Number %d not found.\n", rollToSearch);
}


// Function to update a student's information by roll number
void updateStudent(FILE *file) {
    struct Student student, updatedStudent;
    int rollToUpdate;

    printf("Enter Roll Number to update: ");
    scanf("%d", &rollToUpdate);

    long int currentPosition;
    int found = 0;

    FILE *tempFile = fopen("temp.dat", "wb"); // Create a temporary file

    rewind(file); // Go to the beginning of the original file

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.rollNumber == rollToUpdate) {
            // Student found, display their information
            printf("\nCurrent Student Information:\n");
            printf("Roll Number: %d\n", student.rollNumber);
            printf("Name: %s\n", student.name);
            printf("Phone Number: %s\n", student.phone);
            printf("Email: %s\n", student.email);
            printf("Semester: %s\n", student.semester);
            printf("Section: %s\n", student.section);
            printf("Marks: %.2f\n", student.marks);

            printf("\nEnter updated information:\n");
            updatedStudent.rollNumber = student.rollNumber; // Keep the same roll number

            printf("Enter Name: ");
            scanf(" %[^\n]", updatedStudent.name);
            printf("Enter Phone Number: ");
            scanf(" %[^\n]", updatedStudent.phone);
            printf("Enter Email: ");
            scanf(" %[^\n]", updatedStudent.email);
            printf("Enter Semester: ");
            scanf(" %[^\n]", updatedStudent.semester);
            printf("Enter Section: ");
            scanf(" %[^\n]", updatedStudent.section);
            printf("Enter Marks: ");
            scanf("%f", &updatedStudent.marks);

            found = 1; // Student found and updated

            // Write the updated student record to the temporary file
            fwrite(&updatedStudent, sizeof(struct Student), 1, tempFile);
        } else {
            // Copy the student record to the temporary file as is
            fwrite(&student, sizeof(struct Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        printf("Student information updated successfully!\n");
    } else {
        printf("Student with Roll Number %d not found. No updates made.\n", rollToUpdate);
    }

    file = fopen("students.dat", "ab+"); // Reopen the file for future operations
}



// Function to delete a student by roll number
void deleteStudent(FILE *file) {
    struct Student student, emptyStudent;
    int rollToDelete;

    printf("Enter Roll Number to delete: ");
    scanf("%d", &rollToDelete);

    long int currentPosition;
    int found = 0;

    FILE *tempFile = fopen("temp.dat", "wb"); // Create a temporary file

    rewind(file); // Go to the beginning of the original file

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.rollNumber == rollToDelete) {
            // Student found, display their information
            printf("\nStudent Information Deleted:\n");
            printf("Roll Number: %d\n", student.rollNumber);
            printf("Name: %s\n", student.name);
            printf("Phone Number: %s\n", student.phone);
            printf("Email: %s\n", student.email);
            printf("Semester: %s\n", student.semester);
            printf("Section: %s\n", student.section);
            printf("Marks: %.2f\n", student.marks);

            found = 1; // Student found and deleted
        } else {
            // Copy the student record to the temporary file if it's not the one to be deleted
            fwrite(&student, sizeof(struct Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the temporary file
    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        printf("Student information deleted successfully!\n");
    } else {
        printf("Student with Roll Number %d not found. No deletions made.\n", rollToDelete);
    }

    file = fopen("students.dat", "ab+"); // Reopen the file for future operations
}


// Function to view all student records
void viewStudents(FILE *file) {
    struct Student student;
    rewind(file); // Go to the beginning of the file

    // Create a visual border for the table header
    printf("\n+--------+-----------+----------------------+----------------------+----------------------+----------------------+----------------------+----------+\n");
    printf("| Serial | Roll No.  |        Name          |    Phone Number     |        Email         |      Semester        |      Section         |  Marks   |\n");
    printf("+--------+-----------+----------------------+----------------------+----------------------+----------------------+----------------------+----------+\n");

    int serial = 1;

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        // Print student record with reduced space
        printf("| %6d | %9d | %-20s | %-20s | %-20s | %-20s | %-20s | %8.2f |\n",
               serial, student.rollNumber, student.name, student.phone, student.email, student.semester, student.section, student.marks);
        serial++;

        // Print a horizontal border between rows
        printf("+--------+-----------+----------------------+----------------------+----------------------+----------------------+----------------------+----------+\n");
    }
}

int main() {
    FILE *file = fopen("students.dat", "ab+"); // Open or create the binary file for student data

    if (file == NULL) {
        printf("File cannot be opened or created.\n");
        return 1;
    }

    int choice;

    do {
        printf("\nEnter your choice:\n");
        printf("1. Add Student\n");
        printf("2. Display Student by Roll Number\n");
        printf("3. Update Student by Roll Number\n");
        printf("4. Delete Student by Roll Number\n");
        printf("5. View All Students\n");
        printf("6. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(file);
                break;
            case 2:
                displayStudentByRoll(file);
                break;
            case 3:
                updateStudent(file);
                break;
            case 4:
                deleteStudent(file);
                break;
            case 5:
                viewStudents(file);
                break;
            case 6:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("Choose one:\n");

    } while (choice != 6);

    fclose(file); // Close the file before exiting

    return 0;
}
