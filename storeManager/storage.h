#ifndef STORAGE_H
#define STORAGE_H

int save_to_file(const char *filename, Product *list, int count);
int load_from_file(const char *filename, Product **list, int *count, int *capacity);

#endif
