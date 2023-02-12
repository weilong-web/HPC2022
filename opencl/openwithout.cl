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
    int wholeCol = cols + 2;
    double tmpVal = nowMatrix[row * wholeCol + col];

    if(0 < row && row <= rows && col > 0 && col <= cols){
        
        double surrounding = (nowMatrix[row * wholeCol + col - 1] + 
                             nowMatrix[row * wholeCol + col + 1] +
                             nowMatrix[(row - 1) * wholeCol  + col] + 
                             nowMatrix[(row + 1) * wholeCol  + col]) / 4 - tmpVal;

        double nextValue = tmpVal + constanw * surrounding;
        
        nextMatrix[row * wholeCol + col] = nextValue;
        
    }
}


