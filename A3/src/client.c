#include <windows.h>

#include <stdio.h>

#define MAX_BUFFER 256 
#define TAG "\033[35m[CLIENT]\033[0m"

int main(int argc, char* argv[]) {
    perror(TAG" start");
    HANDLE hPipe;
    DWORD dwMode;
    DWORD dwWritten, dwRead;
    BOOL fSuccess = FALSE;
    char buffer[MAX_BUFFER];
    LPCSTR lpName = "\\\\.\\pipe\\LOCAL\\pipename";
    while (1) {
        hPipe = CreateFile(
                lpName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL);
        if (hPipe != INVALID_HANDLE_VALUE) {
            perror(TAG" pipe handle is valid");
            break;
        }

        if (!WaitNamedPipe(lpName, 2000)) {
            perror(TAG"Could not open pipe");
            ExitProcess(1);
        }
    }
    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
            hPipe,
            &dwMode,
            NULL,
            NULL);
    if (!fSuccess) {
        perror(TAG"Failed to set named pipe handle state");
        ExitProcess(1);
    }

    printf("Enter filename: ");
    scanf("%s", buffer);

    fSuccess = WriteFile(
            hPipe,
            buffer,
            strlen(buffer),
            &dwWritten,
            NULL);

    if (!fSuccess) {
        perror(TAG"Could not write to pipe");
        ExitProcess(1);
    }

    DWORD dwError = ERROR_MORE_DATA;
    do {
        fSuccess = ReadFile(
                hPipe,
                buffer,
                MAX_BUFFER -1,
                &dwRead,
                NULL);
        dwError = GetLastError();
        if (!fSuccess && dwError != ERROR_MORE_DATA) {
            perror(TAG"End reading");
            break;
        }
        buffer[dwRead] = '\0';
        printf(buffer);
    } while (dwError == ERROR_MORE_DATA);

    CloseHandle(hPipe);
    perror(TAG" end");
    return 0;
}
