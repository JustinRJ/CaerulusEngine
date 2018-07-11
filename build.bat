echo DA | xcopy /E "%~dp0dependencies\OIS-1-4\bin\%1\%2\OIS.dll"      "%~dp0\bin\%1\%2"

echo DA | xcopy /E "%~dp0engine\Engine\bin\%1\%2\Caerulus_Engine.exe"           "%~dp0\bin\%1\%2"
echo DA | xcopy /E "%~dp0engine\Core\bin\%1\%2\Caerulus_Core.dll"               "%~dp0\bin\%1\%2"
echo DA | xcopy /E "%~dp0engine\Graphics\bin\%1\%2\Caerulus_Graphics.dll"       "%~dp0\bin\%1\%2"
