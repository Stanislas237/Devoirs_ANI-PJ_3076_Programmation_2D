@echo off
setlocal

echo ============================
echo Suppression du dossier Build
echo ============================
if exist "Build" (
    rmdir /s /q "Build"
    echo Dossier Build supprime.
) else (
    echo Aucun dossier Build trouve.
)
echo.

echo ============================================
echo Recherche de fichiers premake5.lua dans l'arborescence...
echo ============================================
:: Parcourt récursivement tous les sous-dossiers à partir du dossier courant
for /R %%F in (premake5.lua) do (
    echo Fichier premake5.lua trouve : %%F
    call :ClearVSFiles "%%~dpF"
)
echo.
echo Nettoyage termine.
exit /b


:ClearVSFiles
:: %1 correspond au dossier dans lequel a été trouve le premake5.lua
set "folder=%~1"
echo --- Nettoyage dans le dossier : %folder%

:: Suppression des fichiers .sln
if exist "%folder%*.sln" (
    del /q "%folder%*.sln"
    echo    Fichiers .sln supprimes.
)

:: Suppression des fichiers .vcxproj
if exist "%folder%*.vcxproj" (
    del /q "%folder%*.vcxproj"
    echo    Fichiers .vcxproj supprimes.
)

:: Suppression des fichiers .vcxproj.filters
if exist "%folder%*.vcxproj.filters" (
    del /q "%folder%*.vcxproj.filters"
    echo    Fichiers .vcxproj.filters supprimes.
)

:: Suppression des fichiers .vcxproj.user
if exist "%folder%*.vcxproj.user" (
    del /q "%folder%*.vcxproj.user"
    echo    Fichiers .vcxproj.user supprimes.
)
echo.
exit /b
