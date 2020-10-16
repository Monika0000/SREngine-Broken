#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <thread>	
#include <random>
#include <ostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <Psapi.h>

#include <fstream>
#include <vector>
#include <istream>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows

class SRFile {
public:
    static size_t FileSize(std::string filename)
    {
       return std::filesystem::file_size(filename);
    }

    static char* ReadAllBytes(const char* filename, int* read)
    {
        std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        // What happens if the OS supports really big files.
        // It may be larger than 32 bits?
        // This will silently truncate the value/
        int length = pos;

        // Manuall memory management.
        // Not a good idea use a container/.
        char* pChars = new char[length];
        ifs.seekg(0, std::ios::beg);
        ifs.read(pChars, length);

        // No need to manually close.
        // When the stream goes out of scope it will close the file
        // automatically. Unless you are checking the close for errors
        // let the destructor do it.
        ifs.close();
        *read = length;
        return pChars;
    }
	static std::vector<unsigned char> LoadBinaryFile(std::string filename) {
        size_t bytesAvailable = FileSize(filename.c_str());

        std::ifstream inf(filename.c_str());
        if (inf)
        {
            //unsigned char* mDataBuffer = new unsigned char[bytesAvailable];
            std::vector<unsigned char> mDataBuffer = std::vector<unsigned char>(bytesAvailable);
            inf.read((char*)(&mDataBuffer[0]), bytesAvailable);
            size_t counted = inf.gcount();
            return mDataBuffer;
        }

        return std::vector<unsigned char>();
	}

	inline static const std::string GetPathToExe() noexcept {
		TCHAR cwd[100];
		GetCurrentDirectory(100, cwd);
		std::wstring ws(&cwd[0]);
		return std::string(ws.begin(), ws.end());
	}
};

