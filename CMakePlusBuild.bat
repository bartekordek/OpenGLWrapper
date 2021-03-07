@echo off

cls
cd Build-Windows
cmake -DCMAKE_BUILD_TYPE=Debug .. -G "Visual Studio 16 2019"


MSBuild.exe %1 /p:VisualStudioVersion=14.0 /t:Build /nologo /p:Configuration=Debug /p:Platform=x64 /verbosity:m

rem /verbosity
rem You can specify the following verbosity levels:
rem q[uiet], m[inimal], n[ormal] (default), d[etailed], and diag[nostic].