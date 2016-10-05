# How to build Anime Trimmer X on MacOS

## Get SDL frameworks

1. Go to http://www.libsdl.org/
1. `Download SDL2.0` -> `Runtime Binaries` -> `Mac OS X`
1. Download `SDL2-2.0.4.dmg` and double-click to open it
1. Drag and drop `SDL2.framework` into `/Library/Frameworks/`
1. Go to https://www.libsdl.org/projects/SDL_image/
1. Download `SDL2_image-2.0.1.dmg` in the Runtime Libraries and double-click to open it
1. Drag and drop `SDL2_image.framework` into `/Library/Frameworks/`
1. Go to https://www.libsdl.org/projects/SDL_mixer/
1. Download `SDL2_mixer-2.0.1.dmg` in the Runtime Libraries and double-click to open it
1. Drag and drop `SDL2_mixer.framework` into `/Library/Frameworks/`
1. Go to https://www.libsdl.org/projects/SDL_net/
1. Download `SDL2_net-2.0.1.dmg` in the Runtime Libraries and double-click to open it
1. Drag and drop `SDL2_net.framework` into `/Library/Frameworks/`

## Setup XCode with SDL

1. Install and open XCode
1. `XCode` -> `Preferences` -> `Downloads` -> download `Command Line Tools`
 *  and then `/usr/bin/gcc` will be created
1. Create a new XCode project
1. Choose `OS X` -> `Application` -> `Command Line Tool` and press `Next`
1. Choose `C++` for Language and press `Next`
1. Create into any directory
1. Click the project (a blue paper icon in the left side menu)
1. `Build phases` -> `Link Binaly With Libraries`
1. Press `+` , press `Add other` and choose the above‐mentioned SDL frameworks
1. `Build Settings` -> `Search Paths` -> `Header Search Paths`
1. Add paths of the header files below (space-separated)
 * `/Library/Frameworks/SDL2.framework/Headers`
 * `/Library/Frameworks/SDL2_image.framework/Headers`
 * `/Library/Frameworks/SDL2_mixer.framework/Headers`
 * `/Library/Frameworks/SDL2_net.framework/Headers`
1. Set `Always Search User Paths` `Yes` (just in case)

# Get OpenSSL library

1. Go to http://www.openssl.org/
1. `Downloads` -> `openssl-1.0.2j.tar.gz` or later
 * 1.1.x has not supported yet
1. Extract `openssl-1.0.2j.tar.gz`
1. Open the terminal and make sure that you can use a command `perl`
1. `cd` into the `openssl-1.0.2j`
1. `./Configure darwin64-x86_64-cc`
1. `make`
 * and then `libcrypto.a` and `libssl.a` will be created there
1. put the `openssl-1.0.2j` into `/Library/Frameworks` (or any other location)
1. Open the XCode project
1. `Build phases` -> `Link Binaly With Libraries`
1. Press `+` , press `Add other` and choose `libcrypto.a` and `libssl.a`
1. Set `Build Settings` -> `Architectures` -> `Build Active Architecture Only` -> `Debug` `No` (just in case)
1. `Build Settings` -> `Search Paths` -> `Header Search Paths`
1. Add a path of the header files below (space-separated)
 * `/Library/Frameworks/openssl-1.0.2j/include`
1. Be careful not to confuse with `/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.10.sdk/usr/include/openssl/`

# Build the Anime Trimmer X

1. Drag and drop all of `.cpp` files and `.h` files into the left side area of the XCode project
1. Press a triangle button to build and run
 * and then an exec file will be created
1. To run properly, you have to put `file` directory and `save` directory in the same location as this exec file
