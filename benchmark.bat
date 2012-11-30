@echo off

setlocal EnableDelayedExpansion

set INDIR=data\volume
set OUTDIR=test
set RAWVOL=synthetic
set DIMS=64 64 64
set TH=200
set EPS=100

rem mkdir %OUTDIR%

REM for %%A in (1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20) do (
REM raw2df3 64 64 64 data\volume\seg_%%A.raw %OUTDIR%\seg_%%A.df3
REM PrecisionRecall -f %OUTDIR%\seg_%%A.df3 -r %OUTDIR%\seg_1.df3 -m naive
REM )



raw2df3 64 64 64 data\volume\bonsai_lowres.raw %OUTDIR%\bonsai.df3
thseg %OUTDIR%\bonsai.df3 %OUTDIR%\bonsai_seg.df3 205 50

for %%A in (1 2 3 4 5 6 7 8 9 10) do (
set /A e=%EPS% + 2 * %%A
set /A t=%TH% - 5 * %%A
thseg %OUTDIR%\bonsai.df3 %OUTDIR%\bonsai_seg%%A.df3 !t! !e!
PrecisionRecall -f %OUTDIR%\bonsai_seg%%A.df3 -r %OUTDIR%\bonsai_seg.df3 -m naive
)

REM raw2df3 %DIMS% %INDIR%\%RAWVOL%.raw %OUTDIR%\%RAWVOL%.df3
REM thseg %OUTDIR%\%RAWVOL%.df3 %OUTDIR%\%RAWVOL%_seg_0.df3 %TH% %EPS%
REM set /a TH=%TH%+30
REM thseg %OUTDIR%\%RAWVOL%.df3 %OUTDIR%\%RAWVOL%_seg_1.df3 %TH% %EPS%

REM PrecisionRecall -f %OUTDIR%\%RAWVOL%_seg_1.df3 -r %OUTDIR%\%RAWVOL%_seg_0.df3 -m naive

