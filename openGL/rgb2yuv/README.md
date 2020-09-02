This is a simple testing tool for doing color conversion from bmp (RGB) to NV12.
Internally input surface (taken from bmp file) is treated as RGBx. 
The goal of this project is to explore possibilites of doing very 
fast conversion when surfaces are kept in GPU memory.

syntax: rgb2nv12 width height file.bmp number_of_iterations

where:
       width and height:  dimensions of input file
       file.bmp: name of bmp file used as source (must be RGB type)
       number_of_iterations: how many times conversion shader should be called

example:

       ./rgb2nv12 1920x1080 Desktop.bmp 1000

The tool will call shader 1000 and will display statistics.
  
