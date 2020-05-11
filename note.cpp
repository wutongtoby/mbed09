/*
There is something need to do before we use RPC
    1. use mbed add https://gitlab.larc-nthu.net/ee2405_2019/mbed_rpc.git to get the library
    2. Add #include "mbed_rpc.h" into main.cpp

First, we can control the k66f with RPC from the computer host(keyboard or python program)
by string command.

The following code will show that how the RPC is used
*/
///////////////////////////////////////////////////////////////
#include "mbed.h"
#include "mbed_rpc.h" 

RpcDigitalOut myled(LED1,"myled");
/* Here we declare an RpcDititalOut with object name myled, there are something to note
 *  1. We can actually treat RpcDigitalOut as normal DigitalOut if we don't use the RPC call
 *     For example, 'myled.write(0)' can let LED1 light up
 *  2. "myled" can be other, like "myLLLL", it has nothing to do with the object name. 
 *     It is just a label for Rpc control.
 */   

Serial pc(USBTX, USBRX);

int main(void)
{
    char buf[256], outbuf[256];
   
    while (1) {
	momset(buf, 0, 256); 
	// set the input buffer to all zero to cover the last command

	for (int i = 0; ;i++) {
	    char recv = pc.getc();
	    // get message character by character from screen or python
	    if (recv == '\r') {
	    // \r means ENTER
		pr.printf("\r\n"); 
		// if we received an enter, we let the screen have a newline
		break;
	    }
	    buf[i] = pc.putc(recv);
	    // when we type something into the screen, we originally can't see anyting 
	    // on the screen, but k66f can see what we type and print it on the screen
	    // so we can see what we typed
	}

	// RPC::call means this is a static function, 
	// that is, it is shared with all object from class, RPC.
	RPC::call(buf, outbuf);
	// excute the command we store in buf	
	// outbutf may have something after excution, maybe not.
    
	pc.printf("%s\r\n", outbuf);
	// print the result from outbuf
    }
}

void LEDControl(Arguments *in, Reply *out);
// for the function we want to control from outside
// it must have the argument : "Arguments *in, Reply *out"

RPCFunction rpcLED(&LEDControl, "LEDControl");

// we have to note that the funciton is actually call by in main by 
// RPC::call(buf, outbuf); when we key /LEDControl/run 0 1
void LEDControl (Arguments *in, Reply *out)   {
    bool success = true;
    // In this scenario, when using RPC delimit the two arguments with a space.
    x = in->getArg<double>();
    y = in->getArg<double>();
    // Have code here to call another RPC function to wake up specific led or close it.
    char buffer[200], outbuf[256];
    char strings[20];
    int led = x;
    int on = y;
    int n = sprintf(strings, "/myled%d/write %d", led, on); // produce formatted string
    strcpy(buffer, strings);
    RPC::call(buffer, outbuf);
    // excute the command with the type "/myled%d/write %d"

	if (success) {
        out->putData(buffer);
		// this line will put some massage to the outbuf in the main's RPC::call(buf, outbuf)
    } else {
        out->putData("Failed to execute LED control.");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
// Also there is something to be note when we are writing python, the following code is okay!
import serial
import time

serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)

s.write(bytes("\r", 'UTF-8')) // the \r actually enter, which is must needed 
time.sleep(1) // we have to wait some seconds between command
s.write(bytes("/LEDControl/run 1 1\r", 'UTF-8'))

