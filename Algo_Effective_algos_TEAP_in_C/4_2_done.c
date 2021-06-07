/*
Jakubko sa začal učiť cudzie jazyky. Páči sa mu keď objaví nejaké nové komplikované slovo, ktoré obsahuje aspoň 3 samohlásky za sebou, alebo 5 spoluhlások za sebou.
Rozhodol sa, že sa pôjde zdokonaliť v cudzích jazykoch na výlet do zahraničia. Pomôžte mu naplánovať trasu výletu tak, aby mal príležitosť vidieť čo najviac komplikovaných slov.

Zohnali ste si fotky viacerých miest, ktoré by Jakubko mohol navštíviť. Na vstupe je niekoľko cudzích slov, ktoré vidíte na fotkách, každé na samostatnom riadku.
Niektoré slová však nevidieť celé, lebo vo výhľade niečo prekáža. Každé slovo môže pozostávať z veľkých písmien anglickej abecedy a otáznikov, ktoré vyjadrujú,
že na fotke nevidieť dané písmeno slova. Maximálna dĺžka slova je 50 znakov.

Na výstup vypíšte paci ak sa Jakubkovi dané slovo určite bude páčiť, nepaci ak sa mu určite nebude páčiť a neviem ak sa to nedá s istotou určiť.

Poznámka: Kľúčom k riešeniu je uvažovanie, aké znaky dosadiť za ? vo vstupe. Uvedomme si, že za každý otáznik môžeme dosadiť alebo samohlásku alebo spoluhlásku,
no ale potom vzhľadom na okolité písmenká je možné, že už nutne vznikne postupnosť troch samohlások alebo piatich spoluhlások, a teda sa takéto cudzie slovo bude Jakubkovi určite páčiť. Niekedy to nemusí byť jednoznačné (neviem), alebo sa to určite nedá takto dosadiť (nepaci).

Ukážka vstupu:
ZPRPR
TEAP
???
Výstup pre ukážkový vstup:
paci
nepaci
neviem
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int samohlaska(char c){
    if(c == '?')
        return -1;
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y')
        return 1;
    return 0;
}

int paci_sa(char pocitam_samohlasky, char *slovo, int kolko_mam){
    if(slovo[0] == 0)
        return 0;

    if(pocitam_samohlasky == -1){
        if(samohlaska(slovo[0])){
            pocitam_samohlasky = 1;
            kolko_mam = 1;
        }
        else{
            pocitam_samohlasky = 0;
            kolko_mam = 1;
        }
        return paci_sa(pocitam_samohlasky, slovo + 1, kolko_mam);
    }

    //samohlasky
    if(pocitam_samohlasky == 1){
        if(samohlaska(slovo[0]) == 1){
            if(++kolko_mam == 3)
                return 1;
        }
        else{
            pocitam_samohlasky = 0;
            kolko_mam = 1;
        }
    }
    //spoluhlasky
    else{
        if(samohlaska(slovo[0]) == 0){
            if(++kolko_mam == 5)
                return 1;
        }
        else{
            pocitam_samohlasky = 1;
            kolko_mam = 1;
        }
    }
    return paci_sa(pocitam_samohlasky, slovo + 1, kolko_mam);
}

void zmen_slovo(int mod, char *slovo, char *slovo_2, char *slovo_3, char *slovo_4, char *slovo_5, char *slovo_6, char *slovo_7, char *slovo_8){
    int i = 0;
    while(slovo[i] != 0){
        slovo[i] = tolower(slovo[i]);
        slovo_2[i] = slovo[i];
        slovo_3[i] = slovo[i];
        slovo_4[i] = slovo[i];
        slovo_5[i] = slovo[i];
        slovo_6[i] = slovo[i];
        slovo_7[i] = slovo[i];
        slovo_8[i] = slovo[i];
        i++;
    }
    slovo_2[i] = 0;
    slovo_3[i] = 0;
    slovo_4[i] = 0;
    slovo_5[i] = 0;
    slovo_6[i] = 0;
    slovo_7[i] = 0;
    slovo_8[i] = 0;
        
    i = 0;

    while(slovo_2[i] != 0){
        if(slovo_2[i] == '?'){
            if(i == 0){
                if(samohlaska(slovo_2[i+1]))
                    slovo_2[i] = 'b';
                else
                    slovo_2[i] = 'a';
            }
            else
                if(samohlaska(slovo_2[i-1]))
                    slovo_2[i] = 'b';
                else
                    slovo_2[i] = 'a';
        }
        i++;
    }

    i--;
    int y = 1;
    while(i >= 0){
        if(slovo_3[i] == '?'){
            if(y == 1){
                if(i == 0)
                    slovo_3[i] = 'a';
                else if(samohlaska(slovo_3[i-1]))
                    slovo_3[i] = 'b';
                else
                    slovo_3[i] = 'a';
            }
            else
                if(samohlaska(slovo_3[i+1]))
                    slovo_3[i] = 'b';
                else
                    slovo_3[i] = 'a';
        }
        y = 0;
        i--;
    }

    i = 0;
    while(slovo[i] != 0){
        if(slovo[i] == '?'){
            slovo_4[i] = 'a';
            slovo_5[i] = 'b';
        }
        i++;
    }

    i = 0;
    while(slovo[i] != 0){
        if(slovo[i] == '?'){
            if(rand() % 2)
                slovo_6[i] = 'a';
            else
                slovo_6[i] = 'b';
            if(rand() % 2)   
                slovo_7[i] = 'b';
            else
                slovo_7[i] = 'a';
            if(rand() % 2)
                slovo_8[i] = 'a';
            else
                slovo_8[i] = 'b';
        }
        i++;
    }
}

int main(){
    srand(5);
    char slovo[80], slovo_2[80], slovo_3[80], slovo_4[80], slovo_5[80], slovo_6[80], slovo_7[80], slovo_8[80];

    while(scanf("%s", slovo) > 0){
        zmen_slovo(1, slovo ,slovo_2, slovo_3, slovo_4, slovo_5, slovo_6, slovo_7, slovo_8);
        int slov2 = paci_sa(-1, slovo_2, 0), slov3 = paci_sa(-1, slovo_3, 0), slov4 = paci_sa(-1, slovo_4, 0), slov5 = paci_sa(-1, slovo_5, 0), slov6 = paci_sa(-1, slovo_6, 0), slov7 = paci_sa(-1, slovo_7, 0), slov8 = paci_sa(-1, slovo_8, 0);
        printf("%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n", slovo_2, slov2, slovo_3, slov3, slovo_4, slov4, slovo_5, slov5, slovo_6, slov6, slovo_7, slov7, slovo_8, slov8);
        if(slov2 == 1 && slov3 == 1 && slov4 == 1 && slov5 == 1 && slov6 == 1 && slov7 == 1 && slov8 == 1)
            printf("paci\n");
        else if(slov2 == 0 && slov3 == 0 && slov4 == 0 && slov5 == 0 && slov6 == 0 && slov7 == 0 && slov8 == 0)
            printf("nepaci\n");
        else
            printf("neviem\n");
    }

    return 0;
}