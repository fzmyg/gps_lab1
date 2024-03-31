#pragma once
#include"paras.h"
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#define INITSIZE 100
typedef struct List{
    Params*head;
    Params*tail;
    size_t capacity;
}List;

extern Params*get(List*);

extern int isFull(List*);

extern int extendCapa(List*);

extern size_t size(List*);

extern void initList(List*);

extern int insert(List*,unsigned int,Params);