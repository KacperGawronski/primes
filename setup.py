from distutils.core import setup, Extension

primes1 = Extension('primes',
                    sources = ['primes.c','sieve.c','is_prime.c'])

setup (name = 'primes',
       version = '1.0',
       description = 'Prime numbers uitls for Python written in C, require NumPy, Python3+ and POSIX threads support.',
       ext_modules = [primes1])
