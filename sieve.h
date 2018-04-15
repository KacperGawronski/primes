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


#ifndef SIEVE_H
#define SIEVE_H
#include <Python.h>

PyObject* sieve_cfunc (PyObject *dummy, PyObject *args);

#endif
