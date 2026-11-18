#include "xparameters.h"
#include "xv_tpg.h"
#include "xvtc.h"
#include "xgpio.h"
#include "xil_printf.h"

XV_tpg tpg;
XVtc vtc;
XGpio sw;

int main() {
    int status;
    XVtc_Config *vtc_config;

    // Variables to track switch states
    u32 sw_data = 0;
    u32 last_sw_data = 99; // Set to a dummy value so it triggers on the very first loop

    xil_printf("Starting Interactive VGA Pipeline...\n\r");

    // =======================================================
    // 1. Initialize GPIO (The Slide Switches)
    // =======================================================
    status = XGpio_Initialize(&sw, XPAR_AXI_GPIO_0_DEVICE_ID);
    if (status != XST_SUCCESS) {
        xil_printf("ERR: GPIO Initialization Failed!\n\r");
        return XST_FAILURE;
    }
    XGpio_SetDataDirection(&sw, 1, 0xFFFFFFFF);

    // =======================================================
    // 2. Initialize VTC (Timing Controller)
    // =======================================================
    vtc_config = XVtc_LookupConfig(XPAR_V_TC_0_DEVICE_ID);
    XVtc_CfgInitialize(&vtc, vtc_config, vtc_config->BaseAddress);
    XVtc_EnableGenerator(&vtc);

    // =======================================================
    // 3. Initialize TPG (Test Pattern Generator)
    // =======================================================
    XV_tpg_Initialize(&tpg, XPAR_V_TPG_0_DEVICE_ID);
    XV_tpg_Set_height(&tpg, 1080);
    XV_tpg_Set_width(&tpg, 1920);
    XV_tpg_Set_colorFormat(&tpg, 0);
    XV_tpg_EnableAutoRestart(&tpg);
    XV_tpg_Start(&tpg);

    xil_printf("System Ready! Flip the Zybo switches to change the pattern.\n\r");

    // =======================================================
    // 4. The Infinite Interactive Loop
    // =======================================================
    while(1) {
        // Read the current state of the 4 switches (Returns a value from 0 to 15)
    	sw_data = XGpio_DiscreteRead(&sw, 1);

        // Only update the TPG if a switch was actually flipped!
        if (sw_data != last_sw_data) {

        	switch(sw_data) {
        	                case 0: // 0000 (All switches DOWN)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_COLOR_BARS);
        	                    xil_printf("Pattern: Standard Color Bars\n\r");
        	                    break;
        	                case 1: // 0001 (ONLY SW0 is UP)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_RAINBOW_COLOR);
        	                    xil_printf("Pattern: Rainbow Color\n\r");
        	                    break;
        	                case 2: // 0010 (ONLY SW1 is UP)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_H_RAMP);
        	                    xil_printf("Pattern: Horizontal Gradient\n\r");
        	                    break;
        	                case 4: // 0100 (ONLY SW2 is UP)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_CROSS_HATCH);
        	                    xil_printf("Pattern: Cross Hatch\n\r");
        	                    break;
        	                case 8: // 1000 (ONLY SW3 is UP)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_CHECKER_BOARD);
        	                    xil_printf("Pattern: Checkerboard\n\r");
        	                    break;
        	                case 3: // 0011 (SW0 and SW1 UP)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_TARTAN_COLOR_BARS);
        	                    xil_printf("Pattern: Tartan Color Bars\n\r");
        	                    break;
        	                case 5: // 0101 (SW0 and SW2 UP)
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_V_RAMP);
        	                    xil_printf("Pattern: Vertical Gradient\n\r");
        	                    break;
        	                default: // Any other random combination
        	                    XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_COLOR_BARS);
        	                    xil_printf("Pattern: Failsafe Color Bars\n\r");
        	                    break;
        	            }

            // Save the current state so we don't trigger this again until you flip a switch
            last_sw_data = sw_data;
        }
    }

    return 0;
}
