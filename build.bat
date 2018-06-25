echo DA | xcopy /E "%~dp0dependencies\SDL2-2.0.8\bin" "%~dp0\bin"
echo DA | xcopy /E "%~dp0dependencies\OIS-1-4\bin" "%~dp0\bin"

echo DA | xcopy /E "%~dp0engine\Engine\bin\*.exe" "%~dp0\bin"
echo DA | xcopy /E "%~dp0engine\Core\bin\*dll" "%~dp0\bin"
