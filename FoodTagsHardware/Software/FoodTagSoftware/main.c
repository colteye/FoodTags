#include <food_tag_software.h>
#include <RF430_example.h> //Code to help interact with RF430 using I2C.
#include <stdint.h>
#include "msp430.h"

// First address of information memory for MSP430G2553.
#define DATA_START_ADDR 0x1000

// Power of 2 Modulus Operator for indexing.
#define MODULO(x, m) ( x & (m - 1) )

// Verify that a sample is valid.
#define VALID_SAMPLE(t_i_ptr) ( (t_i_ptr->temp <= 1024) && (t_i_ptr->illum <= 1024) )

// Flash sectors are 64 bytes.
#define FLASH_SECTOR_SIZE 64

// Reserved flash size for 24 measurements.
#define MAX_FLASH_SIZE 192

// Struct for each tag sample.
struct Temp_Illum_Time {
    uint16_t temp;
    uint16_t illum;
    uint16_t timestamp;
};

// Function prototypes.
void Init_MSP430(void);
void Init_RF430(void);
void Collect_Temp_Illum_Sample();
void Create_Temp_Illum_NDEF();
void Write_Flash_Segment(uint8_t *src_data, uint16_t dest_seg_addr);
void Read_Flash_Segment(uint8_t *dest_data, uint16_t src_seg_addr);

// Hardcoded product ID, would ideally be programmed by vendor.
uint32_t product_ID = 0x53535454;

// Init a message buffer to store measurements.
uint8_t NDEF_msg_buf[] = TAG_TEMP_ILLUM_DEFAULT_DATA;

// Check if RF430CL330H has woken up.
uint8_t into_fired = 0;

// Main execution.
void main(void)
{
    // Initialize MSP430.
    Init_MSP430();

    // Initialize RF430CL330H NFC transponder.
    Init_RF430();

    // Wait until READY bit has been set.
    while(!(Read_Register(STATUS_REG) & READY));

    // Enable interrupts for End of Read and End of Write.
    Write_Register(INT_ENABLE_REG, EOW_INT_ENABLE + EOR_INT_ENABLE);

    // Configure INTO pin for active low and enable RF.
    Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE + RF_ENABLE);

    // Clear any pending flags.
    PORT_INTO_IFG &= ~INTO;

    // Enable interrupt.
    PORT_INTO_IE |= INTO;

    while (1)
    {
        // Create temperature and illumination measurement.
        Collect_Temp_Illum_Sample();

        // Create an NDEF message for this sample.
        Create_Temp_Illum_NDEF();

        // Write NDEF memory with Capability Container + NDEF message.
        Write_Continuous(0, NDEF_msg_buf, 180);

        // Waiting for an NFC read or write of/to the RF430.
        __bis_SR_register(LPM3_bits + GIE);
        __no_operation();

        // Device has woken up, check status.
        if(into_fired)
        {
            // Before we read/write to RF430CL330H we should disable RF.
            // Clear control reg to disable RF.
            Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE);

            __delay_cycles(75000);

            // ACK the flags to clear.
            Write_Register(INT_FLAG_REG, EOW_INT_FLAG + EOR_INT_FLAG);
            Write_Register(INT_ENABLE_REG, 0);

            // We have serviced INTO.
            into_fired = 0;

            // Need to write the message again for stability.
            Write_Continuous(0, NDEF_msg_buf, 180);

            // Enable interrupts for End of Read and End of Write.
             Write_Register(INT_ENABLE_REG, EOW_INT_ENABLE + EOR_INT_ENABLE);

            // Configure INTO pin for active low and re-enable RF.
            Write_Register(CONTROL_REG, INT_ENABLE + INTO_DRIVE + RF_ENABLE);

            // Re-enable INTO.
            PORT_INTO_IFG &= ~INTO;
            PORT_INTO_IE |= INTO;

            __no_operation();
        }
    }
}

// Initialize MSP430 with 1 MHz Clock.
void Init_MSP430(void)
{
    // Turn off Watch Dog.
    WDTCTL = WDTPW | WDTHOLD;

    // If calibration constant erased trap CPU.
    while (CALBC1_1MHZ==0xFF);

    // Select lowest DCOx and MODx settings
    DCOCTL = 0;

    // Set range.
    BCSCTL1 = CALBC1_1MHZ;

    // SMCLK = MCLK = DCO = 1 MHz   ACLK = LF oscillator.
    DCOCTL = CALDCO_1MHZ;

    // MCLK/64 for Flash Timing Generator
    FCTL2 = FWKEY + FSSEL0 + FN3;
}

// Begin initialization of RF430CL330H.
// Afterwards a delay is necessary to ensure that it has begun.
void Init_RF430(void)
{
    // Configure pins for I2C.
    // Selecting I2C pin function.
    PORT_I2C_SEL0 |= (SCL + SDA);
    PORT_I2C_SEL1 |= (SCL + SDA);

    //configure USCI for I2C.
    UCB0CTL1 |= UCSWRST;                                    // Software reset enabled.
    UCB0CTL0 |= UCMODE_3  + UCMST + UCSYNC;                 // I2C mode, Master mode, sync.
    UCB0CTL1 |= UCSSEL_3 + UCTR;                            // SMCLK = 8MHz, transmitter.
    UCB0BR0 = 80;                                           // Baudrate = SMLK/80 = 100kHz.
    UCB0I2CSA  = 0x0028;                                    // Slave address - determined by pins E0, E1, and E2 on the RF430CL330H.
    UCB0CTL1  &= ~UCSWRST;                                  // Software reset released.

    // RST RF430 in case board powered but MSP430 reset.
    PORT_RST_SEL0 &= ~RST;                                  // Setting as GPIO pin functionality.
    PORT_RST_SEL1 &= ~RST;
    PORT_RST_OUT &= ~RST;
    PORT_RST_DIR |= RST;                                    // RF430CL330H device in Reset.
    __delay_cycles(1000);
    PORT_RST_OUT |= RST;                                    // Release the RF430CL330H from Reset.

    //configure pin for INTO interrupt.
    PORT_INTO_SEL0 &= ~INTO;                                // Confirming GPIO pin functionality.
    PORT_INTO_SEL1 &= ~INTO;
    PORT_INTO_DIR &= ~INTO;                                 // Set as Input.
    PORT_INTO_OUT |= INTO;                                  // Set output register high.
    PORT_INTO_REN |= INTO;                                  // Internal pull-up resistor.
    PORT_INTO_IFG &= ~INTO;                                 // Clear interrupt flag.
    PORT_INTO_IES |= INTO;                                  // Set interrupt trigger as high-to-low transition, INTO active low.

    __delay_cycles(2000000);                                // Leave time for RF430CL33H to initialize; 20ms or greater.
}

// Measure temperature and illumination sensors using ADC.
// sensor_data should be a uint16_t array of size 2.
void Collect_Temp_Illum_Sample()
{
    // Configure ADC to collect sensor data.
    ADC10CTL1 = INCH_1 + CONSEQ_1 + ADC10DIV_7;             // A1/A0, repeat multi channel.
    ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON + ADC10IE;
    ADC10AE0 = 0x03;                                        // P1.0,1 ADC option select.
    ADC10DTC1 = 0x02;                                       // 2 conversions.

    // Wait for ADC to initialize.
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & ADC10BUSY);                          // Wait if ADC10 core is active.

    // Begin sampling.
    uint16_t sensor_data[2] = {0, 0};
    ADC10SA = (uint16_t)sensor_data;                        // Data buffer start.
    ADC10CTL0 |= ENC + ADC10SC;                             // Sampling and conversion start.
    __bis_SR_register(LPM3_bits + GIE);                     // LPM3, ADC10_ISR will force exit.

    // Once sampling is finished disable ADC.
    ADC10CTL0 &= ~ENC;
    ADC10CTL0 &= ~(REFON + ADC10ON);
    ADC10CTL0 = 0;

    // Make sure sensor data is placed correctly!
    struct Temp_Illum_Time sample = { sensor_data[1], sensor_data[0], 0 };

    // Data buffer for reading/writing from flash.
    uint8_t data_buf[FLASH_SECTOR_SIZE];

    // Various indexers.
    uint8_t i, j;
    uint8_t prev_i = 0;
    uint8_t cur_i = 0;
    uint8_t cnt_i = 0;

    // Insert measurement into flash memory.
    // Acts as a pseudo circular queue.
    // e.g 1..2..3..4..5 -> 6..2..3..4..5
    // Infinite loop until return statement is reached (May need to be bound to a reset button?)
    while (1)
    {
        // Iterate over all flash sectors.
        for (i = 0; i < MAX_FLASH_SIZE; i += FLASH_SECTOR_SIZE)
        {
            // Read data into buffer.
            Read_Flash_Segment(data_buf, DATA_START_ADDR + i);

            for (j = 0; j < FLASH_SECTOR_SIZE; j += 8)
            {
                // Each measurement stored as 6 bytes in a 8 byte block.
                struct Temp_Illum_Time *cur_sample = (struct Temp_Illum_Time *)&data_buf[j];
                cur_i = cur_sample->timestamp;

                // If the current sample is invalid, it's address can be inserted into.
                if (!VALID_SAMPLE(cur_sample))
                {
                    // Set appropriate time stamp and write flash segment with new measurement.
                    sample.timestamp = cnt_i;
                    memcpy(cur_sample, &sample, sizeof(struct Temp_Illum_Time));
                    Write_Flash_Segment(data_buf, DATA_START_ADDR + i);
                    return;
                }
                // If all samples are valid, check if the previous time stamp was larger.
                else if (MODULO(prev_i, UINT8_MAX + 1) > cur_i)
                {
                    // If so, we can add the measurement here to enforce the queue.
                    sample.timestamp = MODULO(prev_i + 1, UINT8_MAX + 1);
                    memcpy(cur_sample, &sample, sizeof(struct Temp_Illum_Time));
                    Write_Flash_Segment(data_buf, DATA_START_ADDR + i);
                    return;
                }
                // Set previous to current index.
                prev_i = cur_i;

                // Increment cnt_i with modulus looping (255..256..0..1..2..)
                cnt_i = MODULO(cnt_i + 1, UINT8_MAX + 1);
            }
        }
    }
}

// Create NDEF message from sample flash data and product ID.
void Create_Temp_Illum_NDEF()
{
    // Copy ID into NDEF message buffer.
    memcpy(&NDEF_msg_buf[32], &product_ID, sizeof(uint32_t));

    // Indexers.
    uint8_t i, j;
    uint8_t ndef_i = 36;

    // Data buffer for reading flash.
    uint8_t data_buf[FLASH_SECTOR_SIZE];

    // Iterate over all available flash for sampling.
    for (i = 0; i < MAX_FLASH_SIZE; i += FLASH_SECTOR_SIZE)
    {
        Read_Flash_Segment(data_buf, DATA_START_ADDR + i);

        // Copy flash memory to NDEF message for each sample.
        for (j = 0; j < FLASH_SECTOR_SIZE; j += 8)
        {
            struct Temp_Illum_Time *cur_sample = (struct Temp_Illum_Time *)&data_buf[j];

            if (!VALID_SAMPLE(cur_sample)) return;

            memcpy(&NDEF_msg_buf[ndef_i], cur_sample, 6);
            ndef_i += 6;
        }
    }
}

// Write to information flash with appropriate delays.
void Write_Flash_Segment(uint8_t *src_data, uint16_t dest_seg_addr)
{
    while (FCTL3 & BUSY);                                   // Make sure flash is available

    uint8_t *Flash_ptr;                                     // Flash pointer
    uint16_t i;

    Flash_ptr = (uint8_t *) dest_seg_addr;                  // Initialize Flash pointer

    FCTL1 = FWKEY + ERASE;                                  // Set Erase bit
    FCTL3 = FWKEY;                                          // Clear Lock bit
    *Flash_ptr = 0;                                         // Dummy write to erase Flash segment

    while(FCTL3 & BUSY);                                    // Make sure flash is available

    FCTL1 = FWKEY + WRT;                                    // Set WRT bit for write operation

    for (i = 0; i < FLASH_SECTOR_SIZE; i++)
    {
        while (FCTL3 & BUSY);                               // Make sure flash is available
        *Flash_ptr++ = src_data[i];                         // Write value to flash
    }

    while(FCTL3 & BUSY);                                    // Make sure flash is available

    FCTL1 = FWKEY;                                          // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                                   // Set LOCK bit
}

// Read from information flash with appropriate delays.
void Read_Flash_Segment(uint8_t *dest_data, uint16_t src_seg_addr)
{
    while (FCTL3 & BUSY);                                   // Make sure flash is available

    uint8_t *Flash_ptr;                                     // Flash pointer
    uint16_t i;

    Flash_ptr = (uint8_t *) src_seg_addr;                   // Initialize Flash pointer

    for (i = 0; i < FLASH_SECTOR_SIZE; i++)
    {
        while (FCTL3 & BUSY);                               // Make sure flash is available
        dest_data[i] = *Flash_ptr++;                        // Read flash byte
    }
}

// ADC10 interrupt service routine.
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
    __bic_SR_register_on_exit(LPM3_bits);                   // Clear LPM3 bits.
}

// PORT2 interrupt service routine.
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
    // Check if RF430CL330H INTO interrupt fired.
    if(PORT_INTO_IFG & INTO)
    {
        into_fired = 1;

        PORT_INTO_IE &= ~INTO;                              // Disable INTO.
        PORT_INTO_IFG &= ~INTO;                             // Clear interrupt flag.

        __bic_SR_register_on_exit(LPM3_bits);               // Wake up to handle INTO.
    }
}
