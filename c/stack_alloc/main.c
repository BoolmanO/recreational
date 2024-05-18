#include <stdio.h> // used only for printf (in print_heap), so, you can easily avoid including this one
#include <stdbool.h> // if you need, you can avoid this including too, just define enum or use chars
#define HEAP_SIZE 2048
#define HEAP_SIZE_BYTES 1
#define HEAP_MAX_ALLOC_ATTEMPS 64
#define HEAP_MALLOC_DEFAULT 69 // if the value is 0, it's free in this allocator
// so, my model of "heap" looks like this [HEAP_SIZE_BYTES, value]
// one byte = max allocate 255 bytes at once

unsigned char heap[HEAP_SIZE] = {0};

size_t _pow(size_t base, size_t exp) {
  size_t result = 1;
  for (int i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
}

void print_heap() {
  bool printed_anything = false;
  for (int i = 0; i < HEAP_SIZE; i++ ) {
      if (heap[i] == 0) {
        continue;
      } 
      printed_anything = true;
      printf("%i: %i\n", i, heap[i]);
    }
  if (!printed_anything) {
    printf("Heap is empty.\n");
  }
}

size_t max_allocating_at_once() {
  size_t size = 0;
  for (int i = 0; i < HEAP_SIZE_BYTES; i++) {
      size += _pow(256, i);
  }
}

void normalize_index(size_t* index) {
  if (*index+HEAP_SIZE_BYTES > HEAP_SIZE) {
    *index = 0;
  }
}


bool stack_find(size_t cell_size, size_t* index) {
  normalize_index(index);
  size_t cached_index = *index;
  for (int i = 0; i < HEAP_MAX_ALLOC_ATTEMPS; i++) {
    bool index_occupied = (heap[cached_index] != 0);
    bool last_occupied = (heap[cached_index+cell_size] != 0);
    size_t push = 0;
    
    if (index_occupied) {
      push++;
    }
    if (last_occupied) {
      push = cell_size+1;
    }
    if (push != 0) {
      cached_index += push;
      continue;
    } 
    bool failed = false;
    for (int j = 1; j < cell_size; j*HEAP_SIZE_BYTES) {
      if (heap[cached_index+j] != 0) {
        failed = true;
        break;
      }
    if (failed) {
      cached_index += cell_size+1;
      continue;
    }
    *index = cached_index;
    return true;
    }
  }
  return false;
}

void* stack_alloc(size_t size) {
  // Maximum of allocated at once memory defined by
  // HEAP_SIZE_BYTES
  // keep in mind that you MUST fill the bytes after the allocating,
  // or its being just 0 and marked (looks like) as unallocated
  

  // index just current position
  static size_t index = 0;
  
  if (index+HEAP_SIZE_BYTES > HEAP_SIZE) {
    index = 0;
  }
  normalize_index(&index);
  
  void* ptr = NULL;
  size_t cell_size = size + HEAP_SIZE_BYTES;
  if (cell_size > HEAP_SIZE) {
    // Haven't enough memory, return NULL
    return NULL;
  }


  bool free_space_finded = stack_find(cell_size, &index);
  if (free_space_finded==false) {
    return NULL;
  }
  size_t free_space_index = index;
  index += 1;

  // index here - start of a entire section of a value
  unsigned char *size_as_bytes = (unsigned char *)&size;
  for (int i = 0; i < HEAP_SIZE_BYTES; i++) {
    heap[free_space_index+i] = size_as_bytes[i];
  }
  ptr = &heap[free_space_index+HEAP_SIZE_BYTES];
  return ptr;
}

void* stack_malloc(size_t size) {
  unsigned char* t = stack_alloc(size);
  if (t!=NULL) {
    for (int i=0; i < size; i++) {
      t[i] = HEAP_MALLOC_DEFAULT;
    }
  }
  return t;
}

void stack_free(void* ptr) {
  // here's a index of a pointer
  size_t index = (unsigned char*)ptr-heap;
  // and because of that, this is -, not +.
  size_t index_of_size_bytes = index-HEAP_SIZE_BYTES;

  // calculate the size of cell through iterate the bytes
  size_t size = 0;
  for (int i = 0; i < HEAP_SIZE_BYTES; i++) {
    size += heap[index_of_size_bytes+i] * _pow(256, i);
  }
  heap[index] = 0;
  // also free bytes with size of a memory cell
  for (int i=1; i < HEAP_SIZE_BYTES+1; i++) {
    heap[index-i] = 0;
  }

  unsigned char* byte_ptr = (unsigned char*)ptr;
  for (int i = 0; i < size; i++) {
    *byte_ptr = 0;
    byte_ptr++;
  }
}

int main() {
  int* x = stack_malloc(25*sizeof(int));
  printf("%i\n", heap[0]);
  x[0] = 942069;
  print_heap();
  printf("----\n");
  
  int* m = stack_alloc(sizeof(int));
  *m = 214748369;
   
  stack_free(x);
  print_heap();
  printf("----\n");
  printf("\n");
  print_heap();
  return 0;
}
