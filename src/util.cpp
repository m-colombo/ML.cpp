
#include "util.h"
#include <ctime>
#include <sstream>

using namespace std;

double rand_in_range(double a, double b){
	return (b - a) * ((double) rand() / (double)RAND_MAX) + a ;
}

string createAndGetRunFolder(string const & parent){
    time_t now; time(&now);
	struct tm * t=0;
	localtime_s(t, &now);
    char t_str[20];
    snprintf(t_str,20, "%d-%.2d-%.2d.%.2d-%.2d-%.2d", (t->tm_year+1900), t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    
    return createSubFolder(parent, t_str);
}

string createSubFolder(string const & parent, string const & sub){
    stringstream cmd;
    string dir = parent + "/" + sub;

//  Needed on Windows
//	for (auto& c : dir) {
//		if (c == '/')
//			c = '\\';
//	}
    
    cmd << "mkdir \"" << dir << "\"";
    system(cmd.str().c_str());
    return dir;
}