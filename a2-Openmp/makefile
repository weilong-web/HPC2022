CFLAGS = -O2 -fopenmp -lm

.PHONY : all
all : cell_distances

cell_distances : cell_distances.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf cell_distances
