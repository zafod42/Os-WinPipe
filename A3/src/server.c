#include <windows.h>

#include <stdio.h>

#define MAX_BUFFER 512
#define TAG "\033[34m[SERVER]\033[0m"
#define ERROR_START "\033[31m"
#define ERROR_END "\033[0m"

int main(int argc, char* argv[]) {

    /* Create Named Pipe */
    HANDLE hNamedPipe = INVALID_HANDLE_VALUE;
    BOOL fConnected = FALSE;
    BOOL fSuccess = FALSE;
    DWORD dwBytesRead, dwBytesWritten;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    char buffer[MAX_BUFFER];
    char filename[MAX_BUFFER];
    const LPCSTR lpName = "\\\\.\\pipe\\LOCAL\\pipename";

    char log[MAX_BUFFER];
    char log_out[MAX_BUFFER];

    perror(TAG"Start waiting for clients");
    while(TRUE) {
        hNamedPipe = CreateNamedPipe(
                lpName,                  /* pipe name */
                PIPE_ACCESS_DUPLEX,      /* r/w access */
                PIPE_TYPE_MESSAGE |      /* recieve messages */
                PIPE_READMODE_MESSAGE |  /* read messages */
                PIPE_WAIT,               /* block when read */
                PIPE_UNLIMITED_INSTANCES,/* 256 instances */
                MAX_BUFFER,              /* out buffer */
                MAX_BUFFER,              /* in buffer */
                0,                       /**/
                NULL);                   /**/

        if (hNamedPipe == INVALID_HANDLE_VALUE)
        {
            perror(TAG ERROR_START"Can't create pipe"ERROR_END);
            fprintf(stderr, "Code %d\n", GetLastError());
            ExitProcess(1);
        }
        fConnected = ConnectNamedPipe(hNamedPipe, NULL) ? 
            TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (fConnected) {
            perror(TAG" Client connected");
            fSuccess = ReadFile(hNamedPipe, buffer, MAX_BUFFER, &dwBytesRead, NULL); /* read file name */
            if (!fSuccess) {
                perror(TAG"Error while reading pipe");
                ExitProcess(1);
            }
            buffer[dwBytesRead] = '\0';
            strcpy(filename, buffer);
            hFile = CreateFile(
                    filename,
                    GENERIC_READ,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_READONLY,
                    NULL);
            perror(TAG" CreateFile");
            while (TRUE) {
                fSuccess = ReadFile(hFile, buffer, MAX_BUFFER -1, &dwBytesRead, NULL);
                if (!fSuccess || dwBytesRead == 0) {
                    break;
                }
                buffer[dwBytesRead] = '\0';
                fSuccess = WriteFile(hNamedPipe, buffer, strlen(buffer), &dwBytesWritten, NULL);
                if (!fSuccess) {
                    break;
                }
                strcpy(log_out, TAG);
                sprintf(log, " %d read; %d write", dwBytesRead, dwBytesWritten);
                strcat(log_out, log);
                perror(log_out);
            }
        } else {
            CloseHandle(hNamedPipe);
        }

    }



    return 0;
}
