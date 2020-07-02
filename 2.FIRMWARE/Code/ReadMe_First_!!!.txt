/*
 * ReadMe_First_!!!.txt
 *
 *  Created on:
 *      Author:
 */


This is instruction for config, build, program ... etc for project STM32F....


1. 	Auto generating code from STM32CubeMX file config 
	+	If want to change peripheral configuration, open STM32CubeMX file (*.ioc), change peripheral config needed, 
		save it to generate code.
		
2. How to change code between bootloader and application
		
	+ 	open file "Generated_Code/Core/Startup/startup_stm32f091rctx.s", go to line 90
		
		"	/* Call the application's entry point.*/
 		 	bl main
 		"
 		Change "main" to other function if need (Ex: BringUp_Main, Application_Main, BootLoader_Main, ... )
 		
 	+	Apply "Exclude resource from build" for all of other file if not use. 
 		Ex: if build boolader "Application_Main.c" must be "Exclude resource from build"
 			if build application "BootLoader_Main.c" must be "Exclude resource from build"
 			
 	+   Right click to project -> "Properties" -> "C/C++ Build" -> "Settings" -> Tab "Tool settings"
		-> "MCU GCC Linker" -> "General" -> "Linker scrip -T" apply linker for application or bootloader.
		Linker scrip is locate on "Project_Linker" folder.
