# Garmin Challenge 2021
##### _by Nkateko Senoamadi_

## Introduction

The following document outlines my approach to the Garmin Stellenbosch Challenge 2021.

## The Problem

During a hackathon at Garmin Stellenbosch, you and your teammates are experimenting with sonic sensors for
athletic events like shot-put and javelin. You've managed to put together a system that can transmit a
sound signal at the projectile being thrown, and record the return signal that reflects off it.

Attached are the transmitted signal that is used for all projectiles, and the received signals for a shot-put
and a javelin throw. Can you calculate the speed at which each projectile was thrown?


![ImageOfSetup](./figures/ImageOfSetup.png)

### Given:

* Transmit_1.wav (audio file)

* Shotput_receive_1.wav (audio file)
* Javelin_receive_1.wav (audio file)



## My Approach 
### Determining speed

What we are looking for is the speeds of a shot-put & a javelin given the audio files mentioned above. The solution involves manipulating the Doppler Effect equation in terms of the velocity of the projectile(either shot-put or javelin). The Doppler effect is the change in frequency of a wave in relation to an observer who is moving relative to the wave source.  The general formula is as follows:



![CodeCogsEqn](./figures/doppler_standard_equation.gif)

Where:

​	*![CodeCogsEqn(1)](./figures/CodeCogsEqn.png)

​	*![observer_velocity](./figures/observer_velocity.png)

​	*![speed of sound](./figures/speedofsound.png)

​	*![CodeCogsEqn(2)](./figures/CodeCogsEqn(2).png)

​	*![CodeCogsEqn(3)](./figures/CodeCogsEqn(3).png)
​	

We need to identify the **observer** and the __source__ but in order to that we must separate the problem into two parts path 1 and path 2. Path 1 is the path the sound travels from the sensor to the projectile and path 2 is the path the echoed sound travels from the projectile to the sensor:

#### Path 1:

![path1_illustration](./figures/path1_illustration.png)

The sound waves leave the sensor and hit the projectile. In this case we have a stationery source and an observer moving away from the source. The modified formula is as follows:

![stationery_source observer moving away](./figures/stationery_sourceobservermovingaway.png)

Where:

​	*![FoEqualFo1](./figures/FoEqualFo1.png) 

​	*![velocity of projection](./figures/velocityofprojection.png)

​	*![speed of sound](./figures/speedofsound.png)

​	*![fsourceftransmitted](./figures/fsourceftransmitted.png)

​	*![vsEqzero](./figures/vsEqzero.png)

#### Path 2:

![path2_illustration](./figures/echoed_path.png)

The sound waves echos off the projectile and hits the sensor. In this case we have a stationery observer (the sensor) and source that's moving away from the observer. The modified formula is as follows:

![stationery observer movingobserver](./figures/stationeryobservermovingobserver.png)


Where:

​	*![foEqf_recv](./figures/foEqf_recv.png)

​	*![voEqzero](./figures/voEqzero.png)

​	*![speed of sound](./figures/speedofsound.png)

​	*![f_sEQfo1](./figures/f_sEQfo1.png)

​	*![vsEqVproj](./figures/vsEqVproj.png)



We then substitute and simplify a bit as seen below:

![subf_o1](./figures/subf_o1.gif)

![simp_eq](./figures/simp_eq.gif)

From this point we make the velocity of the projectile the subject of the formula and we get the following:

![V_projFormula](./figures/V_projFormula.gif)

### Processing audio files

Given the derivation it is clear that we must find the the frequency of the transmitted frequency and the received frequency for each projectile. To do this we will use a Discrete Fourier Transform (DFT)  to help us identify the frequencies of the audio files. However, before we can do that we need to process the header of the audio file as seen in the figure below. 

![Screenshot_2021-01-14 C++ Reading the Data part of a WAV file](./figures/Screenshot_2021-01-14C++ReadingtheDatapartofaWAVfile.png)

The data is then processed using C++ paying close attention to the changing endians  (with the help of online resources<sup>[1]</sup> ) and  the data segment of each audio file was stored in a  text file as sample points  separated by newline characters. The output below shows the data extracted from the headers of each audio file.

![image-20210121224114760](./figures/image-20210121224114760.png)

### Getting Frequencies 

The aim is to represent function in the time domain and as functions frequency domain. This is done with the Discrete Fourier Transform (DFT) the data is loaded onto MATLAB and  for the time domain its processed as follows:



![setting up](./figures/settingup.png)

Using the function of the data _x(t)_ (a function of t) we use the Fourier Transform so that i could be represented as _z(f)_ (a function of frequency) .

![fourier](./figures/fourier.png)

Repeating this for all audio the audio files and graphing  _x(t)_  and  _z(f)_  for the Transmit_1 , Shotput_receive_1 and Javelin_receive_1.  

![transmittedgraphs](./figures/transmittedgraphs.png)

The frequency of the transmitted file is found through getting the absolute value of the frequency  with the largest amplitude ( *z(f)* value ).  

We find that the transmitted frequency is 10 000 Hz.

![shotput received graph](./figures/shotput_received_graph.png)

![Javelin recv graph](./figures/Javelin_recv_graph.png)

Similarly, we can find the the frequencies of the received audios for each projectile.

We find the received frequency for the shot-put and javelin is 8132 Hz and 9212 Hz respectively.

Applying the formula (derived earlier) in MATLAB we find the speeds of each projectile **relative to the sensor**. Taking the speed of sound as 343 m/s.

![formula speed](./figures/formulaspeed.png)

Which gives us the following speeds relative to the sensor :

* _Speed of the shot-put = 14.07 m/s_
* _Speed of the javelin= 35.34 m/s_




## Bibliography

1. File reading : http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/

 