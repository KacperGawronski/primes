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


#include "common_defines.h"
#include <Python.h>
#include "is_prime.h"
#include <pthread.h>
#define NUMBER_OF_THREADS 16

static int number_of_threads;



void* prime_worker_thread(void *);


static value_type prime_checked;
static int prime_check;
static value_type prime_sqrt_max;


PyObject* is_prime_cfunc (PyObject *dummy, PyObject *args){
	value_type i;
	int thread_args_data[NUMBER_OF_THREADS];
	if (!PyArg_ParseTuple(args, "K",&prime_checked)) return NULL;
	if((!(prime_checked%2)&&prime_checked!=2)||prime_checked<2)Py_RETURN_FALSE;
	
	prime_sqrt_max=sqrt(prime_checked);
	
	pthread_t threads[NUMBER_OF_THREADS];
	PyEval_InitThreads();
	prime_check=1;
	
	number_of_threads=NUMBER_OF_THREADS;

	for(i=0;i<number_of_threads;++i){
		thread_args_data[i]=i;
		pthread_create(&threads[i], NULL, prime_worker_thread, &thread_args_data[i]);
	}
	for(i=0;i<number_of_threads;++i){
		pthread_join(threads[i],NULL);
	}
	


	if(prime_check>0){Py_RETURN_TRUE;
		}else{ Py_RETURN_FALSE;}
}

void *prime_worker_thread(void *_id){
	int id=*(int*)_id;
	value_type i=3+id*2;
	while(prime_check&&i<=prime_sqrt_max){
		if(!(prime_checked%i)){
			prime_check=0;
			return NULL;
		}
		i+=number_of_threads*2;
	}
	return NULL;
}



