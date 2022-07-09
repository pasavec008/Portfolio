#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//definovana velkost hashovacej tabulky
#define LENGTH 9758997

//struktura jedneho auta, ukazuje na dalsie
typedef struct car{
    unsigned char id;    //id nam nahradza farbu, 0 je cervene auto
    unsigned char size;
    unsigned char x;
    unsigned char y;
    char direction;
    struct car *next;
}CAR;

//struktura stavu, obsahuje prve auto
typedef struct state{
    CAR *red_car;
    unsigned short depth;
    struct state *parrent;
    struct state *next;
    struct state *previous;
    unsigned char last_used_car_id;
    unsigned char last_used_operator;
}STATE;

//pomocna struktura pre vypis operatorov do vystupu
typedef struct last_used{
    unsigned char last_used_car_id;
    unsigned char last_used_operator;
}LAST_USED;

//funkcia na vykreslenie stavu do konzoly
void draw_state(STATE *state){
    if(!state){
        printf("NEMA RIESENIE\n");
        return;
    }
    printf("\n");
    char parkovisko[6][6];
    CAR *auticko = state->red_car;

    for(int i = 0; i < 6; i++)
        for(int y = 0; y < 6; y++)
            parkovisko[i][y] = '*';

    while(auticko != NULL){
        if(auticko->direction == 'h'){
            if(auticko->y > 5 || auticko->x + auticko->size - 1 > 5)
                printf("Auto uslo, chybny stav\n");
            if(parkovisko[auticko->y][auticko->x] != 42 || parkovisko[auticko->y][auticko->x + 1] != 42 || parkovisko[auticko->y][auticko->x + auticko->size - 1] != 42)
                printf("Buracka, chybny stav\n");
            else{
                parkovisko[auticko->y][auticko->x] = auticko->id;
                parkovisko[auticko->y][auticko->x + 1] = auticko->id;
                parkovisko[auticko->y][auticko->x + auticko->size - 1] = auticko->id;
            }
        }
        else if(auticko->direction == 'v'){
            if(auticko->y + auticko->size - 1 > 5 || auticko->x > 5)
                printf("Auto uslo, chybny stav\n");
            if(parkovisko[auticko->y][auticko->x] != 42 || parkovisko[auticko->y + 1][auticko->x] != 42 || parkovisko[auticko->y + auticko->size - 1][auticko->x] != 42)
                printf("Buracka, chybny stav\n");
            else{
                parkovisko[auticko->y][auticko->x] = auticko->id;
                parkovisko[auticko->y + 1][auticko->x] = auticko->id;
                parkovisko[auticko->y + auticko->size - 1][auticko->x] = auticko->id;
            }
        }
        auticko = auticko->next;
    }

    for(int i = 0; i < 6; i++){
        for(int y = 0; y < 6; y++){
            if(parkovisko[i][y] == 42)
                printf("* ");
            else
                printf("%d ", parkovisko[i][y]);
        }
        printf("\n");
    }
    printf("Depth = %d\n\n", state->depth);   
}

//funkcia na vykreslenie stavu do suboru
void draw_state_to_file(STATE *state, FILE *opened_file){
    fprintf(opened_file, "\n");
    char parkovisko[6][6];
    CAR *auticko = state->red_car;

    for(int i = 0; i < 6; i++)
        for(int y = 0; y < 6; y++)
            parkovisko[i][y] = '*';

    while(auticko != NULL){
        if(auticko->direction == 'h'){
            if(auticko->y > 5 || auticko->x + auticko->size - 1 > 5)
                fprintf(opened_file, "Auto uslo, chybny stav\n");
            if(parkovisko[auticko->y][auticko->x] != 42 || parkovisko[auticko->y][auticko->x + 1] != 42 || parkovisko[auticko->y][auticko->x + auticko->size - 1] != 42)
                fprintf(opened_file, "Buracka, chybny stav\n");
            else{
                parkovisko[auticko->y][auticko->x] = auticko->id;
                parkovisko[auticko->y][auticko->x + 1] = auticko->id;
                parkovisko[auticko->y][auticko->x + auticko->size - 1] = auticko->id;
            }
        }
        else if(auticko->direction == 'v'){
            if(auticko->y + auticko->size - 1 > 5 || auticko->x > 5)
                fprintf(opened_file, "Auto uslo, chybny stav\n");
            if(parkovisko[auticko->y][auticko->x] != 42 || parkovisko[auticko->y + 1][auticko->x] != 42 || parkovisko[auticko->y + auticko->size - 1][auticko->x] != 42)
                fprintf(opened_file, "Buracka, chybny stav\n");
            else{
                parkovisko[auticko->y][auticko->x] = auticko->id;
                parkovisko[auticko->y + 1][auticko->x] = auticko->id;
                parkovisko[auticko->y + auticko->size - 1][auticko->x] = auticko->id;
            }
        }
        auticko = auticko->next;
    }

    for(int i = 0; i < 6; i++){
        for(int y = 0; y < 6; y++){
            if(parkovisko[i][y] == 42)
                fprintf(opened_file, "* ");
            else
                fprintf(opened_file, "%d ", parkovisko[i][y]);
        }
        fprintf(opened_file, "\n");
    }
    fprintf(opened_file, "Depth = %d\n\n", state->depth);
}

//funkcia na nacitanie parametrov zo suboru a vytvorenie prveho stavu
STATE *load_state(char *which_file){
    char name[50], id = 0, loaded;
    sprintf(name, "pociatocne_stavy/%s.txt", which_file);
    FILE *opened_file = fopen(name, "r");
    if(!opened_file){
        printf("Subor sa nepodarilo otvorit.\n");
        exit(27);
    }

    STATE *state = (STATE *)malloc(sizeof(STATE));
    state->depth = 0;
    state->next = NULL;
    state->parrent = NULL;
    state->next = NULL;
    state->previous= NULL;
    CAR *help;

    while(fscanf(opened_file, "%d", &loaded) > 0){
        CAR *new_car = (CAR *)malloc(sizeof(CAR));
        if(!id)
            state->red_car = new_car;
        else
            help->next = new_car;

        new_car->id = id++;
        new_car->size = loaded;
        fscanf(opened_file, "%d %d %s", &new_car->x, &new_car->y, name);
        new_car->direction = name[0];
        new_car->next = NULL;
        help = new_car;
    }
    
    fclose(opened_file);
    return state;
}

//funkcia kontrolujuca, ci stav nie je cielovy
int is_answer(STATE *actual){
    if(actual->red_car->x == 4)
        return 1;
    return 0;
}

//funkcia, ktora spravi zo stavu hash kod
int state_hash(STATE *actual){
    CAR *help = actual->red_car;
    unsigned int hash = actual->red_car->x + actual->red_car->y * 7;
    while(help != NULL){
        hash *= (help->x + 1) * 29;
        hash *= (help->y + 1) * 73;
        help = help->next;
    }
    return hash % LENGTH;
}

//funkcia kontrolujuca, ci dva stavy nahodou nie su duplicitne
int are_same(STATE *state1, STATE *state2){
    if(state2 == state1)
        return 0;
    CAR *help1 = state1->red_car, *help2 = state2->red_car;
    char ok;

    while(help1 != NULL){
        ok = 0;
        while(help2 != NULL){
            if(help1->direction == help2->direction && help1->size == help2->size && help1->x == help2->x && help1->y == help2->y){
                ok = 1;
                break;
            }
            help2 = help2->next;
        }
        if(!ok){
            return 0;
        }
        help1 = help1->next;
        help2 = state2->red_car;
    }
    return 1;
}

//operator pre posun auta hore a vytvorenie noveho stavu
STATE *up(STATE *actual, CAR *car_to_process, STATE **already_processed){
    CAR *help = actual->red_car;
    char x = car_to_process->x, y = car_to_process->y - 1;
    if(y < 0 || car_to_process->direction == 'h')
        return NULL;
    while(help != NULL){
        if(help->direction == 'h'){
            if(help->y != y){
                help = help->next;
                continue;
            }   
            else
                if(help->x == x || help->x + 1 == x || help->x + help->size - 1 == x)
                    return NULL;
        }
        else{
            if(help->x != x){
                help = help->next;
                continue;
            }
            else
                if(help->y + help->size - 1 == y)
                    return NULL;
        }
        help = help->next;
    }

    int hash;
    car_to_process->y--;

    hash = state_hash(actual);
        if(already_processed[hash] != NULL)
        while(already_processed[hash] != NULL){
            if(are_same(actual, already_processed[hash++])){
                car_to_process->y++;
                return NULL;
            }
            if(hash >= LENGTH)
                hash -= LENGTH;
        }
            
    car_to_process->y++;

    STATE *new_state = (STATE *)malloc(sizeof(STATE));
    CAR *help2, *help3;
    int id = car_to_process->id;
    *new_state = *actual;


    help = actual->red_car;

    while(help != NULL){
        help3 = (CAR *)malloc(sizeof(CAR));
        if(help->id == 0)
            new_state->red_car = help3;
        else
            help2->next = help3;

        *help3 = *help;
        help2 = help3;

        if(help3->id == id)
            help3->y--;
        help = help->next;
    }

    //printf("UP\n");
    //draw_state(actual);
    //draw_state(new_state);

    new_state->last_used_car_id = car_to_process->id;
    new_state->last_used_operator = 1;
    new_state->depth = actual->depth + 1;
    new_state->parrent = actual;
    return new_state;
}

//operator pre posun auta dole a vytvorenie noveho stavu
STATE *down(STATE *actual, CAR *car_to_process, STATE **already_processed){
    CAR *help = actual->red_car;
    char x = car_to_process->x, y = car_to_process->y + car_to_process->size;
    if(y > 5 || car_to_process->direction == 'h')
        return NULL;
    while(help != NULL){
        if(help->direction == 'h'){
            if(help->y != y){
                help = help->next;
                continue;
            }   
            else
                if(help->x == x || help->x + 1 == x || help->x + help->size - 1 == x)
                    return NULL;
        }
        else{
            if(help->x != x){
                help = help->next;
                continue;
            }
            else
                if(help->y == y)
                    return NULL;
        }
        help = help->next;
    }

    int hash;
    car_to_process->y++;

    hash = state_hash(actual);
        if(already_processed[hash] != NULL)
        while(already_processed[hash] != NULL){
            if(are_same(actual, already_processed[hash++])){
                car_to_process->y--;
                return NULL;
            }
            if(hash >= LENGTH)
                hash -= LENGTH;
        }
            
    car_to_process->y--;

    STATE *new_state = (STATE *)malloc(sizeof(STATE));
    CAR *help2, *help3;
    int id = car_to_process->id;
    *new_state = *actual;


    help = actual->red_car;

    while(help != NULL){
        help3 = (CAR *)malloc(sizeof(CAR));
        if(help->id == 0)
            new_state->red_car = help3;
        else
            help2->next = help3;

        *help3 = *help;
        help2 = help3;

        if(help3->id == id)
            help3->y++;
        help = help->next;
    }

    //printf("DOWN\n");
    //draw_state(actual);
    //draw_state(new_state);
    new_state->last_used_car_id = car_to_process->id;
    new_state->last_used_operator = 2;
    new_state->depth = actual->depth + 1;
    new_state->parrent = actual;
    return new_state;
}

//operator pre posun auta vlavo a vytvorenie noveho stavu
STATE *left(STATE *actual, CAR *car_to_process, STATE **already_processed){
    CAR *help = actual->red_car;
    char x = car_to_process->x - 1, y = car_to_process->y;
    if(x < 0 || car_to_process->direction == 'v')
        return NULL;
    while(help != NULL){
        if(help->direction == 'v'){
            if(help->x != x){
                help = help->next;
                continue;
            }   
            else
                if(help->y == y || help->y + 1 == y || help->y + help->size - 1 == y)
                    return NULL;
        }
        else{
            if(help->y != y){
                help = help->next;
                continue;
            }
            else
                if(help->x + help->size - 1 == x)
                    return NULL;
        }
        help = help->next;
    }

    int hash;
    car_to_process->x--;

    hash = state_hash(actual);
        if(already_processed[hash] != NULL)
        while(already_processed[hash] != NULL){
            if(are_same(actual, already_processed[hash++])){
                car_to_process->x++;
                return NULL;
            }
            if(hash >= LENGTH)
                hash -= LENGTH;
        }
            
    car_to_process->x++;

    STATE *new_state = (STATE *)malloc(sizeof(STATE));
    CAR *help2, *help3;
    int id = car_to_process->id;
    *new_state = *actual;


    help = actual->red_car;

    while(help != NULL){
        help3 = (CAR *)malloc(sizeof(CAR));
        if(help->id == 0)
            new_state->red_car = help3;
        else
            help2->next = help3;

        *help3 = *help;
        help2 = help3;

        if(help3->id == id)
            help3->x--;
        help = help->next;
    }

    //printf("LEFT\n");
    //draw_state(actual);
    //draw_state(new_state);

    new_state->last_used_car_id = car_to_process->id;
    new_state->last_used_operator = 3;
    new_state->depth = actual->depth + 1;
    new_state->parrent = actual;
    return new_state;
}

//operator pre posun auta vpravo a vytvorenie noveho stavu
STATE *right(STATE *actual, CAR *car_to_process, STATE **already_processed){
    CAR *help = actual->red_car;
    char x = car_to_process->x + car_to_process->size, y = car_to_process->y;
    if(x > 5 || car_to_process->direction == 'v')
        return NULL;
    while(help != NULL){
        if(help->direction == 'v'){
            if(help->x != x){
                help = help->next;
                continue;
            }   
            else
                if(help->y == y || help->y + 1 == y || help->y + help->size - 1 == y)
                    return NULL;
        }
        else{
            if(help->y != y){
                help = help->next;
                continue;
            }
            else
                if(help->x == x)
                    return NULL;
        }
        help = help->next;
    }

    int hash;
    car_to_process->x++;

    hash = state_hash(actual);
        if(already_processed[hash] != NULL)
        while(already_processed[hash] != NULL){
            if(are_same(actual, already_processed[hash++])){
                car_to_process->x--;
                return NULL;
            }
            if(hash >= LENGTH)
                hash -= LENGTH;
        }
            
    car_to_process->x--;

    STATE *new_state = (STATE *)malloc(sizeof(STATE));
    CAR *help2, *help3;
    int id = car_to_process->id;
    *new_state = *actual;


    help = actual->red_car;

    while(help != NULL){
        help3 = (CAR *)malloc(sizeof(CAR));
        if(help->id == 0)
            new_state->red_car = help3;
        else
            help2->next = help3;

        *help3 = *help;
        help2 = help3;

        if(help3->id == id)
            help3->x++;
        help = help->next;
    }

    //printf("RIGHT\n");
    //draw_state(actual);
    //draw_state(new_state);

    new_state->last_used_car_id = car_to_process->id;
    new_state->last_used_operator = 4;
    new_state->depth = actual->depth + 1;
    new_state->parrent = actual;
    return new_state;
}

//funkcia, ktora skusi zo sucasneho stavu spustit vsetky operatory a priradit nove stavy tam, kam patria
STATE *create_new_states(STATE *non_processed, STATE *actual, STATE **already_processed, STATE **non_processed_last){

    CAR *help = actual->red_car;
    STATE *new_state;
    int hash;

    while(help != NULL){
        new_state = up(actual, help, already_processed);
        if(new_state){
            //printf("Vytvoril som z up, vozidlo %d\n", help->id);
            hash = state_hash(new_state);
            while(already_processed[hash] != NULL){
                hash++;
                if(hash >= LENGTH)
                    hash -= LENGTH;
            }
            already_processed[hash] = new_state;

            new_state->next = non_processed;
            new_state->previous = NULL;
            if(non_processed)
                non_processed->previous = new_state;
            non_processed = new_state;
            if(!(*non_processed_last))
                *non_processed_last = new_state;
        }
        new_state = down(actual, help, already_processed);
        if(new_state){
            //printf("Vytvoril som z down, vozidlo %d\n", help->id);
            hash = state_hash(new_state);
            while(already_processed[hash] != NULL){
                hash++;
                if(hash >= LENGTH)
                    hash -= LENGTH;
            }
            already_processed[hash] = new_state;
            
            new_state->next = non_processed;
            new_state->previous = NULL;
            if(non_processed)
                non_processed->previous = new_state;
            non_processed = new_state;
            if(!(*non_processed_last))
                *non_processed_last = new_state;
        }
        new_state = left(actual, help, already_processed);
        if(new_state){
            //printf("Vytvoril som z left, vozidlo %d\n", help->id);
            hash = state_hash(new_state);
            while(already_processed[hash] != NULL){
                hash++;
                if(hash >= LENGTH)
                    hash -= LENGTH;
            }
            already_processed[hash] = new_state;
            
            new_state->next = non_processed;
            new_state->previous = NULL;
            if(non_processed)
                non_processed->previous = new_state;
            non_processed = new_state;
            if(!(*non_processed_last))
                *non_processed_last = new_state;
        }
        new_state = right(actual, help, already_processed);
        if(new_state){
            //printf("Vytvoril som z right, vozidlo %d\n", help->id);
            hash = state_hash(new_state);
            while(already_processed[hash] != NULL){
                hash++;
                if(hash >= LENGTH)
                    hash -= LENGTH;
            }
            already_processed[hash] = new_state;
            
            new_state->next = non_processed;
            new_state->previous = NULL;
            if(non_processed)
                non_processed->previous = new_state;
            non_processed = new_state;
            if(!(*non_processed_last))
                *non_processed_last = new_state;
        }
        help = help->next;
    }

    return non_processed;
}

//hlavna funkcia algoritmov, int algo urcuje, ktory algoritmus sa pouzije
STATE *solve_parking(STATE *starting_state, int algo){
    STATE *non_processed = starting_state, *actual, *non_processed_last = starting_state;
    STATE **already_processed = (STATE **)calloc(9758997, sizeof(STATE));
    int hash;

    while(non_processed){
        if(algo == 1){
            actual = non_processed;         //vyberiem najvhodnejsi, teda posledny zapisany
            non_processed = actual->next;   //odstranim ho z nevyriesenych     
        }
        else if(algo == 2){
            actual = non_processed_last;    //vyberiem najvhodnejsi, teda najstarsi
            non_processed_last = non_processed_last->previous;
            if(non_processed_last == NULL)
                non_processed = NULL;
        }

        //je cielovy?
        if(is_answer(actual))
            return actual;

        //vytvor nove stavy z aktualneho
        non_processed = create_new_states(non_processed, actual, already_processed, &non_processed_last);
    }

    return NULL;
}

//funkcia pre vypis vystupu do suboru
void output_operator_sequence(STATE *starting_state, STATE *final_state, char *which_file, int algo){
    char which[50];
    sprintf(which, "vystupne_subory/%s_%d.txt", which_file, algo);
    FILE *output_file = fopen(which, "w");
    STATE *act = final_state;
    LAST_USED array[10000];

    if(!final_state){
        fprintf(output_file, "Tato krizovatka nema riesenie!\n");
        return;
    }


    int max_depth = final_state->depth;

    while(act->parrent != NULL){
        array[act->depth].last_used_car_id = act->last_used_car_id;
        array[act->depth].last_used_operator = act->last_used_operator;
        act = act->parrent;
    }

    draw_state_to_file(starting_state, output_file);
    for(int i = 1; i <= max_depth; i++){
        fprintf(output_file, "Posunul som auto s id %d smerom ", array[i].last_used_car_id);
        if(array[i].last_used_operator == 1)
            fprintf(output_file, "hore.\n");
        else if(array[i].last_used_operator == 2)
            fprintf(output_file, "dole.\n");
        else if(array[i].last_used_operator == 3)
            fprintf(output_file, "vlavo.\n");
        else if(array[i].last_used_operator == 4)
            fprintf(output_file, "vpravo.\n");
    }
    draw_state_to_file(final_state, output_file);

    fclose(output_file);
}

int main(){

    //nacitanie testovacieho suboru
    char which_file[5];
    int algo;
    printf("Ktory subor chces spustit? Zadaj cislo v rozmedzi 1 - 9\n");
    scanf("%s", which_file);
    printf("Ktory algoritmus chces pouzit?\n1 => hladanie do hlbky\n2 => hladanie do sirky\n");
    scanf("%d", &algo);

    

    STATE *starting_state = load_state(which_file), *final_state;

    draw_state(starting_state);


    clock_t start, end;
    start = clock();
    final_state = solve_parking(starting_state, algo);
    end = clock();
    printf("Time: %.2lf\n", (double)(end - start));

    draw_state(final_state);


    output_operator_sequence(starting_state, final_state, which_file, algo);

    printf("koniec");
    return 0;
}