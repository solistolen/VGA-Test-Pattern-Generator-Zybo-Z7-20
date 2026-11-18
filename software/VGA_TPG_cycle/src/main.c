#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"
#include "sleep.h"

#define GPIO_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define CHANNEL 1

int main() {
    XGpio gpio;
    int status;

    status = XGpio_Initialize(&gpio, GPIO_DEVICE_ID);
    if (status != XST_SUCCESS) {
        xil_printf("GPIO init failed\r\n");
        return XST_FAILURE;
    }

    XGpio_SetDataDirection(&gpio, CHANNEL, 0x0); // output

    while (1) {
        XGpio_DiscreteWrite(&gpio, CHANNEL, 0x0); // 000 red
        sleep(2);

        XGpio_DiscreteWrite(&gpio, CHANNEL, 0x1); // 001 green
        sleep(2);

        XGpio_DiscreteWrite(&gpio, CHANNEL, 0x2); // 010 blue
        sleep(2);

        XGpio_DiscreteWrite(&gpio, CHANNEL, 0x3); // 011 color bars
        sleep(2);

        XGpio_DiscreteWrite(&gpio, CHANNEL, 0x4); // 100 checkerboard
        sleep(2);
    }

    return 0;
}
