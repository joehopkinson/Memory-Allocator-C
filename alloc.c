#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct Metadata{
  size_t data_size;
  int is_free;
  int is_head;
  struct Metadata *next_block;
  struct Metadata *prev_block;
} Metadata;

struct Metadata *head = NULL;
struct Metadata *tail = NULL;

struct Metadata *free_head = NULL;
struct Metadata *free_tail = NULL;

int glob_counter = 0;
int free_counter = 0;

size_t alligned_size(size_t size){
  if(size % 8 != 0) size = size + (8 - size % 8);
  return size;
}

struct Metadata *create_node(struct Metadata *node, size_t size){
  if(!head){
    node->is_head = 1;
    node->next_block = NULL;
    node->prev_block = NULL;
  }
  else{
    tail->next_block = node;
    node->prev_block = tail;
  }

  node->is_free = 0;
  node->data_size = alligned_size(size) + sizeof(Metadata);

  tail = node;

  glob_counter++;

  return tail;
}

struct Metadata *check_for_space(size_t size){
  if(free_counter <= 0 || free_head == NULL){
    return NULL;
  }

  struct Metadata *temp = free_head;
  while(temp->next_block != NULL){
    if(temp->data_size >= alligned_size(size) + sizeof(Metadata) && temp->is_free){
      return temp;
    }
    temp = temp->next_block;
  }
  return NULL;
}

void add_to_free(struct Metadata *node){
  if(!free_head){
    node->next_block = NULL;
    node->prev_block = NULL;
    free_head = node;
  }
  else{
    free_tail->next_block = node;
    node->prev_block = free_tail;
  }

  free_tail = node;
  free_counter++;
}

void *mymalloc(size_t size){
  struct Metadata *node;
  if(size <= 0){
    return NULL;
  }
  node = check_for_space(size);
  if(!node){
    node = sbrk(alligned_size(alligned_size(size) + sizeof(Metadata)));
    node = create_node(node, size);

    if(node->is_head){
      head = node;
    }
  }
  else{                                                                         // There exists a free spot
    node->is_free = 0;
  }
  return node + 1;
}

void *mycalloc(size_t nmemb, size_t size){
  int my_size = nmemb * size;
  void *ptr = mymalloc(my_size);
  memset(ptr, 0, my_size);
  return ptr;
}

void myfree(void *ptr){
  if(ptr != NULL){
    struct Metadata *free_node = (struct Metadata *)ptr - 1;
    if(free_node + sizeof(Metadata) + free_node->data_size == sbrk(0)){
      sbrk(0 - (free_node->data_size + sizeof(Metadata)));
    }
    free_node->is_free = 1;
    add_to_free(free_node);
  }
}

void *myrealloc(void *ptr, size_t size){
  if(size == 0 || ptr == NULL){
    return mymalloc(size);
  }

  struct Metadata *reallocate_node = (struct Metadata *)ptr - 1;
  reallocate_node->next_block = NULL;
  reallocate_node->prev_block = tail;

  if(alligned_size(size) + sizeof(Metadata) <= reallocate_node->data_size){
    return ptr;
  }

  void *newBlock = mymalloc(size);

  if(newBlock){
    memcpy(newBlock, ptr, reallocate_node->data_size);
    myfree(ptr);
  }

  return newBlock;
}



/*
 * Enable the code below to enable system allocator support for your allocator.
 * Doing so will make debugging much harder (e.g., using printf may result in
 * infinite loops).
 */
// #if 0
void *malloc(size_t size) { return mymalloc(size); }
void *calloc(size_t nmemb, size_t size) { return mycalloc(nmemb, size); }
void *realloc(void *ptr, size_t size) { return myrealloc(ptr, size); }
void free(void *ptr) { myfree(ptr); }
// #endif
