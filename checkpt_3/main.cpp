#include "mbed.h"
#include "mbed_rpc.h"

// /Rled_blink/run 20
// /Bled_blink/run 10

RpcDigitalOut myled1(LED1,"myled1"); // red
RpcDigitalOut myled3(LED3,"myled3"); //blue
Serial pc(USBTX, USBRX);
void Rled_blink(Arguments *in, Reply *out);
RPCFunction rLED(&Rled_blink, "Rled_blink");
void Bled_blink(Arguments *in, Reply *out);
RPCFunction bLED(&Bled_blink, "Bled_blink");

int main() {
    //The mbed RPC classes are now wrapped to create an RPC enabled version - see RpcClasses.h so don't add to base class

    // receive commands, and send back the responses
    myled1.write(1);
    myled3.write(1);
    char buf[256], outbuf[256];
    while(1) {
        memset(buf, 0, 256);
        for (int i = 0; ; i++) {
            char recv = pc.getc();
            if (recv == '\r') {
                pc.printf("\r\n");
                break;
            }
            buf[i] = pc.putc(recv);
        }
        //Call the static call method on the RPC class
        RPC::call(buf, outbuf);
        pc.printf("%s\r\n", outbuf);
    }
}

void Rled_blink (Arguments *in, Reply *out) {
    int i = in->getArg<int>();
    // Have code here to call another RPC function to wake up specific led or close it.
    char outbuf[256];

    while (i--) {
        RPC::call("/myled1/write 0", outbuf);
        wait(0.1);
        RPC::call("/myled1/write 1", outbuf);
        wait(0.1);
    }
}

void Bled_blink (Arguments *in, Reply *out) {
    int i = in->getArg<int>();
    // Have code here to call another RPC function to wake up specific led or close it.
    char outbuf[256];
    while (i--) {
        RPC::call("/myled3/write 0", outbuf);
        wait(0.1);
        RPC::call("/myled3/write 1", outbuf);
        wait(0.1);
    }
}
