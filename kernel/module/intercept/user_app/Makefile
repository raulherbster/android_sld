all:
	arm-eabi-gcc -lc -ldl -nostdlib -fno-short-enums -Wl,--entry=main,-dynamic-linker=/system/bin/linker,-rpath-link=/home/raul/android_src/prebuilt/ndk/android-ndk-r6/platforms/android-8/arch-arm/usr/lib -I/home/raul/android_src/prebuilt/ndk/android-ndk-r6/platforms/android-8/arch-arm/usr/include -L/home/raul/android_src/prebuilt/ndk/android-ndk-r6/platforms/android-8/arch-arm/usr/lib intercept_seal.c -o intercept_seal.out

clean:
	rm intercept_seal.out
