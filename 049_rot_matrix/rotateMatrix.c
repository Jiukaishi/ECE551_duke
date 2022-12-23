#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
void error(const char* s){
perror(s);
exit(EXIT_FAILURE);
}
void read_file(FILE* f, char m[10][10]){

    for(size_t i=0;i<10;i++){
    int c;
    for(size_t j=0;j<10;j++){
    c = fgetc(f);
    if(c=='\n'||c=='\0'){
            error("line end early");
        }
    if(c==-1){
        error("Unexpected EOF");
    }
    m[i][j] = c;

    }
    c = fgetc(f);
    if(c!='\n')
        error("line too long");

    }
    int c = fgetc(f);
    if(c!=-1)
        error("too much rows");
    return;
}
void print_res(char m[10][10]){
for(int i=0;i<10;i++)
{for(int j=0;j<10;j++)
    {
        printf("%c",m[9-j][i]);
    }
    printf("\n");
}
}

int main(int argc, char* argv[]){
if(argc!=2){
perror("Must have one argument");
return EXIT_FAILURE;
}
FILE* f = fopen(argv[1],"r");
if(f==NULL){
perror("Can not open file");
return EXIT_FAILURE;
}

char matrix[10][10] ;
read_file(f, matrix);
print_res(matrix);

if(fclose(f)!=0){
perror("Fail to close the file");
return EXIT_FAILURE;
}
return EXIT_SUCCESS;
}
