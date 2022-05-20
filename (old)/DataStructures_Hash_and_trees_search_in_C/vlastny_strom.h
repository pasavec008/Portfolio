//vlastna implementacia splay stromov

typedef struct node_strom{
    int data;
    struct node_strom*  parent;
    struct node_strom*  left;
    struct node_strom*  right;
} NODE_STROM;


NODE_STROM *rotation_right(NODE_STROM *spinner){
    NODE_STROM *hlp = spinner->left;

    spinner->left = hlp->right;
    if(hlp->right != NULL)
        hlp->right->parent = spinner;
    hlp->right = spinner;

    if(spinner->parent != NULL){            
        if(spinner->parent->right == spinner)
            spinner->parent->right = hlp;
        if(spinner->parent->left == spinner)
            spinner->parent->left = hlp;
    }
    hlp->parent = spinner->parent;
    spinner->parent = hlp;
    return hlp;
}

NODE_STROM *rotation_left(NODE_STROM *spinner){
    NODE_STROM *hlp = spinner->right;
    
    spinner->right = hlp->left;
    if(hlp->left != NULL)
        hlp->left->parent = spinner;
    hlp->left = spinner;

    if(spinner->parent != NULL){            
        if(spinner->parent->right == spinner)
            spinner->parent->right = hlp;
        if(spinner->parent->left == spinner)
            spinner->parent->left = hlp;
    }
    hlp->parent = spinner->parent;
    spinner->parent = hlp;
    return hlp;
}

NODE_STROM *vlastny_splay(NODE_STROM *future_root){
    if(future_root->parent == NULL)
        return future_root;

    if(future_root->parent->parent == NULL){       //zig, otec je koren
        if(future_root->parent->left == future_root){      //future je lave dieta
            rotation_right(future_root->parent);
            return vlastny_splay(future_root);
        }
        if(future_root->parent->right == future_root){     //future je prave dieta
            rotation_left(future_root->parent);
            return vlastny_splay(future_root);
        }
    }    

    else if(future_root->parent->left == future_root && future_root->parent->parent->left == future_root->parent){       //zig zig, future aj otec obe lave deti
        rotation_right(future_root->parent);
        rotation_right(future_root->parent);
        return vlastny_splay(future_root);
    }
    else if(future_root->parent->right == future_root && future_root->parent->parent->right == future_root->parent){       //zig zig, future aj otec obe prave deti
        rotation_left(future_root->parent);
        rotation_left(future_root->parent);
        return vlastny_splay(future_root);
    }

    else if(future_root->parent->right == future_root && future_root->parent->parent->left == future_root->parent){       //zig zag, future je prave, otec je lave dieta
        rotation_left(future_root->parent);
        rotation_right(future_root->parent);
        return vlastny_splay(future_root);
    }
    else if(future_root->parent->left == future_root && future_root->parent->parent->right == future_root->parent){       //zig zag, future je prave, otec je lave dieta
        rotation_right(future_root->parent);
        rotation_left(future_root->parent);
        return vlastny_splay(future_root);
    }


    printf("Niekde nastala chyba\n");
    return NULL;
}

NODE_STROM *vlastny_search(int data, NODE_STROM *root){
    if(root == NULL){
        //printf("Vlastny_strom nenasiel prvok\n");
        return NULL;
    }
    if(root->data < data)
        return vlastny_search(data, root->right);
    else if(root->data > data)
        return vlastny_search(data, root->left);
    else if(root->data == data){
        //printf("Vlastny strom nasiel prvok\n");
        return root;
    }
}

NODE_STROM *vlastny_insert(int new_data, NODE_STROM *root){
    NODE_STROM *new_node_strom = (NODE_STROM *)malloc(sizeof(NODE_STROM)), *act = root;
    new_node_strom->left = NULL;
    new_node_strom->right = NULL;
    new_node_strom->data = new_data;

    //pripad pre prvy prvok
    if(root == NULL){
        new_node_strom->parent = NULL;
        return new_node_strom;
    }

    while(1){
        if(new_data < act->data){
            if(act->left == NULL){
                act->left = new_node_strom;
                new_node_strom->parent = act;
                return vlastny_splay(new_node_strom);
            }
            act = act->left;
        }
        else if(new_data > act->data){
            if(act->right == NULL){
                act->right = new_node_strom;
                new_node_strom->parent = act;
                return vlastny_splay(new_node_strom);
            }
            act = act->right;
        }
        else{
            printf("Duplikaty nie su povolene\n");
            return root;
        }
    }

    return NULL;
}

void hodnoty_stromu(NODE_STROM *root){          //preorder vypis
    if(root == NULL)
        return;
    printf("%d ", root->data);
    hodnoty_stromu(root->left);
    hodnoty_stromu(root->right);
}
