
rem create directory to hold outputs
set OutputDir=..\Sample-output
mkdir %OutputDir%

rem call MonkeyCAM executable with relevant definition files
..\engine\cli\MonkeyCAM.exe --board board-def.json --machine machine-tool-def.json --binding board-binding-def.json --outdir %OutputDir%
pause

rem display overview html
%OutputDir%\Sample-snowboard-overview.html
