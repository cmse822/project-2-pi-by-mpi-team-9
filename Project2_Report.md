### Part 1: Warm-up Exercises
* Exercise 2.18



* Exercise 2.19




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



* Exercise 2.23




* Exercise 2.27