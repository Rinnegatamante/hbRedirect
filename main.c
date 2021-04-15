#include <vitasdk.h>
#include <taihen.h>
#include <libk/stdlib.h>
#include <libk/stdio.h>

#define HOOKS_NUM 7
static char part_name[5] = {0};
static char titleid[16];

// Hooks related variables
static SceUID g_hooks[HOOKS_NUM];
static tai_hook_ref_t ref[HOOKS_NUM];
static uint8_t cur_hook = 0;

// Generic hooking function
void hookFunction(uint32_t nid, const void* func){
	g_hooks[cur_hook] = taiHookFunctionImport(&ref[cur_hook], TAI_MAIN_MODULE, TAI_ANY_LIBRARY, nid, func);
	cur_hook++;
}

int sceIoOpen_patched(const char *file, int flags, SceMode mode) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[0], fname, flags, mode);
	}
	return TAI_CONTINUE(int, ref[0], file, flags, mode);
}

int sceIoRemove_patched(const char *file) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[1], fname);
	}
	return TAI_CONTINUE(int, ref[1], file);
}

int sceIoRmdir_patched(const char *file) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[2], fname);
	}
	return TAI_CONTINUE(int, ref[2], file);
}

int sceIoMkdir_patched(const char *file, SceMode mode) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[3], fname, mode);
	}
	return TAI_CONTINUE(int, ref[3], file, mode);
}

int sceIoGetstat_patched(const char *file, SceIoStat *stat) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[4], fname, stat);
	}
	return TAI_CONTINUE(int, ref[4], file, stat);
}

int sceIoChstat_patched(const char *file, SceIoStat *stat, int bits) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[5], file, stat, bits);
	}
	return TAI_CONTINUE(int, ref[5], fname, stat, bits);
}

int sceIoDopen_patched(const char *file) {
	char fname[0x400];
	if (file[1] == 'x') {
		snprintf(fname, 0x400, "%s%s", part_name, &file[3]);
		return TAI_CONTINUE(int, ref[6], file);
	}
	return TAI_CONTINUE(int, ref[6], fname);
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {
	
	// Checking if game wants to use a different partition
	sceAppMgrAppParamGetString(0, 12, titleid , 256);
	
	char fname[256];
	snprintf(fname, 256, "ux0:data/hbRedirect/%s.txt", titleid);
	SceUID fd = sceIoOpen(fname, SCE_O_RDONLY, 0777);
	if (fd >= 0) {
		int l = sceIoRead(fd, part_name, 4);
		if (part_name[l - 1] == ':') l--;
		part_name[l] = 0;
		sceIoClose(fd);
	}
	
	if (part_name[0]) {
		// Hooking needed functions
		hookFunction(0x6C60AC61, sceIoOpen_patched);
		hookFunction(0xE20ED0F3, sceIoRemove_patched);
		hookFunction(0xE9F91EC8, sceIoRmdir_patched);
		hookFunction(0x9670D39F, sceIoMkdir_patched);
		hookFunction(0xBCA5B623, sceIoGetstat_patched);
		hookFunction(0x29482F7F, sceIoChstat_patched);
		hookFunction(0xA9283DD0, sceIoDopen_patched);
	}
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	
	if (part_name[0]) {
		// Freeing hooks
		int i;
		for (i = 0; i < HOOKS_NUM; i++){
			taiHookRelease(g_hooks[i], ref[i]);
		}
	}
	
	return SCE_KERNEL_STOP_SUCCESS;
	
}