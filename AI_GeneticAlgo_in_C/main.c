#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_GENES 100
#define LAST_GENERATION 100
#define MUTATION_CHANCE  5 //kolko jedincov z generacie priemerne zmutujem?
#define SIZE_OF_GENERATION 100

typedef struct individual{
    int genes[NUMBER_OF_GENES];
    int fitnes;
}INDIVIDUAL;

typedef struct map{
    int x;
    int y;
    int number_of_stones;
    int number_of_non_stones;
    char **garden;
}MAP;


MAP *load_map(int which){
    char to_load[50];
    sprintf(to_load, "maps/%d.txt", which);
    FILE *map_file = fopen(to_load, "r");

    int x, y;
    fscanf(map_file, "%d %d", &x, &y);

    MAP *map_to_solve = (MAP *)malloc(3 * sizeof(int) + sizeof(char **));
    map_to_solve->number_of_non_stones = 0;

    map_to_solve->garden = (char **)malloc(y * sizeof(char *));
    for(int i = 0; i < y; i++)
        map_to_solve->garden[i] = (char *)malloc(x);

    map_to_solve->x = x;
    map_to_solve->y = y;


    for(y = 0; y < map_to_solve->y; y++)
        for(x = 0; x < map_to_solve->x; x++){
            fscanf(map_file, "%d", &map_to_solve->garden[y][x]);
            if(map_to_solve->garden[y][x] >= 0)
                map_to_solve->number_of_non_stones++;
        }
            

    return map_to_solve;
}

void draw_map(MAP *map_to_solve){
    for(int y = 0; y < map_to_solve->y; y++){
        for(int x = 0; x < map_to_solve->x; x++)
            if(map_to_solve->garden[y][x] >= 0)
                printf("%.2d ", map_to_solve->garden[y][x]);
            else
                printf("KK ", map_to_solve->garden[y][x]);
        printf("\n");
    }

    printf("Number of non-stones is %d\n\n", map_to_solve->number_of_non_stones);
}

void clean_map(MAP *map_to_solve){
    for(int y = 0; y < map_to_solve->y; y++)
        for(int x = 0; x < map_to_solve->x; x++)
            if(map_to_solve->garden[y][x] > 0)
                map_to_solve->garden[y][x] = 0;
}

void fitnes(MAP *map_to_solve, INDIVIDUAL *child, int map_edges){
    int fitnes = 0, number_of_gene = 0, x = -1, y = -1, act_gene, number_of_dig = 0, original;
    char direction = 0, non_suitable;
    while(fitnes < map_to_solve->number_of_non_stones && number_of_gene < NUMBER_OF_GENES){
        //nedosiel som na koniec mapy? zvolim novy start
        if(x < 0 || y < 0 || x >= map_to_solve->x || y >= map_to_solve->y){
            non_suitable = 1;
            act_gene = child->genes[number_of_gene++] % (2 * map_to_solve->x + 2 * map_to_solve->y);
            original = act_gene;
            while(non_suitable){
                if(act_gene < map_to_solve->x){
                    direction = 3;
                    y = 0;
                    x = act_gene;
                }
                else if(act_gene < map_to_solve->x + map_to_solve->y){
                    direction = 4;
                    x = map_to_solve->x - 1;
                    y = act_gene - map_to_solve->x;
                }
                else if(act_gene < map_to_solve->x * 2 + map_to_solve->y){
                    direction = 1;
                    y = map_to_solve->y - 1;
                    x = act_gene - map_to_solve->x - map_to_solve->y;
                }
                else if(act_gene < map_to_solve->x * 2 + map_to_solve->y * 2){
                    direction = 2;
                    x = 0;
                    y = act_gene - map_to_solve->x * 2 - map_to_solve->y;
                }
                else{
                    printf("CHYBA\n");
                    exit(5);
                }

                if(map_to_solve->garden[y][x] == 0){
                    fitnes++;
                    map_to_solve->garden[y][x] = ++number_of_dig;
                    non_suitable = 0;
                }
                else{
                    act_gene++;
                    act_gene = act_gene % map_edges;
                    if(act_gene == original){
                        child->fitnes = fitnes;
                        return;
                    }
                    x = -1;
                    y = -1;
                    continue;
                }
            }
        }


        //skusim sa pohnut do smeru, kde idem
        if(direction == 1)
            y--;
        else if(direction == 2)
            x++;
        else if(direction == 3)
            y++;
        else if(direction == 4)
            x--;
        else{
            printf("CHYBA2\n");
            exit(5);
        }

        //ak vyjdem z mapy, novy zaciatok
        if(x < 0 || y < 0 || x >= map_to_solve->x || y >= map_to_solve->y){
            x = -1;
            y = -1;
            continue;
        }


        //ak narazim, zmenim smer
        if(map_to_solve->garden[y][x] != 0){            
            if(direction == 1)
                y++;
            else if(direction == 2)
                x--;
            else if(direction == 3)
                y--;
            else if(direction == 4)
                x++;
            else{
                printf("CHYBA3\n");
                exit(5);
            }

            if(number_of_gene >= NUMBER_OF_GENES){
                //printf("Dosli geny pri zmene smeru\n");
                break;
            }

            act_gene = child->genes[number_of_gene++];
            if(act_gene % 2 == 0)
                direction++;
            else
                direction--;
            if(direction > 4)
                direction = 1;
            else if(direction < 1)
                direction = 4;
            
            if(direction == 1 && (y - 1 < 0 || map_to_solve->garden[y - 1][x] != 0))
                direction = 3;
            else if(direction == 2 && (x + 1 >= map_to_solve->x || map_to_solve->garden[y][x + 1] != 0))
                direction = 4;
            else if(direction == 3 && (y + 1 >= map_to_solve->y || map_to_solve->garden[y + 1][x] != 0))
                direction = 1;
            else if(direction == 4 && (x - 1 < 0 || map_to_solve->garden[y][x - 1] != 0))
                direction = 2;


            if(direction == 1 && y - 1 >= 0){
                if(map_to_solve->garden[y - 1][x] != 0){
                    child->fitnes = fitnes;
                    return;
                }
            }
            else if(direction == 2 && x + 1 < map_to_solve->x){
                if(map_to_solve->garden[y][x + 1] != 0){
                    child->fitnes = fitnes;
                    return;
                }
            }
            else if(direction == 3 && y + 1 < map_to_solve->y){
                if(map_to_solve->garden[y + 1][x] != 0){
                    child->fitnes = fitnes;
                    return;
                }
            }
            else if(direction == 4 && x - 1 >= 0){
                if(map_to_solve->garden[y][x - 1] != 0){
                    child->fitnes = fitnes;
                    return;
                }
            }

            continue;
        }


        map_to_solve->garden[y][x] = number_of_dig;
        fitnes++;
    }


    child->fitnes = fitnes;

    return;
}

int comparator(const void *y, const void *x){
    return ((INDIVIDUAL *)x)->fitnes - ((INDIVIDUAL *)y)->fitnes;
}

INDIVIDUAL create_new_individual(INDIVIDUAL mom, INDIVIDUAL dad){
    INDIVIDUAL child;
    for(int i = 0; i < NUMBER_OF_GENES; i++){
        if(rand() % 2)
            child.genes[i] = mom.genes[i];
        else
            child.genes[i] = dad.genes[i];
    }
    return child;
}

MAP *solve_map(MAP *map_to_solve, int algo){
    int map_edges = map_to_solve->x * 2 + map_to_solve->y * 2, count = 0;
    INDIVIDUAL generation[SIZE_OF_GENERATION], new_generation[SIZE_OF_GENERATION];

    //vytvorim prvu generaciu, pole jednotlivcov, pre kazdy algo rovnake
    for(int i = 0; i < SIZE_OF_GENERATION; i++)
        for(int y = 0; y < NUMBER_OF_GENES; y++)
            generation[i].genes[y] = rand() % map_edges;


    //while nenajdem dobre riesenie alebo nepridem na generaciu X
    while(count++ < LAST_GENERATION){

        //ohodnotim sucasnu generaciu, ak pri hodnoteni najdem riesenie, vraciam
        for(int i = 0; i < SIZE_OF_GENERATION; i++){
            fitnes(map_to_solve, &generation[i], map_edges);

            if(generation[i].fitnes == map_to_solve->number_of_non_stones){
                printf("Generacia %d, zahrada bola cela pohrabana, %d pohrabanych\n", count, generation[i].fitnes);
                return map_to_solve;
            }
                
                
            clean_map(map_to_solve);
        }

        //priemer fitnes generacie
        double average = 0;
        for(int i = 0; i < SIZE_OF_GENERATION; i++)
            average += generation[i].fitnes;


        //usporiadam pole jedincov podľa fitnes, na zaciatku su najlepsi
        qsort(generation, SIZE_OF_GENERATION, sizeof(INDIVIDUAL), comparator);
        printf("Priemer generacie cislo %d je %.2lf, najlepsia fitnes je %d\n", count, average / SIZE_OF_GENERATION, generation[0].fitnes);

        //vytvorim novu generaciu jednym zo sposobov
        if(algo == 0){  //pure random
            for(int i = 0; i < SIZE_OF_GENERATION; i++)
                for(int y = 0; y < NUMBER_OF_GENES; y++)
                    generation[i].genes[y] = rand() % map_edges;
        }


        else if(algo == 1){ //elitarstvo
            int i, elite = SIZE_OF_GENERATION / 20;
            
            //vyberiem elitu bez zmeny
            for(i = 0; i < elite; i++)
                new_generation[i] = generation[i];

            for(i; i < SIZE_OF_GENERATION; i++){
                int a = rand() % SIZE_OF_GENERATION, b = rand() % SIZE_OF_GENERATION, c = rand() % SIZE_OF_GENERATION, d = rand() % SIZE_OF_GENERATION;
                while(a == b)
                    b = rand() % SIZE_OF_GENERATION;
                while(c == d)
                    d = rand() % SIZE_OF_GENERATION;
                new_generation[i] = create_new_individual((generation[a].fitnes > generation[b].fitnes) ? generation[a] : generation[b], (generation[c].fitnes > generation[d].fitnes) ? generation[c] : generation[d]);
            }
        }


        else if(algo == 2){ //turnaj
            for(int i = 0; i < SIZE_OF_GENERATION; i++){
                int a = rand() % SIZE_OF_GENERATION, b = rand() % SIZE_OF_GENERATION, c = rand() % SIZE_OF_GENERATION, d = rand() % SIZE_OF_GENERATION;
                while(a == b)
                    b = rand() % SIZE_OF_GENERATION;
                while(c == d)
                    d = rand() % SIZE_OF_GENERATION;
                new_generation[i] = create_new_individual((generation[a].fitnes > generation[b].fitnes) ? generation[a] : generation[b], (generation[c].fitnes > generation[d].fitnes) ? generation[c] : generation[d]);
            }
        }

        if(algo == 1 || algo == 2)
            for(int i = 0; i < SIZE_OF_GENERATION; i++)
                generation[i] = new_generation[i];

        //zmutujem par jedincov
        for(int i = 0; i < SIZE_OF_GENERATION; i++){
            int chance = rand() % 100;
            if(chance < MUTATION_CHANCE)
                generation[i].genes[rand() % NUMBER_OF_GENES] = rand() % map_edges;
        }
    }

    //ak pridem na poslednu generaciu, tak ju ohodnotim a vratim najlepsieho z nej
    for(int i = 0; i < SIZE_OF_GENERATION; i++){
        fitnes(map_to_solve, &generation[i], map_edges);

        if(generation[i].fitnes == map_to_solve->number_of_non_stones){
            printf("\nGeneracia %d\n", count);
            return map_to_solve;
        }
            
        clean_map(map_to_solve);
    }

    qsort(generation, SIZE_OF_GENERATION, sizeof(INDIVIDUAL), comparator);
    printf("Generacia %d\nZahrada nebola cela pohrabana\n%d pohrabanych policok z %d\n", count - 1, generation[0].fitnes, map_to_solve->number_of_non_stones);
    fitnes(map_to_solve, &generation[0], map_edges);
    return map_to_solve;
}

int main(){
    srand(1);
    MAP *map_to_solve = load_map(1);
    draw_map(map_to_solve);

    map_to_solve = solve_map(map_to_solve, 0);
    printf("Vysledna mapa pre random\n");
    draw_map(map_to_solve);
    clean_map(map_to_solve);

    map_to_solve = solve_map(map_to_solve, 1);
    printf("Vysledna mapa pre elitarstvo\n");
    draw_map(map_to_solve);
    clean_map(map_to_solve);

    map_to_solve = solve_map(map_to_solve, 2);
    printf("Vysledna mapa pre turnaj\n");
    draw_map(map_to_solve);
    clean_map(map_to_solve);

    printf("koniec");
    return 0;
}