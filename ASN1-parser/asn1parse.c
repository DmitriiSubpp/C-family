#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*

asn1 type            der

SEQUENCE              30
INTEGER               02
OCTETSTRING           04
NULL                  05
OBJECT IDENTIFIER     06
------------------------
02 03 01 00 01 = INTEGER 65537

02 - type
03 -block lengh
01 00 01 hexadecimal of decimal 65337

*/



int hex_to_dec(char *hex){    // функция переводит один октет в десятичное число
long int value = strtol(hex, NULL , 16);
return value;
}


int degree(int a, int b) // функция считает степень
{
    int n =1;
    for (int i=0; i<b ; i++) {n*=a;}
    return n;
}


int len_count(char *hex_str){ // функция переводит строку октетов в десятичное число
    char s[strlen(hex_str)*4];
    for (int i=0; i<strlen(hex_str); i++)
        {
        switch (hex_str[i]){
            case '0': strcat(s, "0000"); break;
            case '1': strcat(s, "0001"); break;
            case '2': strcat(s, "0010"); break;
            case '3': strcat(s, "0011"); break;
            case '4': strcat(s, "0100"); break;
            case '5': strcat(s, "0101"); break;
            case '6': strcat(s, "0110"); break;
            case '7': strcat(s, "0111"); break;
            case '8': strcat(s, "1000"); break;
            case '9': strcat(s, "1001"); break;
            case 'A': strcat(s, "1010"); break;
            case 'B': strcat(s, "1011"); break;
            case 'C': strcat(s, "1100"); break;
            case 'D': strcat(s, "1101"); break;
            case 'E': strcat(s, "1110"); break;
            case 'F': strcat(s, "1111"); break;
        }
    }
    int sum=0; // десятичное число которое получится из двоичного
    int n=-1;
        for (int i=strlen(s);i>0;i--){
            if (s[i]=='1') sum+=degree(2,n) ;
            n++;
        }
    return sum;
}

int get_value(char *out, int block_len, char*block, int octet_index){ // функция парсит значение переданного ей типа

    FILE *file = fopen(out, "a");
    fprintf(file,":");
    int ignore = block_len + octet_index;
    for (int i=0 ; i < block_len;i++)
    {
        fprintf(file, "%c",block[octet_index]);
        if (octet_index == ignore) break;
        octet_index++;
    }
    fclose(file);
    return octet_index-2;
}

int get_block(char *out, int i,char *len,char *hex_str, char *type){ // функция считает длину блока и выводит информацию о нем
    int bLen=0;

    FILE *file = fopen(out, "a");
    fprintf(file,"\n");
    if (len[0]=='8' && len[1]=='2'){
                len[0]=hex_str[i+4];
                len[1]=hex_str[i+5];
                len[2]=hex_str[i+6];
                len[3]=hex_str[i+7];
                bLen = len_count(len);
                fprintf(file, "len = %-5d  %s\t\t", bLen, type);
                fclose(file);
                if (type[0] != 'S') i = get_value(out, bLen*2, hex_str, i+8);
                else i += 6;
                return i;
    }

    else if (len[0]=='8' && len[1]=='1'){
                len[0]=hex_str[i+4];
                len[1]=hex_str[i+5];
                bLen = len_count(len);
                fprintf(file, "len = %-5d  %s\t\t", bLen, type);
                fclose(file);
                if (type[0] != 'S') i = get_value(out, bLen*2, hex_str, i+6);
                else i += 4;
                return i;
    }

    else {
                bLen = len_count(len);
                fprintf(file, "len = %-5d  %s\t\t", bLen, type);
                fclose(file);
                if (type[0] != 'S') i = get_value(out, bLen*2, hex_str, i+4);
                else i += 2;
                return i;
    }

}


void parse(char *out, char *hex_str){ // функция находит типы

    static char current_octet[2];             // массив для хранения октета
    static char len[4];                       // массив для хранения длины блока

    for (int i=0; i<strlen(hex_str) ; i+=2){  // цикл перебирает октеты в hex_str
        current_octet[0]=hex_str[i];
        current_octet[1]=hex_str[i+1];
        len[0]=hex_str[i+2];
        len[1]=hex_str[i+3];

//---------------------------------------------------------------------------------------------------
// обрабатываем SEQUENCE
        if (current_octet[0]== '3' && current_octet[1]== '0')
                i = get_block(out, i, len, hex_str, "SEQUENCE");
//---------------------------------------------------------------------------------------------------
// обрабатываем INTEGER
        else if (current_octet[0]== '0' && current_octet[1]== '2')
                i = get_block(out,i, len, hex_str, "INTEGER");
//---------------------------------------------------------------------------------------------------
// обрабатываем OBJECTIDENTIFIER
        else if (current_octet[0]== '0' && current_octet[1]== '6')
                i = get_block(out,i, len, hex_str, "OBJECTIDENTIFIER");
//---------------------------------------------------------------------------------------------------
// обрабатываем OCTETSTRING
        else if (current_octet[0]== '0' && current_octet[1]== '4')
                i = get_block(out,i, len, hex_str, "OCTETSTRING");
//---------------------------------------------------------------------------------------------------
// обрабатываем NULL
        else if (current_octet[0]== '0' && current_octet[1]== '5')
                printf("NULL\n\n");
//---------------------------------------------------------------------------------------------------
        memset(len, 0, sizeof(len)); //отчистка len
        for (int i = 0; i<4; i++) len[i]='\0'; //отчистка len
    }
    printf("sucsesfully\n");
}

//---------------------------------------------------------------------------------------------------
// переводим base64 в hex

int find(char c){ // поиск символа и возвращение его индекса
    char base64Table[] = {  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
                            'P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d',
                            'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s',
                            't','u','v','w','x','y','z','0','1','2','3','4','5','6','7',
                            '8','9','+','/','=' };
    int i =0;
    for(; i<strlen(base64Table); i++) if (base64Table[i] == c ) break;
    return i;
}

char* getHex(char *base64, char *hex_str){

    int dec;        // три байта в десятичном представлении
    char hex[20];   // три байта в шеснадцатиричном представлении

    for (int i=0; i<strlen(base64); i+=4){

        if (strlen(hex_str)>=900) hex_str = (char*) realloc(hex_str, 1000 * sizeof(char*));

        dec = ((find(base64[i])<<18) + (find(base64[i+1])<<12) + (find(base64[i+2])<<6) + (find(base64[i+3]))) ;

        sprintf(hex, "%X", dec);
// общее кол-во цифр должно быть 6, поэтому необходимо дописать нули в начало, если есть такая необходимость
             if (strlen(hex) == 5) sprintf(hex, "0%X", dec);
        else if (strlen(hex) == 4) sprintf(hex, "00%X", dec);
        else if (strlen(hex) == 3) sprintf(hex, "000%X", dec);
        else if (strlen(hex) == 2) sprintf(hex, "0000%X", dec);
        else if (strlen(hex) == 1) sprintf(hex, "00000%X", dec);

        strcat(hex_str, hex);
    }
// удаляет лишние байты
    if (base64[strlen(base64)-1] == '=' && base64[strlen(base64)-2] != '=') hex_str[strlen(hex_str)-2] = '\0';
    if (base64[strlen(base64)-1] == '=' && base64[strlen(base64)-2] == '=') hex_str[strlen(hex_str)-4] = '\0';

    return hex_str;
}

char *getKey(char *in, char *key){
    int c;
    int i=0;

    FILE *file = fopen(in, "r");


            fgets(key, 64, file);               // пропускаем строку -----BEGIN RSA PRIVATE KEY-----
            while ((c=fgetc(file)) != EOF){     // считываем посимвольно до -----END RSA PRIVATE KEY-----, пропуская '\n'
                if (strlen(key)>1000) key = (char*) realloc(key, 1000 * sizeof(char*));
                if (c != '\n') {key[i] = c; i++;}
                if (c == '-') {key[i-1] = '\0'; break;}
            }

            fclose(file);
            return key;


}

//---------------------------------------------------------------------------------------------------
int main(int argc, char**argv){

    char *in, *out;
    in = argv[1];
    out = argv[2];

    char *key = (char*) malloc(1000 * sizeof(char));
    char *hex = (char*) malloc(1000 * sizeof(char));

    if (in) {
            FILE *file = fopen(in, "r");
            if (file == NULL) {printf("File not found\n"); fclose(file);}
            else {
                key = getKey(in, key);
                hex = getHex(key, hex);
                if (out) parse(out, hex);
                else parse("out.txt", hex);
                fclose(file);
            }
    }
    else printf("Needs exactly 1 argument\n");

    free(key);
    free(hex);
    return 0;
}

