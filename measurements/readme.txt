We performed our experiments on the basic mathematical operations (matrix multiplication, matrix summation), Bresenham 3D algorithm from ROS navigation stack, 
and image processing algorithm (Grayscaling, Blue Channel Extraction). We ran each experiment 100 times and measured execution time. 
We ran each of these with a regular operating system load (no other user application was running in the background), and with high load (4K youtube video running in background). 
This was done to simulate context switch, which is common in robotics applications. To perform these measurements, we used Intel Core-i7 8750H CPU with maximum 4.1 GHz frequency 
running Ubuntu 20.04 operating system with a non-real-time kernel and ROS Galactic version.


The graph on the left is high load.
The graph on the right is low load.
