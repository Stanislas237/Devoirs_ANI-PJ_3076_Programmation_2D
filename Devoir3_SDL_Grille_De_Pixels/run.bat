@echo off
setlocal

:: Vérifier qu'un nom d'application a été fourni en argument
if "%~1"=="" (
    echo Utilisation: %~nx0 nom_application
    exit /b 1
)

if "%~2"=="" (
    echo Utilisation: %~nx0 Release/Debug
    exit /b 1
)

:: Définition des variables
set "BUILDTARGET=Build"
set "OUTPUTDIR=%~2-windows-x86_64"
set "APP_NAME=%~1"

:: Construction du chemin vers l'exécutable
set "EXECUTABLE=.\%BUILDTARGET%\bin\%OUTPUTDIR%\%APP_NAME%\%APP_NAME%.exe"

:: Vérification de l'existence de l'exécutable
if not exist "%EXECUTABLE%" (
    echo Erreur : L'executable "%EXECUTABLE%" est introuvable.
    exit /b 1
)

:: Exécution de l'application
echo Execution de l'application...
"%EXECUTABLE%"
if %errorlevel% neq 0 (
    echo Erreur lors de l'execution.
    exit /b %errorlevel%
)

echo Programme termine.
exit /b 0
