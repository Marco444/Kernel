
#ifndef TIME_DRIVER_H
#define TIME_DRIVER_H
#include <stdint.h>
#include <time.h>
// Funciones de libasm.asm
extern int get_rtc_seconds();
extern int get_rtc_minutes();
extern int get_rtc_hours();
extern int get_rtc_weekday();
extern int get_rtc_day();
extern int get_rtc_month();
extern int get_rtc_year();

int bcd_decimal(uint8_t hex);
void convert_weekday(char *buffer, int weekday);
void time_syscall(char *buffer);
int getSeconds();
#endif