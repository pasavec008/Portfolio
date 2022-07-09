#include <stdio.h>

void *start;


int create_free_block_in_list(void *position, int size, int offset_previous, int offset_next){
    char GLOBAL_INT_SIZE = *((char *)start);

    //nastavime bloku hlavicku a paticku
    if(GLOBAL_INT_SIZE == 1){
        *((char *)position) = (char)offset_previous;
        *((char *)position + GLOBAL_INT_SIZE) = (char)offset_next;
        *((char *)position + 2 * GLOBAL_INT_SIZE) = (char)size;
        *((char *)position + (3 + size)) = (char)size;
    }
    else if(GLOBAL_INT_SIZE == 2){
        *((short int *)position) = (short int)offset_previous;
        *((short int *)(position + GLOBAL_INT_SIZE)) = (short int)offset_next;
        *((short int *)(position + 2 * GLOBAL_INT_SIZE)) = (short int)size;
        *((short int *)(position + (3 * GLOBAL_INT_SIZE + size))) = (short int)size;
    }
    else{
        *((int *)position) = (int)offset_previous;
        *((int *)(position + GLOBAL_INT_SIZE)) = (int)offset_next;
        *((int *)(position + 2 * GLOBAL_INT_SIZE)) = (int)size;
        *((int *)(position + (3 * GLOBAL_INT_SIZE + size))) = (int)size;
    }

    return position - start;
}

void memory_init(void *ptr, unsigned int size){
    start = ptr;

    //podla velkosti celkovej pamate urcime, ktory datovy typ sa bude pouzivat v programe
    if(size < 127)
        *((int *)ptr) = 1;
    else if(size < 32766)
         *((int *)ptr) = 2;
    else
        *((int *)ptr) = 4;
    
    char GLOBAL_INT_SIZE = *((char *)start);

    void *position = ptr + 1 + GLOBAL_INT_SIZE;
    int max_block_size = 16, size_copy = size - 1 - GLOBAL_INT_SIZE;

    //urcime velkost najvacsieho spajaneho zoznamu podla velkosti celkovej pamate
    while(max_block_size  < size_copy){
        if(GLOBAL_INT_SIZE == 1)
            *((char *)position) = -1;
        else if(GLOBAL_INT_SIZE == 2)
            *((short int *)position) = -1;
        else
            *((int *)position) = -1;
        position += GLOBAL_INT_SIZE;
        max_block_size *= 2;
        size_copy -= GLOBAL_INT_SIZE;
    }

    //zapiseme velkost najvacsieho spajaneho zoznamu na zaciatok pamate
    if(GLOBAL_INT_SIZE == 1)
        *((char *)(ptr + 1)) = max_block_size - 1;
    else if(GLOBAL_INT_SIZE == 2)
        *((short int *)(ptr + 1)) = max_block_size - 1;
    else
        *((int *)(ptr + 1)) = max_block_size - 1;

    size_copy -= 4 * GLOBAL_INT_SIZE;   //odratanie pamate potrebnej pre hlavicku a paticku

    //za spajanymi zoznamami vytvorime jeden velky blok zo zvyšnej pamate
    if(GLOBAL_INT_SIZE == 1)
        *((char *)(position)) = create_free_block_in_list(position + GLOBAL_INT_SIZE, size_copy, position - start, -1);
    else if(GLOBAL_INT_SIZE == 2)
        *((short int *)(position)) = create_free_block_in_list(position + GLOBAL_INT_SIZE, size_copy, position - start, -1);
    else
        *((int *)(position)) = create_free_block_in_list(position + GLOBAL_INT_SIZE, size_copy, position - start, -1);
}

int memory_check(void *ptr){
    char GLOBAL_INT_SIZE = *((char *)start);
    int main_header = GLOBAL_INT_SIZE + 1, max_size, control_size1, control_size2;

    //ziskame velkost bloku, ak je vacsia, ako minimalne alokovany blok, rovno vratime 0 (obsadene bloky znacime zapornymi cislami)
    control_size1 = GLOBAL_INT_SIZE == 1 ? *(char *)(ptr - GLOBAL_INT_SIZE) : GLOBAL_INT_SIZE == 2 ? *(short int *)(ptr - GLOBAL_INT_SIZE) : *(int *)(ptr - GLOBAL_INT_SIZE);
    if(control_size1 > -8)
        return 0;

    max_size = GLOBAL_INT_SIZE == 1 ? *(char *)(start + 1)
    : GLOBAL_INT_SIZE == 2 ? *(short int *)(start + 1) : *(int *)(start + 1);

    while(max_size > 16){
        main_header += GLOBAL_INT_SIZE;
        max_size /= 2;
    }

    //ak sa kontrolovana adresa nachadza v hlavnej hlavicke, ktora neprislucha ziadnemu bloku, vratime 0
    if(main_header > ptr - start)
        return 0;

    control_size2 = GLOBAL_INT_SIZE == 1 ? *(char *)(ptr - control_size1) : GLOBAL_INT_SIZE == 2 ? *(short int *)(ptr - control_size1) : *(int *)(ptr - control_size1);
    
    //ak sa rovna hodnota velkosti bloku v hlavicke a paticke, tak vratime 1
    if(control_size1 == control_size2 && (start + (start - ptr)) < start + max_size)
        return 1;

    return 0;
}

void sort_to_list(int offset, int size_of_cutted_block){
    char GLOBAL_INT_SIZE = *((char *)start);
    int list_size = 16, index = 1;

    //urcime podla velkosti, do ktoreho zoznamu patri blok, ktory priradujeme 
    while(size_of_cutted_block + 2 * GLOBAL_INT_SIZE > list_size){
        index++;
        list_size *= 2;
    }

    //upravime offsety v hlavickach a tym priradime blok na zaciatku zoznamu
    if(GLOBAL_INT_SIZE == 1){
        *(char *)(start + *(char *)(start + offset)) = *(char *)(start + offset + GLOBAL_INT_SIZE);
        *(char *)(start + offset + GLOBAL_INT_SIZE) = *(char *)(start + 1 + index * GLOBAL_INT_SIZE);
        *(char *)(start + 1 + index * GLOBAL_INT_SIZE) = offset;
        *(char *)(start + offset) = (char)(1 + index * GLOBAL_INT_SIZE);
        if(*(char *)(start + offset + GLOBAL_INT_SIZE) > 0)
            *(char *)(start + *(char *)(start + offset + GLOBAL_INT_SIZE)) = offset;
    }
    else if(GLOBAL_INT_SIZE == 2){
        *(short int *)(start + *(short int *)(start + offset)) = *(short int *)(start + offset + GLOBAL_INT_SIZE);
        *(short int *)(start + offset + GLOBAL_INT_SIZE) = *(short int *)(start + 1 + index * GLOBAL_INT_SIZE);
        *(short int *)(start + 1 + index * GLOBAL_INT_SIZE) = offset;
        *(short int *)(start + offset) = (short int)(1 + index * GLOBAL_INT_SIZE);
        if(*(short int *)(start + offset + GLOBAL_INT_SIZE) > 0)
            *(short int *)(start + *(short int *)(start + offset + GLOBAL_INT_SIZE)) = offset;
    }
    else{
        *(int *)(start + *(int *)(start + offset)) = *(int *)(start + offset + GLOBAL_INT_SIZE);
        *(int *)(start + offset + GLOBAL_INT_SIZE) = *(int *)(start + 1 + index * GLOBAL_INT_SIZE);
        *(int *)(start + 1 + index * GLOBAL_INT_SIZE) = offset;
        *(int *)(start + offset) = (int)(1 + index * GLOBAL_INT_SIZE);
        if(*(int *)(start + offset + GLOBAL_INT_SIZE) > 0)
            *(int *)(start + *(int *)(start + offset + GLOBAL_INT_SIZE)) = offset;
    }
}

void *cut_block(int offset, int size_of_cutted_block, int size_of_new_block){
    char GLOBAL_INT_SIZE = *((char *)start);

    //nastavime velkosti do hlavicky a paticky a volny blok posleme do funkcie priradenia do zoznamu volnych blokov
    if(GLOBAL_INT_SIZE == 1){
        *(char *)(start + offset + 2 * GLOBAL_INT_SIZE) = size_of_cutted_block;
        *(char *)(start + offset + 3 * GLOBAL_INT_SIZE + size_of_cutted_block) = size_of_cutted_block;
        sort_to_list(offset, size_of_cutted_block);
        *(char *)(start + offset + 4 * GLOBAL_INT_SIZE + size_of_cutted_block) = -size_of_new_block;
        *(char *)(start + offset + 5 * GLOBAL_INT_SIZE + size_of_cutted_block + size_of_new_block) = -size_of_new_block;
    }
    else if(GLOBAL_INT_SIZE == 2){
        *(short int *)(start + offset + 2 * GLOBAL_INT_SIZE) = size_of_cutted_block;
        *(short int *)(start + offset + 3 * GLOBAL_INT_SIZE + size_of_cutted_block) = size_of_cutted_block;
        sort_to_list(offset, size_of_cutted_block);
        *(short int *)(start + offset + 4 * GLOBAL_INT_SIZE + size_of_cutted_block) = -size_of_new_block;
        *(short int *)(start + offset + 5 * GLOBAL_INT_SIZE + size_of_cutted_block + size_of_new_block) = -size_of_new_block;
    }
    else{
        *(int *)(start + offset + 2 * GLOBAL_INT_SIZE) = size_of_cutted_block;
        *(int *)(start + offset + 3 * GLOBAL_INT_SIZE + size_of_cutted_block) = size_of_cutted_block;
        sort_to_list(offset, size_of_cutted_block);
        *(int *)(start + offset + 4 * GLOBAL_INT_SIZE + size_of_cutted_block) = -size_of_new_block;
        *(int *)(start + offset + 5 * GLOBAL_INT_SIZE + size_of_cutted_block + size_of_new_block) = -size_of_new_block;
    }

    //vraciame pointer na alokovany blok
    return (start + offset + 5 * GLOBAL_INT_SIZE + size_of_cutted_block);
}

void *expand(int offset, int size){
    //mensia funkcia sluziaca na rozsirenie bloku o hlavicku, ktoru v alokovanom bloku nepotrebujeme
    char GLOBAL_INT_SIZE = *((char *)start);
    if(GLOBAL_INT_SIZE == 1){
        *(char *)(start + offset) = -(size + 2 * GLOBAL_INT_SIZE);
        *(char *)(start + offset + size + 3 * GLOBAL_INT_SIZE) = -(size + 2 * GLOBAL_INT_SIZE);
    }
    else if(GLOBAL_INT_SIZE == 2){
        *(short int *)(start + offset) = -(size + 2 * GLOBAL_INT_SIZE);
        *(short int *)(start + offset + size + 3 * GLOBAL_INT_SIZE) = -(size + 2 * GLOBAL_INT_SIZE);
    }
    else{
        *(int *)(start + offset) = -(size + 2 * GLOBAL_INT_SIZE);
        *(int *)(start + offset + size + 3 * GLOBAL_INT_SIZE) = -(size + 2 * GLOBAL_INT_SIZE);
    }
    return (start + offset + GLOBAL_INT_SIZE);
}

void *handle_block(int offset, int size){
    //tato funkcia rozhoduje o tom, ci ma zmysel blok rozdelit na dva mensie, alebo ci ho rovno posleme "expandovany" uzivatelovi
    char GLOBAL_INT_SIZE = *((char *)start);
    int size_of_main_block = GLOBAL_INT_SIZE == 1 ? *((char *)(start + offset + 2 * GLOBAL_INT_SIZE)) : GLOBAL_INT_SIZE == 2 ? *((short int *)(start + offset + 2 * GLOBAL_INT_SIZE)) : *((int *)(start + offset + 2 * GLOBAL_INT_SIZE));
    int size_of_new_block = size, size_of_cutted_block = size_of_main_block - size - 2 * GLOBAL_INT_SIZE;

    //rozhoduje sa na zaklade velkosti, ak by bol rozdeleny blok prilis maly pre minimalnu alokaciu, tak program blok nerozdeli
    if(size_of_cutted_block < 8){
        int previous_offset, next_offset;
        if(GLOBAL_INT_SIZE == 1){
            previous_offset = *(char *)(start + offset), next_offset = *(char *)(start + offset + GLOBAL_INT_SIZE);
            *(char *)(start + previous_offset) = next_offset;
            if(next_offset > 0){
                *(char *)(start + next_offset) = previous_offset;
            }
        }
        else if(GLOBAL_INT_SIZE == 2){
            previous_offset = *(short int *)(start + offset), next_offset = *(short int *)(start + offset + GLOBAL_INT_SIZE);
            *(short int *)(start + previous_offset) = next_offset;
            if(next_offset > 0){
                *(short int *)(start + next_offset) = previous_offset;
            }
        }
        else{
            previous_offset = *(int *)(start + offset), next_offset = *(int *)(start + offset + GLOBAL_INT_SIZE);
            *(int *)(start + previous_offset) = next_offset;
            if(next_offset > 0){
                *(int *)(start + next_offset) = previous_offset;
            }
        }
        return expand(offset, size_of_main_block);
    }
    

    return cut_block(offset, size_of_cutted_block, size_of_new_block);
}

int find_suitable_offset(void *actual_position, int max_size, int size, char GLOBAL_INT_SIZE, int list_size){
    int offset;
    max_size++;
    //tato funkcia hlada vhodny blok pre uspesnu alokaciu
    while(list_size <= max_size){

        offset = *(char *)(start) == 1 ? *(char *)actual_position : *(char *)(start) == 2 ? *(short int *)actual_position : *(char *)(start) == 4 ? *(int *)actual_position : 0;

        //ak zoznam neukazuje na ziaden prvok, rovno prejdeme na dalsi
        while(offset <= 0){
            actual_position += GLOBAL_INT_SIZE;
            offset = *(char *)(start) == 1 ? *(char *)actual_position : *(char *)(start) == 2 ? *(short int *)actual_position : *(char *)(start) == 4 ? *(int *)actual_position : 0;
            list_size *= 2;
        }

        //ak by sme mali prehladavat neexistujuce zoznamy, tak vyhlasime, ze sme nenasli vhodny blok
        if(list_size > max_size){
            //printf("Nenasiel som vhodny blok.\n");
            return 0;
        }

        //prehladavame dany zoznam
        while(offset > 0){
            if(*((int *)(start + offset + 2 * GLOBAL_INT_SIZE)) >= (size - 2 * GLOBAL_INT_SIZE))
                return offset;
            offset = *(char *)(start) == 1 ? *(char *)(start + offset + GLOBAL_INT_SIZE) : *(char *)(start) == 2 ? *(short int *)(start + offset + GLOBAL_INT_SIZE)
                    : *(char *)(start) == 4 ? *(int *)(start + offset + GLOBAL_INT_SIZE) : 0;
        }

        actual_position += GLOBAL_INT_SIZE;
        list_size *= 2;
    }

    //printf("Nenasiel som vhodny blok.\n");
    return 0;
}

void *memory_alloc(unsigned int size){
    char GLOBAL_INT_SIZE = *((char *)start);

    if(GLOBAL_INT_SIZE == 4 && size == 8)       //špeciálny prípad pri väčších pamätiach, kedy by sa vytvoril prázdny blok s veľkosťou 0
        size = 12;

    int list_size = 16, max_size, offset;
    void *actual_position = start + 1 + GLOBAL_INT_SIZE;
    max_size = *(char *)(start) == 1 ? *(char *)(start + 1) : *(char *)(start) == 2 ? *(short int *)(start + 1) : *(char *)(start) == 4 ? *(int *)(start + 1) : 0;
    
    //ak je pozadovana velkost vacsia ako velkost najvacsieho zoznamu, tak vratime NULL
    if(size > max_size){
        //printf("Nenasiel som vhodny blok\n");
        return NULL;
    }
    while(size > list_size){
        list_size *= 2;
        actual_position += GLOBAL_INT_SIZE;
        if(max_size < list_size){
            //printf("Nenasiel som vhodny blok");
            return NULL;
        }
    }
    offset = find_suitable_offset(actual_position, max_size, size, GLOBAL_INT_SIZE, list_size);
    if(offset <= 0)
        return NULL;
    return handle_block(offset, size);
}

int left_check(void *ptr){
    //funkcia, ktora kontroluje, ci vlavo od zadaneho pointera je volny blok
    int size_value;
    char GLOBAL_INT_SIZE = *((char *)start);
    if(GLOBAL_INT_SIZE == 1){
        size_value = *(char *)(ptr - 2 * GLOBAL_INT_SIZE);
        if(size_value <= 0 || size_value != *(char *)(ptr - 3 *GLOBAL_INT_SIZE - size_value))
            return 0;
        return size_value;
    }
    else if(GLOBAL_INT_SIZE == 2){
        size_value = *(short int *)(ptr - 2 * GLOBAL_INT_SIZE);
        if(size_value <= 0 || size_value != *(short int *)(ptr - 3 *GLOBAL_INT_SIZE - size_value))
            return 0;
        return size_value;
    }
    else{
        size_value = *(int *)(ptr - 2 * GLOBAL_INT_SIZE);
        if(size_value <= 0 || size_value != *(int *)(ptr - 3 *GLOBAL_INT_SIZE - size_value))
            return 0;
        return size_value;
    }
}

int right_check(void *ptr){
    //funkcia, ktora kontroluje, ci vpravo od zadaneho pointera je volny blok
    int size_value, size_of_checked_block;
    char GLOBAL_INT_SIZE = *((char *)start);
    if(GLOBAL_INT_SIZE == 1){
        size_of_checked_block = -*(char *)(ptr - GLOBAL_INT_SIZE);
        size_value = *(char *)(ptr + size_of_checked_block + 3 * GLOBAL_INT_SIZE);
        if(size_value <= 0 || size_value != *(char *)(ptr + size_of_checked_block + 4 * GLOBAL_INT_SIZE + size_value))
            return 0;
        return size_value;
    }
    else if(GLOBAL_INT_SIZE == 2){
        size_of_checked_block = -*(short int *)(ptr - GLOBAL_INT_SIZE);
        size_value = *(short int *)(ptr + size_of_checked_block + 3 * GLOBAL_INT_SIZE);
        if(size_value <= 0 || size_value != *(short int *)(ptr + size_of_checked_block + 4 * GLOBAL_INT_SIZE + size_value))
            return 0;
        return size_value;
    }
    else{
        size_of_checked_block = -*(int *)(ptr - GLOBAL_INT_SIZE);
        size_value = *(int *)(ptr + size_of_checked_block + 3 * GLOBAL_INT_SIZE);
        if(size_value <= 0 || size_value != *(int *)(ptr + size_of_checked_block + 4 * GLOBAL_INT_SIZE + size_value))
            return 0;
        return size_value;
    }
}

int right_merge(void *ptr, int size){
    //funkcia spajajuca blok s volnym blokom vpravo
    char GLOBAL_INT_SIZE = *((char *)start);
    int offset_old_cur, offset_old_pre, offset_old_nex;

    //najdeme offsety bloku(A) vpravo, bloku(B), ktory ukazuje na blok(A) a taktiez blok(C), na ktory ukazuje blok(A)
    //pomocou nich potom "uvolnime" blok(A) a spojime ho
    if(GLOBAL_INT_SIZE == 1){
        offset_old_cur = (char)(ptr + size + GLOBAL_INT_SIZE - start);
        offset_old_pre = *(char *)(start + offset_old_cur);
        offset_old_nex = *(char *)(start + offset_old_cur + GLOBAL_INT_SIZE);
        if(*(char *)(start + offset_old_pre) == offset_old_cur)
            *(char *)(start + offset_old_pre) = offset_old_nex;
        else
            *(char *)(start + offset_old_pre) = offset_old_nex;

        if(offset_old_nex > 0)
                *(char *)(start + offset_old_nex) = offset_old_pre;

        size += *(char *)(start + offset_old_cur + 2 * GLOBAL_INT_SIZE);
    }
    else if(GLOBAL_INT_SIZE == 2){
        offset_old_cur = (short int)(ptr + size + GLOBAL_INT_SIZE - start);
        offset_old_pre = *(short int *)(start + offset_old_cur);
        offset_old_nex = *(short int *)(start + offset_old_cur + GLOBAL_INT_SIZE);
        if(*(short int *)(start + offset_old_pre) == offset_old_cur)
            *(short int *)(start + offset_old_pre) = offset_old_nex;
        else
            *(short int *)(start + offset_old_pre) = offset_old_nex;

        if(offset_old_nex > 0)
                *(short int *)(start + offset_old_nex) = offset_old_pre;

        size += *(short int *)(start + offset_old_cur + 2 * GLOBAL_INT_SIZE);
    }
    else{
        offset_old_cur = (int)(ptr + size + GLOBAL_INT_SIZE - start);
        offset_old_pre = *(int *)(start + offset_old_cur);
        offset_old_nex = *(int *)(start + offset_old_cur + GLOBAL_INT_SIZE);
        if(*(int *)(start + offset_old_pre) == offset_old_cur)
            *(int *)(start + offset_old_pre) = offset_old_nex;
        else
            *(int *)(start + offset_old_pre) = offset_old_nex;

        if(offset_old_nex > 0)
                *(int *)(start + offset_old_nex) = offset_old_pre;

        size += *(int *)(start + offset_old_cur + 2 * GLOBAL_INT_SIZE);
    }
    return size;
}

int left_merge(void *ptr, int size){
    //funkcia spajajuca blok s volnym blokom vlavo
    char GLOBAL_INT_SIZE = *((char *)start);
    int offset_old_cur, offset_old_pre, offset_old_nex;

    //najdeme offsety bloku(A) vpravo, bloku(B), ktory ukazuje na blok(A) a taktiez blok(C), na ktory ukazuje blok(A)
    //pomocou nich potom "uvolnime" blok(A) a spojime ho
    if(GLOBAL_INT_SIZE == 1){
        offset_old_cur = (char)((ptr - 5 * GLOBAL_INT_SIZE) - *(char *)(ptr - 2 * GLOBAL_INT_SIZE) - start);
        offset_old_pre = *(char *)(start + offset_old_cur);
        offset_old_nex = *(char *)(start + offset_old_cur + GLOBAL_INT_SIZE);
        if(*(char *)(start + offset_old_pre) == offset_old_cur)
            *(char *)(start + offset_old_pre) = offset_old_nex;
        else
            *(char *)(start + offset_old_pre) = offset_old_nex;

        if(offset_old_nex > 0)
                *(char *)(start + offset_old_nex) = offset_old_pre;

        size += *(char *)(start + offset_old_cur + 2 * GLOBAL_INT_SIZE);
    }
    else if(GLOBAL_INT_SIZE == 2){
        offset_old_cur = (short int)((ptr - 5 * GLOBAL_INT_SIZE) - *(short int *)(ptr - 2 * GLOBAL_INT_SIZE) - start);
        offset_old_pre = *(short int *)(start + offset_old_cur);
        offset_old_nex = *(short int *)(start + offset_old_cur + GLOBAL_INT_SIZE);
        if(*(short int *)(start + offset_old_pre) == offset_old_cur)
            *(short int *)(start + offset_old_pre) = offset_old_nex;
        else
            *(short int *)(start + offset_old_pre) = offset_old_nex;

        if(offset_old_nex > 0)
                *(short int *)(start + offset_old_nex) = offset_old_pre;
        size += *(short int *)(start + offset_old_cur + 2 * GLOBAL_INT_SIZE);
    }
    else{
        offset_old_cur = (int)((ptr - 5 * GLOBAL_INT_SIZE) - *(int *)(ptr - 2 * GLOBAL_INT_SIZE) - start);
        offset_old_pre = *(int *)(start + offset_old_cur);
        offset_old_nex = *(int *)(start + offset_old_cur + GLOBAL_INT_SIZE);
        if(*(int *)(start + offset_old_pre) == offset_old_cur)
            *(int *)(start + offset_old_pre) = offset_old_nex;
        else
            *(int *)(start + offset_old_pre) = offset_old_nex;

        if(offset_old_nex > 0)
                *(int *)(start + offset_old_nex) = offset_old_pre;
        size += *(int *)(start + offset_old_cur + 2 * GLOBAL_INT_SIZE);
    }
    return size;
}

int merge(void *ptr, int left, int right, int size){
    //funkcia riadiaca spajanie blokov
    char GLOBAL_INT_SIZE = *((char *)start);
    int offset_old_cur, offset_old_pre, offset_old_nex;


    if(right > 0)
        size = right_merge(ptr, size);

    //ak pripojim blok zlava, musim sa prepocitat na hlavicku spojeneho bloku
    if(left > 0){
        size = left_merge(ptr, size);
        offset_old_cur = GLOBAL_INT_SIZE == 1 ? (char)((ptr - 5 * GLOBAL_INT_SIZE) - *(char *)(ptr - 2 * GLOBAL_INT_SIZE) - start) :
        GLOBAL_INT_SIZE == 2 ? (short int)((ptr - 5 * GLOBAL_INT_SIZE) - *(short int *)(ptr - 2 * GLOBAL_INT_SIZE) - start) :
        (int)((ptr - 5 * GLOBAL_INT_SIZE) - *(int *)(ptr - 2 * GLOBAL_INT_SIZE) - start);
        ptr = start + offset_old_cur;
    }
    else
        ptr -= GLOBAL_INT_SIZE;

    if(right > 0)
        size += 4 * GLOBAL_INT_SIZE;
    if(left > 0)
        size += 4 * GLOBAL_INT_SIZE;
    size -= 2 * GLOBAL_INT_SIZE;

    //zapis velkosti do hlavicky a paticky
    if(GLOBAL_INT_SIZE == 1){
        *(char *)(ptr + 2 * GLOBAL_INT_SIZE) = (char)size;
        *(char *)(ptr + 3 * GLOBAL_INT_SIZE + size) = (char)size;
    }
    else if(GLOBAL_INT_SIZE == 2){
        *(short int *)(ptr + 2 * GLOBAL_INT_SIZE) = (short int)size;
        *(short int *)(ptr + 3 * GLOBAL_INT_SIZE + size) = (short int)size;
    }
    else{
        *(int *)(ptr + 2 * GLOBAL_INT_SIZE) = (int)size;
        *(int *)(ptr + 3 * GLOBAL_INT_SIZE + size) = (int)size;
    }


    sort_to_list((int)(ptr - start), size);
    
    return 0;
}

int memory_free(void *valid_ptr){

    //program skontroluje platnost pointera
    if(memory_check(valid_ptr) == 0)
        return 1;

    //prepocitame sa k velkosti a spolu s pointerom ich posleme do dalsich funkcii
    char GLOBAL_INT_SIZE = *((char *)start);
    int size, control_size, main_header = GLOBAL_INT_SIZE + 1, max_size;
    size = GLOBAL_INT_SIZE == 1 ? *(char *)(valid_ptr - GLOBAL_INT_SIZE) : GLOBAL_INT_SIZE == 2 ? *(short int *)(valid_ptr - GLOBAL_INT_SIZE) : *(int *)(valid_ptr - GLOBAL_INT_SIZE);

    return merge(valid_ptr, left_check(valid_ptr), right_check(valid_ptr), -size);
}


#include<stdlib.h>

void memory_scheme(void *memory, int x){
    int i, y;
    void *memory_copy = memory;
    char GLOBAL_INT_SIZE = *((char *)start);
    if(GLOBAL_INT_SIZE == 1){

        /*for(i = 0; i < x; i++){
            for(y = 0; y < 4; y++){
                printf("%.3d %.3d %.3d %.3d     ", ((char *)memory_copy), ((char *)(memory_copy+2)), ((char *)(memory_copy+4)), ((char *)(memory_copy+6)));
                memory_copy += 8;
            }
            printf("\n");
        }*/

        memory_copy = memory;
        printf("Vypisujem char\n");
        printf("%.3d \n", *((char *)memory_copy));
        memory_copy++;

        for(i = 0; i < x; i++){
            for(y = 0; y < 4; y++){
                printf("%.3d %.3d %.3d %.3d     ", *((char *)memory_copy), *((char *)(memory_copy+1)), *((char *)(memory_copy+2)), *((char *)(memory_copy+3)));
                memory_copy += 4;
            }
            printf("\n");
        }
    }
    else if(GLOBAL_INT_SIZE == 2){

        /*for(i = 0; i < x; i++){
            for(y = 0; y < 4; y++){
                printf("%.3d %.3d %.3d %.3d     ", ((char *)memory_copy), ((char *)(memory_copy+2)), ((char *)(memory_copy+4)), ((char *)(memory_copy+6)));
                memory_copy += 8;
            }
            printf("\n");
        }*/

        memory_copy = memory;
        printf("Vypisujem short\n");
        printf("%.5d \n", *((char *)memory_copy));
        memory_copy++;
        for(i = 0; i < x; i++){
            for(y = 0; y < 3; y++){
                printf("%.5d %.5d %.5d %.5d     ", *((short int *)memory_copy), *((short int *)(memory_copy+2)), *((short int *)(memory_copy+4)), *((short int *)(memory_copy+6)));
                memory_copy += 8;
            }
            printf("\n");
        }
    }
    else{

        /*for(i = 0; i < x; i++){
            for(y = 0; y < 4; y++){
                printf("%.3d %.3d %.3d %.3d     ", ((char *)memory_copy), ((char *)(memory_copy+4)), ((char *)(memory_copy+8)), ((char *)(memory_copy+12)));
                memory_copy += 16;
            }
            printf("\n");
        }*/

        memory_copy = memory;
        printf("Vypisujem int\n");
        printf("%.8d \n", *((char *)memory_copy));
        memory_copy++;
        for(i = 0; i < x; i++){
            for(y = 0; y < 4; y++){
                printf("%.8d %.8d %.8d %.8d     ", *((int *)memory_copy), *((int *)(memory_copy+4)), *((int *)(memory_copy+8)), *((int *)(memory_copy+12)));
                memory_copy += 16;
            }
            printf("\n");
        }
    }
    

    printf("\n");
}

int test_static(int velkost_bloku){
    int uspesne_alokovana = 0, i, k[200000], y = 0;
    char GLOBAL_INT_SIZE = *((char *)start);
    char *o;

    while (o = memory_alloc(velkost_bloku)){
        printf("%d\n",(int)o-(int)start);
        k[y] = (int)o;
        uspesne_alokovana += velkost_bloku;
        for(i = 0; i < velkost_bloku; i += GLOBAL_INT_SIZE){
            o[i] = velkost_bloku - i;
            if(GLOBAL_INT_SIZE == 2)
                o[i + 1] = 0;
        }
        //for(i = 0; i < velkost_bloku; i += GLOBAL_INT_SIZE)
        //    printf("%d\n", o[i]);
        y++;
    }


    for(i = 0; i < y; i++)
        memory_free((void *)k[i]);

    return uspesne_alokovana;
}

int test_random(int velkost_bloku_od, int velkost_bloku_do){
    int uspesne_alokovana = 0, i, k[200000], y = 0;
    char GLOBAL_INT_SIZE = *((char *)start);
    char *o;

    int r = rand() % (velkost_bloku_do - velkost_bloku_od + 1) + velkost_bloku_od;
    while(r % 4 != 0)
            r = rand() % (velkost_bloku_do - velkost_bloku_od + 1) + velkost_bloku_od;

    while (o = memory_alloc(r)){
        printf("%d\n",(int)o-(int)start);
        k[y] = (int)o;
        uspesne_alokovana += r;
        for(i = 0; i < r; i += GLOBAL_INT_SIZE){
            o[i] = r - i;
            if(GLOBAL_INT_SIZE == 2)
                o[i + 1] = 0;
            if(GLOBAL_INT_SIZE == 4){
                o[i + 2] = 0;
                o[i + 3] = 0;
            }
        }
        //for(i = 0; i < r; i += GLOBAL_INT_SIZE)
        //    printf("%d\n", o[i]);
        r = rand() % (velkost_bloku_do - velkost_bloku_od + 1) + velkost_bloku_od;
        while(r % 4 != 0)
            r = rand() % (velkost_bloku_do - velkost_bloku_od + 1) + velkost_bloku_od;
        y++;
    }

    memory_scheme(start, 10);

    for(i = 0; i < y; i++)
        memory_free((void *)k[i]);

    return uspesne_alokovana;
}

int main(){
    char memory[600000];
    int pociatocna_pamat, velkost_bloku, uspesne_alokovana_1, uspesne_alokovana_2, velkost_bloku_od, velkost_bloku_do;

    
    //****************TESTOVACÍ SCENÁR ČÍSLO 1.2****************//
    /*
    pociatocna_pamat = 201;
    velkost_bloku = 24;
    memory_init(memory, pociatocna_pamat);
    memory_scheme(start, 10);
    uspesne_alokovana_1 = test_static(velkost_bloku);
    uspesne_alokovana_2 = test_static(velkost_bloku);
    memory_scheme(start, 5);


    printf("Pociatocna pamat: %d\nVelkost alokovanych blokov: %d\nUspesne alokovana: %d\nEfektivnost: %.2lf%%\n",
        pociatocna_pamat, velkost_bloku, uspesne_alokovana_1, (double)uspesne_alokovana_1 / (double)pociatocna_pamat * 100);
    if(uspesne_alokovana_1 == uspesne_alokovana_2)
        printf("Efektivnosti sa rovnaju");
    else
        printf("Efektivnosti sa nerovnaju");
    */
    //********************************************************//


    //****************TESTOVACÍ SCENÁR ČÍSLO 2.2****************//
    
    pociatocna_pamat = 201;
    velkost_bloku_od = 8;
    velkost_bloku_do = 24;
    memory_init(memory+300, pociatocna_pamat);
    printf("\n%d\n", *(char *)start);

    uspesne_alokovana_1 = test_random(velkost_bloku_od, velkost_bloku_do);
    uspesne_alokovana_2 = test_random(velkost_bloku_od, velkost_bloku_do);
    

    printf("Pociatocna pamat: %d\nVelkost alokovanych blokov: %d - %d\nUspesne alokovana prva: %d\nUspesne alokovana druha: %d\nEfektivnost prva: %.2lf%%\nEfektivnost druha: %.2lf%%\n",
        pociatocna_pamat, velkost_bloku_od, velkost_bloku_do, uspesne_alokovana_1, uspesne_alokovana_2, (double)uspesne_alokovana_1 / (double)pociatocna_pamat * 100, (double)uspesne_alokovana_2 / (double)pociatocna_pamat * 100);
    
    //********************************************************//


    //****************TESTOVACÍ SCENÁR ČÍSLO 3.1****************//
    /*
    pociatocna_pamat = 50000;
    velkost_bloku_od = 500;
    velkost_bloku_do = 5000;
    memory_init(memory, pociatocna_pamat);

    uspesne_alokovana_1 = test_random(velkost_bloku_od, velkost_bloku_do);
    uspesne_alokovana_2 = test_random(velkost_bloku_od, velkost_bloku_do);


    printf("Pociatocna pamat: %d\nVelkost alokovanych blokov: %d - %d\nUspesne alokovana prva: %d\nUspesne alokovana druha: %d\nEfektivnost prva: %.2lf%%\nEfektivnost druha: %.2lf%%\n",
        pociatocna_pamat, velkost_bloku_od, velkost_bloku_do, uspesne_alokovana_1, uspesne_alokovana_2, (double)uspesne_alokovana_1 / (double)pociatocna_pamat * 100, (double)uspesne_alokovana_2 / (double)pociatocna_pamat * 100);
    */
    //********************************************************//


    //****************TESTOVACÍ SCENÁR ČÍSLO 4.1****************//
    /*
    pociatocna_pamat = 500000;
    velkost_bloku_od = 8;
    velkost_bloku_do = 50000;
    memory_init(memory, pociatocna_pamat);

    uspesne_alokovana_1 = test_random(velkost_bloku_od, velkost_bloku_do);
    uspesne_alokovana_2 = test_random(velkost_bloku_od, velkost_bloku_do);


    printf("Pociatocna pamat: %d\nVelkost alokovanych blokov: %d - %d\nUspesne alokovana prva: %d\nUspesne alokovana druha: %d\nEfektivnost prva: %.2lf%%\nEfektivnost druha: %.2lf%%\n",
        pociatocna_pamat, velkost_bloku_od, velkost_bloku_do, uspesne_alokovana_1, uspesne_alokovana_2, (double)uspesne_alokovana_1 / (double)pociatocna_pamat * 100, (double)uspesne_alokovana_2 / (double)pociatocna_pamat * 100);
    */
    //********************************************************//



    return 0;
}