Email: Andrew_Farrell@student.uml.edu

Degree of Success: 100%

Discussion: I'll be honest, for some reason, this assignment gave me a run for
my money. There was something about combining semaphores, ring buffers, and 
shared memory at the same time which made my head spin. Regardless, I have met 
the project requirements (after a couple of late nights.)

I didn't want to manually test the various buffer sizes to see which ones
had a high or low deadlock rate so I did the next best thing; I spent 2x as long writing a script to do it for me. One of my choices was to make the size of buffer an argument to the consumer and producer program, but then there would need to be additional synchronization and I'd have to change alot of code so it was a no go. The script changes the header file, recompiles the project, and runs the executable. After doing that some amount of times, it looks at the results and plots it. I wanted to make the plot in ASCII but decided to stick to matplotlib because it's what I have experience in.

Logically, and as confirmed by my funny looking graph in `deadlockOverBuffer.png`, the deadlock frequency and buffer size have a linear inverse relationship. That is, as the buffer size increases, the deadlock frequency decreases. Intuitively this makes sense because if there is more room for the producers and consumers to swap donuts, there is a lesser chance one buffer will happen to be empty while another buffer happens to be full with producers and consumers in the right buffers for a deadlock to occur. I'd guess that the 50% deadlock rate with 1 producer and 5 consumers happens around the 40 buffer size area. 

As for changing the number of consumers, there is a similar store. The slope in
the `deadlockOverConsumer.png` shows that as the number of consumers increases, 
so too does the rate of deadlock (assuming a static buffer size of 40.) To 
script in this one, it was probably as hard as the first time around since I
had to switch to making changes in the given shell script. I kept trying to
use bash commands which wasnt working and I'm not used to shell scripting so 
it took a bit. 

The shell script can still be run just fine using a single argument. Using a second
argument will just make it use a different number of consumers (I did this so 
the python script could just pass it a variable)

My submission will certainly have more files than is necessary but the outline says
there must be a *minimum* of four seperate files so. Not the worst thing.