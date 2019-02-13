echo DA | xcopy /E /Y "%~dp0engine\Core\bin\%1\%2\*.dll"		"%~dp0\bin\%1\%2"
echo DA | xcopy /E /Y "%~dp0engine\Graphics\bin\%1\%2\*.dll"		"%~dp0\bin\%1\%2"
echo DA | xcopy /E /Y "%~dp0engine\Engine\bin\%1\%2\*.exe"		"%~dp0\bin\%1\%2"
echo DA | xcopy /E /Y "%~dp0assets\*"					"%~dp0\bin\%1\%2\assets"
