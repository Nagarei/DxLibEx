rd /s /q ..\DxLibEx_Web\en\html
rd /s /q ..\DxLibEx_Web\ja\html
if defined DOXYGEN_PATH (
	"%DOXYGEN_PATH%\doxygen.exe" Doxyfile_ja.doxy
	"%DOXYGEN_PATH%\doxygen.exe" Doxyfile_en.doxy
) else (
	doxygen.exe Doxyfile_ja.doxy
	doxygen.exe Doxyfile_en.doxy
)
