#define SIP_EN_DIS ENABLE
#define SPI_TYPE MASTER // MASTER SLAVE

inline void SPI_INI(void)
{
	SSPSTAT= 01000000b; // output time sampling
	SSPCON=0011000b; //Mode 1,1 SPI Master Mode, Fosc/4 bit time
}
inline void SPI_INI(void)
{
	SSPSTAT= 01000000b; // output time sampling
	SSPCON=00110100b;   // SPI Slave Mode, Slave Select Required, SSP is on
}
SSPBUF= // to be updated when data need to be trasmitted

// spi initialisation
To reset or reconfigure
SPI mode, clear the SSPEN bit, re-initialize the
SSPCON registers and then set the SSPEN bit. This
configures the SDI, SDO, SCK and SS pins as serial
port pins.

//SPI Apllication
Both processors should be
programmed to the same Clock Polarity (CKP), then
both controllers would send and receive data at the
same time. Whether the data is meaningful (or dummy
data) depends on the application software. This leads
to three scenarios for data transmission:
• Master sends data – Slave sends dummy data
• Master sends data – Slave sends data
• Master sends dummy data – Slave sends data

// master trasmission
The master can initiate the data transfer at any time
because it controls the SCK. The master determines
when the slave (Processor 2, Figure 13-2) is to
broadcast data by the software protocol.
In Master mode, the data is transmitted/received as
soon as the SSPBUF register is written to.