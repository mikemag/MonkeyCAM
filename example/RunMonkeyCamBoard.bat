
rem create directory to hold outputs
set OutputDir=Outputs
mkdir %OutputDir%

rem call MonkeyCAM executable with relevant definition files
..\cli\MonkeyCAM.exe --board board-def.json --machine machine-tool-def.json  --outdir %OutputDir%
pause

rem display overview html
%OutputDir%\Sample-snowboard-overview.html
