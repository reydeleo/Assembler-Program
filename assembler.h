//
//  assembler.c
//  phase1
//
//  Created by reynaldo deleo on 12/3/17.
//  Copyright © 2017 reynaldo deleo. All rights reserved.
//

// Reynaldo De Leo
// CMPE 3334-02
//
// This program has phase 2 and phase 3 what phase 2
// it does is that is makes the intermediate file
// as well as a symbol table as well as check for
// errors that might exist in the program and what phase 3
// does is that it constructs the listing file and the
// object file and copies the errors that were found in
// in phase 2.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct mnemonic {
    
    char name[20];
    int opcode;
    
};

struct symbol {
    char name[20];
    int long addressLocation;
};

struct string{
    char str[100];
    char address[100];
    long int intAddress;
    char sourceLine[180];
    char machineCode[100];
    long int lengthOfMachineCode;
    char errors[100];
    long int errorsInt[6];
    
};


//global variables
int long locctr = 0;
char * label;
char * operation;
char * operand;
struct mnemonic optable[25];
int long startingAddress = 0;
struct symbol symbolTable[600];//symbol table
int itrSym = 0;//the itr of the symbol table
int errors[20];//this is to put store the errors
int errorsItr = 0;//this is going to be the itr for the erros array
struct string orderedInstructions[100];
int orderedInstItr = 0;

//this is function is to clear the str char array
void clearing(char * a)
{
    for(int i = 0; i < 180; i++)
    {
        a[i] = '\0';
    }
}

void clearingErrorsArray()
{
    for(int i = 0; i < 20; i++)
    {
        errors[i] = 0;
    }
}

void makingToUppercase(char * x)
{
    for(int i = 0; x[i]; i++)
    {
        if(x[i] >= 97 && x[i] <= 122)
        {
            x[i] = x[i] - 32;
        }
    }
    
}

void optableMaker()
{
    
    //ADD
    struct mnemonic ADD;
    strcpy(ADD.name, "ADD");
    ADD.opcode = 0x18;
    optable[0] = ADD;
    
    //AND
    struct mnemonic AND;
    strcpy(AND.name, "AND");
    AND.opcode = 0x58;
    optable[1] = AND;
    
    //COMP
    struct mnemonic COMP;
    strcpy(COMP.name, "COMP");
    COMP.opcode = 0x28;
    optable[2] = COMP;
    
    //DIV
    struct mnemonic DIV;
    strcpy(DIV.name, "DIV");
    DIV.opcode = 0x24;
    optable[3] = DIV;
    
    //J
    struct mnemonic J;
    strcpy(J.name, "J");
    J.opcode = 0x3c;
    optable[4] = J;
    
    //JEQ
    struct mnemonic JEQ;
    strcpy(JEQ.name, "JEQ");
    JEQ.opcode = 0x30;
    optable[5] = JEQ;
    
    //JGT
    struct mnemonic JGT;
    strcpy(JGT.name, "JGT");
    JGT.opcode = 0x34;
    optable[6] = JGT;
    
    //JLT
    struct mnemonic JLT;
    strcpy(JLT.name, "JLT");
    JLT.opcode = 0x38;
    optable[7] = JLT;
    
    //JSUB
    struct mnemonic JSUB;
    strcpy(JSUB.name, "JSUB");
    JSUB.opcode = 0x48;
    optable[8] = JSUB;
    
    //LDA
    struct mnemonic LDA;
    strcpy(LDA.name, "LDA");
    LDA.opcode = 0x00;
    optable[9] = LDA;
    
    //LDCH
    struct mnemonic LDCH;
    strcpy(LDCH.name, "LDCH");
    LDCH.opcode = 0x50;
    optable[10] = LDCH;
    
    //LDL
    struct mnemonic LDL;
    strcpy(LDL.name, "LDL");
    LDL.opcode = 0x08;
    optable[11] = LDL;
    
    //LDX
    struct mnemonic LDX;
    strcpy(LDX.name, "LDX");
    LDX.opcode = 0x04;
    optable[12] = LDX;
    
    //MUL
    struct mnemonic MUL;
    strcpy(MUL.name, "MUL");
    MUL.opcode = 0x20;
    optable[13] = MUL;
    
    //OR
    struct mnemonic OR;
    strcpy(OR.name, "OR");
    OR.opcode = 0x44;
    optable[14] = OR;
    
    //RD
    struct mnemonic RD;
    strcpy(RD.name, "RD");
    RD.opcode = 0xD8;
    optable[15] = RD;
    
    //RSUB
    struct mnemonic RSUB;
    strcpy(RSUB.name, "RSUB");
    RSUB.opcode = 0x4C;
    optable[16] = RSUB;
    
    //STA
    struct mnemonic STA;
    strcpy(STA.name, "STA");
    STA.opcode = 0x0C;
    optable[17] = STA;
    
    //STCH
    struct mnemonic STCH;
    strcpy(STCH.name, "STCH");
    STCH.opcode = 0x54;
    optable[18] = STCH;
    
    //STL
    struct mnemonic STL;
    strcpy(STL.name, "STL");
    STL.opcode = 0x14;
    optable[19] = STL;
    
    //STX
    struct mnemonic STX;
    strcpy(STX.name, "STX");
    STX.opcode = 0x10;
    optable[20] = STX;
    
    //SUB
    struct mnemonic SUB;
    strcpy(SUB.name, "SUB");
    SUB.opcode = 0x1C;
    optable[21] = SUB;
    
    //TD
    struct mnemonic TD;
    strcpy(TD.name, "TD");
    TD.opcode = 0xE0;
    optable[22] = TD;
    
    //TIX
    struct mnemonic TIX;
    strcpy(TIX.name, "TIX");
    TIX.opcode = 0x2C;
    optable[23] = TIX;
    
    //WD
    struct mnemonic WD;
    strcpy(WD.name, "WD");
    WD.opcode = 0xDC;
    optable[24] = WD;
    
    
}





int optableSearcher(char * mnemonic)
{
    if(mnemonic == NULL)
    {
        return -6;
    }
    
    int opcode = 0;
    
    //    for(int i = 0; mnemonic[i]; i++)//this loop makes all of the lettes in mnemonic upper case
    //    {
    //        mnemonic[i] = toupper(mnemonic[i]);
    //
    //    }
    
    for(int i = 0; i < 25; i++)
    {
        if(strcmp(optable[i].name, mnemonic) == 0)
        {
            locctr = locctr + 3;
            opcode = optable[i].opcode;
            
        }
    }
    return opcode;
}





void symbolTableInserter()
{
    for(int i = 0; label[i]; i++)//this makes sure that the name that is being saved for the label is all uppercase
    {
        label[i] = toupper(label[i]);
    }
    
    if(strlen(label) > 6)//this is to check the length of the label
    {
        errors[errorsItr] = 2;
        errorsItr++;
    }
    
    if(label[0] < 65 || label[0] > 90)//this is to check that the label is not illegal and begins with a letter
    {
        errors[errorsItr] = 2;
        errorsItr++;
    }
    
    for(int i = 0; label[i]; i++)//this is to check that the label is only composed of #s and letters
    {
        if((label[i] >= 48 && label[i] <= 57) || (label[i] >= 65 && label[i] <= 90))
        {
            //this means that it is either a # or a letter
        }
        else
        {
            errors[errorsItr] = 2;
            errorsItr++;
        }
    }
    
    //this is checking that the symbol that I am about to insert is not already in the symbol table
    for(int i = 0; i < 500; i++)
    {
        if(strcmp(symbolTable[i].name, label) == 0)
        {
            errors[errorsItr] = 1;
            errorsItr++;
        }
    }
    
    if(itrSym == 499)//this is if there are too many symbols
    {
        errors[errorsItr] = 7;
        errorsItr++;
    }
    
    struct symbol newEntry;
    strcpy(newEntry.name, label);
    newEntry.addressLocation = locctr;
    
    symbolTable[itrSym] = newEntry;
    itrSym++;
}






void symbolTablePrinter()
{
    for(int i = 0; i < itrSym; ++i)
    {
        printf("%s  ", symbolTable[i].name);
        printf("%lx\n", symbolTable[i].addressLocation);
        
        
        
    }
}







void tokenizing(char * a)
{
    makingToUppercase(a);
    char * endptr;
    char * token = a;
    token = strtok(a, " \t\n");
    
    if((a[0] >= 65 && a[0] <= 90) || (a[0] >= 48 && a[0] <= 57))//this is to check that there is a label and it can start with a letter or #
    {
        label  = token;
        printf("Label: %s\n", label);
        
        token = strtok(NULL, " \t\n");
        
        operation = token;
        printf("operation: %s\n", operation);
        
        token = strtok(NULL, " \t\n");
        
        operand = token;
        printf("operand: %s\n", operand);
        
        if(operation != NULL && operand != NULL)
        {
            if(strcmp(operation, "START") == 0){
                locctr = strtol(operand,&endptr,16);
                operation = "START";
                //saving starting adress so that I can calculate the program length
                char * endptr;
                startingAddress = strtol(operand, &endptr, 16);
                
                for(int i = 0; operand[i]; i++)//this is to check that the operand that comes with the START directive is a hexadecimal #
                {
                    if((operand[i] >= 48 && operand[i] <= 57) || (operand[i] >= 65 && operand[i] <= 70))
                    {
                        //then this is fine
                    }
                    else
                    {
                        errors[errorsItr] = 5;
                        errorsItr++;
                    }
                }
                
            }
        }
        else if(operation != NULL){
            if(strcmp(operation, "START") == 0 && operand == NULL)//this is in case that there is no operand
            {
                errors[errorsItr] = 5;
                errorsItr++;
            }
        }
    }
    else if(a[0] == ' ' || a[0] == '\t')
    {
        
        printf("%s", "Label:\n");
        label = NULL;
        operation = token;
        printf("operation: %s\n", operation);
        
        if(operation != NULL)
        {
            if(strcmp(operation, "RSUB") == 0)
            {
                label = NULL;
                operand = NULL;
                printf("%s", "operand:\n");
                return;
            }
            
            token = strtok(NULL, " \t\n");
            
            operand = token;
            printf("operand: %s\n", operand);
        }
        
    }
}







void constantCntr()//this function deals with the BYTE directive
{
    char * chars = operand;
    int numChars = 0;
    unsigned long stringLength = 0;
    
    if(operand != NULL)
    {
        unsigned long stringLength = strlen(operand);
        makingToUppercase(operand);
        for(int i = 0; i < stringLength; i++)
        {
            if((chars[i] >= 48 && chars[i] <= 57) || (chars[i] >= 65 && chars[i] <= 90)) // as long as it is a number or any capital letter
            {
                numChars++;
            }
        }
        
        if(chars[0] == 'C')
        {
            locctr = locctr + (numChars - 1);
            if((numChars - 1) > 30)
            {
                errors[errorsItr] = 4;
                errorsItr++;
            }
        }
        else if(chars[0] == 'X')
        {
            locctr = locctr + ((numChars - 1)/2);
            if(numChars > 33)
            {
                errors[errorsItr] = 4;
                errorsItr++;
            }
            if((numChars-1)%2 != 0)
            {
                errors[errorsItr] = 4;
                errorsItr++;
            }
        }
        else if(chars[0] != 'C' || chars[0] != 'X')
        {
            errors[errorsItr] = 4;
            errorsItr++;
        }
        // from this part down is just to check if both of the quotation marks are there
        int numberOfQuotationMarks = 0;
        for(int i = 0; operand[i]; i++)
        {
            if(operand[i] == 39)
            {
                numberOfQuotationMarks++;
            }
        }
        if(numberOfQuotationMarks != 2)
        {
            errors[errorsItr] = 4;
            errorsItr++;
        }
        
    }
    else if(operand == NULL)
    {
        errors[errorsItr] = 4;
        errorsItr++;
    }
    
}





int checkingIfItsANumber(char * x)
{
    int fakeBoolean = 1;
    
    for(int i = 0; x[i]; i++)
    {
        if(x[i] >= 48 && x[i] <= 57){
            
        }
        else
        {
            fakeBoolean = 0;
        }
    }
    return fakeBoolean;
    
}





void readingFile(char * file)
{
    int opcde = 0;
    char commentChecker;
    int long programLength = 0;
    int swtch = 0;
    
    FILE * fl;
    char str[180];
    //opens file to write to
    FILE * fp;
    fp = fopen("intermediate2.txt", "w");
    
    //opens the file that it is going to be reading from
    fl = fopen(file, "r");
    
    if((fl = fopen(file, "r")))
    {
        fgets(str, 180, fl);//this reads a line from a file
        commentChecker = str[0];
        
        if(commentChecker == '.' )
        {
            while(commentChecker == '.')//while its still a comment print it and get the next line
            {
                fprintf(fp, "%s", str);
                clearing(str);
                fgets(str, 180, fl);
                commentChecker = str[0];
            }
        }
        
        while(!feof(fl))
        {
            fprintf(fp, "%s", str);
            tokenizing(str);//this splits up the line into the label operation operand
            
            if(operation != NULL)//this is so that it stops when it sees the END
            {
                if(strcmp(operation, "END") == 0)
                {
                    swtch = 1;
                    break;
                }
                
            }
            
            fprintf(fp,"%lx\n",locctr);//the locctr has to be up here
            
            if(label != NULL)
            {
                symbolTableInserter();
            }
            
            
            opcde = optableSearcher(operation);
            
            if(opcde == 0)
            {
                if(strcmp(operation, "WORD") == 0)
                {
                    locctr = locctr + 3;
                    opcde = -2;
                    if(operand == NULL)//this is to check that the operand is not missing
                    {
                        errors[errorsItr] = 4;
                        errorsItr++;
                    }
                    else if(checkingIfItsANumber(operand) == 0)//this is to check that the operand is a number
                    {
                        errors[errorsItr] = 4;
                        errorsItr++;
                    }
                }
                else if(strcmp(operation, "RESB") == 0)
                {
                    int long operandNum = 0;
                    
                    if(operand != NULL)
                    {
                        char * endptr;
                        int long operandNum = strtol(operand,&endptr, 10);//making the string into an int
                        locctr = locctr + operandNum;
                        if(checkingIfItsANumber(operand) == 0)
                        {
                            errors[errorsItr] = 4;
                        }
                    }
                    else if(operand == NULL)
                    {
                        errors[errorsItr] = 4;
                        errorsItr++;
                    }
                    opcde = -3;
                }
                else if(strcmp(operation, "RESW") == 0)
                {
                    int long operandNum = 0;
                    
                    if(operand != NULL)
                    {
                        char * endptr;
                        int long operandNum = strtol(operand,&endptr, 10);//making the string into an int
                        locctr = locctr + (operandNum * 3);
                        
                        if(checkingIfItsANumber(operand) == 0)
                        {
                            errors[errorsItr] = 4;
                            errorsItr++;
                        }
                    }
                    else if(operand == NULL)
                    {
                        errors[errorsItr] = 4;
                        errorsItr++;
                    }
                    //                    locctr = locctr + (operandNum * 3);
                    opcde = -4;
                }
                else if(strcmp(operation, "BYTE") == 0)
                {
                    opcde = -5;
                    constantCntr();
                }
                else if(strcmp(operation, "LDA") == 0)
                {
                    //dont do nothing just leave it like that becauase LDA has the opcode 0
                }
                else if(strcmp(operation, "START") == 0){
                    opcde = -1;
                }
                else
                {
                    opcde = -6;
                    errors[errorsItr] = 3;
                    errorsItr++;
                }
            }
            fprintf(fp, "%x\n", opcde);
            fprintf(fp, "%s\n", operand);
            fprintf(fp, "%s\n", "errors: ");
            for(int i = 0; i < 20; ++i)
            {
                if(errors[i] != 0)
                {
                    fprintf(fp,"%d ", errors[i]);
                    
                }
            }
            //here I am clearing the errors array and setting the errorsItr back to 0
            clearingErrorsArray();
            errorsItr = 0;
            
            fputs("\n", fp);
            fputs("\n", fp);
            printf("%s", "\n");
            clearing(str);
            fgets(str, 180, fl);
            
            commentChecker = str[0];
            if(commentChecker == '.')
            {
                while(commentChecker == '.')
                {
                    fprintf(fp, "%s", str);
                    clearing(str);
                    fgets(str, 180, fl);
                    commentChecker = str[0];
                    
                }
            }
        }
    }
    if(swtch == 0)//this happens if theres no END directive
    {
        fputs("Error: missing END directive \n", fp);
        
    }
    if(locctr > 32768)//this happens if the program is too long
    {
        fputs("Error: 8\n", fp);
    }
    programLength  = locctr - startingAddress;
    fprintf(fp, "%s", "Program Length: ");
    fprintf(fp,"%lx",programLength);
    
    fclose(fp);//closing the intermediate file
    
    
}
//-------------------------------------------------------------------------------------------------------------------------------------------

//structs for Phase 3
struct errorStruct{
    
    int errorCode;
    char meaningOfCode[100];
    
};

//global variables for Phase 3
struct string units[600];
int unitsItr = 0;
long int lengthOfTextRec = 0;
char startStatement[100];
char endStatement[50];
char errorsForStartStatement[20];
long int errorsForStartStatementI[5];
char errorsForEndStatement[40];
int errorsInListingFileFlag = 0;
struct errorStruct errorCodeTable[8];


void errorTableMaker()
{
    //error 1
    struct errorStruct error1;
    strcpy(error1.meaningOfCode, "duplicate labels");
    error1.errorCode = 1;
    errorCodeTable[0] = error1;
    
    //error2
    struct errorStruct error2;
    strcpy(error2.meaningOfCode, "illegal label");
    error2.errorCode = 2;
    errorCodeTable[1] = error2;
    
    //error3
    struct errorStruct error3;
    strcpy(error3.meaningOfCode, "illegal operation");
    error3.errorCode = 3;
    errorCodeTable[2] = error3;
    
    //error4
    struct errorStruct error4;
    strcpy(error4.meaningOfCode, "missing or illegal operand on data storage directive");
    error4.errorCode = 4;
    errorCodeTable[3] = error4;
    
    //error5
    struct errorStruct error5;
    strcpy(error5.meaningOfCode, "missing or illegal operand on START directive");
    error5.errorCode = 5;
    errorCodeTable[4] = error5;
    
    //error6
    struct errorStruct error6;
    strcpy(error6.meaningOfCode, "missing or illegal operand on END directive");
    error6.errorCode = 6;
    errorCodeTable[5] = error6;
    
    //error7
    struct errorStruct error7;
    strcpy(error7.meaningOfCode, "too many symbols in source program");
    error7.errorCode = 7;
    errorCodeTable[6] = error7;
    
    //error8
    struct errorStruct error8;
    strcpy(error8.meaningOfCode, "program too long");
    error8.errorCode = 8;
    errorCodeTable[7] = error8;
    
}




void generalClearer(char * arrayToClear, int sizeOfArray){
    for(int i = 0; i < sizeOfArray; ++i){
        arrayToClear[i] = '\0';
    }
    
}

//this function checks for both lines where that have
//nothing on them and it also checks for lines that
//are comments. It return a 1 if the line is completely
//empty and it returns a 2 if the line is a comment line
int checkingIfLineEmpty(char * x)
{
    int answer = 0;
    
    if(x[0] == '\n'/*||x[0] == '.'*/){
        answer = 1; //this means yes
    }
    else if(x[0] == '.'){
        answer = 2; //this is going to mean its a comment
    }
    return answer;
}

int checkingForCommaX(char * a)
{
    int length = 0;
    int answer = -1; // a 1 means yes and a 0 means no and a -1 means its not working
    
    length = strlen(a);
    
    for(int i = 0; (i <= length); ++i){
        if(a[i] == '\n'){
            if(a[i-1] == 'X' && a[i-2] == ','){
                a[i-1] = '\0';
                a[i-2] = '\0';
                answer = 1;
            }
            else
                answer = 0;
        }
    }
    return answer;
}

int symbolTableSearcher(char * s)
{
    int address = 0;
    char stringWithoutNewLineChar[50];
    
    strcpy(stringWithoutNewLineChar, s);
    for(int i = 0; stringWithoutNewLineChar[i]; i++){
        if(stringWithoutNewLineChar[i] == '\n'){
            stringWithoutNewLineChar[i] = '\0';
        }
    }
    
    for(int i = 0; i < 600; i++){
        if(strcmp(symbolTable[i].name, stringWithoutNewLineChar) == 0){
            address = symbolTable[i].addressLocation;
        }
    }
    return address;
}

void integerToStringConverter(int a, char * b){
    sprintf(b, "%04x", a);
}

void strippingOff(char * a){
    for(int i = 0; a[i]; ++i){
        if(a[i] == '\n')
            a[i] = '\0';
    }
}

void universalStripper(char * array, int lengthOfArray)
{
    for(int i = 0; i < lengthOfArray; ++i){
        if(array[i] == '\n')
            array[i] = '\0';
    }
}

int checkingForDirective(char * x){
    int answer = 0;
    
    if(strcmp("ffffffff", x) == 0){
        answer = 1;//START
    }
    else if(strcmp("fffffffb",x) == 0){
        answer = 2;//BYTE
    }
    else if(strcmp("fffffffe", x) == 0){
        answer = 3;//WORD
    }
    else if(strcmp("fffffffc", x) == 0){
        answer = 4;//RESW
    }
    else if(strcmp("fffffffd", x) == 0){
        answer = 5;//RESB
    }
    
    return answer;
}

//looks for a specific character in a
//string and sets it to '\0'
void stringModifier(char * x, char a){
    for(int i = 0; i < 180; ++i){
        if(x[i] == a){
            x[i] = '\0';
        }
    }
}

void ByteDirectiveHandler(char * a, char * b){
    
    char hexNum[5];
    //this is when there are characters in the operand of BYTE
    if(a[0] == 'C'){
        stringModifier(a, 'C');
        stringModifier(a, '\'');
        for(int i = 0; i < 180; ++i){
            if(a[i] != '\0'){
                sprintf(hexNum, "%02x", (int)a[i]);
                strcat(b, hexNum);
            }
        }
    }
    //this is when the operand of BYTE is just a hex number
    else if(a[0] == 'X'){
        stringModifier(a, 'X');
        stringModifier(a, '\'');
        int j = 0;
        for(int i = 0; i < 180; ++i){
            if(a[i] != '\0'){
                b[j] = a[i];
                j++;
            }
        }
    }
}

void WordDirectiveHandler(char * lineReader, char * endStr){
    long int hexNum;
    
    hexNum = strtol(lineReader, NULL, 10);
    sprintf(endStr, "%06lx", hexNum);
    struct string instruction;
    strcpy(instruction.str, endStr);
    orderedInstructions[orderedInstItr] = instruction;
    orderedInstItr++;
}

int checkingForEnd(char * a){
    int answer = 0;
    char copy[180];
    strcpy(copy, a);
    char * token = NULL;
    
    token = strtok(a, " \t");
    if(strcmp(token, "END") == 0){
        answer = 1;
        strcpy(endStatement, copy);
    }
    
    
    return answer;
}

//this is so that all of the things in the units array have their machine code length set
//this function also takes off the new line char at the end of the source line and line addresses
//also gives all units an int line address
void settingTheMachineCodeLength()
{
    for(int i = 0; i < unitsItr; ++i){
        units[i].lengthOfMachineCode = strlen(units[i].machineCode);
    }
    //this is taking off the new line character from the line addresses
    for(int i = 0; i < unitsItr; ++i){
        for(int j = 0; units[i].address[j]; ++j){
            if(units[i].address[j] == '\n')
                units[i].address[j] = '\0';
        }
    }
    //this takes off the new line character from the source line
    for(int i = 0; i < unitsItr; ++i){
        for(int j = 0; units[i].sourceLine[j]; ++j){
            if(units[i].sourceLine[j] == '\n')
                units[i].sourceLine[j] = '\0';
        }
    }
    //giving all of the units an line address that is an int
    for(int i = 0; i < unitsItr; ++i){
        units[i].intAddress = strtol(units[i].address, NULL, 16);
    }
    
    
}


void copyingErrors(char * x)
{
    universalStripper(x, 180);
    char * error;
    
    error = strtok(x, " ");
    
}



void readingAndWritingFromIntermediate()
{
    char copier[100];
    int indexBit;
    char concatenatedStr[100];
    char arrayToChkEnd[180];
    char str2[180];
    FILE * fr;
    FILE * fw;
    int emptyLineChecker = -1;
    int address;
    struct string unit; //this is a struct that is going to contain machine code, copy of source line, address, and length of machine code
    int startStatementCounter = 0;
    struct string unitForComment;
    
    fr = fopen("intermediate2.txt", "r");
    fw = fopen("intermediateWriter.txt", "w");
    
    if((fr = fopen("intermediate2.txt", "r"))){
        
        fgets(str2,180,fr);
        
        emptyLineChecker = checkingIfLineEmpty(str2);
        if(emptyLineChecker == 1){
            while(emptyLineChecker /*!= 0*/ == 1 ){
                clearing(str2);
                fgets(str2,180,fr);
                emptyLineChecker = checkingIfLineEmpty(str2);
            }
        }
        if(emptyLineChecker == 2){
            while(emptyLineChecker == 2){
                strcpy(unitForComment.sourceLine, str2);
                strcpy(unitForComment.str, "comment");
                unitForComment.machineCode[0] = '\0';
                unitForComment.lengthOfMachineCode = 0;
                units[unitsItr] = unitForComment;
                unitsItr++;
                clearing(str2);
                fgets(str2,180,fr);
                emptyLineChecker = checkingIfLineEmpty(str2);
            }
        }
        
        while(!feof(fr))
        {
            strcpy(copier, str2);//this is to check for a double start statement error
            strcpy(unit.sourceLine, str2);//getting the source line into the unit
            
            //this is to check for the end statement so that I can break
            strcpy(arrayToChkEnd, str2);
            if(checkingForEnd(arrayToChkEnd) == 1)
                break;
            
            fgets(str2,180,fr);
            strcpy(unit.address, str2);//getting the address of the statement into the unit
            clearing(str2);
            
            fgets(str2,180,fr);//opcode
            strcpy(concatenatedStr, str2);//getting the opcode into concatenatedString
            strippingOff(concatenatedStr);
            
            //this is so that it handles the START directive
            if(checkingForDirective(concatenatedStr) == 1){
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                universalStripper(str2, 180);
                strcpy(errorsForStartStatement, str2);//made an array just to store the errors of the start statement
                fgets(str2,180,fr);
                strcpy(startStatement, copier);
                startStatementCounter++;
            }
            //this is to handle BYTE
            else if(checkingForDirective(concatenatedStr) == 2){
                clearing(str2);
                generalClearer(concatenatedStr, 100);
                fgets(str2,180,fr);
                strippingOff(str2);
                ByteDirectiveHandler(str2, concatenatedStr);
                printf("%s\n", concatenatedStr);
                struct string instruction;
                strcpy(instruction.str, concatenatedStr);
                
                strcpy(unit.machineCode, concatenatedStr);//getting the machine code into the unit for BYTE
                
                
                orderedInstructions[orderedInstItr] = instruction;
                orderedInstItr++;
                
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                
                universalStripper(str2, 180);//getting the errors from pass 1 into the unit
                strcpy(unit.errors, str2);
                units[unitsItr] = unit;//adding the unit into the array of units
                unitsItr++;
                
                
                fgets(str2,180,fr);
            }
            //this is to handle WORD
            else if(checkingForDirective(concatenatedStr) == 3){
                generalClearer(concatenatedStr, 100);
                clearing(str2);
                fgets(str2,180,fr);
                strippingOff(str2);
                WordDirectiveHandler(str2, concatenatedStr);
                
                strcpy(unit.machineCode, concatenatedStr);//getting the machine code into the unit for WORD
                
                printf("%s\n", concatenatedStr);
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                
                universalStripper(str2, 180);//getting the errors from pass 1 into the unit for WORD
                strcpy(unit.errors, str2);
                units[unitsItr] = unit;
                unitsItr++;
                
                fgets(str2,180,fr);
            }
            //this is to handle RESW and RESB
            else if(checkingForDirective(concatenatedStr) == 4 || checkingForDirective(concatenatedStr) == 5){
                generalClearer(concatenatedStr, 100);
                concatenatedStr[0] = ' ';
                struct string instruction;
                strcpy(instruction.str, concatenatedStr);
                
                strcpy(unit.machineCode, concatenatedStr);//getting the machine code into the unit for RESW and RESB
                
                orderedInstructions[orderedInstItr] = instruction;
                orderedInstItr++;
                printf("%s\n", concatenatedStr);
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                
                universalStripper(str2, 180);
                strcpy(unit.errors, str2);//getting the errors from pass 1 into the unit for RESW and RESB
                units[unitsItr] = unit;
                unitsItr++;
                
                
                fgets(str2,180,fr);
            }
            else{
                
                long int hexNum;
                hexNum = strtol(concatenatedStr, NULL, 16);
                generalClearer(concatenatedStr, 100);
                sprintf(concatenatedStr, "%02lx", hexNum);
                
                clearing(str2);
                
                fgets(str2,180,fr);//mnemonic name
                indexBit = checkingForCommaX(str2);//this will return a 1 if it does have a ,x and a 0 if it doesnt
                address = symbolTableSearcher(str2);
                if(indexBit == 1)
                    address = address + 32768;//here I am adding the index bit to the address
                char addressArray[10];
                integerToStringConverter(address, addressArray);
                
                
                fgets(str2,180,fr);
                fgets(str2,180,fr);
                
                universalStripper(str2, 180);
                strcpy(unit.errors, str2);//getting the errors from pass 1 into the unit for the general case
                
                fgets(str2,180,fr);
                
                strcat(concatenatedStr,addressArray);
                printf("%s\n", concatenatedStr);
                
                struct string instruction;
                strcpy(instruction.str, concatenatedStr);
                
                strcpy(unit.machineCode, concatenatedStr);//getting the machine code into the unit for general case
                units[unitsItr] = unit;
                unitsItr++;
                
                orderedInstructions[orderedInstItr] = instruction;
                orderedInstItr++;
            }
            
            
            
            
            emptyLineChecker = checkingIfLineEmpty(str2);
            if(emptyLineChecker == 1){
                while(emptyLineChecker /*!= 0*/ == 1){
                    clearing(str2);
                    fgets(str2,180,fr);
                    emptyLineChecker = checkingIfLineEmpty(str2);
                }
            }
            if(emptyLineChecker == 2){
                while(emptyLineChecker == 2){
                    strcpy(unitForComment.sourceLine, str2);
                    strcpy(unitForComment.str, "comment");
                    unitForComment.machineCode[0] = '\0';
                    unitForComment.lengthOfMachineCode = 0;
                    unitForComment.errors[0] = '\0';
                    units[unitsItr] = unitForComment;
                    unitsItr++;
                    clearing(str2);
                    fgets(str2,180,fr);
                    emptyLineChecker = checkingIfLineEmpty(str2);
                }
            }
        }
    }
    else{
        printf("%s", "not opened");
    }
    
    fclose(fr);
    fclose(fw);
    
}

//this function is going to take in a character array that is
//supposed to include the errors and if the the very
//first character in the character array is not a null that
//means that there are errors and its going to set the error
//flag variable to 1 so that the program knows not the run the
//object file maker function
void errorsInListing(char * str)
{
    if(str[0] != '\0'){
        errorsInListingFileFlag = 1;
    }
    
}

//this function is going to take the string where the errors
//are stated as numbers and going to translate them into
//actual integers and store them in an integer array inside of the unit
void codeToStatement(char * s, int indexOfUnit)
{
    int i = 0;
    char * token = strtok(s, " ");
    units[indexOfUnit].errorsInt[i] = strtol(token, NULL, 10);
    i++;
    units[indexOfUnit].errorsInt[i] = -1;
    token = strtok(NULL, " ");
    if(token == NULL)
        return;
    else{
        while(token != NULL){
            units[indexOfUnit].errorsInt[i] = strtol(token, NULL, 10);
            i++;
            token = strtok(NULL, " ");
        }
        units[indexOfUnit].errorsInt[i] = -1;
    }
}

void charToInt(char * s)
{
    int i = 0;
    char * token = strtok(s, " ");
    errorsForStartStatementI[i] = strtol(token, NULL, 10);
    i++;
    errorsForStartStatementI[i] = -1;
    token = strtok(NULL, " ");
    if(token == NULL)
        return;
    else{
        while(token != NULL){
            errorsForStartStatementI[i] = strtol(token, NULL, 10);
            i++;
            token = strtok(NULL, " ");
        }
        errorsForStartStatementI[i] = -1;
    }
    
}

void makingTheListingFile()
{
    FILE * writer;
    writer = fopen("listingFile.txt", "w");
    
    
    fprintf(writer, "                                   %s", startStatement); //*all of this is just for the start statement
    if(errorsForStartStatement[0] != '\0'){
        fprintf(writer, "%s", "Errors: ");
        charToInt(errorsForStartStatement);
        for(int i = 0; errorsForStartStatementI[i] != -1; ++i){
            fprintf(writer, "%s", errorCodeTable[errorsForStartStatementI[i]-1].meaningOfCode);
        }
    }
    errorsInListing(errorsForStartStatement);
    fputs("\n", writer);                                                        //*
    fputs("\n", writer);                                                        //*
    for(int i = 0; i < unitsItr; ++i){
        if(strcmp(units[i].str, "comment") == 0){
            fprintf(writer,"%s\n", units[i].sourceLine);
        }
        else{
            fprintf(writer, "%s         %s              %s", units[i].address, units[i].machineCode, units[i].sourceLine);
            fprintf(writer, "%s", "\n");
            
            if(units[i].errors[0] != '\0'){
                codeToStatement(units[i].errors, i);
                fprintf(writer, "%s", "Errors: ");
                for(int j = 0; units[i].errorsInt[j] != -1; ++j){
                    fprintf(writer, "%s ", errorCodeTable[units[i].errorsInt[j] - 1].meaningOfCode);
                }
            }
            errorsInListing(units[i].errors);
            fprintf(writer, "%s", "\n");
            fprintf(writer, "%s", "\n");
            fprintf(writer, "%s", "\n");
        }
    }
    
    
    fprintf(writer, "%s", endStatement);
    
    fclose(writer);
    
}
//this function is to figure out from what index to what index
//in my units array I am going to look at
int figuringOutIndexes(int * a, int index)
{
    int updatedIndex = 0;
    int i = index;
    int itrOfA = 0;
    
    while((lengthOfTextRec + (units[i].lengthOfMachineCode) <= 60) && i < unitsItr ){
        if(strcmp(units[i].machineCode, " ") != 0 && strcmp(units[i].str, "comment") != 0){
            lengthOfTextRec = lengthOfTextRec + units[i].lengthOfMachineCode;
            a[itrOfA] = i;
            i++;
            itrOfA++;
        }
        else if(strcmp(units[i].machineCode, " ") == 0){
            while(strcmp(units[i].machineCode, " ") == 0){
                i++;
            }
            updatedIndex = i;
            return updatedIndex;
            
        }
        else if(strcmp(units[i].str, "comment") == 0){
            i++;
        }
    }
    
    
    updatedIndex = i;
    return updatedIndex;
    
}

void intGeneralClearer(int * a, int sizeOfArray){
    for(int i = 0; i < sizeOfArray; ++i){
        a[i] = -1;
    }
}

//this function is going to be to pad the name part of the header
//record in the object file with spaces if the length of the
//name is not 6
//
//although the length of the character array that I created is 7
//I made the for loop so that it only looks up until 6 so that
//it doesnt change the null character that I have at the end
//because that null character is necessary there
void padder(char * str)
{
    for(int i = 0; i < 6; ++i){
        if(str[i] == '\0'){
            str[i] = ' ';
        }
    }
}


void makingTheObjectFile()
{
    FILE * write;
    FILE * read;
    char nameHeader[7];
    char str3[180];
    long int progLen = 0;
    int indexes[15];
    int currentIndex = 0;
    
    
    write = fopen("objectFile.txt", "w");
    read = fopen("intermediate2.txt", "r");
    
    fgets(str3,180, read);
    char buffer[180];
    strcpy(buffer, str3);
    char * token = strtok(buffer, " \t");//name section in the header record
    strcpy(nameHeader, token);
    padder(nameHeader);
    
    //this is getting the load address for the header file
    clearing(str3);
    fgets(str3,180, read);
    long int loadAddr;
    loadAddr = strtol(str3, NULL,10);
    
    //this gets the program length for the header file
    while(!feof(read)){
        fgets(str3,180, read);
        if(checkingForEnd(str3) == 1){
            fgets(str3,180, read);
            char * tkn = strtok(str3," ");
            tkn = strtok(NULL, " ");
            tkn = strtok(NULL, " ");
            progLen = strtol(tkn, NULL, 16);
        }
    }
    fprintf(write, "H%s%06ld%06lx\n", nameHeader, loadAddr, progLen);
    
    
    
    intGeneralClearer(indexes, 15);
    
    while(currentIndex < unitsItr){
        currentIndex = figuringOutIndexes(indexes, currentIndex);
        fputs("T", write);
        fprintf(write, "%06lx", units[indexes[0]].intAddress);
        fprintf(write, "%02lx", lengthOfTextRec/2);
        for(int i = 0; indexes[i] != -1; ++i){
            fprintf(write, "%s", units[indexes[i]].machineCode);
        }
        fputs("\n", write);
        intGeneralClearer(indexes, 15);
        lengthOfTextRec = 0;
    }
    fprintf(write, "E%06ld", loadAddr);
    
    fclose(write);
    fclose(read);
}



//int main()
//{
//    //these three function calls have to do with phase 2
//    optableMaker();
//    readingFile();
//    //    symbolTablePrinter();
//    readingAndWritingFromIntermediate();
//    //    printing();
//    errorTableMaker();
//    settingTheMachineCodeLength();
//    makingTheListingFile();
//    if(errorsInListingFileFlag == 1){
//        return 0;
//    }
//    makingTheObjectFile();
//    
//    return 0;
//    
//    
//}

