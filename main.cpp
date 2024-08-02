#include <iostream>
#include <Python.h>


// Function to print sys.path
void print_sys_path() {
    PyRun_SimpleString(
        "import sys\n"
        "print('Current sys.path:')\n"
        "for path in sys.path:\n"
        "    print(path)\n"
    );
}

// Function to call the Python function 'load' in the given module
PyObject* call_load_function(const std::string& module_name, const int argument) {
    // Initialize the Python interpreter
    Py_Initialize();

    // Add the directory containing the module to the Python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../extensions/')");  // replace with actual path

    // Print sys.path to verify
    print_sys_path();

    // Import the module
    PyObject* pName = PyUnicode_DecodeFSDefault(module_name.c_str());
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Failed to load the Python module");
    }

    // Get the 'load' function from the module
    PyObject* pFunc = PyObject_GetAttrString(pModule, "load");
    if (!pFunc || !PyCallable_Check(pFunc)) {
        if (PyErr_Occurred())
            PyErr_Print();
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
        throw std::runtime_error("Cannot find function 'load'");
    }

    // Call the 'load' function
    PyObject* pArgs = PyTuple_Pack(1, PyLong_FromLong(argument));
    PyObject* pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    if (pValue == nullptr) {
        PyErr_Print();
        throw std::runtime_error("Call to 'load' failed");
    }

    return pValue; // Return the result
}

void process_module_data(PyObject* module_data) {
    PyObject* pTook = PyObject_GetAttrString(module_data, "took");
    PyObject* pResult = PyObject_GetAttrString(module_data, "result");

    if (pTook && PyLong_Check(pTook) && pResult && PyLong_Check(pResult)) {
        long took = PyLong_AsLong(pTook);
        long result = PyLong_AsLong(pResult);
        std::cout << "Result from Python: took = " << took << " microseconds, result = " << result << std::endl;
    } else {
        std::cerr << "Invalid data in ModuleData" << std::endl;
    }

    Py_XDECREF(pTook);
    Py_XDECREF(pResult);
}

int main()  {
    try {
        PyObject *result = call_load_function("extension", 20);
        process_module_data(result);
    }
    catch(const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    }
    Py_Finalize();
    return 0;
}
