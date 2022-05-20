typedef struct node_hash{
    int data;
    struct node_hash *next;
}NODE_HASH;

NODE_HASH *vlastna_hash_init(int size){
    NODE_HASH *hash_array = (NODE_HASH *)malloc(size * sizeof(NODE_HASH));
    for(int i = 0; i < size; i++){
        (&hash_array[i])->data = 0;
        (&hash_array[i])->next = NULL;
    }
    return hash_array;
}

int vlastna_hash(int key, int size){
    return key % size;
}

NODE_HASH *vlastna_hash_expand(NODE_HASH *old_table, int *old_size);      //je tu kvoli implicitnej deklaracii

NODE_HASH *vlastna_hash_insert(NODE_HASH *table, int key, int number_of_nodes, int *size_of_array){
    if((double)number_of_nodes / (double)*size_of_array >= 0.55)
        table = vlastna_hash_expand(table, size_of_array);

    int position = vlastna_hash(key, *size_of_array);

    //ak na danom indexe nic nie je, tak tam vlozim data
    if((&table[position])->data == 0){
        (&table[position])->data = key;
        (&table[position])->next = NULL;
        return table;
    }

    //inak vytvorim novy uzol, ktory pripojim na koniec spajaneho zoznamu daneho indexu
    NODE_HASH *new_node = (NODE_HASH *)malloc(sizeof(NODE_HASH)), *act = &table[position];
    new_node->data = key;
    new_node->next = NULL;
    while(act->next != NULL)
        act = act->next;
    act->next = new_node;

    return table;
}

NODE_HASH *vlastna_hash_expand(NODE_HASH *old_table, int *old_size){
    int new_size = *old_size * 2 + 1;
    if(new_size % 3 == 0)
        new_size +=2;
    NODE_HASH *act = old_table, *hlp, *new_table = vlastna_hash_init(new_size);
    for(int i = 0; i < *old_size; i++){
        if((&act[i])->data != 0){
            vlastna_hash_insert(new_table, (&act[i])->data, 0, &new_size);
            hlp = &act[i];
            while(hlp->next != NULL){
                vlastna_hash_insert(new_table, hlp->next->data, 0, &new_size);
                hlp = hlp->next;
            }
        }
    }
    *old_size *= 2;

    return new_table;
}

int vlastna_hash_search(NODE_HASH *table, int key, int size_of_array){
    int position = vlastna_hash(key, size_of_array);
    NODE_HASH *act = &table[position];
    while(act->data != key){
        if(act->next != NULL)
            act = act->next;
        else{
            //printf("Vlastna_hash nenasla prvok\n");
            return 0;
        }
    }
    //printf("Vlastna hash nasla prvok\n");
    return 1;
}

void mini_vypis(NODE_HASH *table, int size_of_array){
    NODE_HASH *act = table;
    for(int i = 0; i < size_of_array; i++)
        printf("%.2d ", (&act[i])->data);

    act = table;
    printf("\n");
    for(int i = 0; i < size_of_array; i++){
        if((&act[i])->next != NULL)
            printf("%.2d ", (&act[i])->next->data);
        else
            printf("00 ");
        }

    act = table;
    printf("\n");
    for(int i = 0; i < size_of_array; i++){
        if((&act[i])->next != NULL){
            if((&act[i])->next->next != NULL)
                printf("%.2d ", (&act[i])->next->next->data);
            else
                printf("00 ");
        }
        else
            printf("00 ");
    }
    printf("\n\n");
}