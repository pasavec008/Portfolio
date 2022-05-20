#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>                        //kniznica na funkciu merania casu
#define SIZE 2000000


#include "prevzaty_strom.h"

#include "vlastny_strom.h"

#include "prevzata_hash.h"                  //funkcie Insert() a Search()

#include "vlastna_hash.h"                   //funkcie vlastna_hash_insert() a vlastna_hash_search()              

void nacitaj_cisla(int *pole_cisel, char *test_case, int kolko){
    FILE *subor = fopen(test_case, "r");

    for(int i = 0; i < kolko; i++)
        fscanf(subor, "%d", &pole_cisel[i]);

    fclose(subor);
}

double test_moja_hash(int *cisla, int pocet, int hladam_odkial, double *cas_insert, double *cas_search, int aj_neexistujuce){
    int number_of_nodes = 1, table_size = 10007;
    NODE_HASH *vlastna_hash_array = vlastna_hash_init(table_size);
    double time;
    clock_t start, end, start2, end2;
    
    start = clock();
    start2 = clock();
    for(int i = 0; i < pocet; i++)
        vlastna_hash_array = vlastna_hash_insert(vlastna_hash_array, cisla[i], number_of_nodes++, &table_size);
    end2 = clock();
    *cas_insert = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);
    
    start2 = clock();
    if(aj_neexistujuce){
        int neexistujuce = 1500001;
        for(int i = 0; i < pocet; i++)
            vlastna_hash_search(vlastna_hash_array, neexistujuce, table_size);
    }
    else
        for(hladam_odkial; hladam_odkial < pocet; hladam_odkial++)
            vlastna_hash_search(vlastna_hash_array, cisla[hladam_odkial], table_size);
    end2 = clock();
    *cas_search = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);
    end = clock();
    time = ((double)(end - start)) / ((double)CLOCKS_PER_SEC);
    
    return time;
}

double test_moj_strom(int *cisla, int pocet, int hladam_odkial, double *cas_insert, double *cas_search, int aj_neexistujuce){

    NODE_STROM *root = NULL;

    double time;
    clock_t start, end, start2, end2;

    start = clock();
    start2 = clock();
    for(int i = 0; i < pocet; i++){
        root = vlastny_insert(cisla[i], root);
    }
    end2 = clock();
    *cas_insert = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);

    start2 = clock();
    if(aj_neexistujuce){
        int neexistujuce = 1500001;
        for(int i = 0; i < pocet; i++)
            vlastny_search(neexistujuce, root);
    }
    else
        for(hladam_odkial; hladam_odkial < pocet; hladam_odkial++)
            vlastny_search(cisla[hladam_odkial], root);

    end2 = clock();
    *cas_search = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);
    end = clock();

    time = ((double)(end - start)) / ((double)CLOCKS_PER_SEC);
    return time;
}

double test_cudzia_hash(int *cisla, int pocet, int hladam_odkial, double *cas_insert, double *cas_search, int aj_neexistujuce){
    double time;
    clock_t start, end, start2, end2;
    
    count = 0;                                  //vynulovanie globalneho countu
    memset(arr, 0, SIZE * sizeof(int));         //vynulovanie globalneho pola
    
    start = clock();
    start2 = clock();
    for(int i = 0; i < pocet; i++)
        Insert(cisla[i]);
    end2 = clock();
    *cas_insert = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);

    start2 = clock();
    if(aj_neexistujuce){
        int neexistujuce = 1500001;
        for(int i = 0; i < pocet; i++)
            Search(neexistujuce);
    }
    else
        for(hladam_odkial; hladam_odkial < pocet; hladam_odkial++)
            Search(cisla[hladam_odkial]);
    end2 = clock();
    *cas_search = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);

    end = clock();
    time = ((double)(end - start)) / ((double)CLOCKS_PER_SEC);
    return time;
}

double test_cudzi_strom(int *cisla, int pocet, int hladam_odkial, double *cas_insert, double *cas_search, int aj_neexistujuce){
    node *root = NULL;
    double time;
    clock_t start, end, start2, end2;
    start = clock();
    start2 = clock();
    for(int i = 0; i < pocet; i++){
        root = insert(cisla[i], root);
    }
    end2 = clock();
    *cas_insert = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);
    start2 = clock();
    if(aj_neexistujuce){
        int neexistujuce = 1500001;
        for(int i = 0; i < pocet; i++)
            find(neexistujuce, root);
    }
    else
        for(hladam_odkial; hladam_odkial < pocet; hladam_odkial++)
            find(cisla[hladam_odkial], root);

    end2 = clock();
    *cas_search = ((double)(end2 - start2)) / ((double)CLOCKS_PER_SEC);
    end = clock();

    time = ((double)(end - start)) / ((double)CLOCKS_PER_SEC);
    
    return time;
}

void test_case_1(int aj_neexistujuce){
    //50 000 cisel, nahodnych v intervale od 1 - 1 000 000, searchujem vsetky vlozene prvky
    //zmen hore definiciu SIZE na minimalne 50 000

    int pocet = 50000, pole_cisel[50000];
    double cas = 0, cas_insert, cas_search, cas_insert_vysledok = 0, cas_search_vysledok = 0;
    nacitaj_cisla(pole_cisel, "test1.txt", pocet);

    printf("\n***************TEST_CASE_1***************\n");

    for(int i = 0; i < 10; i++){
        cas += test_moj_strom(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moj_strom (splay):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzi_strom(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzi_strom (AVL):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_moja_hash(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moja_hash (retazenie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzia_hash(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzia_hash (otvorene adresovanie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);


    printf("\n*****************************************\n\n");
}

void test_case_2(int aj_neexistujuce){
    //50 000 cisel, postupne iducich v intervale od 1 - 50 000, searchujem vsetky vlozene prvky
    //zmen hore definiciu SIZE na minimalne 50 000

    int pocet = 50000, pole_cisel[50000];
    double cas = 0, cas_insert, cas_search, cas_insert_vysledok = 0, cas_search_vysledok = 0;
    nacitaj_cisla(pole_cisel, "test2.txt", pocet);

    printf("\n***************TEST_CASE_2***************\n");

    for(int i = 0; i < 10; i++){
        cas += test_moj_strom(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moj_strom (splay):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzi_strom(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzi_strom (AVL):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_moja_hash(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moja_hash (retazenie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzia_hash(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzia_hash (otvorene adresovanie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);


    printf("\n*****************************************\n\n");
}

void test_case_3(int aj_neexistujuce){
    //10 000 cisel, postupne nasobky cisla 10 000, searchujem vsetky vlozene prvky
    //zmen hore definiciu SIZE na minimalne 10 000

    int pocet = 10000, pole_cisel[10000];
    double cas = 0, cas_insert, cas_search, cas_insert_vysledok = 0, cas_search_vysledok = 0;
    nacitaj_cisla(pole_cisel, "test3.txt", pocet);

    printf("\n***************TEST_CASE_3***************\n");

    for(int i = 0; i < 10; i++){
        cas += test_moj_strom(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moj_strom (splay):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzi_strom(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzi_strom (AVL):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_moja_hash(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moja_hash (retazenie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzia_hash(pole_cisel, pocet, 0, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzia_hash (otvorene adresovanie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);


    printf("\n*****************************************\n\n");
}

void test_case_4(int aj_neexistujuce){
    //1 000 000 cisel, nahodnych v intervale od 1 - 1 500 000, searchujem len druhú polovicu (500 000) prvkov
    //zmen hore definiciu SIZE na minimalne 1 000 000
    int *pole_cisel = (int *)malloc(1000000 * sizeof(int));
    int pocet = 1000000;
    double cas = 0, cas_insert, cas_search, cas_insert_vysledok = 0, cas_search_vysledok = 0;
    nacitaj_cisla(pole_cisel, "test4.txt", pocet);

    printf("\n***************TEST_CASE_4***************\n");

    for(int i = 0; i < 10; i++){
        cas += test_moj_strom(pole_cisel, pocet, 500000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moj_strom (splay):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzi_strom(pole_cisel, pocet, 500000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzi_strom (AVL):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_moja_hash(pole_cisel, pocet, 500000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moja_hash (retazenie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzia_hash(pole_cisel, pocet, 500000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzia_hash (otvorene adresovanie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);


    printf("\n*****************************************\n\n");
}

void test_case_5(int aj_neexistujuce){
    //10 000 cisel, nahodnych v intervale od 1 - 1 000 000, searchujem len druhú polovicu (5000) prvkov
    //zmen hore definiciu SIZE na minimalne 10 000
    int pocet = 10000, pole_cisel[10000];
    double cas = 0, cas_insert, cas_search, cas_insert_vysledok = 0, cas_search_vysledok = 0;
    nacitaj_cisla(pole_cisel, "test1.txt", pocet);

    printf("\n***************TEST_CASE_5***************\n");

    for(int i = 0; i < 10; i++){
        cas += test_moj_strom(pole_cisel, pocet, 5000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
        
    printf("\nPriemerny cas potrebny pre moj_strom (splay):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzi_strom(pole_cisel, pocet, 5000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzi_strom (AVL):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_moja_hash(pole_cisel, pocet, 5000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre moja_hash (retazenie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);

    cas = 0;
    cas_insert_vysledok = 0;
    cas_search_vysledok = 0;

    for(int i = 0; i < 10; i++){
        cas += test_cudzia_hash(pole_cisel, pocet, 5000, &cas_insert, &cas_search, aj_neexistujuce);
        cas_insert_vysledok += cas_insert;
        cas_search_vysledok += cas_search;
    }
    printf("\nPriemerny cas potrebny pre cudzia_hash (otvorene adresovanie):\nCelkovy: %lf sekundy\nInsert: %lf sekundy\nSearch: %lf sekundy\n", cas/10, cas_insert_vysledok/10, cas_search_vysledok/10);


    printf("\n*****************************************\n\n");
}

int main(){
    //argument 0 pre vyhladavanie len tych prvkov, ktore sa v strukturach nachadzaju
    //argument 1 pre vyhladavanie specialneho prvku X krat, ktory sa v ziadnej zo struktur nenachadza , kde X je pocet prvkov v strukture

    //test_case_1(0);
    //test_case_2(0);
    //test_case_3(0);
    //test_case_4(0);
    //test_case_5(0);

    //test_case_1(1);
    //test_case_2(1);
    //test_case_3(1);
    //test_case_4(1);
    //test_case_5(1);
    
    return 0;
}

