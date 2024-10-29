#include "MVote.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


size_t my_free(void *ptr)
{
	if (ptr==NULL){
		printf("Void free\n");
		return 0;
	}
	size_t i=malloc_usable_size(ptr);
	free(ptr);
	return i;
}

