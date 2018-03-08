del /f student.db
del /f index.bin
del /f available.bin
gcc In-Memory_Indexing-with-Availability-Lists.c
a.exe --first-fit student.db < input1.txt > output.txt
a.exe --first-fit student.db < input2.txt > output.txt