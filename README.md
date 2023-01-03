# Detours Demo
演示怎样使用[detours](https://github.com/microsoft/Detours) hook函数。本项目显示了hook  SendMessge函数。hook后不再发送消息。


# How to run
安装好[cmake](https://cmake.org/download/)（可使用默认安装，选择把cmake自动加入到命令行）
cd build。然后运行以下命令
```
cmake .. -G "Visual Studio 17 2022" -A Win32
```
过程如下。

```
D:\srccode\detoursdemo\build>cmake .. -G "Visual Studio 17 2022" -A Win32
CMake Deprecation Warning at CMakeLists.txt:2 (cmake_minimum_required):
  Compatibility with CMake < 2.8.12 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.19045.
-- The C compiler identification is MSVC 19.33.31630.0
-- The CXX compiler identification is MSVC 19.33.31630.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/bin/Hostx64/x86/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.33.31629/bin/Hostx64/x86/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: D:/srccode/detoursdemo/build

D:\srccode\detoursdemo\build>
```

然后在build目录中打开生成的sln，把Demo项目设置为启动项，编译运行即可。

# 编译自己的detours
本项目的detours从[https://github.com/iherewaitfor/detoursdemo.git](https://github.com/microsoft/Detours) colone下来后.

打开 Detours\vc文件夹下的Detours.sln 进行编译。
本项目选择了x86、releaseMD进行编译。

编译后，使用.h和.lib
## detours.h
Detours\include\detours.h
## detours.lib
Detours\lib.X86\detours.lib

# 主要参考
https://github.com/microsoft/Detours

[detours官方使用文档https://github.com/microsoft/Detours/wiki/Using-Detours](https://github.com/microsoft/Detours/wiki/Using-Detours)

https://blog.csdn.net/yp18792574062/article/details/120939021