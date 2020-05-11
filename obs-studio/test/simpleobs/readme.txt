windows编译obs-studio 64版本
参考文档 https://github.com/obsproject/obs-studio/wiki/Install-Instructions#windows
1、下载依赖文件: 注意必须忽略此步骤!!!
https://obsproject.com/downloads/dependencies2017.zip
解压到 C:\qtproject\obs-studio\obs-deps
2、下载cmake 
https://fossies.org/windows/misc/cmake-3.17.1-win64-x64.msi/
3、Windows 10 SDK (minimum version of 10.0.18362.0)
https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/
4、安装依赖库
pacman -S mingw64/mingw-w64-x86_64-vulkan
pacman -S msys/libcurl-devel
5、打开cmake-gui
5.1、设置obs源代码、build目录
5.2、设置DepsPath为 C:\qtproject\obs-studio\obs-deps\win64
5.3、设置qt为C:\Qt\Qt5.10.0\5.10.0\msvc2017_64
Qt5Core等设置为C:/Qt/Qt5.10.0/5.10.0/msvc2017_64/lib/cmake/Qt5Core
5.4、搜索86将86全部改成64 (非常重要!!!)
5.5、点击Configure、generate 再open project
6、vs2017
只需要编译libobs
6.1 配置x64: 将win32改成x64
6.2 错误"vs2017 unistd.h:  No such file or directory"
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

6.3 将依赖项 ..\deps\jansson\lib\Debug\jansson_d.lib 改成 ..\x64\Debug\jansson_d.lib
