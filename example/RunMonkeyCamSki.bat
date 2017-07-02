
rem create directory to hold outputs
set OutputDir=..\Sample-output
mkdir %OutputDir%

rem call MonkeyCAM executable with relevant definition files
..\cli\MonkeyCAM.exe --board ski-def.json --machine machine-tool-def.json --binding binding-def.json  --bindingdist 32.0 --outdir %OutputDir%
pause

rem display overview html
%OutputDir%\Sample-ski-overview.html
