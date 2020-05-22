# Brainfuck JIT VM
This project started as a result of my boredom, but has become a friendly competition between some folks.

I'd like to say that it's fast, ~~but it does not have a jit (yet).~~ and that's because it is. For the execution of the Mandelbrot BF application, execution time went from 11 seconds to 1.3 seconds. Scroll down if you'd like to see the comparison.

## JIT Optimizations
Of course, multiple basic operations such as `>>>` and `+++` are summed together. This happens at the parsing stage and results in a free speed bonus. The JIT, if enabled, takes these summed operations and compiles them down to x86_64 machine code (only windows 64-bit for now). After a majority of the machine code is built, a peephole optimizer scans for any improvements. For now there is only one pattern that will result in a noticeable speed improvement (7%), but more can be added later.

Lets get onto actually seeing the speed difference.

#### Here's a comparison
<img src="http://i.michaelwflaherty.com/u/lVQyFzWbpY.gif" width="500" height="400"/>
<img src="http://i.michaelwflaherty.com/u/fsnLWmxiAg.gif" width="500" height="400"/>

## License
Public domain. Do what you want.
