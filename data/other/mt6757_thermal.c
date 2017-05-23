

 Thermal_Management_MT6757.pdf
 
 核心目的：
 	1.避免手机的元器件因过热而损坏
 	2.保障人们安全使用
 	
 
    mtk_cooler_shutdown.c 这个文件通过 proc fs发送signal到上层，上层发送广播ACTION_REQUEST_SHUTDOWN
    thermal_management将会迅速关机

    {
        thermal_manager

        linux thermal framework内核驱动模块
            在linux thermal framework和thermal device drivers之间还有一个mtk_thermal_monitor

        mtk_thermal_monitor最主要的功能就是定期记录zone devices和cooling devices的状态，当然还有其他一些功能    
        
        
        主要代码：
            kernel-4.4/include/linux/thermal.h		//定义了zone devices,cooling devices
            kernel-4.4/drivers/thermal/thermal_core.c
            kernel-4.4/drivers/thermal/backward_compatible.c
            
         sys节点		/sys/class/thermal
            
         
         mtk_thermal_monitor代码:
         	kernel-4.4/drivers/misc/mediatek/include/mt-plat/mtk_thermal_monitor.h 
         	kernel-4.4/drivers/misc/mediatek/thermal/mtk_thermal_monitor.c  
         	
         	
         thermal_zone:
         	kernel-4.4/drivers/misc/mediatek/thermal/common/thermal_zones/	
         	
         P15	关于thermal_config中使用的参数意思 NTC table
         mtk_ts_bts.c和mtk_ts_btsmdpa.c	
         kernel-4.4/drivers/misc/mediatek/thermal/common/thermal_zones/mtk_ts_btsmdpa.c
		 kernel-4.4/drivers/misc/mediatek/thermal/common/thermal_zones/mtk_ts_bts.c
		 
		 
		 coolers
		 当static CPU cooler 起作用的时候可以在log中找到set_static_cpu_power_limit这个关键字
		 
		 adaptive thermal management主要目的是想让CPU Tj控制在稳定的范围内，调整AP侧的系统功耗
    	 thermal_management会CPU频率动态调节策略通信选择一个合适的CPU OPP挡位(应该是一个数组存储很多频率对应的功率)
    	 
    	 P23 一些参数的意思
    	 
    	 
    	 温升降电流
    	 kernel-4.4/drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct.c  
    	 kernel-4.4/drivers/misc/mediatek/thermal/common/coolers/mtk_cooler_bcct_v1.c
	
		 可以通过/proc/driver/thermal/clbcct 	节点改变电流
		 
		 1 <bcct00 limit> <bcct01 limit> <bcct02 limit>
		 
		 ABCCT：adaptive battery charging current throttling
		
		 如果触发了这个条件将会在一个范围内调节充电电流，以此来实现目标温度
		 
		 modem侧的温升，通过限制传送时间和减少功率放大器active的时间来降温

    }  	
    
    ipo power off
