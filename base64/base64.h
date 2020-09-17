#ifndef _BASE64_H_
#define _BASE64_H_

#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string base64encode(BYTE const*, unsigned int);
std::vector<BYTE> base64decode(std::string const&);

#endif
