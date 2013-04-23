/*
 * =====================================================================================
 *
 *       Filename:  show_dir_tree.c
 *
 *    Description:  show a directories tree. the dir is passed by the pararme
 *
 *        Version:  1.0 基础框架
 *					1.01 给stat（）增加了错误检测 03/12/2011
 *					1.02 //没开始，完善出错效果
 *        Created:  03/11/2011 12:31:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 *version:1.01 show style
	//read dirs

	 fesff\
			|__fesf
			|__bs
			|__bbeiwi\
					 |__fjeisof
	   aae\
	   fegbjj\
			 |__fjeisfj
version:1.02 shwo styel

      fesff\
	  |     |__fesf
	  |     |__bs
	  |     |__bbeiwi\
	  |	    |    	 |__fjeisof
	  |__aae\
	  |__fegbjj\
	  |      |__fjeisfj

 * =====================================================================================
 */
 

/* 在用stat（）是要注意他的参数是path，而dirent->d_name 只是名字，
 * 要注意用chdir切换到被打开的目录 */
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>


#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>

#define		MAX_SEARCH_DEPTH	10		/*  */

//递归的调用显示dirtree
void search_dir_tree(const char *dir_name, int current_depth, int start_index) //the start_index is first word before speac 
{
	struct dirent *dir_item = NULL ;
	//open dir
	DIR *dir_stream = NULL ;
	if(NULL == (dir_stream = opendir(dir_name))) {
		perror("opendir occured failed\n") ;
		exit(1) ;
	}
	//into current dir, the stat() request a path, but the dir_item->d_name is a name
	//so into current dir
	chdir(dir_name) ;
		
	//read the dir items
	while(dir_item = readdir(dir_stream))
	{
		//get name test if is a directory
		struct stat item_stat ;
		if(0 != strcmp(dir_item->d_name, ".") &&
			0 != strcmp(dir_item->d_name, ".."))
		{
			 if(0 == stat(dir_item->d_name, &item_stat)) {//the stat may be failed
				//printf("name%s\n", dir_item->d_name) ;
				if(!S_ISDIR(item_stat.st_mode)) {
					printf("%*s", start_index,"") ;
					printf("|__%s\n", dir_item->d_name) ; 
				} else {
					printf("%*s", start_index, "") ;
					printf("|__%s\\\n", dir_item->d_name) ; 
					//printf("%d\n", current_depth) ;
					if(current_depth < MAX_SEARCH_DEPTH -1) {
						search_dir_tree(dir_item->d_name,
									current_depth+1, 
									start_index + strlen(dir_item->d_name) + 3) ;	
					}//else {printf("|__...\n") ;}		
				}
			 }
		}	
	}
	
	//close dir
	if(0 != closedir(dir_stream)) {
				printf("closedir failure\n") ;
				exit(1) ;
	}
	//go back father dir
	chdir("..") ;
}	
	

	

int main(int argc, char *argv[])
{
	char *pdir = NULL ;
	char *default_dir = "./" ; //default dir is currte dir
	int current_depth = 0;
	int start_index = 0 ; 

	if(argc > 2) {
		printf("pararment too mach\n") ;
		exit(1) ;
	} else {
		(1 == argc) ? (pdir=default_dir) : (pdir=argv[1]) ;	
	}

	//go to dir
	chdir(pdir) ;
	//test if a directory
	struct stat dir_stat ;
	if(-1 != stat(pdir, &dir_stat)) { //when the pdir is no such name the stat() failed
		if(S_ISDIR(dir_stat.st_mode)) { // show the root directory
			printf("%s\\\n", pdir) ;
			search_dir_tree(pdir, current_depth+1, start_index + strlen(pdir) + 1) ;
		} else { 
			printf("%s, No such directory\n", pdir) ;
		}
	}
	else
	{
		printf("%s, No such directory\n", pdir) ;
	}

	return 0 ;
}

