#include <windows.h>

#include <stdio.h>

#define MAX_BUFFER 256
#define TAG "\033[34m[FILTER]\033[0m"

char get_snake_case(char ch) {
    char result = ch;
    if (ch >= 'A' && ch <= 'Z') {
        result = ch + 32;
    } else if (ch == ' ') {
        result = '_';
    }
    return result;
}

int is_punctuation(char ch) {
    int result = 0;
    char punct[] = ".,?!<>;:'\"`";
    for (int i = 0; punct[i] != '\0'; ++i) {
        if (punct[i] == ch) {
            result = 1;
            break;
        }
    }
    return result;
}

void snake_case(char* destination, char* source) {
    int i;
    int j = 0;
    for(i = 0; '\0' != source[i]; ++i) {
        if (source[i] == ' ' && ((source[i + 1] == ' ') || is_punctuation(source[i]))) {
            continue;
        } else if (is_punctuation(source[i])) {
            continue;
        }
        destination[j] = get_snake_case(source[i]);
        j++;
    }
    destination[j] = '\0';    
}

int main() {
    STARTUPINFO si;
    GetStartupInfo(&si);
    
    HANDLE hInput, hOutput;
    hInput = si.hStdInput;
    hOutput = si.hStdOutput;

    const char* FILTER_START = TAG"Start processing";
    const char* FILTER_END = TAG"End processing";
    const char* FILTER_TAG = TAG"Processed ";

    const char* LOG_FORMAT = "read %d; written %d";

    char log[MAX_BUFFER];
    char log_out[MAX_BUFFER];

    char buffer[MAX_BUFFER];
    char result[MAX_BUFFER];

    DWORD dwRead;
    DWORD dwWritten;
    BOOL bResult = FALSE;

    perror(FILTER_START);
    while(TRUE) {
        bResult = ReadFile(hInput, buffer, MAX_BUFFER - 1, &dwRead, NULL);
        if (!bResult || dwRead == 0) {
            break;
        }
        buffer[dwRead] = '\0';
        snake_case(result, buffer);
        bResult = WriteFile(hOutput, result, MAX_BUFFER -1, &dwWritten, NULL);
        if (!bResult) {
            break;
        }
        sprintf(log, LOG_FORMAT, dwRead, dwWritten);
        strcpy(log_out, FILTER_TAG);
        strcat(log_out, log);
        perror(log_out);
        fflush(stderr);
    }
    perror(FILTER_END);
    
    CloseHandle(hInput);
    CloseHandle(hOutput);
    return 0;
}
