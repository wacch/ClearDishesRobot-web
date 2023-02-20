/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
 * 
 *  Copyright (C) 2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN  
 *
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 * 
 *  @(#) $Id: arm_tool.h 2757 2016-03-10 15:03:12Z ertl-honda $
 */

/*
 *  ARM�̃n�[�h�E�F�A�����̒�`
 */

#ifndef TOPPERS_ARM_TOOL_H
#define TOPPERS_ARM_TOOL_H

#if __TARGET_ARCH_ARM == 7

/* Performance Monitors Cycle Counter */
#define CP15_PMCCNTR_WRITE(x)    Asm("mcr p15, 0, %0, c9, c13, 0"::"r"(x))
#define CP15_PMCCNTR_READ(x)     Asm("mrc p15, 0, %0, c9, c13, 0":"=r"(x))

/* Performance Monitors Control Register */
#define CP15_PMCR_WRITE(x)  Asm("mcr p15, 0, %0, c9, c12, 0":: "r"(x))
#define CP15_PMCR_READ(x)   Asm("mrc p15, 0, %0, c9, c12, 0":"=r"(x))

#define CP15_PMCR_PMCCNTR_D      0x08U
#define CP15_PMCR_PMCCNTR_CLEAR  0x04U
#define CP15_PMCR_ALLCNTR_ENABLE 0x01U

#define CP15_PMCNTENSET_WRITE(x)  Asm("mcr p15, 0, %0, c9, c12, 1":: "r"(x))
#define CP15_PMCNTENSET_READ(x)   Asm("mrc p15, 0, %0, c9, c12, 1":"=r"(x))

#define CP15_PMCNTENSET_CCNTR_ENABLE 0x80000000U

#elif __TARGET_ARCH_ARM == 6

#define CP15_PMCCNTR_READ(x)     Asm("mrc p15, 0, %0, c15, c12, 1":"=r"(x))
#define CP15_PMCCNTR_WRITE(x)    Asm("mcr p15, 0, %0, c15, c12, 1"::"r"(x))

#define CP15_PMCR_READ(x)     Asm("mrc p15, 0, %0, c15, c12, 0":"=r"(x))
#define CP15_PMCR_WRITE(x)    Asm("mcr p15, 0, %0, c15, c12, 0"::"r"(x))

#define CP15_PMCR_PMCCNTR_CLEAR 0x04
#define CP15_PMCR_ALLCNTR_ENABLE 0x01

#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_CONTROL_READ(x)     Asm("mrc p15, 0, %0, c1, c0, 0":"=r"(x))
#define CP15_CONTROL_WRITE(x)    Asm("mcr p15, 0, %0, c1, c0, 0"::"r"(x))

#define CP15_AUXILIARY_READ(x)   Asm("mrc p15, 0, %0, c1, c0, 1":"=r"(x))
#define CP15_AUXILIARY_WRITE(x)  Asm("mcr p15, 0, %0, c1, c0, 1"::"r"(x))

#define CP15_ICACHE_INVALIDATE()           Asm("mcr p15, 0, %0, c7, c5, 0"::"r"(0))
#if __TARGET_ARCH_ARM == 6
#define CP15_DCACHE_INVALIDATE()           Asm("mcr p15, 0, %0, c7, c6, 0"::"r"(0))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE() Asm("MCR p15, 0, %0, c7, c14, 0"::"r"(0))
#elif __TARGET_ARCH_ARM == 7
#define CP15_BPI_INVALIDATEALL(x)           Asm("mcr p15, 0, %0, c7, c5, 6"::"r"(x))
#define CP15_DCACHE_INVALIDATE(x)           Asm("mcr p15, 0, %0, c7, c6, 2"::"r"(x))
#define CP15_DCACHE_CLEAN(x)                Asm("mcr p15, 0, %0, c7, c10,2"::"r"(x))
#define CP15_DCACHE_CLEAN_AND_INVALIDATE(x) Asm("MCR p15, 0, %0, c7, c14, 2"::"r"(x))
#endif /* __TARGET_ARCH_ARM == 7 */

#if __TARGET_ARCH_ARM == 6
#define CP15_PBUFFER_FLUSH()               Asm("mcr p15, 0, %0, c7, c5, 4"::"r"(0):"memory")
#define CP15_DATA_SYNC_BARRIER()           Asm("mcr p15, 0, %0, c7, c10, 4"::"r"(0):"memory")
#define CP15_DATA_MEMORY_BARRIER()         Asm("mcr p15, 0, %0, c7, c10, 5"::"r" (0):"memory")
#elif __TARGET_ARCH_ARM == 7
#define CP15_PBUFFER_FLUSH()               Asm("isb":::"memory")
#define CP15_DATA_SYNC_BARRIER()           Asm("dsb":::"memory")
#define CP15_DATA_MEMORY_BARRIER()         Asm("dmb":::"memory")
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_INVALIDATE_UNIFIED_TLB() Asm("mcr p15, 0, %0, c8, c7,  0"::"r"(0))

#define CP15_TTBCR_WRITE(x) Asm("mcr p15, 0, %0, c2, c0, 2"::"r"(x))
#define CP15_TTB0_READ(x)   Asm("mrc p15, 0, %0, c2, c0, 0":"=r"(x))
#define CP15_TTB0_WRITE(x)  Asm("mcr p15, 0, %0, c2, c0, 0"::"r"(x))

#define CP15_DOMAINS_WRITE(x) Asm("mcr p15, 0, %0, c3, c0, 0":: "r"(x))
#define CP15_ASID_SET(x)      Asm("mcr p15, 0, %0, c13, c0, 1" ::"r"(x))

#define CP15_CPUID_READ(x)     Asm("mrc p15, 0, %0, c0, c0, 5":"=r"(x))
#define CP15_CACHE_SIZE_ID_READ(x)         Asm("mrc p15, 1, %0, c0, c0, 0":"=r"(x))

#if __TARGET_ARCH_ARM == 7
#define CP15_CACHE_SIZE_SELECTION_WRITE(x) Asm("mcr p15, 2, %0, c0, c0, 0"::"r"(x))
#define CP15_CACHE_SIZE_SELECTION_READ(x)  Asm("mrc p15, 2, %0, c0, c0, 0":"=r"(x))
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_TLB_FLASH_ALL()  Asm("mcr p15, 0, %0, c8, c7, 0" ::"r"(0))
#define CP15_TLB_FLASH_MVA(x) Asm("mcr p15, 0, %0, c8, c7, 3" ::"r"(x))

#define CPU15_PONLY_THREAD_ID_READ(x)  Asm("mrc p15, 0, %0, c13, c0, 4":"=r"(x))
#define CPU15_PONLY_THREAD_ID_WRITE(x) Asm("mcr p15, 0, %0, c13, c0, 4":: "r"(x))

#define CP15_SET_VBAR(x)    Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_TLB_INVALIDATE_ALL()  Asm("mcr p15, 0, %0, c8, c7, 0" ::"r"(0))
#endif /* TOPPERS_ARM_TOOL_H */
