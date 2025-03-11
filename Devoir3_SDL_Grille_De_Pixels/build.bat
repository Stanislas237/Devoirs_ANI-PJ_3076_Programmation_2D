@echo off
setlocal

if "%~1"=="" (
    echo Utilisation: %~nx0 Debug/Release
    exit /b 1
)

:: Définition des chemins
set "VS_BUILD_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

:: Vérification de l'existence de MSBuild.exe
if not exist "%VS_BUILD_PATH%" (
    echo Erreur : MSBuild introuvable. Vérifiez votre installation de Visual Studio.
    exit /b 1
)

:: Compilation
echo Compilation en cours...
"%VS_BUILD_PATH%" Prog2D.sln /p:Configuration=%~1 /p:Platform=x64
if %errorlevel% neq 0 (
    echo Erreur lors de la compilation.
    exit /b %errorlevel%
)

echo Compilation terminee avec succes.
exit /b 0
