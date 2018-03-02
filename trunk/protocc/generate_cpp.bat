@echo off
mkdir out
del out\\*.*
protoc --cpp_out=./out *.proto

cd out\
for %%s in (*.pb.cc) do (
	for %%r in (%%~ns) do (
		ren %%s %%~nr.cpp
	)	
)

for %%s in (*.pb.h) do (
	for %%r in (%%~ns) do (
		ren %%s %%~nr.h
	)	
)
pause