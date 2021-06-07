#include <stdio.h>
#include <stdlib.h>

typedef struct uzol{
    int id;
    int cesta;
    int dlzka;
    int index;
    char typ;
    char hodnota;
    char vybaveny;

    struct uzol *hore;
    struct uzol *dole;
    struct uzol *vlavo;
    struct uzol *vpravo;

} UZOL;

void vypis_graf(UZOL *graf, int n, int m){
    for(int y = 0; y < n; y++){
        for(int x = 0; x < m; x++){
            printf("%c ", graf[x + y*m].typ);
        }
        printf("\n");
    }

    printf("\n\n");

    for(int y = 0; y < n; y++){
        printf("  ");
        for(int o = 0; o < m; o++){
            if(graf[o + y*m].hore != NULL && o != m-1)
                printf("||    ");
            else if(graf[o + y*m].hore != NULL)
                printf("||  ");
            else if(o != m-1)
                printf("      ");
            else
                printf("    ");
        }
        printf("\n");
        for(int x = 0; x < m; x++){
            if(graf[x + y*m].vlavo != NULL)
                printf("= ");
            else
                printf("  ");
            printf("%c%c ", graf[x + y*m].typ, graf[x + y*m].typ);
            if(graf[x + y*m].vpravo != NULL)
                printf("=");
            else
                printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

UZOL *sprav_graf(char **mapa, int n, int m){
    UZOL *graf;
    int kolko = n * m;
    graf = (UZOL *) malloc(kolko * sizeof(UZOL));

    //najskor vytvorime prvky grafu
    for(int i = 0; i < kolko; i++){
        graf[i].hore = NULL;
        graf[i].dole = NULL;
        graf[i].vlavo = NULL;
        graf[i].vpravo = NULL;
        graf[i].id = i;
    }

    //nasledne vyplnim typy policok podla mapy
    for(int y = 0; y < n; y++)
        for(int x = 0; x < m; x++){
            graf[x+y*m].typ = mapa[y][x];
            if(mapa[y][x] == 'H')
                graf[x+y*m].hodnota = 2;
            else if(mapa[y][x] == 'N')
                graf[x+y*m].hodnota = 5;
            else
                graf[x+y*m].hodnota = 1;
        }
            

    //teraz vytvorim hrany medzi jednotlivymi uzlami
    for(int y = 0; y < n; y++)
        for(int x = 0; x < m; x++){
            if(graf[x + y * m].typ != 'N'){
                if(graf[x + y * m].hore == NULL && y > 0 && graf[x + (y - 1) * m].typ != 'N'){
                    graf[x + y * m].hore = &graf[x + (y - 1) * m];
                    graf[x + (y - 1) * m].dole = &graf[x + y * m];
                }

                if(graf[x + y * m].dole == NULL && y < n - 1 && graf[x + (y + 1) * m].typ != 'N'){
                    graf[x + y * m].dole = &graf[x + (y + 1) * m];
                    graf[x + (y + 1) * m].hore = &graf[x + y * m];
                }

                if(graf[x + y * m].vlavo == NULL && x > 0 && graf[x - 1 + y * m].typ != 'N'){
                    graf[x + y * m].vlavo = &graf[x - 1 + y * m];
                    graf[x - 1 + y * m].vpravo = &graf[x + y * m];
                }

                if(graf[x + y * m].vpravo == NULL && x < m - 1 && graf[x + 1 + y * m].typ != 'N'){
                    graf[x + y * m].vpravo = &graf[x + 1 + y * m];
                    graf[x + 1 + y * m].vlavo = &graf[x + y * m];
                }
            }
        }
    
    //vypis_graf(graf, n, m);
    return graf;
}

void heap_bubble(UZOL **heap, UZOL *prvok, int index){
    //vyplavenie prvku smerom nahor v halde
    while(index != 0 && prvok->dlzka < heap[(index - 1) / 2]->dlzka){
        heap[(index - 1) / 2]->index = index;
        heap[index] = heap[(index - 1) / 2];
        heap[(index - 1) / 2] = prvok;
        index = (index - 1) / 2;
        prvok->index = index;
    }
}

void heap_reverse_bubble(UZOL **heap, int pocet){
    //potapanie prvku v halde smerom nadol
    int index = 0, local_min;
    UZOL *pomocny;
    while(2 * index + 1 < pocet){
        if(2 * index + 2 < pocet){
            if(heap[2 * index + 1]->dlzka < heap[2 * index + 2]->dlzka){
                if(heap[index]->dlzka > heap[2 * index + 1]->dlzka){
                    pomocny = heap[index];
                    pomocny->index = 2 * index + 1;
                    heap[index] = heap[2 * index + 1];
                    heap[index]->index = index;
                    heap[2 * index + 1] = pomocny;
                    index = 2 * index + 1;
                }
                else{
                    return;
                }
            }  
            else{
                if(heap[index]->dlzka > heap[2 * index + 2]->dlzka){
                    pomocny = heap[index];
                    pomocny->index = 2 * index + 2;
                    heap[index] = heap[2 * index + 2];
                    heap[index]->index = index;
                    heap[2 * index + 2] = pomocny;
                    index = 2 * index + 2;
                }
                else{
                    return;
                }
            }    
        }
        else{
            if(heap[index]->dlzka > heap[2 * index + 1]->dlzka){
                pomocny = heap[index];
                pomocny->index = 2 * index + 1;
                heap[index] = heap[2 * index + 1];
                heap[index]->index = index;
                heap[2 * index + 1] = pomocny;
                index = 2 * index + 1;
            }
            else{
                return;
            }
                
        }
    }
}

void dijkstra_drakobijec(int prvy, UZOL *graf, int pocet){
    //dijkstrov algoritmus
    int vybavene_pocet = 0, nevybavene_pocet = 0;
    UZOL **vybavene = (UZOL **)malloc(pocet * sizeof(UZOL *)), **nevybavene = (UZOL **)malloc(pocet * sizeof(UZOL *));

    //prvy zaradime do vybavenych, ostatne do nevybavenych
    for(int i = 0; i < pocet; i++){
        if(i == prvy){
            graf[i].vybaveny = 1;
            vybavene[vybavene_pocet++] = &graf[i];
            graf[i].cesta = graf[i].id;
            graf[i].dlzka = graf[i].hodnota;
        }
        else if(graf[i].typ != 'N'){        //nepriechodne skaly neberiem
            graf[i].vybaveny = 0;
            graf[i].dlzka = pocet * pocet;
            graf[i].index = nevybavene_pocet;
            nevybavene[nevybavene_pocet++] = &graf[i];
        }
    }


    //hlavny cyklus algoritmu
    UZOL aktualny = *vybavene[0];
    while(nevybavene_pocet != 0){
        //upravim vzdialenosti susedov pridaneho vybaveneho uzla
        if(aktualny.vlavo != NULL && aktualny.vlavo->vybaveny == 0){
            if(aktualny.dlzka + aktualny.vlavo->hodnota < aktualny.vlavo->dlzka){
                aktualny.vlavo->dlzka = aktualny.dlzka + aktualny.vlavo->hodnota;
                aktualny.vlavo->cesta = aktualny.id;
                heap_bubble(nevybavene, aktualny.vlavo, aktualny.vlavo->index);
            }
        }
        if(aktualny.vpravo != NULL && aktualny.vpravo->vybaveny == 0){
            if(aktualny.dlzka + aktualny.vpravo->hodnota < aktualny.vpravo->dlzka){
                aktualny.vpravo->dlzka = aktualny.dlzka + aktualny.vpravo->hodnota;
                aktualny.vpravo->cesta = aktualny.id;
                heap_bubble(nevybavene, aktualny.vpravo, aktualny.vpravo->index);
            }
        }
        if(aktualny.hore != NULL && aktualny.hore->vybaveny == 0){
            if(aktualny.dlzka + aktualny.hore->hodnota < aktualny.hore->dlzka){
                aktualny.hore->dlzka = aktualny.dlzka + aktualny.hore->hodnota;
                aktualny.hore->cesta = aktualny.id;
                heap_bubble(nevybavene, aktualny.hore, aktualny.hore->index);
            } 
        }
        if(aktualny.dole != NULL && aktualny.dole->vybaveny == 0){
            if(aktualny.dlzka + aktualny.dole->hodnota < aktualny.dole->dlzka){
                aktualny.dole->dlzka = aktualny.dlzka + aktualny.dole->hodnota;
                aktualny.dole->cesta = aktualny.id;
                heap_bubble(nevybavene, aktualny.dole, aktualny.dole->index);
            }
        }
        //nevybavene uzly - vyberiem ten s najmensou vzdialenostou

        vybavene[vybavene_pocet] = nevybavene[0];
        nevybavene[0]->vybaveny = 1;
        nevybavene[0] = nevybavene[nevybavene_pocet - 1];         //do korena dam posledny list
        nevybavene[0]->index = 0;
        heap_reverse_bubble(nevybavene, nevybavene_pocet - 1);
        aktualny = *vybavene[vybavene_pocet];
        //nezabudnem znizit/zvysit pocty
        nevybavene_pocet--;
        vybavene_pocet++;
    }

    /*
    for(int i = 0; i < vybavene_pocet; i++){
        printf("%d %d %d %d\n", vybavene[i]->dlzka, vybavene[i]->index, vybavene[i]->id, vybavene[i]->vybaveny);
    }
    */

    free(vybavene);
    free(nevybavene);
}

int najdi_v_mape(int *ciele, int pocet, UZOL *graf, int *pocet_princezien){
    //najde draka a princezne a zisti, ci nie su blokovane
    for(int i = 0; i < pocet; i++){
        if(graf[i].typ == 'D'){
            if(graf[i].dlzka == pocet*pocet)
                return 0;
            ciele[0] = i;
        }
            
        else if(graf[i].typ == 'P'){
            if(graf[i].dlzka == pocet*pocet)
                return 0;
            ciele[++(*pocet_princezien)] = i;
        }
            
    }
    return 1;
}

int *zloz_cestu(UZOL *graf, int index, int pociatocny, int *dlzka){
    int k = 1, *cesta, index_p = index;
    while(index_p != pociatocny){
        //printf("%d %d\n", pociatocny, index_p);
        index_p = graf[index_p].cesta;
        k++;
    }

    cesta = (int *)malloc(k * sizeof(int));

    for(int i = 0; i < k; i++){
        cesta[k-i-1] = index;
        index = graf[index].cesta;
    }

    *dlzka = k;

    return cesta;
}

int *spoj_cesty(int *prva_cesta, int *druha_cesta, int *dlzka_prvej, int *dlzka_druhej){
    int *finalna_cesta = (int *)malloc((*dlzka_prvej + *dlzka_druhej - 1) * sizeof(int)), i, dlzka_finalnej = *dlzka_prvej + *dlzka_druhej - 1, k;

    /*
    printf("Prva\n");
    for(int o = 0; o < *dlzka_prvej; o++){
        printf("%d ", prva_cesta[o]);
    }
    printf("\n");
    printf("Druha\n");
    for(int o = 0; o < *dlzka_druhej; o++){
        printf("%d ", druha_cesta[o]);
    }
    printf("\n");
    */
    
    
    for(i = 0; i < *dlzka_prvej; i++)
        finalna_cesta[i] = prva_cesta[i];

    k = 1;
    for(int y = i; y < dlzka_finalnej; y++)
        finalna_cesta[y] = druha_cesta[k++];


    *dlzka_prvej = dlzka_finalnej;

    /*
    printf("finalna\n");
    for(int o = 0; o < *dlzka_prvej; o++){
        printf("%d ", finalna_cesta[o]);
    }
    printf("\n\n");
    */
    

    free(prva_cesta);
    free(druha_cesta);
    return finalna_cesta;
}

int *permutacie(int **pole, int pocet_cielov, char *prenos, int uroven){
    char cesta[5];
    int k = 1, *pomocna, *pomocna2 = NULL;
    sprintf(cesta, prenos);

    if(uroven == pocet_cielov - 1){
        pomocna = (int *)malloc(6 * sizeof(int));
        pomocna[0] = -uroven;
        for(int i = 0; i < uroven; i++){
            if(i == 0)
                pomocna[0] += pole[0][cesta[i]-'0'];
            else
                pomocna[0] += pole[cesta[i-1]-'0'][cesta[i]-'0'];
        }
        for(int i = 0; i < uroven; i++)
            pomocna[i+1] = cesta[i] - '0';
        //printf("%d %d %d %d %d %d\n", pomocna[0], pomocna[1], pomocna[2], pomocna[3], pomocna[4], pomocna[5]);
        return pomocna;
    }
        
    for(int i = 0; i < pocet_cielov - uroven - 1; i++){
        for(int y = 0; y < pocet_cielov - 1; y++){
            if(cesta[y] == k + '0'){
                k++;
                y = -1;
            }    
        }
        cesta[uroven] = k++ + '0';
        pomocna = permutacie(pole, pocet_cielov, cesta, uroven + 1);
        if(pomocna2 == NULL || pomocna[0] < pomocna2[0]){
            free(pomocna2);
            pomocna2 = pomocna;
        }
        else
            free(pomocna);
            
        cesta[uroven] = 'N';
    }
 
    

    return pomocna2;
}

int *uzasna_zachranna_akcia_vsetkych_princezien(int *ciele, UZOL *graf, int pocet, int pocet_princezien, int *dlzka_pomocnej_cesty){
    int pocet_cielov = pocet_princezien + 1, **vzdialenosti = (int **)malloc(pocet_cielov * sizeof(int *));
    //alokujem 2D pole vzdialenosti
    for(int i = 0; i < pocet_cielov; i++)
        vzdialenosti[i] = (int *)malloc(pocet_cielov * sizeof(int));


    //naplnim 2D pole vzdialenosti
    for(int i = 0; i < pocet_cielov; i++){
        dijkstra_drakobijec(ciele[i], graf, pocet);
        for(int k = 0; k < pocet_cielov; k++){
            vzdialenosti[i][k] = graf[ciele[k]].dlzka;
            vzdialenosti[k][i] = graf[ciele[k]].dlzka;
        }
    }

    char prenos[6] = "NNNNN";
    prenos[5] = 0;
    for(int i = pocet_princezien; i < 5; i++)
        prenos[i] = 'X';
    //int *cesta = (int *)malloc(pocet_cielov * sizeof(int));
    //cesta[0] = 0;
    
    int *minimum = permutacie(vzdialenosti, pocet_cielov, prenos, 0);

    /*
    for(int q = 0; q < pocet_princezien; q++){
        printf("%d ", graf[ciele[minimum[q+1]]]);
    }
    printf("\n");
    */
    /*
    for(int i = 0; i < pocet_cielov; i++)
        for(int k = 0; k < pocet_cielov; k++)
            printf("princezna %d\n", vzdialenosti[i][k]);
    printf("\n");
    */


    for(int i = 0; i < pocet_cielov; i++)
        free(vzdialenosti[i]);
    free(vzdialenosti);

    //zacinam vzdy drakom
    dijkstra_drakobijec(ciele[0], graf, pocet);
    int *cesta, dlzka, *finalna_cesta = NULL;
    //dlzka_pomocnej_cesty = (int *)malloc(sizeof(int));
    for(int i = 0; i < pocet_cielov - 1; i++){
        dlzka = 0;
        if(i == 0)
            cesta = zloz_cestu(graf, ciele[minimum[i+1]], ciele[i], &dlzka);
        else
            cesta = zloz_cestu(graf, ciele[minimum[i+1]], ciele[minimum[i]], &dlzka);
        if(finalna_cesta == NULL){
            finalna_cesta = cesta;
            *dlzka_pomocnej_cesty = dlzka;
        }
        else
            finalna_cesta = spoj_cesty(finalna_cesta, cesta, dlzka_pomocnej_cesty, &dlzka);
        
        /*
        for(int y = 0; y < dlzka; y++){
            printf("%d ",cesta[y]);
        }
        printf("cesticka\n");
        */
        //printf("ciel %d\n", ciele[minimum[i+1]]);
        dijkstra_drakobijec(ciele[minimum[i+1]], graf, pocet);
    }
    return finalna_cesta;
}

int *dijkstra(int *dlzka_cesty, UZOL *graf, int pocet, int zobudeny_drak){
    int dlzka = 0, *ciele = (int *)malloc(6 * sizeof(int)), pocet_princezien = 0, *finalna_cesta, *pomocna_cesta, *dlzka_pomocnej_cesty;

    dijkstra_drakobijec(0, graf, pocet);
    

    if(najdi_v_mape(ciele, pocet, graf, &pocet_princezien) == 0){
        printf("Minimalne jeden z cielov cesty je zablokovany!\n");
        return NULL;
    }

    printf("Parameter T je %d a ja pridem na draka %d\n", zobudeny_drak, graf[ciele[0]].dlzka);
    if(zobudeny_drak < graf[ciele[0]].dlzka){
        printf("Drak je pridaleko!\n");
        return NULL;
    }

    finalna_cesta = zloz_cestu(graf, ciele[0], 0, dlzka_cesty);

    /*
    for(int i = 0; i < *dlzka_cesty; i++){
        printf("%d ",finalna_cesta[i]);
    }
    printf("\n");
    */

    dlzka_pomocnej_cesty = (int *)malloc(sizeof(int));
    pomocna_cesta = uzasna_zachranna_akcia_vsetkych_princezien(ciele, graf, pocet, pocet_princezien, dlzka_pomocnej_cesty);
    finalna_cesta = spoj_cesty(finalna_cesta, pomocna_cesta, dlzka_cesty, dlzka_pomocnej_cesty);

    /*
    printf("drak sa nachadza na indexe: %d\n", ciele[0]);
    for(int i = 0; i < pocet_princezien; i++){
        printf("princezna cislo %d sa nachadza na indexe: %d\n", i+1, ciele[i+1]);
    }
    printf("%c %c %c", graf[50].typ, graf[75].typ, graf[20].typ);
    */


   return finalna_cesta;
}

int *postav_cestu(UZOL *graf, int *povodna_cesta, int sirka, int *dlzka_cesty, int *cas){
    //naformatuje cestu do suradnicovych dvojic
    int *vysledna_cesta = (int *)malloc(*dlzka_cesty * 2 * sizeof(int));

    for(int i = 0; i < *dlzka_cesty; i++){
        vysledna_cesta[2 * i] = povodna_cesta[i] % sirka;
        vysledna_cesta[2 * i + 1] = povodna_cesta[i] / sirka;
        *cas += graf[povodna_cesta[i]].hodnota;
    }
    free(povodna_cesta);
    return vysledna_cesta;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    UZOL *graf;
    int *vysledna_cesta, pocet = n * m;
    graf = sprav_graf(mapa, n, m);
    vysledna_cesta = dijkstra(dlzka_cesty, graf, pocet, t);
    if(vysledna_cesta == NULL){
        free(graf);
        *dlzka_cesty = 0;
        return NULL;
    }
    int cas = 0;
    vysledna_cesta = postav_cestu(graf, vysledna_cesta, m, dlzka_cesty, &cas);
    //uvolni graf
    free(graf);

    printf("Najkratsia cesta s casom: %d\n", cas);

    
    for(int i = 0; i < *dlzka_cesty; i++){
        printf("(%d %d) ", vysledna_cesta[i * 2], vysledna_cesta[i * 2 + 1]);
    }
    printf("\n");
    
    
    
    return vysledna_cesta;
}

void preset(){
    char **mapa;
    int dlzka_cesty, *cesta;
    int n = 10;
    int m = 10;
    int t = 12;
    mapa = (char **)malloc(n * sizeof(char *));
    mapa[0]="CCHCNHCCHN";
    mapa[1]="NNCCCHHCCC";
    mapa[2]="DNCCNNHHHC";
    mapa[3]="CHHHCCCCCC";
    mapa[4]="CCCCCNHHHH";
    mapa[5]="PCHCCCNNNN";
    mapa[6]="NNNNNHCCCC";
    mapa[7]="CCCCCPCCCC";
    mapa[8]="CCCNNHHHHH";
    mapa[9]="HHHPCCCCCC";
    cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
}

void dataset(char *nazov){
    printf("\nNazov mapy: %s\n", nazov);

    FILE *subor = fopen(nazov, "r");
    int n, m, t, *cesta, dlzka_cesty;
    char **mapa;

    fscanf(subor, "%d %d %d", &n, &m, &t);
    mapa = (char **)malloc(n * sizeof(char *));
    for(int i = 0; i < n; i++)
        mapa[i] = (char *)malloc(m * sizeof(char *));
    for(int i = 0; i < n; i++)
        fscanf(subor, "%s", mapa[i]);

    cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);

    /*
    for(int i = 0; i < dlzka_cesty; i++){
        printf("%d %d\n", cesta[i*2], cesta[i*2+1]);
    }
    printf("\n");
    */
    

    fclose(subor);
    for(int i = 0; i < n; i++)
        free(mapa[i]);
    free(mapa);
}


int main(){
    //je zapnuty vypis do riadku... (do stlpca sa niektore mapy nezmestia)
    //...vo funkcii dataset je zakomentovany klasicky vypis do stlpca

    //preset();                         //klasicka zo zadania
    dataset("1.txt");                 //klasicka zo zadania

    //custom made mapy
    //dataset("2.txt");                 //custom 39x39 mapa
    //dataset("3.txt");                 //custom 1x14 mapa so 4 princeznami na zaciatku a piatou za drakom
    //dataset("4.txt");                 //custom minimalisticka 1x2, s jednym drakom a jednou princeznou
    //dataset("5.txt");                 //custom uzka, vysoka mapa 10x1 s drakom na startovacom policku
    //dataset("6.txt");                 //custom cik-cakovita 10x10


    //random mapy
    //dataset("7.txt");                 //random mapa 100x100 s piatimi princeznami
    //dataset("8.txt");                 //random mapa 5x100 s dvoma princeznami
    //dataset("9.txt");                 //random mapa 100x100 so zablokovanou princeznou v pravom dolnom rohu
    //dataset("10.txt");                //random mapa 27x27 s drakom na zaciatku a princeznou na konci
    //dataset("11.txt");                //random mapa 100x100 s drakom na zaciatku a princeznami okolo neho
    //dataset("12.txt");                //random mapa 25x50 predelena nepriechodnou stenou a jednou princeznou nedostupnou

    //dataset("13.txt");                //random mapa 17x83 s dvomi princeznami
    //dataset("14.txt");                //random mapa 61x24 s tromi princeznami
    //dataset("15.txt");                //random mapa 7x34 so styrmi princeznami
    //dataset("16.txt");                //random mapa 30x25, kde sa neviem pohnut zo startu
    //dataset("17.txt");                //random mapa 1x3, kde zaciatok (0, 0) je nepriechodna prekazka


    //printf("Ukoncujem\n");
    return 0;
}