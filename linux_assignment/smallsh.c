#include "smallsh.h"

static char inpbuf[MAXBUF];
static char tokbuf[2*MAXBUF];
static char *ptr = inpbuf;
static char *tok = tokbuf;

static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

int userin(char* p){
	int c, count;
	ptr = inpbuf;
	tok = tokbuf;

	printf("%s", p);
	count = 0;

	while(1) {
		if ((c = getchar()) == EOF)
			return EOF;
		if (count < MAXBUF)
			inpbuf[count++] = c;
		if (c == '\n' && count < MAXBUF) {
			inpbuf[count] = '\0';
			return count;
		}
		if (c == '\n' || count >= MAXBUF) {
			printf("smallsh: input line too long\n");
			count = 0;
			printf("%s", p);
		}
	}
}

int gettok(char** outptr) {
	int type;
	*outptr = tok;
	while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	*tok++ = *ptr;
	switch (*ptr++) {
		case '\n':
			type = EOL;
			break;
		case '&':
			type = AMPERSAND;
			break;
		case ';':
			type = SEMICOLON;
			break;
		default:
			type = ARG;
			while(inarg(*ptr))
				*tok++ = *ptr++;
	}
	*tok++ = '\0';
	return type;
}

int inarg(char c) {
	char *wrk;
	for (wrk = special; *wrk; wrk++){
		if (c == *wrk)
			return 0;
	}
	return 1;
}

void procline() {
	char *arg[MAXARG + 1];
	int toktype, type;
	int narg = 0;
	for (;;) {
		switch (toktype = gettok(&arg[narg])) {
			case ARG:
				if (narg < MAXARG)
					narg++;
				break;
			case EOL:
			case SEMICOLON:
			case AMPERSAND:
				if (toktype == AMPERSAND) type = BACKGROUND;
				else type = FOREGROUND;
				if (narg != 0) {
					arg[narg] = NULL;
					runcommand(arg, type);
				}
				if (toktype == EOL) return;
				narg = 0;
				break;
		}
	}
}

int runcommand(char **cline, int where) {
	pid_t pid;
	int status, fd;
	if (strcmp(*cline, "exit") == 0)
		exit(0);
	else if (strcmp(*cline, "cd") == 0) {
		if (cline[1] == 0)
			chdir(getenv("HOME"));
		else if (cline[2] != 0)
			perror("Two command error");
		else if (chdir(cline[1]) == -1)
			perror(cline[1]);
		return 0;
	}
	else {
		switch(pid = fork()) {
			case -1:
				perror("smallsh");
				return -1;
			case 0:
				redirect(cline);
				execvp(*cline, cline);
				perror(*cline);
				exit(1);
		}
	}
	if (where == BACKGROUND) {
		printf("[Process id] %d\n", pid);
		return 0;
	}
	if (waitpid(pid, &status, 0) == -1)
		return -1;
	else
		return status;
}

int redirect(char **cline) {
	int i, fd;
	for (i = 0; cline[i] != NULL; i++)
		if (!strcmp(cline[i], ">"))
			break;
	if (cline[i]) {
		if (!cline[i+1]) return -1;
		else {
			if ((fd = open(cline[i+1], O_RDWR | O_CREAT, 0644)) == -1) {
				perror(cline[i+1]);
				return -1;
			}
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		cline[i] = "NULL";
		cline[i+1] = "NULL";
		for (i = i; cline[i] != NULL; i++) {
			cline[i] = cline[i+2];
		}
		return 0;
	}
	else return -1;
}
