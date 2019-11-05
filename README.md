# PES Project 3 Readme


**Team Members:** 
- Prayag Milan Desai
- Rahul Ramaprasad 


---

**Description of repository contents:**  
>This repository contains twelve folders, each having multiple source and header files. The description of the folders are as follows:  

**Folder board:**  
This folder contains the library and source files related to the KL25Z FRDM board which have pin init functions and configuration functions. 

**Folder CMSIS:**  
This folder contains source and header files which provide interfaces to processor and peripherals, real-time operating systems, and middleware components.

**Folder drivers:**  
This folder contains drivers for clock, gpio, communication protocols, touch sensor, timer, flash,etc.

**Folder utilities:** 
This folder contains source and header file which facilitate the debugging process.

**Folder startup:**
This folder contains source and header file for the boot process of the freedom board.

**Folder source_common:** 
This folder contains source files which are common to all targets and are compiled based on the macro definitions. The application is made
modular by defining multiple source files and are placed in target specific includes folder. The main.c is defined here, which is compiled 
for all targets.

**Folder frdm_includes:** 
This folder contains user defined source and header files which contain functions which are specific only to the FRDM board such as ledControl.

**Folder pc_includes:** 
This folder contains user defined source and header files which contain functions which are specific only to the PC such as ledControl.


**Folder debug_frdm:**
This folder contains the object files, which are created after the compilation of all the source files. This folder also contains the output of the compilation process which is ***'.axf'*** file. The '.axf' file will be executed on the FRDM board.

**Folder debug_pc:**
This folder contains the object files, which are created after the compilation of all the source files. This folder also contains the output of the compilation process which is ***'.exe'*** file. The '.exe' file will be executed on the MCUXpresso terminal.

**Folder common_includes**  
This folder contains source files which are common to both the PC and FRDM board such as Memory Test functions, Logger functions, Malloc functions. These files will be compiled for both the platforms.  

**Folder uCUnit**  
This folder contains functions of uCunit, which is a unit test framework. This unit test framework is used to create and monitor test cases for the program.   
The file `unitTestResults.out` has the output of the unit tests captured in it.

---

**Installation/execution notes for the C code in this repository:**  
All the code in this repository should be executed using the following command:  
`make -r -j3 "target"`  
Where, "target" has four choices
- *frdm_release*: This target builds the output for the KL25Z.The output LEDs will change according to the results of memory tests, but no log output
will be available.  
- *frdm_debug*: This target builds the output for the KL25Z. The output LEDs will change according to the results of memory test, and a log output is displayed.
- *pc_release*: This target builds the output for the PC. The output LEDs will print on console according to the memory tests. 
- *pc-debug*:  This target builds the output for the PC. The output LEDs will print according to the memory tests and a log output is displayed.  

**The PC executable (outputPC.exe) is located in the debug_pc folder, and the ARM executable (outputFRDM.axf) is located in the root of the directory**     

Always run the 'make clean' command before rebuilding other targets.  
Executing the above command results in .exe file which can be executed on the pc target by right-clicking and choosing terminal and .axf file which can be executed on FRDM board by clicking the debug option.  
This project has the following requirements, that are fulfilled using the aforementioned command:  
- ARM-GCC cross-compiler : The 'arm-none-eabi-gcc' command is used to compile the FRDM board target code
- Macro flag : The '-D $(target_macro_definition)' to compile the target specific sections of code 
- No errors and warnings: The -Wall flag enables a set of warnings, and the -Werror flag treats the warnings as errors. Since the output of the compiler shows no error, the code in this repo is without any compiler warnings and errors  


**Issues faced**  
Tried to implement a custom malloc function for the FRDM board, but we could not figure out a way to allocate a block of memory in the board for us to use for our custom function. However, we implemented a custom malloc function on the PC.   
Also, while making the code modular we ran into the issue of multiple definitions of functions and variables and we resolved that by using extern keyword and creating a common header file.  

**References**  
http://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html - the custom malloc functions were taken from this website     
https://mcuoneclipse.com/2018/08/26/tutorial-%CE%BCcunit-a-unit-test-framework-for-microcontrollers/ - the reference and tutorial for the uCUnit testing framework


