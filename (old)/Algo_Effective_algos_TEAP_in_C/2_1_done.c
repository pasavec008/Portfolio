/*
Bernard a Chryzostom majú na stole veľa ceruziek. Keďže nemajú papier, tak si vymysleli hru, ktorou by sa zabavili.
Na striedačku si berú ceruzky a ten, kto zoberie posledné zvyšné ceruzky vyhráva. Aby to ale nebolo také nudné, obmedzili si možné ťahy.
Hru sa hrajú na viac krát a v každej hre môžu mať inú množinu povolených ťahov. Povolený ťah je počet ceruziek, koľko môžu zobrať na jeden ťah.

Na vstupe je niekoľko hier. Pre každú hru je najprv zadané celé číslo N - počet povolených ťahov.
Nasleduje N celých čísel T_i vyjadrujúcich povolené ťahy a dve celé čísla A a B.
V každej hre spočítajte pre koľko čísel X (A <= X <= B) vyhrá Bernard, ak by sa hrali hru s X ceruzkami a Bernard by ťahal ako prvý.

Obmedzenia:
1 <= N <= 50,
1 <= T_i <= 100,
1 <= A <= B,
1 <= B <= 100000

Ukážka vstupu:
1
1
1000 1000
10
1 2 3 4 5 6 7 8 9 10
1 100000
4
1 3 7 19
1 100000
Výstup pre ukážkový vstup:
0
90910
50000
*/

#include <stdio.h>
#include <stdlib.h>

int *setup_vyhry(int *vyhry, int koniec, int *povolene_tahy, int pocet_povolenych_tahov){
    int win;
    vyhry[0] = 0;
    for(int i = 1; i <= koniec; i++){
        win = 0;
        for(int ii = 0; ii < pocet_povolenych_tahov; ii++){
            if(i - povolene_tahy[ii] >= 0)
                if(vyhry[i - povolene_tahy[ii]] == 0){
                    win = 1;
                    break;
                }
        }
        if(win)
            vyhry[i] = 1;
        else 
            vyhry[i] = 0;
    }

    return vyhry;
}

int je_win(int pocet_ceruziek, int *povolene_tahy, int pocet_povolenych_tahov, int *vyhry){
    if(vyhry[pocet_ceruziek])
        return 1;
    return 0;
}

int main(){
    int pocet_povolenych_tahov, start, koniec;

    while(scanf("%d", &pocet_povolenych_tahov) > 0){
        int *povolene_tahy = (int *)malloc(pocet_povolenych_tahov * sizeof(int));
        
        for(int i = 0; i < pocet_povolenych_tahov; i++)
            scanf("%d", &povolene_tahy[i]);

        scanf("%d %d", &start, &koniec);
        
        int vyhry[500000] = {0};

        setup_vyhry(vyhry, koniec, povolene_tahy, pocet_povolenych_tahov);

        int pocet_vitaz = 0;
        
        for(int i = start; i <= koniec; i++)
            if(je_win(i, povolene_tahy, pocet_povolenych_tahov, vyhry))
                pocet_vitaz++;


        printf("%d\n", pocet_vitaz);
        free(povolene_tahy);
    }

    return 0;
}