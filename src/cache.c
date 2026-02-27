#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>

//gcc -o test3 src/cache.c -lShlwapi
getenv("APPDATA");
int Running = 0

LPSTR openConversation(char* cachePath, char* conversationId){
    LPSTR dir = (LPSTR)strcat(cachePath, strcat("\\", conversationId));
    int DirectoryCreationStatus = CreateDirectory(dir, NULL);
    if(DirectoryCreationStatus || GetLastError() == ERROR_ALREADY_EXISTS){
        return dir;
    }
    return NULL;
}

int openCacheFile(char* dirPath){
    SYSTEMTIME sTime;
    GetLocalTime(&sTime);
    char dateStr[15];
    GetDateFormatA(0, 0, &sTime, "dd'-'MM'-'yyyy'.bin", dateStr, sizeof(dateStr));
    puts(dateStr);

    char path[MAX_PATH];
    strcpy_s(path, sizeof(path), dirPath);
    
    PathAppendA(path, dateStr);
    puts(path);
    HANDLE FileHandle = CreateFile((LPSTR)path, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(FileHandle == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        fprintf(stderr, "CreateFile failed: %lu\n", err);
        return 1;
    }

    while(Running){
        puts("Writing and Reading frfr");
    }

    EncryptFileA((LPSTR)path);
    CloseHandle(FileHandle);
    
    return 0;
}

int RemoveCacheBefore(char* Date){
    return 0;
}

int main(){
    openCacheFile("E:\\ChatApp\\TempCache");
    return 0;
}