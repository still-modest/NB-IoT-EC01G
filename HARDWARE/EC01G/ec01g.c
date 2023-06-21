#include "ec01g.h"

char *strx=0,*stry=0,*Readystrx,*Errstrx ; 	//返回值指针判断
extern unsigned char  RxBuffer[2500];
extern unsigned int RxCounter;
unsigned char locBuffer[1500];
unsigned int locDataSize;
double longitude;
double latitude;
void Uart1_SendStr(char*SendBuf)//串口1打印数据
{
	while(*SendBuf)
	{
        while((USART1->SR&0X40)==0);//等待发送完成 
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}

void Clear_Buffer(void)//清空缓存
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		IWDG_Feed();//喂狗
	
}

void EC01G_Init(void)//对EC01G初始化
{
	while(strx==NULL)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"ECRDY");
	}
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
	
	/********************查询sim卡的ECICCID信息********************/
	printf("AT+ECICCID\r\n");//查询sim卡的ECICCID信息
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECICCID\r\n");////查询sim卡的ECICCID信息
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/**************附着网络********************/
	printf("AT+CGATT=1\r\n");//附着网络
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+CGATT=1\r\n");////附着网络
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/********************根据运营商修改APN********************/
	printf("AT+CGDCONT=1,\"IP\",CMNET\r\n");//根据运营商修改APN
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+CGDCONT=1,\"IP\",CMNET\r\n");////根据运营商修改APN
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/********************激活网络********************/
	printf("AT+CGACT=1\r\n");//激活网络
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+CGACT=1\r\n");////激活网络
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
			Clear_Buffer();
	}
	Clear_Buffer();
	/********************关闭飞行模式********************/
	printf("AT+CFUN=1\r\n");//关闭飞行模式
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+CFUN=1\r\n");////关闭飞行模式
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/********************当前网络注册状态********************/
	printf("AT+CEREG?\r\n");//当前网络注册状态
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,1");//查看是否返回+CEREG: 0,1
	stry=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,2");//查看是否返回+CEREG: 0,2
	while(strx==NULL && stry==NULL)
	{
			Clear_Buffer();
			printf("AT+CEREG?\r\n");
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,1");//查看是否返回+CEREG: 0,1
			stry=strstr((const char*)RxBuffer,(const char*)"+CEREG: 0,2");//查看是否返回+CEREG: 0,2
	}
	Clear_Buffer();
	
    IWDG_Feed();//喂狗
}

/*创建并配置客户端*/
void MQTT_Config(u8 *ProductKey,u8 *DeviceName,u8 *DeviceSecret)
{
	/********************配置保活时间********************/
	printf("AT+ECMTCFG=\"keepalive\",0,130\r\n");//配置保活时间
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"keepalive\",0,130\r\n");////配置保活时间
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************配置会话类型********************/
	printf("AT+ECMTCFG=\"session\",0,0\r\n");//配置会话类型
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"session\",0,0\r\n");////配置会话类型
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************配置消息发送超时时间********************/
	printf("AT+ECMTCFG=\"timeout\",0,20\r\n");//配置消息发送超时时间
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"timeout\",0,20\r\n");////配置消息发送超时时间
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************配置要使用的MQTT协议版本********************/
	printf("AT+ECMTCFG=\"version\",0,4\r\n");//配置要使用的MQTT协议版本
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"version\",0,4\r\n");////配置要使用的MQTT协议版本
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************配置云类型和云发送数据的格式********************/
	printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");//配置云类型和云发送数据的格式
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");////配置云类型和云发送数据的格式
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************配置云类型和云发送数据的格式********************/
	printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");//配置云类型和云发送数据的格式
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG =\"cloud\",0,2,1\r\n");////配置云类型和云发送数据的格式
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	
	/********************配置阿里云的设备信息,需要阿里云三元素********************/
	printf("AT+ECMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n", ProductKey, DeviceName, DeviceSecret);//配置阿里云的设备信息,需要阿里云三元素
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+ECMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n", ProductKey, DeviceName, DeviceSecret);//配置阿里云的设备信息,需要阿里云三元素
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();

	IWDG_Feed();//喂狗
}

void MQTT_Link(u8 *HostAddress, u8 *HostPort)
{
	/********************打开客户端连接********************/
	printf("AT+ECMTOPEN=0,\"%s\",%s\r\n", HostAddress, HostPort);//打开客户端连接
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+ECMTOPEN: 0,0");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+ECMTOPEN=0,\"%s\",%s\r\n", HostAddress, HostPort);//打开客户端连接
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+ECMTOPEN: 0,0");////查看是否返回OK
	}
	Clear_Buffer();
	
	/********************连接客户端到 MQTT 服务器********************/
	printf("AT+ECMTCONN=0,\"12345\"\r\n");//连接客户端到 MQTT 服务器
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"+ECMTCONN: 0,0,0");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+ECMTCONN=0,\"12345\"\r\n");//连接客户端到 MQTT 服务器
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"+ECMTCONN: 0,0,0");////查看是否返回OK
	}
	Clear_Buffer();
	
	IWDG_Feed();//喂狗
}

void GPS_Init(void)
{
	/**************打开GPS********************/
	printf("AT+GPS=1\r\n");//打开GPS
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			printf("AT+GPS=1\r\n");////打开GPS
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK");
	}
	Clear_Buffer();
	/**************设置GPS信息发送频率********************/
	printf("AT+GPSRD=10\r\n");//设置GPS信息发送频率
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"OK*35");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+GPSRD=10\r\n");////设置GPS信息发送频率
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"OK*35");
	}
	Clear_Buffer();
	/**************设置GPS信息发送频率********************/
	printf("AT+LOCATION=1\r\n");//设置GPS信息发送频率
	delay_ms(500);
	strx=strstr((const char*)RxBuffer,(const char*)"AT+LOCATION=1");//查看是否返回OK
	while(strx==NULL)
	{
			Clear_Buffer();
			//printf("AT+LOCATION=1\r\n");////设置GPS信息发送频率
			delay_ms(500);
			strx=strstr((const char*)RxBuffer,(const char*)"AT+LOCATION=1");
			IWDG_Feed();//喂狗
	}
	Clear_Buffer();
	
	IWDG_Feed();//喂狗
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
		temp=NMEA_Str2num(p1+posx,&dx);		//字符串转换为整数数值
		longitude = temp/NMEA_Pow(10,dx+2);	//得到°
		
		rs = temp%NMEA_Pow(10,dx+2);				//得到'	
		
		longitude = longitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为° 
		longitude = longitude/100000;
	}
	
	posx=NMEA_Comma_Pos(p1,5);	
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		//字符串转换为整数数值
		latitude = temp/NMEA_Pow(10,dx+2);	//得到°
		
		rs = temp%NMEA_Pow(10,dx+2);				//得到'	
		
		latitude = latitude*NMEA_Pow(10,5)+(rs*NMEA_Pow(10,5-dx))/60;//转换为° 
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
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
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
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
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

