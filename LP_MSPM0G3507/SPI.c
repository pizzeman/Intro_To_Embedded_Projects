#include "SPI.h"

static SPI_Regs *spi_sr; // local SPI pointer for SendByte() and ReceiveByte()

void InitializeSPI(SPI_Regs *spi, SPI_Config *spi_config)
{
    uint32_t Mask = 0;

    // Initialize local SPI structure pointer for use with SendByte() and ReceiveByte().
    spi_sr = spi;

    // RSTCLR to SPI1 peripherals
    Mask = SPI_RSTCTL_KEY_UNLOCK_W;         //   bits 31-24 unlock key 0xB1
    Mask |= SPI_RSTCTL_RESETSTKYCLR_CLR;    //   bit 1 is Clear reset sticky bit
    Mask |= SPI_RSTCTL_RESETASSERT_ASSERT;  //   bit 0 is reset gpio port
    spi->GPRCM.RSTCTL = Mask;

    // Enable power to SPI peripherals
    Mask = SPI_PWREN_KEY_UNLOCK_W;          //   bits 31-24 unlock key 0x26
    Mask |= SPI_PWREN_ENABLE_ENABLE;        //   bit 0 is Enable Power
    spi->GPRCM.PWREN = Mask;

    // Configure SPI SCLK, PICO, and POCI IOMUX functions.
    IOMUX->SECCFG.PINCM[spi_config->SCK_index]  = spi_config->SCK_iomuxMode;
    IOMUX->SECCFG.PINCM[spi_config->PICO_index]  = spi_config->PICO_iomuxMode;
    IOMUX->SECCFG.PINCM[spi_config->POCI_index]  = spi_config->POCI_iomuxMode;
    IOMUX->SECCFG.PINCM[spi_config->CS_index]  = spi_config->CS_iomuxMode;

    ClockDelay(POWER_STARTUP_DELAY);

    spi->CLKSEL = SPI_CLKSEL_SYSCLK_SEL_ENABLE; // bit 3 SYSCLK

    spi->CLKDIV = spi_config->divideRatio;

    // bits 2-0 n (0 to 7), divide by n+1
    // Set the bit rate clock divider to generate the serial output clock
    //     outputBitRate = (spiInputClock) / ((1 + SCR) * 2)
    spi->CLKCTL = (CLOCK_FREQUENCY/(spi_config->divideRatio+1))/((1+spi_config->scr)*2);

    // bit 14 CSCLR=0 not cleared
    // bits 13-12 CSSEL=0 CS0
    // bit 9 SPH = 0
    // bit 8 SPO = 0
    // bits 6-5 FRF = 00 (3 wire)
    // bits 4-0 n=7, data size is n+1 (8bit data)
    spi->CTL0 = spi_config->ctl0;

    // bits 29-24 RXTIMEOUT=0
    // bits 23-16 REPEATX=0 disabled
    // bits 15-12 CDMODE=0 manual
    // bit 11 CDENABLE=0 CS3
    // bit 7-5 =0 no parity
    // bit 4=1 MSB first
    // bit 3=0 POD (not used, not peripheral)
    // bit 2=1 CP controller mode
    // bit 1=0 LBM disable loop back
    // bit 0=1 enable SPI
    spi->CTL1 = spi_config->ctl1;

    // Rx event as soon as 1 byte received.
    spi->IFLS |= SPI_IFLS_RXIFLSEL_LVL_1_4;
}

void SPISendByte(uint8_t SendData)
{
    // Wait while the transmit buffer is not empty.

    // Transmit the data using the TXDATA register.

    // Wait until the transmitter is not busy (that is, transmission is complete).
}

uint8_t SPIReceiveByte(void)
{
    uint8_t ReturnValue = 0;

    // First, empty the Rx FIFO of any "garbage" data from previous transmissions.
    // While it is not empty, read the data in the Rx FIFO.
    while (!((spi_sr->STAT & SPI_STAT_RFE_MASK) == SPI_STAT_RFE_EMPTY)){
      ReturnValue = spi_sr->RXDATA;
    }

    // Transmit dummy data (using SPISendByte for example) to generate SPI clock.
    SPISendByte(0xFF);

    // Wait while the Rx FIFO is empty (that is, no data received yet).
    while ((spi_sr->STAT & SPI_STAT_RFE_MASK) == SPI_STAT_RFE_EMPTY);

    // Assign ReturnValue received data from the RXDATA register.
    ReturnValue = spi_sr->RXDATA;

    return ReturnValue;
}
