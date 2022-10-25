#ifndef __BSP_ARM_MPU__H__
#define __BSP_ARM_MPU__H__

#define MPU_EN

#define REGION_1K       0x00000009U
#define REGION_2K       0x0000000AU
#define REGION_4K       0x0000000BU
#define REGION_8K       0x0000000CU
#define REGION_16K      0x0000000DU
#define REGION_32K      0x0000000EU
#define REGION_64K      0x0000000FU
#define REGION_128K     0x00000010U
#define REGION_256K     0x00000011U
#define REGION_1M       0x00000013U
#define REGION_2M       0x00000014U
#define REGION_4M       0x00000015U
#define REGION_8M       0x00000016U
#define REGION_16M      0x00000017U
#define REGION_32M      0x00000018U
#define REGION_64M      0x00000019U
#define REGION_128M     0x0000001AU
#define REGION_256M     0x0000001BU
#define REGION_512M     0x0000001CU
#define REGION_1G       0x0000001DU
#define REGION_2G       0x0000001EU
#define REGION_4G       0x0000001FU

#define REGION_EN       0x00000001U

/* GPRs */
#define ARM_GPR0        r0
#define ARM_GPR1        r1
#define ARM_GPR2        r2
#define ARM_GPR3        r3
#define ARM_GPR4        r4
#define ARM_GPR5        r5
#define ARM_GPR6        r6
#define ARM_GPR7        r7
#define ARM_GPR8        r8
#define ARM_GPR9        r9
#define ARM_GPR10       r10
#define ARM_GPR11       r11
#define ARM_GPR12       r12
#define ARM_GPR13       r13
#define ARM_GPR14       r14
#define ARM_GPR15       r15
#define ARM_CPSR        cpsr

/* Coprocessor number defines */
#define ARM_CP0         0
#define ARM_CP1         1
#define ARM_CP2         2
#define ARM_CP3         3
#define ARM_CP4         4
#define ARM_CP5         5
#define ARM_CP6         6
#define ARM_CP7         7
#define ARM_CP8         8
#define ARM_CP9         9
#define ARM_CP10        10
#define ARM_CP11        11
#define ARM_CP12        12
#define ARM_CP13        13
#define ARM_CP14        14
#define ARM_CP15        15

/* Coprocessor control register defines */
#define ARM_CR0         cr0
#define ARM_CR1         cr1
#define ARM_CR2         cr2
#define ARM_CR3         cr3
#define ARM_CR4         cr4
#define ARM_CR5         cr5
#define ARM_CR6         cr6
#define ARM_CR7         cr7
#define ARM_CR8         cr8
#define ARM_CR9         cr9
#define ARM_CR10		cr10
#define ARM_CR11		cr11
#define ARM_CR12		cr12
#define ARM_CR13		cr13
#define ARM_CR14		cr14
#define ARM_CR15		cr15

/* Current Processor Status Register (CPSR) Bits */
#define CPSR_THUMB_MODE         0x20U
#define CPSR_MODE_BITS          0x1FU
#define CPSR_SYSTEM_MODE        0x1FU
#define CPSR_UNDEFINED_MODE     0x1BU
#define CPSR_DATA_ABORT_MODE    0x17U
#define CPSR_SVC_MODE           0x13U
#define CPSR_IRQ_MODE           0x12U
#define CPSR_FIQ_MODE           0x11U
#define CPSR_USER_MODE          0x10U

#define CPSR_IRQ_ENABLE         0x80U
#define CPSR_FIQ_ENABLE         0x40U

#define CPSR_N_BIT              0x80000000U
#define CPSR_Z_BIT              0x40000000U
#define CPSR_C_BIT              0x20000000U
#define CPSR_V_BIT              0x10000000U



#define SHAREABLE               0x00000004U //shareable
#define STRONG_ORDERD_SHARED    0x00000000U	//strongly ordered, always shareable

#define DEVICE_SHARED           0x00000001U	//device, shareable
#define DEVICE_NONSHARED        0x00000010U	//device, non shareable

#define NORM_NSHARED_WT_NWA     0x00000002U //Outer and Inner write-through, no write-allocate non-shareable
#define NORM_SHARED_WT_NWA      0x00000006U //Outer and Inner write-through, no write-allocate shareable

#define NORM_NSHARED_WB_NWA     0x00000003U //Outer and Inner write-back, no write-allocate non shareable
#define NORM_SHARED_WB_NWA      0x00000007U //Outer and Inner write-back, no write-allocate shareable

#define NORM_NSHARED_NCACHE     0x00000008U //Outer and Inner Non cacheable  non shareable
#define NORM_SHARED_NCACHE      0x0000000CU //Outer and Inner Non cacheable shareable

#define NORM_NSHARED_WB_WA      0x0000000BU //Outer and Inner write-back non shared
#define NORM_SHARED_WB_WA       0x0000000FU //Outer and Inner write-back shared

/* inner and outer cache policies can be combined for different combinations */

#define NORM_IN_POLICY_NCACHE   0x00000020U	//inner non cacheable
#define NORM_IN_POLICY_WB_WA    0x00000021U	//inner write back write allocate
#define NORM_IN_POLICY_WT_NWA   0x00000022U	//inner write through no write allocate
#define NORM_IN_POLICY_WB_NWA   0x00000023U	//inner write back no write allocate

#define NORM_OUT_POLICY_NCACHE  0x00000020U	//outer non cacheable
#define NORM_OUT_POLICY_WB_WA   0x00000028U	//outer write back write allocate
#define NORM_OUT_POLICY_WT_NWA  0x00000030U	//outer write through no write allocate
#define NORM_OUT_POLICY_WB_NWA  0x00000038U	//outer write back no write allocate

#define NO_ACCESS               (0x00000000U<<8U) //No access
#define PRIV_RW_USER_NA         (0x00000001U<<8U) //Privileged access only
#define PRIV_RW_USER_RO         (0x00000002U<<8U) //Writes in User mode generate permission faults
#define PRIV_RW_USER_RW         (0x00000003U<<8U) //Full Access
#define PRIV_RO_USER_NA         (0x00000005U<<8U) //Privileged eead only
#define PRIV_RO_USER_RO         (0x00000006U<<8U) //Privileged/User read-only

#define EXECUTE_NEVER           (0x00000001U<<12U) //Bit 12


/* CP15 defines */
/* C0 Register defines */
#ifdef __GNU_C__
#define ARM_CP15_MAIN_ID              "p15, 0, %0,  c0,  c0, 0"
#define ARM_CP15_CACHE_TYPE           "p15, 0, %0,  c0,  c0, 1"
#define ARM_CP15_TCM_TYPE             "p15, 0, %0,  c0,  c0, 2"
#define ARM_CP15_TLB_TYPE             "p15, 0, %0,  c0,  c0, 3"
#define ARM_CP15_MPU_TYPE             "p15, 0, %0,  c0,  c0, 4"
#define ARM_CP15_MULTI_PROC_AFFINITY  "p15, 0, %0,  c0,  c0, 5"
#define ARM_CP15_PROC_FEATURE_0       "p15, 0, %0,  c0,  c1, 0"
#define ARM_CP15_PROC_FEATURE_1       "p15, 0, %0,  c0,  c1, 1"
#define ARM_CP15_DEBUG_FEATURE_0      "p15, 0, %0,  c0,  c1, 2"

#define ARM_CP15_CACHE_SIZE_ID        "p15, 1, %0,  c0,  c0, 0"
#define ARM_CP15_CACHE_LEVEL_ID       "p15, 1, %0,  c0,  c0, 1"
#define ARM_CP15_AUXILARY_ID          "p15, 1, %0,  c0,  c0, 7"

#define ARM_CP15_CACHE_SIZE_SEL       "p15, 2, %0,  c0,  c0, 0"
/* C1 Register Defines */
#define ARM_CP15_SYS_CONTROL          "p15, 0, %0,  c1,  c0, 0"
#define ARM_CP15_AUX_CONTROL          "p15, 0, %0,  c1,  c0, 1"
#define ARM_CP15_CP_ACCESS_CONTROL    "p15, 0, %0,  c1,  c0, 2"
#else
//#include <arm_ghs.h> //QAC modify
extern uint32 __MRC(int32 coproc, int32 opcode1,
	int32 CRn, int32 CRm, int32 opcode2);
extern void __MCR(int32 coproc, int32 opcode1, uint32 Rd,
	int32 CRn, int32 CRm, int32 opcode2);


#define ARM_CP15_MAIN_ID                15,0,0,0,0
#define ARM_CP15_CACHE_TYPE             15,0,0,0,1
#define ARM_CP15_TCM_TYPE               15,0,0,0,2
#define ARM_CP15_TLB_TYPE               15,0,0,0,3
#define ARM_CP15_MPU_TYPE               15,0,0,0,4
#define ARM_CP15_MULTI_PROC_AFFINITY    15,0,0,0,5
#define ARM_CP15_PROC_FEATURE_0         15,0,0,1,0
#define ARM_CP15_PROC_FEATURE_1         15,0,0,1,1
#define ARM_CP15_DEBUG_FEATURE_0        15,0,0,1,2

#define ARM_CP15_CACHE_SIZE_ID          15,1,0,0,0
#define ARM_CP15_CACHE_LEVEL_ID         15,1,0,0,1
#define ARM_CP15_AUXILARY_ID            15,1,0,0,7

#define ARM_CP15_CACHE_SIZE_SEL         15,2,0,0,0
/* C1 Register Defines */
#define ARM_CP15_SYS_CONTROL            15,0,1,0,0
#define ARM_CP15_AUX_CONTROL            15,0,1,0,1
#define ARM_CP15_CP_ACCESS_CONTROL      15,0,1,0,2
#endif
/* ARM_CP15_CONTROL bit defines */
#define ARM_CP15_CONTROL_TE             0x40000000U //Thumb Execution
#define ARM_CP15_CONTROL_AFE            0x20000000U //Access Flag Enable
#define ARM_CP15_CONTROL_TRE            0x10000000U //TEX remap enable
#define ARM_CP15_CONTROL_NMFI           0x08000000U //Nonmaskable Fast Interrupt Enable
#define ARM_CP15_CONTROL_EE             0x02000000U //Exception endianess
#define ARM_CP15_CONTROL_RR             0x00004000U //Round-robin bit
#define ARM_CP15_CONTROL_V              0x00002000U //Base location of Exception register
#define ARM_CP15_CONTROL_I              0x00001000U //Insturction Cache Enable
#define ARM_CP15_CONTROL_Z              0x00000800U //Branch Prediction Enable
#define ARM_CP15_CONTROL_C              0x00000004U //Enable Data Cache
#define ARM_CP15_CONTROL_A              0x00000002U //Strict Alignment
#define ARM_CP15_CONTROL_M              0x00000001U //MPU enable

#ifdef __GNU_C__
/* C5 */
#define ARM_CP15_DATA_FAULT_STATUS        "p15, 0, %0,  c5,  c0, 0"
#define ARM_CP15_INST_FAULT_STATUS        "p15, 0, %0,  c5,  c0, 1"

#define ARM_CP15_AUX_DATA_FAULT_STATUS    "p15, 0, %0,  c5,  c1, 0"
#define ARM_CP15_AUX_INST_FAULT_STATUS    "p15, 0, %0,  c5,  c1, 1"
/* C6 */
#define ARM_CP15_DATA_FAULT_ADDRESS       "p15, 0, %0,  c6,  c0, 0"
#define ARM_CP15_INST_FAULT_ADDRESS       "p15, 0, %0,  c6,  c0, 2"
#define ARM_CP15_MPU_REG_BASEADDR         "p15, 0, %0,  c6,  c1, 0"
#define ARM_CP15_MPU_REG_SIZE_EN          "p15, 0, %0,  c6,  c1, 2"
#define ARM_CP15_MPU_REG_ACCESS_CTRL      "p15, 0, %0,  c6,  c1, 4"
#define ARM_CP15_MPU_MEMORY_REG_NUMBER    "p15, 0, %0,  c6,  c2, 0"

/* C7 */
#define ARM_CP15_NOP                      "p15, 0, %0,  c7,  c0, 4"
#define ARM_CP15_INVAL_IC_POU             "p15, 0, %0,  c7,  c5, 0"
#define ARM_CP15_INVAL_IC_LINE_MVA_POU    "p15, 0, %0,  c7,  c5, 1"
#define ARM_CP15_INVAL_BTAC               "p15, 0, %0,  c7,  c5, 6"

/* C9 */
#define ARM_CP15_ATCM_REG_SIZE_ADDR       "p15, 0, %0,  c9, c1, 1"
#define ARM_CP15_BTCM_REG_SIZE_ADDR       "p15, 0, %0,  c9, c1, 0"
#define ARM_CP15_TCM_SELECTION            "p15, 0, %0,  c9, c2, 0"
#define ARM_CP15_PERF_MONITOR_CTRL        "p15, 0, %0,  c9, c12, 0"
#define ARM_CP15_COUNT_ENABLE_SET         "p15, 0, %0,  c9, c12, 1"
#define ARM_CP15_COUNT_ENABLE_CLR         "p15, 0, %0,  c9, c12, 2"
#define ARM_CP15_V_FLAG_STATUS            "p15, 0, %0,  c9, c12, 3"
#define ARM_CP15_SW_INC                   "p15, 0, %0,  c9, c12, 4"
#define ARM_CP15_EVENT_CNTR_SEL           "p15, 0, %0,  c9, c12, 5"
#define ARM_CP15_PERF_CYCLE_COUNTER       "p15, 0, %0,  c9, c13, 0"
#define ARM_CP15_EVENT_TYPE_SEL           "p15, 0, %0,  c9, c13, 1"
#define ARM_CP15_PERF_MONITOR_COUNT       "p15, 0, %0,  c9, c13, 2"
#define ARM_CP15_USER_ENABLE              "p15, 0, %0,  c9, c14, 0"
#define ARM_CP15_INTR_ENABLE_SET          "p15, 0, %0,  c9, c14, 1"
#define ARM_CP15_INTR_ENABLE_CLR          "p15, 0, %0,  c9, c14, 2"

/* C15 */
#define ARM_CP15_SEC_AUX_CTRL             "p15, 0, %0, c15,  c0, 0"
#else
/* C5 */
#define ARM_CP15_DATA_FAULT_STATUS        15,0,5,0,0
#define ARM_CP15_INST_FAULT_STATUS        15,0,5,0,1

#define ARM_CP15_AUX_DATA_FAULT_STATUS    15,0,5,1,0
#define ARM_CP15_AUX_INST_FAULT_STATUS    15,0,5,1,1
/* C6 */
#define ARM_CP15_DATA_FAULT_ADDRESS       15,0,6,0,0
#define ARM_CP15_INST_FAULT_ADDRESS       15,0,6,0,2
#define ARM_CP15_MPU_REG_BASEADDR         15,0,6,1,0
#define ARM_CP15_MPU_REG_SIZE_EN          15,0,6,1,2
#define ARM_CP15_MPU_REG_ACCESS_CTRL      15,0,6,1,4
#define ARM_CP15_MPU_MEMORY_REG_NUMBER    15,0,6,2,0

/* C7 */
#define ARM_CP15_NOP                      15,0,7,0,4
#define ARM_CP15_INVAL_IC_POU             15,0,7,5,0
#define ARM_CP15_INVAL_IC_LINE_MVA_POU    15,0,7,5,1
#define ARM_CP15_INVAL_BTAC               15,0,7,5,6

/* C9 */
#define ARM_CP15_ATCM_REG_SIZE_ADDR       15,0,9,1,1
#define ARM_CP15_BTCM_REG_SIZE_ADDR       15,0,9,1,0
#define ARM_CP15_TCM_SELECTION            15,0,9,2,0
#define ARM_CP15_PERF_MONITOR_CTRL        15,0,9,12,
#define ARM_CP15_COUNT_ENABLE_SET         15,0,9,12,
#define ARM_CP15_COUNT_ENABLE_CLR         15,0,9,12,
#define ARM_CP15_V_FLAG_STATUS            15,0,9,12,
#define ARM_CP15_SW_INC                   15,0,9,12,
#define ARM_CP15_EVENT_CNTR_SEL           15,0,9,12,
#define ARM_CP15_PERF_CYCLE_COUNTER       15,0,9,13,
#define ARM_CP15_EVENT_TYPE_SEL           15,0,9,13,
#define ARM_CP15_PERF_MONITOR_COUNT       15,0,9,13,
#define ARM_CP15_USER_ENABLE              15,0,9,14,
#define ARM_CP15_INTR_ENABLE_SET          15,0,9,14,
#define ARM_CP15_INTR_ENABLE_CLR          15,0,9,14,

/* C15 */
#define ARM_CP15_SEC_AUX_CTRL             15,0,15,0,0
#endif

/* CP15 operations */
//Write Control Register configuration data
#ifdef __GNU_C__
#define mmcr(rn, v)	__asm__ __volatile__("mcr " rn : : "r" (v));
#else
#define _MCR(coproc, opc1, CRn, CRm, opc2, value)  do { \
						__MCR(coproc, opc1, value, CRn, CRm, opc2); \
						}while(0)
#define mmcr(reg, value)    _MCR(reg, value)
#endif


//Read Control Register configuration data
#ifdef __GNU_C__
#define mmrc(rn)	({uint32  rval = 0U;\
                __asm__ __volatile__("mrc " rn : "=r" (rval));\
                rval;})
#else
#define mmrc(rn) __MRC(rn)
#endif

extern uint32 __nc_dmastart[];  //__nc_dmastart => __nc_dmastart[] modify for codesonar warning

// MPU DEFAULT DEFINES
#define MPU_MAX_REGION         12U//SYSTEEM DEFINED // 16U(MAX)

// MPU DEFINES
#define REGION_ENABLE           1U
#define REGION_DISABLE          0U

// MPU START ADDR
#define MPU_BOOT_ADDR           0x00000000U
#define MPU_VIOC_ADDR           0x12000000U
#define MPU_PERI_ADDR           0x1B000000U
#ifdef TCC803x_CS
#define MPU_SRAM0_ADDR          0x06000000U
#define MPU_SRAM0_END           0x06040000U
#define MPU_SRAM1_ADDR          0x07000000U
#define MPU_SNOR_ADDR           0x08000000U
#define MPU_VECTOR_ADDR         0xFFFF0000U
#define MPU_A7S_BOOT_ADDR       0x80000000U
#define MPU_A7S_BACKUP_ADDR     0xA0000000U
#define MPU_CBUS_ADDR           0x17000000U
#else
#define MPU_SRAM0_ADDR          0xC0000000U
#define MPU_SNOR_ADDR           0xD0000000U
#endif

typedef struct _MPU_CONFIG
{
	uint32 region_enable;
	uint32 region_base;
	uint32 region_size;
	uint32 region_attr;
} MPU_CONFIG;

//MPU_CONFIG mpu_config[MPU_MAX_REGION];

void mpu_init(void);
uint32 mem_get_dma_base(void);

#endif /* __BSP_ARM_MPU__H__ */
