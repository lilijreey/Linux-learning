all:

LIB_SRC= s1.c s2.c s3.c

LIB_REAL_NAME   = libmys.so.1.1.0
LIB_SONAME      = libmys.so.1
LIB_LINKER_NAME = libmys.so

OBJECTS= ${LIB_LINKER_NAME} ${LIB_SONAME} ${LIB_REAL_NAME}

RM= rm -rf

test:
	@echo objects: ${OBJECTS}

## compilet no soname shared-lib Note complite with read lib
nosoname:
	gcc -Wall -fPIC -shared -o ${LIB_REAL_NAME} ${LIB_SRC}
	gcc -Wall main.c ${LIB_REAL_NAME} 

## compilet with soname shared-lib
soname:
	gcc -Wall -fPIC -shared -Wl,-soname,${LIB_SONAME} -o ${LIB_REAL_NAME} ${LIB_SRC}
# link real-name but symbol is soname
	gcc -Wall main.c ${LIB_REAL_NAME} 

## compilet with specifiy search path, this not need set search path
withpath:
	gcc -Wall -fPIC -shared -Wl,-soname,${LIB_SONAME} -o ${LIB_REAL_NAME} ${LIB_SRC}
	gcc -Wall -Wl,-rpath,/home/god/codes/linux/shared-lib main.c ${LIB_REAL_NAME}


## compilet with $ORIGIN, set to search exec relecot path
## The $ORIGIN will speant a.out path
origin:
	gcc -Wall -fPIC -shared -Wl,-soname,${LIB_SONAME} -o ${LIB_REAL_NAME} ${LIB_SRC}
	gcc -Wall -Wl,-rpath,'$$ORIGIN' main.c ${LIB_REAL_NAME}

## use version-script control visibility
version:
	gcc -Wall -fPIC -shared -Wl,-soname,${LIB_SONAME},--version-script,export.map -o ${LIB_REAL_NAME} ${LIB_SRC}
	gcc -Wall -Wl,-rpath,/home/god/codes/linux/shared-lib main.c ${LIB_REAL_NAME}

# create soname symbol-ln that ld.so will find it
# create linker name. ln to the last version of soname-lib or real-lib.
# linker name allows compilte version-independent link commands.
# e.g gcc -Wall main.c libmy.so not cear about so version
ln:
	ln -s ${LIB_REAL_NAME} ${LIB_SONAME} 
	ln -s ${LIB_SONAME} ${LIB_LINKER_NAME}

clean:
	${RM} a.out *.o ${OBJECTS}
