#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int process_Rank, size_Of_Cluster;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    if (process_Rank == 0){
        int arr[4][4] = {1, 2, 3, 4, 
                         5, 6, 7, 8, 
                         9, 10, 11, 12,
                         1, 2, 3, 4};
        
        MPI_Datatype packed_block;
        // 2: 一共两行
        // 2: 每个block里, 连续取两个值
        // 4: stride = 4
        MPI_Type_vector(2, 2, 4, MPI_INT, &packed_block);
        MPI_Type_commit(&packed_block);
        for(int i=1; i<4; i++){
            MPI_Send(&arr[i/2 * 2][(i%2) * 2] , 1, packed_block, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        int received[4];
        MPI_Recv(received , 4, MPI_INT, 0 , 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int m=0; m<4; m++){
            printf("%d ", received[m]);
        }
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}

// #include <stdio.h>
// #include <mpi.h>

// int main(int argc, char** argv) {
//     int process_Rank, size_Of_Cluster, message_Item;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
//     MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

//     if(process_Rank == 0){
//         message_Item = 42;
//         for(int i=1; i<4; i++){
//             MPI_Send(&message_Item, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
//         }
//         printf("Message Sent: %d\n", message_Item);
//     }

//     else{
//         MPI_Recv(&message_Item, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         printf("Message Received: %d\n", message_Item);

//     }

//     MPI_Finalize();
//     return 0;
// }