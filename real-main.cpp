// Here's a wirish include:
#include <usart.h> // libmaple access
#include <wirish.h>
#include "HardwareSerialFlowControl.h"

/*
    the FT232R supports all standard baud rate and non-standard baud rates from 183 to 3Mb/s
    rate = 3000000/(n+x)
    n is an integer between 2 and 16384 (2^14)
    x can be a sub-integer 0,0.125,0.25,0.375, 0.5, 0.625, 0.75, or 0.875
*/

// Serial3.begin(BAUDRATE);
// try faster 9600, 19200, 14400, 38400, 57600, 56000, 115200, 128000,
// 230400,460800, 153600, 256000,921600
/*
Hardware connections
--------------------
From the the maple documentaiton the USART Pin Map
         Tx     Rx      Ck      cts     rts
Serial3	D29	D30	D31	D32	D33
(this also applies to the Maple RET6 edition)
These pins are also listed as being ***5V tolerant***

--------------- for the maple-ret6 edition -----------------
Pin, GPIO, ADC,  Timer,     I2C,     UART/USART, SPI,    5 V?
D32, PB13, -,    1_CH1N,    -,       3_CTS,      2_SCK,  Yes
D33, PB14, -,    1_CH2N,    -,       3_RTS,      2_MISO, Yes

--------------------- for the maple ------------------------
D32, PB13, -,    -,         -,       3_CTS,      2_SCK,  Yes
D33, PB14, -,    -,         -,       3_RTS,      2_MISO, Yes

The FT 2232H pin map
--------------------
pin 16  ADBUS0   TXD
pin 17  ADBUS1   RXD
pin 18  ADBUS2   RTS
pin 19  ADBUS3   CTS
pin 20  ADBUS4   DTR

*/


// works at 2000000
#define BAUDRATE 2000000
// #define BAUDRATE 2250000 // doesn't work with ft323R (basic) not an allowed rate
#define NLOOPS 2 // check serial every NLOOPS count ???

int gState = 0;
int gSeconds =0;
int g_loop_cnt=0;

HardwareSerialFlowControl Serial3fc(USART3, BOARD_USART3_TX_PIN, BOARD_USART3_RX_PIN,
				    BOARD_USART3_CTS_PIN, // D32 cts
				    BOARD_USART3_RTS_PIN); //D33 rts

void serial_term(void)
{
  if (! (g_loop_cnt %NLOOPS) ) {
    if (Serial3fc.available()) {
      uint8 input = Serial3fc.read();
      switch(input) {
      case '\r':
	break;

      case ' ':
	//      SerialUSB.println("spacebar, nice!");
	break;

      case '?':
      case 'h':
	// cmd_print_help();
	break;

      case 'u':
	// SerialUSB.println("Hello World!");
	break;

      case 'w':
	Serial3fc.println("Hello!");
	break;

      case 'm':
	// cmd_serial1_serial3();
	break;

      case 'E':
	//cmd_serial1_echo();
	break;

      case '.':
	break;

      case 'd':
	// Dumping from 0 to 1023 int16 data to Serial3 LSB first 
	for(int16 ii =0; ii < 1024; ii++) {
	  Serial3fc.write((char)ii);  
	  Serial3fc.write((char) (ii >> 8));
	}


	// SerialUSB.println("Disabling USB.  Press BUT to re-enable.");
	//  SerialUSB.end();
	// pinMode(BOARD_BUTTON_PIN, INPUT);
	//  while (!isButtonPressed())
	//      ;
	//  SerialUSB.begin();
	break;

      case 'n':
	//cmd_adc_stats();
	break;

      case 'N':
	// cmd_stressful_adc_stats();
	break;

      case 'e':
	// cmd_everything();
	break;

      case 'W':

	break;

      case 'U':

	// Dumping data to Serial3fc. 
	Serial3fc.flush();
	for(int ii =0; ii < 256; ii++) {
	  Serial3fc.write((unsigned char)ii); // is this buffered?
	}

	  
	break;

      case 'g':
	// cmd_sequential_gpio_toggling();
	break;

      case 'G':
	// cmd_gpio_toggling();
	break;

      case 'l': // toggle the state (light blinking)
	// gState ^= gState;
	toggleLED();
	break;
	
      case 'j':
	// cmd_sequential_debug_gpio_toggling();
	break;

      case 'J':
	// cmd_debug_gpio_toggling();
	break;

      case 'B':
	// cmd_but_test();
	break;

      case 'f':
	Serial3fc.println("Wiggling D4 as quickly as possible in bursts. "
			"Press any key to stop.");
	pinMode(4, OUTPUT);
	{
	  int toggle=0;
	  while (!Serial3fc.available()) {
	    // fast_gpio(4);
	    // delay(1);
	    digitalWrite(BOARD_LED_PIN, toggle);
	    toggle ^= 1;
	  }
	}
	break;

      case 'p':
	// cmd_sequential_pwm_test();
	break;

      case '_':
	// Serial3fc.println("Delaying for 5 seconds...");
	// delay(5000);
	break;

        // Be sure to update cmd_print_help() if you implement these:

      case 't':               // TODO

	break;

      case 'T':               // TODO

	break;

      case 's':

	break;

        // Be sure to update cmd_print_help() if you implement these:

      case 'i':               // TODO

	break;

      case 'I':               // TODO
	Serial3fc.println("Unimplemented.");
	break;

      case 'r':
	// cmd_gpio_monitoring();
	break;

      case 'a':
	// cmd_sequential_adc_reads();
	break;

      case 'b':
	// cmd_board_info();
	break;

      case '+':
	// cmd_gpio_qa();
	break;

      default: // -------------------------------
	// Serial3fc.print("Unexpected byte: 0x");
	//  Serial3fc.print((int)input, HEX);
	//  Serial3fc.println(", press h for help.");
	;
      }

      // Serial3fc.print("> ");


    }

  }

}

void setup()
{
    /* Set up the LED to blink  */
    pinMode(BOARD_LED_PIN, OUTPUT);

    SerialUSB.end(); // turn off SerialUSB because it generates a lot of interrupts in libmaple 
    /* Set up Serial3 flow control */
    
    // temporary approach should move to 
    // turn on RTS/CTS
    // usart_dev *usart3fc = Serial3fc.c_dev();
    // usart3fc->regs->CR3 |= (1 << USART_CR3_RTSE_BIT)| (1 << USART_CR3_CTSE_BIT);

    // 
    /* per exmaple: CTS and Rx and configured as input floating
    // Configure USART2 RTS and USART2 Tx as alternate function push-pull 
    GPIO_InitStructure.GPIO_Pin = GPIO_RTSPin | GPIO_TxPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOx, &GPIO_InitStructure);

    // Configure USART2 CTS and USART2 Rx as input floating 
    GPIO_InitStructure.GPIO_Pin = GPIO_CTSPin | GPIO_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
    }
    */

    /* per example: RTS and Tx are configured as alternate function push-pull */
    //    gpio_set_mode(usart3, //this->
    //		  /*gpio_dev* */ rts,
    //		  /*uint8*/ cts_bit,
    //		  // GPI_INPUT_FLOATING or  GPIO_AF_OUTPUT_PP or?
		  
    // see  _dev, rx, GPIO_INPUT_FLOATING);


    // gpio_set_mode(tx_dev, tx, GPIO_AF_OUTPUT_PP);

    Serial3fc.begin(BAUDRATE);
    
}



void loop() {
  serial_term();

  gSeconds++;
  g_loop_cnt++;
}








// Standard libmaple init() and main.
//
// The init() part makes sure your board gets set up correctly. It's
// best to leave that alone unless you know what you're doing. main()
// is the usual "call setup(), then loop() forever", but of course can
// be whatever you want.

__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();

    while (true) {
        loop();
    }

    return 0;
}
