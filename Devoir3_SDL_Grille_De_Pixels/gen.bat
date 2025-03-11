@echo off
echo Generating project files...
.\External\Bin\Premake5\premake5.exe vs2022
if %errorlevel% neq 0 (
    echo Erreur lors de la génération des fichiers de projet.
    exit /b %errorlevel%
)
echo Generation terminee.