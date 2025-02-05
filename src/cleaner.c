#include "cleaner.h"
#include <ctype.h>
#include <string.h>

static void trim_trailing_whitespace(char *buf, int *p_i, long *p_fsz, enum line_ending le)
{
	int i = *p_i;
	long fsz = *p_fsz;

	while (i>0
			&& buf[i-1]!='\n'
			&& (le==LE_LF || buf[i-1]!='\r')
			&& isspace((int)buf[i-1])) {
		memmove(buf+i-1, buf+i, fsz-i);
		--fsz;
		--i;
	}

	*p_i = i;
	*p_fsz = fsz;
}

static void correct_line_ending(char *buf, int *p_i, long *p_fsz, enum line_ending le)
{
	int i = *p_i;
	long fsz = *p_fsz;

	if (le != LE_UNKNOWN) {
		if (le==LE_LF) {
			if (i>0 && buf[i-1]=='\r') {
				memmove(buf+i-1, buf+i, fsz-i);
				--fsz;
				--i;
			}
		} else if (le==LE_CRLF) {
			if (i==0 || buf[i-1]!='\r') {
				memmove(buf+i+1, buf+i, fsz-i);
				buf[i] = '\r';
				++fsz;
				++i;
			}
		}

		*p_i = i;
		*p_fsz = fsz;
	}
}

/* TODO: Check if fsz becomes bigger than MAX_FILE_SIZE at any point */
extern int clean_str(char *buf, long fsz, enum line_ending desired_le)
{
	int i;
	enum line_ending le = desired_le;

	/* Identify line ending */
	if (desired_le == LE_UNKNOWN) {
		i=0;
		while (i<fsz && buf[i]!='\n') {
			++i;
		}
		if (i<fsz && buf[i]=='\n') {
			le = (i>0 && buf[i-1]=='\r') ? LE_CRLF : LE_LF;
		}
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
			correct_line_ending(buf, &i, &fsz, le);
			trim_trailing_whitespace(buf, &i, &fsz, le);

			/* Trim multiple new lines */
			if (le==LE_LF) {
				while (i>0
						&& buf[i-1]=='\n'
						&& (i==fsz-1
							|| i<2
							|| buf[i-2]=='\n'
							|| (i>2 && buf[i-2]=='\r' && buf[i-3]=='\n'))) {
					memmove(buf+i-1, buf+i, fsz-i);
					--fsz;
					--i;
					correct_line_ending(buf, &i, &fsz, le);
				}
			} else if (LE_CRLF) {
				while (i>1
						&& buf[i-2]=='\n'
						&& (i==fsz-1
							|| i<4
							|| buf[i-3]=='\n'
							|| buf[i-4]=='\n')) {
					memmove(buf+i-2, buf+i, fsz-i);
					fsz-=2;
					i-=2;
					correct_line_ending(buf, &i, &fsz, le);
				}
			}

			trim_trailing_whitespace(buf, &i, &fsz, le);

			if (buf[i]=='\n') {
				--i;
			}
		}
	}

	/* Remove leading new line */
	if (le == LE_LF) {
		if (fsz>0 && buf[0]=='\n') {
			memmove(buf, buf+1, fsz-1);
			--fsz;
		}
	} else if (le == LE_CRLF) {
		if (fsz>1 && buf[1]=='\n') {
			memmove(buf, buf+2, fsz-2);
			fsz-=2;
		}
	}

	buf[fsz] = 0;

	return 0;
}
