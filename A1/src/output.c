/*
 *  output.c - reads Windows pipe and converts string with such rule:
 *          Everything converts to snake_case
 */

#include <windows.h>

#include <stdio.h>


#define TAG "\033[33m[OUTPUT]\033[0m "

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
     
    char buffer[256];
    char result[256];

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
        snake_case(result, buffer);
        bResult = WriteFile(hOutFile, result, strlen(result), NULL, NULL);
        if (!bResult) {
            perror(TAG"Failed to output txt to file");
        }
    }
    perror(TAG"End read");
    CloseHandle(hOutFile);
    return 0;
}
