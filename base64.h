#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
using namespace std;
typedef unsigned char BYTE;

string base64encode(BYTE const* buf, unsigned int bufLen);
vector<BYTE> base64decode(string const&);

#endif
