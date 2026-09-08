/* Copyright (C) 2026 wqshao All rights reserved.
 *
 *  File Name    : opcode_asm.c
 *  Author       : wqshao
 *  Created Time : 2026-03-25 10:46:41
 *  Description  :
 */

#include <stdio.h>

int main(int argc, const char *argv[])
{
	asm volatile (
		".word 0xd503201f \t\n" //nop
		".word 0xd503201f \t\n" //nop
		".word 0xd503201f \t\n" //nop
		".word 0x2a0203e5 \t\n"
		".word 0x6b18005f \t\n"
		".word 0x54000309 \t\n"
		".word 0x34ff9dda \t\n"
		".word 0x384016c1 \t\n"
		".word 0xd503201f \t\n" //nop
		".word 0xd503201f \t\n" //nop
		".word 0xd503201f \t\n" //nop
		);
	return 0;
}
