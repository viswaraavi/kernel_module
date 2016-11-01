all: benchmark validate

benchmark: benchmark.c
	$(CC) -g -O0 -pthread benchmark.c -o benchmark -lckv
validate: validate.c
	$(CC) -g -O0 -pthread  validate.c -o validate -lckv
clean:
	rm -f benchmark validate
