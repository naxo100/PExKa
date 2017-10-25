
#ifndef PARAMS_H
#define PARAMS_H

#define DEBUG true

typedef double FL_TYPE;

typedef unsigned long big_id;
typedef unsigned int mid_id;
typedef unsigned short short_id;
typedef unsigned char small_id;

typedef unsigned int pop_size;

#include <utility>
template<typename T> using two = std::pair<T,T>;

class False : public std::exception {};


#endif
