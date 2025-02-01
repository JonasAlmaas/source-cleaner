#include "cleaner.h"
#include <ctype.h>
#include <string.h>

enum line_ending {
	LE_UNKNOWN=0,
	LE_LF,
	LE_CRLF,
};

static void trim_trailing_whitespace(char *buf, int *p_i, long *p_fsz, enum line_ending le)
{
	int i = *p_i;
	long fsz = *p_fsz;

	while (i>0
			&& buf[i-1]!='\n'
			&& (le==LE_LF || buf[i-1]!='\r')
			&& isspace((int)buf[i-1])) {
		memcpy(buf+i-1, buf+i, fsz-i);
		--fsz;
		--i;
	}

	*p_i = i;
	*p_fsz = fsz;
}

/* TODO: Check if fsz becomes bigger than MAX_FILE_SIZE */
extern int src_clean(char *buf, long fsz)
{
	int i;
	enum line_ending le = LE_UNKNOWN; /* TODO: Allow use to allow desired line ending */

	/* Check what line ending the file uses */
	i=0;
	while (i<fsz && buf[i]!='\n') {
		++i;
	}
	if (i<fsz && buf[i]=='\n') {
		le = buf[i-1] == '\r' ? LE_CRLF : LE_LF;
	}

	/* Add new line to the end of the file if it is missing */
	if (buf[fsz-1] != '\n') {
		if (le != LE_UNKNOWN) {
			if (le == LE_CRLF){
				buf[fsz++] = '\r';
			}
			buf[fsz++] = '\n';
		}
	}

	i=fsz-1;
	while (i>0) {
		while (i>0 && buf[i]!='\n') {
			--i;
		}
		if (buf[i]=='\n') {
#if 0
			/* Correct line ending */
			if (le != LE_UNKNOWN) {
				if (le==LE_CRLF) {
					/*--i;*/
					if (i==0 || buf[i-1]!='\r') {
						/* TODO: Add '\r' */
					}
				} else if (le==LE_LF && i>0 && buf[i-1]=='\r') {
					/* TODO: Remove '\r' */
				}
			}
#endif
			trim_trailing_whitespace(buf, &i, &fsz, le);

			/* Trim multiple new lines */
			if (le==LE_LF) {
				while (i>0
						&& buf[i-1]=='\n'
						&& (i==fsz-1 || i<3 || buf[i-2]=='\n')) {
					memcpy(buf+i-1, buf+i, fsz-i);
					--fsz;
					--i;
				}
			} else if (LE_CRLF) {
				while (i>2
						&& buf[i-3]=='\n'
						&& (i==fsz-1 || i<6 || buf[i-5]=='\n')) {
					memcpy(buf+i-2, buf+i, fsz-i);
					fsz-=2;
					i-=2;
				}
			}

			trim_trailing_whitespace(buf, &i, &fsz, le);

			if (buf[i]=='\n') {
				--i;
			}
		}
	}

	/* Remove leading new line on the first line */
	if (le == LE_LF) {
		if (fsz>0 && buf[0]=='\n') {
			memcpy(buf+i-1, buf+i, fsz-i);
			--fsz;
		}
	} else if (le == LE_CRLF) {
		if (fsz>1 && buf[1]=='\n') {
			memcpy(buf+i-2, buf+i, fsz-i);
			fsz-=2;
		}
	}

	buf[fsz] = 0;

	return 0;
}
