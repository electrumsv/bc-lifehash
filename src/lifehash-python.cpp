#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include "lifehash.h"


static PyObject* lifehash_1(PyObject* self, PyObject* args)
{
    const char* file_name;
    const unsigned char* hash_data;
    Py_ssize_t hash_size;

    if (!PyArg_ParseTuple(args, "sy#", &file_name, &hash_data, &hash_size))
        return NULL;

    if (hash_size != 32) {
        PyErr_SetObject(PyExc_ValueError, PyUnicode_FromString("SHA256 hash must be 32 bytes long."));
        return NULL;
    }

    LifeHashImage* lifehash_image = lifehash_make_from_data(hash_data, hash_size, lifehash_version2, 64, false);
    if (lifehash_image == NULL) {
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Lifehash function returned no image data."));
        return NULL;
    }

    int stb_result = stbi_write_png(file_name, (int)lifehash_image->width, (int)lifehash_image->height, 3, lifehash_image->colors, (int)lifehash_image->width*3);
    lifehash_image_free(lifehash_image);
    if (stb_result == 0) {
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Lifehash function returned no image data."));
        return NULL;
    }
    
    return PyLong_FromLong(stb_result);
}

static const char* lifehash_doc = "This is a version of the bc-lifehash code wrapped as a Python module.";

static PyMethodDef LifeHashMethods[] = {
    {"method1",  lifehash_1, METH_VARARGS, "The first method."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef lifehash_module = {
    PyModuleDef_HEAD_INIT,
    "lifehash",   /* name of module */
    lifehash_doc, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    LifeHashMethods
};

PyMODINIT_FUNC
PyInit_lifehash(void)
{
    return PyModule_Create(&lifehash_module);
}

