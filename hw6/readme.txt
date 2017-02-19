I multithreaded the ray tracer by taking the main method for loop that
calculates the color of each pixel and put it into a function. I made the
function take two arguments that represented the start and end column width that
the loop will go through. Thus I split up the image so that each thread
generated the pixel colors for a separate section of the image.

Some issues I ran into were that I needed to output the pixels after all the
threads were joined and not while the threads were executing, since the order of
the print statements affects the layout of the pixels. So I stored the pixel
data in an array and output the array after all the threads were joined. I also
encountered a problem when passing the thread number to the function given to
pthread_create. This is because the last argument is a void*, so I needed to
create new memory with malloc in order to pass the thread number correctly. If I
did not do this and used the address of a local variable instead, the local
variable would go out of scope and the thread would not calculate the pixel
colors for the correct location.

Based on the timing performance, multithreading improves performance very
well. Doubling the number of threads almost halves the runtime of the ray
tracer. This is because generating the color of each pixel is independent of the
other pixels, so the task can be split up easily.
