/*
Jakubko oslavuje narodeniny a na oslavu chce pozvať všetkých svojich kamarátov zo všetkých krúžkov, ktoré navštevuje.
Kamarátov chce usadiť k stolom tak, aby sa nestalo, že pri jednom stole sedia dvaja (alebo viacerí) kamaráti z rovnakého krúžku.
Pomôžte Jakubkovi rozhodnúť, či také usadenie všetkých kamarátov k stolom existuje.

Na štandardnom vstupe bude viacero scenárov za sebou. Každý scenár obsahuje najskôr dve čísla N (1 <= N <= 100) počet krúžkov,
ktoré Jakubko navštevuje, a M (1 <= M <= 100) počet stolov, ktoré má Jakubko na oslave k dispozícií.
Nasleduje N čísel Ni (1 <= Ni <= 100) počet kamarátov z i-teho krúžku, a M čísel Mj (2 <= Mj <= 100) počet miest pri j-tom stole. Jakubko pri stole nesedí.

Pre každý scenár na štandardný výstup vypíšte 1, ak také rozsadenie všetkých kamarátov k stolom existuje, inak vypíšte 0.

Ukážka vstupu:
4 5 
4 5 3 5 
3 5 2 6 4 
4 5 
4 5 3 5 
3 5 2 6 3
Výstup pre ukážkový vstup:
1
0
*/

#include <stdio.h>
#include <stdlib.h>

int compare_function_r(const void * x, const void * y){
   return (*(int*)y - *(int*)x);
}

int organizacia(int *kruzky, int *stoly, int pocet_kruzkov, int pocet_stolov){
    for(int i = 0; i < pocet_kruzkov; i++){
        for(int k = 0; k < kruzky[i]; k++){
            if(k >= pocet_stolov || stoly[k] == 0)
                return 0;
            stoly[k]--;
        }
        qsort(stoly, pocet_stolov, sizeof(int), compare_function_r);
    }
    return 1;
}

int main(){
    int pocet_stolov, pocet_kruzkov;


    while(scanf("%d %d", &pocet_kruzkov, &pocet_stolov) == 2){
        int *kruzky = (int *)malloc(pocet_kruzkov * sizeof(int)), *stoly = (int *)malloc(pocet_stolov * sizeof(int));

        for(int i = 0; i < pocet_kruzkov; i++)
            scanf("%d", &kruzky[i]);
        for(int i = 0; i < pocet_stolov; i++)
            scanf("%d", &stoly[i]);

        qsort(kruzky, pocet_kruzkov, sizeof(int), compare_function_r);
        qsort(stoly, pocet_stolov, sizeof(int), compare_function_r);

        printf("%d\n", organizacia(kruzky, stoly, pocet_kruzkov, pocet_stolov));
        

        free(kruzky);
        free(stoly);
    }
    

    return 0;
}