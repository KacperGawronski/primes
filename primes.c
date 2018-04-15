/*
Copyright 2018 Kacper Gawroński.
This file, and all files created by Kacper Gawroński in project
primes, are licensed under GNU General Public License.

primes is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
primes is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with primes.  If not, see <http://www.gnu.org/licenses/>.
*/


#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include <math.h>
#include <pthread.h>
#include "sieve.h"
#include "is_prime.h"

extern char *sieve_data;
extern PyObject* sieve_cfunc (PyObject *dummy, PyObject *args);
extern PyObject* is_prime_cfunc (PyObject *dummy, PyObject *args);
extern char sieve_check;

static PyMethodDef primesmethods[] = {
    {
	    "sieve",sieve_cfunc,
        METH_VARARGS,
        "Eratosthenes Sieve implementation.\nAs parameter it takes maximum number which should be checked.\n Returns array of NPY_BOOL that contains true/false for each number depending if it is prime or not.\n Starts with 0."//Doc string
    },
    {
	    "is_prime",is_prime_cfunc,
        METH_VARARGS,
        "Brute-force check if number is a prime number."//Doc string
    },
    {NULL, NULL, 0, NULL} /* Sentinel */
};



static struct PyModuleDef primes =
{
	PyModuleDef_HEAD_INIT,
	"primes", /* name of module */
	"Primes utils.\nCurrently contains sieve(n) and is_prime(x) functions.",          /* module documentation, may be NULL */
	-1,          /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
	primesmethods
};


static void free_data(void){
	if(sieve_check)PyMem_RawFree(sieve_data);
	sieve_check=0;
}

PyMODINIT_FUNC PyInit_primes(void)
{
	sieve_check=0;
	import_array();
	Py_AtExit(free_data);
    return PyModule_Create(&primes);
}

