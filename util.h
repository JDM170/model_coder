#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <mutex>
#include "md5/md5.h"

void mLockedCout(const std::string);
void keyConversion(const std::string, unsigned int*);
std::string getFileName(const std::string&, std::string*);

#endif // UTIL_H
