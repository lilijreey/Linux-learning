
extern xx_buf[] ;
/*extern int show ;*/

int show;
int main()
{
    int i=3;
    if (&i > &show)
        return 1;
    else 
        return 0;
}
