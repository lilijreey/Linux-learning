/* set start as enter pointer 有着可知，程序的header并不一定是首先被执行的地方  
 * 入口点可以是任何地址 */

ENTRY(start) 

SECTIONS {
	. = 1M; /* 在1M开始加载kernel, 这个是因为从0到1M之间的内存有其他用途，比如BIOS向量表，VGA等 */

	.boot : 
	{
		/* ensure that the mulitiboot header is at beginning */
		*(.multiboot_header)
        }

	.text :
	{ 
		*(.text)
        }
}
