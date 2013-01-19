all: create_thread

CC := gcc
LIB := -D_REENTRANT -lpthread
OPTION := -Wall -std=gnu99

# -O 优化
O:=-O$(o) #immediately assigment

define compile-and-run
	$(CC) $(OPTION) $(O)  $< $(LIB)  -o $@
endef

create_thread: create_thread.c
	$(compile-and-run)

#for git command
m ?= up ci
COMMIT_MSG=$(m)
RC := git
commit:
	$(RC) commit -a -m "$(COMMIT_MSG)"

push:
	$(RC) push origin master

st:
	$(RC) status

.PHONY: commit push all st

