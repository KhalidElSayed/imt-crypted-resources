/**
 *  Copyright (c) 2013, Inmite s.r.o. (www.inmite.eu).
 *
 * All rights reserved. This source code can be used only for purposes specified
 * by the given license contract signed by the rightful deputy of Inmite s.r.o.
 * This source code can be used only by the owner of the license.
 *
 * Any disputes arising in respect of this agreement (license) shall be brought
 * before the Municipal Court of Prague.
 *
 */

#include "CryptedDataUtil.h"
#include <cstdio>
#include <cstdlib>

#define hex(c) ((c >= '0' && c <= '9')?c - '0':((c >= 'a' && c <= 'f')?10 + c - 'a':((c >= 'A' && c <= 'F')?10 + c - 'A':-1)))

char *hex2bytes(char *hex, size_t len) {
    if (len % 2 != 0) {
        return NULL;
    }
    char *result = (char*)malloc(len / 2);
    for (int i = 0; i < len / 2; i++) {
        char upper = hex(hex[i * 2]);
        char lower = hex(hex[i * 2 + 1]);
        result[i] = upper * 16 + lower;
    }
    return result;
}

char* CryptedDataUtil::dataFromCryptedData(char* encryptedData, size_t dataLength, char *symKey, size_t symKeyLength, size_t * outputLength) {
    char *originalBytes = (char*) malloc(dataLength * sizeof(char));
    char *binarySymKey = hex2bytes(symKey, symKeyLength);
    
    for (size_t i = 0; i < dataLength; i++) {
        originalBytes[i] = encryptedData[i] ^ binarySymKey[i % (symKeyLength / 2)];
    }
    
    free(binarySymKey);
    
    // return the length of the resulting data
    *outputLength = dataLength;
    
    return originalBytes;
}

char* CryptedDataUtil::cryptedDataFromData(char* originalData, size_t dataLength, char *symKey, size_t symKeyLength, size_t * outputLength) {
    char *originalBytes = (char*) malloc(dataLength * sizeof(char));
    char *binarySymKey = hex2bytes(symKey, symKeyLength);
    
    for (size_t i = 0; i < dataLength; i++) {
        originalBytes[i] = originalData[i] ^ binarySymKey[i % (symKeyLength / 2)];
    }
    
    free(binarySymKey);
    
    // return the length of the resulting data
    *outputLength = dataLength;
    
    return originalBytes;
}

char* CryptedDataUtil::dataFromCryptedFile(const char* fileName, char *symKey, size_t symKeyLength, size_t * outputLength) {
    FILE* pFile;
    long lSize;
    char* encryptedData;
    size_t result;
    
    pFile = fopen(fileName, "rb" );
    if (pFile == NULL) {
        return NULL;
    }
    
    // obtain file size:
    fseek(pFile , 0 , SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    
    // allocate memory to contain the whole file:
    encryptedData = (char*) malloc(sizeof(char) * lSize);
    if (encryptedData == NULL) {
        return NULL;
    }
    
    // copy the file into the buffer:
    result = fread(encryptedData, 1, lSize, pFile);
    if (result != lSize) {
        return NULL;
    }
    
    fclose (pFile);
    
    char *originalBytes = dataFromCryptedData(encryptedData, lSize, symKey, symKeyLength, outputLength);
    
    free (encryptedData);
    
    return originalBytes;
    
}