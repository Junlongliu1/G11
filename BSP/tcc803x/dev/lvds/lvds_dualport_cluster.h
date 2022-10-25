/****************************************************************************************
 *   FileName    : lvds_dualport_cluster.h
 *   Description :
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

 This source code contains confidential information of Telechips.
 Any unauthorized use without a written permission of Telechips including not limited
 to re-distribution in source or binary form is strictly prohibited.
 This source code is provided AS IS and nothing contained in this source code
 shall constitute any express or implied warranty of any kind, including without limitation,
 any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
 copyright or other third party intellectual property right.
 No warranty is made, express or implied, regarding the information accuracy,
 completeness, or performance.
 In no event shall Telechips be liable for any claim, damages or other liability arising from,
 out of or in connection with this source code or the use in the source code.
 This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
 between Telechips and Company.
 *
 ****************************************************************************************/

#ifndef __LVDS_LGPOC_CLUSTER_H__
#define	__LVDS_LGPOC_CLUSTER_H__
#include <types.h>

extern void BL_On(void);
extern void init_lvds_preset(void);
extern void init_lvds(void);

extern int32 get_lvds_signal_ready(void);
extern int32 get_reset_is_fail(void);
extern void reset_tx_splitter(void);
extern void masking_lvds_err_interrupt(void);

#endif
