/*
 * Philips UDA1342 mixer device driver
 *
 * Copyright (c) 2003 Stéphane Magnenat <stephane.magnenat@epfl.ch>
 * Copyright (c) 2004 K-TEAM SA by Cédric Gaudin <cedric.gaudin@k-team.com>
 *
 * This code is base on the structure of uda1341.c from Nicolas Pitre
 * and ak4535.c from Jamey Hicks
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 * CG [02-06-2004] Added differential input support 
 * CG [06-06-2004] Changed default input level to 100:100 for the two channels
 * AM [17-02-2005] - Added IOctls to get a gain setting
 *                   SOUND_GAIN_LINE1 + SOUND_MIXER_LINE2
 *                 - Changed default input level to 72 (0dB amplification on mixer gain )
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/soundcard.h>
#include <linux/i2c.h>

#include <asm/uaccess.h>

#include "uda1342.h"

/* mask to describe which features we have */
#define REC_MASK	( SOUND_MASK_LINE1 | SOUND_MASK_LINE2 )
#define DEV_MASK	( REC_MASK | SOUND_MASK_VOLUME | SOUND_MASK_BASS | SOUND_MASK_TREBLE )

/* Enabling this print some debug informations */
//#define DEBUG
#undef DEBUG


static int uda1342_configure(struct uda1342 *uda, struct uda1342_cfg *conf)
{
	int ret = 0;
	switch (conf->fs)
	{
		case 256: uda->regs.system.system_clock_frequency = UDA1342_SYSTEM_SC_256FS; break;
		case 384: uda->regs.system.system_clock_frequency = UDA1342_SYSTEM_SC_384FS; break;
		case 512: uda->regs.system.system_clock_frequency = UDA1342_SYSTEM_SC_512FS; break;
		case 768: uda->regs.system.system_clock_frequency = UDA1342_SYSTEM_SC_768FS; break;
		default:  ret = -EINVAL; break;
	}
	uda->regs.system.data_format = conf->format;

	if (ret == 0 && uda->active)
	{
		uda->write(uda, UDA1342_REG_SYSTEM, *(unsigned short *)&uda->regs.system);
	}
	return ret;
}

/* resample volume vol from base_min (0) to base_max(100) => min to max */
static int resample_volume(int vol, int min, int max)
{
	int left, right;
	int base_min=0, base_max=100;

	left = vol & 0xff;
	right = vol >> 8;

	left = min+(((max - min)*(left - base_min))/(base_max - base_min));
	right = min+(((max - min)*(right - base_min))/(base_max - base_min));

	left &= 0xff;
	right &= 0xff;

	return left | right << 8;
}

static void correctSystemFromMixer(struct uda1342 *uda)
{
	/* select correct config from mixer values */
	if (uda->line1 == 0)
	{
		if (uda->line2 == 0)
		{
			uda->regs.system.adc_mode = UDA1342_SYSTEM_AM_POWER_OFF;
		}
		else
		{
#ifdef CONFIG_SOUND_PXA_UDA1342_DIFF
			uda->regs.system.adc_mode = UDA1342_SYSTEM_AM_INPUT_2_DIFF;
#else
			uda->regs.system.adc_mode = UDA1342_SYSTEM_AM_INPUT_2;
#endif /* CONFIG_SOUND_PXA_UDA1342_DIFF */
		}
	}
	else
	{
		if (uda->line2 == 0)
		{
#ifdef CONFIG_SOUND_PXA_UDA1342_DIFF
			uda->regs.system.adc_mode = UDA1342_SYSTEM_AM_INPUT_1_DIFF;
#else
			uda->regs.system.adc_mode = UDA1342_SYSTEM_AM_INPUT_1;
#endif /* CONFIG_SOUND_PXA_UDA1342_DIFF */
		}
		else
		{
			uda->regs.system.adc_mode = UDA1342_SYSTEM_AM_MIXING_MODE;
		}
	}

	/* shutdown DAC if main volume is 0 */
	if (uda->volume)
	{
		uda->regs.system.dac_power_control = 1;
	}
	else
	{
		uda->regs.system.dac_power_control = 0;
	}
	/* commit to chip */
	uda->write(uda, UDA1342_REG_SYSTEM, *(unsigned short *)&uda->regs.system);
}

static void correctFeaturesFromMixer(struct uda1342 *uda)
{
	/* select correct config from mixer values, commit to chip is
	   external to this function */
	if ((uda->bass) || (uda->treble))
	{
		uda->regs.dac_features.mode = UDA1342_DAC_FEATURES_M_MAX;
	}
	else
	{
		uda->regs.dac_features.mode = UDA1342_DAC_FEATURES_M_FLAT;
	}
}

static int uda1342_mixer_ioctl(struct uda1342 *uda, int cmd, void *arg)
{
	int ret = 0;
	int oldLine1Vol;
	int oldLine2Vol;

	/* information IOCTL */
	if (cmd == SOUND_MIXER_INFO)
	{
		struct mixer_info mi;

		strncpy(mi.id, "UDA1342", sizeof(mi.id));
		strncpy(mi.name, "Philips UDA1342", sizeof(mi.name));
		mi.modify_counter = uda->mod_cnt;
		return copy_to_user(arg, &mi, sizeof(mi));
	}

	/* write IOCTL */
	if (_IOC_DIR(cmd) & _IOC_WRITE)
	{
		int nr = _IOC_NR(cmd);
		int val;
		ret = get_user(val, (int *)arg);
		if (ret)
			return ret;

		switch (nr)
		{
		case SOUND_MIXER_VOLUME:
			/* set volume.  val =  0 to 100 => 224 to 0 */
			uda->volume = val;
			uda->mod_cnt++;
			val = resample_volume(val, 224, 0);
			uda->regs.dac_master_volume.left = val & 0xff;
			uda->regs.dac_master_volume.right = (val>>8) & 0xff;
			uda->write(uda, UDA1342_REG_DAC_MASTER_VOLUME, *(unsigned short *)&uda->regs.dac_master_volume);
			correctSystemFromMixer(uda);
			break;

		case SOUND_MIXER_BASS:
			/* set bass.    val = 0 to 100 => 0 to 12 */
			/* we can set this only for both channels, ignoring right setting */
			uda->bass = val;
			uda->mod_cnt++;
			val = resample_volume(val, 0, 12);
			uda->regs.dac_features.bass_boost = val & 0x0f;
			correctFeaturesFromMixer(uda);
			uda->write(uda, UDA1342_REG_DAC_FEATURES, *(unsigned short *)&uda->regs.dac_features);
			break;

		case SOUND_MIXER_TREBLE:
			/* set treble.  val = 0 to 100 => 0 to 3 */
			/* we can set this only for both channels, ignoring right setting */
			uda->treble = val;
			uda->mod_cnt++;
			val = resample_volume(val, 0, 3);
			uda->regs.dac_features.treble = val & 0x3;
			correctFeaturesFromMixer(uda);
			uda->write(uda, UDA1342_REG_DAC_FEATURES, *(unsigned short *)&uda->regs.dac_features);
			break;

		case SOUND_MIXER_LINE1:
			/* set line 1.  val = 0 to 100 => -128 to 48 */
			/* we can set this only for both channels, ignoring right setting */
		        uda->line1 = val;
			uda->mod_cnt++;
			uda->regs.input_1.mixer_gain = resample_volume(val, -128, 48) & 0xff;
			
			/* uda->regs.input_1.input_gain = 8; 
			   ndlr : removed because an approriate ioctl be added to achieve this 
			          especially 8 means that input adc gain just have +24[dB]
				  which is too much for most application */

			/*uda->regs.input_1.input_gain = resample_volume(val, 0, 8) & 0x0f;*/
			uda->write(uda, UDA1342_REG_INPUT_1, *(unsigned short *)&uda->regs.input_1);
			correctSystemFromMixer(uda);
			break;

		case SOUND_MIXER_LINE2:
			/* set line 2.  val = 0 to 100 => -128 to 48 
			   we can set this only for both channels, ignoring right setting */
			uda->line2 = val;
			uda->mod_cnt++;
			uda->regs.input_2.mixer_gain = resample_volume(val, -128, 48) & 0xff;
			
			/* uda->regs.input_2.input_gain = 8;
			   ndlr : removed because an approriate ioctl be added to achieve this 
			          especially 8 means that input adc gain just have +24[dB]
				  which is too much for most application */
	

			uda->write(uda, UDA1342_REG_INPUT_2, *(unsigned short *)&uda->regs.input_2);
			correctSystemFromMixer(uda);
			break;
			
		case SOUND_MIXER_IGAIN:
			/* set line1 and line2 gain. val = 0 to 100 => 0 to 8 
			   we need to get volume because we need to set the register correctly
			   ndlr: there is no 2 channels gain ioctls so we cannot set
			         the line 1 gain and the line2 gain individually */
				 
				 
			/* we keep just the volume from the input register */
			oldLine1Vol = uda->line1 & 0xFF;
			oldLine2Vol = uda->line2 & 0xFF;
			
			/* we just add gain in the line registers */
			uda->line1 |= (val<<8);  
			uda->line2 |= (val<<8);  
			
			/* Two modification on the codec settings so we increment by 2 */ 
			//uda->mod_cnt += 2;
			uda->mod_cnt++;
			
			/* Set the "input_1" register with gain and volume */
			uda->regs.input_1.input_gain = resample_volume(val, 0, 8) & 0x0f;
			uda->regs.input_2.input_gain = resample_volume(val, 0, 8) & 0x0f;
			
			/* Set the "input_2" register with gain and volume */
			uda->regs.input_1.mixer_gain = resample_volume(oldLine1Vol, -128, 48) & 0xff;
			uda->regs.input_2.mixer_gain = resample_volume(oldLine2Vol, -128, 48) & 0xff;
			
			/* Write the registers not sure that correctSystemFromMixer should be called
			   twice... */
			uda->write(uda, UDA1342_REG_INPUT_1, *(unsigned short *)&uda->regs.input_1);
			correctSystemFromMixer(uda);
			
			uda->mod_cnt++;						
			uda->write(uda, UDA1342_REG_INPUT_2, *(unsigned short *)&uda->regs.input_2);
			correctSystemFromMixer(uda);	
						
			break;
			
		case SOUND_MIXER_RECSRC:
			break;

		default:
			ret = -EINVAL;
		}
	}

	/* read IOCTL */
	if (ret == 0 && _IOC_DIR(cmd) & _IOC_READ)
	{
		int nr = _IOC_NR(cmd);
		int val;

		switch (nr)
		{
		case SOUND_MIXER_VOLUME:     val = uda->volume;	break;
		case SOUND_MIXER_BASS:       val = uda->bass;	break;
		case SOUND_MIXER_TREBLE:     val = uda->treble;	break;
		case SOUND_MIXER_LINE1:      val = uda->line1;	break;
		case SOUND_MIXER_LINE2:      val = uda->line2;	break;
		case SOUND_MIXER_RECSRC:     val = REC_MASK;	break;
		case SOUND_MIXER_RECMASK:    val = REC_MASK;	break;
		case SOUND_MIXER_DEVMASK:    val = DEV_MASK;	break;
		case SOUND_MIXER_STEREODEVS: val = DEV_MASK;	break;
		default:             val = 0; ret = -EINVAL;	break;
		}

		/* return value to userspace */
		if (ret == 0)
			ret = put_user(val, (int *)arg);
	}

	return ret;
}

static UDA1342_regs_t UDA1342_REGS_DFLT =
{
	UDA1342_SYSTEM_DFLT,
	UDA1342_SUB_SYSTEM_DFLT,
	UDA1342_DAC_FEATURES_DFLT,
	UDA1342_DAC_MASTER_VOLUME_DFLT,
	UDA1342_DAC_MIXER_VOLUME_DFLT,
	UDA1342_INPUT_MIXER_DFLT,
	UDA1342_INPUT_MIXER_DFLT
};

static struct uda1342 *uda1342_constructor(int (*device_write)(struct uda1342 *uda, int regaddr, unsigned short value), void *device_io_data)
{
	/* allocate uda struct */
	struct uda1342 *uda = kmalloc(sizeof(*uda), GFP_KERNEL);

	/* fills regs with default value, no access, device disabled */
	if (uda)
	{
		uda->regs = UDA1342_REGS_DFLT;
		uda->mod_cnt = 0;
		uda->active = 0;
		uda->volume = 100 | (100<<8);
		uda->bass = 0;
		uda->treble = 0;
		uda->line1 = 72 ; 	// mixer Gain is 0[dB] for both line1 and line2
		uda->line2 = 72 ; 	
		uda->write = device_write;
		uda->io_data = device_io_data;
	}

	return uda;
}

static void uda1342_destructor(struct uda1342 *uda)
{
	/* free the uda struct */
	kfree(uda);
}

/*
	Note : we do not need sync
static void uda1342_sync(struct uda1342 *uda)
{
	uda->write(uda, UDA1342_REG_SYSTEM, *(unsigned short *)&uda->regs.system);
	uda->write(uda, UDA1342_REG_SUB_SYSTEM, *(unsigned short *)&uda->regs.sub_system);
	uda->write(uda, UDA1342_REG_DAC_FEATURES, *(unsigned short *)&uda->regs.dac_features);
	uda->write(uda, UDA1342_REG_DAC_MASTER_VOLUME, *(unsigned short *)&uda->regs.dac_master_volume);
	uda->write(uda, UDA1342_REG_DAC_MIXER_VOLUME, *(unsigned short *)&uda->regs.dac_mixer_volume);
	uda->write(uda, UDA1342_REG_INPUT_1, *(unsigned short *)&uda->regs.input_1);
	uda->write(uda, UDA1342_REG_INPUT_2, *(unsigned short *)&uda->regs.input_2);
}
*/

static int uda1342_open(struct uda1342 *uda)
{
	/* increment usage counter */
	uda->active++;
#ifdef DEBUG
	printk("Open UDA1342\n");
#endif
	return 0;
}

static void uda1342_close(struct uda1342 *uda)
{
	/* decrement usage counter */
	uda->active--;
#ifdef DEBUG
	printk("Close UDA1342\n");
#endif
}


/*
	I2C specific begins here
*/

static int uda1342_write_reg_i2c(struct uda1342 *uda, int regaddr, unsigned short value)
{
	char buffer[3];
	int r;
	struct i2c_client *clnt=(struct i2c_client *)uda->io_data;

	buffer[0] = regaddr;
	buffer[1] = value >> 8;
	buffer[2] = value & 0xff;

#ifdef DEBUG
	printk("Writing to address 0x%x (reg = 0x%x, data = 0x%x)\n", clnt->addr, regaddr, value);
#endif
	r = i2c_master_send(clnt, buffer, 3);
	if (r != 3)
	{
		printk(KERN_ERR "%s: write failed, status %d\n", __FUNCTION__, r);
	}
	return r;
}

extern struct i2c_driver uda1342_i2c;

static int uda1342_attach(struct i2c_adapter *adap, int addr, unsigned short flags, int kind)
{
	struct uda1342 *uda;
	struct i2c_client *clnt;

	 /* Unique ID allocation */
	static int uda1342_i2c_client_id = 0;

	/* allocate an I2C client struct */
	clnt = kmalloc(sizeof(*clnt), GFP_KERNEL);
	if (!clnt)
		return -ENOMEM;

	/* fill the I2C client struct */
	strncpy(clnt->name, UDA1342_NAME, sizeof(clnt->name));
	clnt->id = uda1342_i2c_client_id++;
	clnt->flags = I2C_CLIENT_ALLOW_USE;
	clnt->addr = addr;
	clnt->adapter = adap;
	clnt->driver = &uda1342_i2c;
	clnt->usage_count = 0;

	/* create the uda struct */
	uda = uda1342_constructor(uda1342_write_reg_i2c, clnt);
	if (!uda)
		return -ENOMEM;
	clnt->data = uda;

	/* attach to I2C subsystem */
	i2c_attach_client(clnt);

#ifdef DEBUG
	printk("I2C UDA1342 attached to I2C subsystem on address %x\n", addr);
#endif
	return 0;
}

static int uda1342_detach_client(struct i2c_client *clnt)
{
	i2c_detach_client(clnt);

	uda1342_destructor(clnt->data);
	kfree(clnt);

	return 0;
}

/* Addresses to scan */
static unsigned short normal_i2c[] = { 0x1b, 0x1a, I2C_CLIENT_END };
static unsigned short normal_i2c_range[] = { I2C_CLIENT_END };
static unsigned short probe[]        = { I2C_CLIENT_END, I2C_CLIENT_END };
static unsigned short probe_range[]  = { I2C_CLIENT_END, I2C_CLIENT_END };
static unsigned short ignore[]       = { I2C_CLIENT_END, I2C_CLIENT_END };
static unsigned short ignore_range[] = { I2C_CLIENT_END, I2C_CLIENT_END };
static unsigned short force[]        = { ANY_I2C_BUS, 0x1b, 0x1a, I2C_CLIENT_END, I2C_CLIENT_END };

static struct i2c_client_address_data addr_data = {
	normal_i2c, normal_i2c_range,
	probe, probe_range,
	ignore, ignore_range,
	force
};

static int uda1342_attach_adapter(struct i2c_adapter *adap)
{
#ifdef DEBUG
	printk("Attach adapter UDA1342\n");
#endif
	return i2c_probe(adap, &addr_data, uda1342_attach);
}

static int uda1342_command(struct i2c_client *clnt, unsigned int cmd, void *arg)
{
	int ret = -EINVAL;

	if (_IOC_TYPE(cmd) == 'M')
		ret = uda1342_mixer_ioctl(clnt->data, cmd, arg);
	else if (cmd == I2C_UDA1342_CONFIGURE)
		ret = uda1342_configure(clnt->data, arg);
	else if (cmd == I2C_UDA1342_OPEN)
		ret = uda1342_open(clnt->data);
	else if (cmd == I2C_UDA1342_CLOSE)
		(ret = 0), uda1342_close(clnt->data);

	return ret;
}

static void uda1342_inc_use(struct i2c_client *clnt)
{
	MOD_INC_USE_COUNT;
}

static void uda1342_dec_use(struct i2c_client *clnt)
{
	MOD_DEC_USE_COUNT;
}

static struct i2c_driver uda1342_i2c = {
	name:           UDA1342_NAME,
	id:             I2C_DRIVERID_UDA1342,
	flags:          I2C_DF_NOTIFY,
	attach_adapter: uda1342_attach_adapter,
	detach_client:  uda1342_detach_client,
	command:        uda1342_command,
	inc_use:        uda1342_inc_use,
	dec_use:        uda1342_dec_use
};

static int __init uda1342_init(void)
{
#ifdef DEBUG
	printk("UDA1342 add driver\n");
#endif
	return i2c_add_driver(&uda1342_i2c);
}

static void __exit uda1342_exit(void)
{
#ifdef DEBUG
	printk("UDA1342 del driver\n");
#endif
	i2c_del_driver(&uda1342_i2c);
}

module_init(uda1342_init);
module_exit(uda1342_exit);

MODULE_AUTHOR("Stephane Magnenat");
MODULE_DESCRIPTION("Philips UDA1342 CODEC driver");
