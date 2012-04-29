/*
 * Glue audio driver for the PXA25X & Philips UDA1342 codec using I2C.
 *
 * Copyright (c) 2003 Stéphane Magnenat <stephane.magnenat@epfl.ch>
 *
 * This code is based on h3900-uda1380.c from Nicolas Pitre and
 * Jamey Hicks
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License.
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/sound.h>
#include <linux/soundcard.h>
#include <linux/i2c.h>
#include <linux/kmod.h>

#include <asm/semaphore.h>
#include <asm/uaccess.h>
#include <asm/hardware.h>
#include <asm/dma.h>

#include <asm-arm/arch-pxa/pxa-regs.h>

#include "pxa-audio.h"

#include "uda1342.h"

/* Enabling this print some debug informations */
/*#define DEBUG */
#ifdef DEBUG
#define DPRINTK( x... )  printk( ##x )
#else
#define DPRINTK( x... )
#endif

#define AUDIO_NAME		"PXA25X UDA1342 I2C"

#define AUDIO_RATE_DEFAULT	44100

static int
mixer_ioctl(struct inode *inode, struct file *file, uint cmd, ulong arg)
{
	/*
	 * We only accept mixer (type 'M') ioctls.
	 */
	if (_IOC_TYPE(cmd) != 'M')
		return -EINVAL;

	return i2c_command(file->private_data, cmd, (void *)arg);
}

static int mixer_open(struct inode *inode, struct file *file)
{
	struct i2c_client *uda1342;

	uda1342 = i2c_get_client(I2C_DRIVERID_UDA1342, I2C_ALGO_PXA, NULL);

	if (uda1342 == NULL) {
		printk("No UDA1342 detected.  Is i2c-adap-pxa loaded?\n");
		return -ENODEV;
	}

	file->private_data = uda1342;
	i2c_inc_use_client(uda1342);

	return 0;
}

static int mixer_release(struct inode *inode, struct file *file)
{
	i2c_dec_use_client(file->private_data);
	return 0;
}

static struct file_operations pxa_uda1342_i2c_mixer_fops = {
	open:		mixer_open,
	release:	mixer_release,
	ioctl:		mixer_ioctl,
	owner:		THIS_MODULE
};


/*
 * Audio interface
 */

static long audio_samplerate = AUDIO_RATE_DEFAULT;

static void pxa_uda1342_i2c_set_samplerate(struct i2c_client *uda1342, long val)
{
	struct uda1342_cfg cfg;
	int clk_div = 0;

	/* wait for any frame to complete */
	udelay(125);

	if (val >= 48000)
		val = 48000;
	else if (val >= 44100)
		val = 44100;
	else if (val >= 22050)
		val = 22050;
	else if (val >= 16000)
		val = 16000;
	else
		val = 8000;

	/* Select the clock divisor */
	switch (val) {
	case 8000:
		cfg.fs = 256;
		clk_div = SADIV_BITCLK_512_00_KHZ;
		break;
	case 16000:
		cfg.fs = 256;
		clk_div = SADIV_BITCLK_1_024_MHZ;
		break;
	case 22050:
		cfg.fs = 256;
		clk_div = SADIV_BITCLK_1_418_MHZ;
		break;
	case 44100:
		cfg.fs = 256;
		clk_div = SADIV_BITCLK_2_836_MHZ;
		break;
	case 48000:
		cfg.fs = 256;
		clk_div = SADIV_BITCLK_3_072_MHZ;
		break;
	}

	cfg.format = UDA1342_SYSTEM_IF_I2S;
	i2c_command(uda1342, I2C_UDA1342_CONFIGURE, &cfg);
	PXA_SADIV = clk_div;
	audio_samplerate = val;
}

static void pxa_uda1342_i2c_audio_init(void *data)
{
	unsigned long flags;
	struct i2c_client *uda1342 = (struct i2c_client *)data;

	/* if codec is not active, init stuff */
	if (((struct uda1342 *)(uda1342->data))->active==0)
	{
		local_irq_save(flags);

		/* Enable I2S */
		set_GPIO_mode(GPIO28_BITCLK_OUT_I2S_MD);
		set_GPIO_mode(GPIO29_SDATA_IN_I2S_MD);
		set_GPIO_mode(GPIO30_SDATA_OUT_I2S_MD);
		set_GPIO_mode(GPIO31_SYNC_I2S_MD);
		set_GPIO_mode(GPIO32_SYSCLK_I2S_MD);

		/* Enable the clock to I2S unit */
		/*
			Note : I2S SysClock has to be enabled on UDA1342 power-on reset
			So this is just to be clean and give some reference
		*/
		CKEN |= CKEN8_I2S;
		PXA_SACR0 = SACR0_ENB | SACR0_BCKD_OUTPUT | SACR0_RFTH(8) | SACR0_TFTH(8);
		PXA_SACR1 = 0;
		PXA_SADIV = SADIV_BITCLK_2_836_MHZ; /* 44.1 KHz */

		local_irq_restore(flags);
	}

#ifdef DEBUG
	printk("I2S audio init completed\n");
#endif

	/* Initialize the UDA1342 internal state */
	i2c_command(uda1342, I2C_UDA1342_OPEN, 0);
}

static void pxa_uda1342_i2c_audio_shutdown(void *data)
{
	struct i2c_client *uda1342 = (struct i2c_client *)data;

	/* disable the audio power and all signals leading to the audio chip,
	excepted the clocks, only if codec is not active any more */
	i2c_command(uda1342, I2C_UDA1342_CLOSE, 0);

	if (((struct uda1342 *)(uda1342->data))->active==0)
	{
		PXA_SACR0 = SACR0_BCKD_OUTPUT;
		PXA_SACR1 = 0;
	}
}

static int pxa_uda1342_i2c_audio_ioctl(struct inode *inode, struct file *file,
			     uint cmd, ulong arg)
{
	long val;
	int ret = 0;
	audio_state_t *s = (audio_state_t *)file->private_data;
	struct i2c_client *uda1342 = s->data;

	/*
	 * These are platform dependent ioctls which are not handled by the
	 * generic pxa-audio module.
	 */
	switch (cmd) {
	case SNDCTL_DSP_STEREO:
		ret = get_user(val, (int *) arg);
		if (ret)
			return ret;
		/* the UDA1342 is stereo only */
		ret = (val == 0) ? -EINVAL : 1;
		return put_user(ret, (int *) arg);

	case SNDCTL_DSP_CHANNELS:
	case SOUND_PCM_READ_CHANNELS:
		/* the UDA1342 is stereo only */
		return put_user(2, (long *) arg);

	case SNDCTL_DSP_SPEED:
		ret = get_user(val, (long *) arg);
		if (ret) break;
		pxa_uda1342_i2c_set_samplerate(uda1342, val);
		/* fall through */

	case SOUND_PCM_READ_RATE:
		return put_user(audio_samplerate, (long *) arg);

	case SNDCTL_DSP_SETFMT:
	case SNDCTL_DSP_GETFMTS:
		/* we can do 16-bit only */
		return put_user(AFMT_S16_LE, (long *) arg);

	default:
		/* Maybe this is meant for the mixer (As per OSS Docs) */
		return mixer_ioctl(inode, file, cmd, arg);
	}

	return ret;
}


static audio_stream_t output_stream = {
	name:			"UDA1342 audio out",
	dcmd:			DCMD_TXPCDR,
	drcmr:			&DRCMRTXSADR,
	dev_addr:		__PREG(PXA_SADR),
};

static audio_stream_t input_stream = {
	name:			"UDA1342 audio in",
	dcmd:			DCMD_RXPCDR,
	drcmr:			&DRCMRRXSADR,
	dev_addr:		__PREG(PXA_SADR),
};

static audio_state_t audio_state = {
	output_stream:		&output_stream,
	input_stream:		&input_stream,
	hw_init:			pxa_uda1342_i2c_audio_init,
	hw_shutdown:		pxa_uda1342_i2c_audio_shutdown,
	client_ioctl:		pxa_uda1342_i2c_audio_ioctl,
	sem:			__MUTEX_INITIALIZER(audio_state.sem),
};

static int pxa_uda1342_i2c_audio_release(struct inode *inode, struct file *file)
{
	audio_state_t *s = (audio_state_t *)file->private_data;
	struct i2c_client *uda1342 = s->data;
	int res;

	i2c_dec_use_client(uda1342);

	res = pxa_audio_release(inode, file);

	/* TODO : when using pm and 2.6, remove this from here */
	pxa_uda1342_i2c_audio_shutdown(uda1342);

	return res;
}

static int pxa_uda1342_i2c_audio_open(struct inode *inode, struct file *file)
{
	int err;
	struct i2c_client *uda1342;

	uda1342 = i2c_get_client(I2C_DRIVERID_UDA1342, I2C_ALGO_PXA, NULL);

	if (uda1342 == NULL)
	{
		printk("No UDA1342 detected.  Is i2c-adap-pxa loaded?\n");
		return -ENODEV;
	}

	audio_state.data = uda1342;

	err = pxa_audio_attach(inode, file, &audio_state);

	/* TODO : when using pm and 2.6, remove this from here */
	pxa_uda1342_i2c_audio_init(uda1342);

	if (err == 0)
		i2c_inc_use_client(uda1342);

	return err;
}

/*
 * Missing fields of this structure will be patched with the call
 * to pxa_audio_attach().
 */
static struct file_operations pxa_uda1342_i2c_audio_fops = {
	open:		pxa_uda1342_i2c_audio_open,
	release:	pxa_uda1342_i2c_audio_release,
	owner:		THIS_MODULE
};


static int audio_dev_id, mixer_dev_id;

static int __init pxa_uda1342_i2c_module_init(void)
{
	/* register devices */
	audio_dev_id = register_sound_dsp(&pxa_uda1342_i2c_audio_fops, -1);
	mixer_dev_id = register_sound_mixer(&pxa_uda1342_i2c_mixer_fops, -1);

	/* try to bring in i2c support */
	request_module("i2c-adap-pxa");
	request_module("uda1342");

	printk( KERN_INFO "PXA25X UDA1342 I2C audio support initialized\n" );
	return 0;
}

static void __exit pxa_uda1342_i2c_module_exit(void)
{
	unregister_sound_dsp(audio_dev_id);
	unregister_sound_mixer(mixer_dev_id);
}

module_init(pxa_uda1342_i2c_module_init);
module_exit(pxa_uda1342_i2c_module_exit);

MODULE_AUTHOR("Stehane Magnenat, Nicolas Pitre, George France, Jamey Hicks");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Glue audio driver for the PXA25X & Philips UDA1342 codec using I2C.");

EXPORT_NO_SYMBOLS;
