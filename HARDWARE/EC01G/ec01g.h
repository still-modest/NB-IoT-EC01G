#ifndef __EC01G_H
#define __EC01G_H
#include "usart.h"	 
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "delay.h"
#include "stm32f10x.h"

void Uart1_SendStr(char*SendBuf);//����1��ӡ����
void Clear_Buffer(void);//��ջ���
void EC01G_Init(void);//��ʼ��EC01G
void MQTT_Config(u8 *ProductKey,u8 *DeviceName,u8 *DeviceSecret);//����MQTT
void MQTT_Link(u8 *HostAddress, u8 *HostPort);//MQTT����
void GPS_Init(void);
void getLocationData(void);
uint8_t NMEA_Comma_Pos(uint8_t *buf,uint8_t cnt);
int NMEA_Str2num(uint8_t *buf,uint8_t*dx);
uint32_t NMEA_Pow(uint8_t m,uint8_t n);
void publish_locationData(double lonData, double latData);
#endif
