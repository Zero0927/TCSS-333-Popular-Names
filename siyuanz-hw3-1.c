//Siyuan Zhou
#include<stdio.h>
#include<string.h>

#define Year 10
#define Name_length 20
#define Rank 0
#define Top_number  100
#define Name_num (Top_number*Year)

int All_files(char (*names)[Name_length], int (*rank)[Year], char**files, unsigned fileNum);
int Single_file(char (*names)[Name_length], int (*rank)[Year], int num, const char*file, int yearIndex);
int Name_process(char (*names)[Name_length], int (*rank)[Year], const char*name, int yearIndex, int rankInYear);
void Sort(char (*names)[Name_length], int (*rank)[Year], int num);
void Ouput_file(char (*names)[Name_length], int (*rank)[Year], int num, char**title, char*fileName);

int All_files(char (*names)[Name_length], int (*rank)[Year], char**files, unsigned fileNum) {
    int num = 0, year = 0;
    while (fileNum--) {
        num = Single_file(names, rank, num, *files, year);
        files++;
        year++;
    }
    return num;
}

void Get_name(const char*str, char*name) {
    while (*str != ',')
        *name++ = *str++;
    *name = 0;
}

int Single_file(char (*names)[Name_length], int (*rank)[Year], int num, const char*file, int yearIndex) {
    FILE*pf;
    int i = 0;
    char name[Name_length], tmp[128];
    if (NULL == (pf = fopen(file, "r")))
        return 0;
    while(i<Top_number){
        fscanf(pf, "%s", tmp);
        Get_name(tmp, name);
        num += Name_process(names, rank, name, yearIndex, 1 + i);
        i++;
    }
    fclose(pf);
    return num;
}

int search_rank(char (*names)[Name_length], const char*name) {
    int index = 0;
    while (strlen(names[index]) > 0 && strcmp(name, names[index]) != 0)
        index++;
    return index;
}


int Name_process(char (*names)[Name_length], int (*rank)[Year], const char*name, int yearIndex, int rankInYear) {
    int inserIndex, i = 0;
    inserIndex = search_rank(names, name);
    if (strlen(names[inserIndex]) == 0) {
        strcpy(names[inserIndex], name);
        while (i<Year){
            rank[inserIndex][i] = Rank;
            i++;
        }
        rank[inserIndex][yearIndex] = rankInYear;
        names[inserIndex + 1][0] = 0;
        return 1;
    } else {
        rank[inserIndex][yearIndex] = rankInYear;
        return 0;
    }
}

void switch_rank(char (*names)[Name_length], int (*rank)[Year], int sa, int sb) {
    char tmp[Name_length];
    int i = 0, tr;
    if (sa == sb)
        return;
    strcpy(tmp, names[sa]);
    strcpy(names[sa], names[sb]);
    strcpy(names[sb], tmp);
    while(i< Year){
        tr = rank[sa][i];
        rank[sa][i] = rank[sb][i];
        rank[sb][i] = tr;
        i++;
    }
}

void Sort(char (*names)[Name_length], int (*rank)[Year], int num) {
    int i = 0, j, min;
    while(i < num){
        min = i;
        for (j = i + 1; j < num; ++j) {
            if (strcmp(names[j], names[min]) < 0)
                min = j;
        }
        if (min != i) {
            switch_rank(names, rank, min, i);
        }
        i++;
    }
}

void Ouput_file(char (*names)[Name_length], int (*rank)[Year], int num, char**title, char*fileName) {
    FILE*pf;
    int i = 0, n;
    pf = fopen(fileName, "w");
    if (!pf)
        return;
    fprintf(pf, "%s", title[0]);
    while(i< Year){
        fprintf(pf, ",%s", title[1 + i]);
        i++;
    }
    n = 0;
    while (num--) {
        fprintf(pf, "\n");
        fprintf(pf, "%s", names[n]);
            int i =0;
            while(i < Year){
            if (Rank == rank[n][i])
                fprintf(pf, ",");
            else
                fprintf(pf, ",%d", rank[n][i]);
            i++;
        }
        n++;
    }
    fclose(pf);
}

void main() {
    char *files[] = {"yob1920.txt", "yob1930.txt", "yob1940.txt", "yob1950.txt",
        "yob1960.txt", "yob1970.txt", "yob1980.txt", "yob1990.txt",
        "yob2000.txt", "yob2010.txt"};
    char*title[] = {"name",
        "1920", "1930", "1940", "1950", "1960",
        "1970", "1980", "1990", "2000", "2010"};
    char names[Name_num][Name_length] = {0};
    int rank[Name_num][Year] = {0};
    int num;
    num = All_files(names, rank, files, sizeof (files) / sizeof (files[0]));
    Sort(names, rank, num);
    Ouput_file(names, rank, num, title, "summary.csv");
}

