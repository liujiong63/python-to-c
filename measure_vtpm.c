#include <python2.7/Python.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>


void main()
{
    // python initialize
    Py_Initialize();
    if (!Py_IsInitialized()){
        goto error;
    }

    char *path = "/usr/lib/python2.7/site-packages/testapp/";
    char *file = "testmodule";
    char *class = "testclass";

    PyRun_SimpleString("import sys");
    char prefix[100];
    strcpy(prefix, "sys.path.append('");
    strcat(prefix, path);
    strcat(prefix, "')");
    PyRun_SimpleString(prefix);

    const char *name = "abcdefg";
    const char *host = "12.34.56.78";
    int port = 5678;
    PyObject *pName, *pModule, *pDict, *pClass, *pArgs;

    pName = PyString_FromString(file);
    pModule = PyImport_Import(pName);
    if (!pModule){
        goto error;
    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict){
        goto error;
    }

    pClass = PyDict_GetItemString(pDict, class);
    if ( !pClass || !PyCallable_Check(pClass)){
        goto error;
    }

    // prepare args
    pArgs = Py_BuildValue("(ssi)", name, host, port);;

    // initialize class instance
    PyObject *res = PyObject_CallObject(pClass, pArgs);

    // call class method
    PyObject *ret = PyObject_CallMethod(res, "testclass_method", NULL);
    char *result = NULL;
    PyArg_Parse(ret, "s", &result);
    printf("result is: %s\n", result);
 
    // clean 
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pDict);
    Py_DECREF(pClass);
    Py_DECREF(pArgs);
    Py_Finalize();
error:
    return;
}
