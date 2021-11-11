#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include "lifehash.h"


static PyObject* write_jpg_for_sha256_hash(PyObject* self, PyObject* args)
{
    const char* file_name;
    const unsigned char* hash_data;
    Py_ssize_t hash_size;
    unsigned char jpg_quality;

    if (!PyArg_ParseTuple(args, "sy#b", &file_name, &hash_data, &hash_size, &jpg_quality))
        return NULL;

    if (hash_size != 32) {
        PyErr_SetObject(PyExc_ValueError, PyUnicode_FromString("SHA256 hash must be 32 bytes long."));
        return NULL;
    }

    if (jpg_quality < 10 || jpg_quality > 100) {
        PyErr_SetObject(PyExc_ValueError, PyUnicode_FromString("Quality must be between 10 and 100."));
        return NULL;
    }

    LifeHashImage* lifehash_image = lifehash_make_from_data(hash_data, hash_size, lifehash_version2, 128, false);
    if (lifehash_image == NULL) {
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Lifehash function returned no image data."));
        return NULL;
    }

    int stb_result;
    const int channel_count = 3;
    const int final_width = 256;
    const int final_height = 256;
    uint8_t* resized_image_data = (uint8_t*)malloc(final_width * final_height * channel_count);
    stb_result = stbir_resize_uint8(lifehash_image->colors, (int)lifehash_image->width, (int)lifehash_image->height, (int)lifehash_image->width * channel_count,
        resized_image_data, final_width, final_height, final_width * channel_count, channel_count);
    lifehash_image_free(lifehash_image);
    if (stb_result == 0) {
        free(resized_image_data);
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Image resize failed."));
        return NULL;
    }

    stb_result = stbi_write_jpg(file_name, final_width, final_height, channel_count, resized_image_data, 30);
    free(resized_image_data);
    if (stb_result == 0) {
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Image write failed."));
        return NULL;
    }

    return PyLong_FromLong(stb_result);
}

static PyObject* write_png_for_sha256_hash(PyObject* self, PyObject* args)
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

    LifeHashImage* lifehash_image = lifehash_make_from_data(hash_data, hash_size, lifehash_version2, 128, false);
    if (lifehash_image == NULL) {
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Lifehash function returned no image data."));
        return NULL;
    }

    int stb_result;
    const int channel_count = 3;
    const int final_width = 256;
    const int final_height = 256;
    uint8_t* resized_image_data = (uint8_t*)malloc(final_width * final_height * channel_count);
    stb_result = stbir_resize_uint8(lifehash_image->colors, (int)lifehash_image->width, (int)lifehash_image->height, (int)lifehash_image->width * channel_count,
        resized_image_data, final_width, final_height, final_width * channel_count, channel_count);
    lifehash_image_free(lifehash_image);
    if (stb_result == 0) {
        free(resized_image_data);
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Image resize failed."));
        return NULL;
    }

    stb_result = stbi_write_png(file_name, final_width, final_height, channel_count, resized_image_data, (int)final_width*channel_count);
    free(resized_image_data);
    if (stb_result == 0) {
        PyErr_SetObject(PyExc_Exception, PyUnicode_FromString("Image write failed."));
        return NULL;
    }
    
    return PyLong_FromLong(stb_result);
}

static const char* lifehash_doc = "This is a version of the bc-lifehash code wrapped as a Python module.";

static PyMethodDef LifeHashMethods[] = {
    {"write_jpg_for_sha256_hash",  write_jpg_for_sha256_hash, METH_VARARGS, "Generate a lifehash image for a SHA256 hash and write it to the specified file as a JPG image."},
    {"write_png_for_sha256_hash",  write_png_for_sha256_hash, METH_VARARGS, "Generate a lifehash image for a SHA256 hash and write it to the specified file as a PNG image."},
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

