#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <thread>
#include <conio.h>
#include "base64/base64.h"
#include "tea/tea.h"
#include "util.h"

using namespace std;

bool decodeFile(const string fpath, const string key) {
    // Key conversion
    unsigned int k[4];
    keyConversion(key, k);
    mLockedCout("[OUTPUT] Key converted\n");

    // Reading file
    ifstream ifile(fpath, ios::in | ios::binary);
    struct stat results;
    if(stat(fpath.c_str(), &results) != 0) {
        mLockedCout("[ERROR] File '" + fpath + "' not found\n");
        return false;
    }
    string fbuffer64;
    ifile >> fbuffer64;
    ifile.close();
    mLockedCout("[OUTPUT] File '" + fpath + "' has been read\n");

    // Base64 decoding
    vector<unsigned char> fbuffer = base64decode(fbuffer64);
    size_t fbuffer_size = fbuffer.size();
    mLockedCout("[OUTPUT] File '" + fpath + "' has been Base64-decoded\n");

    // TEA decoding
    //unsigned char* vbuffer = new unsigned char[fbuffer_size];
    //memset(vbuffer, 0, fbuffer_size);
    unsigned int v[2];
    memset(v, 0, sizeof(v));
    memcpy(&v[1], &fbuffer[fbuffer_size], 4);
    for(unsigned int i = fbuffer_size; i >= 0; i--) {
        v[0] = *(unsigned int*)&fbuffer[i];
        decode(&v[0], &k[0]);
        memcpy(&fbuffer[i], &v[0], 4);
    }
    memcpy(&fbuffer[fbuffer_size - 4], &v[1], 4);
    mLockedCout("[OUTPUT] File '" + fpath + "' has been TEA-decoded\n");

    // Filename generating
    string filefolder;
    string old_filename = getFileName(fpath, &filefolder);
    string fullpath = filefolder + old_filename + ".dec";

    // Writing file
    /*ofstream ofile(fullpath.c_str(), ios::out | ios::binary);
    if(ofile.good())
        ofile.clear();
    ofile.write((char*)&vbuffer, fbuffer_size);
    ofile.close();*/
    mLockedCout("[OUTPUT] File '" + fullpath + "' has been written\n");

    //delete[] vbuffer;

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
            thread* t = new thread(decodeFile, tfpath, key);
            threads[i] = t;
        }
        for(int i = 0; i < argc - 1; i++) {
            threads[i]->join();
        }
    } else {
        threads = new thread*[1];
        thread* t = new thread(decodeFile, fpath, key);
        threads[0] = t;
        t->join();
    }

    for(size_t i = 0; i < argc - 1; i++) {
        delete threads[i];
    }
    delete[] threads;

    cout << "[OUTPUT] Selected files are decrypted, press any key to close program." << endl;
    //getch();

    return 0;
}
