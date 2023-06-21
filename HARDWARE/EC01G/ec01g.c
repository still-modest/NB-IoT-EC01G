#include "ec01g.h"

char *strx=0,*stry=0,*Readystrx,*Errstrx ; 	//����ֵָ���ж�
extern unsigned char  RxBuffer[2500];
extern unsigned int RxCounter;
unsigned char locBuffer[1500];
unsigned int locDataSize;
double longitude;
double latitude;
void Uart1_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART1->SR&0X40)==0);//�ȴ�������� 
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}

void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
		IWDG_Feed();//ι��
	
}

void EC01G_Init(void)//��EC01G��ʼ��
{
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"ECRDY");
	}
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	
	/********************��ѯsim����ECICCID��Ϣ********************/
	printf("AT+ECICCID\r\n");//��ѯsim����ECICCID��Ϣ
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECICCID\r\n");////��ѯsim����ECICCID��Ϣ
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/**************��������********************/
	printf("AT+CGATT=1\r\n");//��������
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+CGATT=1\r\n");////��������
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/********************������Ӫ���޸�APN********************/
	printf("AT+CGDCONT=1,\"IP\",CMNET\r\n");//������Ӫ���޸�APN
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+CGDCONT=1,\"IP\",CMNET\r\n");////������Ӫ���޸�APN
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/********************��������********************/
	printf("AT+CGACT=1\r\n");//��������
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+CGACT=1\r\n");////��������
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
			Clear_Buffer();
	}
	Clear_Buffer();
	/********************�رշ���ģʽ********************/
	printf("AT+CFUN=1\r\n");//�رշ���ģʽ
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+CFUN=1\r\n");////�رշ���ģʽ
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/********************��ǰ����ע��״̬********************/
	printf("AT+CEREG?\r\n");//��ǰ����ע��״̬
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,1");//�鿴�Ƿ񷵻�+CEREG: 0,1
	stry=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,2");//�鿴�Ƿ񷵻�+CEREG: 0,2
	while(strx==NULL && stry==NULL)
	{
			Clear_Buffer();
			printf("AT+CEREG?\r\n");
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,1");//�鿴�Ƿ񷵻�+CEREG: 0,1
			stry=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,2");//�鿴�Ƿ񷵻�+CEREG: 0,2
	}
	Clear_Buffer();
	
    IWDG_Feed();//ι��
}

/*���������ÿͻ���*/
void MQTT_Config(u8 *ProductKey,u8 *DeviceName,u8 *DeviceSecret)
{
	/********************���ñ���ʱ��********************/
	printf("AT+ECMTCFG=\"keepalive\",0,130\r\n");//���ñ���ʱ��
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"keepalive\",0,130\r\n");////���ñ���ʱ��
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************���ûỰ����********************/
	printf("AT+ECMTCFG=\"session\",0,0\r\n");//���ûỰ����
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"session\",0,0\r\n");////���ûỰ����
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************������Ϣ���ͳ�ʱʱ��********************/
	printf("AT+ECMTCFG=\"timeout\",0,20\r\n");//������Ϣ���ͳ�ʱʱ��
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"timeout\",0,20\r\n");////������Ϣ���ͳ�ʱʱ��
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************����Ҫʹ�õ�MQTTЭ��汾********************/
	printf("AT+ECMTCFG=\"version\",0,4\r\n");//����Ҫʹ�õ�MQTTЭ��汾
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"version\",0,4\r\n");////����Ҫʹ�õ�MQTTЭ��汾
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************���������ͺ��Ʒ������ݵĸ�ʽ********************/
	printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");//���������ͺ��Ʒ������ݵĸ�ʽ
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");////���������ͺ��Ʒ������ݵĸ�ʽ
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************���������ͺ��Ʒ������ݵĸ�ʽ********************/
	printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");//���������ͺ��Ʒ������ݵĸ�ʽ
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");////���������ͺ��Ʒ������ݵĸ�ʽ
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************���ð����Ƶ��豸��Ϣ,��Ҫ��������Ԫ��********************/
	printf("AT+ECMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n", ProductKey, DeviceName, DeviceSecret);//���ð����Ƶ��豸��Ϣ,��Ҫ��������Ԫ��
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n", ProductKey, DeviceName, DeviceSecret);//���ð����Ƶ��豸��Ϣ,��Ҫ��������Ԫ��
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();

	IWDG_Feed();//ι��
}

void MQTT_Link(u8 *HostAddress, u8 *HostPort)
{
	/********************�򿪿ͻ�������********************/
	printf("AT+ECMTOPEN=0,\"%s\",%s\r\n", HostAddress, HostPort);//�򿪿ͻ�������
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+ECMTOPEN: 0,0");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+ECMTOPEN=0,\"%s\",%s\r\n", HostAddress, HostPort);//�򿪿ͻ�������
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+ECMTOPEN: 0,0");////�鿴�Ƿ񷵻�OK
	}
	Clear_Buffer();
	
	/********************���ӿͻ��˵� MQTT ������********************/
	printf("AT+ECMTCONN=0,\"12345\"\r\n");//���ӿͻ��˵� MQTT ������
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+ECMTCONN: 0,0,0");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+ECMTCONN=0,\"12345\"\r\n");//���ӿͻ��˵� MQTT ������
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+ECMTCONN: 0,0,0");////�鿴�Ƿ񷵻�OK
	}
	Clear_Buffer();
	
	IWDG_Feed();//ι��
}

void GPS_Init(void)
{
	/**************��GPS********************/
	printf("AT+GPS=1\r\n");//��GPS
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+GPS=1\r\n");////��GPS
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/**************����GPS��Ϣ����Ƶ��********************/
	printf("AT+GPSRD=10\r\n");//����GPS��Ϣ����Ƶ��
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK*35");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+GPSRD=10\r\n");////����GPS��Ϣ����Ƶ��
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK*35");
	}
	Clear_Buffer();
	/**************����GPS��Ϣ����Ƶ��********************/
	printf("AT+LOCATION=1\r\n");//����GPS��Ϣ����Ƶ��
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"AT+LOCATION=1");//�鿴�Ƿ񷵻�OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+LOCATION=1\r\n");////����GPS��Ϣ����Ƶ��
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"AT+LOCATION=1");
			IWDG_Feed();//ι��
	}
	Clear_Buffer();
	
	IWDG_Feed();//ι��
}

void getLocationData(void)
{
	uint8_t* p1,posx,dx;
	uint32_t temp;	   
	float rs;
	
	strcpy((char *)locBuffer, (const char *)RxBuffer);
	locDataSize = RxCounter;
	
	p1 = (uint8_t*)strstr((const char*)locBuffer,(const char*)"GNRMC");
	
	posx = NMEA_Comma_Pos(p1,3);
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		//�ַ���ת��Ϊ������ֵ
		longitude = temp/NMEA_Pow(10,dx+2);	//�õ���
		
		rs = temp%NMEA_Pow(10,dx+2);				//�õ�'	
		
		longitude = longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//ת��Ϊ�� 
		longitude = longitude/100000;
	}
	
	posx=NMEA_Comma_Pos(p1,5);	
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		//�ַ���ת��Ϊ������ֵ
		latitude = temp/NMEA_Pow(10,dx+2);	//�õ���
		
		rs = temp%NMEA_Pow(10,dx+2);				//�õ�'	
		
		latitude = latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//ת��Ϊ�� 
		latitude = latitude/100000;
	}

//	sprintf(longStr, "%.5f", longitude);
//	sprintf(latStr, "%.5f", latitude);
	publish_locationData(longitude, latitude);

	
}

uint8_t NMEA_Comma_Pos(uint8_t *buf,uint8_t cnt)
{
	uint8_t *local = buf;
	while(cnt)
	{
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//����'*'���߷Ƿ��ַ�,�򲻴��ڵ�cx������
		if(*buf==',')cnt--;
		buf++;
	}
	return buf-local;
}

int NMEA_Str2num(uint8_t *buf,uint8_t *dx)
{
	uint8_t *p=buf;
	uint32_t ires=0,fres=0;
	uint8_t ilen=0,flen=0,i;
	uint8_t mask=0;
	int res;
	while(1) //�õ�������С���ĳ���
	{
		if(*p=='-'){mask|=0X02;p++;}//�Ǹ���
		if(*p==','||(*p=='*'))break;//����������
		if(*p=='.'){mask|=0X01;p++;}//����С������
		else if(*p>'9'||(*p<'0'))	//�зǷ��ַ�
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//ȥ������
	for(i=0;i<ilen;i++)	//�õ�������������
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//���ȡ5λС��
	*dx=flen;	 		//С����λ��
	for(i=0;i<flen;i++)	//�õ�С����������
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}

uint32_t NMEA_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;
	while(n--)result*=m;
	return result;
}

void publish_locationData(double lonData, double latData)
{
	//char json[] ="AT+ECMTPUB=0,0,0,0,\"/sys/a13WJ9XtGs5/measurement/thing/event/property/post\",\"{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"GeoLocation\":{\"value\":{\"Longitude\":%.5f,\"Latitude\":%.5f,\"CoordinateSystem\":1}},\"method\":\"thing.event.property.post\"}\"";
	
	printf("AT+ECMTPUB=0,0,0,0,\"/sys/a13WJ9XtGs5/measurement/thing/event/property/post\",\"{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"GeoLocation\":{\"value\":{\"Longitude\":%.5f,\"Latitude\":%.5f,\"CoordinateSystem\":1}}},\"method\":\"thing.event.property.post\"}\"\r\n",lonData,latData);


}

