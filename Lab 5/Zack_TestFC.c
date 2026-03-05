// Zack Lee
// 03/03/2026
// Zack_TestFC.c

#include "Zack_LibFC.h"

int main(void) {
    // Define variables to control the menu loop and track the currently open file
    bool check = true;
    bool hasOpenFile = false;
    char currentFile[FILENAME_SIZE] = "";

    // Main menu loop to interact with the user and perform file operations based on their choices
    while (check) {
        int choice;
        int ch;

        // Display the menu options to the user
        displayMenu();

        // Prompt the user for their choice and read it safely
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            continue;
        }

        // Validate the user's choice to ensure it is within the acceptable range
        if (choice < 1 || choice > 7) {
            printf("Invalid choice. Please select a number between 1 and 7.\n");
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            continue;
        }

        // Perform the action corresponding to the user's choice using a switch statement
        switch (choice) {
            case 1:
                createFile();
                pressEnterToContinue();
                break;
            case 2:
                hasOpenFile = openFile(currentFile);
                pressEnterToContinue();
                break;
            case 3:
                if (!hasOpenFile) {
                    printf("No file is currently open. Use option 2 first.\n");
                } else {
                    writeFile(currentFile);
                }
                pressEnterToContinue();
                break;
            case 4:
                if (!hasOpenFile) {
                    printf("No file is currently open. Use option 2 first.\n");
                } else {
                    readFile(currentFile);
                }
                pressEnterToContinue();
                break;
            case 5:
                if (!hasOpenFile) {
                    printf("No file is currently open.\n");
                } else {
                    closeFile(currentFile);
                    hasOpenFile = false;
                }
                pressEnterToContinue();
                break;
            case 6:
                deleteFile();
                pressEnterToContinue();
                break;
            case 7:
                printf("Exiting the program. Goodbye!\n");
                check = false;
                break;
            default:
                break;
        }
    }

    // Return 0 to indicate successful program termination
    return 0;
}
