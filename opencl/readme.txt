A simple model for heat diffusion in 2-dimensional space, splits it up into n_x times n_x small boxes. In each time step, the temperature h(i,j) of a box with coordinates i and j is updated as

h(i,j) + c * ( (h(i-1,j) + h(i+1,j) + h(i,j-1) + h(i,j+1))/4 - h(i,j) )

where c is a diffusion constant. We consider the case of bounded region of box shape whose boundary has constant temperature 0.
