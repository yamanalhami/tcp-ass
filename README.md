to compile client.c :
gcc client.c -o cli

to compile server.c :
gcc server.c -o ser

to run code:
run the server first : ./ser 5001
then run the client : ./cli 127.0.0.0 5001 200000 input.txt
