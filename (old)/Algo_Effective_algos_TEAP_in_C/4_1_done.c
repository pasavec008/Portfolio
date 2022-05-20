/*
Keď minule Jakubko nemohol zaspať, zobral si papier a začal si kresliť kopčeky.

Prvý riadok vstupu obsahuje číslo N (1 ≤ N ≤ 20) vyjadrujúce počet úloh. Každá úloha pozostáva z riadku obsahujúceho dve celé čísla D (2 ≤ D ≤ 5000) a V (1 ≤ V ≤ 200).

Výstup bude obsahovať N riadkov, pre každú úlohu jeden. Obsahovať budú počet rôznych kopcov majúcich šírku D metrov a výšku presne V metrov.
Kopec pozostáva len z troch možných úsekov: stúpanie široké 1 meter a vysoké 1 meter, klesanie široké 1 meter a vysoké 1 meter a rovinka široká 1 meter.
Jednotlivé úseky musia na seba nadväzovať, výška nesmie presiahnuť V metrov, na začiatku a na konci musí mať kopec nulovú výšku
a nikde inde nesmie byť výška ≤ 0 (takže kopec napríklad nemôže začínať alebo končiť rovinkou).

Keďže takých kopcov môže byť veľmi veľa, vypíšte počet kopcov ako zvyšok po delení číslom 100 000 007.

Ukážka vstupu:
2
6 2
30 5
Výstup pre ukážkový vstup:
7
11640085
*/

#include <stdio.h>
#include <stdlib.h>

int kopceky(int **tabulka, int aktualna_pozicia, int aktualna_vyska, int d, int v){
    while(1){
        //ak som dole
        if(aktualna_vyska == 0){
            //ak som v strede
            if(aktualna_pozicia != 0 && aktualna_pozicia != d){
                tabulka[aktualna_pozicia][aktualna_vyska] = 0;
                aktualna_vyska++;
                continue;
            }
            //ak som na zaciatku
            else if(aktualna_pozicia == 0){
                tabulka[aktualna_pozicia][aktualna_vyska] = 1;
                tabulka[aktualna_pozicia + 1] = (int *)calloc(v + 1, sizeof(int));
                if(aktualna_pozicia > 1)
                    free(tabulka[aktualna_pozicia - 2]);
                aktualna_pozicia++;
                continue;
            }
            //ak som na konci
            else{
                tabulka[aktualna_pozicia][aktualna_vyska] = tabulka[aktualna_pozicia - 1][1];
                return tabulka[aktualna_pozicia][aktualna_vyska];
            }
                
        }
        //ak som hore alebo na diagonalach
        else if(aktualna_vyska == v || aktualna_pozicia == aktualna_vyska){
            tabulka[aktualna_pozicia][aktualna_vyska] = (tabulka[aktualna_pozicia - 1][aktualna_vyska] + tabulka[aktualna_pozicia - 1][aktualna_vyska - 1]) % 100000007;
            tabulka[aktualna_pozicia + 1] = (int *)calloc(v + 1, sizeof(int));
            if(aktualna_pozicia > 1)
                free(tabulka[aktualna_pozicia - 2]);
            aktualna_vyska = 0;
            aktualna_pozicia++;
            continue;
        }
        else if(aktualna_pozicia + aktualna_vyska == d){
            if(aktualna_vyska == v)
                tabulka[aktualna_pozicia][aktualna_vyska] = (tabulka[aktualna_pozicia - 1][aktualna_vyska] + tabulka[aktualna_pozicia - 1][aktualna_vyska - 1]) % 100000007;
            else
                tabulka[aktualna_pozicia][aktualna_vyska] = (tabulka[aktualna_pozicia - 1][aktualna_vyska + 1] + tabulka[aktualna_pozicia - 1][aktualna_vyska] + tabulka[aktualna_pozicia - 1][aktualna_vyska - 1]) % 100000007;
            tabulka[aktualna_pozicia + 1] = (int *)calloc(v + 1, sizeof(int));
            if(aktualna_pozicia > 1)
                free(tabulka[aktualna_pozicia - 2]);
            aktualna_vyska = 0;
            aktualna_pozicia++;
            continue;
        }
        //ak som v strede
        else{
            tabulka[aktualna_pozicia][aktualna_vyska] = (tabulka[aktualna_pozicia - 1][aktualna_vyska + 1] + tabulka[aktualna_pozicia - 1][aktualna_vyska] + tabulka[aktualna_pozicia - 1][aktualna_vyska - 1]) % 100000007;
            aktualna_vyska++;
            continue;
        }
    }
    
}

int main(){
    int pocet_uloh, d, v;
    scanf("%d", &pocet_uloh);

    for(int i = 0; i < pocet_uloh; i++){
        scanf("%d %d", &d, &v);

        if(d < 2*v){
            printf("0\n");
            continue;
        }
        else if(d == 2*v || v == 1){
            printf("1\n");
            continue;
        }

        int **tabulka = (int **)malloc((d + 1) * sizeof(int *)), vysledok1, vysledok2;
        tabulka[0] = (int *)calloc(v + 1, sizeof(int));
        vysledok1 = kopceky(tabulka, 0, 0, d, v);
        free(tabulka[d]);
        free(tabulka[d - 1]);

        tabulka[0] = (int *)calloc(v + 1, sizeof(int));
        vysledok2 = kopceky(tabulka, 0, 0, d, v - 1);
        free(tabulka[d]);
        free(tabulka[d - 1]);
        free(tabulka);

        printf("%d\n", (vysledok1 - vysledok2 + 100000007) % 100000007);
    }
    
    return 0;
}