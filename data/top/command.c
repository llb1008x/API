﻿

看log要细心，有耐心

1.adb常用的命令

:
注意：
     要使用adb命令，手机需要打开USB debug。
     电脑这边要对adb进行配置

     安装软件：
		adb install   apk文件名称.apk
	
     重新安装该软件：
		adb install -r apk文件名称.apk

     卸载apk软件：
		adb uninstall apk包名.apk

     查看手机上的运行日志：
		adb logcat

     查看手机是否连接：
		adb devices
	
     A为手机路径，B为电脑路径，意思为：把文件从手机中复制到电脑上
    	adb pull	<远程路径>  <本地路径> 

     A为手机路径，B为电脑路径，意思为：把文件从电脑复制到手机上
     	adb push	<本地路径>  <远程路径>

     进入手机的超级终端Terminal
     	adb shell

     重新挂载文件系统(使系统分区重新可写,或者改变读写权限): 
		adb remount		/* mount -o remount,rw -t yaffs2 /dev/block/mtdblock3 /system */
    
     重启手机
		adb reboot
	
     重启到Recovery界面
		adb reboot recovery
	
     重启到bootloader界面
		adb reboot bootloader


    手机正常的log在/sdcard/mtklog下（死机log在/data/anr下）
		adb pull  /sdcard/mtklog/mobilelog    /home/llb/project/DEBUG/
	
		adb pull  /sdcard/GNLogs   /home/llb/project/DEBUG/


    手机功耗问题导出电池和相关的参数：
		adb shell dumpsys batterystats > battersystats.log
		adb shell cat /sys/kernel/debug/wakeup_sources > wakeup_sources.log


    **通过wifi连接ADB
	{
	
		adb shell "setprop persist.adb.tcp.port 5555"
		

		Disconnect the device from the PC USB, and do the following from your command prompt using adb over WiFi (replace "192.168.68.167" with your device  IP address):

		adb connect 192.168.68.167 
	}
	

	**这个例子有待验证
	{
		例子：刷官方高版本后重新获取ROOT和降级方法（部份操作）

		进行adb运行环境
		adb shell

		设置超级用户模式
		su

		映射出目录
		mount -o remount,rw -t yaffs2 /dev/block/mtdblock3 /system

		退出su
		exit

		退出shell准备接受下一个adb命令
		exit

		增加flash_image文件到手机上
		adb push flash_image.zip /system/bin/flash_image

		recovery先复制到卡上
		adb push recovery-RA-heroc-v1.6.2.img /sdcard

		再次进入adb环境
		adb shell

		修改文件属性
		chmod 0755 /system/bin/flash_image
	}
	
	
	打开串口log打印开关
    adb shell setprop persist.uartconsole.enable 1	
	
	
	打印上层main_log,system_log
	adb  shell logcat -b main 
	adb  shell logcat -b system
	
	
	adb  shell logcat -b main -v threadtime >error_log1.txt


	MTK平台GPIO adb控制方法如下：

	列出GPIO的状态：
	adb shell "cat /sys/devices/virtual/misc/mtgpio/pin"

	//举例，通过控制GPIO输出一个低电平，可以用示波器查看，如果你要控制其他GPIO，请把145换成别的数字
	//设为GPIO24为  mode 0，
	adb shell "echo -wmode 24 0 > /sys/devices/virtual/misc/mtgpio/pin"
	//设为输出
	adb shell "echo -wdir 24 1 > /sys/devices/virtual/misc/mtgpio/pin"
	//输出高
	adb shell "echo -wdout 24 1 > /sys/devices/virtual/misc/mtgpio/pin"
	//输出低
	adb shell "echo -wdout 24 0 > /sys/devices/virtual/misc/mtgpio/pin"




	高通平台adb 命令控制GPIO状态的方法高通有对应文档，但尝试后不能用，如有需要请按如下方法使用：

    In sdm660, it has north/center/south tlmm, each tlmm has different base register address, like north tlmm is 0x3000000+NORTH,
you could check pinctrl-sdm660.c to get each gpio's base register address .'

    #define NORTH 0x00900000 
    #define CENTER 0x00500000 
    #define SOUTH 0x00100000 
    #define REG_SIZE 0x1000

    PINGROUP(29, CENTER, blsp_spi8_a, blsp_uart6_b, NA, NA, phase_flag22, NA, NA, NA, NA),

    So GPIO29 cfg register address is 0x3000000+CENTER + REG_SIZE *29

    config CONFIG_DEVMEM=y, then use  /system/bin/r  register address


    寄存器bit位表示的意思可查看高通文档：SP80-P8754-6（SDM660/SDM630 Linux Android Software Porting Manual）  section 4.1.1 GPIO configuration

eg：
        1. 读寄存器TLMM_GPIO_CFG29(0x3000000+CENTER + REG_SIZE * 29)命令
        adb shell
        system/bin/r 0x351D000
        输出：0351d000: 00000201

        2. 读TLMM_GPIO_IN_OUT29(0x3000000+CENTER + REG_SIZE * 29 +4 )命令：
        adb shell
        system/bin/r 0x351D004
        输出：0351d004: 00000003

        3. 写TLMM_GPIO_CFG29寄存器：
        system/bin/r 0x351D000 0x01
        输出：0351d000: 00000001



2.fastboot常用命令(这写命令是G1602A烧写相关的文件，相关命令还要重新看)：
进入fastboot模式
要加sudo权限，按音量下
adb reboot-bootloader

	fastboot flash modem NON-HLOS.bin
	fastboot flash splash splash.img
	fastboot flash sbl1 sbl1.mbn
	fastboot flash sbl1bak sbl1.mbn
	fastboot flash rpm  rpm.mbn
	fastboot flash rpmbak rpm.mbn
	fastboot flash tz tz.mbn
	fastboot flash tzbak tz.mbn
	fastboot flash devcfg devcfg.mbn
	fastboot flash devcfgbak devcfg.mbn

	fastboot flash dsp adspso.bin
	fastboot flash aboot emmc_appsboot.mbn
	fastboot flash abootbak emmc_appsboot.mbn
	fastboot flash boot  boot.img
	fastboot flash recovery recovery.img
	fastboot flash system system.img
	fastboot flash cache cache.img
	fastboot flash persist persist.img
	fastboot flash mdtp mdtp.img


	fastboot flash cmnlib cmnlib.mbn
	fastboot flash cmnlibbak cmnlib.mbn

	fastboot flash cmnlib64 cmnlib64.mbn
	fastboot flash cmnlib64bak cmnlib64.mbn
	fastboot flash keymaster keymaster.mbn
	fastboot flash keymasterbak  keymaster.mbn

	fastboot flash userdata userdata.img

	当然这里手机除了用组合键进入fastboot还可以用adb命令进入到相应的模式烧写文件


qcom 烧写 当然要换路径
	sudo ./fastboot flash modem /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/NON-HLOS.bin
	sudo ./fastboot flash splash /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/splash.img
	sudo ./fastboot flash sbl1 /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/sbl1.mbn
	sudo ./fastboot flash sbl1bak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/sbl1.mbn
	sudo ./fastboot flash rpm  /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/rpm.mbn
	sudo ./fastboot flash rpmbak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/rpm.mbn
	sudo ./fastboot flash tz /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/tz.mbn
	sudo ./fastboot flash tzbak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/tz.mbn
	sudo ./fastboot flash devcfg /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/devcfg.mbn
	sudo ./fastboot flash devcfgbak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/devcfg.mbn

	sudo ./fastboot flash dsp /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/adspso.bin
	sudo ./fastboot flash aboot /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/emmc_appsboot.mbn
	sudo ./fastboot flash abootbak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/emmc_appsboot.mbn
	sudo ./fastboot flash boot  /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/boot.img
	sudo ./fastboot flash recovery /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/recovery.img
	sudo ./fastboot flash system /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/system.img
	sudo ./fastboot flash cache /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/cache.img
	sudo ./fastboot flash persist /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/persist.img
	sudo ./fastboot flash mdtp /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/mdtp.img


	sudo ./fastboot flash cmnlib /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/cmnlib.mbn
	sudo ./fastboot flash cmnlibbak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/cmnlib.mbn

	sudo ./fastboot flash cmnlib64 /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/cmnlib64.mbn
	sudo ./fastboot flash cmnlib64bak /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/cmnlib64.mbn
	sudo ./fastboot flash keymaster /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/keymaster.mbn
	sudo ./fastboot flash keymasterbak  /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/keymaster.mbn

	sudo ./fastboot flash userdata /home/llb/project/PRO/version/17G06A/BJ17G06A01_A_T0003/userdata.img

	sudo ./fastboot reboot



3.git/repo常用命令：	
注意git/repo相关的操作如git status这个很重要，进行相关操作时一定要注意当前分支是否干净

	git:
		1.清理残留
		git clean  -fdx  .  清理编译或残留的一些文件

		git reset HEAD

		git checkout     .  恢复到以前提交的状态

		
		2.git代码更新命令
		git fetch			更新代码(下载到.git文件夹内还未导出到项目里)
		git merge origin/code_main_dev  合并到主分支


		3.git log --oneline察看所有提交情况

	
		4.git diff HEAD  与主目录的差异(这个是当前分支所有的文件比较)
	

		5.git status	 察看工作区的状态

		
		6.git rm -r --cache 文件和路径	删除工作区内的文件，就是不跟踪这个文件的状态


		7.git branch -D         删除分支

		
		8.git checkout -b  LINUX_G1605A_GNDCR#_LLB  origin/code_main_dev	跟踪主分支创建自己的本地分支

		
		9.git tag   察看软件版本号
		git 中的tag指向一次commit的id，通常用来给开发分支做一个标记，如标记一个版本号。

		打标签
		git tag -a v1.01 -m "Relase version 1.01"
		注解：git tag 是打标签的命令，-a 是添加标签，其后要跟新标签号，-m 及后面的字符串是对该标签的注释。
		
		删除标签
		git tag -d v1.01
		注解：-d 表示删除，后面跟要删除的tag名字

		
		10. git ls-files 察看版本库区内的文件


		**11.git 回滚操作，回撤（这个命令还有疑问，相关内容）
		git    commit   --amend  
		合并缓存区的修改和最近的一次commit, 然后用生成的新的commit替换掉老的. 如果缓存区没有内容, 那么利用amend可以修改上一次commit的描述.
	

		12.常用的命令(提交之前先fetch，merge避免修改的冲突)
		git add .			添加文件到暂存区
		git fetch 
		git merge  origin/code_main_dev
		git commit -m  "LINUX_17G05A_GNDCR#_LLB" 提交自己的修改
		git push origin LINUX_17G05A_GNDCR#_LLB:code_main_dev 提交到提交自己的代码到远程代码库 

		添加gerrit服务器之后提交要先提交到gerrit
		git push origin LINUX_17G05A_GNDCR#_LLB:refs/for/code_main_dev
		
		
		13.删除一个远程分支
		git push origin --delete  <branchName>这样远程分支是删除了，但是本地仍然有远程分支的副本所以要下面的命令
		
		git branch -d -r origin/pending-issues-in-project只是删除该分支
		
		git remote prune origin删除所有此类陈旧的远程分支机构。也可以更新在git remote update --prune origin


		14.git rebase
		git  rebase  origin/code_main_dev     丢弃本地的commit ，并同步迁移到所要提交的分支，自动更新主分支的提交 
		从命令格式，可以看到git-rebae命令至少需要一个参数，那就是，这个参数可以是一个分支名称，也可以是一次有效的commit。
		
		
		15.git clone gitserver:/L09_BBT5201A_MT6572TDV1.MP.P34_JB3_130428
		ssh GerritServer			

		git rebase origin/code_main_dev

		

	repo:
		1.repo forall -cvp git branch -a   察看整个repo管理目录下的分支


		2.repo更新整个项目工程
		repo forall -vcp git fetch			把服务器代码库上的代码下到本地
		repo forall -vcp git branch <your branch name> GNBJ-LTE-MT37-AN-G1605A_CTA-031.00.000-161014AA
		repo forall -vcp git checkout <your branch name>
	
		
		3.repo forall -cvp gitk 察看项目下所有分支的情况（图形界面）
		
			
		4.repo封装的操作 对整个项目文件都操作
		清除
		repo forall -vcp git clean -dfx
		repo forall -vcp git reset HEAD
		repo forall -vcp git checkout .
		更新
		repo forall -vcp git fetch
		repo forall -vcp git merge origin/code_main_dev
		
		追踪服务器的分支，建立自己的本地分支
		repo forall -vcp git branch 17g05a_sp_main_compiler origin/17g05a_sp_main_compiler
		
		repo forall -vcp git checkout 17g05a_sp_main_compiler
		
		当然之前要先清掉本地的改动或提交
		git checkout .;git clean -dxf
		
		然后才能git fetch ，git merge 
		
		repo forall -vcp git branch  -avv
	
		
			
		5.下载深圳项目代码
		./repo_19.9.0.151 init -u ssh://lilubao@19.9.0.151:29418/manifest.git -b android_mtk_n_6737m_65_mp1 -m master.xml 

		./repo_19.9.0.151 init -u ssh://lilubao@19.9.0.151:29418/manifest -b gionee_packages_apk_amigo -m SW17W05_oversea_master.xml

		第一次提交之前通过hook使每次提交生成change ID	
		repo_19.9.0.151 forall  -c  "scp  -p  -P  29418 lilubao@19.9.0.151:hooks/commit-msg  .git/hooks/"
		
		
		6.更新代码有关APK的目录怎么处理
		git clean -dxf 
		git reset HEAD
		git checkout .	


		7.海外，深圳代码构建的方式
		android代码下载：
		mkdir ~/alps
		cd ~/alps
		./repo_19.9.0.151 init -u ssh://lilubao@19.9.0.151:29418/manifest -b android_mtk_l_6753_65c_mp3 -m master.xml
		./repo_19.9.0.151 sync --no-tags
		./repo_19.9.0.151 start master --all

		cd ../gionee_packages_apk_amigo_3.1
		./repo_19.9.0.151 init -u ssh://lilubao@19.9.0.151:29418/manifest.git -b gionee_packages_apk_amigo_3.1 -m BBL7505_master.xml
		./repo_19.9.0.151 sync --no-tags
		./repo_19.9.0.151 start master --all
		下载完成后，结构必须是：
		.
		├── alps
		│   └── android_mtk_l_6753_65c_mp3
		└── gionee_packages_apk_amigo_3.1
		

		hi all：
          为了方便后续维护、代码同步，智能机项目的gionee目录和modem目录开启代码修改注释和提交注释的检查机制。
        具体检查项：
		1. 使用git提交代码的时候commit信息规范：

			提交分类  #ID 描述

			提交分类：  Fixed、Revert

			#ID： ID为redmine系统中问题的ID，此处可以为多个#ID,#ID

			描述：简短说明。

			示例：
--->		git commit -m "Fixed #ID 问题描述"
			git commit -m "Fixed #ID,#ID,#ID 问题描述"

		2. 对文本类型的代码文件修改，必须有gionee注释，并且请按照下面的规则：
			注释符Gionee  <Module>  <Auther>  <date>  描述  结束符

		注释符：//、#、/**/、<!-- -->。请结合文件类型选择。

				Module：以Amigo_、GN_BSP_、GN_Oversea_、gn_by_开头和Bug。

				Auther：修改代码的作者的Email ID(后缀就不要了)

				Date： 修改代码的日期。如20161115（2016-11-15也可以）

		描述：add for CR 或者modify for CR 或者 remove for CR 等（不管怎么描述请带上Bug号）。

		结束符： begin 或者 end 表示结尾

		注意：北京的 Module选择gn_by_

		示例：#Gionee <gn_by_scm> <ranyi> <2017-01-02> add for #5678 begin 

--->	#Gionee <gn_by_charging> <lilubao> <2017-01-02> add for  begin


		3. 如果以上两个规则都不满足，则将无法正常push代码，请各位尤其注意。

		如有疑问请联系 赖洪浩。

		需求：
			1.新下载项目更新repo脚本2.3
				http://10.8.0.20/redmine/attachments/download/16110/repo_init_env_sv2.3.7z
			2.已经下载的项目在.git同级目录执行  
				scp -r -p -P 29418 xxx@10.8.10.19:hooks/ .git

--->   	scp -r -p -P 29418 lilubao@10.8.10.19:hooks/ .git






4.进入MTK工程模式暗码
		*#*#3646633#*#*

		*#446633#

		*#*#4636#*#*   安卓工程模式

		*#8702#        测试模式

		*#2523#	       察看手机信息

		*#837500#      察看手机版本

		*#837504#	   察看系统的一些信息

		*#837726#	   老化测试

		*#837004#      appsiper配置打开很多内容


5.项目编译相关的命令：

		脚本编译的版本(当然不同的项目还有很多不同的版本)：
	
		BBL7337A ：全版本user，debug
	
		BBL7337A_CMCC：要送到中国移动部测试用的，基本的软硬件，没有金立定制的软件      
	
		BBL7337A_CT：

		BBL7337A_CTA ：         

		BBL7337A_platform   ：没有上层framework以上的，只有底层驱动及以下的部分

		BBL7337A_sign:签名版


		 编译命令：
		./TmakeGionee  G1605A   -C  将gionee目录内的代码拷贝到各各目录下，覆盖
		./TmakeGionee  G1605A   -n  全编整个项目
		./TmakeGionee  G1605A   -r  remake 重编，在原有基础之上
		./TmakeGionee  G1605A   -i  bootimage  lk  system  <bootimage|systemimage|bootimage|modem>编译不同的模块
		./TmakeGionee  G1605A   -v  <user|eng|userdebug>编译不同的版本


		编译某个目录
		./TmakeGionee  G1605A  -p  vendor/mediatek/proprietary/external/charger/
		./TmakeGionee  G1605A  -p  vendor/mediatek/proprietary/external/libshowlogo/
		推送到手机bin下

		adb remount （重新挂载系统分区，使系统分区重新可写，不然无法将文件，bin推送到system/bin）
		adb push /home/llb/project/PRO/source/G1605A/L28_6737M_65_G1605A_M0.MP1_V2.140.3_160908_ALPS/android_mtk_6737m_65_mp/out/target/product/gnbj6737t_66_m0/system/bin/kpoc_charger   system/bin/
		adb push /home/llb/project/PRO/source/G1605A/L28_6737M_65_G1605A_M0.MP1_V2.140.3_160908_ALPS/android_mtk_6737m_65_mp/out/target/product/gnbj6737t_66_m0/system/lib/libshowlogo.so  system/lib/
		
		当前项目环境下编写一个测试程序
		在vendor创建一个目录
		/home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/vendor/mediatek/proprietary/external/motor_test
		添加Android.mk脚本
		{
			LOCAL_PATH := $(call my-dir)

			include $(CLEAR_VARS)

			LOCAL_MODULE_TAGS := optional

			LOCAL_MODULE := moto_test	//要使用的源码文件生成的文件

			LOCAL_SRC_FILES := $(call all-subdir-c-files)

			include $(BUILD_EXECUTABLE)
		}

		在out目录下生成二进制文件
		/home/llb/project/PRO/source/17G05A/L30_6757_17G05A_N0.MP5_161227_ALPS/android_mtk_6757_mp/out/target/product/gnbj6757_66_n/system/bin


		./TmakeGionee  G1605A  -a

		17G02A编译
		{
			repo init -u gitserver:/manifest.git -m 17G02.xml ; repo sync

			17G02 安装 pyyaml pip uninstall pyyaml
			sudo apt-get install libyaml-cpp-dev
			pip install pyyaml

			sudo pip uninstall pyyaml
			sudo apt-get install libyaml-cpp-dev
			sudo pip install pyyaml

			sudo apt-get install libyaml-dev
			解压附件 执行如下命令
			python setup.py build
			python setup.py install

			sudo apt-get install libxml-simple-perl

			sudo apt-get install libssl-dev

			sudo apt-get -y install sqlite3
			sudo apt-get -y install libsqlite3-dev 


			sudo apt-get install  smbclient
			sudo apt-get install  python-yaml

		}


		调整log时间
		./ktime -i kernel_log  -o  kernel_log_output



		反编dtb文件
		./out/target/product/gnbj6757_66_n/obj/KERNEL_OBJ/scripts/dtc/dtc  -I dtb -O dts -o  gnbj6757_66_n.dts ./out/target/product/gnbj6757_66_n/obj/KERNEL_OBJ/arch/arm64/boot/dts/mediatek/gnbj6757_66_n.dtb


		改变电脑上java jdk javac的版本
		sudo update-alternatives --config java
	
		sudo update-alternatives --config javac

		sudo update-alternatives --config javadoc
	
		察看当前的java版本java -version


		本地编译签名版本
		{
			目前我们的项目中 BBL7515A BBL7516A 仅这两个项目开启了 SecurityBoot 功能（此功能是为防止刷机）；
			由于相关的秘钥保存于服务器上（私钥不能公开），因此开发工程师无法在本地进行签名版本的调试；
			为满足各童鞋可以在本地编译出签名的镜像文件，并打上签名进行调试，现开发了一个小工具，方便大家进行签名，具体使用步骤如下（工具见附件）：
			1. 将附件解压到本地，任意目录，建议创建一个空目录，方便存放，同时将你需要签名的文件也放入其中，如下（此处以logo.bin举例）：
				

			2. 命令行下进入该目录，然后按照下面的格式执行：
			用法: ./gn_sign_clint.py [PROJECT_NAME] [TYPE] [File Name]
			参数说明：
			PROJECT_NAME --- 需要签名的项目名称
			TYPE ----------- 指定签名的类型,可能的值:
							img --- 镜像文件: bin/img
							ota --- 整包或者差分包: zip
			File Name ------ 需要签名的文件
			举例：
				./gn_sign_clint.py BBL7515A img logo.bin
				./gn_sign_clint.py BBL7516A ota BBL7337A_update_amigo3.5.0_T0037.zip

			3. 等待命令完成：
			命令完成后，会将签名后的文件放入到 gionee_sign_release/ranyi_BBL7515A_signed_2016-09-02-17-00-24.zip 目录，并且以压缩包的形式；	

			需要注意的几点：
			1. 执行命令必须严格按照上面例子的格式，并且TYPE参数是必须的，并且只能是 img 或者 ota，这是为了指定签名类型（ota包和镜像签名流程不一样）；
			2. 目前暂时只支持单个文件进行签名，后续会逐步完善多个镜像文件的签名方式；


			编译签名版本的方法：
			./TmakeGionee BBL7515A_sign -n                    编译 BBL7515A debug版本的签名版本
			./TmakeGionee BBL7515A_sign -v user -n        编译 BBL7515A user版本的签名版本

			编译完成后，在release目录生成的刷机镜像中，需要签名的只是以下10个


			现在签名工具可以支持压缩包上传签名了，当有多个镜像文件需要签名的时候，可以这样操作：
			1. 将需要签名的多个镜像压缩打包，目前仅支持3中格式的压缩包： zip  7z  rar
			2. 签名命令的参数直接添加该压缩包的名字：./gn_sign_clint.py BBL7515A img sign.zip 


			北研所有带签名功能项目的签名方式如下：
			./gn_sign_clint.py G1605A img sign.zip 
			./gn_sign_clint.py G1602A img BJ_G1602A_sign_T0146.zip 
			./gn_sign_clint.py BBL7515A img logo.bin
			./gn_sign_clint.py BBL7516A ota BBL7337A_update_amigo3.5.0_T0037.zip


			对于高通平台项目（G1602系列）的签名方式要注意以下几点：
			1. 压缩包名以 BJ_G1602A （具体项目名）打头；
			2. 必须 zip 格式（一定要在linux下压缩）；
			3. zip压缩包内不能包含目录；
			4. 压缩包内必须包含的文件列表：
				8976_fuseblower_USER.xml   
				8976_secimage.xml   
				sign_img_list.txt
				需要签名的镜像文件
				
		}
	 	
		
		更改编译项目脚本
		device/gionee_bj/gnbj6757_66_n/ProjectConfig.mk


		关于17G16A 单烧boot.img后WIFI不能使用的问题。
		单烧boot.img后，需要重新编译Wlan模块。相关命令如下：
		1. Compile CMD:
		./TmakeGionee 17G16A -p vendor/qcom/opensource/wlan/prima/

		2. Target:
		out/target/product/[$Platform]/obj/vendor/qcom/opensource/wlan/prima/pronto_wlan.ko

		3. PUSH to DUT:
		adb push pronto_wlan.ko /system/lib/modules/pronto/

		4. Resatrt DUT:
		adb reboot


		**编译android原生代码相关的命令(这个要实践，搭建android开发环境，编译原生代码)


		关闭sensors
		2. Please follow below steps to disable sensor to test again.
		#1
		LINUX\android\device\qcom\common\rootdir\etc\init.qcom.sh
		...
		//start_sensor
		#2.
		adsp_proc\qdsp6\main\src\main.c
		#if defined(SIM)
		volatile boolean RunSensors = FALSE;
		#else
		volatile boolean RunSensors = TRUE; // Change to FALSE from TRUE
		#endif

6.linux 下常用命令

	locate && find && grep三个命令
	
	find命令：
		find   路径   -name  文件名
		find   路径   -type  f文件/d目录
		
    	查找目录并列出目录下的文件(为找到的每一个目录单独执行ls命令，没有选项-print时文件列表前一行不会显示目录名称)
		find ./ -type d -print -exec ls {} \;
    	(这里的参数 -print是显示找到的文件或者目录；而-exec 相当于创建了一个子进程去执行后面的shell命令；分号不能少，不然不是一个完整的命令)
	
		查找文件名匹配*.c的文件(\*.c如果是*.c好像不行)
		find ./ -name \*.c
	
		查找文件更新日时在距现在时刻二天以内的文件
		find ./ -mtime -2
		查找文件更新日时在距现在时刻二天以上的文件
		find ./ -mtime +2
		
		查找文件size小于10k的文件或目录
		find ./ -size -10k
		
		查找所有者权限有读权限的目录或文件
		find ./ -perm -u=r
		
		查找空文件并删除
		find ./ -empty -type f -print -delete
		
		
	
	grep命令：
		常用组合为grep -rinl "要查找的字符串" 查找范围
		grep  [options] PATTERN [FILE...]即grep [选项]   要查找的字符串    查找范围(查找路径)，

		-r 是递归查找，查找所有文件包含子目录，-i 忽略大小写，-n 是显示行号，-l 只列出匹配的文件名

		


	挂在命令mount ，umount
	
	
		umount  /dev/hda2    ， umount /usr  卸载磁盘分区或设备文件
		
		mount  -t  ext4  /dev/hda5 /mnt/hda5  (t后面跟的参数是文件系统的类型，当然是可以省略的因为linux会自动检测)
		
		mount –o umask=000, iocharset=cp936   /dev/hda5  /mnt/hda5 (解决挂在之后没有权限)



		**有待验证
		{
			nohup command    >/dev/null 2>&1
	
			>/dev/null 2>&1  标准输出和错误输出重定向到空的设备描述符中 （垃圾箱）
			nohup  command 启动一些程序
		}

	
	
		格式化U盘：
		sudo umount /dev/sda1    # 必须先卸载该分区
		# 格式化为 FAT 分区
		sudo mkfs.vfat -F 32 /dev/sda1       # -F 参数必须大写，参数有 12，16 和 32，分别对应 FAT12，FAT16，FAT32 

		sudo fdisk  /dev/sdb 对相应的磁盘进行操作


		文件管理进程
		ps -A | grep nautilus


		tcpdump 
		截取本机（192.168.31.147）和主机114.114.114.114之间的数据
		tcpdump -n -i eth0 host 192.168.31.147 and 114.114.114.114

		还有截取全部进入服务器的数据可以使用以下的格式
		tcpdump -n -i eth0 dst 192.168.31.147
		或者服务器有多个IP 可以使用参数
		tcpdump -n -i eth0 dst 192.168.31.147  or  192.168.31.157

		我们抓取全部进入服务器的TCP数据包使用以下的格式，大家可以参考下
		tcpdump -n -i eth0 dst 192.168.31.147 or 192.168.31.157 and tcp
		从本机出去的数据包
		tcpdump -n -i eth0 src 192.168.31.147 or 192.168.31.157
		tcpdump -n -i eth0 src 192.168.31.147 or 192.168.31.157 and port ! 22 and tcp
		或者可以条件可以是or  和 and  配合使用即可筛选出更好的结果



		sudo apt-get -f install


		sudo fdisk -l	
		sudo df -T -h

7.通过ftp挂在烧写服务器上的rom(当然也可以用文件系统直接登陆，拷贝rom包到本地)

	(这个命令有问题，下面的可以用)
	sudo curlftpfs -o rw,allow_other,uid=1000,gid=1000 ftp://gnftp:gionee@10.8.0.22/software_release/git_release/2016-10/02_ANDROID/G1605A/   /home/llb/project/WORK/ftp/

	GID为GroupId，即组ID，用来标识用户组的唯一标识符
	UID为UserId，即用户ID，用来标识每个用户的唯一标示符
	通过cat /etc/passwd | grep 你的用户名
	会出现类似root:x:0:0:root:/root:/bin/bash的字样
	那两个0就是uid和gid了.



	
	挂载
	sudo mount -t cifs -o username=gnftp,password=gionee //10.8.0.22/software_release   ~/project/WORK/ftp/

	卸载
	fusermount -u  ~/project/WORK/ftp/ 或 umount  ~/project/WORK/ftp/


	挂载到以前的项目服务器上，这两个都是y3_driver只是路径不同
	sudo sshfs -o allow_other y3_driver@10.8.0.24:/home/y3_driver  /home/llb/project/WORK/ssh            密码：y3_driver 

	sudo sshfs -o allow_other y3_driver@10.8.0.24:/home/y3_driver/extend_data/guojianqiu    /home/llb/project/WORK/ssh_extend
	(03:14:56 PM) guojianqiu:

	通过ssh挂载服务器 ssh y3_driver@10.8.0.24

	sudo mount -t cifs -o username=Administrator,password=123456,dir_mode=0777,vers=2.1 //10.8.8.108/d  /home/llb/project/WORK/win_service/

8.vim配置的快捷方式
	按F5可以直接编译并执行C、C++、java代码以及执行shell脚本，按“F8”可进行C、C++代码的调试

	自动插入文件头 ，新建C、C++源文件时自动插入表头：包括文件名、作者、联系方式、建立时间等，读者可根据需求自行更改

	映射“Ctrl + A”为全选并复制快捷键，方便复制代码

	按“F2”可以直接消除代码中的空行

	“F3”可列出当前目录文件，打开树状文件目录

	支持鼠标选择、方向键移动

	代码高亮，自动缩进，显示行号，显示状态行

	按“Ctrl + P”可自动补全

	[]、{}、()、""、' '等都自动补全

	其他功能读者可以研究以下文件
	


9.gdb命令
产生可以调试的文件
gcc -g  test.c -o test

l 列出源代码

p 打印变量

start 开始运行

next 单步运行

b 设置断点

bt 显示函数堆栈
当然在这之前要step进入才能查看函数堆栈，不然直接运行过去了

f 进入哪个函数堆栈


10.usb抓包
都要管理员权限：
mount -t debugfs none /sys/kernel/debug

modprobe usbmon

查看当前的USB device：
cat  /sys/kernel/debug/usb/devices

要想开始监听 全部 Bus 上的全部USB数据包：
cat  /sys/kernel/debug/usb/usbmon/0u