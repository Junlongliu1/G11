/****************************************************************************
 *   FileName    : mt_adc.c
 *   Description : Multi-input ADC
 ****************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited to re-
distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code shall
constitute any express or implied warranty of any kind, including without limitation, any warranty
of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
or other third party intellectual property right. No warranty is made, express or implied,
regarding the information's accuracy, completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************/
#include <types.h>
#include <bsp.h>
#include <clock.h>
#include <main.h>
#include "reg_phys.h"
#include "tcc_ckc.h"
#include "mt_adc.h"
#include "device_share_config.h"
#include "debug.h"
#include "bsp_int.h"

/* Defines */
#define HwMC_ADC          	 	((volatile uint32 *)(MICOM_BASE_ADDR + 0x700000))

#define TCC_ADC_SWRESET_REG		0x1B930010 //24th bit
#define TCC_ADC_CLOCK_REG 		0x1B931074 //PCLK_CFG_ADC_CTRL

#define DMA_BAS_ADDR			0x1B600000
#define DMA_SREQ_REG			(DMA_BAS_ADDR + 0x24)
#define DMA_CONFIG_REG			(DMA_BAS_ADDR + 0x30)
#define DMA_SYNC_REG			(DMA_BAS_ADDR + 0x34)

#define DMA0_SRC_ADDR_REG		(DMA_BAS_ADDR + 0x100)
#define DMA0_DEST_ADDR_REG		(DMA_BAS_ADDR + 0x104)
#define DMA0_LLI_REG			(DMA_BAS_ADDR + 0x108)
#define DMA0_CONTROL_REG 		(DMA_BAS_ADDR + 0x10C)
#define DMA0_CONFIG_REG 		(DMA_BAS_ADDR + 0x110)

#define DMA_HIDDEN_REG 			0x1B93001C

#define ADC_PERI_CLOCK			CKC_MHZ(12)
#define ADC_CONV_CYCLE			30

#define ADC_writel	writel
#define ADC_readl	readl

//#define ADC_DBG_ENABLED
//#define ADC_DMA_ENABLED
//#define ADC_TEST_ENABLED
#define ADC_AUTO_SCAN_ENABLED

#ifdef ADC_DBG_ENABLED
#define ADC_DBG(fmt,args...)   printf(fmt,## args)
#else
#define ADC_DBG(fmt,...)   do {} while(0)
#endif

/* Variables */
//uint32 ADCCompltedFlag = 0;	// removed for codesonar warning
#ifdef ADC_DMA_ENABLED
uint32 DMAData[100];
#endif

#ifdef ADC_TEST_ENABLED
/* Key definition*/
#define KEY_NULL		0x00
#define	KEY_1			0x01
#define	KEY_2			0x02
#define	KEY_3			0x03
#define	KEY_4			0x04
#define	KEY_5			0x05
#define	KEY_6			0x06

#define KEY_ENTER		0x11
#define KEY_SCAN		0x12
#define KEY_SETUP		0x13
#define KEY_MENU		0x14
#define KEY_DISP		0x15
#define KEY_TRACKDOWN		0x16
#define KEY_TRACKUP		0x17
#define KEY_FOLDERDOWN		0x18
#define KEY_FOLDERUP		0x19
#define KEY_POWER		0x1a
#define KEY_RADIO		0x1b
#define KEY_MEDIA		0x1c
#define KEY_PHONE		0x1d //KEY_HANDSFREE
#define KEY_VOLUMEUP		0x1e
#define KEY_VOLUMEDOWN		0x1f

typedef struct tcc_adc_button
{
	uint32	lower;
	uint32	upper;
	uint16  vkcode;
	uint8	name[15];
}tcc_adc_button;
static struct tcc_adc_button adc_btn_map_00[] =
{
	/* ADC_CHANNEL0 */
	{ 0x00,  0x83, KEY_ENTER,	"ENTER      "},
	{0x266, 0x2EE, KEY_SCAN,	"SCAN       "},
	{0x732, 0x8CC, KEY_SETUP,	"SETUP      "},
	{0x9E5, 0xC18, KEY_MENU,	"MENU       "},
	{0xD4A, 0xFA0, KEY_DISP,	"DISP       "},
};
static struct tcc_adc_button adc_btn_map_01[] =
{
	/* ADC_CHANNEL1 */
	{ 0x6B,  0x83, KEY_TRACKDOWN,	"TRACK DOWN "},
	{0x732, 0x8CC, KEY_TRACKUP,	"TRACK UP   "},
	{0x9E5, 0xC18, KEY_FOLDERDOWN,	"FOLDER DOWN"},
	{0xD4A, 0xFA0, KEY_FOLDERUP,	"FOLDER UP  "},
};
static struct tcc_adc_button adc_btn_map_02[] =
{
	/* ADC_CHANNEL2 */
	{ 0x00,  0x83, KEY_POWER,	"POWER      "},
	{0x732, 0x8CC, KEY_RADIO,	"RADIO      "},
	{0x9E5, 0xC18, KEY_MEDIA,	"MEDIA      "},
	{0xD4A, 0xFA0, KEY_PHONE,	"PHONE      "},
};
static struct tcc_adc_button adc_btn_map_10[] =
{
	/* ADC_CHANNEL10 */
	{ 0x6B,  0x83, KEY_1,		"KEY1       "},
	{0x266, 0x2EE, KEY_2,		"KEY2       "},
	{0x52B, 0x651, KEY_3,		"KEY3       "},
	{0x732, 0x8CC, KEY_4,		"KEY4       "},
	{0x9E5, 0xC18, KEY_5,		"KEY5       "},
	{0xD4A, 0xFA0, KEY_6,		"KEY6       "},
};

typedef struct tcc_adc_button_map
{
	int32 size;
	tcc_adc_button * button;
}tcc_adc_button_map;

struct tcc_adc_button_map adc_btn_map_ptr[] =
{
	{ARRAY_SIZE(adc_btn_map_00), adc_btn_map_00},
	{ARRAY_SIZE(adc_btn_map_01), adc_btn_map_01},
	{ARRAY_SIZE(adc_btn_map_02), adc_btn_map_02},
	{0, NULL},	// 03
	{0, NULL},	// 04
	{0, NULL},	// 05
	{0, NULL},	// 06
	{0, NULL},	// 07
	{0, NULL},	// 08
	{0, NULL},	// 09
	{ARRAY_SIZE(adc_btn_map_10), adc_btn_map_10},
	{0, NULL}	// 11
};

#define IS_ADC_RNG(x,btn)	(((btn.lower<=x) && (x<=btn.upper)) ? 1 : 0)
#endif /* ADC_TEST_ENABLED */

static void tcc_adc_delay(int32 delay);
static void tcc_adc_isr(void *arg);
//static void tcc_adc_SWreset_Set(void);
//tatic void tcc_adc_SWreset_Clear(void);
static int32 tcc_adc_Clock_Enable(void);
//static int32 tcc_adc_Clock_Disable(void);
//static void tcc_adc_start_MultiChannel(int32 MultiChannel, int32 Type);
static int32 tcc_adc_CheckDoneBit(void);
//static int32 tcc_adc_GetCVTCount(void);

//void init_adc(int32 Type);
//uint32 tcc_adc_read(int32 Channel);
//uint32* mt_adc_auto_scan(void);
//void tcc_adc_watching_evt(void);

/* Functions */
static void tcc_adc_delay(int32 delay)
{
	int32 i;
	/* wait 1 cycle */
	delay = delay * 50;	// 600/12 (CPU_CLOCK/ADC_PERI_CLOCK)
	for (i=0 ; i<delay ; i++)
	{
		NOP_DELAY();
	}
}

static void tcc_adc_isr(void *arg)
{
	uint32 IRQStatus = 0;
	IRQStatus = ADC_readl(HwMC_ADC+IRQ_DREQ_STS_REG);
	IRQStatus |= 1;
	ADC_writel(IRQStatus, HwMC_ADC+IRQ_DREQ_STS_REG);

	//ADCCompltedFlag= 1;
}
/* GHC Warning : declared b	ut never referenced
static void tcc_adc_SWreset_Set(void)
{
	ADC_writel(ADC_readl(TCC_ADC_SWRESET_REG) &= ~(1<<24), TCC_ADC_SWRESET_REG);
}

static void tcc_adc_SWreset_Clear(void)
{
	ADC_writel(ADC_readl(TCC_ADC_SWRESET_REG) |= (1<<24), TCC_ADC_SWRESET_REG);
}
*/

static int32 tcc_adc_Clock_Enable(void)
{
	int32 ret = -1;

	ret = tcc_set_peri(PERI_ADC, ENABLE, ADC_PERI_CLOCK); //set clock
	if (ret != 0)
	{
		ADC_DBG("failed to enable adc clock\n");
	}

	return ret;
}

/* GHC Warning : declared but never referenced
static int32 tcc_adc_Clock_Disable(void)
{
	int32 ret = -1;

	ret = tcc_set_peri(PERI_ADC, DISABLE, ADC_PERI_CLOCK); //set clock
	if (ret != 0)
	{
		ADC_DBG("failed to disable adc clock\n");
	}

	return ret;
}
*/

#ifdef ADC_DMA_ENABLED
void tcc_adc_DMA_Init(int32 Channel, int32 DMAChannel)
{
}

void tcc_adc_DMA_Enable(int32 Channel)
{
}

void tcc_adc_DMA_Disable(int32 Channel)
{
}
#endif

static int32 tcc_adc_CheckDoneBit(void)
{
	int32 ret = 0;
	ret = ((ADC_readl(HwMC_ADC + SMP_CMD_STS_REG) & 0x80000000) >> 31);

	return ret;
}


/* GHC Warning : declared but never referenced
static int32 tcc_adc_GetCVTCount(void)
{
	int32 ret = 0;

	ret = ADC_readl(HwMC_ADC + SMP_CMD_STS_REG) & 0x1f00000;//CVT_CNT

	return ret;
}
*/

#ifdef ADC_DMA_ENABLED
void tcc_adc_Set_DMA(void)
{
	ADC_writel((ADC_readl(DMA_CONFIG_REG) |= (1<<0)), DMA_CONFIG_REG);	//DMA Enable
//	DMA_SREQ_REG	|= (1<<12);
//	DMA_SYNC_REG |= (1<<12);
	ADC_writel(0xFFFFFFFF, DMA_HIDDEN_REG);

	//DMA 0 channel
	ADC_writel(0x1B700080, DMA0_SRC_ADDR_REG);	//ADC OUTPUT(0 Channl)
	ADC_writel((uint32)&DMAData[0], DMA0_DEST_ADDR_REG);
	ADC_writel((uint32)&DMAData[0], DMA0_LLI_REG );
	ADC_writel(((2<<21)|(2<<18)|(0<<15)|(0<<12)|(100<<0)), DMA0_CONTROL_REG);//
	ADC_writel((ADC_readl(DMA0_CONFIG_REG) |= (2<<11)|(12<<1)|(1<<0)),DMA0_CONFIG_REG); 	//0 channel of DMA Enable
}
#endif

void init_adc(int32 Type)
{
	int32 ret = -1;
	ret = tcc_adc_Clock_Enable();
	if (ret != 0) {
		ADC_DBG("init_adc failed\n");
	}
	else {
		ADC_DBG("init_adc\n");
	}
	if(Type == ADC_MODE_IRQ)
	{
		ret = BSP_IntVectSet(INT_ADC, PRIORITY_NO_MEAN, INT_TYPE_LEVEL_HIGH, tcc_adc_isr, 0);//BSP_NULL);
		if(ret != 1) {
			ADC_DBG("failed to set irq\n");
			return;
		}
		BSP_IntSrcEn(INT_ADC);
	}
#ifdef ADC_DMA_ENABLED
	else if(Type == ADC_MODE_DMA)
	{
		tcc_adc_Set_DMA();
	}
#endif
}

/* GHC Warning : declared but never referenced
static void tcc_adc_start_MultiChannel(int32 MultiChannel, int32 Type)
{
	if(MultiChannel != 0)
	{
		ADC_writel((MultiChannel&0x1FF), HwMC_ADC+SMP_CMD_STS_REG);
		if(Type ==ADC_MODE_IRQ)
		{
			//HwMC_ADC->uADC_CFG.bReg.IRQ_EN = 1;
			ADC_writel((ADC_readl(HwMC_ADC + CTRL_CFG_REG) | 0x100), (HwMC_ADC + CTRL_CFG_REG));//IRQ_EN
		}
#ifdef ADC_DMA_ENABLED
		if(Type ==ADC_MODE_DMA)
		{
			tcc_adc_Set_DMA();
			//HwMC_ADC->uADC_CFG.bReg.DREQ_EN= 1;
			ADC_writel((ADC_readl(HwMC_ADC + CTRL_CFG_REG) | 0x200), (HwMC_ADC + CTRL_CFG_REG));//DREQ_EN
		}
#endif
	}
}
*/

uint32 tcc_adc_read(int32 Channel)
{
	uint32 data = 0xFFFF;
	uint32 i, timeout=ADC_CONV_CYCLE*2;

	if((ADC_CHANNEL0 <= Channel) && (Channel < ADC_CHANNEL_MAX))
	{
		ADC_writel(1<<(Channel&0x1FF), HwMC_ADC+SMP_CMD_STS_REG);

		if (tcc_adc_CheckDoneBit() == 0)
		{
			for (i = 0; i < timeout; i++)
			{
				if (tcc_adc_CheckDoneBit() != 0)
				{
					ADC_DBG("wait done = %d\n",i);
					break;
				}
				tcc_adc_delay(1);	/* wait 1 cycle for ADC peri */
			}
			if (i >= timeout)
			{
				printf("adc timeout waiting for sampling complete\n");
			}
		}
		ADC_DBG("(ch %d)cmd = 0x%x, data addr = 0x%x\n", Channel,ADC_readl(HwMC_ADC+SMP_CMD_STS_REG), HwMC_ADC+SDOUT0_REG);
		data = ADC_readl(HwMC_ADC+SDOUT0_REG);
	}
	else
	{
		ADC_DBG("Wrong param\n");
	}

	return data;
}

uint32* mt_adc_auto_scan(void)
{
#ifdef ADC_AUTO_SCAN_ENABLED
	int32 ch;
	uint32 read_data = 0;
	static uint32 adc_scan_val[ADC_CHANNEL_MAX];

	for (ch = ADC_CHANNEL0; ch < ADC_CHANNEL_MAX; ch++)
	{
		read_data = tcc_adc_read(ch);
		adc_scan_val[ch] = read_data;
		//ADC_DBG("adc_data[%d] : %d\n", ch, read_data);
	}

	return adc_scan_val;
#else
	printf("Not supported ADC auto scan\n");
	return NULL;
#endif
}

void tcc_adc_watching_evt(void)
{
#ifdef ADC_TEST_ENABLED
	uint32 *adc_buff = NULL;
	uint32 adc_data = 0x0;
	int32 ch = 0, i = 0;
	uint16 old_key = 0, new_key = 0;

	for(;;)
	{
		adc_buff = mt_adc_auto_scan();
		if (adc_buff == NULL)
			break;

		new_key=KEY_NULL;
		for (ch=ADC_CHANNEL0; ch<ADC_CHANNEL_MAX; ch++)
		{
			if (adc_btn_map_ptr[ch].size == 0)
				continue;
			switch (ch)
			{
				case ADC_CHANNEL0:
				case ADC_CHANNEL1:
				case ADC_CHANNEL2:
				case ADC_CHANNEL10:
					adc_data = (adc_buff[ch]&0xFFF);
					for(i=0; i<adc_btn_map_ptr[ch].size; i++){
						if (IS_ADC_RNG(adc_data,(adc_btn_map_ptr[ch].button[i])))
						{
							new_key = adc_btn_map_ptr[ch].button[i].vkcode;
							if (old_key==new_key)
								printf("\033[1A");
							printf("[AD%02d] : 0x%03X : %s\n", ch, adc_data, adc_btn_map_ptr[ch].button[i].name);
							old_key = new_key;
							continue;
						}
					}
					break;
				default:
					break;
			}
		}
		tcc_adc_delay(10);
		if ((old_key!=KEY_NULL) && (new_key==KEY_NULL))
		{
			old_key=KEY_NULL;
		}
	}
#else /* ADC_TEST_ENABLED */
	printf("Not supported ADC Test code\n");
#endif /* ADC_TEST_ENABLED */
}
