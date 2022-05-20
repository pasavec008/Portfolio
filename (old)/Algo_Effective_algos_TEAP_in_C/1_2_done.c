/*
Skupina Iron Maiden po koncerte na Topfeste sa vracia naspäť domov do Londýna. Teraz sa potrebujú zbaliť.
Majú veľa vecí - kostýmy, aparatúru, gitary, bicie, pareničky a korbáčiky,...
K dispozícii majú ľubovoľný počet kamiónov, no keďže zaplatenie každého kamiónu niečo stojí, chcú minimalizovať celkové náklady.
Každý kamión má nosnosť 300 krabíc. Keďže chcú mať vo veciach poriadok, balia ich spolu do väčších balíkov, každý pojme 101 až 300 krabíc.
Tvojou úlohou je zistiť koľko najmenej kamiónov potrebujú na úspešné prepravenie vecí (zabalených vo väčších balíkoch) domov.

Každý riadok obsahuje celé číslo N predstavujúce počet väčších balíkov do ktorých sú zabalené veci, a N čísel x[i], kde x[i] je počet krabíc zabalených v i-tom balíku.
Pre každý riadok vypíšte jedno číslo - minimálny počet kamiónov, ktoré musíme zaplatiť na prevezenie všetkých krabíc (ktoré sú balené vo väčších balíkoch).

Obmedzenia:
1 <= N <= 50,
101 <= x[i] <= 300

Ukážka vstupu:
5 150 150 150 150 150
9 101 101 101 101 101 101 101 101 101
Výstup pre ukážkový vstup:
3
5
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    int kolko_balikov;
    while(scanf("%d", &kolko_balikov) > 0){
        int counting_sort_array[301] = {0}, *baliky = (int *)malloc(kolko_balikov * sizeof(int)), *baliky_new = (int *)malloc(kolko_balikov * sizeof(int)), cervene = 0, zelene = 0, zlte = 0;

        for(int i = 0; i < kolko_balikov; i++){
            scanf("%d", &(baliky[i]));
            if(baliky[i] > 199)
                cervene++;
            else if(baliky[i] < 151)
                zelene++;
            else
                zlte++;

            counting_sort_array[baliky[i]]++;
        }

        int sucet = 0;
        for(int i = 0; i < 301; i++){
            sucet += counting_sort_array[i];
            counting_sort_array[i] = sucet;
        }

        for(int i = 0; i < kolko_balikov; i++){
            baliky_new[(counting_sort_array[baliky[i]]--) - 1] = baliky[i];
        }

        int pocet = kolko_balikov;
        int start = 0;  //tu zacinaju zelene
        int koniec = zelene + zlte - 1; //tu koncia zlte

        while(start < zelene && start < koniec){
            if(baliky_new[start] + baliky_new[koniec] <= 300){
                pocet--;
                start++;
                koniec--;
            }
            else
                koniec--;
        }
        


        printf("%d\n", pocet);
        free(baliky);
        free(baliky_new);
    }

    return 0;
}