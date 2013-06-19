/* LiteSQL - Split implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
//#include "compatibility.hpp"
#include "litesql/split.hpp"
#include <string>
#include <string.h>
#include <cstring>
#include <cstdlib>

using namespace litesql;    
using namespace std;

Split::Split(const string& s, const string& delim) {
  char * buf = strdup((char*) s.c_str());
    char * ptr = buf;
    int len = delim.size();
    vector<char*> pointers;
    pointers.push_back(ptr);
    while((ptr = strstr(ptr, delim.c_str()))) {
        *ptr = '\0';
        ptr += len;
        pointers.push_back(ptr);
    }
    for (vector<char*>::iterator i = pointers.begin();
         i != pointers.end();
         ++i)
        push_back(string(*i));

    free(buf);
}
Split Split::slice(int start, int end) const {
    Split data;
    if (start < 0)
        start = start+size();
    if (end < 0)
        end = end+size();
    if (start >= static_cast<int>(size()))
        start = size()-1;
    if (end >= static_cast<int>(size()))
        end = size()-1;
    if (start >= end)
        return data;
    for (int i = start; i < end; i++)
        data.push_back(this->operator[](i));
    return data;
}

string Split::join(const vector<string>& strings,const string& delim){
    string res;
    for (const_iterator it = strings.begin(); it != strings.end(); it++) {
        if (it != strings.begin())
          res.append(delim);
        res.append(*it);
    }
    return res;
}

string Split::join(const string& delim) const {
  return join(*this,delim);  
}

Split & Split::extend(const vector<string> & s) {
  reserve(size()+s.size());
  for (vector<string>::const_iterator it = s.begin();it != s.end(); it++)
        push_back(*it);
    return *this;
}

