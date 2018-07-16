

LIB := libhello.so
OUT := test.out

all:$(LIB) $(OUT) 

$(OUT):test.cpp
	g++ $< -o $@ -L . -lhello

$(LIB):hello.c
	gcc $< -o $@ -shared -fPIC

clean:
	rm *.out libhello.so
