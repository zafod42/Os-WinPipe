#include <windows.h>

#include <stdio.h>

int main(int argc, char* argv[]) {

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES pipeSA;
    STARTUPINFO srcSI, sinkSI;
    PROCESS_INFORMATION srcPI, sinkPI;

    perror("Try to create pipe");
    /* Create pipe with inheritence */
    pipeSA.bInheritHandle = TRUE;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &pipeSA, 0)) {
        perror("Could not create pipe");
        ExitProcess(1);
    }
    perror("Create pipe");

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
             NULL, "./source.exe",
             NULL, NULL, TRUE, 0,
             NULL, NULL, &srcSI, &srcPI)) 
    { 
        perror ("Could not process");
        ExitProcess(1);
    }
    perror("Create read process");

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
             NULL, "./output.exe",
             NULL, NULL, TRUE, 0,
             NULL, NULL, &srcSI, &srcPI)) 
    { 
        perror ("Could not process");
        ExitProcess(1);
    }
    perror("Create write process");
    perror("exit");

    return 0;
}
