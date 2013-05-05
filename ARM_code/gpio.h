/* gpio.h
 * Written by Ethan Trovillion and Ethan Warth
 * Part of the BlueLock project
 * ECE 395 Spring 2013
 * University of Illinois Urbana-Champaign
 */
#ifndef __GPIO_H__
#define __GPIO_H__

#include "LPC11xx.h"

/* Bit Masking Constants for GPIO port pins */
#define GPIO_P0  0x00000001
#define GPIO_P1  0x00000002
#define GPIO_P2  0x00000004
#define GPIO_P3  0x00000008
#define GPIO_P4  0x00000010
#define GPIO_P5  0x00000020
#define GPIO_P6  0x00000040
#define GPIO_P7  0x00000080
#define GPIO_P8  0x00000100
#define GPIO_P9  0x00000200
#define GPIO_P10 0x00000400
#define GPIO_P11 0x00000800
#define GPIO_ALL 0x00000fff

/* Test the state, enable, or disable the GPIO unit */
#define GPIO_status()  (LPC_SYSCON->SYSAHBCLKCTRL & 0x00000040)
#define GPIO_enable()  (LPC_SYSCON->SYSAHBCLKCTRL |= 0x00000040)
#define GPIO_disable() (LPC_SYSCON->SYSAHBCLKCTRL &= 0xffffffbf)
#define GPIO_interrupt_enable() NVIC_EnableIRQ((IRQn_Type) 31ul)
#define GPIO_interrupt_disable() NVIC_DisableIRQ((IRQn_Type) 31ul)

/* GPIOn Data */
#define GPIO0_data(p) (LPC_GPIO0->DATA & (p))
#define GPIO1_data(p) (LPC_GPIO1->DATA & (p))
#define GPIO2_data(p) (LPC_GPIO2->DATA & (p))
#define GPIO3_data(p) (LPC_GPIO3->DATA & (p))

/* Set output bit */
#define GPIO0_output_high(p) (LPC_GPIO0->DATA |= (p))
#define GPIO1_output_high(p) (LPC_GPIO1->DATA |= (p))
#define GPIO2_output_high(p) (LPC_GPIO2->DATA |= (p))
#define GPIO3_output_high(p) (LPC_GPIO3->DATA |= (p))

/* Clear output bit */
#define GPIO0_output_low(p) (LPC_GPIO0->DATA &= ~(p))
#define GPIO1_output_low(p) (LPC_GPIO1->DATA &= ~(p))
#define GPIO2_output_low(p) (LPC_GPIO2->DATA &= ~(p))
#define GPIO3_output_low(p) (LPC_GPIO3->DATA &= ~(p))

/* Toggle output bit */
#define GPIO0_output_toggle(p) (LPC_GPIO0->DATA ^= (p))
#define GPIO1_output_toggle(p) (LPC_GPIO1->DATA ^= (p))
#define GPIO2_output_toggle(p) (LPC_GPIO2->DATA ^= (p))
#define GPIO3_output_toggle(p) (LPC_GPIO3->DATA ^= (p))

/* GPIOn Direction Status */
#define GPIO0_dir_status(p) (LPC_GPIO0->DIR & (p))
#define GPIO1_dir_status(p) (LPC_GPIO1->DIR & (p))
#define GPIO2_dir_status(p) (LPC_GPIO2->DIR & (p))
#define GPIO3_dir_status(p) (LPC_GPIO3->DIR & (p))

/* GPIOn Enable input */
#define GPIO0_dir_input(p) (LPC_GPIO0->DIR &= ~(p))
#define GPIO1_dir_input(p) (LPC_GPIO1->DIR &= ~(p))
#define GPIO2_dir_input(p) (LPC_GPIO2->DIR &= ~(p))
#define GPIO3_dir_input(p) (LPC_GPIO3->DIR &= ~(p))

/* GPIOn Enable output */
#define GPIO0_dir_output(p) (LPC_GPIO0->DIR |= (p))
#define GPIO1_dir_output(p) (LPC_GPIO1->DIR |= (p))
#define GPIO2_dir_output(p) (LPC_GPIO2->DIR |= (p))
#define GPIO3_dir_output(p) (LPC_GPIO3->DIR |= (p))

/* GPIOn Interrupt Enable */
#define GPIO0_interrupt_enable(p) (LPC_GPIO0->IE |= (p))
#define GPIO1_interrupt_enable(p) (LPC_GPIO1->IE |= (p))
#define GPIO2_interrupt_enable(p) (LPC_GPIO2->IE |= (p))
#define GPIO3_interrupt_enable(p) (LPC_GPIO3->IE |= (p))

/* GPIOn Interrupt Disable */
#define GPIO0_interrupt_disable(p) (LPC_GPIO0->IE &= ~(p))
#define GPIO1_interrupt_disable(p) (LPC_GPIO1->IE &= ~(p))
#define GPIO2_interrupt_disable(p) (LPC_GPIO2->IE &= ~(p))
#define GPIO3_interrupt_disable(p) (LPC_GPIO3->IE &= ~(p))

/* GPIOn Interrupt Status */
#define GPIO0_interrupt_status() (LPC_GPIO0->MIS & 0x00000FFF)
#define GPIO1_interrupt_status() (LPC_GPIO1->MIS & 0x00000FFF)
#define GPIO2_interrupt_status() (LPC_GPIO2->MIS & 0x00000FFF)
#define GPIO3_interrupt_status() (LPC_GPIO2->MIS & 0x00000FFF)

/* GPIOn Clear Interrupt */
#define GPIO0_interrupt_clear(p) (LPC_GPIO0->IC |= (p))
#define GPIO1_interrupt_clear(p) (LPC_GPIO1->IC |= (p))
#define GPIO2_interrupt_clear(p) (LPC_GPIO2->IC |= (p))
#define GPIO3_interrupt_clear(p) (LPC_GPIO3->IC |= (p))

#endif
