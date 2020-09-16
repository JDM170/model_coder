#include "util.h"

#define MIN(x,y) (((x)<(y)) ? (x) : (y))

std::mutex mu;

void mLockedCout(const std::string str) {
    mu.lock();
        std::cout << str;
    mu.unlock();
}

std::string getFileName(const std::string& s, std::string* pathwithoutname) {
    char separator = '/';
#ifdef _WIN32
    separator = '\\';
#endif
    size_t i = s.rfind(separator, s.length());
    if(i != std::string::npos) {
        pathwithoutname->clear();
        pathwithoutname->append(s.substr(0, i + 1));
        return s.substr(i + 1, s.length() - i);
    }
    return s;
}

void keyConversion(const std::string key, unsigned int* k) {
    std::string encoded_key = md5(key).substr(0, 16); // MD5 encoding
    unsigned int kbuffer[4];
    memset(kbuffer, 0, sizeof(kbuffer));
    memcpy(kbuffer, encoded_key.c_str(), MIN(encoded_key.length(), 16));
    for(int i = 0; i < 4; i++)
        k[i] = kbuffer[i];
}
