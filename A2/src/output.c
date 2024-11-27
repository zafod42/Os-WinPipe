/*
 *  output.c - reads Windows pipe and converts string with such rule:
 *          Everything converts to snake_case
 */

#include <windows.h>

#include <stdio.h>
#include <string.h>

#define TAG "\033[33m[OUTPUT]\033[0m "


int main() {
    STARTUPINFO si;
    GetStartupInfo(&si);
     

    const char* LOG_FORMAT = "read %d";
    const char* TAGGER = TAG;
    char buffer[256];
    char log_out[256];
    char log[256];

    HANDLE hOutFile = CreateFile(
            "output.txt",
            GENERIC_WRITE,
            FILE_SHARE_WRITE | FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );

    BOOL bResult = FALSE;
    DWORD dwRead;
    perror(TAG"Start read");
    while (TRUE) {
        bResult = ReadFile(si.hStdInput, buffer, 255, &dwRead, NULL);
        if (!bResult || dwRead == 0) {
            break;
        }
        buffer[dwRead] = '\0';
        sprintf(log, LOG_FORMAT, dwRead);
        strcpy(log_out, TAGGER);
        strcat(log_out, log);
        perror(log_out);
        fflush(stderr);
        bResult = WriteFile(hOutFile, buffer, strlen(buffer), NULL, NULL);
        if (!bResult) {
            perror(TAG"Failed to output txt to file");
            break;
        }
    }
    perror(TAG"End read");
    CloseHandle(hOutFile);
    return 0;
}
