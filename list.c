#include"list.h"

void initList(List*pl){
    pl->head = (Params*)malloc(INITSIZE*sizeof(Params));
    if(pl->head==NULL){
        perror("init error");
        exit(-1);
    }
    pl->tail = pl->head;
    pl->capacity=INITSIZE;
}

Params*get(List*pl){
    return pl->head;
}

int isFull(List*pl){
    if(size(pl)==pl->capacity){
        return 1;
    }
    return 0;
}

int extendCapa(List*pl){
    Params*new_head = (Params*)malloc(sizeof(Params)*(pl->capacity)*2);
    if(new_head==NULL){
        perror("malloc error");
        return -1;
    }
    /* copy original data to new place*/
    for(int i = 0 ;i < (pl->capacity) ;i++){
        new_head[i]=pl->head[i];
    }
    free(pl->head);
    pl->head=new_head;
    pl->tail=pl->head+(pl->capacity);
    pl->capacity=(pl->capacity)*2;
    return 0;
}

size_t size(List*pl){
    return pl->tail-pl->head;
}

int insert(List*l,unsigned int index,Params p)
{   
    if(isFull(l)){             /*1表示满*/
        if(extendCapa(l)!=0)   /*0拓展成功*/
            exit(-1);          
    }
    l->head[index]=p;
    l->tail++;    
}