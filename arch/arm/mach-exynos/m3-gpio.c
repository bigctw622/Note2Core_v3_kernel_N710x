/*
 *  linux/arch/arm/mach-exynos/c2-gpio.c
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS - GPIO setting in set board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/serial_core.h>
#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/gpio-cfg.h>
#include <plat/regs-serial.h>
#include <mach/gpio-midas.h>
#include <mach/pmu.h>

extern int s3c_gpio_slp_cfgpin(unsigned int pin, unsigned int config);
extern int s3c_gpio_slp_setpull_updown(unsigned int pin, unsigned int config);

struct gpio_init_data {
	uint num;
	uint cfg;
	uint val;
	uint pud;
	uint drv;
};

static struct gpio_init_data c2_init_gpios[] = {
	{GPIO_GSENSE_SDA_18V,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_GSENSE_SCL_18V,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_NFC_SDA_18V,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_NFC_SCL_18V,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},

	{GPIO_PMIC_SDA,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_PMIC_SCL,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_PMIC_DVS1,
	 S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_PMIC_IRQ,
	 S3C_GPIO_SFN(0xF), S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_NFC_IRQ,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},

	{GPIO_BT_WAKE,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_BT_HOST_WAKE,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_nPOWER,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},

	{GPIO_BUCK2_SEL,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{GPIO_BUCK3_SEL,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{GPIO_BUCK4_SEL,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},

	{GPIO_WLAN_EN,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_ZERO, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV4},
	{GPIO_WLAN_HOST_WAKE,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{GPIO_WLAN_SDIO_CMD,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_WLAN_SDIO_D0,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_WLAN_SDIO_D1,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_WLAN_SDIO_D2,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{GPIO_WLAN_SDIO_D3,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},

	{GPIO_FUEL_ALERT,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},

	{GPIO_V_BUS_INT,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{GPIO_WPC_INT,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},

	{GPIO_CAM_MCLK,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV3},
	{GPIO_VTCAM_MCLK,
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV3},

	/* NC */
	{EXYNOS4_GPA0(4),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPA0(5),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPA1(4),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPA1(5),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},

	{EXYNOS4_GPY0(0),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY0(1),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY0(2),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY0(3),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY0(4),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY0(5),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY1(2),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY1(3),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY2(1),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPY2(2),
	 S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE, S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	/* GPIO_AP2MDM_PMIC_RESET_N */
	{EXYNOS4_GPY2(3), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_UP, S5P_GPIO_DRVSTR_LV4},
};


static unsigned int c2_sleep_gpio_table[][3] = {
	{EXYNOS4_GPA0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(1), S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP},
	{EXYNOS4_GPA0(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPA0(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPA0(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA0(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPA1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPA1(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPB(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPB(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP},
	{EXYNOS4_GPB(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPB(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPC0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPC0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC0(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPC1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC1(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPD0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPD0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPD0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPD0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPD1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPD1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPD1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPD1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPF0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF0(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF0(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF0(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF0(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPF1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF1(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF1(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	/* GPF1(6) T0 LTE prev level, if not mdm notice it as crash  */
	{EXYNOS4_GPF1(6),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF1(7), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPF2(0), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF2(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF2(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF2(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF2(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPF2(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF2(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPF3(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF3(1), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF3(2), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF3(3), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF3(4), S3C_GPIO_SLP_OUT1,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF3(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPK0(0), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK0(1), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK0(2), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK0(3), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK0(4), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK0(5), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK0(6), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPK1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK1(3), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK1(4), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK1(5), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK1(6), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPK2(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK2(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPK3(0), S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK3(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPL0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL0(4), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPL0(6), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPL1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPL2(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL2(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPL2(6), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPL2(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	/* GPX1(0) VDDMIN (pda active) set to low at Sleep */
	{EXYNOS4_GPX1(0),  S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_DOWN},
	/* GPX3(2) T0 LTE HOST PORT PWR, hold previous level */
	{EXYNOS4_GPX3(2),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
#ifdef CONFIG_TARGET_LOCALE_EUR
	{EXYNOS4_GPX1(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* FM_RST */
#endif
	{EXYNOS4_GPY0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY2(0), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPY2(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY2(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	/* AP2MDM_PMIC_RESET_N */
	{EXYNOS4_GPY2(3),  S3C_GPIO_SLP_PREV,
						S3C_GPIO_PULL_UP},
	{EXYNOS4_GPY2(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPY2(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPY3(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY3(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY4(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY4(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY5(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY5(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY6(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY6(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPZ(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPZ(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPZ(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	/* Exynos4212 specific gpio */
	{EXYNOS4212_GPJ0(0), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(1), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPJ0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPJ0(4), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(5), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPJ1(0), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ1(1), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ1(2), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPJ1(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPM0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM0(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM0(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM1(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM1(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM1(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPM2(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM2(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM2(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM2(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM2(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPM3(0), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM3(1), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM3(2), S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	/* GPM3(3) set to pull-down for reducing sleep current consumption  */
	{EXYNOS4212_GPM3(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM3(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM4(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM4(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM4(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPV0(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV0(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPV1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV1(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPV2(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV2(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPV3(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(2), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(3), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(4), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(5), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(6), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV3(7), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPV4(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPV4(1), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
};

static unsigned int c2_sleep_gpio_table_rev01[][3] = {
  /* */
};


struct sleep_table {
	unsigned int (*ptr)[3];
	unsigned int size;
};

#define GPIO_TABLE(_ptr) \
{ \
	.ptr  = _ptr, \
	.size = ARRAY_SIZE(_ptr) \
} \

#define GPIO_TABLE_NULL \
{ \
	.ptr  = NULL, \
	.size = 0 \
} \

static struct sleep_table c2_sleep_table[] = {
	GPIO_TABLE(c2_sleep_gpio_table),	/* Rev0.0(0x0) */
	GPIO_TABLE(c2_sleep_gpio_table_rev01),	/* Rev0.0(0x1) */
};


static void config_sleep_gpio_table(unsigned int array_size,
				    unsigned int (*gpio_table)[3])
{
	u32 i, gpio;

	for (i = 0; i < array_size; i++) {
		gpio = gpio_table[i][0];
		s3c_gpio_slp_cfgpin(gpio, gpio_table[i][1]);
		s3c_gpio_slp_setpull_updown(gpio, gpio_table[i][2]);
	}
}

static void c2_config_sleep_gpio_table(void)
{
	int i;
	int index = min(ARRAY_SIZE(c2_sleep_table), system_rev + 1);

	for (i = 0; i < index; i++) {
		if (c2_sleep_table[i].ptr == NULL)
			continue;

		config_sleep_gpio_table(c2_sleep_table[i].size,
					c2_sleep_table[i].ptr);
	}
}


/* To save power consumption, gpio pin set before enterling sleep */
void midas_config_sleep_gpio_table(void)
{
	c2_config_sleep_gpio_table();
}

/* Intialize gpio set in midas board */
void midas_config_gpio_table(void)
{
	u32 i, gpio;

	printk(KERN_DEBUG "%s\n", __func__);

	for (i = 0; i < ARRAY_SIZE(c2_init_gpios); i++) {
		gpio = c2_init_gpios[i].num;
		if (gpio <= EXYNOS4212_GPV4(1)) {
			s3c_gpio_cfgpin(gpio, c2_init_gpios[i].cfg);
			s3c_gpio_setpull(gpio, c2_init_gpios[i].pud);

			if (c2_init_gpios[i].val != S3C_GPIO_SETPIN_NONE)
				gpio_set_value(gpio, c2_init_gpios[i].val);

			s5p_gpio_set_drvstr(gpio, c2_init_gpios[i].drv);
		}
	}
}
