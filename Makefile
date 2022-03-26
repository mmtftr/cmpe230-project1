objects = main.o 

matlang2c: $(objects)
	cc -o matlang2c $(objects)

test:
	(cd tests && make test)

.PHONY: test
