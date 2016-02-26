# Surfing Moth Project
Data capture project for Red Bull High Performance Technology

### About this project
During the summer of 2015, right after finishing my freshman year of college, I worked with Red Bull North America's High Performance Technology team to design and build technology to help improve the performance of Red Bull's athletes. The specific project that I was involved in involved created a system that would log data from an active watercraft on the ocean. To do this, we had to come up with a design that would sturdy, reliable, and completely waterproof. We picked out a selection of sensors that would caputure the desired data from the craft, and chose a computer that we would use to log the data. Most of the concepts and tools that we were going to be using were completely new to myself and the other engineers on the team, but the goal seemed simple enough: get the sensors talking to the computer, save the data to a text file, and analyze the data later. These "simpile" tasks turned out to involve a much greater degree of complexity than we had suspected.

As the only software engineer on the team, my focus was mostly on designing the program that would talk to our sensors and log the data. We originally planned to use an Intel Edison, but after a few weeks of working with one, we decided that a BeagleBone Black would be more suited to our needs. We were using close to ten different sensors, which between them used four different communication protocols (SPI, UART, etc.). Speed and efficiency was critical to the system, and having just finished a class that required me to learn C, I decided that C would be the best choice of language for the main program. I became kind of fixated on using C for the remainder of the project, which I now condsider to have been a mistake. As the program got larger and more complex, I was quickly in over my head with concepts that I was still completely new to, doing lots of bit manipulation, working with pthreads, writing my own SPI interface, etc. I ended up reacheing out to some folks that had worked on a similar datalogging project with an Intel Edison and got some help with the concepts I was struggling with.

In the end, after hundreds of emails and dozens of super late nights, everything got finished and we were able to put our system to the test out on the water. Not everything went as smoothly as we had hoped, but we were still able to learn a lot from the project. I now feel a lot more comfortable writing code for embeddded systems and SOCs, working with standard linux tools, working with hardware, soldering, and reaching out for help when I face problems that are beyond my scope of knowledge.
