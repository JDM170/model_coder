#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "base64.h"
#include "tea.h"

#define MIN(x,y) (((x)<(y)) ? (x) : (y))

using namespace std;

string getFileName(const string& s, string* pathwithoutname) {
    char sep = '/';
#ifdef _WIN32
    sep = '\\';
#endif
    size_t i = s.rfind(sep, s.length());
    if (i != string::npos) {
        pathwithoutname->clear();
        pathwithoutname->append(s.substr(0, i + 1));
        return s.substr(i + 1, s.length() - i);
    }
    return s;
}

int main(const int argc, const char* argv[])
{
    const string fpath = "test.txd";
    const string key = "JdzFR2XLDaBtpGGD";

    // Key conversion
    unsigned int k[4];
    unsigned int kbuffer[4];
    memset(k, 0, sizeof(k));
    memset(kbuffer, 0, sizeof(kbuffer));
    memcpy(kbuffer, key.c_str(), MIN(key.length(), 16));
    for (int i = 0; i < 4; i++)
        k[i] = kbuffer[i];
    cout << "[OUTPUT] Key converted" << endl;

    // Reading file
    ifstream file(fpath, ios::in | ios::binary);
    struct stat results;
    stat(fpath.c_str(), &results);
    size_t file_size = results.st_size;
    char* fbuffer = new char[file_size];
    file.read(fbuffer, file_size);
    file.close();
    cout << "[OUTPUT] File has been read" << endl;

    // Creating buffer
    size_t vbuffer_size = file_size;
    if (vbuffer_size % 4 > 0)
        vbuffer_size += 4 - (vbuffer_size % 4);
    unsigned char* vbuffer = new unsigned char [vbuffer_size];
    memset(vbuffer, 0, vbuffer_size);
    memcpy(vbuffer, fbuffer, file_size);

    // Crypting file
    size_t obuffer_size = vbuffer_size + 4;
    char* obuffer = new char[obuffer_size];
    unsigned int v[2];
    memset(v, 0, sizeof(v));
    for(unsigned int i = 0; i < file_size; i += 4) {
        v[0] = *(unsigned int*)&vbuffer[i];
        encode(&v[0], &k[0]);
        memcpy(&obuffer[i], &v[0], 4);
        memcpy(vbuffer, fbuffer, file_size);
    }
    memcpy(&obuffer[obuffer_size - 4], &v[1], 4);
    delete[] fbuffer;
    delete[] vbuffer;

    // Base64 encoding
    string ob64 = base64encode((unsigned char*)obuffer, obuffer_size);
    delete[] obuffer;
    cout << "[OUTPUT] File has been crypted" << endl;

    // File name generating
    string filefolder;
    string old_filename = getFileName(fpath, &filefolder);
    string fullpath = filefolder + old_filename + ".enc";

    // Writing file
    ofstream ofile(fullpath.c_str(), ios::out | ios::binary);
    if (ofile.good()) {
        ofile.clear();
    }
    ofile << ob64;
    ofile.close();
    cout << "[OUTPUT] File has been written, exiting..." << endl;

    return 0;
}
