if defined DOXYGEN_PATH (
	"%DOXYGEN_PATH%\doxygen.exe" Doxyfile_ja.doxy
	"%DOXYGEN_PATH%\doxygen.exe" Doxyfile_en.doxy
) else (
	doxygen.exe Doxyfile_ja.doxy
	doxygen.exe Doxyfile_en.doxy
)
