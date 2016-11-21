@echo off

REM Windows script to configure gmailQt and solution file in IDE
REM requirement: Qt 5 should be installed and path to qmake
REM should listed in PATH environment variable
REM *note: usually this script is run from Visual Studio Command Prompt


cd ..\prj
echo "configuring %cd%"
qmake -tp vc

cd ..\examples
for /D %%d in (*) do (
        echo "configuring %%d"
        cd "%%d"
        qmake -tp vc
        cd ..
)

cd ..\bin

echo "opening IDE.."
..\prj\googleQt.sln

pause
