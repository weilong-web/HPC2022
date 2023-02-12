BINS = dotprod matmul dotprod_float4 dotprod_reduction
CFLAGS = -O2 -lOpenCL

.PHONY : all
all : $(BINS) 

love : opencl2.c
	gcc $(CFLAGS) -o $@ $<
	./love
	gcc -O2 -lOpenCL -o love; ./love
-O2 -lOpenCL
matmul : matmul.c
	gcc $(CFLAGS) -o $@ $<

dotprod_float4 : dotprod_float4.c
	gcc $(CFLAGS) -o $@ $<

dotprod_reduction : dotprod_reduction.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)