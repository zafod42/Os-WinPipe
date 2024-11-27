/*
 *  output.c - reads Windows pipe and converts string with such rule:
 *          Everything converts to snake_case
 */

#include <windows.h>

#include <stdio.h>

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

    int i = 0;
    BOOL bResult;
    DWORD dwRead;
    char tmp;
    while (dwRead != 0) {
        bResult = ReadFile(si.hStdInput, &tmp, 1, &dwRead, NULL);
        if (!bResult) {
            perror("Failed");
            exit(1);
        }
        buffer[i] = tmp;
        i++;
    }
    snake_case(result, buffer);
    printf("%s\n", result);

    return 0;
}
