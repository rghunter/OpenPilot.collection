/*
 * Philips UDA1342 mixer device driver
 *
 * Copyright (c) 2003 Stéphane Magnenat <stephane.magnenat@epfl.ch>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 */

 #ifndef __UDA1342_H
 #define __UDA1342_H

#define UDA1342_NAME "uda1342"

/* const used to for the command callback with I2C */
#define I2C_UDA1342_CONFIGURE	0x13800001
#define I2C_UDA1342_OPEN	0x13800002
#define I2C_UDA1342_CLOSE	0x13800003

/* struct used to configurate the codec,
 * called by plateforme init not ioctl
 */
struct uda1342_cfg
{
	unsigned int fs:16;
	unsigned int format:3;
};

/*
 * UDA1342 internal state variables.
 * The default values are defined to sane initial operating values.
 */

/* UDA1342 system and sub-system settings */

#define UDA1342_SYSTEM_IF_I2S			0
#define UDA1342_SYSTEM_IF_LSB16			1
#define UDA1342_SYSTEM_IF_LSB18			2
#define UDA1342_SYSTEM_IF_LSB20			3
#define UDA1342_SYSTEM_IF_MSB			4
#define UDA1342_SYSTEM_IF_MSB16			5
#define UDA1342_SYSTEM_IF_MSB18			6
#define UDA1342_SYSTEM_IF_MSB20			7

#define UDA1342_SYSTEM_SC_256FS			0
#define UDA1342_SYSTEM_SC_384FS			1
#define UDA1342_SYSTEM_SC_512FS			2
#define UDA1342_SYSTEM_SC_768FS			3

#define UDA1342_SYSTEM_AM_POWER_OFF		0
#define UDA1342_SYSTEM_AM_INPUT_1		1
#define UDA1342_SYSTEM_AM_INPUT_2		2
#define UDA1342_SYSTEM_AM_NOT_USED		3
#define UDA1342_SYSTEM_AM_CHAN_SWAP		4
#define UDA1342_SYSTEM_AM_INPUT_1_DIFF	5
#define UDA1342_SYSTEM_AM_INPUT_2_DIFF	6
#define UDA1342_SYSTEM_AM_MIXING_MODE	7

typedef struct
{
	/* DAC polarity : 0 = non-inverting, 1 = inverting */
	u_int dac_polarity:1;
	/* DAC power control : 0 = DAC power off, 1 = DAC power on */
	u_int dac_power_control:1;
	/* data input format : see above */
	u_int data_format:3;
	/* system clock frequency : see above */
	u_int system_clock_frequency:2;
	/* must be set to 0 */
	const u_int nu:1;
	/* ADC polarity : 0 = non-inverting, 1 = inverting */
	u_int pad:1;
	/* ADC mode : see above */
	u_int adc_mode:3;
	/* DC filter : 0 = disable output DC filtering, 1 = enable */
	u_int dc_filter:1;
	/* bypass mixer DC filter : 0 = enable mixer DC filtering, 1 = disable */
	u_int bypass_mixer_dc_filter:1;
	/* quick mode switch : 0 = soft mode change, 1 = quick mode change */
	u_int quick_mode_switch:1;
	/* reset : 0 = no reset, 1 = reset registers to default */
	u_int reset:1;
} UDA1342_SYSTEM;

#define UDA1342_SYSTEM_DFLT \
	{ 0, 1, UDA1342_SYSTEM_IF_I2S, UDA1342_SYSTEM_SC_256FS, \
	0, 0, UDA1342_SYSTEM_AM_INPUT_1_DIFF, 1, 0, 0, 0 }



#define UDA1342_SUB_SYSTEM_MP_NO_OUTPUT		0
#define UDA1342_SUB_SYSTEM_MP_OV_DETECT		1
#define UDA1342_SUB_SYSTEM_MP_RESERVED		2
#define UDA1342_SUB_SYSTEM_MP_SIL_DETECT	3

#define UDA1342_SUB_SYSTEM_SD_3200		0
#define UDA1342_SUB_SYSTEM_SD_4800		1
#define UDA1342_SUB_SYSTEM_SD_9600		2
#define UDA1342_SUB_SYSTEM_SD_19200		3

#define UDA1342_SUB_SYSTEM_OS_SINGLE	0
#define UDA1342_SUB_SYSTEM_OS_DOUBLE	1
#define UDA1342_SUB_SYSTEM_OS_QUAD		2

typedef struct
{
	/* multi purprose output (status pin) : see above */
	u_int multi_purprose_output:2;
	/* silence detection period : see above */
	u_int silence_detection_period:2;
	/* DAC mixer : 0 = disable mixer, 1 = enable */
	u_int dac_mixer:1;
	/* DAC mixing position switch : 0 = before sound features, 1 = after */
	u_int dac_mixing_position_switch:1;
	/* input oversampling rate : see above */
	u_int input_oversampling_rate:2;
	/* must be set to 0 */
	const u_int nu:8;
} UDA1342_SUB_SYSTEM;

#define UDA1342_SUB_SYSTEM_DFLT \
	{ UDA1342_SUB_SYSTEM_MP_NO_OUTPUT, \
	UDA1342_SUB_SYSTEM_SD_3200, 0, 0, UDA1342_SUB_SYSTEM_OS_SINGLE, 0 }



/* UDA1342 DAC settings */

#define UDA1342_DAC_FEATURES_DE_NO		0
#define UDA1342_DAC_FEATURES_DE_32		1
#define UDA1342_DAC_FEATURES_DE_44		2
#define UDA1342_DAC_FEATURES_DE_48		3
#define UDA1342_DAC_FEATURES_DE_96		4

#define UDA1342_DAC_FEATURES_M_FLAT		0
#define UDA1342_DAC_FEATURES_M_MIN		1
#define UDA1342_DAC_FEATURES_M_MIN2		2
#define UDA1342_DAC_FEATURES_M_MAX		3

typedef struct
{
	/* de-emphasis : see above */
	u_int de_emphasis:3;
	/* quick mute : 0 = soft mute mode, 1 = quick mute mode */
	u_int quick_mute_mode :1;
	/* mute : 0 = no muting, 1 = muting */
	u_int mute:1;
	/* mute digital : 0 = no muting, 1 = muting */
	u_int mute_dac:1;
	/* mute ADC : 0 = no muting, 1 = muting */
	u_int mute_adc:1;
	/* silence detector switch : 0 = disable silence detector, 1 = enable */
	u_int silence_detector_switch:1;
	/* treble : from 0 : 0/0 to 3 : 6/6 (min/max) */
	u_int treble:2;
	/* bass boost : from 0 : 0/0 to 15 : 18/24 (min/max) */
	u_int bass_boost:4;
	/* mode : see above */
	u_int mode:2;
} UDA1342_DAC_FEATURES;

#define UDA1342_DAC_FEATURES_DFLT \
	{ UDA1342_DAC_FEATURES_DE_NO, 0, 0, 0, 0, 0, 0, 0, \
	UDA1342_DAC_FEATURES_M_FLAT }


typedef struct
{
	/* attenuation for right channel 0 (0x00) to inf (0xe0) */
	u_int right:8;
	/* attenuation for left 0 channel (0x00) to inf (0xe0) */
	u_int left:8;
} UDA1342_DAC_MASTER_VOLUME;

#define UDA1342_DAC_MASTER_VOLUME_DFLT	{ 0, 0 }

typedef struct
{
	/* attenuation from digital interface 0 (0x00) to inf (0xb8) */
	u_int digital:8;
	/* attenuation from ADC converter 0 (0x00) to inf (0xb8) */
	u_int adc:8;
} UDA1342_DAC_MIXER_VOLUME;

#define UDA1342_DAC_MIXER_VOLUME_DFLT	{ 0, 0 }



/* UDA1342 ADC settings */

typedef struct
{
	/* ADC mixer gain : -inf (-128) to 24 (48) */
	int mixer_gain:8;
	/* ADC input ampilifier gain : 0 (0) to 24 (8) */
	u_int input_gain:4;
	/* must be set to 0 */
	const u_int nu:4;
} UDA1342_INPUT_MIXER;

#define UDA1342_INPUT_MIXER_DFLT	{ 0, 0, 0 }

/*
* All registers
*/
typedef struct {
	/* 0x00, system */
	UDA1342_SYSTEM system;
	/* 0x01, sub-system */
	UDA1342_SUB_SYSTEM sub_system;
	/* 0x02 to 0x0f, reserved */
	/* 0x10, DAC features */
	UDA1342_DAC_FEATURES dac_features;
	/* 0x11, DAC master volume */
	UDA1342_DAC_MASTER_VOLUME dac_master_volume;
	/* 0x12, DAC mixer volume */
	UDA1342_DAC_MIXER_VOLUME dac_mixer_volume;
	/* 0x13 to 0x1f, reserved */
	/* 0x20, ADC input and mixer gain channel 1 */
	UDA1342_INPUT_MIXER input_1;
	/* 0x21, ADC input and mixer gain channel 1 */
	UDA1342_INPUT_MIXER input_2;
	/* 0x22 to 0x2f, reserved */
	/* 0x30, evaluation */
	/* 0x31 to 0xff, reserved */
} UDA1342_regs_t;

/* position of registers in UDA1342 */
#define UDA1342_REG_SYSTEM             0x00
#define UDA1342_REG_SUB_SYSTEM         0x01
#define UDA1342_REG_DAC_FEATURES       0x10
#define UDA1342_REG_DAC_MASTER_VOLUME  0x11
#define UDA1342_REG_DAC_MIXER_VOLUME   0x12
#define UDA1342_REG_INPUT_1            0x20
#define UDA1342_REG_INPUT_2            0x21

/* This struct is the "object" representing the UDA.
   It has a pointer and some data to a write function
   for IO operations.
   It is object oriented
*/
struct uda1342
{
	/* uda's internal registers mapping */
	UDA1342_regs_t regs;

	/* all volumes */
	unsigned short	volume;
	unsigned short	bass;
	unsigned short	treble;
	unsigned short	line1;
	unsigned short	line2;

	/* number of modifications to the codec */
	int mod_cnt;
	/* is device active ? */
	int active;

	/* IO access */
	/* write function */
	int (*write)(struct uda1342 *uda, int regaddr, unsigned short value);
	/* io data */
	void *io_data;
};

#endif
