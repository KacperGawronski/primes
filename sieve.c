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
#include "sieve.h"
#include <Python.h>
#include <numpy/arrayobject.h>
#include <pthread.h>

static int number_of_threads;
value_type current_max=0;

char sieve_check;
char * sieve_data=NULL;


void *sieve_worker_thread(void *);

value_type sqrt_max;

PyObject* sieve_cfunc (PyObject *dummy, PyObject *args){
	import_array();
	value_type max_number;
	value_type i;
	int thread_args_data[NUMBER_OF_THREADS];
	if (!PyArg_ParseTuple(args, "K",&max_number)) return NULL;
	if(current_max==0||max_number>current_max){
		current_max=max_number;
		if(sieve_check){
			PyMem_RawFree(sieve_data);
			sieve_check=0;
		}
		sieve_data=PyMem_RawMalloc(max_number+1);
		if(sieve_data!=NULL){
			memset(sieve_data,1,max_number+1);
			sieve_check=1;
		}else{
			return NULL;
		}
		if(max_number>=0)sieve_data[0]=0;
		if(max_number>=1)sieve_data[1]=0;
		sqrt_max=sqrt(max_number);
		for(i=4;i<=max_number;i+=2){
			sieve_data[i]=0;
		}
		
		
		PyEval_InitThreads();
		number_of_threads=NUMBER_OF_THREADS<max_number+1?NUMBER_OF_THREADS:max_number+1;
		
		pthread_t threads[NUMBER_OF_THREADS];
		
		for(i=0;i<number_of_threads;++i){
			thread_args_data[i]=i;
			pthread_create(&threads[i], NULL, sieve_worker_thread, &thread_args_data[i]);
		}
		for(i=0;i<number_of_threads;++i){
			pthread_join(threads[i],NULL);
			
		}
	}
	
    npy_intp dims[1] = {max_number+1};
	PyObject * sieve = PyArray_SimpleNewFromData(1, (npy_intp*) dims, NPY_BOOL, sieve_data);
	
	return sieve;
}

void *sieve_worker_thread(void* _id){
	int id=*(int*)_id;
	value_type i,cur_value;
	for(i=3+id*2;i<=sqrt_max;i+=number_of_threads*2){
		if(sieve_data[i])
			for(cur_value=i*3;cur_value<=current_max;cur_value+=2*i){	
				sieve_data[cur_value]=0;
			}
	}
	return NULL;
}

