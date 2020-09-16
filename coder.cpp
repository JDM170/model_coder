#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <conio.h>
#include "base64/base64.h"
#include "tea/tea.h"
#include "util.h"

using namespace std;

bool encodeFile(const string fpath, const string key) {
    // Key conversion
    unsigned int k[4];
    keyConversion(key, k);
    mLockedCout("[OUTPUT] Key converted\n");

    // Reading file
    ifstream file(fpath, ios::in | ios::binary);
    struct stat results;
    if(stat(fpath.c_str(), &results) != 0) {
        mLockedCout("[ERROR] File '" + fpath + "' not found\n");
        return false;
    }
    size_t file_size = results.st_size;
    char* fbuffer = new char[file_size];
    file.read(fbuffer, file_size);
    file.close();
    mLockedCout("[OUTPUT] File '" + fpath + "' has been read\n");

    // Creating buffer
    size_t vbuffer_size = file_size;
    if(vbuffer_size % 4 > 0)
        vbuffer_size += 4 - (vbuffer_size % 4);
    unsigned char* vbuffer = new unsigned char [vbuffer_size];
    memset(vbuffer, 0, vbuffer_size);
    memcpy(vbuffer, fbuffer, file_size);

    // TEA encoding
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
    mLockedCout("[OUTPUT] File '" + fpath + "' has been TEA-coded\n");

    // Base64 encoding
    string ob64 = base64encode((unsigned char*)obuffer, obuffer_size);
    mLockedCout("[OUTPUT] File '" + fpath + "' has been Base64-coded\n");

    // Filename generating
    string filefolder;
    string old_filename = getFileName(fpath, &filefolder);
    string fullpath = filefolder + old_filename + ".enc";

    // Writing file
    ofstream ofile(fullpath.c_str(), ios::out | ios::binary);
    if(ofile.good())
        ofile.clear();
    ofile << ob64;
    ofile.close();
    mLockedCout("[OUTPUT] File '" + fullpath + "' has been written\n");

    delete[] fbuffer;
    delete[] vbuffer;
    delete[] obuffer;

    return true;
}

int main(const int argc, const char* argv[]) {
    // Checking argc and file path
    string fpath;
    if(argc > 1) {
        fpath.assign(argv[1]);
    } else {
        mLockedCout("[OUTPUT] You can open file(s) with this program\n[OUTPUT] Or drag'n'drop on it\n[INPUT] Enter filename (without spaces): ");
        cin >> fpath;
    }

    // Reading key
    string key;
    cout << "[INPUT] Enter key: ";
    cin >> key;
    cout << endl;

    thread** threads;
    if(argc > 1) {
        threads = new thread*[argc - 1];
        for(int i = 0; i < argc - 1; i++) {
            string tfpath(argv[i + 1]);
            thread* t = new thread(encodeFile, tfpath, key);
            threads[i] = t;
        }
        for(int i = 0; i < argc - 1; i++) {
            threads[i]->join();
        }
    } else {
        threads = new thread*[1];
        thread* t = new thread(encodeFile, fpath, key);
        threads[0] = t;
        t->join();
    }

    for(size_t i = 0; i < argc - 1; i++) {
        delete threads[i];
    }
    delete[] threads;

    cout << "[OUTPUT] Selected files are encrypted, press any key to close program." << endl;
    getch();

    return 0;
}
