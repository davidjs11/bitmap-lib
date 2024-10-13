all:
	@gcc -o main main.c bitmap.c -I.

clean:
	@rm main
