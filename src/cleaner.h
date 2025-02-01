#ifndef CLEANER_H_INCLUDED
#define CLEANER_H_INCLUDED

/* TODO: Is this enough? Should I use a heap allocation instead? */
enum {MAX_FILE_SIZE=65536};

enum line_ending {
	LE_UNKNOWN=0,
	LE_LF,
	LE_CRLF,
};

extern int clean_str(char *buf, long fsz, enum line_ending desired_le);

#endif /* CLEANER_H_INCLUDED */
