#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <conio.h>
#include "base64/base64.h"
#include "md5/md5.h"
#include "tea/tea.h"

#define MIN(x,y) (((x)<(y)) ? (x) : (y))

using namespace std;
mutex mu;

string getFileName(const string& s, string* pathwithoutname) {
    char separator = '/';
#ifdef _WIN32
    separator = '\\';
#endif
    size_t i = s.rfind(separator, s.length());
    if(i != string::npos) {
        pathwithoutname->clear();
        pathwithoutname->append(s.substr(0, i + 1));
        return s.substr(i + 1, s.length() - i);
    }
    return s;
}

string encodeKey(const string key) {
    return md5(key).substr(0, 16);
}

bool encodeFile(const string fpath, const string key) {
    // Key conversion
    unsigned int k[4];
    unsigned int kbuffer[4];
    memset(k, 0, sizeof(k));
    memset(kbuffer, 0, sizeof(kbuffer));
    memcpy(kbuffer, key.c_str(), MIN(key.length(), 16));
    for(int i = 0; i < 4; i++)
        k[i] = kbuffer[i];
    mu.lock(); cout << "[OUTPUT] Key converted" << endl; mu.unlock();

    // Reading file
    ifstream file(fpath, ios::in | ios::binary);
    struct stat results;
    if(stat(fpath.c_str(), &results) != 0) {
        mu.lock(); cout << "[ERROR] File '" << fpath << "' not found" << endl; mu.unlock();
        return false;
    }
    size_t file_size = results.st_size;
    char* fbuffer = new char[file_size];
    file.read(fbuffer, file_size);
    file.close();
    mu.lock(); cout << "[OUTPUT] File '" << fpath << "' has been read" << endl; mu.unlock();

    // Creating buffer
    size_t vbuffer_size = file_size;
    if(vbuffer_size % 4 > 0)
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
    mu.lock(); cout << "[OUTPUT] File '" << fpath << "' has been crypted" << endl; mu.unlock();

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
    mu.lock(); cout << "[OUTPUT] File '" << fullpath << "' has been written" << endl; mu.unlock();

    return true;
}

int main(const int argc, const char* argv[]) {
    // Checking argc and file path
    string fpath;
    if(argc > 1) {
        fpath.assign(argv[1]);
    } else {
        mu.lock();
        cout << "[OUTPUT] You can open file(s) with this program" << endl
             << "[OUTPUT] Or drag'n'drop on it" << endl
             << "[INPUT] Enter filename (without spaces): ";
        mu.unlock();
        cin >> fpath;
    }

    // Reading key
    string key;
    cout << "[INPUT] Enter key: ";
    cin >> key;
    cout << endl;
    key = encodeKey(key);

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
