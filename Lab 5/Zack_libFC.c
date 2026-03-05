// Zack Lee
// 03/03/2026
// Lab 5

#include "Zack_LibFC.h"

// Function definitions for file operations
// Function to create a file based on user input
void createFile(void) {
    // Define a buffer to hold the filename input
    char file[FILENAME_SIZE];

    // Prompt the user for the filename and read it safely
    printf("Enter the filename to create: ");
    if (scanf("%255s", file) != 1) {
        printf("Invalid filename input.\n");
        return;
    }

    // check if the file already exists by trying to open it for reading
    FILE *checkFile = fopen(file, "r");
    if (checkFile != NULL) {
        printf("File '%s' already exists. Please choose a different name.\n", file);
        fclose(checkFile);
        return;
    }

    // Attempt to create the file
    FILE *fp = fopen(file, "w");
    if (fp == NULL) {
        printf("Error creating file '%s'.\n", file);
        return;
    }

    // If the file was created successfully, print a confirmation message
    printf("File '%s' created successfully.\n", file);
    fclose(fp);
}

// Function to open a file and check if it exists
bool openFile(char *fileName) {
    
    printf("Enter the filename to open: ");
    if (scanf("%255s", fileName) != 1) {
        printf("Invalid filename input.\n");
        return false;
    }

    // Attempt to open the file for reading to check if it exists
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error opening file '%s'.\n", fileName);
        return false;
    }

    // If the file was opened successfully, print a confirmation message
    printf("File '%s' opened successfully.\n", fileName);
    fclose(fp);
    return true;
}

// Function to write predefined content to the currently open file
void writeFile(const char *fileName) {
    // Check if a file is currently open before attempting to write
    if (fileName == NULL || fileName[0] == '\0') {
        printf("No file is currently open. Use option 2 first.\n");
        return;
    }

    // Attempt to open the file for writing
    FILE *fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Error opening file '%s' for writing.\n", fileName);
        return;
    }

    // Define the content to be written to the file
    const char *intro =
        "Hello, my name is Zack Lee.\n"
        "I am a computer science student at Central Washington University,\n"
        "and I am currently taking CS 470: Operating Systems.\n"
        "This project helps me understand file handling in C.\n"
        "I am excited to learn more about operating systems and how they manage files.\n"
        "Thank you for reading my introduction!\n";

    // Write the content to the file and print a confirmation message
    fprintf(fp, "%s", intro);
    printf("Data written to file '%s' successfully.\n", fileName);
    fclose(fp);
}

// Function to read and display the content of the currently open file
void readFile(const char *fileName) {
    // Check if a file is currently open before attempting to read
    if (fileName == NULL || fileName[0] == '\0') {
        printf("No file is currently open. Use option 2 first.\n");
        return;
    }

    // Attempt to open the file for reading
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error opening file '%s' for reading.\n", fileName);
        return;
    }

    // Read the content of the file line by line and print it to the console
    char line[100];
    printf("Reading from file '%s'... \n", fileName);
    printf("Datat read from file '%s': \n", fileName);
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    // close the file
    fclose(fp);
}

// Function to close the currently open file by clearing the filename buffer
void closeFile(char *fileName) {
    if (fileName == NULL) {
        return;
    }

    fileName[0] = '\0';
    printf("File closed.\n");
}

// Function to delete a file based on user input
void deleteFile(void) {
    // Define a buffer to hold the filename input
    char file[FILENAME_SIZE];

    // Prompt the user for the filename and read it safely
    printf("Enter the filename to delete: ");
    if (scanf("%255s", file) != 1) {
        printf("Invalid filename input.\n");
        return;
    }

    // Attempt to delete the file and print a confirmation message based on the result
    if (remove(file) == 0) {
        printf("File '%s' deleted successfully.\n", file);
    } else {
        printf("Error deleting file '%s'.\n", file);
    }
}

// Function to prompt the user to press Enter to continue
void pressEnterToContinue(void) {
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    printf("\nPress Enter to continue...");
    (void)getchar();
}

// function to display the menu options to the user
void displayMenu(void) {
    // Display the menu options to the user
        printf("\n\n--- Menu ---\n");
        printf("1. Create a file\n");
        printf("2. Open a file\n");
        printf("3. Write to a file\n");
        printf("4. Read from a file\n");
        printf("5. Close a file\n");
        printf("6. Delete a file\n");
        printf("7. Exit\n");
}
