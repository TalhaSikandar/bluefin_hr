#include "hr.h"

// Function to get the file modification timestamp
time_t get_file_modification_time(const char *file_path) {
    struct stat file_stat;
    if (stat(file_path, &file_stat) == 0) {
        return file_stat.st_mtime;
    }
    fprintf(stderr, "Error retrieving file stats for '%s': %s\n", file_path, strerror(errno));
    return -1; // Return -1 if stat fails
}

// Function to load a shared library and retrieve functions dynamically
int load_library(HotReloadLibrary *lib, const char *library_path, const char *function_names[], int num_functions) {
    if (!lib || !library_path || !function_names || num_functions <= 0) {
        fprintf(stderr, "Invalid arguments passed to load_library\n");
        return -1;
    }

    // Open the dynamic library
    lib->library_path = library_path;
    lib->handle = dlopen(library_path, RTLD_LAZY);
    if (!lib->handle) {
        fprintf(stderr, "Error loading library '%s': %s\n", library_path, dlerror());
        return -1;
    }

    // Allocate memory for functions
    lib->num_functions = num_functions;
    lib->functions = (GenericFunction *)malloc(num_functions * sizeof(GenericFunction));
    if (!lib->functions) {
        fprintf(stderr, "Error allocating memory for functions in library '%s'\n", library_path);
        dlclose(lib->handle);
        lib->handle = NULL;
        return -1;
    }

    // Load each function dynamically from the library
    for (int i = 0; i < num_functions; ++i) {
        lib->functions[i].func_name = function_names[i];
        lib->functions[i].func_ptr = dlsym(lib->handle, function_names[i]);
        if (!lib->functions[i].func_ptr) {
            fprintf(stderr, "Error finding function '%s' in library '%s': %s\n", function_names[i], library_path, dlerror());
            free(lib->functions);
            lib->functions = NULL;
            dlclose(lib->handle);
            lib->handle = NULL;
            return -1;
        }
        lib->functions[i].user_data = NULL; // Initialize user data to NULL
    }

    // Get the file's modification timestamp
    lib->last_modified = get_file_modification_time(library_path);
    if (lib->last_modified == -1) {
        fprintf(stderr, "Error retrieving modification time for library '%s'\n", library_path);
    }

    return 0;
}

// Function to invoke a generic function from the library
void generic_call(HotReloadLibrary *lib, const char *func_name, void *params) {
    if (!lib || !func_name) {
        fprintf(stderr, "Invalid arguments passed to generic_call\n");
        return;
    }

    for (int i = 0; i < lib->num_functions; ++i) {
        if (strcmp(lib->functions[i].func_name, func_name) == 0) {
            // Cast the function pointer to the appropriate function type
            void (*function)(void *) = (void (*)(void *))lib->functions[i].func_ptr;
            function(params); // Call the function with the provided parameters
            return;
        }
    }
    fprintf(stderr, "Function '%s' not found in library '%s'\n", func_name, lib->library_path);
}

// Function to unload a library and clean up resources
void unload_library(HotReloadLibrary *lib) {
    if (!lib) return;

    if (lib->handle) {
        // Unload the library
        dlclose(lib->handle);
        lib->handle = NULL;
    }

    // Free the function array memory
    if (lib->functions) {
        free(lib->functions);
        lib->functions = NULL;
    }

    lib->num_functions = 0;
}

// Function to reload a library and its functions
int reload_library(HotReloadLibrary *lib, const char *library_path, const char *function_names[], int num_functions) {
    if (!lib || !library_path || !function_names || num_functions <= 0) {
        fprintf(stderr, "Invalid arguments passed to reload_library\n");
        return -1;
    }

    // Unload the old library if already loaded
    unload_library(lib);

    // Load the new library
    return load_library(lib, library_path, function_names, num_functions);
}

// Function to check the library modification time and reload if modified
void check_and_reload_library(HotReloadLibrary *lib, const char *library_path, const char *function_names[], int num_functions) {
    if (!lib || !library_path || !function_names || num_functions <= 0) {
        fprintf(stderr, "Invalid arguments passed to check_and_reload_library\n");
        return;
    }

    time_t current_mod_time = get_file_modification_time(library_path);
    if (current_mod_time == -1) {
        fprintf(stderr, "Error retrieving modification time for library '%s'\n", library_path);
        return;
    }

    if (current_mod_time > lib->last_modified) {
        printf("Library '%s' has been modified. Reloading...\n", library_path);
        if (reload_library(lib, library_path, function_names, num_functions) == 0) {
            printf("Library '%s' successfully reloaded.\n", library_path);
        } else {
            fprintf(stderr, "Failed to reload library '%s'\n", library_path);
        }
    }
}
