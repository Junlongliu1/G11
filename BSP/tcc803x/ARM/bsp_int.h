/*
*********************************************************************************************************
*   FileName    : bsp_int.h
*   Description :
*********************************************************************************************************
*
*   TCC Version : 1.0
*   Copyright (c) Telechips Inc.
*   All rights reserved
*
* This source code contains confidential information of Telechips.
* Any unauthorized use without a written permission of Telechips including not limited to re-
* distribution in source or binary form is strictly prohibited.
* This source code is provided "AS IS" and nothing contained in this source code shall
* constitute any express or implied warranty of any kind, including without limitation, any warranty
* of merchantability, fitness for a particular purpose or non-infringement of any patent, copyright
* or other third party intellectual property right. No warranty is made, express or implied,
* regarding the information's accuracy, completeness, or performance.
* In no event shall Telechips be liable for any claim, damages or other liability arising from, out of
* or in connection with this source code or the use in the source code.
* This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
* between Telechips and Company.
*********************************************************************************************************
*/

#ifndef  BSP_INT_PRESENT
#define  BSP_INT_PRESENT

/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/
//----------------------------------------------------------------------------------------------------------------------------
// Define 
//----------------------------------------------------------------------------------------------------------------------------
#define NOF_MC_IRQ_SGI      16          // Software Generated  Interrupts
#define NOF_MC_IRQ_PPI      16          // Priviate Peripheral Interrupts
#define NOF_MC_IRQ_SPI      224         // Shared   Peripheral Interrupts
#define NOF_MC_IRQ          (NOF_MC_IRQ_SGI + NOF_MC_IRQ_PPI + NOF_MC_IRQ_SPI)

#define NOF_GIC_REG_1BIT    8
#define NOF_GIC_REG_2BIT    16
#define NOF_GIC_REG_4BIT    32
#define NOF_GIC_REG_8BIT    64

//----------------------------------------------------------------------------------------------------------------------------
// GIC400 distributor register structure
//----------------------------------------------------------------------------------------------------------------------------
// 0x1000           : GICD_CTLR                 , RW, 0x00000000    Distrubutor control register
// 0x1004           : GICD_TYPER                , RO, -             Interrupt controller type register
// 0x1008           : GICD_IIDR                 , RO, 0x0200143B    Distributor implementer identification register
// 0x1080 ~ 0x10BC  : GICD_IGROUPRn             , RW, 0x00000000    Interrupt group registers
// 0x1100 ~ 0x113C  : GICD_ISENABLERn           , RW, 0x00000000    Interrupt set-enable regsiters
// 0x1180 ~ 0x11BC  : GICD_ICENABLERn           , RW, 0x00000000    Interrupt clear-enable regsiters
// 0x1200 ~ 0x123C  : GICD_ISPENDRn             , RW, 0x00000000    Interrupt set-pending regsiters
// 0x1280 ~ 0x12BC  : GICD_ICPENDRn             , RW, 0x00000000    Interrupt clear-enable regsiters
// 0x1300 ~ 0x133C  : GICD_ISACTIVERn           , RW, 0x00000000    Interrupt set-active regsiters
// 0x1380 ~ 0x13BC  : GICD_ICACTIVERn           , RW, 0x00000000    Interrupt clear-active regsiters
// 0x1400 ~ 0x15FC  : GICD_IPRIORITYRn          , RW, 0x00000000    Interrupt priority registers
// 0x1800 ~ 0x19FC  : GICD_ITARGETSRn           , RW, 0x00000000    Interrupt processor targets regsiters
// 0x1C00 ~ 0x1C7C  : GICD_ICFGRn               , RW, 0x55555555    Interrupt configuration registers
// 0x1D00           : GICD_PPISR                , RO, 0x00000000    Private peripheral interrupt status register
// 0x1F00           : GICD_SGIR                 , WO, -             Software generated interrupt register
// 0x1F10 ~ 0x1F1C  : GICD_CPENDSGIRn           , WR, 0x00000000    SGI clear-pending registers
// 0x1F20 ~ 0x1F2C  : GICD_SPENDSGIRn           , WR, 0x00000000    SGI set-pending registers
#if 0
// MC_GICD_CTLR_U
typedef struct {
    uint32            EnableGrp0          :  1;   // [   00], RW
    uint32            EnableGrp1          :  1;   // [   01], RW
    uint32                                : 30;   // [31:02]
} MC_GICD_CTLR;

typedef union {
    uint32            nReg;
    MC_GICD_CTLR    bReg;
} MC_GICD_CTLR_U;

// MC_GICD_TYPER_U
typedef struct {
    uint32            ITLinesNumber       :  5;   // [04:00]
    uint32            CPUNumber           :  3;   // [07:05]
    uint32                                :  2;   // [09:08]
    uint32            SecurityExtn        :  1;   // [10]
    uint32            LSPI                :  5;   // [15:11]
    uint32                                : 16;   // [31:16]
} MC_GICD_TYPER;

typedef union {
    uint32            nReg;
    MC_GICD_TYPER   bReg;
} MC_GICD_TYPER_U;

// MC_GICD_IIDR_U
typedef struct {
    uint32            Implementer         : 12;   // [11:00]
    uint32            Revision            :  4;   // [15:12]
    uint32            Variant             :  4;   // [19:16]
    uint32                                :  4;   // [23:20]
    uint32            ProductID           :  8;   // [31:24]
} MC_GICD_IIDR;

typedef union {
    uint32            nReg;
    MC_GICD_IIDR    bReg;
} MC_GICD_IIDR_U;

// MC_GICD_1BIT_GRP_U
typedef struct {
    uint32            nReg[NOF_GIC_REG_1BIT];
} MC_GICD_1BIT_ID_U;

// MC_GICD_2BIT_ID_U
typedef union {
    uint32            nReg[NOF_GIC_REG_2BIT];
} MC_GICD_2BIT_ID_U;

// MC_GICD_4BIT_ID_U
typedef union {
    uint32            nReg[NOF_GIC_REG_4BIT];
} MC_GICD_4BIT_ID_U;

// MC_GICD_8BIT_ID_U
typedef union {
    uint32            nReg[NOF_GIC_REG_8BIT];
} MC_GICD_8BIT_ID_U;

// MC_GICD_SGIR_U
typedef struct {
    uint32            SGIINTID            :  4;   // [03:00]
    uint32                                : 11;   // [14:04]
    uint32            SATT                :  1;   // [   15]
    uint32            CPUTargetList       :  8;   // [23:16]
    uint32            TargetListFilter    :  2;   // [25:24]
    uint32                                :  6;   // [31:26]
} MC_GICD_SGIR_S;
typedef union {
    uint32            nReg;
    MC_GICD_SGIR_S  bReg;
} MC_GICD_SGIR_U;
//----------------------------------------------------------------------------------------------------------------------------
// GIC400 Distributor register structure
//----------------------------------------------------------------------------------------------------------------------------

typedef struct {
    MC_GICD_CTLR_U      uGICD_CTLR;                         // 0x000        
    MC_GICD_TYPER_U     uGICD_TYPER;                        // 0x004        
    MC_GICD_IIDR_U      uGICD_IIDR;                         // 0x008        
    uint32                reserved_00c[(0x080-0x00c)/4];      // 0x00c ~ 0x07c

    MC_GICD_1BIT_ID_U   uGICD_IGROUPR;                  // 0x080 ~ 0x09c
    uint32                reserved_0a0[(0x100-0x0a0)/4];  // 0x0a0 ~ 0x0fc

    MC_GICD_1BIT_ID_U   uGICD_ISENABLER;                // 0x100 ~ 0x11c
    uint32                reserved_120[(0x180-0x120)/4];  // 0x120 ~ 0x17c

    MC_GICD_1BIT_ID_U   uGICD_ICENABLER;                // 0x180 ~ 0x19c
    uint32                reserved_1a0[(0x200-0x1a0)/4];  // 0x1a0 ~ 0x1fc

    MC_GICD_1BIT_ID_U   uGICD_ISPENDR;                  // 0x200 ~ 0x21c
    uint32                reserved_220[(0x280-0x220)/4];  // 0x220 ~ 0x27c

    MC_GICD_1BIT_ID_U   uGICD_ICPENDR;                  // 0x280 ~ 0x29c
    uint32                reserved_2a0[(0x300-0x2a0)/4];  // 0x2a0 ~ 0x2fc

    MC_GICD_1BIT_ID_U   uGICD_ISACTIVER;                // 0x300 ~ 0x31c
    uint32                reserved_320[(0x380-0x320)/4];  // 0x320 ~ 0x37c

    MC_GICD_1BIT_ID_U   uGICD_ICACTIVER;                // 0x380 ~ 0x39c
    uint32                reserved_3a0[(0x400-0x3a0)/4];  // 0x3a0 ~ 0x3fc

    MC_GICD_8BIT_ID_U   uGICD_IPRIORITYR;               // 0x400 ~ 0x4fc
    uint32                reserved_500[(0x800-0x500)/4];  // 0x500 ~ 0x7fc

    MC_GICD_8BIT_ID_U   uGICD_ITARGETSR;                // 0x800 ~ 0x8fc
    uint32                reserved_900[(0xc00-0x900)/4];  // 0x900 ~ 0xbfc

    MC_GICD_2BIT_ID_U   uGICD_ICFGR;                    // 0xc00 ~ 0xc3c
    uint32                reserved_c40[(0xf00-0xc40)/4];  // 0xc40 ~ 0xefc

    MC_GICD_SGIR_U      uGICDSGIR;                      // 0xf00
} MC_GICD;
#endif

//----------------------------------------------------------------------------------------------------------------------------
// GIC400 CPU I/F register structure
//----------------------------------------------------------------------------------------------------------------------------
// 0x2000           : GICC_CTLR                 , RW, 0x00000000    CPU interface control register
// 0x2004           : GICC_PMR                  , RW, 0x00000000    Interrupt priority mask register
// 0x2008           : GICC_BPR                  , RW, 0x00000000    Binary pointer register
// 0x200C           : GICC_IAR                  , RW, 0x00000000    Interrupt acknowledge register
// 0x2010           : GICC_EOIR                 , RW, 0x00000000    End of interrupt register
// 0x2014           : GICC_RPR                  , RW, 0x00000000    Running priority register
// 0x2018           : GICC_HPPIR                , RW, 0x00000000    Highest priority pending interrupt register
#if 0
// MC_GICC_CTLR_U
typedef struct {
    uint32            EnableGrp0          :  1;   // [   00]
    uint32            EnableGrp1          :  1;   // [   01]
    uint32            AckCtl              :  1;   // [   02]
    uint32            FIQEn               :  1;   // [   03]
    uint32            CBPR                :  1;   // [   04]
    uint32            FIQBypDisGrp0       :  1;   // [   05]
    uint32            IRQBypDisGrp0       :  1;   // [   06]
    uint32            FIQBypDisGrp1       :  1;   // [   07]
    uint32            IRQBypDisGrp1       :  1;   // [   08]
    uint32            EOImodeS            :  1;   // [   09]
    uint32            EOImodeNS           :  1;   // [   10]
    uint32                                : 21;   // [31:11]
} MC_GICC_CTLR;

typedef union {
    uint32            nReg;
    MC_GICC_CTLR    bReg;
} MC_GICC_CTLR_U;

// MC_GICC_PMR_U
typedef struct {
    uint32            Priority            :  8;   // [07:00]
    uint32                                : 24;   // [31:08]
} MC_GICC_PMR;

typedef union {
    uint32            nReg;
    MC_GICC_PMR     bReg;
} MC_GICC_PMR_U;

// MC_GICC_BPR_U
typedef struct {
    uint32            BinaryPoint         :  3;   // [02:00]
    uint32                                : 29;   // [31:03]
} MC_GICC_BPR;

typedef union {
    uint32            nReg;
    MC_GICC_BPR     bReg;
} MC_GICC_BPR_U;

// MC_GICC_IAR_U
typedef struct {
    uint32            InterruptID         : 10;   // [09:00]
    uint32            CPUID               :  3;   // [12:10]
    uint32                                : 19;   // [31:13]
} MC_GICC_IAR;

typedef union {
    uint32            nReg;
    MC_GICC_IAR     bReg;
} MC_GICC_IAR_U;

// MC_GICC_EOIR_U
typedef struct {
    uint32            EOIINTID            : 10;   // [09:00]
    uint32            CPUID               :  3;   // [12:10]
    uint32                                : 19;   // [31:13]
} MC_GICC_EOIR;

typedef union {
    uint32            nReg;
    MC_GICC_EOIR    bReg;
} MC_GICC_EOIR_U;

// MC_GICC_RPR_U
typedef struct {
    uint32            Priority            :  8;   // [07:00]
    uint32                                : 24;   // [31:08]
} MC_GICC_RPR;

typedef union {
    uint32            nReg;
    MC_GICC_RPR     bReg;
} MC_GICC_RPR_U;

// MC_GICC_HPPIR_U
typedef struct {
    uint32            PENDINTID           :  10;  // [09:00]
    uint32            CPUID               :   3;  // [12:10]
    uint32                                :  19;  // [31:13]
} MC_GICC_HPPIR;

typedef union {
    uint32            nReg;
    MC_GICC_HPPIR   bReg;
} MC_GICC_HPPIR_U;

//----------------------------------------------------------------------------------------------------------------------------
// GIC400 CPU I/F register structure
//----------------------------------------------------------------------------------------------------------------------------
typedef struct {
    MC_GICC_CTLR_U      uGICC_CTLR;     // 0x000
    MC_GICC_PMR_U       uGICC_PMR;      // 0x004
    MC_GICC_BPR_U       uGICC_BPR;      // 0x008
    MC_GICC_IAR_U       uGICC_IAR;      // 0x00c
    MC_GICC_EOIR_U      uGICC_EOIR;     // 0x010
    MC_GICC_RPR_U       uGICC_RPR;      // 0x014
    MC_GICC_HPPIR_U     uGICC_HPPIR;    // 0x018
} MC_GICC;
#endif

typedef  void (*BSP_INT_FNCT_PTR)(void *);

#define INT_TYPE_LEVEL_HIGH     0x00000001u
#define INT_TYPE_LEVEL_LOW      0x00000002u
#define INT_TYPE_EDGE_RISING    0x00000004u
#define INT_TYPE_EDGE_FALLING   0x00000008u
#define INT_TYPE_EDGE_BOTH      (INT_TYPE_EDGE_RISING|INT_TYPE_EDGE_FALLING)

#define PRIORITY_NO_MEAN        16u

#define GIC_PPI_START           16u
#define GIC_SPI_START           32u


#define PPI_VIRT_TIMER          (GIC_PPI_START + 11)

/*
Software Generated Interrupt (SGI) This interrupt is generated explicitly by software by writing to a dedicated distributor register, 
the Software Generated Interrupt Register. It is most commonly used for inter-core communication. 
SGIs can be targeted at all, or at a selected group of cores in the system. Interrupt numbers 0-15 are reserved for this. 
The software manages the exact interrupt number used for communication.

Private Peripheral Interrupt (PPI) This interrupt is generated by a peripheral that is private to an individual core. Interrupt numbers 16-31 are reserved for this. 
PPIs identify interrupt sources private to the core, and are independent of the same source on another core, for example, per-core timer.

Shared Peripheral Interrupt (SPI) This interrupt is generated by a peripheral that the Interrupt Controller can route to more than one core. 
Interrupt numbers 32-1020 are used for this. SPIs are used to signal interrupts from various peripherals accessible across the whole system.
*/

// PPI(Private Peripheral Interrupt) number assign
typedef enum                                              
{                   
	/*Software Generated Interrupt (SGI) */
    INT_PPI_SWI_0                = 0,              
    INT_PPI_SWI_1                = 1,              
    INT_PPI_SWI_2                = 2,              
    INT_PPI_SWI_3                = 3,              
    INT_PPI_SWI_4                = 4,              
    INT_PPI_SWI_5                = 5,              
    INT_PPI_SWI_6                = 6,              
    INT_PPI_SWI_7                = 7,              
    INT_PPI_SWI_8                = 8,              
    INT_PPI_SWI_9                = 9,              
    INT_PPI_SWI_10               = 10,             
    INT_PPI_SWI_11               = 11,             
    INT_PPI_SWI_12               = 12,             
    INT_PPI_SWI_13               = 13,             
    INT_PPI_SWI_14               = 14,             
    INT_PPI_SWI_15               = 15,       
    /*Private Peripheral Interrupt (PPI)*/
    INT_PPI_PPI0_RESERVED        = 16,             
    INT_PPI_PPI1_RESERVED        = 17,             
    INT_PPI_PPI2_RESERVED        = 18,             
    INT_PPI_PPI3_RESERVED        = 19,             
    INT_PPI_PPI4_RESERVED        = 20,             
    INT_PPI_PPI5_RESERVED        = 21,             
    INT_PPI_PPI6_RESERVED        = 22,             
    INT_PPI_PPI7_RESERVED        = 23,             
    INT_PPI_PPI8_RESERVED        = 24,             
    INT_PPI_PPI9_VIRT_MAINT      = 25,             
    INT_PPI_PPI10_HYP_TIMER      = 26,             
    INT_PPI_PPI11_VIRT_TIMER     = 27,             
    INT_PPI_PPI12_nFIG           = 28,             
    INT_PPI_PPI13_SEC_TIMER      = 29,             
    INT_PPI_PPI14_NONSEC_TIMER   = 30,             
    INT_PPI_PPI15_nIRQ           = 31              
}BST_INT_PPI_SRC_TYPE;                                                

/* 2.2.2 Interrupt Source Signals */
typedef enum
{
    INT_CAN                  = (GIC_SPI_START + 0),    
    INT_GPSB               = (GIC_SPI_START + 1),    
    INT_GPSB1               = (GIC_SPI_START + 2),    
    INT_GPSB2               = (GIC_SPI_START + 3),    
    INT_GPSB0_DMA           = (GIC_SPI_START + 4),    
    INT_GPSB1_DMA           = (GIC_SPI_START + 5),    
    INT_GPSB2_DMA           = (GIC_SPI_START + 6),    
    INT_UART0               = (GIC_SPI_START + 7),    
    INT_UART1               = (GIC_SPI_START + 8),    
    INT_UART2               = (GIC_SPI_START + 9),    
    INT_UART3               = (GIC_SPI_START + 10),   
    INT_UT_DMA0           = (GIC_SPI_START + 11),   
    INT_UT_DMA1           = (GIC_SPI_START + 12),   
    INT_UT_DMA2           = (GIC_SPI_START + 13),   
    INT_UT_DMA3           = (GIC_SPI_START + 14),   
    INT_I2C               = (GIC_SPI_START + 15),   
    INT_I2C1               = (GIC_SPI_START + 16),   
    INT_I2C2               = (GIC_SPI_START + 17),   
    INT_IC_TC0              = (GIC_SPI_START + 18),   
    INT_IC_TC1              = (GIC_SPI_START + 19),   
    INT_IC_TC2              = (GIC_SPI_START + 20),   
    INT_DMA_PL080            = (GIC_SPI_START + 21),   
    INT_ADC                  = (GIC_SPI_START + 22),   
    INT_TIMER_0              = (GIC_SPI_START + 23),   
    INT_TIMER_1              = (GIC_SPI_START + 24),   
    INT_TIMER_2              = (GIC_SPI_START + 25),   
    INT_TIMER_3              = (GIC_SPI_START + 26),   
    INT_TIMER_4              = (GIC_SPI_START + 27),   
    INT_TIMER_5              = (GIC_SPI_START + 28),   
    INT_WATCHDOG             = (GIC_SPI_START + 29),   
    INT_DEFAULT_SLV_ERR      = (GIC_SPI_START + 30),   
    INT_MBOX0_SLV_S          = (GIC_SPI_START + 31),   
    INT_MBOX0_SLV_NS         = (GIC_SPI_START + 32),   
    INT_MBOX1_SLV_S          = (GIC_SPI_START + 33),   
    INT_MBOX1_SLV_NS         = (GIC_SPI_START + 34),   
    INT_MBOX2_SLV            = (GIC_SPI_START + 35),   
    INT_MBOX0_MST_S          = (GIC_SPI_START + 36),   
    INT_MBOX0_MST_NS         = (GIC_SPI_START + 37),   
    INT_MBOX1_MST_S          = (GIC_SPI_START + 38),   
    INT_MBOX1_MST_NS         = (GIC_SPI_START + 39),   
    INT_MBOX2_MST            = (GIC_SPI_START + 40),   
    INT_SFMC                 = (GIC_SPI_START + 41),   
    INT_NSAID_FILTER         = (GIC_SPI_START + 42),   
    INT_SGID_FILTER          = (GIC_SPI_START + 43),   
    INT_SFMC_MID_FILTER      = (GIC_SPI_START + 44),   
    INT_IMC_MID_FILTER       = (GIC_SPI_START + 45),   
    INT_EXT0       = (GIC_SPI_START + 46),   
    INT_EXT1       = (GIC_SPI_START + 47),   
    INT_EXT2       = (GIC_SPI_START + 48),   
    INT_EXT3       = (GIC_SPI_START + 49),   
    INT_EXT4       = (GIC_SPI_START + 50),   
    INT_EXT5       = (GIC_SPI_START + 51),   
    INT_EXT6       = (GIC_SPI_START + 52),   
    INT_EXT7       = (GIC_SPI_START + 53),   
    INT_EXT8       = (GIC_SPI_START + 54),   
    INT_EXT9       = (GIC_SPI_START + 55),   
    INT_EXTn0      = (GIC_SPI_START + 56),   
    INT_EXTn1      = (GIC_SPI_START + 57),   
    INT_EXTn2      = (GIC_SPI_START + 58),   
    INT_EXTn3      = (GIC_SPI_START + 59),   
    INT_EXTn4      = (GIC_SPI_START + 60),   
    INT_EXTn5      = (GIC_SPI_START + 61),   
    INT_EXTn6      = (GIC_SPI_START + 62),   
    INT_EXTn7      = (GIC_SPI_START + 63),   
    INT_EXTn8      = (GIC_SPI_START + 64),   
    INT_EXTn9      = (GIC_SPI_START + 65),   
    INT_CR5_PMU              = (GIC_SPI_START + 66),   
    INT_GPSB3                = (GIC_SPI_START + 67),
    INT_GPSB3_DMA            = (GIC_SPI_START + 68),
    INT_RESERVED_69          = (GIC_SPI_START + 69),   
    INT_RESERVED_70          = (GIC_SPI_START + 70),   
    INT_RESERVED_71          = (GIC_SPI_START + 71),   
    INT_RESERVED_72          = (GIC_SPI_START + 72),   
    INT_RESERVED_73          = (GIC_SPI_START + 73),   
    INT_RESERVED_74          = (GIC_SPI_START + 74),   
    INT_FMU_IRQ          = (GIC_SPI_START + 75),   
    INT_FMU_FIQ          = (GIC_SPI_START + 76),  
    INT_RESERVED_77          = (GIC_SPI_START + 77),   
    INT_RESERVED_78          = (GIC_SPI_START + 78),   
    INT_RESERVED_79          = (GIC_SPI_START + 79),   
    INT_PMGPIO                 = (GIC_SPI_START + 80),
    INT_PMU_WDT                 = (GIC_SPI_START + 207),
    INT_CBUS_WDT                 = (GIC_SPI_START + 205),
    INT_SRC_CNT                 = (GIC_SPI_START + NOF_MC_IRQ_SPI)   // determine the array size of BSP_IntVectTbl[] 

} BST_INT_SRC_TYPE;

#define INT_CB_WDT0	 INT_WATCHDOG // for core compatibility
#define INT_MBOX1 	INT_MBOX0_SLV_NS // for core compatibility
#define EINT_START_INT  INT_EXT0
#define EINT_END_INT    INT_EXT9
#define EINT_NUM 		(INT_EXTn0 - INT_EXT0)

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        BSP_GICPreInit     (void);

void        BSP_Int_Init        (void);

void        BSP_IntSrcEn        (uint32        int_id);

void        BSP_IntSrcDis       (uint32        int_id);

void        BSP_IntPrioMaskSet  (uint32        prio);

void        BSP_IntPrioSet(uint32 int_id, uint32  prio);

uint8 BSP_IntVectSet      (uint32        int_id,
                                 uint32        int_prio,
                                 uint8        int_type,
                                 BSP_INT_FNCT_PTR  int_fnct,
                                 void          *int_arg);

void        BSP_IntHandlerWithParam (uint32 ulICCIAR );
void        BSP_IntHandler      (void);

uint8 BSP_IntExtSet      (uint32        int_id,
	                         uint32 gpio);


/*
*********************************************************************************************************
*                                              ERROR CHECKING
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
