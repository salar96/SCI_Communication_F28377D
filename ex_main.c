#include "driverlib.h"
#include "device.h"
#include "sci.h"
void sendString(const char* str)
{
    while (*str) // While the character the pointer is pointing to is not null
    {
        while(!SCI_isSpaceAvailableNonFIFO(SCIA_BASE)); // Wait until space is available
        SCI_writeCharBlockingNonFIFO(SCIA_BASE, *str); // Send the character
        str++; // Move the pointer to the next character
    }
}
void main(void)
{
    // Initialize device clock and peripherals
    Device_init();

    // Initialize GPIO and configure GPIO pins for UART
    Device_initGPIO();
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA); // Configuration for SCI TX
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA); // Configuration for SCI RX

    // Initialize and configure GPIO for LED
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED1, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED1, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_LED2, GPIO_PIN_TYPE_STD);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_LED2, GPIO_DIR_MODE_OUT);
    // Configure UART settings
    uint32_t config = SCI_CONFIG_WLEN_8 | // 8 bit data
                      SCI_CONFIG_STOP_ONE | // One stop bit
                      SCI_CONFIG_PAR_NONE; // No parity

    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 115200, config);
    SCI_enableModule(SCIA_BASE);
    SCI_enableFIFO(SCIA_BASE);
    SCI_resetChannels(SCIA_BASE); // Clear any previous configuration
    GPIO_writePin(DEVICE_GPIO_PIN_LED1,1);
    GPIO_writePin(DEVICE_GPIO_PIN_LED2,1);
    while(1)
    {
        // Send character 'A' via UART
        while(!SCI_isSpaceAvailableNonFIFO(SCIA_BASE)); // Wait until space is available
        SCI_writeCharBlockingNonFIFO(SCIA_BASE, 'A');
        GPIO_togglePin(DEVICE_GPIO_PIN_LED1);
        // Wait for acknowledgment from PC
        //while(!SCI_isDataAvailableNonFIFO(SCIA_BASE));  // Wait for data
        //char ack = SCI_readCharBlockingNonFIFO(SCIA_BASE);  // Read the acknowledgment
        //GPIO_togglePin(DEVICE_GPIO_PIN_LED2);
        //if (ack == '1')
        //{
          //  GPIO_togglePin(DEVICE_GPIO_PIN_LED2);  // Toggle LED1
        //}

        DEVICE_DELAY_US(500000);  // Delay for 1 second (or adjust as needed)
    }
}

