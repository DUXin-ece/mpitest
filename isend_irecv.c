#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int process_Rank, size_Of_Cluster;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    int partial_result = 0;
    int total_result = 0;

    if (process_Rank == 0){
        int arr[4][4] = {1, 2, 3, 4, 
                         5, 6, 7, 8, 
                         9, 10, 11, 12,
                         1, 2, 3, 4};
        
        // Datatype
        MPI_Datatype packed_block;
        MPI_Type_vector(1, 4, 4, MPI_INT, &packed_block);
        MPI_Type_commit(&packed_block);
        MPI_Request request;
        for(int i=1; i<4; i++){
            MPI_Isend(&arr[i][0] , 1, packed_block, i, 0, MPI_COMM_WORLD, &request);
            // do anything here
            MPI_Wait(&request, MPI_STATUS_IGNORE);
        }
        partial_result = 10;
    }
    else{
        int received[4][1];
        MPI_Request request;
        MPI_Irecv(received , 4, MPI_INT, 0 , 0, MPI_COMM_WORLD,  &request);
        // Do other things here
        printf("Hello sbk\n");
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        for(int m=0; m<4; m++){
            printf("%d ", received[m][0]);
            partial_result += received[m][0];
        }
        printf("\n");
        
    }

    
    MPI_Reduce( &partial_result, 
                &total_result, 
                1, 
                MPI_INT, 
                MPI_SUM, 
                0 /*which node holds the result*/, 
                MPI_COMM_WORLD);

    if(process_Rank == 0)
    {
        printf("The sum of all ranks is %d.\n", total_result);
    }

    MPI_Finalize();

    return 0;
}
