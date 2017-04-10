int main(int argc, char *argv[]) {
     (-1 == execl("/usr/bin/cat",
                     "top ", //argv[0]
                     "/dev/zero", //argv[1]
                     "show.c",
                     NULL
                    )) 
     (-1 == execlp("cat",
                     "top ", //argv[0]
                     "/dev/zero", //argv[1]
                     "show.c",
                     NULL
                    )) {
