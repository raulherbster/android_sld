#CROSS_COMPILE=~/android_src/prebuilt/linux-x86/toolchain/arm-eabi-4.4.3/bin/arm-eabi- 
#ARCH=arm
obj-m += android_module.o

all:
	make -C /home/raul/android_kernel/goldfish/ M=$(PWD) modules
clean:
	make -C /home/raul/android_kernel/goldfish/ M=$(PWD) clean
