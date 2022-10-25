/****************************************************************************
 *   FileName    : reg_phy.h
 *   Description :
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
#ifndef __BSP_REG_PHYS__H__
#define __BSP_REG_PHYS__H__

#ifndef __iomem
#define __iomem
#endif

#define REG64(addr) ((volatile uint64 *)(addr))
#define REG32(addr) ((volatile uint32 *)(addr))
#define REG16(addr) ((volatile uint16 *)(addr))
#define REG8(addr) ((volatile uint8 *)(addr))

#define writel(v, a) (*REG32(a) = (v))
#define readl(a) (*REG32(a))

#if defined(TCC803x_CS)
#define TCC_SNOR_BASE           0x08000000
#else
#define TCC_SNOR_BASE           0xD0000000
#endif

#define MICOM_BASE_ADDR     0x1B000000
/* Part 2. SMU & CKC */

#define TCC_SUBSYS_BASE         0x1B930000
#define TCC_CKC_BASE		0x1B931000 //8030 checked
#define TCC_TIMER_BASE	0x1B933000
#define TCC_WDT_BASE		0x1B934000 /* WTD Timer : 32 bits */
#define TCC_GPIO_BASE		0x1B935000 //8030 checked

#define TCC_PMGPIO_BASE	0x1B937800
#define TCC_BACKUP_RAM_BASE	0x1B937000

#define TCC_VIC_BASE		0x14100000

#define HwADDR_GPIOA                     (0x14200000)
#define HwADDR_GPIOF                     (0x14200140)
#define HwADDR_GPIOG                     (0x14200180)
#define HwADDR_GPIOSD0                   (0x14200280)
#define HwADDR_GPIOH                     (0x14200400)

#define TCC_PMU_BASE		0x14400000

#define TCC_PMU_BASE_MICOM			0x1B936000
#define TCC_PMU_GPIOA_SEL_OFFSET	0x148
#define TCC_PMU_GPIOB_SEL_OFFSET	0x14C
#define TCC_PMU_GPIOC_SEL_OFFSET	0x150
#define TCC_PMU_GPIOE_SEL_OFFSET	0x154
#define TCC_PMU_GPIOG_SEL_OFFSET	0x158
#define TCC_PMU_GPIOH_SEL_OFFSET	0x15C
#define TCC_PMU_GPIOMA_SEL_OFFSET	0x160

#define TCC_SMU_CFG_BASE	0x14600000

/* Part 3. Graphic */
#define TCC_GPU3D_BASE		0x10000000
#define TCC_GPU2D_BASE		0x1A000000


/* Part 4. Memory */
#define TCC_MBUSCFG_BASE	0x13500000
#define TCC_HwMMU_BASE		0x13000000


/* Part 5. IO */
#define TCC_ADC_BASE		0x16053000
#define TCC_EDI_BASE		0x16430000
#define TCC_UART_BASE		0x1B200000 //checked
#define TCC_UDMA_BASE		0x1B250000
#define TCC_GDMA_BASE		(0x1B600000UL)
#define TCC_I2C_BASE		0x1B300000
#define TCC_GPSB_BASE		0x1B100000
#define TCC_IOBUSCFG_BASE	0x16051000
#define TCC_IOBUSCFG1_BASE	0x16494400
//#define TCC_ICTC_BASE       0x16055000

//PWM
#define TCC_PWM_BASE				(0x1B400000)
#define TCC_PWM_MODULE0_BASE		(0x1B400000)
#define TCC_PWM_MODULE1_BASE		(0x1B410000)
#define TCC_PWM_MODULE2_BASE		(0x1B420000)
#define TCC_PWM_PORTCFG_BASE		(0x1B430000)
#define TCC_PWM_MODULEx_BASE(X)		(TCC_PWM_BASE + ((X)*0x10000))

//ADMA, DAI
#define HwAUDIO2_ADMA_BASE				(0x16700000)
#define HwAUDIO2_DAI_BASE					(0x16701000)

//ADMA, DAI
#define HwAUDIO1_ADMA_BASE				(0x16200000)
#define HwAUDIO1_DAI_BASE					(0x16201000)

//ADMA, DAI, CDIF, SPDIF_TX
#define HwAUDIO3_ADMA_BASE				(0x16800000)
#define HwAUDIO3_DAI_BASE					(0x16801000)
#define HwAUDIO3_CDIF_BASE				(0x16801080)
#define HwAUDIO3_SPDIFTX_BASE				(0x16802000)

//MC_ADMA, MC_DAI, MC_CDIF, MC_SPDIF_TX, MC_SPDIF_RX
#define HwAUDIO0_ADMA_BASE				(0x16100000)
#define HwAUDIO0_DAI_BASE					(0x16101000)
#define HwAUDIO0_CDIF_BASE				(0x16101080)
#define HwAUDIO0_SPDIFTX_BASE				(0x16102000)
#define HwAUDIO0_SPDIFRX_BASE				(0x16102800)

#define HwADDR_A0_PCFG                      (0x16103000)
#define HwADDR_A1_PCFG                      (0x16203000)
#define HwADDR_A2_PCFG                      (0x16703000)
#define HwADDR_A3_PCFG                      (0x16803000)


/* Part 6. High Speed H/O */

#define TCC_HSIOBUSCFG_BASE	0x119A0000


/* Part 7. Display */

#define TCC_DBUSCFG_BASE	0x12380000


/* Part 8. Video */

#define TCC_VBUSCFG_BASE	0x15100000


/* Part 10. CPU */
#define TCC_CPUBUSCFG_BASE	0x17000000
//#define TCC_CPU_WDT0_BASE	0x17010000
//#define TCC_CPU_WDT4_BASE	0x17050000
#define TCC_CPU0_BASE		0x17100000
#define TCC_CPU1_BASE		0x17200000
#define TCC_GIC_DIST_BASE	0x17F01000
#define TCC_GIC_CPU_BASE	0x17F02000

#define TCC_GIC_CDIST_BASE	0x17301000
#define TCC_GIC_CCPU_BASE	0x17302000

/* Part 11. MICOM */
#define TCC_ICTC_BASE       0x1B500000

#endif /* __BSP_REG_PHYS__H__ */
