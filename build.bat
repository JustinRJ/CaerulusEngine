echo DA | xcopy /E "%~dp0\dependencies\SDL2-2.0.8\bin" "%~dp0\bin"
echo DA | xcopy /E "%~dp0\dependencies\OIS-1-4\bin" "%~dp0\bin"

echo DA | xcopy /E "%~dp0\engine\Engine\bin\*.exe" "%~dp0\bin"
echo DA | xcopy /E "%~dp0\engine\Core\bin\*dll" "%~dp0\bin"
