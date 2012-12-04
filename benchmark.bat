@echo off

setlocal EnableDelayedExpansion

set INDIR=data\volume
set OUTDIR=_test
set RAWVOL=bonsai_lowres
set DIMS=64 64 64
set TH=200
set EPS=100
rem set METHOD=distmatch
set METHOD=naive


if not exist %INDIR%\%RAWVOL%.raw goto :exit

raw2df3 %DIMS% %INDIR%\%RAWVOL%.raw %OUTDIR%\%RAWVOL%.df3
thseg %OUTDIR%\%RAWVOL%.df3 %OUTDIR%\%RAWVOL%_seg.df3 205 50

for %%A in (1 2 3 4 5 6 7 8 9 10) do (
set /A e=%EPS% + 2 * %%A
set /A t=%TH% - 5 * %%A
thseg %OUTDIR%\%RAWVOL%.df3 %OUTDIR%\%RAWVOL%_seg%%A.df3 !t! !e!
PrecisionRecall -f %OUTDIR%\%RAWVOL%_seg%%A.df3 -r %OUTDIR%\%RAWVOL%_seg.df3 -m %METHOD%
)

:exit