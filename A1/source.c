#include <windows.h>

#include <stdio.h>
#include <string.h>

int main() {
    STARTUPINFO si;
    GetStartupInfo(&si);

    char buffer[256];

    scanf("%s", buffer);

    DWORD dwWritten;
    DWORD dwHelloLen = strlen(buffer);
    BOOL bResult;
    bResult = WriteFile(si.hStdOutput, buffer, dwHelloLen, &dwWritten, NULL);
    if (!bResult) {
        perror("Could not write to hStdOutput");
        exit(1);
    }
    if (dwHelloLen != dwWritten) {
        fprintf(stderr, "Error while write %d bytes; Expected %d\n", dwWritten, dwHelloLen);
        exit(1);
    }
    return 0;
}
