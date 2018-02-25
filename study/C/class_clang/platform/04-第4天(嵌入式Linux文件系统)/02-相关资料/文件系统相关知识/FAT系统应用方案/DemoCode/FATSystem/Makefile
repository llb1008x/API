#####################################################################
#																	 
#	Created by u'nSP IDE		14:39:19	09/26/06
#
#####################################################################




APPDIR	= C:\PROGRA~1\Sunplus\UNSPID~1.0

OUTDIR	= .\Debug

CC	= $(APPDIR)\gcc

AS	= $(APPDIR)\xasm16

LD	= $(APPDIR)\xlink16

AR	= $(APPDIR)\xLib16

RESC	= $(APPDIR)\resc

RM	= $(APPDIR)\rm -f

INCLUDES	= -I"E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup" -I"C:/Program Files/Sunplus/unSP IDE 2.0.0/library/CLib16/include"

BODY	= -body SPCE060A_061A -bfile "C:\Program Files\Sunplus\unSP IDE Common\Body\SPCE060A_061A.cpt"

BINFILE	= "$(OUTDIR)\FATSystem.S37"

ARYFILE	= "$(OUTDIR)\FATSystem.ary"

SBMFILE	= "$(OUTDIR)\FATSystem.sbm"

OPT	= -S -gstabs -Wall -mglobal-var-iram

ASFLAGS	= -t2 -d

CASFLAGS	= -t2 

CFLAGS	= $(OPT) -B$(APPDIR)\ $(INCLUDES) 

BINTYPE	= -as

LDFLAGS	= 

EXTRAFLAGS	= 


OBJFILES	= \
	"$(OUTDIR)\Resource.obj" \
	"$(OUTDIR)\IRQ.obj" \
	"$(OUTDIR)\Main.obj" \
	"$(OUTDIR)\SD_Hardware.obj" \
	"$(OUTDIR)\SD_CRC.obj" \
	"$(OUTDIR)\SD_Driver.obj" \
	"$(OUTDIR)\SD_Command.obj" \
	"$(OUTDIR)\Key.obj" \
	"$(OUTDIR)\SPLC501User.obj" \
	"$(OUTDIR)\SPLC501Driver_IO.obj" \
	"$(OUTDIR)\DataOSforLCD.obj" \
	"$(OUTDIR)\FAT_File.obj" \
	"$(OUTDIR)\FAT_Dir.obj" \
	"$(OUTDIR)\FAT_Disk.obj" \
	"$(OUTDIR)\FAT_Driver.obj" \
	"$(OUTDIR)\FAT_FAT.obj" \
	"$(OUTDIR)\FAT_FDT.obj" \
	"$(OUTDIR)\FAT_Cache.obj" 

"$(OUTDIR)\Resource.obj": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\Resource.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\Resource.lst" -o "$(OUTDIR)\Resource.obj" "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\Resource.asm" 

"$(OUTDIR)\IRQ.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\IRQ.c" ".\SPCE061A.h" ".\Key\Key.h" "SPCE061A.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/IRQ.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/IRQ.c" 

"$(OUTDIR)\IRQ.obj": "$(OUTDIR)\IRQ.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\IRQ.lst" -o "$(OUTDIR)\IRQ.obj" "$(OUTDIR)\IRQ.asm" 

"$(OUTDIR)\Main.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\Main.c" ".\FAT\FAT.h" ".\SD\SD_INC.h" ".\SPLC501\SPLC501User.h" ".\Key\Key.h" ".\SD\SD_Hardware.h" ".\SD\SD_CRC.h" ".\SD\SD_Command.h" ".\SD\SD_Driver.h" "C:\Program Files\Sunplus\unSP IDE 2.0.0\library\CLib16\include\String.h" "C:\Program Files\Sunplus\unSP IDE 2.0.0\library\CLib16\include\_ansi.h" "C:\Program Files\Sunplus\unSP IDE 2.0.0\library\CLib16\include\sys\reent.h" "C:\Program Files\Sunplus\unSP IDE 2.0.0\library\CLib16\include\stddef.h" "spce061a.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Main.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/Main.c" 

"$(OUTDIR)\Main.obj": "$(OUTDIR)\Main.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\Main.lst" -o "$(OUTDIR)\Main.obj" "$(OUTDIR)\Main.asm" 

"$(OUTDIR)\SD_Hardware.obj": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SD\SD_Hardware.asm" ".\SD\SD_CFG.inc" "spce061a.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\SD_Hardware.lst" -o "$(OUTDIR)\SD_Hardware.obj" "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SD\SD_Hardware.asm" 

"$(OUTDIR)\SD_CRC.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SD\SD_CRC.c" ".\SD\SD_INC.h" ".\SD\SD_Hardware.h" ".\SD\SD_CRC.h" ".\SD\SD_Command.h" ".\SD\SD_Driver.h" "spce061a.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_CRC.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/SD/SD_CRC.c" 

"$(OUTDIR)\SD_CRC.obj": "$(OUTDIR)\SD_CRC.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\SD_CRC.lst" -o "$(OUTDIR)\SD_CRC.obj" "$(OUTDIR)\SD_CRC.asm" 

"$(OUTDIR)\SD_Driver.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SD\SD_Driver.c" ".\SD\SD_INC.h" ".\SD\SD_Hardware.h" ".\SD\SD_CRC.h" ".\SD\SD_Command.h" ".\SD\SD_Driver.h" "spce061a.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_Driver.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/SD/SD_Driver.c" 

"$(OUTDIR)\SD_Driver.obj": "$(OUTDIR)\SD_Driver.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\SD_Driver.lst" -o "$(OUTDIR)\SD_Driver.obj" "$(OUTDIR)\SD_Driver.asm" 

"$(OUTDIR)\SD_Command.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SD\SD_Command.c" ".\SD\SD_INC.h" ".\SD\SD_Hardware.h" ".\SD\SD_CRC.h" ".\SD\SD_Command.h" ".\SD\SD_Driver.h" "spce061a.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SD_Command.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/SD/SD_Command.c" 

"$(OUTDIR)\SD_Command.obj": "$(OUTDIR)\SD_Command.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\SD_Command.lst" -o "$(OUTDIR)\SD_Command.obj" "$(OUTDIR)\SD_Command.asm" 

"$(OUTDIR)\Key.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\KEY\Key.c" ".\KEY\Key.h" "SPCE061A.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/Key.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/KEY/Key.c" 

"$(OUTDIR)\Key.obj": "$(OUTDIR)\Key.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\Key.lst" -o "$(OUTDIR)\Key.obj" "$(OUTDIR)\Key.asm" 

"$(OUTDIR)\SPLC501User.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SPLC501\SPLC501User.c" ".\SPLC501\SPLC501User.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/SPLC501User.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/SPLC501/SPLC501User.c" 

"$(OUTDIR)\SPLC501User.obj": "$(OUTDIR)\SPLC501User.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\SPLC501User.lst" -o "$(OUTDIR)\SPLC501User.obj" "$(OUTDIR)\SPLC501User.asm" 

"$(OUTDIR)\SPLC501Driver_IO.obj": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SPLC501\SPLC501Driver_IO.asm" ".\SPLC501\SPLC501Driver_IO.inc" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\SPLC501Driver_IO.lst" -o "$(OUTDIR)\SPLC501Driver_IO.obj" "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SPLC501\SPLC501Driver_IO.asm" 

"$(OUTDIR)\DataOSforLCD.obj": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SPLC501\DataOSforLCD.asm" 
	$(AS) $(ASFLAGS) $(INCLUDES) -l "$(OUTDIR)\DataOSforLCD.lst" -o "$(OUTDIR)\DataOSforLCD.obj" "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\SPLC501\DataOSforLCD.asm" 

"$(OUTDIR)\FAT_File.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_File.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_File.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_File.c" 

"$(OUTDIR)\FAT_File.obj": "$(OUTDIR)\FAT_File.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_File.lst" -o "$(OUTDIR)\FAT_File.obj" "$(OUTDIR)\FAT_File.asm" 

"$(OUTDIR)\FAT_Dir.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_Dir.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_Dir.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_Dir.c" 

"$(OUTDIR)\FAT_Dir.obj": "$(OUTDIR)\FAT_Dir.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_Dir.lst" -o "$(OUTDIR)\FAT_Dir.obj" "$(OUTDIR)\FAT_Dir.asm" 

"$(OUTDIR)\FAT_Disk.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_Disk.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_Disk.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_Disk.c" 

"$(OUTDIR)\FAT_Disk.obj": "$(OUTDIR)\FAT_Disk.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_Disk.lst" -o "$(OUTDIR)\FAT_Disk.obj" "$(OUTDIR)\FAT_Disk.asm" 

"$(OUTDIR)\FAT_Driver.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_Driver.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_Driver.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_Driver.c" 

"$(OUTDIR)\FAT_Driver.obj": "$(OUTDIR)\FAT_Driver.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_Driver.lst" -o "$(OUTDIR)\FAT_Driver.obj" "$(OUTDIR)\FAT_Driver.asm" 

"$(OUTDIR)\FAT_FAT.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_FAT.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_FAT.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_FAT.c" 

"$(OUTDIR)\FAT_FAT.obj": "$(OUTDIR)\FAT_FAT.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_FAT.lst" -o "$(OUTDIR)\FAT_FAT.obj" "$(OUTDIR)\FAT_FAT.asm" 

"$(OUTDIR)\FAT_FDT.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_FDT.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_FDT.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_FDT.c" 

"$(OUTDIR)\FAT_FDT.obj": "$(OUTDIR)\FAT_FDT.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_FDT.lst" -o "$(OUTDIR)\FAT_FDT.obj" "$(OUTDIR)\FAT_FDT.asm" 

"$(OUTDIR)\FAT_Cache.asm": "E:\_Work\_2007GraduateProject\FAT System\FATSystem_backup\FAT\FAT_Cache.c" ".\FAT\FAT.h" "SD\SD_INC.h" 
	$(CC) $(CFLAGS) -o "$(OUTDIR)/FAT_Cache.asm" "E:/_Work/_2007GraduateProject/FAT System/FATSystem_backup/FAT/FAT_Cache.c" 

"$(OUTDIR)\FAT_Cache.obj": "$(OUTDIR)\FAT_Cache.asm"
	$(AS) $(CASFLAGS) $(INCLUDES) -l "$(OUTDIR)\FAT_Cache.lst" -o "$(OUTDIR)\FAT_Cache.obj" "$(OUTDIR)\FAT_Cache.asm" 


.SUFFIXES : .c .asm .obj .s37 .tsk .res

all :	 "$(OUTDIR)" $(BINFILE)

"$(OUTDIR)" :
	if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(BINFILE) : $(OBJFILES) 
	$(LD) $(BINTYPE) $(ARYFILE) $(BINFILE) $(LDFLAGS) $(BODY) $(EXTRAFLAGS)

compile :	 $(OBJFILES)

clean :
	$(RM) "$(OUTDIR)\Resource.obj" 
	$(RM) "$(OUTDIR)\Resource.lst" 
	$(RM) "$(OUTDIR)\IRQ.obj" 
	$(RM) "$(OUTDIR)\IRQ.lst" 
	$(RM) "$(OUTDIR)\IRQ.asm" 
	$(RM) "$(OUTDIR)\Main.obj" 
	$(RM) "$(OUTDIR)\Main.lst" 
	$(RM) "$(OUTDIR)\Main.asm" 
	$(RM) "$(OUTDIR)\SD_Hardware.obj" 
	$(RM) "$(OUTDIR)\SD_Hardware.lst" 
	$(RM) "$(OUTDIR)\SD_CRC.obj" 
	$(RM) "$(OUTDIR)\SD_CRC.lst" 
	$(RM) "$(OUTDIR)\SD_CRC.asm" 
	$(RM) "$(OUTDIR)\SD_Driver.obj" 
	$(RM) "$(OUTDIR)\SD_Driver.lst" 
	$(RM) "$(OUTDIR)\SD_Driver.asm" 
	$(RM) "$(OUTDIR)\SD_Command.obj" 
	$(RM) "$(OUTDIR)\SD_Command.lst" 
	$(RM) "$(OUTDIR)\SD_Command.asm" 
	$(RM) "$(OUTDIR)\Key.obj" 
	$(RM) "$(OUTDIR)\Key.lst" 
	$(RM) "$(OUTDIR)\Key.asm" 
	$(RM) "$(OUTDIR)\SPLC501User.obj" 
	$(RM) "$(OUTDIR)\SPLC501User.lst" 
	$(RM) "$(OUTDIR)\SPLC501User.asm" 
	$(RM) "$(OUTDIR)\SPLC501Driver_IO.obj" 
	$(RM) "$(OUTDIR)\SPLC501Driver_IO.lst" 
	$(RM) "$(OUTDIR)\DataOSforLCD.obj" 
	$(RM) "$(OUTDIR)\DataOSforLCD.lst" 
	$(RM) "$(OUTDIR)\FAT_File.obj" 
	$(RM) "$(OUTDIR)\FAT_File.lst" 
	$(RM) "$(OUTDIR)\FAT_File.asm" 
	$(RM) "$(OUTDIR)\FAT_Dir.obj" 
	$(RM) "$(OUTDIR)\FAT_Dir.lst" 
	$(RM) "$(OUTDIR)\FAT_Dir.asm" 
	$(RM) "$(OUTDIR)\FAT_Disk.obj" 
	$(RM) "$(OUTDIR)\FAT_Disk.lst" 
	$(RM) "$(OUTDIR)\FAT_Disk.asm" 
	$(RM) "$(OUTDIR)\FAT_Driver.obj" 
	$(RM) "$(OUTDIR)\FAT_Driver.lst" 
	$(RM) "$(OUTDIR)\FAT_Driver.asm" 
	$(RM) "$(OUTDIR)\FAT_FAT.obj" 
	$(RM) "$(OUTDIR)\FAT_FAT.lst" 
	$(RM) "$(OUTDIR)\FAT_FAT.asm" 
	$(RM) "$(OUTDIR)\FAT_FDT.obj" 
	$(RM) "$(OUTDIR)\FAT_FDT.lst" 
	$(RM) "$(OUTDIR)\FAT_FDT.asm" 
	$(RM) "$(OUTDIR)\FAT_Cache.obj" 
	$(RM) "$(OUTDIR)\FAT_Cache.lst" 
	$(RM) "$(OUTDIR)\FAT_Cache.asm" 
	$(RM) $(BINFILE) $(SBMFILE) 

.c.asm:
	$(CC) $(CFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

.asm.obj:
	$(AS) $(ASFLAGS) $(INCLUDES) -o "$(OUTDIR)/$@" $<

