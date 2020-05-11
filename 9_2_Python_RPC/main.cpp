#include "mbed.h"
#include "mbed_rpc.h"

/**
 *  This example program has been updated to use the RPC implementation in the new mbed libraries
 *  This example demonstrates using RPC over serial
 */

//Use the RPC enabled wrapped class  - see RpcClasses.h for more info

// the string behind is just a label, can be different from the name
// Also, if we are not using the remote call, we can use RpcDigitalOut object
// just like how we use DigitalOut object
RpcDigitalOut ASD(LED1,"myled"); // LED_RED
RpcDigitalOut myled2(LED2,"myled2"); // LED_GREEN
RpcDigitalOut myled3(LED3,"myled3"); // LED_BLUE

Serial pc(USBTX, USBRX);

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    char buf[256], outbuf[256];
    ASD.write(1);
    myled2.write(1);
    myled3.write(1);

    while(1) {
        memset(buf, 0, 256); // clear buffer(set the buffer to zero)
        for(int i = 0; ; i++) {
            char recv = pc.getc();
            if (recv == '\r') { // meas that we input 'enter'
                pc.printf("\r\n");
                break;
            }
            buf[i] = pc.putc(recv);
        }
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf); 
        // will do something according to the 'but', and will save it reponse into 'outbf'
        pc.printf("%s\r\n", outbuf);
    }
}
