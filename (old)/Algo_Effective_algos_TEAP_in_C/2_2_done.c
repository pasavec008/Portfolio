/*
Jakubko sa učí sčitovať. Pre daný reťazec číslic určite koľko najmenej znamienok sčítania je potrebné do reťazca vložiť, aby výsledný výraz mal nejaký konkrétny súčet.
Napr. ak chceme pre reťazec 303 dosiahnuť súčet 6, tak stačí vložiť jedno znamienko sčítania (3+03=6).

Vstup: Štandardný vstup môže obsahovať zápisy viacerých úloh. Každý zápis obsahuje reťazec čísel obsahujúci najviac 10 znakov a súčet, ktorý chceme dosiahnuť, oddelené medzerou.

Výstup: Pre každé zadanie úlohy na vstupe vypíšte na štandardný výstup jeden riadok obsahujúci jedno číslo
-- najmenší počet znamienok sčítania, ktoré je nutné do reťazca na vstupe vložiť, aby výsledný výraz mal požadovaný súčet.
Ak nie je možné súčet dosiahnuť, vypíšte IMPOSSIBLE

Ukážka vstupu:
999 27
101 2
999 100
Výstup pre ukážkový vstup:
2
1
IMPOSSIBLE
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int napln_pocitaj(int pocet_cifier, long cislo, long vysledok){
    int tabulka[10] = {0}, repeat; 
    long pomocna;

    pomocna = cislo;
    for(int i = 0; i < pocet_cifier; i++){
        repeat = (int)ceil(pow(10, pocet_cifier - i - 1));
        tabulka[i] = pomocna / repeat;
        pomocna = pomocna % repeat;
    }



    //hladanie
    int *pomocne_pole = (int *)malloc(sizeof(int) * (pocet_cifier - 1)), pocet_plus = 1;
    for(int i = 0; i < pocet_cifier; i++)
        pomocne_pole[i] = 0;

    pomocna = (int)ceil(pow(2, pocet_cifier - 1)) ;
    int swap, index;
    

    for(int i = 1; i < pomocna; i++){
        swap = 0;
        for(int j = 0; j < pocet_cifier - 2; j++){
            if(pomocne_pole[pocet_cifier - 2 - j] == 0 && pomocne_pole[pocet_cifier - 3 - j] == 1){
                pomocne_pole[pocet_cifier - 2 - j] = 1;
                pomocne_pole[pocet_cifier - 3 - j] = 0;

                index = pocet_cifier - 1 - j;
                while(index < pocet_cifier - 1 && pomocne_pole[index] != 0)
                    index++;
                for(int aaa = index; aaa <= pocet_cifier - 2; aaa++){
                    if(pomocne_pole[aaa] == 1){
                        if(index != aaa){
                            pomocne_pole[index] = 1;
                            pomocne_pole[aaa] = 0;
                            while(index < pocet_cifier - 1 && pomocne_pole[index] != 0)
                                index++;
                        }
                    }
                }
                swap = 1;
                break;
            }
        }
        if(i == 1)
            pomocne_pole[0] = 1;
        else if(!swap){
            pocet_plus++;
            int j = 0;
            for(j; j < pocet_cifier - 1; j++){
                if(pomocne_pole[j] == 0){
                    pomocne_pole[j] = 1;
                    break;
                }
            }
            index = j + 1;
            while(index < pocet_cifier - 1 && pomocne_pole[index] != 0)
                index++;
            for(int aaa = index; aaa <= pocet_cifier - 2; aaa++){
                if(pomocne_pole[aaa] == 1){
                    if(index != aaa){
                        pomocne_pole[index] = 1;
                        pomocne_pole[aaa] = 0;
                        while(index < pocet_cifier - 1 && pomocne_pole[index] != 0)
                            index++;
                    }
                }
            }
        }


        long vysledok_moj = 0;
        int nuly_v_poradi, k;
        for(int ii = 0; ii < pocet_cifier; ii){
            nuly_v_poradi = 1;
            k = ii;
            while(pomocne_pole[k] == 0 && k++ < pocet_cifier - 1)
                nuly_v_poradi++;
            for(int ok = 0; ok < nuly_v_poradi; ok++){
                vysledok_moj += (tabulka[pocet_cifier - 1 - ii++]) * (int)ceil(pow(10, ok));
            }
        }

        if(vysledok_moj == vysledok){
            free(pomocne_pole);
            return pocet_plus;
        }

    }
    
    free(pomocne_pole);
    return 0;
}

int main(){
    long cislo, vysledok, pomocna;
    int pocet_cifier, pocet_plus;

    while(scanf("%ld", &cislo) > 0){
        scanf("%ld", &vysledok);
        if(cislo == vysledok){
            printf("0\n");
            continue;
        }
        else if(vysledok > cislo){
            printf("IMPOSSIBLE\n");
            continue;
        }

        pomocna = cislo;
        pocet_cifier = 1;

        while(pomocna / 10 != 0){
            pomocna /= 10;
            pocet_cifier++;
        }

        pocet_plus = napln_pocitaj(pocet_cifier, cislo, vysledok);

        if(pocet_plus == 0)
            printf("IMPOSSIBLE\n");
        else
            printf("%d\n", pocet_plus);
    }


    return 0;
}