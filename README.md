# ESIOT 2022-23: Embedded Tuner

Embedded Tuner is a musical tuner which can be used to tune any kind of acoustic or electric musical instruments. It analyzes the audio signal from the microphone or the input jack, detects the pitch and shows the corresponding note on the display. It can also play any desired note with the buzzer to check the intonation by ear. 

## Features

The following features have been implemented and tested on the board.
- Check pitch detection output on display with a precision-scale tuning cursor.
- Tune acoustic instruments or voice using microphone input.
- Tune electric instruments using jack input.
- Enable the buzzer pressing joystick button and select the note you want to hear using user buttons.   
- Select the desired reference pitch using user buttons (this also changes the buzzer pitch).     

## Authors

Samuele Pozzani, Enea Strambini, Luca Landolfo.

# Requirements

## Hardware requirements

Embedded Tuner requires a Texas Instruments MSP432 microcontroller, which is based on a 32-bit ARM Cortex-M4 CPU. The [Educational Booster Pack](https://www.ti.com/tool/BOOSTXL-EDUMKII) with its additional hardware peripherals is also required.  
If desired, a female jack connector can be connected to the ADC input pin 5.5 and to the ground GND pin.  
Development and testing have been performed on MSP-EXP432P401R Launchpad development kit.  

## Software requirements

Embedded Tuner requires [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB) and [MSP Graphics Library](https://www.ti.com/tool/MSP-GRLIB) to be downloaded and linked to the project. Embedded Tuner code can be built and burned to the board using [Code Composer Studio IDE](https://www.ti.com/tool/CCSTUDIO), more information in the [Getting Started](#getting-started) section.

# Getting Started 

To compile and execute Embedded Tuner on your TI launchpad, follow the steps:

1. Download, install and execute CCStudio.
2. Create a new project into CCStudio by clicking File > New > CCS Project.
3. Select the correct target board and the project name, and click Finish.
4. Locate the project in your file system and clone this repository inside its root folder. 
5. Modify Arm Compiler > Include Options in project properties by adding the DriverLib source folder as follows:
`~\driver\source`
6. Modify Arm Linker > File Search Path in project properties by adding DriverLib and GrLib object files as follows:
`~\driver\source\ti\grlib\lib\ccs\m4f\grlib.a`  
`~\driver\source\ti\devices\msp432p4xx\driverlib\ccs\msp432p4xx_driverlib.lib`
7. Build and burn the project to the board. Enjoy! 

# Project description

The following sections describe the most interesting parts of the implementation.

## Pitch detection 

As described by Anssi Klapuri in his book "Signal Processing Methods for Music Transcription", **pitch** is a perceptual property of sounds that allows their ordering on a frequency-related scale. It defines whether a sound is "higher" or "lower" in a musical sense, and it can be quantified as a frequency (pitch is also called fundamental frequency). To "tune" a musical instrument means that we want that the sound of the instrument matches an exact pitch. For instance, on a guitar we can tune an open string by tightening or loosening that string.

A Pitch Detection Algorithm (PDA) is an algorithm designed to estimate the pitch of a quasi-periodic or oscillating signal. In this project, two different algorithms have been tested:

- [Harmonic Product Spectrum](http://musicweb.ucsd.edu/~trsmyth/analysis/Harmonic_Product_Spectrum.html) (HPS) designed by Noll in 1969, which analyzes the audio signal in the frequency domain, therefore the [ARM CMSIS DSP](https://www.keil.com/pack/doc/CMSIS/DSP/html/index.html) FFT function is used. The implementation is very simple but it proved to have inadequate performance.
- [Yin Algorithm](http://audition.ens.fr/adc/pdf/2002_JASA_YIN.pdf) developed by Cheveigne and Kawahara in 2001, which is much more complex but computes the pitch estimation in the time domain. The C implementation can be found on [GitHub](https://github.com/ashokfernandez/Yin-Pitch-Tracking), thanks to [Ashok Fernandez](https://github.com/ashokfernandez/). A nice explanation of this algorithm is on [YouTube](https://www.youtube.com/watch?v=W585xR3bjLM).   

Yin Algorithm ended up outperforming HPS (in terms of accuracy), therefore it was chosen for the implementation. 

## Pitch and music notation

Completely destroying every artistic and romantic aspects, it is safe to say that the physical cause of music are vibrations of mechanical systems, which are measured in hertz. For historical reasons, in Western music only twelve notes of fixed frequencies are used, they are mathematically related to each other and are defined around the central note A4. In this project, the pitch for the note A4 is called *reference pitch*. 

Jumping over [centuries of music hystory](https://en.wikipedia.org/wiki/Concert_pitch), in 1939 the International Organization for Standardization (ISO) defined the note A4 to be tuned to 440 Hz, now known as **concert pitch**. This pitch is the default reference pitch for the Embedded Tuner. Different modern symphony orchestras around the world are used to tune A4 differently, for instance 443 Hz in Russia or 442 Hz in Europe. For this reason, the Embedded Tuner allows the user to select the desired reference pitch.

The [note-naming convention](https://en.wikipedia.org/wiki/Musical_note) specifies a letter, sharps/flats, and an octave number. Any note is an integer away from middle A (A4): let this distane be denoted by n (or *note number*). If the note is above A4, then n is positive; if it is below A4, the n is negative. The frequency f of the note n is then:

$$ f = 2^{n \over 12} \cdot 440 \textrm{ Hz} $$

Almost every function declared in `tuner.h` is based on this equation.

## ADC and memory management with DMA 

This tuner implementation continuously analyzes an array of 1024 samples at 16 kHz (so that it is possible to detected very high frequencies for the [Nyquist Theorem](https://en.wikipedia.org/wiki/Nyquist%E2%80%93Shannon_sampling_theorem), and these values can be configured in the `peripherals.h` file). The only way this board can process all this information from ADC and also keep all its functionalities is by using the DMA module. DMA is a hardware part that ensures data transfer between different memory regions without wasting CPU cycles. DMA is configured in PingPong mode, so that a memory buffer is processed while another one is being filled with data from ADC. The process is described below: 

ADC keeps sampling data at 16 kHz and stores it in ADC MEM0 region (16 bits), one sample at a time. DMA copies that sample from ADC MEM0 to `data_array1` at index i, which is then incremented. Once the index reaches the threshold of 1024 (samples size), an interrupt is raised which wakes the CPU from LPM0 (sleep mode), sets the `*data_array` pointer to `data_array1` and configures DMA to keep moving data to the second array `data_array2`. The Yin Algorithm is now executed on `data_array1` while new data is sent to `data_array2`. After completing another 1024 ADC iterations, the interrupt is called, `*data_array` pointer is set to `data_array2`, DMA restarts moving data to `data_array1`, and Yin Algorithm is executed on `data_array2`. 

## Buzzer

The buzzer is an electrical audio signaling device which can output a musical note based on "how many times per second it is played". It is possible to create a vibration (therefore a note) at a certain frequency very easily by controlling the buzzer with a system timer.  
In this project the MSP432 Timer_A0 is configured at 48 MHz in PWM mode. A divider of 64 is selected to deal with an actual timer of 48 MHz / 64 = 750 kHz. Then the PWM period register is configured so that when the timer reaches the value stored in that register the buzzer makes a single vibration. To play an A4 at 440 Hz, we want the buzzer to make 440 vibrations every second. In this case, we just need to store the value 750 kHz / 440 Hz = 1704 ticks to produce the desired note.

```C
/* file: src/peripherals/timers.c */
const Timer_A_PWMConfig buzzer_pwmConfig =
{
    TIMER_A_CLOCKSOURCE_SMCLK,          // SMCLK = 48 MHz
    TIMER_A_CLOCKSOURCE_DIVIDER_64,     // SMCLK / 64 = 750 kHz
    0,                                  // Value into period register [clock ticks]
    TIMER_A_CAPTURECOMPARE_REGISTER_4,  // Capture-compare register 4 selected
    TIMER_A_OUTPUTMODE_TOGGLE_SET,      // Compare output mode (set toggle bit)
    500                                 // PWM duty cycle (const)
};

Timer_A_generatePWM(TIMER_A0_BASE, &buzzer_pwmConfig);  // Config Timer_A0 in PWM mode

/* file: src/peripherals/io.c */
TIMER_A0->CCR[0] = SMCLK_FREQUENCY / 64 / 440;   // Set period register to play A4 at 440 Hz
```

## Buttons debouncing with Timer32

When a button on the board is pressed once by the user, it happens that the corresponding interrupt is generated multiple times. This issue is caused by electrical noise which occurs when the switch is on the threshold position between on and off: it is possible that an interrupt is generated the first time, the ISR is executed, then the button input is still oscillating and another interrupt is generated. This bouncing can last about 100ms. 

To avoid this issue a debouncer is needed. It can be a hardware device like a logic counter connected to a timer or even a simple capacitor. Otherwise a software solution may be preferable: in this project, whenever a button is pressed and the corresponding ISR is executed, a countdown with the system Timer32 is started, and it will last 1/4 of the clock cycles per second (therefore 48 Mhz / 4 = 12 Mega ticks = 250 ms). If a second interrupt is generated because of bouncing, the ISR checks whether the timer reached 0, otherwise it just returns.

```C 
/* file: src/peripherals/isr.c */
void PORT3_IRQHandler(void)
{
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);
    
    if (is_debouncing()) 
        return;             // Still bouncing
    
    debounce_button();      // Start timer32
    
    /* Execute ISR */
    /* ... */
}
```

## Project Layout

```
Embedded Tuner
|
├───include             # Project header files
│   └───LcdDriver       # Display driver header files
|
└───src                 # Project C source files                 
    ├───LcdDriver       # Display driver C source files
    └───peripherals     # Board-specific hardware management
```

# Acknowledgments

<a href="https://www.unitn.it/"><img src="./docs/unitn-logo.jpg" width="300px"></a>

## Copyright

MIT Licence or otherwise specified. See [license](./LICENSE.txt) file for details.