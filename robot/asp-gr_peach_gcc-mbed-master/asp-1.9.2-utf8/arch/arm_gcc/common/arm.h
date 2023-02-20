/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel  
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2012 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: arm.h 2757 2016-03-10 15:03:12Z ertl-honda $
 */

/*
 *  ARM�̃n�[�h�E�F�A�����̒�`
 */

#ifndef TOPPERS_ARM_H
#define TOPPERS_ARM_H

/* 
 *  ARM �̃n�[�h�E�F�A�����̃c�[���ˑ����̒�` 
 */ 
#include <arm_tool.h>

/*
 *  ARM ��O�x�N�^
 */
#define SVC_Vector    UINT_C(0x00)
#define UND_Vector    UINT_C(0x04)
#define SWI_Vector    UINT_C(0x08)
#define PRFA_Vector   UINT_C(0x0C)
#define DATAA_Vector  UINT_C(0x10)
#define IRQ_Vector    UINT_C(0x18)
#define FIQ_Vector    UINT_C(0x1C)

/*
 * ARM ��O�x�N�^�ԍ�
 */
#define SVC_Number    UINT_C(0)
#define UND_Number    UINT_C(1)
#define SWI_Number    UINT_C(2)
#define PRFA_Number   UINT_C(3)
#define DATAA_Number  UINT_C(4)
#define UNNOWN_Number UINT_C(5)
#define IRQ_Number    UINT_C(6)
#define FIQ_Number    UINT_C(7)

/*
 *  CPSR �����݋֎~�r�b�g
 */
#define CPSR_INT_MASK UINT_C(0xC0)
#define CPSR_IRQ_BIT  UINT_C(0x80)
#define CPSR_FIQ_BIT  UINT_C(0x40)

/*
 *  CPSR �̃��[�h�r�b�g
 */
#define CPSR_MODE_MASK   UINT_C(0x1f)
#define CPSR_USER        UINT_C(0x10)
#define CPSR_FIQ         UINT_C(0x11)
#define CPSR_IRQ         UINT_C(0x12)
#define CPSR_SVC         UINT_C(0x13)
#define CPSR_ABT         UINT_C(0x17)
#define CPSR_UND         UINT_C(0x1B)
#define CPSR_SYS         UINT_C(0x1F)

/*
 *  �R�v���̃r�b�g��`
 */
#define CP15_CONTROL_XP_BIT   (1 << 23)
#define CP15_CONTROL_V_BIT    (1 << 13)
#define CP15_CONTROL_I_BIT    (1 << 12)
#define CP15_CONTROL_Z_BIT    (1 << 11)
#define CP15_CONTROL_C_BIT    (1 <<  2)
#define CP15_CONTROL_M_BIT    (1 <<  0)

#if __TARGET_ARCH_ARM == 6
#define CP15_AUXILIARY_SA_BIT (1 << 5)
#define CP15_CPUID_BIT        (0x0f)
#else /* __TARGET_ARCH_ARM == 7 */
#define CP15_AUXILIARY_SA_BIT (1 << 6)
#define CP15_AUXILIARY_EX_BIT (1 << 7)
#define CP15_CPUID_BIT        (0x03)
#endif /* __TARGET_ARCH_ARM == 7 */

#define CP15_TTB0_RGN_S       (1 << 1)
#define CP15_TTB0_RGN_WBWA    (1 << 3)

#if __TARGET_ARCH_ARM == 7
#define CP15_TTB0_IRGN_WBWA   ((1 << 6)|(0))
#endif /* __TARGET_ARCH_ARM == 7 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  �R�v����p�������[�`��
 */
#if (__TARGET_ARCH_ARM == 6) || (__TARGET_ARCH_ARM == 7)
/*
 *  High exception vector ���g�����̐ݒ�
 */
Inline void
set_high_vector(bool_t enable)
{
	uint32_t control;

	CP15_CONTROL_READ(control);

	if (enable) {
		control |= CP15_CONTROL_V_BIT;
	}
	else {
		control &= ~CP15_CONTROL_V_BIT;
	}

	CP15_CONTROL_WRITE(control);
}

/*
 *  �v���Z�b�TINDEX�i0�I���W���j�̎擾
 */
Inline uint32_t
x_prc_index(void)
{
	uint32_t index;

	CP15_CPUID_READ(index);
	return((index & 0x0fU));
}

/*
 *  Data Synchronization Barrier
 *  �E��ɔ��s���ꂽ read �� write �̏I����҂�
 *  �E�L���b�V���C�u�����`�v���f�B�N�V�����CTLB�̑���̏I����҂�
 *  �E���̖��߂̌�ɏ����ꂽ���߂͎��s����Ȃ� 
 */
Inline void
data_sync_barrier(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Data Memory Barrier
 *  �E�v���O�����̋L�q�ɏ]���āC��ɏ����ꂽ���߂ł̃������A�N�Z�X��
 *     �I������܂ő҂D
 */
Inline void
data_memory_barrier(void)
{
	CP15_DATA_MEMORY_BARRIER();
}


/* 
 *  �L���b�V���֘A
 */
/*
 *  TLB�̖�����
 */
Inline void
invalidate_unfied_tlb(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  D�L���b�V���̖�����
 */
Inline void
dcache_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  D�L���b�V���̃N���[��
 */
Inline void
dcache_clean(void)
{
#if __TARGET_ARCH_ARM == 6
#error
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t  ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_CLEAN(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  D�L���b�V���̃N���[���Ɩ�����
 */
Inline void
dcache_clean_and_invalidate(void)
{
#if __TARGET_ARCH_ARM == 6
	CP15_DCACHE_CLEAN_AND_INVALIDATE();
#elif __TARGET_ARCH_ARM == 7
	uint32_t bits = 0;
	uint32_t  ways = 0;
	uint32_t sets = 0;

	CP15_CACHE_SIZE_SELECTION_WRITE(0);
	CP15_PBUFFER_FLUSH();
	for (ways = 0; ways < 4; ways++){
		for (sets = 0; sets < 256; sets++){
			bits = ways << 30 | sets << 5;
			CP15_DCACHE_CLEAN_AND_INVALIDATE(bits);
		}
	}
#endif /* __TARGET_ARCH_ARM == 7 */
}

/*
 *  I�L���b�V���̖�����
 */
Inline void
icache_invalidate(void)
{
	CP15_ICACHE_INVALIDATE();
}

/*
 *  �v���t�F�b�`�o�b�t�@���N���A
 */
Inline void
pbuffer_flash(void)
{
	CP15_PBUFFER_FLUSH();
}

/*
 *  �u�����`�v���f�B�N�^�[�̑S������
 */
Inline void
bpi_invalidate_all(void)
{
	CP15_BPI_INVALIDATEALL(0);
	CP15_DATA_SYNC_BARRIER();
	CP15_PBUFFER_FLUSH();
}

/*
 *  D�L���b�V�����J�n
 */
extern void dcache_enable(void);

/*
 *  D�L���b�V�����~
 */
extern void dcache_disable(void);

/*
 *  I�L���b�V���̊J�n
 */
extern void icache_enable(void);

/*
 *  I�L���b�V�����~
 */
extern void icache_disable(void);
	 
/*
 *  I/D�L���b�V���𗼕���L����
 */
extern void cache_enable(void);

/*
 *  I/D�L���b�V���𗼕��𖳌���
 */
extern void cache_disable(void);

/*
 *  �u�����`�v���f�B�N�^�[��L����
 */
Inline void
btac_enable(void)
{
	uint32_t tmp;

	CP15_CONTROL_READ(tmp);
	tmp |= CP15_CONTROL_Z_BIT;
	CP15_CONTROL_WRITE(tmp);
}

/*
 *  MMU�֘A
 */
/*
 * �ϊ��e�[�u���ւ̐ݒ���e
 * va   : ���z�A�h���X
 * pa   : �����A�h���X
 * size : �T�C�Y
 * ns   : Secure/Non-Secure
 * s    : ���L�w��
 * tex  : C B�Ƃ̑g�ݍ��킹�ŕω�
 * ap   : �A�N�Z�X��
 * c    : �L���b�V��
 * b    : �o�b�t�@
 */
typedef struct{
	uintptr_t   va;
	uintptr_t   pa;
	uint32_t    size;
#if __TARGET_ARCH_ARM == 7
	uint8_t     ns;
#endif /* __TARGET_ARCH_ARM == 7 */
	uint8_t     s;
	uint8_t     tex;
	uint8_t     ap;
	uint8_t     c;
	uint8_t     b;
}MEMORY_ATTRIBUTE;

/*
 *  MMU�̏�����
 */
extern void mmu_init(void);

/*
 *  MMU�ɂ�郁�����̃}�b�s���O
 */
extern void mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute);
#endif /* (__TARGET_ARCH_ARM == 6) || (__TARGET_ARCH_ARM == 7) */

#endif  /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_ARM_H */
