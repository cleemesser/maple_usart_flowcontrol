Notes:
- order may matter currently :
  1. set up gpios
  2. in usart_init: setup ring buffer, enable clock and irq
  3. enable register CR1 to set single buffer transmission mode, multibuffer receiver mode, 8n1 parity
  4. enable registers in CR3 to activate rts and cts
  5. set baud rate



testing with olimex maple clone at 2Mb/s mostly maple->PC via FT232RL
---------------------------------------------------------------------
2523961 cycles performed 5169072128 bytes (5169.1 MB) transferred before stopping
test ran for 27756.9 seconds

In [4]: 5169.1/27756.9

KeyboardInterrupt

In [4]: 5169.1/27756.9
Out[4]: 0.18622756864059026

In [5]: 


next test with ret6 again
-------------------------

How Do I Use It? 
================

Look at rules.mk. It's a Makefile fragment that tells libmaple's
Makefile how to build your project's sources. Use it as a template for
starting your own project.

Build this project and link it with libmaple like this:

1. Set the environment variable LIB_MAPLE_HOME to point to where you
   have the libmaple Git repository.

2. Tell make to use libmaple's Makefile, and have it pay attention to
   your rules.mk, like this:

   $ make -f $LIB_MAPLE_HOME/Makefile USER_MODULES='/path/to/this/project/foolib /path/to/this/project'

   The first part of USER_MODULES tells libmaple to use this
   directory's rules.mk. The second part tells it about an extra
   library, foolib, that your project uses, which has its own
   rules.mk.

What Else?
==========

You're using the libmaple Makefile, so you can also use any of the
variables it respects. For instance, to compile for Maple Mini, use
BOARD=maple_mini in the usual way:

$ make -f $LIB_MAPLE_HOME/Makefile BOARD=maple_mini USER_MODULES='/path/to/this/project/foolib /path/to/this/project'

You can also use any of the libmaple Makefile targets. For example, to
"make install":

$ make -f $LIB_MAPLE_HOME/Makefile USER_MODULES='/path/to/this/project/foolib /path/to/this/project

To save typing, there's a shell script, make-helper.sh, that you can
call which just runs the above command line, and passes any additional
arguments along to make (note: it puts the build path in the directory
you call it from). Examples:

$ ./make-helper.sh clean
$ ./make-helper.sh BOARD=maple_mini install

Happy hacking!
