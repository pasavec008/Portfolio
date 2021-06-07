#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tile{
    char value;
    char type;
    char hidden;
}TILE;

TILE **readMinefield(char *file, int *size){
    FILE *minefield;
    TILE **game;
    minefield = fopen(file, "r");
    fscanf(minefield, "%d", size);

    game = (TILE **)malloc(*size * sizeof(TILE *));
    for(int i = 0; i < *size; i++){
        game[i] = (TILE *)malloc(*size * sizeof(TILE));
        for(int y = 0; y < *size; y++){
            fscanf(minefield, "%d", &game[i][y].value);
            if(game[i][y].value == 0)
                game[i][y].type = 0;
            else
                game[i][y].type = 1;
        }       
    }
    return game;
}

void writeMinefield(TILE **game, int size){
    printf("  ");
    for(int o = 1; o <= size; o++)
        printf(" %d", o);
    printf("\n");
    for(int y = 0; y < size; y++){
        for(int i = 0; i < size; i++){
            if(i == 0)
                printf("%d ", y + 1);
            if(game[y][i].hidden == 1)
                printf("|%c", 254);
            else
                if(game[y][i].type == 1)
                    printf("|*");
                else
                    printf("|%d", game[y][i].value);
        }
        printf("|\n");
    }
    //printf("\n");
}

void checkMines(TILE **game, int x, int y, int size){
    int mines = 0;
    if(x > 0 && y > 0)
        if(game[y - 1][x - 1].type == 1)
            mines++;
    if(y > 0)
        if(game[y - 1][x].type == 1)
            mines++;
    if(x < size - 1 && y > 0)
        if(game[y - 1][x + 1].type == 1)
            mines++;
    if(x > 0)
        if(game[y][x - 1].type == 1)
            mines++;
    if(x < size - 1)
        if(game[y][x + 1].type == 1)
            mines++;
    if(x > 0 && y < size - 1)
        if(game[y + 1][x - 1].type == 1)
            mines++;
    if(y < size - 1)
        if(game[y + 1][x].type == 1)
            mines++;
    if(x < size - 1 && y < size - 1)
        if(game[y + 1][x + 1].type == 1)
            mines++;

    game[y][x].value = mines;
}

void setValues(TILE **game, int size){
    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            if(game[y][x].type != 1){
                checkMines(game, x, y, size);
            }
        }
    }
}

TILE **generateMineField(int size, int numberOfMines){
    TILE **game;
    int x,y;

    game = (TILE **)malloc(size * sizeof(TILE *));
    for(int i = 0; i < size; i++){
        game[i] = (TILE *)malloc(size * sizeof(TILE));
        for(int y = 0; y < size; y++){
            game[i][y].type = 0;
            game[i][y].hidden = 1;
        }    
    }
    for(int i = 0; i < numberOfMines; i++){
        x = rand() % size;
        y = rand() % size;
        if(game[y][x].type == 1)
            i--;
        else
            game[y][x].type = 1;
    }

    setValues(game, size);

    return game;
}

int move(TILE **game, int x, int y, int size){
    game[y][x].hidden = 0;

    if(game[y][x].type == 1){
        printf("Prehral si!\n");
        return 0;
    }

    if(game[y][x].value == 0){
        if(x > 0 && y > 0 && game[y - 1][x - 1].hidden != 0)
            move(game, x - 1, y - 1, size);
        if(y > 0 && game[y - 1][x].hidden != 0)
            move(game, x, y - 1, size);
        if(x < size - 1 && y > 0 && game[y - 1][x + 1].hidden != 0)
            move(game, x + 1, y - 1, size);
        if(x > 0 && game[y][x - 1].hidden != 0)
            move(game, x - 1, y, size);
        if(x < size - 1 && game[y][x + 1].hidden != 0)
            move(game, x + 1, y, size);
        if(x > 0 && y < size - 1 && game[y + 1][x - 1].hidden != 0)
            move(game, x - 1, y + 1, size);
        if(y < size - 1 && game[y + 1][x].hidden != 0)
            move(game, x, y + 1, size);
        if(x < size - 1 && y < size - 1 && game[y + 1][x + 1].hidden != 0)
            move(game, x + 1, y + 1, size);
    }
    
    return 1;
}

int main(){
    srand(time(0));
    TILE **game;
    int size, x, y, numberOfMines;

    printf("Zvol si velkost pola a pocet min v poli, napriklad: 9 10 (pole bude mat velkost 9x9 a bude v nom 10 min)\n");
    scanf("%d %d", &size, &numberOfMines);

    game = generateMineField(size, numberOfMines);

    writeMinefield(game, size);
    printf("Policko odhalis napisanim dvojice suradnic, napriklad: 1 1 (odhali lavy horny roh)\n");

    while(scanf("%d %d", &x, &y) != 0){
        if(x <= size && x > 0 && y <= size && y > 0)
            if(!move(game, x - 1, y - 1, size))
                break;
        writeMinefield(game, size);
        printf("Policko odhalis napisanim dvojice suradnic\n");
    }

    writeMinefield(game, size);

    return 0;
}