#include <common.h>

#if 0
	#define DEBUGN    printf
#else
	#define DEBUGN(x, args ...) {}
#endif

#include <nand.h>
#include <asm/arch/s3c2410.h>
#include <asm/io.h>

#define __REGb(x)    (*(volatile unsigned char *)(x))
#define __REGi(x)    (*(volatile unsigned int *)(x))

#define NF_BASE  0x4e000000             //Nand���üĴ�������ַ
#define NFCONF   __REGi(NF_BASE + 0x0)  //ƫ�ƺ��ǵõ����üĴ�������ַ
#define NFCONT   __REGi(NF_BASE + 0x4)  //ƫ�ƺ�õ�Nand���ƼĴ�������ַ
#define NFCMD    __REGb(NF_BASE + 0x8)  //ƫ�ƺ�õ�Nandָ��Ĵ�������ַ
#define NFADDR   __REGb(NF_BASE + 0xc)  //ƫ�ƺ�õ�Nand��ַ�Ĵ�������ַ
#define NFDATA   __REGb(NF_BASE + 0x10) //ƫ�ƺ�õ�Nand���ݼĴ�������ַ
#define NFMECCD0 __REGi(NF_BASE + 0x14) //ƫ�ƺ�õ�Nand����������ECC0�Ĵ�������ַ
#define NFMECCD1 __REGi(NF_BASE + 0x18) //ƫ�ƺ�õ�Nand����������ECC1�Ĵ�������ַ
#define NFSECCD  __REGi(NF_BASE + 0x1C) //ƫ�ƺ�õ�Nand��������ECC�Ĵ�������ַ
#define NFSTAT   __REGb(NF_BASE + 0x20) //ƫ�ƺ�õ�Nand״̬�Ĵ�������ַ
#define NFSTAT0  __REGi(NF_BASE + 0x24) //ƫ�ƺ�õ�Nand ECC0״̬�Ĵ�������ַ
#define NFSTAT1  __REGi(NF_BASE + 0x28) //ƫ�ƺ�õ�Nand ECC1״̬�Ĵ�������ַ
#define NFMECC0  __REGi(NF_BASE + 0x2C) //ƫ�ƺ�õ�Nand����������ECC0״̬�Ĵ�������ַ
#define NFMECC1  __REGi(NF_BASE + 0x30) //ƫ�ƺ�õ�Nand����������ECC1״̬�Ĵ�������ַ
#define NFSECC   __REGi(NF_BASE + 0x34) //ƫ�ƺ�õ�Nand��������ECC״̬�Ĵ�������ַ
#define NFSBLK   __REGi(NF_BASE + 0x38) //ƫ�ƺ�õ�Nand�鿪ʼ��ַ
#define NFEBLK   __REGi(NF_BASE + 0x3c) //ƫ�ƺ�õ�Nand�������ַ

 

#define S3C2440_NFCONT_nCE  (1<<1)
#define S3C2440_ADDR_NALE   0x0c
#define S3C2440_ADDR_NCLE   0x08

 

ulong IO_ADDR_W = NF_BASE;

 

static void s3c2440_hwcontrol(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
    struct nand_chip *chip = mtd->priv;

    DEBUGN("hwcontrol(): 0x%02x 0x%02x\n", cmd, ctrl);

    if (ctrl & NAND_CTRL_CHANGE) {
        IO_ADDR_W = NF_BASE;

        if (!(ctrl & NAND_CLE))                //Ҫд���ǵ�ַ
            IO_ADDR_W |= S3C2440_ADDR_NALE;
        if (!(ctrl & NAND_ALE))                //Ҫд��������
            IO_ADDR_W |= S3C2440_ADDR_NCLE;

        if (ctrl & NAND_NCE) 
            NFCONT &= ~S3C2440_NFCONT_nCE;    //ʹ��nand flash
        else
            NFCONT |= S3C2440_NFCONT_nCE;     //��ֹnand flash
    }

    if (cmd != NAND_CMD_NONE)
        writeb(cmd,(void *)IO_ADDR_W);
}

 

static int s3c2440_dev_ready(struct mtd_info *mtd)
{
    DEBUGN("dev_ready\n");
    return (NFSTAT & 0x01);
}

 

int board_nand_init(struct nand_chip *nand)
{
    u_int32_t cfg;
    u_int8_t tacls, twrph0, twrph1;
    S3C24X0_CLOCK_POWER * const clk_power = s3c24x0_get_base_clock_power();

 

    DEBUGN("board_nand_init()\n");

 

    clk_power->CLKCON |= (1 << 4);

 

    twrph0 = 4; twrph1 = 2; tacls = 0;

 

    cfg = (tacls<<12)|(twrph0<<8)|(twrph1<<4);
    NFCONF = cfg;


    cfg = (1<<6)|(1<<4)|(0<<1)|(1<<0);
    NFCONT = cfg;
 

    /* initialize nand_chip data structure */
    nand->IO_ADDR_R = nand->IO_ADDR_W = (void *)0x4e000010;

 

    /* read_buf and write_buf are default */
    /* read_byte and write_byte are default */

    /* hwcontrol always must be implemented */
    nand->cmd_ctrl = s3c2440_hwcontrol;

 

    nand->dev_ready = s3c2440_dev_ready;

 

    return 0;
}
