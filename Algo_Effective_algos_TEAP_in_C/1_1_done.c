// uloha1-1.c -- Marek Adamovič, 17.2.2021 09:15

/*
Humanitárny konvoj sa presúva na Ukrajinu do hlavného mesta Kyjev, aby pomohol tunajším ľuďom.
Ako jeden z organizátorov si sa dozvedel, že konvoj má prechádzať cez most s nosnosťou N. Vieš, že na most sa nezmestí viac ako K vozidiel súčasne.
Zároveň poznáš hmotnosť každého vozidla. (Naopak nevieš vopred povedať akým spôsobom budú vozidlá cez most prechádzať).
Tvojou úlohou je zistiť, či je prejazd cez most pre konvoj bezpečný, alebo nie.

Na vstupe je niekoľko riadkov, každý obsahuje čísla N, K, M a M čísel h[i], ktoré určujú hmotnosť vozidiel konvoja.
Pre každý riadok na vstupe vypíšte Ano ak konvoj môže bezpečne prejsť cez most bez ohľadu nato v akom poradí budú vozidlá cez most prechádzať, inak vypíšte Nie

Obmedzenia:
1 <= N <= 10000,
1 <= K <= M,
1 <= M <= 9,
1 <= h[i] <= 1000

Ukážka vstupu:
1234 3 4 1000 200 4 30
20 4 5 10 10 10 10 10
Výstup pre ukážkový vstup:
Ano
Nie
*/

#include <stdio.h>
#include <stdlib.h>

int porovnaj(const void *x, const void *y){
    int x_cislo = *((int *)x);
    int y_cislo = *((int *)y);

    if(x_cislo > y_cislo)
        return -1;
    if(x_cislo < y_cislo)
        return 1;
    return 0;
}

int main(){
    int nosnost, mesti_sa, pocet_aut;
    while(scanf("%d %d %d", &nosnost, &mesti_sa, &pocet_aut) > 0){
        int *auta = (int *)malloc(pocet_aut * sizeof(int));
        for(int i = 0; i < pocet_aut; i++)
            scanf("%d", &auta[i]);

        qsort (auta, pocet_aut, sizeof(int), porovnaj);

        int kontrolna_nosnost = 0;
        for(int i = 0; i < mesti_sa; i++)
            kontrolna_nosnost += auta[i];

        if(kontrolna_nosnost <= nosnost)
            printf("Ano\n");
        else
            printf("Nie\n");

        free(auta);
    }

    return 0;
}