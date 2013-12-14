CC = gcc
CFLAGS1 = -g 
CFLAGS2 = -ldl

exploit_1:
	@$(CC) $(CFLAGS1) exploit_1.c -o exploit_1 $(CFLAGS2)

exploit_2:
	@$(CC) $(CFLAGS1) exploit_2.c -o exploit_2 $(CFLAGS2)

all:
	@$(CC) $(CFLAGS1) exploit_1.c -o exploit_1 $(CFLAGS2)
	@$(CC) $(CFLAGS1) exploit_2.c -o exploit_2 $(CFLAGS2)

clean:
	@rm -rf exploit_1
	@rm -rf exploit_2
	@rm -rf badfile