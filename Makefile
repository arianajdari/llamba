lib_multiplication.so : multiplication.o
	g++ -shared -Wl,-soname,python_bindings/out/lib_multiplication.so -o python_bindings/out/lib_multiplication.so python_bindings/out/multiplication.o 

multiplication.o:
	g++ -c -fPIC -Iinclude -Ipython_bindings/helpers python_bindings/bindings/multiplication_binding/multiplication.cpp -o python_bindings/out/multiplication.o



lib_summation.so : summation.o
	g++ -shared -Wl,-soname,python_bindings/out/lib_summation.so -o python_bindings/out/lib_summation.so python_bindings/out/summation.o 

summation.o:
	g++ -c -fPIC -Iinclude -Ipython_bindings/helpers python_bindings/bindings/summation_binding/summation.cpp -o python_bindings/out/summation.o



lib_blue_channel.so : blue_channel.o
	g++ -shared -Wl,-soname,python_bindings/out/lib_blue_channel.so -o python_bindings/out/lib_blue_channel.so python_bindings/out/blue_channel.o 

blue_channel.o:
	g++ -c -fPIC -Iinclude -Ipython_bindings/helpers python_bindings/bindings/blue_channel_binding/blue_channel.cpp -o python_bindings/out/blue_channel.o



lib_grayscale.so : grayscale.o
	g++ -shared -Wl,-soname,python_bindings/out/lib_grayscale.so -o python_bindings/out/lib_grayscale.so python_bindings/out/grayscale.o 

grayscale.o:
	g++ -c -fPIC -Iinclude -Ipython_bindings/helpers python_bindings/bindings/grayscale_binding/grayscale.cpp -o python_bindings/out/grayscale.o



lib_bresenham.so : bresenham.o
	g++ -shared -Wl,-soname,python_bindings/out/lib_bresenham.so -o python_bindings/out/lib_bresenham.so python_bindings/out/bresenham.o 

bresenham.o:
	g++ -c -fPIC -Iinclude -Ipython_bindings/helpers python_bindings/bindings/bresenham_binding/bresenham.cpp -o python_bindings/out/bresenham.o


all: lib_bresenham.so lib_blue_channel.so lib_multiplication.so lib_summation.so lib_grayscale.so