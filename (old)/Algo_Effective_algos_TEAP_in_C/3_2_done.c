// uloha3-2.c -- Marek Adamovič, 2.3.2021 08:18

/*
Malý Jakubko sa hral s binárnymi číslami. Dané je číslo v binárnom zápise, vašou úlohou je zistiť na koľko najmenej častí je potrebné ho rozdeliť tak,
aby každá zo vzniknutých častí predstavovala binárny zápis nejakej mocniny čísla 5 (napr. 5, 25, 125) bez núl na začiatku.

Na štandardnom vstupe je viac riadkov, na každom z nich je číslo zapísané v binárnom zápise, ktorého dĺžka je najviac 50 číslic.
Pre každé číslo na vstupe postupne vypíšte jeden riadok na výstup, obsahujúci jedno číslo: najmenší počet časti na ktorý je potrebné zadané číslo rozdeliť tak,
aby vzniknuté časti každá predstavovala binárny zápis nejakej mocniny čísla 5, resp. vypíšte -1 ak sa to nedá tak rozdeliť.

Ukážka vstupu:
101101101
0000
Výstup pre ukážkový vstup:
3
-1
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void napln(char nasobky[22][60], int *velkost_nasobkov){
    sprintf(nasobky[0], "%s", "1");
    sprintf(nasobky[1], "%s", "101");
    sprintf(nasobky[2], "%s", "11001");
    sprintf(nasobky[3], "%s", "1111101");
    sprintf(nasobky[4], "%s", "‭1001110001‬");
    sprintf(nasobky[4], "%s", nasobky[4] + 3);
    sprintf(nasobky[5], "%s", "‭110000110101");
    sprintf(nasobky[5], "%s", nasobky[5] + 3);
    sprintf(nasobky[6], "%s", "‭11110100001001‬");
    sprintf(nasobky[6], "%s", nasobky[6] + 3);
    sprintf(nasobky[7], "%s", "10011000100101101‬");
    sprintf(nasobky[8], "%s", "1011111010111100001‬");
    sprintf(nasobky[9], "%s", "111011100110101100101‬");
    sprintf(nasobky[10], "%s", "100101010000001011111001‬");
    sprintf(nasobky[11], "%s", "10111010010000111011011101‬");
    sprintf(nasobky[12], "%s", "1110100011010100101001010001‬");
    sprintf(nasobky[13], "%s", "1001000110000100111001110010101‬");
    sprintf(nasobky[14], "%s", "101101011110011000100000111101001‬");
    sprintf(nasobky[15], "%s", "11100011010111111010100100110001101‬");
    sprintf(nasobky[16], "%s", "10001110000110111100100110111111000001‬");
    sprintf(nasobky[17], "%s", "1011000110100010101111000010111011000101‬");
    sprintf(nasobky[18], "%s", "110111100000101101101011001110100111011001‬");
    sprintf(nasobky[19], "%s", "100010101100011100100011000001001000100111101‬");
    sprintf(nasobky[20], "%s", "10101101011110001110101111000101101011000110001‬");
    sprintf(nasobky[21], "%s", "1101100011010111001001101011011100010111011110101‬");

    for(int i = 0; i < 22; i++){
        int ii = 0;
        //printf("%s\n", nasobky[i]);

        while(nasobky[i][ii] == '0' || nasobky[i][ii] == '1')
            ii++;
        nasobky[i][ii] = '\0';
        //printf("%s\n", nasobky[i]);
    }

    velkost_nasobkov[0] = 1;
    velkost_nasobkov[1] = 3;
    velkost_nasobkov[2] = 5;
    velkost_nasobkov[3] = 7;
    velkost_nasobkov[4] = 10;
    velkost_nasobkov[5] = 12;
    velkost_nasobkov[6] = 14;
    velkost_nasobkov[7] = 17;
    velkost_nasobkov[8] = 19;
    velkost_nasobkov[9] = 21;
    velkost_nasobkov[10] = 24;
    velkost_nasobkov[11] = 26;
    velkost_nasobkov[12] = 28;
    velkost_nasobkov[13] = 31;
    velkost_nasobkov[14] = 33;
    velkost_nasobkov[15] = 35;
    velkost_nasobkov[16] = 38;
    velkost_nasobkov[17] = 40;
    velkost_nasobkov[18] = 42;
    velkost_nasobkov[19] = 45;
    velkost_nasobkov[20] = 47;
    velkost_nasobkov[21] = 49;
}

int main(){

    char nasobky[22][60] = {0};
    int velkost_nasobkov[22], pokus;
    napln(nasobky, velkost_nasobkov);
    char cislo[110];

    while(scanf("%s", cislo) > 0){
        pokus = 0;
        while(cislo[0] != '1' && cislo[0] != '0')
            sprintf(cislo, "%s", cislo + 1);
        while(cislo[pokus] == '0' || cislo[pokus] == '1'){
            pokus++;
        }
        cislo[pokus] = '\0';
        int velkost_cisla = pokus, *aktualny_stav = (int *)malloc(sizeof(int) * velkost_cisla);
        char *pomocna;

        for(int i = 0; i < velkost_cisla; i++)
            aktualny_stav[i] = 0;
      
        //printf("%d ", velkost_cisla);
        for(int i = 0; i < velkost_cisla; i++){
            if(i != 0 && aktualny_stav[i - 1] == 0)
                continue;
            for(int ii = 21; ii >= 0; ii--){
                pomocna = strstr(cislo + i, nasobky[ii]);
                //printf("hladam v %s hodnotu %s, nasiel som %d co porovnam s %d\n",cislo+i, nasobky[ii], (int)pomocna, (int)(cislo + i));
                
                /*
                for(int k = 0; k < velkost_cisla; k++)
                    printf("%d", aktualny_stav[k]);
                printf("\n");
                */
                
                if(pomocna && pomocna == cislo + i){
                    if(aktualny_stav[i + velkost_nasobkov[ii] - 1] > aktualny_stav[i-1] + 1 || aktualny_stav[i + velkost_nasobkov[ii] - 1] == 0){
                        aktualny_stav[i + velkost_nasobkov[ii] - 1] = aktualny_stav[i-1] + 1;
                    }
                }
            }
        }

        /*
        for(int i = 0; i < velkost_cisla; i++)
            printf("%d", aktualny_stav[i]);
        printf("\n");
        printf("cislo %s vysledok %d\n", cislo,aktualny_stav[velkost_cisla - 1]);
        */

       
        if(aktualny_stav[velkost_cisla - 1] == 0)
            printf("-1\n");
        else
            printf("%d\n", aktualny_stav[velkost_cisla - 1]);
        free(aktualny_stav);
        
    }

    return 0;
}