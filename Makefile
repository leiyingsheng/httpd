
vpath %.c src
vpath %.h src
vpath %_test.c src/test

# tests = utils_test map_test
tests = map_test
objects = utils.o request.o map.o context.o

.PHONY: testAll clean

httpd: httpd.o $(objects)
	gcc -o httpd $^ -static

testAll: $(tests)
	-./$<

$(tests):%:%.c $(objects)
	gcc -o $@ $< $(objects) -lcunit

clean:
	-rm httpd *.o *_test