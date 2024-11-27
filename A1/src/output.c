/*
 *  output.c - reads Windows pipe and converts string with such rule:
 *          Everything converts to snake_case
 */

#include <windows.h>

#include <stdio.h>


#define TAG "\033[33m[OUTPUT]\033[0m "

int main() {
    STARTUPINFO si;
    GetStartupInfo(&si);
     
    char buffer[256] = { 0 };

    HANDLE hOutFile = CreateFile(
            "output.txt",
            GENERIC_WRITE,
            FILE_SHARE_WRITE | FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );

    BOOL bResult = TRUE;
    DWORD dwRead = 1;
    perror(TAG"Start read");
    while (ReadFile(si.hStdInput, buffer, 255, &dwRead, NULL) && dwRead > 0) {
        buffer[dwRead] = '\0';
        bResult = WriteFile(hOutFile, buffer, strlen(buffer), NULL, NULL);
        if (!bResult) {
            perror(TAG"Failed to output txt to file");
        }
    }
    perror(TAG"End read");
    CloseHandle(hOutFile);
    return 0;
}
