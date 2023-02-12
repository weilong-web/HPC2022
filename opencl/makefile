BINS = diffusion
CFLAGS = -O2 -lOpenCL

.PHONY : diffusion
diffusion : opencl1.c
	gcc $(CFLAGS) -o $@ $<
.PHONY : clean
clean :
	rm -rf $(BINS)
