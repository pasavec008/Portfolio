#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct tile{
    int value;
    int type;
}TILE;

void writeSudoku(TILE **game, int size){
    for(int y = 0; y < size; y++){
        for(int i = 0; i < size; i++){
            if(i % 3 == 0)
                printf(" |");
            if(game[y][i].value == 0)
                printf(" x");
            else
                printf(" %d", game[y][i].value);
        }
        printf(" |\n");
        if((y + 1) % 3 == 0 && y != 0 && y != size - 1){
            for(int x = 0; x < size * 3; x++)
                printf("_");
            printf("\n");
        }
    }
    printf("\n\n");
}

TILE **readSudoku(char *file, int *size){
    FILE *sudoku;
    TILE **game;
    sudoku = fopen(file, "r");
    fscanf(sudoku, "%d", size);

    game = (TILE **)malloc(*size * sizeof(TILE *));
    for(int i = 0; i < *size; i++){
        game[i] = (TILE *)malloc(*size * sizeof(TILE));
        for(int y = 0; y < *size; y++){
            fscanf(sudoku, "%d", &game[i][y].value);
            if(game[i][y].value == 0)
                game[i][y].type = 0;
            else
                game[i][y].type = 1;
        }       
    }
    return game;
}

int validation(TILE **game, int size, int xPosition, int yPosition){
    if(game[yPosition][xPosition].value > size)
        return 0;

    int x, value = game[yPosition][xPosition].value;

    //validacia riadku
    for(x = 0; x < size; x++)
        if(game[yPosition][x].value == value && x != xPosition)
            return 0;

    //validacia stlpca
    for(x = 0; x < size; x++)
        if(game[x][xPosition].value == value && x != yPosition)
            return 0;

    //validacia kocky
    int sqSize = (int)sqrt(size);
    int yControl = yPosition / sqSize * sqSize, xControl = xPosition / sqSize * sqSize;
    for(int y = 0; y < sqSize; y++)
        for(x = 0; x < sqSize; x++)
            if(game[yControl + y][xControl + x].value == value && (yControl + y != yPosition && xControl + x != xPosition))
                return 0;
    
    return 1;
}

int solveSudoku(TILE **game, int size, int xPosition, int yPosition){
    while(yPosition != size){
        if(game[yPosition][xPosition].type != 0){
            xPosition++;
            if(xPosition >= size){
                xPosition = 0;
                yPosition++;
            }
        }
        else{
            game[yPosition][xPosition].value++;
            if(validation(game, size, xPosition, yPosition)){
                xPosition++;
                if(xPosition >= size){
                    xPosition = 0;
                    yPosition++;
                }
            }
            else{
                if(game[yPosition][xPosition].value >= size){
                    game[yPosition][xPosition].value = 0;
                    xPosition--;
                    if(xPosition < 0){
                        xPosition = size - 1;
                        yPosition--;
                        if(yPosition < 0){
                            printf("Nema korektne riesenie.\n");
                            return 1;
                        }
                    }
                    while(game[yPosition][xPosition].type != 0){
                        xPosition--;
                        if(xPosition < 0){
                            xPosition = size - 1;
                            yPosition--;
                            if(yPosition < 0){
                                printf("Nema korektne riesenie.\n");
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

int main(){
    TILE **game;
    int size;
    game = readSudoku("hardest.txt", &size);

    writeSudoku(game, size);

    solveSudoku(game, size, 0, 0);

    writeSudoku(game, size);

    getchar();

    return 0;
}