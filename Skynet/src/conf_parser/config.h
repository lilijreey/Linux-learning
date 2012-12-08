/**
 *============================================================
 *  @file      config.h
 *  @brief     用于解析配置文件。配置文件的格式为：\n
 *    ============================================\n
 *             \#key        value       |  #号打头的行被视为注释 \n
 *             log_dir      ./log \n
 *             log_size     32000000 \n
 *    ============================================
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *============================================================
 */

#ifndef LIBant_CONFIG_H_
#define LIBant_CONFIG_H_

/**
  * @brief 解析配置文件file_name。成功返回后就可以调用config_get_inval或者config_get_strval
  *         来获取配置文件的内容了???  * @param file_name 配置文件路径名???  * @see config_get_intval, config_get_strval
  * @return 成功则返???，失败则返回-1???  */
int   config_init(const char* file_name);
/**
  * @brief  清除解析配置文件时分配的内存。调用该函数后则再也不能使用config_get_inval或者config_get_strval
  *         来获取配置文件的内容了，同时config_get_strval之前返回的所有指针都不能继续使用???  * @see config_get_intval, config_get_strval
  * @return void
  */
void  config_exit();

// return -1 on error
int   mmap_config_file(const char* file_name, char** buf);
// return number of words splitted
int   str_split(const char* ifs, char* line, char* field[], int n);

/**
  * @brief 获取配置项key的整型值。如果找不到key，则返回值为def。如果key对应的值是非数字字符，则行为未定义???  *        故请小心校验配置文件和代码???  * @param key 配置项key???  * @param def 如果配置文件中没有配置项key，则返回def???  * @see config_get_strval
  * @return 配置项key对应的整型值，或者def???  */
int   config_get_intval(const char* key, int def);
/**
  * @brief 获取配置项key的字符创型的值???  * @param key 配置项key???  * @see config_get_intval
  * @return 配置项key对应的字符串型的型值。这个指针在调用config_exit之前一直都是有效可用的???  */
char* config_get_strval(const char* key);

/*
inline const char * config_get_strval_with_def(const char* key,const char* def ){
	const char *  v= config_get_strval(key)  ;
	if ( v!=NULL){
		return  v;
	}else{
		return def;
	}
}
*/

#endif // LIBant_CONFIG_H_
  
