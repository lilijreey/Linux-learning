#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "kernel_ds/hash_algo.h"
#include "kernel_ds/list.h"

#include "config.h"


#define INIKEY_HASHBITS	8

static list_head_t ini_key_head[1 << INIKEY_HASHBITS];

static int has_init;

typedef struct config_pair {
	struct list_head list;
	char* val;
	char  key[];
} config_pair_t;

//------------------------------------------------------------
// Static Functions
//
static inline struct list_head*
config_key_hash(const char* key) 
{	
	return &ini_key_head[r5hash(key) & ((1 << INIKEY_HASHBITS) - 1)];
}

// return -1 on error, 0 on success
static int config_put_value(const char* key, const char* val) 
{
	int len = strlen(key) + 1;
	struct config_pair* mc = malloc(sizeof(struct config_pair) + len);
	if (!mc) {
		return -1;
	}
	memcpy(mc->key, key, len);
	mc->val = strdup(val);
	if (!mc->val) {
		free(mc);
		return -1;
	}
	ant_list_add(&mc->list, config_key_hash(key));

	return 0;
}

// return -1 on error, 0 on success
static int parse_config(char* buffer)
{
	static const char myifs[256]
			= { [' '] = 1, ['\t'] = 1, ['\r'] = 1, ['\n'] = 1, ['='] = 1 };

	char*  field[2];
	char*  start = buffer;
	size_t len   = strlen(buffer);
	while (buffer + len > start) {
		char* end = strchr(start, '\n');
		if (end) {
			*end = '\0';
		}
		if ((*start != '#') && (str_split(myifs, start, field, 2) == 2)) {
			if (config_put_value(field[0], field[1]) == -1) {
				return -1;
			}
		}
		if (end) {
			start = end + 1;
		} else {
			break;
		}
	}
	return 0;
}
//------------------------------------------------------------

int config_init(const char* file_name)
{
	int ret_code = -1;

	if (!has_init) {
		int i;
		for (i = 0; i < (1 << INIKEY_HASHBITS); i++) {
			INIT_LIST_HEAD(&ini_key_head[i]);
		}
		has_init = 1;
	}

	char* buf;
	int   len = mmap_config_file(file_name, &buf);
	if (len > 0) {
		ret_code = parse_config(buf);
		munmap(buf, len);
	}

	return ret_code;
}

void config_exit()
{
	int i;
	for (i = 0; i < (1 << INIKEY_HASHBITS); ++i) {
		list_head_t* p = ini_key_head[i].next;
		while (p != &ini_key_head[i]) {
			config_pair_t* mc = list_entry(p, config_pair_t, list);
			p = p->next;
			free(mc->val);
			free(mc);
		}
	}
}

int mmap_config_file(const char* file_name, char** buf)
{
	int ret_code = -1;

	int fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		return -1;
	}

	int len = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);

	*buf = mmap(0, len + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (*buf != MAP_FAILED) {
		read(fd, *buf, len);
		(*buf)[len] = 0;
		ret_code = len + 1;
	}
	close(fd);

	return ret_code;
}

/*
 * NULL IFS: default blanks
 * first byte is NULL, IFS table
 * returns number of words splitted up to on success
 */
int str_split(const char* ifs, char* line, char* field[], int n)
{
	static const char default_ifs[256]
			= { [' '] = 1, ['\t'] = 1, ['\r'] = 1, ['\n'] = 1, ['='] = 1 };

	if (ifs == 0) {
		ifs = default_ifs;
	}

	int i = 0;
	while (1) {
		while (ifs[(unsigned char)*line]) {
			++line;
		}
		if (*line == '\0') {
			break;
		}
		field[i++] = line;

		// remove tailing spaces
		if (i >= n) {
			line += strlen(line) - 1;
			while (ifs[(unsigned char)*line]) {
				--line;
			}
			line[1] = '\0';
			break;
		}

		// add a tailing '\0'
		while (*line && !ifs[(unsigned char)*line]) {
			++line;
		}
		if (!*line) {
			break;
		}
		*line++ = '\0';
	}

	return i;
}

int config_get_intval(const char* key, int def)
{
	char* val = config_get_strval(key);
	if (val == 0) {
		return def;
	}
	return atoi(val);
}

char* config_get_strval(const char* key)
{
	struct config_pair* mc;

	list_head_t* hlist = config_key_hash(key);
	list_for_each_entry(mc, hlist, list) {
		if (!strcmp(key, mc->key)) {
			return mc->val;
		}
	}

	return 0;
}

