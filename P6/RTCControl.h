/*
 * MyPrint.c
 *
 * Created: 5/7/2021 8:13:09 PM
 * Author : jopc1
 */ 


#ifndef RTCCONTROL_H_
#define RTCCONTROL_H_

#define SECONDS_ADDR 0x00
typedef union {
	struct  {
		unsigned char un_seconds 	:4,
		dec_seconds    :3,
		ch_sec    :1;
	} field;
	unsigned char reg;
} seconds_type;

#define MINUTES_ADDR 0x01
typedef union {
	struct  {
		unsigned char un_minutes  	:4,
		dec_minutes    :3,
		RESERVED    :1;
	} field;
	unsigned char reg;
} minutes_type;

#define HOURS_ADDR 0x02
typedef union {
	struct  {
		unsigned char un_hours  	:4,
		dec_hours    :1,
		PMAM_dec_hours :1,
		format_hours :1,
		RESERVED    :1;
	} field;
	unsigned char reg;
} hours_type;

#define DAY_ADDR 0x03
typedef union {
	struct {
		unsigned char day	:3,
		RESERVED	:5;
	} field;
	unsigned char reg;
} day_type;

#define DATE_ADDR 0x04
typedef union {
	struct {
		unsigned char un_date	:4,
		dec_date	:2,
		RESERVED	:2;
	} field;
	unsigned char reg;
} date_type;

#define MONTH_ADDR 0x05
typedef union {
	struct  {
		unsigned char un_month  	:4,
		dec_mont    :1,
		RESERVED    :3;
	} field;
	unsigned char reg;
} month_type;

#define YEAR_ADDR 0x06
typedef union {
	struct {
		unsigned char un_year :4,
		dec_year	:4;
	} field;
	unsigned char reg;
} year_type;

typedef struct {
	seconds_type seconds;
	minutes_type minutes;
	hours_type hours;
	day_type day;
	date_type date;
	month_type month;
	year_type year;
} time_type;

void sendI2CDataArray(unsigned char SlAddr, unsigned char ptr, unsigned char *ptrData, unsigned char Size);
void receiveI2CDataArray(unsigned char SlAddr, unsigned char ptr, unsigned char *ptrData, unsigned char Size);
void SendI2CData(unsigned char Data);
void StopCond();
void I2Cinit();

#endif /* RTCCONTROL_H_ */