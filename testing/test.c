#include <stdio.h>
#include <string.h>

int main() {
    FILE * f;
    f=fopen("/Users/pietroarsi/Desktop/ROBE/code/API2020TestGenerator/testSets/1594837639/level5/test10.txt", "r");
    char s[1024];

    //fscanf(f, "%s", s);
    fgets(s, 1024, f);
    while(strcmp(s, "q") != 0) {
        printf("%s", s);
        fgets(s, 1024, f);
        //fscanf(f, "%s", s);
    }
}