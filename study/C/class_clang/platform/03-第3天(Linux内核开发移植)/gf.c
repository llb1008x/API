#include <common.h>
#include <command.h>
//#define CONFIG_FUN 1

#ifdef CONFIG_FUN
inline fun(void)
{
	.......
}
#else
inline fun(void)
{}
#endif

int	main(struct cmd_tbl_s *cmd, int flag, int argc, char *argv[])
{
	fun();
	.....
	fun();
	.....
	fun();
	return 0;
}
*************************************************************************************
#include <common.h>
#include <command.h>
#define CONFIG_FUN 1
int	main(struct cmd_tbl_s *cmd, int flag, int argc, char *argv[])
{
#ifdef CONFIG_FUN
	fun();
#endif
	.....
#ifdef CONFIG_FUN
	fun();
#endif
	.....
#ifdef CONFIG_FUN
	fun();
#endif
	return 0;
}


# mw 0xe0200c00 0x1111   将GPH0_0到0_4配置成输出状态
# mw 0xe0200c04 0xf			将GPH0_0到0_4配置成输出高电平状态
# mw 0xe0200c04 0xb			将GPH0_0到0_4四个引脚中的三个配置成高电平配置成输出高电平状态