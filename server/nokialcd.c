#include <wiringPi.h>
#include "nokialcd.h"
#include "nokialcd-config.h"

static void nokialcd_write_byte(unsigned char c);
static void nokialcd_reset(void);

void nokialcd_init(void)
{
	// Nokia LCD Outputs
	pinMode(NOKIALCD_SCE_BIT, OUTPUT);
	pinMode(NOKIALCD_RST_BIT, OUTPUT);
	pinMode(NOKIALCD_DC_BIT, OUTPUT);
	pinMode(NOKIALCD_MOSI_BIT, OUTPUT);
	pinMode(NOKIALCD_SCLK_BIT, OUTPUT);
	// Set all outputs LOW
	digitalWrite(NOKIALCD_SCE_BIT, LOW);
	digitalWrite(NOKIALCD_DC_BIT, LOW);
	digitalWrite(NOKIALCD_MOSI_BIT, LOW);
	digitalWrite(NOKIALCD_SCLK_BIT, LOW);
	// Except RST which idles HIGH
	digitalWrite(NOKIALCD_RST_BIT, HIGH);
  
  	nokialcd_reset();
  
	nokialcd_write_command(0x21);   
	nokialcd_write_command(0xBF);   
	nokialcd_write_command(0x04);   
	nokialcd_write_command(0x14);
	nokialcd_write_command(0x20);   
	nokialcd_write_command(0x0C);  

  	nokialcd_home();
  	nokialcd_clear();
}

void nokialcd_clear()
{
  unsigned int i;

  digitalWrite(NOKIALCD_SCE_BIT, LOW);
  // DC = 1 data
  digitalWrite(NOKIALCD_DC_BIT, HIGH);
  digitalWrite(NOKIALCD_MOSI_BIT, LOW); 
 
  for (i = 0; i < 4032; i++) {
    /* pulse the CLOCK line */
    digitalWrite(NOKIALCD_SCLK_BIT, HIGH);
    digitalWrite(NOKIALCD_SCLK_BIT, LOW);
  }
  digitalWrite(NOKIALCD_SCE_BIT, HIGH);
}

void nokialcd_home()
{
  nokialcd_write_command(0x40); //y=0
  nokialcd_write_command(0x80); //x=0
}


void nokialcd_write_command(unsigned char c)
{
  // DC = 0 command
  digitalWrite(NOKIALCD_DC_BIT, 0);
  nokialcd_write_byte(c);
}

void nokialcd_write_data(unsigned char c)
{
  // DC = 1 data
  digitalWrite(NOKIALCD_DC_BIT, 1);
  nokialcd_write_byte(c);
}

static void nokialcd_write_byte(unsigned char c)
{
  unsigned char i;
  digitalWrite(NOKIALCD_SCE_BIT, LOW);
  for (i = 0; i < 8; i++)
  {
    if (c & 0x80) {
      /* set bit to 1 */
      digitalWrite(NOKIALCD_MOSI_BIT, HIGH);  
    } else {
      /* set bit to 0 */
      digitalWrite(NOKIALCD_MOSI_BIT, LOW);
    }
    /* pulse the CLOCK line */
    digitalWrite(NOKIALCD_SCLK_BIT, HIGH);
    digitalWrite(NOKIALCD_SCLK_BIT, LOW);
    /* get the next bit ready for pushing out */
    c = (c << 1);
  } 
  digitalWrite(NOKIALCD_SCE_BIT, HIGH);
}

static void nokialcd_reset(void)
{
  digitalWrite(NOKIALCD_RST_BIT, LOW);
  digitalWrite(NOKIALCD_RST_BIT, HIGH);
}
