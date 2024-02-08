### Part 1: Warm-up Exercises
* Exercise 2.18
     
     In the second loop, which is a parallel loop, it writes to the same elements of the array 'a' without any coordination. This lack of synchronization can lead to a conflict condition where multiple threads access and modify the same memory locations simultaneously. As a result, the final values of the elements in array 'a' can be incorrect.


* Exercise 2.19

    False sharing occurs when multiple threads concurrently access different variables that are located within the same cache line. With a chunksize of 1, adjacent elements of array 'a' are likely to be stored in the same cache line, leading to unnecessary cache line invalidations and transfers between CPU caches as each thread modifies its respective element. This can significantly degrade performance. To mitigate false sharing and improve performance, it is recommended to use a larger chunksize that ensures each thread operates on a distinct portion of the data, minimizing cache line contention. The optimal chunk size depends on the architecture and specific characteristics of the hardware.



* Exercise 2.21
    
    There is still a problem left with this code: the boundary conditions from the original, global, version have not been taken into account. Give code that solves that problem.
    ```
    if (myTaskID==0 && i == 0)
        a[i] = (b[i]+bright)/2
    else if (myTaskID==nTasks-1 && i == nTasks)
    a[i] = (b[i]+bleft)/2 
    ```
    For the case of myTaskID==0, only two values are averaged, as there is no value left of the first element. For the case of myTaskID==nTasks-1, again only two values are averaged, as there is no value right of the last element.

* Exercise 2.22

    Take another look at equation (2.5) and give pseudocode that solves the problem using non-blocking sends and receives. What is the disadvantage of this code over a blocking solution?

    A non-blocking coded up solution is:

    ```
    MPI_Comm_rank(MPI_COMM_WORLD,&myTaskID);
    MPI_Comm_size(MPI_COMM_WORLD,&nTasks);
        
    if (myTaskID==0) leftproc = nTasks-1;
    else leftproc = myTaskID-1;
        
    if (myTaskID==nTasks-1)
    MPI_Isend(&x[i], 1, MPI_DOUBLE, 0, myTaskID, MPI_COMM_WORLD, &handle_send);
    else
    MPI_Isend(&x[i], 1, MPI_DOUBLE, myTaskID+1, myTaskID, MPI_COMM_WORLD, &handle_send);

    if (myTaskID==0)
    MPI_Irecv(&x_rec, 1, MPI_DOUBLE, nTasks-1, nTasks-1, MPI_COMM_WORLD, &handle_receive);
    else
    MPI_Irecv(&x_rec, 1, MPI_DOUBLE, myTaskID-1, myTaskID-1, MPI_COMM_WORLD, &handle_receive);

    MPI_Wait(&handle_receive);
        
    y[myTaskID] = y[myTaskID] + x_rec;
    ```

    The above code sends and receives when ready, and waits to do the summation until the receive is complete. It also handles the irregularities at index 0 in the array. Although this code will likely perform better than its blocked solution, it is at the expense of complicating the buffer semantics.



* Exercise 2.23
    
    To address this question effectively, we can analyze the bandwidth and latency aspects independently.

    In a purely distributed model, every node communicates directly with every other node. For example, if there are n nodes, each node needs to send messages to n-1 other nodes, resulting in a total of n*(n-1) messages. In a hybrid model, there's a centralized server. Each node now only needs to send messages to the server. This reduces the number of messages sent. Resulting in n messages in total. (since there's only one message at a time on the bandwidth). Hence, the hybrid model offers significant bandwidth savings compared to the purely distributed model, as it diminishes the volume of messages transmitted across the constrained bandwidth.

    In a purely distributed model, messages must traverse the network directly between nodes, potentially resulting in increased latency as each message independently traverses the network. Conversely, in a hybrid model, messages are routed through a centralized server. While this may introduce additional latency due to the intermediary step, it has the potential to be lower than the latency experienced in the purely distributed model, especially if the centralized server is strategically positioned and the network infrastructure is optimized.




* Exercise 2.27

    Border Case 1: Computation Takes Zero Time, Only Communication:

    All resources are dedicated to communication tasks without any time spent on computation.The gain from overlapping computation and communication would be maximal since there is no overlap. Any overlap in this case would reduce the time spent on communication without impacting computation time.

    Border Case 2: Only Computation, No Communication:

    All resources are dedicated to computation, and there is no time spent on communication. Overlapping computation and communication in this case is irrelevant since there is no communication to overlap with computation.

    General Case 3: Both computation and communication occur simultaneously:

    There is some overlap between them. The gain from overlapping computation and communication in this case depends on several factors:
    The extent to which computation and communication can be overlapped without affecting the overall performance.
    The nature of the tasks being performed and their dependencies. Some tasks may require data from communication before computation can proceed, while others may not. The efficiency of the system in managing and coordinating overlapping tasks. By optimizing the overlap between computation and communication, it's possible to reduce the overall execution time of a task or process.