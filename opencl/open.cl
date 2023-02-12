__kernel
void
diffuse(
    __global double *nowMatrix,
    __global double *nextMatrix,
    float constanw,
    int rows,
    int cols
    )
{
    int row = get_global_id(0);
    int col = get_global_id(1);
    double tmpVal = nowMatrix[row * cols + col];
    if( row==0  && col==0){
        double surrounding = (0+ 
                             nowMatrix[row * cols + col + 1] +
                             0 + 
                             nowMatrix[(row + 1) * cols  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }
    if(row==0 && col == cols-1){
        double surrounding = (nowMatrix[row * cols + col - 1] + 
                             0 +
                             0 + 
                             nowMatrix[(row + 1) * cols  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }
        if(row==rows-1 && col ==0){
        double surrounding = (0 + 
                             nowMatrix[row * cols + col + 1] +
                             nowMatrix[(row - 1) * cols  + col] + 
                             0) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }

        if(row==rows-1 &&  col == cols-1){
        double surrounding = (nowMatrix[row * cols + col - 1] + 
                             0 +
                             nowMatrix[(row - 1) * cols  + col] + 
                             0) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }
        if(0 < row && row < rows-1 && col == 0){
        double surrounding = (0 + 
                             nowMatrix[row * cols + col + 1] +
                             nowMatrix[(row - 1) * cols  + col] + 
                             nowMatrix[(row + 1) * cols  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }    if(0 < row && row < rows-1 && col == cols-1){
        double surrounding = (nowMatrix[row * cols + col - 1] + 
                             0 +
                             nowMatrix[(row - 1) * cols  + col] + 
                             nowMatrix[(row + 1) * cols  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }    if(row==0 && col > 0 && col < cols-1){
        double surrounding = (nowMatrix[row * cols + col - 1] + 
                             nowMatrix[row * cols + col + 1] +
                             0 + 
                             nowMatrix[(row + 1) * cols  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }    if(row ==rows-1 && col > 0 && col < cols-1){
        double surrounding = (nowMatrix[row * cols + col - 1] + 
                             nowMatrix[row * cols + col + 1] +
                             nowMatrix[(row - 1) * cols  + col] + 
                             0) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }
    





    if(0 < row && row < rows-1 && col > 0 && col < cols-1){
        double surrounding = (nowMatrix[row * cols + col - 1] + 
                             nowMatrix[row * cols + col + 1] +
                             nowMatrix[(row - 1) * cols  + col] + 
                             nowMatrix[(row + 1) * cols  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * cols + col] = nextValue;
        
    }
}


__kernel
void
calAvg(
    __global double *c,
    __local double *scratch,
    __const int sz,
    __global double *result)
{
    int gsz = get_global_size(0);
    int gix = get_global_id(0);
    int lsz = get_local_size(0);
    int lix = get_local_id(0);

    double acc = 0;
    for ( int cix = get_global_id(0); cix < sz; cix += gsz )
      acc += c[cix];

    scratch[lix] = acc;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(int offset = lsz/2; offset > 0; offset /= 2) {
      if ( lix < offset )
       scratch[lix] += scratch[lix+offset];
    barrier(CLK_LOCAL_MEM_FENCE);
    }

    if ( lix == 0 )
      result[get_group_id(0)] = scratch[0];
}
