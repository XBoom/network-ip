#ifndef BASIC_H
#define BASIC_H

//内存置为0
#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define MALLOC(X) calloc(1,X)
#define FREE(X) free(x)
#endif