#include <stdio.h>
#include<stdlib.h>
#include<conio.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define DATABASE_FILE "blood_bank_database.txt"

// Structure to store person data
struct Person {
    char name[MAX_NAME_LENGTH];
    char bloodGroup[5]; // For future expansion, e.g., 'A+', 'B-', etc.
    char contactNumber[15];
    char address[MAX_ADDRESS_LENGTH];
};

// Function prototypes
void displayMenu();
void readDatabase();
void addPersonData();
void deletePersonData();
void findPeopleByBloodGroup();

int main() {
    int choice;
    
    while (1) {
        system("cls");
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        getchar(); // Clear the newline character from the input buffer
        switch (choice) {
            case 1:
                readDatabase();
                getch();
                break;
            case 2:
                addPersonData();
                getch();
                break;
            case 3:
                deletePersonData();
                getch();
                break;
            case 4:
                findPeopleByBloodGroup();
                getch();
                break;
                case 5:exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
                getch();
        }
    }
    return 0;
}

void displayMenu() {
    printf("\nBlood Bank Database Menu");
    printf("\n------------------------\n");
    printf("1. Display all records\n");
    printf("2. Add new data\n");
    printf("3. Delete data\n");
    printf("4. Blood group needed\n");
    printf("5. Exit\n");
}
 
 
void readDatabase() {
    struct Person person;
    FILE* database = fopen(DATABASE_FILE, "r");

    if (database == NULL) {
        printf("Database file does not exist. Please add data first.\n");
        return;
    }

    printf("\nBlood Bank Database Contents:\n");
    printf("--------------------------------\n");
    while (fread(&person, sizeof(struct Person), 1, database)) {
        printf("Name: %s\n", person.name);
        printf("Blood Group: %s\n", person.bloodGroup);
        printf("Contact Number: %s\n", person.contactNumber);
        printf("Address: %s\n", person.address);
        printf("--------------------------------\n");
    }

    fclose(database);
}

// Function to update the database
void addPersonData() {
    struct Person person;
    FILE* database = fopen(DATABASE_FILE, "a");

    printf("\nEnter the name: ");
    fgets(person.name, MAX_NAME_LENGTH, stdin);
    person.name[strcspn(person.name, "\n")] = '\0'; 

    printf("Enter the blood group: ");
    fgets(person.bloodGroup, 5, stdin);
    person.bloodGroup[strcspn(person.bloodGroup, "\n")] = '\0'; 

    printf("Enter the contact number: ");
    fgets( person.contactNumber, 15, stdin);
    person.contactNumber[strcspn(person.contactNumber, "\n")] = '\0'; 

    printf("Enter the address: ");
    fgets( person.address, MAX_ADDRESS_LENGTH, stdin);
    person.address[strcspn(person.address, "\n")] = '\0'; 

    fwrite(&person, sizeof(struct Person), 1, database);
    fclose(database);

    printf("Person data added successfully!\n");
}

// Function to delete a person from the database
void deletePersonData() {
    struct Person person;
    char nameToDelete[MAX_NAME_LENGTH];
    FILE* sourceDatabase = fopen(DATABASE_FILE, "r");
    FILE* tempDatabase = fopen("temp_blood_bank_database.txt", "w");

    if (sourceDatabase == NULL) {
        printf("Database file does not exist. Please add data first.\n");
        return;
    }
    // Search for the person by name
    printf("\nEnter the name of the person to delete: ");
    fgets(nameToDelete, MAX_NAME_LENGTH, stdin);
    nameToDelete[strcspn(nameToDelete, "\n")] = '\0'; 

    int found = 0;
    while (fread(&person, sizeof(struct Person), 1, sourceDatabase)) {
        if (strcmp(person.name, nameToDelete) != 0) {
            fwrite(&person, sizeof(struct Person), 1, tempDatabase);
        } else {
            found = 1;
        }
    }

    fclose(sourceDatabase);
    fclose(tempDatabase);

    if (found) {
        remove(DATABASE_FILE);
        rename("temp_blood_bank_database.txt", DATABASE_FILE);
        printf("Person data deleted successfully!\n");
    } else {
        remove("temp_blood_bank_database.txt");
        printf("Person not found in the database.\n");
    }
}
// Function to search for people with a specific blood group
void findPeopleByBloodGroup() {
    struct Person person;
    char bloodGroup[5];
    FILE* database = fopen(DATABASE_FILE, "r");

    if (database == NULL) {
        printf("Database file does not exist. Please add data first.\n");
        return;
    }
    printf("\nEnter the blood group: ");
    fgets(bloodGroup, 5, stdin);
    bloodGroup[strcspn(bloodGroup, "\n")] = '\0'; 
    printf("\nPeople with blood group %s:\n", bloodGroup);
    printf("--------------------------------\n");
    int found = 0;
    while (fread(&person, sizeof(struct Person), 1, database)) {
        if (strcmp(person.bloodGroup, bloodGroup) == 0) {
            printf("Name: %s\n", person.name);
            printf("Contact Number: %s\n", person.contactNumber);
            printf("Address: %s\n", person.address);
            printf("--------------------------------\n");
            found = 1;
        }
    }
    fclose(database);
    if (!found) {
        printf("\t!!! ALERT !!!\n");
        printf("No people with blood group %s found in the database.\nSEARCH PEOPLE HAVING BLOOD GROUP %s !!", bloodGroup,bloodGroup);
        
    }
}