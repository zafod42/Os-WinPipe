#include <windows.h>

#include <stdio.h>
#include <string.h>

#define TAG "\033[32m[SOURCE]\033[0m "
#define MAX_BUFFER 256 

int main() {
    STARTUPINFO si;
    GetStartupInfo(&si);

    const char* END_WRITE = TAG"End write";
    char buffer[MAX_BUFFER];
    char log_format[MAX_BUFFER] = " Read %d bytes; Written %d bytes";
    char log[MAX_BUFFER];
    char out_log[MAX_BUFFER];

    BOOL bResult;
    DWORD dwRead;
    HANDLE hInFile = CreateFile(
            "file.txt",
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
            );
    while (ReadFile(hInFile, buffer, MAX_BUFFER - 1, &dwRead, NULL) && dwRead > 0) {
        buffer[dwRead] = '\0';

        DWORD dwWritten;
        DWORD dwHelloLen = strlen(buffer);
        perror(TAG"Start write");
        bResult = WriteFile(si.hStdOutput, buffer, dwHelloLen, &dwWritten, NULL);
        if (!bResult) {
            perror(TAG"Could not write to hStdOutput");
            exit(1);
        }
        if (dwHelloLen != dwWritten) {
            fprintf(stderr, "Error while write %d bytes; Expected %d\n", dwWritten, dwHelloLen);
            exit(1);
        }
        sprintf(log, log_format, dwRead, dwWritten);
        strcpy(out_log, END_WRITE);
        strcat(out_log, log);
        perror(out_log);
        fflush(stderr);
    }
    CloseHandle(si.hStdOutput);
    CloseHandle(hInFile);
    return 0;
}
