// Reynaldo De Leo
// CMPE 3334-02
//
// This program is the command interpreter
// for the assembler that is going to be
// throughout the semester
//
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sicengine.c"
#include "assembler.h"

char command1[128];
char command2[128];
char command3[128];
char str[128];
int firstExecutable = 0;

/**************************************************/

//this functions clears the arrays in which the strings for the program are stored
void clearin(char * a, char * b, char * c, char * d)
{
    for(int i = 0; i < 128; i++)
    {
        a[i] = '\0';
    }
    
    for(int i = 0; i < 128; i++)
    {
        b[i] = '\0';
    }
    
    for(int i = 0; i < 128; i++)
    {
        c[i] = '\0';
    }
    
    for(int i = 0; i < 128; i++)
    {
        d[i] = '\0';
    }
    
}

/**************************************************/

//puts commands into command 1 and commmand 2 and command 3 only if the user actually puts words
void scanningCommand(char * str)
{
    

    int itr = 0;
    char* checker = &str[itr];
    int i = 0; //this is the iterator for the string command 1
    int a = 0; //iterator for command 2 string
    int b = 0; //iterator for command 3 string
    int swtch = 1;

    
    
    while(*checker != '\n')
    {
        if(*checker >= 46 && *checker <= 122 && swtch == 1)
        {
//            printf("%c", *checker);
            command1[i] = *checker;
            i = i + 1;
            itr = itr + 1;
            checker = &str[itr];
            
            if(*checker == ' ')
            {
                swtch = 2;
            }
            
        }
        else if(*checker >= 46 && *checker <= 122 && swtch == 2)
        {
            command2[a] = *checker;
            a = a + 1;
            itr = itr + 1;
            checker = &str[itr];
            
            if(*checker == ' ')
            {
                swtch = 3;
            }
            
        }
        else if(*checker >= 46 && *checker <= 122 && swtch == 3)
        {
            command3[b] = *checker;
            b = b + 1;
            itr  = itr + 1;
            checker = &str[itr];
        }
        else
        {
            itr = itr + 1;
            checker = &str[itr];
        }
    }
}

/****************************************************/

void load(char * filename)
{
    FILE * reader;
    reader = fopen(filename, "r");
    char str[180];
    char adr[7];
    long int adrx;//this is going to be the address that is at the beginning of the text record
    char sizeOfTRec[3];
    long int sizeOfTRecx;

   
    while(!feof(reader)){
        fgets(str, 180, reader);
        
        if(str[0] == 'H'){
            generalClearer(str, 180);
        }
        else if(str[0] == 'T'){
            
            for(int i = 1; i < 7; ++i){
                adr[i-1] = str[i];
            }
            adrx = strtol(adr, NULL, 16);
            
            
            sizeOfTRec[0] = str[7];
            sizeOfTRec[1] = str[8];
            sizeOfTRecx = strtol(sizeOfTRec,NULL, 16);
            ADDRESS a = adrx;
            for(int i = 0, itr = 9;  i < sizeOfTRecx; ++i, itr = itr + 2){
                WORD mem;
                char twoDigits[3];
                twoDigits[0] = str[itr];
                twoDigits[1] = str[itr + 1];
                twoDigits[2] ='\0';
                long int number = strtol(twoDigits, NULL, 16);
                mem[0] = number;
                PutMem(a, mem, 0);
                a++;
            }
            
        }
        else if(str[0] == 'E'){
            sscanf(str+1, "%06x", &firstExecutable);
        }
        

    }
    
}
/****************************************************/

void execute()
{
    ADDRESS startingAddr[0];
    startingAddr[0] = firstExecutable;
    SICRun(startingAddr, 0);
}

/******************************************************/

void debug()
{
    printf("%s", "not yet implemented\n");
}

/*************************************************/
void dump(char * start, char * end)
{
    long int startx = strtol(start, NULL, 16);
    long int endx = strtol(end, NULL, 16);
    
    ADDRESS strt = startx;
    WORD memory;
    
    for(long int i = startx; i <= endx; ++i){
        GetMem(i, memory, 0);
        if(i % 16 == 0){
            printf("\n%lx%c",i, ':' );
        }
        printf("%02X ", (int)memory[0]);
    }
    
}
/***************************************************/

void assemble(char * filename)
{
    //these three function calls have to do with phase 2
    optableMaker();
    readingFile(filename);
    //    symbolTablePrinter();
    readingAndWritingFromIntermediate();
    //    printing();
    errorTableMaker();
    settingTheMachineCodeLength();
    makingTheListingFile();
    if(errorsInListingFileFlag == 1){
        return;
    }
    makingTheObjectFile();
    
}
/*******************************************************/

//this functions actually acts on the command that it is given at determines the appropriate action
void interpretingCommand(char * command1, char * command2, char * command3)
{
    
    if(strcmp(command1,"load") != 0 && strcmp(command1,"execute") != 0 && strcmp(command1,"debug") != 0 && strcmp(command1,"dump") != 0 && strcmp(command1,"help") != 0 && strcmp(command1,"assemble") != 0 && strcmp(command1,"directory") != 0 && strcmp(command1,"exit") != 0)
    {
        printf("%s", "that is not a valid command please enter an actual command\n");
        printf("%s", "if you dont know the commands type 'help'\n");

    }
    
    if(strcmp(command1, "load") == 0 && command2[0] != '\0' && command3[0] == '\0')//load filename
    {
        load(command2);
    }
    else if(strcmp(command1, "load") == 0 && command2[0] == '\0')
    {
        printf("%s", "you need to specify a filename\n");
        
    }
    else if(strcmp(command1, "load") == 0 && command2[0] != '\0' && command3[0] != '\0')
    {
        printf("%s", "theres too many parameters\n");

    }
    
    
    if(strcmp(command1, "help") == 0 && command2[0] == '\0' && command3[0] == '\0')//help
    {
        printf("%s", "\navailable commands are:\n");
        printf("%s", "\nload filename\nexecute\ndebug\ndump start end\nassemble filename\ndirectory\nexit\n");

    }
    else if(strcmp(command1, "help") == 0 && command2[0] != '\0' && command3[0] != '\0')
    {
        printf("%s", "'help' doesnt require parameters\n");

    }
    else if(strcmp(command1, "help") == 0 && command2[0] != '\0')
    {
        printf("%s", "'help' doesnt require parameters\n");

    }
        
            
    if(strcmp(command1, "directory") == 0 && command2[0] == '\0' && command3[0] == '\0')//directory
    {
        system("ls");
    }
    else if(strcmp(command1, "directory") == 0 && command2[0] != '\0' && command3[0] != '\0')
    {
        printf("%s", "'directory' doesnt require parameters\n");
        
    }
    else if(strcmp(command1, "directory") == 0 && command2[0] != '\0')
    {
        printf("%s", "'help' doesnt require parameters\n");
        
    }
    
    
    if(strcmp(command1, "execute") == 0 && command2[0] == '\0')//execute
    {
        execute();
    }
    else if(strcmp(command1, "execute") == 0 && command2[0] != '\0')
    {
        printf("%s", "execute doesnt take any parameters\n");
    }
    
    //debug
    if(strcmp(command1, "debug") == 0 && command2[0] == '\0')
    {
        debug();
    }
    else if(strcmp(command1, "debug") == 0 && command2[0] != '\0')
    {
        printf("%s", "debug doesnt take any parameters\n");
    }
    
    //dump
    if(strcmp(command1, "dump") == 0 && command2[0] != '\0' && command3[0] != '\0')
    {
        dump(command2, command3);
    }
    else if(strcmp(command1, "dump") == 0 && command2[0] == '\0')
    {
        printf("%s", "the function call dump requires parameters\n");
    }
    else if(strcmp(command1, "dump") == 0 && command2[0] != '\0' && command3[0] == '\0' )
    {
        printf("%s", "the function call dump requires two  parameters\n");
        
    }
    
    //assemble
    if(strcmp(command1, "assemble") == 0 && command2[0] != '\0')
    {
        assemble(command2);
    }
    else if(strcmp(command1, "assemble") == 0 && command2[0] == '\0')
    {
        printf("%s", "this function requires a parameter\n");
    }
    
    
        
    
    
    
    
}


/*****************************************************************/


int main()
{
    char str[128];
    int exitFlag = 0;
    
    SICInit();
    printf("Hello, please enter a command\n");
    
    do
    {
        fgets(str,128,stdin);
        scanningCommand(str);//puts the commands into strings
        interpretingCommand(command1, command2, command3); // interprets commands
        printf("%c",'\n', stdout);
        
        if(strcmp(command1, "exit") == 0)
        {
            exitFlag = 1;
        }
        
        clearin(str, command1, command2, command3);
        
    }
    while(exitFlag != 1);
    
    
    return 0;
}
