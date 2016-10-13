# How to build Anime Trimmer X on Windows

## Get SDL frameworks

1. Go to http://www.libsdl.org/
1. Go `Download SDL2.0` -> `Source Code`and download `SDL2-2.0.4.zip`
1. Extract and put it in `C:\SDL2-2.0.4` (or any other location)
1. Go to https://www.libsdl.org/projects/SDL_image/
1. Download `SDL2_image-devel-2.0.1-VC.zip` in the Development Libraries
1. Extract and put it in `C:\SDL2_image-2.0.1` (or any other location)
1. Go to https://www.libsdl.org/projects/SDL_mixer/
1. Download `SDL2_mixer-devel-2.0.1-VC.zip` in the Development Libraries
1. Extract and put it in `C:\SDL2_mixer-2.0.1` (or any other location)
1. Go to https://www.libsdl.org/projects/SDL_net/
1. Download `SDL2_net-devel-2.0.1-VC.zip` in the Development Libraries
1. Extract and put it in `C:\SDL2_net-2.0.1` (or any other location)

## Set up Visual C++

1. Get Visual Studio Community 2015 (not a distributable package)
 * Run the installer exe and choose `default` for `the type of installation`
1. Get Windows SDK for your version of Windows
 * Run the installer exe (check all boxes for now)
1. Open the folder `SDL2-2.0.4` which you put above
1. double-click to open `SDL2-2.0.4\VisualC\SDL.sln`
1. Choose not `Debug` but `Release` in the tool bar (`WIN32` is `WIN32`)
1. Go `Project` -> `Properties` -> `Configuration Properties` -> `C/C++` -> `Code Generation`
1. Set `Runtime Library` not `Multi-threaded DLL (/MD)` but `Multi-threaded (/MT)`
1. Go `File` -> `New` -> `Project` -> `Visual C++` and `Install VisualC++ 2015 Tool for Windows Desktop`
 * to solve an error of missing `CL.exe`
1. `Build` -> `Build Solution`
 * if you can't find this menu, try `Project` -> `Reload Project`
1. check if you've got tageted files in a folder `SDL2-2.0.4\VisualC\Win32\Release`
 * `SDL2.lib`, `SDL2main.lib`, `SDL2.dll`

# Get OpenSSL library

1. Go to http://www.openssl.org/
1. `Downloads` -> `openssl-1.0.2j.tar.gz` or later
 * 1.1.x has not been examined yet
1. Extract `openssl-1.0.2j.tar.gz` (by 7-zip) and put it in `C:\openssl-1.0.2j` (or any other location)
1. Download Active Perl installer and run
 * Choose `Typical` for `Setup Type`
 * Check `Add Perl to the PATH environment variable`
1. Open the command prompt and `cd` into `openssl-1.0.2j`
1. execute `C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin\vcvars.bat`
 * to enable a command `nmake`
1. `perl Configure no-asm --prefix=x86 VC-WIN32`
1. `ms\do_ms.bat`
1. Open a file `ms\ntdll.mak` by a text editor and edit below
 * Change `/MD` into `/MT` at line `CFLAG=`
1. `nmake -f ms\ntdll.mak install`
1. `nmake -f ms\ntdll.mak clean`
1. check if you've got tageted files in a folder `openssl-1.0.2j\x86\bin`
 * `lib\ssleay32.lib`, `lib\libeay32.lib`, `bin\ssleay32.dll`, `bin\libeay32.dll`, `include\openssl\*.h`

# Build the Anime Trimmer X

1. Open the Vislual Studio
1. Go `File` -> `New` -> `Project` and choose `win32 project`
 * `application type: Windows application`
 * `additional options: Empty project`
1. Add source files in the solution explorer
 * put all of `.cpp` files into `Source files`
 * put all of `.h` files into `Header files`
 * put `a_trimmer.rc` into `Source files`
1. Go `Project` -> `Properties` -> `Configuration Properties` -> `C/C++` -> `Code Generation`
1. Set `Runtime Library` not `Multi-threaded DLL (/MD)` but `Multi-threaded (/MT)`
1. Choose not `Debug`, `x64` but `Release`, `x86` in the tool bar
1. Go `Project` -> `Properties` -> `Configuration Properties` -> `VC++ Directories`
1. add paths below in the `Include Directories` (semicolon-separated)
 * `C:\SDL2-2.0.4\include`
 * `C:\openssl-1.0.2j\x86\include`
 * `C:\SDL2_image-2.0.1\include`
 * `C:\SDL2_mixer-2.0.1\include`
 * `C:\SDL2_net-2.0.1\include`
1. add paths below in the `Library Directories` (semicolon-separated)
 * `C:\SDL2-2.0.4\VisualC\Win32\Release`
 * `C:\openssl-1.0.2j\x86\lib`
 * `C:\SDL2_image-2.0.1\lib/x86`
 * `C:\SDL2_mixer-2.0.1\lib/x86`
 * `C:\SDL2_net-2.0.1\lib/x86`
1. `Build` -> `Build Solution`
 * and then an exec file will be created in `Documents\Visual Studio 2015\Projects\project_name\Release`
1. To run properly, you have to put files below in the same location as this exec file
 * `file` folder and `save` folder
 * `SDL2.dll`, `libeay32.dll`, `ssleay32.dll` (which you've built above)
 * `SDL2_image.dll`, `zlib.dll`, `libjpeg-9.dll`, `libpng16-16.dll` (contained in `SDL2_image-2.0.1`)
 * `SDL_mixer.dll`, `libogg-0.dll`, `libvorbis-0.dll`, `libvorbisfile-3.dll` (contained in `SDL2_mixer-2.0.1`)
 * `SDL_net.dll` (contained in `SDL2_net-2.0.1`)
