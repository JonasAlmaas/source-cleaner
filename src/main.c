#include "cleaner.h"
#include <stdio.h>

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
	status = clean_str(buf, fsz, LE_UNKNOWN);

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
