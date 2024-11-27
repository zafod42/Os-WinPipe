#include <windows.h>

#include <stdio.h>

#define TAG "\033[31m[RUNNER]\033[0m "

int main(int argc, char* argv[]) {

    HANDLE hFilterReadPipe, hSrcWritePipe, hSinkReadPipe, hFilterWritePipe;
    SECURITY_ATTRIBUTES pipeSA, filterToSinkSA;
    STARTUPINFO srcSI, sinkSI, filterSI;
    PROCESS_INFORMATION srcPI, sinkPI, filterPI;

    /* Create pipe with inheritence */
    ZeroMemory(&pipeSA, sizeof(pipeSA));
    pipeSA.nLength = sizeof(pipeSA);
    pipeSA.bInheritHandle = TRUE;
    if (!CreatePipe(&hFilterReadPipe, &hSrcWritePipe, &pipeSA, 0)) {
        perror(TAG"Could not create pipe");
        fprintf(stderr, TAG"Code: %d\n", GetLastError());
        ExitProcess(1);
    }
    perror(TAG"Create \033[32msrc\033[0m -> \033[34mfilter\033[0m pipe");
    fflush(stderr);

    /* Create filter -> sink pipe */
    ZeroMemory(&filterToSinkSA, sizeof(filterToSinkSA));
    filterToSinkSA.nLength = sizeof(filterToSinkSA);
    filterToSinkSA.bInheritHandle = TRUE;
    if (!CreatePipe(&hSinkReadPipe, &hFilterWritePipe, &filterToSinkSA, 0)) {
        perror(TAG"Could not create pipe");
        fprintf(stderr, TAG"Code: %d\n", GetLastError());
        ExitProcess(1);
    }
    perror(TAG"Create \033[34mfilter\033[0m -> \033[33msink\033[0m pipe");
    fflush(stderr);

    /* Create read process */
    ZeroMemory(&srcSI, sizeof(srcSI));
    srcSI.cb = sizeof(srcSI);
    srcSI.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    srcSI.hStdOutput = hSrcWritePipe;
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
    perror(TAG"Create \033[32mread\033[0m process");
    fflush(stderr);

    /* Create filter process */
    ZeroMemory(&filterSI, sizeof(filterSI));
    filterSI.cb = sizeof(filterSI);
    filterSI.hStdInput = hFilterReadPipe;         /* Input from Read Pipe */
    filterSI.hStdOutput = hFilterWritePipe;       /* Output to Write Pipe */
    filterSI.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    filterSI.dwFlags = STARTF_USESTDHANDLES;
    ZeroMemory(&filterPI, sizeof(filterPI));
    /* CreateProcess(...) */
    if (!CreateProcess(
             NULL, "bin/filter.exe",
             NULL, NULL, TRUE, 0,
             NULL, NULL, &filterSI, &filterPI)) 
    { 
        perror (TAG"Couldn't start filter process");
        fprintf(stderr, "Code %d\n", GetLastError());
        ExitProcess(1);
    }
    perror(TAG"Create \033[34mfilter\033[0m process");
    fflush(stderr);

    /* Create write process */
    ZeroMemory(&sinkSI, sizeof(sinkSI));
    sinkSI.cb = sizeof(sinkSI);
    sinkSI.hStdInput = hSinkReadPipe;
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
    perror(TAG"Create \033[33mwrite\033[0m process");
    fflush(stderr);

    CloseHandle(hFilterReadPipe);
    CloseHandle(hSinkReadPipe);
    CloseHandle(hFilterWritePipe);
    CloseHandle(hSrcWritePipe);
    perror(TAG"Closed \033[4mall\033[0m handles");
    fflush(stderr);

    WaitForSingleObject(srcPI.hProcess, INFINITE);
    perror(TAG"\033[32msrc\033[0m ended");
    WaitForSingleObject(filterPI.hProcess, INFINITE);
    perror(TAG"\033[34mfilter\033[0m ended");
    WaitForSingleObject(sinkPI.hProcess, INFINITE);
    perror(TAG"\033[33msink\033[0m ended");
    fflush(stderr);

    CloseHandle(srcPI.hProcess);
    CloseHandle(srcPI.hThread);
    CloseHandle(sinkPI.hProcess);
    CloseHandle(sinkPI.hThread);
    CloseHandle(filterPI.hProcess);
    CloseHandle(filterPI.hThread);
    perror(TAG"\033[31mrunner\033[0m ended");
    fflush(stderr);
    return 0;
}
