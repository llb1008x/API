/*
*	add by hbb
*/
#include <common.h>
#include <command.h>
#include <net.h>

void print_menu_info()
{
		printf("\r[b]  bootloader	  : write the u-boot.bin   image into board...\n\
		\r[k]  kernel	  : write the	 zImage    image into board...\n\
		\r[r]  rootfs	  : write the rootfs.yaffs image into board...\n\
		\r[i]  all image	  : write all image(u-boot.bin/zImage/rootfs.yaffs)into board...\n\
		\r[a]  app	  : write the	 app.bin   image into board...\n\
		\r[q]  if you press the 'q' key,will quit the menu list...\n");
}

void write_app()
{
	char cmdbuf[255] = "tftp 0x40000000 app.bin";
	run_command(cmdbuf,0);	
	printf("\n\rif you want to continue write image,please press 'n' key...\n\
			\rif you want quit menu,you should press 'q' key...\n");
}

void write_boot()
{
	char cmdbuf[255] = "tftp 0x40000000 u-boot.bin";
	char *argv_erase[5]  = { NULL, "erase",  "0x0", "0x100000", NULL, };
	char *argv_write[6]  = { NULL, "write",  "0x40000000", "0x0", "0x100000", NULL, };

	run_command(cmdbuf,0);		
	
	argv_erase[2] = "0x0";
	argv_erase[3] = "0x100000";
	do_nand(NULL, 0, 4, argv_erase);
	argv_write[3] = "0x0";
	argv_write[4] = "0x100000";
	do_nand(NULL, 0, 5, argv_write);

	printf("\n\rif you want to continue write image,please press 'n' key...\n\
			\rif you want quit menu,you should press 'q' key...\n");
}

void write_kernel()
{
	char cmdbuf[255] = "tftp 0x40000000 zImage";
	char *argv_erase[5]  = { NULL, "erase",  "0x0", "0x100000", NULL, };
	char *argv_write[6]  = { NULL, "write",  "0x40000000", "0x0", "0x100000", NULL, };

	run_command(cmdbuf,0);			

	argv_erase[2] = "0x600000";
	argv_erase[3] = "0x500000";
	do_nand(NULL, 0, 4, argv_erase);
	
	argv_write[3] = "0x600000";
	argv_write[4] = "0x500000";
	do_nand(NULL, 0, 5, argv_write);			

	printf("\n\rif you want to continue write image,please press 'n' key...\n\
			\rif you want quit menu,you should press 'q' key...\n");
}

int write_rootfs()
{
	char cmdbuf[255] = "tftp 0x40000000 rootfs.yaffs";
	char *argv_erase[5]  = { NULL, "erase",  "0x0", "0x100000", NULL, };
	char *argv_write[6]  = { NULL, "write",  "0x40000000", "0x0", "0x100000", NULL, };

	run_command(cmdbuf,0);			

	argv_erase[2] = "0xE00000";
	argv_erase[3] = "0x6E00000";
	do_nand(NULL, 0, 4, argv_erase);
			
	sprintf(cmdbuf,"0x%x",NetBootFileXferSize);
	printf("\n yaffs size:%s\n",cmdbuf);
	argv_write[1] = "write.yaffs";
	argv_write[3] = "0xE00000";
	argv_write[4] = cmdbuf;
	do_nand(NULL, 0, 5, argv_write);	
	printf("\n\rif you want to continue write image,please press 'n' key...\n\
			\rif you want quit menu,you should press 'q' key...\n");
}

void write_all_image()
{
	char cmdbuf[255] = "";
	char *argv_erase[5]  = { NULL, "erase",  "0x0", "0x100000", NULL, };
	char *argv_write[6]  = { NULL, "write",  "0x40000000", "0x0", "0x100000", NULL, };

	//update bootloader
	strcpy(cmdbuf,"tftp 0x40000000 ");
	strcat(cmdbuf,"u-boot.bin");
	run_command(cmdbuf,0);		
	
	argv_erase[2] = "0x0";
	argv_erase[3] = "0x100000";
	do_nand(NULL, 0, 4, argv_erase);
	argv_write[3] = "0x0";
	argv_write[4] = "0x100000";
	do_nand(NULL, 0, 5, argv_write);

	//update kernel
	strcpy(cmdbuf,"tftp 0x40000000 ");
	strcat(cmdbuf,"zImage");
	run_command(cmdbuf,0);			

	argv_erase[2] = "0x600000";
	argv_erase[3] = "0x500000";
	do_nand(NULL, 0, 4, argv_erase);
	
	argv_write[3] = "0x600000";
	argv_write[4] = "0x500000";
	do_nand(NULL, 0, 5, argv_write);

	//update filesystem
	strcpy(cmdbuf,"tftp 0x40000000 ");
	strcat(cmdbuf,"rootfs.yaffs");
	run_command(cmdbuf,0);			
	
	argv_erase[2] = "0xE00000";
	argv_erase[3] = "0x6E00000";
	do_nand(NULL, 0, 4, argv_erase);
	
	sprintf(cmdbuf,"0x%x",NetBootFileXferSize);
	argv_write[1] = "write.yaffs";
	argv_write[3] = "0xE00000";
	argv_write[4] = cmdbuf;
	do_nand(NULL, 0, 5, argv_write);
	printf("\n\rif you want to continue write image,please press 'n' key...\n\
			\rif you want quit menu,you should press 'q' key...\n");
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char get_char = 0;
	print_menu_info();

	do{
		get_char = getc();
		switch(get_char)
		{
			case 'a':
				write_app();
				continue;
			case 'b':
				write_boot();
				continue;
			case 'k':
				write_kernel();
				continue;
			case 'r':
				write_rootfs();
				continue;
			case 'i':
				write_all_image();
				continue;
			case 'n':
				print_menu_info();
				continue;
			case 'q':
				printf("\nquit menu succeed!\n\n");
				break;
			default:
				printf("no such command or not support!\n");
				continue;
		}
	}while(get_char!= 'q');

	return;
}


U_BOOT_CMD(
	menu, CFG_MAXARGS, 1,	do_menu,
	"menu    - display the command of boot/kernel/rootfs on the list\n",
	"\r- [b] write the u-boot.bin bootloader image into MEM by 'b' key...\n\
	\r- [k] write the zImage kernel image into MEM by 'k' key...\n\
	\r- [r] write the rootfs.yaffs rootfs image into MEM by 'r' key...\n\
	\r- [i] write all image(u-boot.bin/zImage/rootfs.yaffs)into MEM by 'i' key...\n\
	\r- [a] write the app.bin application image into MEM by 'a' key...\n\
	\r- [q] if you press the 'q' key,will quit the menu list...\n\
	you can use this command open a menu list in order to write image conveniency"
);

