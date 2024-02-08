#include <iostream>
#include <fstream>
#include <sstream>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &name_len);

    // Construct unique filename for each process using rank
    std::stringstream filename_stream;
    filename_stream << "output_" << rank << ".txt";
    std::string filename = filename_stream.str();

    // Open file for writing
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Write to file
    outfile << "Hello from rank " << rank << " out of " << size
            << " on processor " << processor_name << std::endl;

    // Close file
    outfile.close();

    MPI_Finalize();
    return 0;
}
