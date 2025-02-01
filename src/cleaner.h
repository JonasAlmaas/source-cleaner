#ifndef CLEANER_H_INCLUDED
#define CLEANER_H_INCLUDED

/* TODO: Is this enough? Should I use a heap allocation instead? */
enum {MAX_FILE_SIZE=65536};

extern int src_clean(char *buf, long fsz);

#endif /* CLEANER_H_INCLUDED */
