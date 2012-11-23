#ifndef NOKIALCD_H_
#define NOKIALCD_H_

void nokialcd_init(void);
void nokialcd_clear();
void nokialcd_home();
void nokialcd_write_command(unsigned char c);
void nokialcd_write_data(unsigned char c);

#define NOKIALCD_BLANK  0x08
#define NOKIALCD_NORMAL 0x0C
#define NOKIALCD_ALLON  0x09
#define NOKIALCD_INVERSE 0x0D

#define NOKIALCD_HORIZONTAL 0x20
#define NOKIALCD_VERTICAL 0x22

#endif /*NOKIALCD_H_*/
