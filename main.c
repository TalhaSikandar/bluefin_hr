#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "hr.h"

#define POLL_INTERVAL 1 // Polling interval in seconds

int main() {
    // Define the functions in each library
    const char *library1_functions[] = {"func1", "func2"};
    const char *library2_functions[] = {"func3", "func4"};

    // Create library structures
    HotReloadLibrary libraries[MAX_LIBRARIES] = {0};

    // Load libraries and their functions
    if (load_library(&libraries[0], "library1.so", library1_functions, 2) != 0) return -1;
    if (load_library(&libraries[1], "library2.so", library2_functions, 2) != 0) return -1;

    // Call a function from library 1
    int num = 10;
    generic_call(&libraries[0], "func1", &num);

    // Call a function from library 2
    double dnum = 3.14;
    generic_call(&libraries[1], "func4", &dnum);

    // Start the polling mechanism to check for library updates
    while (1) {
        sleep(POLL_INTERVAL); // Wait for the polling interval
        // Check and reload libraries if necessary
        check_and_reload_library(&libraries[0], "library1.so", library1_functions, 2);
        check_and_reload_library(&libraries[1], "library2.so", library2_functions, 2);
    }

    // Clean up and unload libraries before exit
    unload_library(&libraries[0]);
    unload_library(&libraries[1]);

    return 0;
}
