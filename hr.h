#ifndef HR_H_
#define HR_H_

#include <stdio.h>      // Standard I/O operations
#include <stdlib.h>     // Memory allocation and other utilities
#include <string.h>     // String manipulation functions
#include <dlfcn.h>      // Dynamic linking functions
#include <pthread.h>    // For threading, if needed
#include <unistd.h>     // For POSIX API (e.g., sleep)
#include <sys/stat.h>   // File statistics for timestamps
#include <time.h>       // Time functions

// Maximum number of libraries that can be managed
#define MAX_LIBRARIES 10

/**
 * Structure representing a generic function in a dynamic library.
 */
typedef struct {
    const char *func_name;  // Name of the function
    void *func_ptr;         // Pointer to the function
    void *user_data;        // Optional user data to pass to the function
} GenericFunction;

/**
 * Structure representing a dynamically loaded library with its associated metadata.
 */
typedef struct {
    void *handle;                 // Handle to the dynamic library
    const char *library_path;     // File path of the library
    GenericFunction *functions;   // Array of functions in the library
    int num_functions;            // Number of functions loaded from the library
    time_t last_modified;         // Last modification timestamp of the library file
} HotReloadLibrary;

/**
 * Loads a shared library and retrieves specified functions dynamically.
 * 
 * @param lib           Pointer to the HotReloadLibrary structure.
 * @param library_path  Path to the shared library file.
 * @param function_names Array of function names to load from the library.
 * @param num_functions Number of functions to load.
 * @return              0 on success, -1 on failure.
 */
int load_library(HotReloadLibrary *lib, const char *library_path, const char *function_names[], int num_functions);

/**
 * Invokes a generic function from the dynamically loaded library.
 * 
 * @param lib       Pointer to the HotReloadLibrary structure.
 * @param func_name Name of the function to invoke.
 * @param params    Parameters to pass to the function.
 */
void generic_call(HotReloadLibrary *lib, const char *func_name, void *params);

/**
 * Unloads a dynamically loaded library and releases associated resources.
 * 
 * @param lib Pointer to the HotReloadLibrary structure.
 */
void unload_library(HotReloadLibrary *lib);

/**
 * Reloads a library and its functions if necessary.
 * 
 * @param lib           Pointer to the HotReloadLibrary structure.
 * @param library_path  Path to the shared library file.
 * @param function_names Array of function names to load from the library.
 * @param num_functions Number of functions to load.
 * @return              0 on success, -1 on failure.
 */
int reload_library(HotReloadLibrary *lib, const char *library_path, const char *function_names[], int num_functions);

/**
 * Checks if the library file has been modified and reloads it if needed.
 * 
 * @param lib           Pointer to the HotReloadLibrary structure.
 * @param library_path  Path to the shared library file.
 * @param function_names Array of function names to load from the library.
 * @param num_functions Number of functions to load.
 */
void check_and_reload_library(HotReloadLibrary *lib, const char *library_path, const char *function_names[], int num_functions);

#endif // HR_H_
