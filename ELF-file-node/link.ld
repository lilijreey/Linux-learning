xx_buf = 0x1000010;

SECTIONS
{
	.text : 
	{
		*(.text)
		/*PROVIDE(show = 0x00000010 ); /* 定义一个预备符号,位置为text端后+10的地方*/
        }

	.my_text : { *(.text)}
}
