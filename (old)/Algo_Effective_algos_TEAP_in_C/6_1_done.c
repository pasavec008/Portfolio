/*
Jakubko sa pripravuje na programátorský test z rekurzie. Prechádza si staršie úlohy a snaží sa ich vyriešiť rekurzívne.
Našiel úlohu na určenie všetkých N prvkových variácií s opakovaním z K-prvkovej množiny.

Na vstupe sú prirodzené čísla N a K, napíšte rekurzívny program, ktorý na výstup vypíše všetky N prvkové variácie s opakovaním z K-prvkovej množiny čísel 1, 2, ..., K.

Ukážka vstupu:
3 2
Výstup pre ukážkový vstup:
1 1 1 
1 1 2 
1 2 1 
1 2 2 
2 1 1 
2 1 2 
2 2 1 
2 2 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void variacie(int n, int k, int level, int *numbers){
    if(level == n){
        for(int i = 0; i < n; i++)
            printf("%d ", numbers[i]);
        printf("\n");
        free(numbers);
        return;
    }
    for(int i = 0; i < k; i++){
        int *numbers_new = (int *)malloc(sizeof(int) * n);
        if(level != 0)
            memcpy(numbers_new, numbers, n * sizeof(int));
        numbers_new[level] = i + 1;
        variacie(n, k, level + 1, numbers_new);
    }
}

int main(){
    int n, k;

    scanf("%d %d", &n, &k);
    variacie(n, k, 0, NULL);
    

    return 0;
}