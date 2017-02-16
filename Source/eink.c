#include <eink.h>

#include <stm32f10x.h>

void RCCInit(void){
	// ��λRCCʱ������(���ڵ���ģʽ) 
	RCC_DeInit();

	// ʹ���ⲿ���� 
	RCC_HSEConfig(RCC_HSE_ON);

	// �ȴ��ⲿ�����ȶ�
	if(RCC_WaitForHSEStartUp() == SUCCESS){
		// �������໷Ƶ��PLLCLK = 8MHz * 9 = 72 MHz 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	}else{
		// ʹ���ڲ����� 
		RCC_HSICmd(ENABLE);
		// �ȴ����������ȶ� 
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
		// �������໷Ƶ��PLLCLK = 8MHz/2 * 16 = 64 MHz 
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_16);
	}

	// ʹ��FLASH��Ԥȡ���� 
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	//���ô�����ʱֵ,FLASH_Latency_2Ϊ������ʱ����
	FLASH_SetLatency(FLASH_Latency_2);
	//����ϵͳ����ʱ��
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	//���ø����豸����ʱ�ӣ�RCC_HCLK_Div1Ϊϵͳʱ�ӳ���1
	RCC_PCLK2Config(RCC_HCLK_Div1); 
	//���õ����豸����ʱ�ӣ�RCC_HCLK_Div2Ϊϵͳʱ�ӳ���2
	RCC_PCLK1Config(RCC_HCLK_Div2);
	//ʹ�����໷��Ƶ
	RCC_PLLCmd(ENABLE);

	// �ȴ����໷��Ƶ���Ƶ���ȶ� 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	// ѡ�����໷ʱ��Ϊϵͳʱ��Դ 
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	// �ȴ�������� 
	while(RCC_GetSYSCLKSource() != 0x08);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,
						ENABLE);
}

void GPIOInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10;    //abcd OE ST LT0���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_14 |GPIO_Pin_15;    //abcd OE ST LT0���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    //abcd OE ST LT0���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;    //VPP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	
	// LED GPIO Define
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13;    //abcd OE ST LT0���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void DELAY_100nS(size_t t){
	for(int i=0;i<t;++i)
		for(int j=0;j<1;++j);
}

void lcd_chkstatus(void){
	unsigned char busy;
	do{
		SPI4W_WRITECOM(0x71);
		busy = nBUSY;
		busy =!(busy & 0x01);        
	}while(busy);   
	DELAY_mS(200);
}

void EPD_W21_Init(void){
	nBS_L;				//4 wire spi mode selected
	
	nRST_L;				//module reset	
	DELAY_mS(1000);
	nRST_H;
	DELAY_mS(1000);
}

void SPI4W_sendData(unsigned char data){
	for(short scnt=0; scnt<8; ++scnt){
		if(data&0x80){
			SDA_H;
		}else{
			SDA_L;
        }
		DELAY_100nS(10);
		SCLK_H;
		DELAY_100nS(10);
		SCLK_L;
		data<<=1;
		DELAY_100nS(10);
	}
}

void SPI4W_WRITECOM(unsigned char INIT_COM){
	nCS_H;
	nCS_L;
	SCLK_L;
	nDC_L;
	SPI4W_sendData(INIT_COM);
	nCS_H; 
}

void SPI4W_WRITEDATA(unsigned char INIT_DATA){
	nCS_H;
	nCS_L;
	SCLK_L;
	nDC_H;
	SPI4W_sendData(INIT_DATA);
	nCS_H;
}

void MCU_write_flash(unsigned char command){
	for(short i=0; i<8; ++i){
		SCLK_L;
		if (command&0x80){
			SDA_H;
		}else{       
			SDA_L;
		}
		command <<= 1;
		SCLK_H;
    }
	SCLK_L;
}

void displayImage(const unsigned char *Image){
    EPD_W21_Init();
    
    SPI4W_WRITECOM(0x01); 
    SPI4W_WRITEDATA (0x37);	    //POWER SETTING
    SPI4W_WRITEDATA (0x00);
//  SPI4W_WRITEDATA (0x3c);
//  SPI4W_WRITEDATA (0x3c);
    
    SPI4W_WRITECOM(0X00);			//PANNEL SETTING
    SPI4W_WRITEDATA(0xCF);
    SPI4W_WRITEDATA(0x08);
    
    SPI4W_WRITECOM(0x06);         //boost�趨
    SPI4W_WRITEDATA (0xc7);	   	
    SPI4W_WRITEDATA (0xcc);
    SPI4W_WRITEDATA (0x28);

    SPI4W_WRITECOM(0x30);			//PLL setting
    SPI4W_WRITEDATA (0x3c);        

    SPI4W_WRITECOM(0X41);			//TEMPERATURE SETTING
    SPI4W_WRITEDATA(0x00);

    SPI4W_WRITECOM(0X50);			//VCOM AND DATA INTERVAL SETTING
    SPI4W_WRITEDATA(0x77);

    SPI4W_WRITECOM(0X60);			//TCON SETTING
    SPI4W_WRITEDATA(0x22);

    SPI4W_WRITECOM(0x61);        	//tres			��ɫ640*384
    SPI4W_WRITEDATA (0x02);		//source 640
    SPI4W_WRITEDATA (0x80);
    SPI4W_WRITEDATA (0x01);		//gate 384
    SPI4W_WRITEDATA (0x80);

    SPI4W_WRITECOM(0X82);			//VDCS SETTING
    SPI4W_WRITEDATA(0x1E);		//decide by LUT file

    SPI4W_WRITECOM(0xe5);			//FLASH MODE		   	
    SPI4W_WRITEDATA(0x03);
    
    
    transferImage(Image);
    
    SPI4W_WRITECOM(0x04);	 	    //POWER ON	

    lcd_chkstatus();
    
    SPI4W_WRITECOM(0x12);		//display refresh
    DELAY_mS(100);

    lcd_chkstatus();

/**********************************flash sleep**********************************/
    SPI4W_WRITECOM(0X65);			//FLASH CONTROL
    SPI4W_WRITEDATA(0x01);

    nCS_L;					//MFCSB ����
    MCU_write_flash(0xB9);
    nCS_H;					//MFCSB ����

    SPI4W_WRITECOM(0X65);			//FLASH CONTROL
    SPI4W_WRITEDATA(0x00);
/**********************************flash sleep**********************************/	

    SPI4W_WRITECOM(0x02);
    lcd_chkstatus();

    SPI4W_WRITECOM(0x07);
    SPI4W_WRITEDATA(0xa5);
}

void transferImage(const unsigned char *Image){
	unsigned char temp1, temp2 = 0;
	
	SPI4W_WRITECOM(0x10); // Start the transfer
	for(int i=0; i<30720;++i){
		temp1 = Image[i];
		for(int j=0;j<8;++j){
			if(temp1&0x80){
				temp2 = 0x00;
			}else{
				temp2 = 0x03;
			}
			temp2 <<= 4;
			temp1 <<= 1;
			++j;
			if(temp1&0x80){
				temp2 |= 0x00;
			}else{
				temp2 |= 0x03;
			}
			temp1 <<= 1;
			SPI4W_WRITEDATA(temp2);
		}
	}
}
