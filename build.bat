echo DA | xcopy /E "%~dp0engine\Core\bin\%1\%2\*.dll"           "%~dp0\bin\%1\%2"
echo DA | xcopy /E "%~dp0engine\Input\bin\%1\%2\*.dll"          "%~dp0\bin\%1\%2"
echo DA | xcopy /E "%~dp0engine\Graphics\bin\%1\%2\*.dll"       "%~dp0\bin\%1\%2"
echo DA | xcopy /E "%~dp0engine\Engine\bin\%1\%2\*.exe"         "%~dp0\bin\%1\%2"
