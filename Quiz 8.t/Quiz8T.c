// Zack Lee
// 03/08/2026
// Quiz 8.T
// Paging Address Translator


#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Define page size and page table
    const int page_size = 1024;
    const int page_table[4] = {5, 2, 9, 1};
    int addresses;

    // Get number of logical addresses from user
    printf("Enter number of logical addresses (N): ");
    if (scanf("%d", &addresses) != 1 || addresses < 0) {
        return 1;
    }

    // Read all logical addresses first
    int *logical_addresses = malloc((size_t)addresses * sizeof(int));
    if (addresses > 0 && logical_addresses == NULL) {
        return 1;
    }

    // Read all logical addresses
    printf("Enter logical address(es), one per line:\n");
    for (int i = 0; i < addresses; i++) {
        if (scanf("%d", &logical_addresses[i]) != 1) {
            free(logical_addresses);
            return 1;
        }
    }

    // Print all translations after input is complete
    for (int i = 0; i < addresses; i++) {
        int logical_address = logical_addresses[i];
        // Calculate page number and offset
        int page_number = logical_address / page_size;
        int offset = logical_address % page_size;

        // Validate page number and calculate physical address
        if (page_number < 0 || page_number > 3) {
            printf("Logical: %d | INVALID (page out of range)\n", logical_address);
        } else {
            // Get frame number from page table and calculate physical address
            int frame = page_table[page_number];
            int physical = frame * page_size + offset;
            printf("Logical: %d | Page: %d | Offset: %d | Frame: %d | Physical: %d\n",
                   logical_address, page_number, offset, frame, physical);
        }
    }

    free(logical_addresses);
    return 0;
}
