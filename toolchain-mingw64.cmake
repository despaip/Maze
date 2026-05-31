set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Путь к папке, куда вы распаковали SFML для Windows
set(SFML_DIR "/home/q/sfml-windows/SFML-2.6.1/lib/cmake/SFML" CACHE PATH "Путь к SFMLConfig.cmake")

# Поиск библиотек и include в этой же папке
set(CMAKE_FIND_ROOT_PATH "/home/q/sfml-windows")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
