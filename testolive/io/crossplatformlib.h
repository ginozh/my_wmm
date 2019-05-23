#ifndef CROSSPLATFORMLIB_H
#define CROSSPLATFORMLIB_H

#include <QString>

#ifdef _WIN32
	#include <Windows.h>
	#define LibAddress GetProcAddress
	#define LibClose FreeModule
	#define ModulePtr HMODULE
#elif defined(__linux__) || defined(__APPLE__)
	#include <dlfcn.h>
	#define LibAddress dlsym
	#define LibClose dlclose
	#define ModulePtr void*
#endif

ModulePtr LibLoad(const QString& filename);
QStringList LibFilter();

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
class NSWindow;

CFBundleRef BundleLoad(const QString& filename);
void BundleClose(CFBundleRef bundle);
#endif

#endif // CROSSPLATFORMLIB_H
