#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_PATH_LEN 260

//ИСПРАВИТЬ НАЛИЧИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
//МЫ ДОЛЖНЫ ВСЕГДА ПЕРЕДАВАТЬ ЗНАЧЕНИЯ В ФУНКЦИЯХ СОРТИРОВКИ
char** fileNames = NULL;
long long* fileSizes = NULL;
int fileCount = 0;

void swap(int i, int j);
int loadFiles(const char* path);
void freeMemory();
void printFiles();
void bubbleSort();
void сhoiceSort();
void insertionSort();
void merge(int left, int mid, int right);
void mergeSort(int left, int right);
void quickSort(int left, int right);

int main() {
    setlocale(LC_ALL, "Russian");
    char path[MAX_PATH_LEN];
    char input[260];

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);

    printf_s("Введите путь к каталогу:\n");
    fgets(path, MAX_PATH_LEN, stdin);
    path[strcspn(path, "\n")] = 0;

    if (loadFiles(path))
        return 0;

    while (1) {
        printf_s(
            "\n1 - Пузырёк\n"
            "2 - Выбором\n"
            "3 - Вставками\n"
            "4 - Слиянием\n"
            "5 - Быстрая\n"
            "0 - Выход\n"
            "Ваш выбор: "
        );

        if (!fgets(input, sizeof(input), stdin)) {
            continue;
        }   

        input[strcspn(input, "\n")] = '\0';

        char* end1;
        int choice = strtol(input, &end1, 10);

        if (end1 == input || *end1 != '\0') {
            printf_s("Ошибка: введите число от 0 до 5\n");
            continue;
        }

        if (choice == 0)
            break;

        QueryPerformanceCounter(&start);

        switch (choice) {
        case 1: bubbleSort(); break;
        case 2: сhoiceSort(); break;
        case 3: insertionSort(); break;
        case 4: mergeSort(0, fileCount - 1); break;
        case 5: quickSort(0, fileCount - 1); break;
        default:
            continue;
        }

        QueryPerformanceCounter(&end);
        printFiles();

        printf_s("\nВремя: %.6f сек\n",
            (double)(end.QuadPart - start.QuadPart) / freq.QuadPart);
    }


    freeMemory();
    return 0;
}
void swap(int i, int j) {
    long long tmpSize = fileSizes[i];
    fileSizes[i] = fileSizes[j];
    fileSizes[j] = tmpSize;

    char* tmpName = fileNames[i];
    fileNames[i] = fileNames[j];
    fileNames[j] = tmpName;
}

int loadFiles(const char* path) {
    WIN32_FIND_DATAA findData;
    HANDLE hFind;
    char searchPath[MAX_PATH_LEN];

    snprintf(searchPath, MAX_PATH_LEN, "%s\\*", path);

    hFind = FindFirstFileA(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Ошибка: каталог не найден.\n");
        return 1;
    }

    fileCount = 0;
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            fileCount++;
    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);

    if (fileCount == 0) {
        printf("В каталоге нет файлов.\n");
        return 1;
    }

    fileNames = malloc(fileCount * sizeof(char*));
    fileSizes = malloc(fileCount * sizeof(long long));

    hFind = FindFirstFileA(searchPath, &findData);
    int i = 0;

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            size_t len = strlen(findData.cFileName) + 1;

            fileNames[i] = malloc(len);
            strcpy_s(fileNames[i], len, findData.cFileName);

            fileSizes[i] =
                ((long long)findData.nFileSizeHigh << 32) |
                findData.nFileSizeLow;
            i++;
        }
    } while (FindNextFileA(hFind, &findData));

    FindClose(hFind);
    return 0;
}


void freeMemory() {
    if (!fileNames || !fileSizes)
        return;

    for (int i = 0; i < fileCount; i++)
        free(fileNames[i]);

    free(fileNames);
    free(fileSizes);

}


void printFiles() {
    printf("\nИмя файла                              Размер (байт)\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < fileCount; i++)
        printf("%-35s %18lld\n", fileNames[i], fileSizes[i]);
}


void bubbleSort() {
    int i, j;
    for (i = 0; i < fileCount - 1; i++)
        for (j = 0; j < fileCount - i - 1; j++)
            if (fileSizes[j] > fileSizes[j + 1])
                swap(j, j + 1);
}

void сhoiceSort() {
    int i, j;
    for (i = 0; i < fileCount; i++)
    {
        long long min = fileSizes[i];
        int minind = i;
        for (j = i + 1; j < fileCount; j++)
        {
            if (min > fileSizes[j])
            {
                min = fileSizes[j];
                minind = j;
            }
        }
        if (minind != i) {
            swap(i, minind);
        }
    }
}

void insertionSort() {
    int i;
    for (i = 1; i < fileCount; i++) {
        int j = i - 1;
        long long tmp = fileSizes[i];
        while (j >= 0 && fileSizes[j] > tmp) {
            fileSizes[j + 1] = fileSizes[j];
            j--;
        }
        fileSizes[j + 1] = tmp;
    }
}

void merge(int left, int mid, int right) {
    int n = right - left + 1;

    long long* tmpSizes = malloc(n * sizeof(long long));
    char** tmpNames = malloc(n * sizeof(char*));

    int it1 = 0, it2 = 0, k = 0;
    int i = it1 + left, j = it2 + mid + 1;

    while (i <= mid && j <= right) {
        if (fileSizes[i] < fileSizes[j]) {
            tmpSizes[k] = fileSizes[i];
            tmpNames[k++] = fileNames[i++];
        }
        else {
            tmpSizes[k] = fileSizes[j];
            tmpNames[k++] = fileNames[j++];
        }
    }

    while (i <= mid) {
        tmpSizes[k] = fileSizes[i];
        tmpNames[k++] = fileNames[i++];
    }

    while (j <= right) {
        tmpSizes[k] = fileSizes[j];
        tmpNames[k++] = fileNames[j++];
    }

    for (it1 = 0; it1 < k; it1++) {
        fileSizes[left + it1] = tmpSizes[it1];
        fileNames[left + it1] = tmpNames[it1];
    }

    free(tmpSizes);
    free(tmpNames);
}

void mergeSort(int left, int right) {
    int mid;
    if ((left + 1) > right) return;
    mid = left + (right - left) / 2;
    mergeSort(left, mid);
    mergeSort(mid + 1, right);
    merge(left, mid, right);
}

void quickSort(int left, int right)
{
    int n1 = left;
    int n2 = right;
    int mid = n1 + (n2 - n1) / 2;
    int i = n1, j = n2;
    long long p = fileSizes[mid];
    do {
        while (fileSizes[i] < p) i++;
        while (fileSizes[j] > p) j--;
        if (i <= j) {
            swap(i, j);
            i++;
            j--;
        }
    } while (i <= j);
    if (n1 < j) {
        quickSort(n1, j);
    }
    if (i < n2)
        quickSort(i, n2);
}
