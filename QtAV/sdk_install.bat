mkdir C:\msys64\mingw64\include\QtAV\
copy /y C:\qtproject\QtAV\lib_win_x86_64\*Qt*AV*.a* C:\msys64\mingw64\lib\
copy /y C:\qtproject\QtAV\lib_win_x86_64\libQtAV5.a C:\msys64\mingw64\lib\libQt5AV.a
copy /y C:\qtproject\QtAV\lib_win_x86_64\libQtAVd5.a C:\msys64\mingw64\lib\libQt5AVd.a
copy /y C:\qtproject\QtAV\tools\install_sdk\mkspecs\features\av.prf C:\msys64\mingw64\share\qt5\mkspecs\features\av.prf
copy /y C:\qtproject\QtAV\tools\install_sdk\mkspecs\modules\qt_lib_av*.pri C:\msys64\mingw64\share\qt5\mkspecs\modules\
copy /y C:\qtproject\QtAV\bin\Qt*AV*.dll C:\msys64\mingw64\bin\
mkdir C:\msys64\mingw64\include\QtAVWidgets\
copy /y C:\qtproject\QtAV\lib_win_x86_64\*Qt*AV*.a* C:\msys64\mingw64\lib\
copy /y C:\qtproject\QtAV\lib_win_x86_64\libQtAVWidgets5.a C:\msys64\mingw64\lib\libQt5AVWidgets.a
copy /y C:\qtproject\QtAV\lib_win_x86_64\libQtAVWidgetsd5.a C:\msys64\mingw64\lib\libQt5AVWidgetsd.a
copy /y C:\qtproject\QtAV\tools\install_sdk\mkspecs\features\avwidgets.prf C:\msys64\mingw64\share\qt5\mkspecs\features\avwidgets.prf
copy /y C:\qtproject\QtAV\tools\install_sdk\mkspecs\modules\qt_lib_avwidgets*.pri C:\msys64\mingw64\share\qt5\mkspecs\modules\
copy /y C:\qtproject\QtAV\bin\Qt*AV*.dll C:\msys64\mingw64\bin\
copy /y C:\qtproject\QtAV\tools\install_sdk\..\..\src\QtAV\*.h C:\msys64\mingw64\include\QtAV\
copy /y C:\qtproject\QtAV\tools\install_sdk\..\..\src\QtAV\QtAV C:\msys64\mingw64\include\QtAV\
copy /y C:\qtproject\QtAV\tools\install_sdk\..\..\widgets\QtAVWidgets\*.h C:\msys64\mingw64\include\QtAVWidgets\
copy /y C:\qtproject\QtAV\tools\install_sdk\..\..\widgets\QtAVWidgets\QtAVWidgets C:\msys64\mingw64\include\QtAVWidgets\
mkdir C:\msys64\mingw64\include\QtAV\5.10.0\QtAV\
xcopy /s /q /y /i C:\qtproject\QtAV\tools\install_sdk\..\..\src\QtAV\private C:\msys64\mingw64\include\QtAV\private
xcopy /s /q /y /i C:\qtproject\QtAV\tools\install_sdk\..\..\src\QtAV\private C:\msys64\mingw64\include\QtAV\5.10.0\QtAV\private
xcopy /s /q /y /i C:\qtproject\QtAV\bin\QtAV C:\msys64\mingw64\share\qt5\qml\QtAV
copy /y C:\qtproject\QtAV\tools\install_sdk\..\..\qml\plugins.qmltypes C:\msys64\mingw64\share\qt5\qml\QtAV\
