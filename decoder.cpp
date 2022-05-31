#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <thread>
#include <conio.h>
#include "base64/base64.h"
#include "tea/tea.h"
#include "util.h"

using namespace std;

bool decodeFile(const string fpath, const unsigned int* key) {
    // Reading file
    struct stat results;
    if(stat(fpath.c_str(), &results) != 0) {
        mLockedCout("[ERROR] File '" + fpath + "' not found\n");
        return false;
    }
    ifstream ifile(fpath, ios::in | ios::binary);
    string fbuffer64;
    ifile >> fbuffer64;
    ifile.close();
    //mLockedCout("[OUTPUT] File '" + fpath + "' has been read\n"); // Debug

    // Base64 decoding
    vector<unsigned char> fbuffer = base64decode(fbuffer64);
    size_t fbuffer_size = fbuffer.size();
    //mLockedCout("[OUTPUT] File '" + fpath + "' has been Base64-decoded\n"); // Debug

    // TEA decoding
    unsigned char* vbuffer = new unsigned char[fbuffer_size];
    memset(vbuffer, 0, fbuffer_size);
    unsigned int value[2];
    memset(value, 0, sizeof(value));
    memcpy(&value[1], &fbuffer[fbuffer_size - 4], 4);
    for(int i = fbuffer_size - 8; i >= 0; i -= 4) {
        value[0] = *(unsigned int*)&fbuffer[i];
        decode(&value[0], &key[0]);
        memcpy(&vbuffer[i], &value[0], 4);
    }
    //mLockedCout("[OUTPUT] File '" + fpath + "' has been TEA-decoded\n"); // Debug

    // Filename generating
    string filefolder;
    string old_filename = getFileName(fpath, &filefolder);
    string fullpath = filefolder + old_filename + ".dec";

    // Writing file
    ofstream ofile(fullpath.c_str(), ios::out | ios::binary);
    if(ofile.good())
        ofile.clear();
    for(unsigned int i = 0; i < fbuffer_size; i++)
        ofile << vbuffer[i];
    ofile.close();
    //mLockedCout("[OUTPUT] File '" + fullpath + "' has been written\n"); // Debug
    mLockedCout("[OUTPUT] File '" + fullpath + "' has been decoded\n");

    delete[] vbuffer;

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
    string skey;
    cout << "[INPUT] Enter key: ";
    cin >> skey;
    cout << endl;

    // Key conversion
    unsigned int bkey[4];
    keyConversion(skey, bkey);
    //cout << "[OUTPUT] Key converted" << endl; // Debug

    thread** threads;
    if(argc > 1) {
        threads = new thread*[argc - 1];
        for(int i = 0; i < argc - 1; i++) {
            string tfpath(argv[i + 1]);
            thread* t = new thread(decodeFile, tfpath, bkey);
            threads[i] = t;
        }
        for(int i = 0; i < argc - 1; i++) {
            threads[i]->join();
        }
    } else {
        threads = new thread*[1];
        thread* t = new thread(decodeFile, fpath, bkey);
        threads[0] = t;
        t->join();
    }

    for(int i = 0; i < argc - 1; i++) {
        delete threads[i];
    }
    delete[] threads;

    cout << "[OUTPUT] Selected files are decrypted, press any key to close program." << endl;
    getch();

    return 0;
}
