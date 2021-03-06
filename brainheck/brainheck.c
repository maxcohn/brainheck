// brainheck.c
// 
// brainheck is my personal implementation of a brainfuck interpreter
//
// TODO implement additional syntax

#include <stdio.h>
#include <stdlib.h>

#define TAPE_SIZE 30000

long read_program(char *infile, char **program){
    // store size of file
    long fileSize = 0;
    FILE *fp = fopen(infile, "r");

    // get number of bytes in the program
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);

    // reset file position to beginning
    fseek(fp, 0L, SEEK_SET);	

    // allocate memory for program to be read in
    *program = calloc(fileSize, sizeof(char));	

    // if there way a memory error, quit
    if(*program == NULL)
        exit(-1);

    // read all text in program
    fread(*program, sizeof(char), fileSize, fp);
    fclose(fp);

    return fileSize;
}

int main(int argc, char** argv){

    //TODO correct for bad input

    // pointer to the program we're going to interpret
    char *program = NULL;
    char *ip = NULL;

    // create 'tape' and initialize all elements to 0
    int tape[TAPE_SIZE] = {0};
    int *p = tape;

    // read the file
    long programSize = read_program(argv[1], &program);
    ip = program;

    //printf("%s", program);

    // while we haven't hit the end of the program
    while(ip < program + programSize){
        switch(*ip){
            // move pointer right
            case '>':
                // if p is going to run off tape to the right
                if(p >= tape + TAPE_SIZE)
                    return -1;
                p++;
                break;
            
            // move pointer left
            case '<':
                // if p is going to run off tape to the left
                if(p <= tape)
                    return -1;
                p--;
                break;

            // print char in current space
            case '.':
                putchar(*p);
                fflush(stdout);
                break;
            
            // read char into current space
            case ',':
                *p = getchar();
                break;

            // begin loop
            case '[':
                if(*p == 0){
                    // count of how many loops have occured
                    // this makes sure we have correct matching brackets
                    int loopCount = 1;
                    while(loopCount){
                        ip++;
                        if (*ip == '[') loopCount++;
                        if (*ip == ']') loopCount--;
                    }
                }
                break;
            
            // end loop
            case ']':
                if(*p != 0){
                    // count how many loops have occured
                    int loopCount = 1;
                    while(loopCount){
                        ip--;
                        if(*ip == '[') loopCount--;
                        if(*ip == ']') loopCount++;
                    }
                }
                break;

            // increment current value
            case '+':
                (*p)++;
                break;
            
            // decrememnt current value
            case '-':
                (*p)--;
                break;

            // print numeric value of current space
            case ':':
                printf("%d", *p);
                break;

            // read number into current space
            case ';':
                scanf("%d", p); // read number directly into p. is this bad?
                //TODO add error checking??
                break;
            
            // comment
            case '#':
                // loop until we hit another #
                while(*(++ip) != '#'){
                    // if our input program pointer goes out of the program,
                    // and the main loop will end
                    if(ip >= program + programSize)
                        break;
                }

                break;

            // otherwise, ignore char
            default:
                break;
        }

        // move to next character of program
        ip++;
    }


    free(program);

}

