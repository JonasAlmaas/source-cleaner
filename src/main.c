#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* TODO: Is this enough? Should I use a heap allocation instead? */
enum {MAX_FILE_SIZE=65536};

enum line_ending {
	LE_UNKNOWN=0,
	LE_LF,
	LE_CRLF,
};

static int src_clean(char *buf, long fsz)
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
			/* Trim trailing whitespace */
			while (i>0
					&& buf[i-1]!='\n'
					&& (le==LE_LF || buf[i-1]!='\r')
					&& isspace((int)buf[i-1])) {
				memcpy(buf+i-1, buf+i, fsz-i);
				--fsz;
				--i;
			}

			/* Trim multiple new lines */
			if (le==LE_LF) {
				while (i>0 && buf[i-1]=='\n' && (i<3 || buf[i-2]=='\n')) {
					memcpy(buf+i-1, buf+i, fsz-i);
					--fsz;
					--i;
				}
			} else if (LE_CRLF) {
				while (i>4 && buf[i-3]=='\n' && buf[i-5]=='\n') {
					memcpy(buf+i-2, buf+i, fsz-i);
					fsz-=2;
					i-=2;
				}
			}

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

int main(int argc, char **argv)
{
	FILE *fp;
	char buf[MAX_FILE_SIZE];
	long fsz;
	int status;

	/* Options */
	int dry_run = 0;

	if (argc > 1) {
		fp = fopen(argv[1], "r+");
		if (!fp) {
			return 1;
		}
	} else {
		fp = stdin;
	}

	fseek(fp, 0l, SEEK_END);
	fsz = ftell(fp);
	fseek(fp, 0l, SEEK_SET);

	if (fsz>MAX_FILE_SIZE) {
		fclose(fp);
		return 2;
	}

	fread(buf, sizeof(char), fsz, fp);
	status = src_clean(buf, fsz);

	if (!dry_run) {
		if (argc > 1) {
			freopen(argv[1], "w", fp);
		} else {
			fp = stdout;
		}
		fprintf(fp, buf);
	}

	fclose(fp);

	return status;
}
