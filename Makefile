
vpath %.c src
vpath %.h src
vpath %_test.c src/test

CFLAGS= -g -o0

tests = utils_test map_test
objects = utils.o request.o map.o context.o respone.o httpd.o 

.PHONY: testAll clean

httpd: $(objects)
	gcc $(CFLAGS) $^ -o httpd 

$(objects):%.o:%.c %.h
	gcc $(CFLAGS) -c $< -o $@

testAll: $(tests)
	./$<

$(tests):%:%.c $(objects)
	gcc -o $@ $< $(objects) -lcunit

clean:
	-rm httpd *.o *_test