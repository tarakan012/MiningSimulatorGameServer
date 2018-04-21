#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <stdio.h>
#include <string>
//#include <sys\stat.h>

namespace MS { 
    namespace Server {

#ifdef __cplusplus
extern "C" {
#endif

    std::string GetDataFromFile(const std::string filename)
    {
        std::string buffer;
        FILE * fp = fopen(filename.c_str(), "rb");
        if  (!fp)
        {
            return NULL;
        }
        auto descriptor = fileno(fp);
        struct stat statBuf;
        if (fstat(descriptor, &statBuf) == -1)
        {
            fclose(fp);
            return NULL;
        }
        size_t size = statBuf.st_size;
        buffer.resize(size);
        size_t readSize = fread((void*)buffer.c_str(),1,size,fp);
        if (readSize < size)
        {
            buffer.resize(readSize);
            return NULL;
        }
        return buffer;
    }


#ifdef __cplusplus
}
#endif

    }
}

#endif /* FILEUTILS_H */

