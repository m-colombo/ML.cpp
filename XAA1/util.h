#ifndef __ML_UTIL__
#define __ML_UTIL__
#include <string> 

double rand_in_range(double a, double b);

std::string createAndGetRunFolder(std::string const & parent);
std::string createSubFolder(std::string const & parent, std::string const & sub);
#endif
