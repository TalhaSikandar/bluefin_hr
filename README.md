# Hot Reloading Dynamic Library Framework

This project demonstrates a framework for dynamically loading shared libraries at runtime, enabling hot-reloading functionality. The framework is designed to facilitate efficient development and deployment of modular systems where updates to library functions do not require restarting the application.

---

## 📚 **Static Linking vs Dynamic Linking**

### **Static Linking**
- In static linking, all the required libraries are bundled with the executable at compile-time.
- **Pros**:
  - Single executable file; no dependency on external libraries at runtime.
  - Faster execution since no linking is required at runtime.
- **Cons**:
  - Larger binary size.
  - Requires recompilation for any library updates.

### **Dynamic Linking**
- Dynamic linking resolves library references at runtime using shared objects (`.so` files in Linux).
- **Pros**:
  - Smaller binary size.
  - Libraries can be updated without recompiling the application.
  - Enables features like hot-reloading.
- **Cons**:
  - Slight overhead due to runtime linking.
  - Dependency on external library files.

---

## 🚀 **Project Overview**

This framework extends the concept of dynamic linking to implement **hot-reloading**:
- Dynamically loaded libraries can be updated without restarting the application.
- The framework monitors libraries for changes and reloads them automatically.

---

## 🔧 **How It Works**

The framework consists of:
1. **Core Functions (`hr.h`, `hr.c`)**  
   - Provides the APIs for loading, unloading, and reloading shared libraries dynamically.  
   - Monitors file modification timestamps to detect changes and trigger reloads.  

2. **Main Program (`main.c`)**  
   - Demonstrates the use of the provided APIs to dynamically load and invoke functions from shared libraries.

3. **Shared Libraries (`library1.c`, `library2.c`)**  
   - Example libraries containing functions that can be hot-reloaded.

4. **Script (`script.sh`)**  
   - Automates the process of compilation and execution for the framework and libraries.

---

## 🔧 **Steps to Use the Framework**

### 1. **Clone the Repository**
   ```bash
   git clone <repository-url>
   cd hot_reloading
   ```

### 2. **Understand the Directory Structure**
   - `hr.h` / `hr.c`: Core hot-reloading wrapper implementation.
   - `main.c`: Demonstrates the API usage.
   - `library1.c` / `library2.c`: Example shared libraries.
   - `script.sh`: Automates building and running the example program.

### 3. **Compile the Main Program and Libraries**
   Run the provided script:
   ```bash
   ./script.sh
   ```
   **What it does:**
   - Creates an `output/` directory for binaries.
   - Compiles `main.c` into the main program.
   - Compiles `library1.c` and `library2.c` into shared libraries (`.so` files).

### 4. **Run the Main Program**
   ```bash
   ./output/main
   ```

   **Behavior:**
   - Dynamically loads functions from `library1.so` and `library2.so`.
   - Invokes library functions via the `generic_call` API.
   - Monitors library modification timestamps to detect updates.

---

## 📄 **Using the APIs in Your Project**

Here’s a step-by-step guide to integrating the framework:

### **1. Initialize a `HotReloadLibrary`**
Create an instance of the `HotReloadLibrary` structure for each library you want to load.

### **2. Load a Library**
Use `load_library` to load a shared library and its functions dynamically:
```c
const char *functions[] = {"func1", "func2"}; // Function names in the library
HotReloadLibrary my_lib;

if (load_library(&my_lib, "path/to/library.so", functions, 2) != 0) {
    fprintf(stderr, "Failed to load library\n");
    return -1;
}
```

### **3. Invoke a Function**
Use `generic_call` to invoke a function from the library:
```c
generic_call(&my_lib, "func1", params);
```

### **4. Monitor for Changes**
Call `check_and_reload_library` periodically to check for updates and reload the library if it has been modified:
```c
check_and_reload_library(&my_lib, "path/to/library.so", functions, 2);
```

### **5. Unload the Library**
Clean up resources when the library is no longer needed:
```c
unload_library(&my_lib);
```

---

## 🔄 **Practical Usage**

### **Customizing `script.sh`**
Modify `script.sh` to adapt the framework to your project:
- Update `FLASHES` to include all `.c` files for your shared libraries.
- Update `MAIN_PROGRAM_SRC` to point to your main application.

Example:
```bash
FLASHES=("libmath.c" "libutils.c")
MAIN_PROGRAM_SRC="app_main.c"
```

Run the script to build your program:
```bash
./script.sh
```

### **Integrating into Larger Systems**
- Use the hot-reloading API to manage shared modules in systems requiring frequent updates (e.g., plugins, game engines, or modular applications).
- Monitor changes to specific libraries using `check_and_reload_library` in a separate thread or main event loop.

---

## 💪 **Testing the Framework**

1. Modify one of the shared libraries (e.g., `library1.c`).
2. Recompile only the library:
   ```bash
   gcc -shared -o output/library1.so library1.c
   ```
3. Observe the main program detecting the change and reloading the updated library.

---

## 🛡️ **Best Practices**

1. **Ensure Thread-Safety**: If using multiple threads, synchronize access to shared libraries.
2. **Validate Library Paths**: Use absolute paths to avoid runtime errors.
3. **Handle Errors Gracefully**: Check return values from all API functions to detect failures early.

