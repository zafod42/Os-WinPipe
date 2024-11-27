#include <windows.h>

#include <stdio.h>

#define TAG "\033[31m[RUNNER]\033[0m "

int main(int argc, char* argv[]) {

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES pipeSA;
    STARTUPINFO srcSI, sinkSI;
    PROCESS_INFORMATION srcPI, sinkPI;

    perror(TAG"Try to create pipe");
    /* Create pipe with inheritence */
    ZeroMemory(&pipeSA, sizeof(pipeSA));
    pipeSA.nLength = sizeof(pipeSA);
    pipeSA.bInheritHandle = TRUE;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &pipeSA, 0)) {
        perror(TAG"Could not create pipe");
        fprintf(stderr, TAG"Code: %d\n", GetLastError());
        ExitProcess(1);
    }
    perror(TAG"Create pipe");

    /* Create read process */
    ZeroMemory(&srcSI, sizeof(srcSI));
    srcSI.cb = sizeof(srcSI);
    srcSI.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    srcSI.hStdOutput = hWritePipe;
    srcSI.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    srcSI.dwFlags = STARTF_USESTDHANDLES;
    ZeroMemory(&srcPI, sizeof(srcPI));
    /* CreateProcess(...) */
    if (!CreateProcess(
             NULL, "bin/source.exe",
             NULL, NULL, TRUE, 0,
             NULL, NULL, &srcSI, &srcPI)) 
    { 
        perror (TAG"Couldn't start read process");
        fprintf(stderr, "Code %d\n", GetLastError());
        ExitProcess(1);
    }
    perror(TAG"Create read process");

    /* Create write process */
    ZeroMemory(&sinkSI, sizeof(sinkSI));
    sinkSI.cb = sizeof(sinkSI);
    sinkSI.hStdInput = hReadPipe;
    sinkSI.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    sinkSI.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    sinkSI.dwFlags = STARTF_USESTDHANDLES;
    ZeroMemory(&sinkPI, sizeof(sinkPI));
    /* CreateProcess(...) */
    if (!CreateProcess(
             NULL, "bin/output.exe",
             NULL, NULL, TRUE, 0,
             NULL, NULL, &sinkSI, &sinkPI)) 
    { 
        perror (TAG"Couldn't start write process");
        fprintf(stderr, TAG"Code %d\n", GetLastError());
        ExitProcess(1);
    }
    perror(TAG"Create write process");


    WaitForSingleObject(srcPI.hProcess, INFINITE);
    WaitForSingleObject(sinkPI.hProcess, INFINITE);

    CloseHandle(srcPI.hProcess);
    CloseHandle(srcPI.hThread);
    CloseHandle(sinkPI.hProcess);
    CloseHandle(sinkPI.hThread);

    return 0;
}
