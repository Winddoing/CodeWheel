#!/bin/bash
##########################################################
# Copyright (C) 2025 wqshao All rights reserved.
#  File Name    : dfd_pcie_debug.sh
#  Author       : wqshao
#  Created Time : 2025-11-04 09:23:45
#  Description  :
##########################################################

machine_type=`uname -m`

echo "machine_type: $machine_type"

## DEBUG REG
if [ "$machine_type" = "aarch64" ]; then
	lspci -nn

	SUBSYSTEM_GEN_COTROL_1=$(devmem 0xf7000d50) #0x50
	SUBSYSTEM_GEN_STATUS_1=$(devmem 0xf7000d54) #0x54
	SUBSYSTEM_GEN_STATUS_2=$(devmem 0xf7000d58) #0x58
	SUBSYSTEM_INT_COTROL_1=$(devmem 0xf7000d5c) #0x5c
	SUBSYSTEM_DEBUG_1=$(devmem 0xf7000d60) #0x60
	SUBSYSTEM_DEBUG_2=$(devmem 0xf7000d64) #0x64
	SUBSYSTEM_DEBUG_3=$(devmem 0xf7000d68) #0x68
	SUBSYSTEM_DEBUG_4=$(devmem 0xf7000d6c) #0x6c
	SUBSYSTEM_POWER_REQ=$(devmem 0xf7000d80) #0x80
else
	## 调试
	echo -e "\n\n ******** Debug Shell ********\n\n"
	SUBSYSTEM_GEN_COTROL_1=0x00000081
	SUBSYSTEM_GEN_STATUS_1=0xF0000080
	SUBSYSTEM_GEN_STATUS_2=0x0000FFD1
	SUBSYSTEM_INT_COTROL_1=0xfff50000
	SUBSYSTEM_DEBUG_1=0x038B3111
	SUBSYSTEM_DEBUG_1=0x03EAA311
	SUBSYSTEM_DEBUG_2=0x08000010
	SUBSYSTEM_DEBUG_3=0x00000000
	SUBSYSTEM_DEBUG_4=0x00000000
	SUBSYSTEM_POWER_REQ=0x00000300
fi

echo "SUBSYSTEM Gen COTROL 1(0x50) = $SUBSYSTEM_GEN_COTROL_1"
echo "SUBSYSTEM Gen STATUS 1(0x54) = $SUBSYSTEM_GEN_STATUS_1"
echo "SUBSYSTEM Gen STATUS 2(0x58) = $SUBSYSTEM_GEN_STATUS_2"
echo "SUBSYSTEM INT COTROL 1(0x5c) = $SUBSYSTEM_INT_COTROL_1"
echo "     SUBSYSTEM DEBUG 1(0x60) = $SUBSYSTEM_DEBUG_1"		#cxpl_debug_info[31:0]
echo "     SUBSYSTEM DEBUG 2(0x64) = $SUBSYSTEM_DEBUG_2"  		#cxpl_debug_info[63:32]
echo "     SUBSYSTEM DEBUG 3(0x68) = $SUBSYSTEM_DEBUG_3"
echo "     SUBSYSTEM DEBUG 4(0x6c) = $SUBSYSTEM_DEBUG_4"
echo "   SUBSYSTEM POWER REQ(0x80) = $SUBSYSTEM_POWER_REQ"

###################################################################################################

rec_det_str[0x00]="No device"
rec_det_str[0xc0]="X2"
rec_det_str[0xf0]="X4"
rec_det_str[0xff]="X8"

# PIPE spec Table 6-4: Status Interface Signals
phy_powerdown_str[0]="P0, normal operation"
phy_powerdown_str[1]="P0s, low recovery time latency,power saving state"
phy_powerdown_str[2]="P1, longer recovery time latency, lower power state"
phy_powerdown_str[3]="P2, lowest power state"
phy_powerdown_str[4]="POWER_STATE_4 Phy specific"
phy_powerdown_str[5]="POWER_STATE_5 Phy specific"
phy_powerdown_str[6]="POWER_STATE_6 Phy specific"
phy_powerdown_str[7]="POWER_STATE_7 Phy specific"
phy_powerdown_str[8]="POWER_STATE_8 Phy specific"
phy_powerdown_str[9]="POWER_STATE_9 Phy specific"
phy_powerdown_str[10]="POWER_STATE_10 Phy specific"
phy_powerdown_str[11]="POWER_STATE_11 Phy specific"
phy_powerdown_str[12]="POWER_STATE_12 Phy specific"
phy_powerdown_str[13]="POWER_STATE_13 Phy specific"
phy_powerdown_str[14]="POWER_STATE_14 Phy specific"
phy_powerdown_str[15]="POWER_STATE_15 Phy specific"

phy_rate_str[0]="2.5 GT/s"
phy_rate_str[1]="5.0 GT/s"
phy_rate_str[2]="8.0 GT/s"
phy_rate_str[3]="16.0 GT/s"

pm_curnt_state_str[0]="3'b000: L0 and others"
pm_curnt_state_str[1]="3'b001: L0s"
pm_curnt_state_str[2]="3'b010: L1"
pm_curnt_state_str[3]="3'b011: L2"
pm_curnt_state_str[4]="3'b100: L3"

pm_dstate_str[0]="000b: D0"
pm_dstate_str[1]="001b: D1"
pm_dstate_str[2]="010b: D2"
pm_dstate_str[3]="011b: D3"
pm_dstate_str[4]="100b: Uninitialized"

echo -e "\n"
echo "===> 54_Subsystem General Status 1:"
printf "  RECEIVER_DETECTED[31:24](RO) = 0x%02x (%s) : 检测到对端存在\n" $((SUBSYSTEM_GEN_STATUS_1 >> 24 & 0xff)) "${rec_det_str[$((SUBSYSTEM_GEN_STATUS_1 >> 24 & 0xff))]}"
printf "   PM_L1_ENTRY_STARTED[23](RO) = 0x%x : 当前链路开始进入 L1 state\n" $((SUBSYSTEM_GEN_STATUS_1 >> 23 & 0x1))
printf "     PM_LINKST_L2_EXIT[22](RO) = 0x%x : 当前链路正在退出 L2 state\n" $((SUBSYSTEM_GEN_STATUS_1 >> 22 & 0x1))
printf "       PM_LINKST_IN_L2[21](RO) = 0x%x : 当前链路处于 L2 state\n" $((SUBSYSTEM_GEN_STATUS_1 >> 21 & 0x1))
printf "       PM_LINKST_IN_L1[20](RO) = 0x%x : 当前链路处于 L1 state\n" $((SUBSYSTEM_GEN_STATUS_1 >> 20 & 0x1))
printf "      PM_LINKST_IN_L0S[19](RO) = 0x%x : 电源管理处于 L0s state\n" $((SUBSYSTEM_GEN_STATUS_1 >> 19 & 0x1))
printf "       PHY_POWRDOWN[18:16](RO) = 0x%x (%s) : 当前PHY的powerdown状态\n" $((SUBSYSTEM_GEN_STATUS_1 >> 16 & 0x7)) "${phy_powerdown_str[$((SUBSYSTEM_GEN_STATUS_1 >> 16 & 0x7))]}"
printf "           PHY_RATE[15:14](RO) = 0x%x (%s) : 当前PHY的速度状态\n" $((SUBSYSTEM_GEN_STATUS_1 >> 14 & 0x3)) "${phy_rate_str[$((SUBSYSTEM_GEN_STATUS_1 >> 14 & 0x3))]}"
printf "          CDM_IN_RESET[13](RO) = 0x%x : 配置空间的寄存器还处于复位状态，即不可以访问状态\n" $((SUBSYSTEM_GEN_STATUS_1 >> 13 & 0x1))
printf " BRDG_SLV_XFER_PENDING[12](RO) = 0x%x : AXI Slave non-DBI 通道有包要进行发送\n" $((SUBSYSTEM_GEN_STATUS_1 >> 12 & 0x1))
printf " BRDG_DBI_XFER_PENDING[11](RO) = 0x%x : AXI Slave DBI 接口有包要进行发送\n" $((SUBSYSTEM_GEN_STATUS_1 >> 11 & 0x1))
printf "     RADM_XFER_PENDING[10](RO) = 0x%x : 接收队列有包处于被处理状态\n" $((SUBSYSTEM_GEN_STATUS_1 >> 10 & 0x1))
printf "        VC0_Q_NOT_EMPTY[9](RO) = 0x%x : 接收队列不为空，即当前依然有包在接收缓存中待处理\n" $((SUBSYSTEM_GEN_STATUS_1 >> 9 & 0x1))
printf "           VC_QOVERFLOW[8](RO) = 0x%x : 接收队列发生了溢出\n" $((SUBSYSTEM_GEN_STATUS_1 >> 8 & 0x1))
printf "         PHY_EXIT_RESET[7](RO) = 0x%x : PHY退出了复位状态，即所有lane完成初始化拉起phystatus信号告诉controller可以撤销复位了\n" $((SUBSYSTEM_GEN_STATUS_1 >> 7 & 0x1))
# PM_CURNT_STATE 状态在插不插设备时，读取一直为0, 切换到L2时显示正常
printf "       PM_CURNT_STATE[6:4](RO) = 0x%x (%s) : PM controller中显示当前power状态(表示当前电源状态pm_curnt_state输出用于调试目的)\n" $((SUBSYSTEM_GEN_STATUS_1 >> 4 & 0x7)) "${pm_curnt_state_str[$((SUBSYSTEM_GEN_STATUS_1 >> 4 & 0x7))]}"
printf "              PM_PME_EN[3](RO) = 0x%x : PMCSR寄存器中的PME Enable位\n" $((SUBSYSTEM_GEN_STATUS_1 >> 3 & 0x1))
printf "            PM_DSTATE[2:0](RO) = 0x%x (%s) : 当前设备的D-state\n" $((SUBSYSTEM_GEN_STATUS_1 >> 0 & 0x7)) "${pm_dstate_str[$((SUBSYSTEM_GEN_STATUS_1 >> 0 & 0x7))]}"



ltssm_state_str[0]="6'h00: S_DETECT_QUIET"
ltssm_state_str[1]="6'h01: S_DETECT_ACT"
ltssm_state_str[2]="6'h02: S_POLL_ACTIVE"
ltssm_state_str[3]="6'h03: S_POLL_COMPLIANCE"
ltssm_state_str[4]="6'h04: S_POLL_CONFIG"
ltssm_state_str[5]="6'h05: S_PRE_DETECT_QUIET"
ltssm_state_str[6]="6'h06: S_DETECT_WAIT"
ltssm_state_str[7]="6'h07: S_CFG_LINKWD_START"
ltssm_state_str[8]="6'h08: S_CFG_LINKWD_ACEPT"
ltssm_state_str[9]="6'h09: S_CFG_LANENUM_WAI"
ltssm_state_str[10]="6'h0A: S_CFG_LANENUM_ACEPT"
ltssm_state_str[11]="6'h0B: S_CFG_COMPLETE"
ltssm_state_str[12]="6'h0C: S_CFG_IDLE"
ltssm_state_str[13]="6'h0D: S_RCVRY_LOCK"
ltssm_state_str[14]="6'h0E: S_RCVRY_SPEED"
ltssm_state_str[15]="6'h0F: S_RCVRY_RCVRCFG"
ltssm_state_str[16]="6'h10: S_RCVRY_IDLE"
ltssm_state_str[17]="6'h11: S_L0"
ltssm_state_str[18]="6'h12: S_L0S"
ltssm_state_str[19]="6'h13: S_L123_SEND_EIDLE"
ltssm_state_str[20]="6'h14: S_L1_IDLE"
ltssm_state_str[21]="6'h15: S_L2_IDLE"
ltssm_state_str[22]="6'h16: S_L2_WAKE"
ltssm_state_str[23]="6'h17: S_DISABLED_ENTRY"
ltssm_state_str[24]="6'h18: S_DISABLED_IDLE"
ltssm_state_str[25]="6'h19: S_DISABLED"
ltssm_state_str[26]="6'h1A: S_LPBK_ENTRY"
ltssm_state_str[27]="6'h1B: S_LPBK_ACTIVE"
ltssm_state_str[28]="6'h1C: S_LPBK_EXIT"
ltssm_state_str[29]="6'h1D: S_LPBK_EXIT_TIMEOUT"
ltssm_state_str[30]="6'h1E: S_HOT_RESET_ENTRY"
ltssm_state_str[31]="6'h1F: S_HOT_RESET"
ltssm_state_str[32]="6'h20: S_RCVRY_EQ0"
ltssm_state_str[33]="6'h21: S_RCVRY_EQ1"
ltssm_state_str[34]="6'h22: S_RCVRY_EQ2"
ltssm_state_str[35]="6'h23: S_RCVRY_EQ3"

echo -e "\n"
echo "===> 58_Subsystem General Status 2: Link UP LTSSM STATUS"
printf "             INT_STS[31:16](R) = 0x%04x (%s) : 中断状态\n" $((SUBSYSTEM_GEN_STATUS_2 >> 16 & 0xffff)) $([ $((SUBSYSTEM_GEN_STATUS_2 >> 16 & 0xffff)) -eq 0 ] && echo "无中断" || echo "有中断")
printf "     LANE_EXIT_RESET[15:8](RO) = 0x%02x : 每条lane的退出复位标志，每个通道1bit\n" $((SUBSYSTEM_GEN_STATUS_2 >> 8 & 0xff))
printf "           RDLH_LINK_UP[7](RO) = 0x%x : 数据链路层连接成功\n" $((SUBSYSTEM_GEN_STATUS_2 >> 7 & 0x1))
printf "           SMLH_LINK_UP[6](RO) = 0x%x : 物理链路层训练连接成功\n" $((SUBSYSTEM_GEN_STATUS_2 >> 6 & 0x1))
printf "     SMLH_LTSSM_STATE[5:0](RO) = 0x%x (%s) : 物理层状态机实时状态\n" $((SUBSYSTEM_GEN_STATUS_2 >> 0 & 0x3f)) "${ltssm_state_str[$((SUBSYSTEM_GEN_STATUS_2 >> 0 & 0x3f))]}"

echo -e "\n"
echo "===> 5C_Subsystem Interrupt Control 1: INT"
printf "                  INT_EN[31:0] = 0x%08x : 中断使能\n" $SUBSYSTEM_INT_COTROL_1


echo -e "\n"
echo "===> 80_Subsystem Transmit Message AND Power Request: Power Request"
printf "              APP_CLK_PM_EN[9] = 0x%x : 用户逻辑支持Clock PM feature，也可以用于移除\n" $((SUBSYSTEM_POWER_REQ >> 9 & 0x1))
printf "                    CLK_REQ[8] = 0x%x : 用户逻辑已经准备好进入移除reference clock\n" $((SUBSYSTEM_POWER_REQ >> 8 & 0x1))
printf "                 UNLOCK_REQ[7] = 0x%x : 发送unlock message\n" $((SUBSYSTEM_POWER_REQ >> 7 & 0x1))
printf "               XFER_PENDING[6] = 0x%x : 用户准备发包阻止IP进入到低功耗状态\n" $((SUBSYSTEM_POWER_REQ >> 6 & 0x1))
printf "            READY_ENTER_L23[5] = 0x%x : 本端的用户逻辑已经准备好进入Application Ready to Enter L23状态，用于低功耗L2状态的进入\n" $((SUBSYSTEM_POWER_REQ >> 5 & 0x1))
printf "           PME_TURN_OFF_REQ[4] = 0x%x : 发送PME_Turn_Off消息\n" $((SUBSYSTEM_POWER_REQ >> 4 & 0x1))
printf "                 PM_XMT_PME[3] = 0x%x : 发送PME消息\n" $((SUBSYSTEM_POWER_REQ >> 3 & 0x1))
printf "          OUTBAND_PWRUP_CMD[2] = 0x%x : 发送PME消息\n" $((SUBSYSTEM_POWER_REQ >> 2 & 0x1))
printf "                REQ_EXIT_L1[1] = 0x%x : 退出L1请求\n" $((SUBSYSTEM_POWER_REQ >> 1 & 0x1))
# 0x1 (IDLE_TO_L1)：当控制器持续空闲一段时间后，将进入ASPM L1状态
# 0x0 (L0S_TO_L1)：仅当收发双方均处于L0s状态并持续空闲后，控制器才会进入ASPM L1状态
printf "              ENTER_ASPM_L1[0] = 0x%x : 进入ASPM请求\n" $((SUBSYSTEM_POWER_REQ >> 0 & 0x1))




# reg
# SUBSYSTEM_DEBUG_1 —— cxpl_debug_info[31:0]
# SUBSYSTEM_DEBUG_2 —— cxpl_debug_info[63:32]
DEBUG_INFO=$(( (SUBSYSTEM_DEBUG_2 << 32) | SUBSYSTEM_DEBUG_1 ))
DEBUG_INFO_EI=$SUBSYSTEM_DEBUG_3

echo -e "\n\n"
echo "--------------> DFD DWC DEBUG REG:"
printf "         DEBUG_INFO[63:0] = 0x%016x\n" $DEBUG_INFO
printf "      DEBUG_INFO_EI[15:0] = 0x%04x\n" $DEBUG_INFO_EI


echo -e "\n"
echo "===> DEBUG_INFO: cxpl_debug_info[63:0](O)"
printf "      smlh_scrambler_disable[63] = 0x%x : 链路加扰功能已禁用\n" $(( DEBUG_INFO >> 63 & 0x1))
printf "           smlh_link_disable[62] = 0x%x : TSSM处于禁用状态，链路不可操作\n" $(( DEBUG_INFO >> 62 & 0x1))
printf "       smlh_link_in_training[61] = 0x%x : LTSSM正在执行链路训练\n" $(( DEBUG_INFO >> 61 & 0x1))
printf "    smlh_ltssm_in_pollconfig[60] = 0x%x : LTSSM正处于Polling.Configuration状态\n" $(( DEBUG_INFO >> 60 & 0x1))
printf "         smlh_training_rst_n[59] = 0x%x : LTSSM协商链路复位\n" $(( DEBUG_INFO >> 59 & 0x1))
printf "          0000b: Reserved[58:55] = 0x%x : \n" $(( DEBUG_INFO >> 55 & 0xf))
printf " mac_phy_txdetectrx_loopback[54] = 0x%x : PIPE接收端检测/环回模式请求\n" $(( DEBUG_INFO >> 54 & 0x1))
printf "       mac_phy_txelecidle[0][53] = 0x%x : PIPE发送电气空闲请求\n" $(( DEBUG_INFO >> 53 & 0x1))
printf "     mac_phy_txcompliance[0][52] = 0x%x : PIPE 发送端兼容性测试请求\n" $(( DEBUG_INFO >> 52 & 0x1))
printf "                app_init_rst[51] = 0x%x : 应用程序发起的训练重置请求\n" $(( DEBUG_INFO >> 51 & 0x1))
printf "           000b: Reserved[50:48] = 0x%x : \n" $(( DEBUG_INFO >> 48 & 0x7))
printf "         rmlh_ts_link_num[47:40] = 0x%02x : 链路伙伴发布/确认的链路号\n" $(( DEBUG_INFO >> 40 & 0xff))
printf "            00b: Reserved[39:38] = 0x%x : \n" $(( DEBUG_INFO >> 38 & 0x3))
printf "           xmtbyte_skip_sent[37] = 0x%x : 已发送SKP有序集\n" $(( DEBUG_INFO >> 37 & 0x1))
printf "                smlh_link_up[36] = 0x%x : LTSSM报告物理链路已就绪 / 对于环回主设备而言，LTSSM处于Loopback.Active状态\n" $(( DEBUG_INFO >> 36 & 0x1))
printf "             smlh_inskip_rcv[35] = 0x%x : 接收端报告SKP有序集接收状态\n" $(( DEBUG_INFO >> 35 & 0x1))
printf "               smlh_ts1_rcvd[34] = 0x%x : 检测到TS1训练序列（脉冲信号）\n" $(( DEBUG_INFO >> 34 & 0x1))
printf "               smlh_ts2_rcvd[33] = 0x%x : 检测到TS2训练序列（脉冲信号）\n" $(( DEBUG_INFO >> 33 & 0x1))
printf "          smlh_rcvd_lane_rev[32] = 0x%x : 接收端检测到通道翻转(reversal)\n" $(( DEBUG_INFO >> 32 & 0x1))
printf "        smlh_ts_link_ctrl[31:28] = 0x%x : 链路伙伴通告的链路控制位\n" $(( DEBUG_INFO >> 28 & 0xf))
printf "    smlh_ts_lane_num_is_k237[27] = 0x%x : 当前正在接收k28.7（PAD）字符替代通道编号\n" $(( DEBUG_INFO >> 27 & 0x1))
printf "    smlh_ts_link_num_is_k237[26] = 0x%x : 当前正在接收k28.7（PAD）字符替代链路编号\n" $(( DEBUG_INFO >> 26 & 0x1))
printf "           rmlh_rcvd_idle[0][25] = 0x%x : 接收器正在接收逻辑空闲序列\n" $(( DEBUG_INFO >> 25 & 0x1))
printf "           rmlh_rcvd_idle[1][24] = 0x%x : （仅适用于16位物理层(PHY)接口）第2n个符号亦为空闲状态\n" $(( DEBUG_INFO >> 24 & 0x1))
printf "            mac_phy_txdata[23:8] = 0x%04x : PIPE接口发送数据\n" $(( DEBUG_INFO >> 8 & 0xffff))
printf "            mac_phy_txdatak[7:6] = 0x%x : PIPE发送端K字符指示信号\n" $(( DEBUG_INFO >> 6 & 0x3))
printf "           smlh_ltssm_state[5:0] = 0x%x (%s) : LTSSM当前状态\n" $(( DEBUG_INFO >> 0 & 0x3f)) "${ltssm_state_str[$(( DEBUG_INFO >> 0 & 0x3f))]}"

echo -e "\n"
echo "===> DEBUG_INFO_EI: cxpl_debug_info_ei[15:0](O)"
printf "Group 1 (pulse) - 接收有序集的相关信息:\n"
printf "                                          EIOS detected [0] = 0x%x\n" $(( DEBUG_INFO_EI >> 0 & 0x1))
printf "Group 2 (level) - LTSSM正处于依赖接收端电气空闲信号（rxelecidle=0）的状态之一:\n"
printf "                                                     L1 [1] = 0x%x\n" $(( DEBUG_INFO_EI >> 1 & 0x1))
printf "                                                     L2 [2] = 0x%x\n" $(( DEBUG_INFO_EI >> 2 & 0x1))
printf "                                                  RxL0s [3] = 0x%x\n" $(( DEBUG_INFO_EI >> 3 & 0x1))
printf "                                               Disabled [4] = 0x%x\n" $(( DEBUG_INFO_EI >> 4 & 0x1))
printf "                                           Detect.Quiet [5] = 0x%x\n" $(( DEBUG_INFO_EI >> 5 & 0x1))
printf "                                         Polling.Active [6] = 0x%x\n" $(( DEBUG_INFO_EI >> 6 & 0x1))
printf "                                     Polling.Compliance [7] = 0x%x\n" $(( DEBUG_INFO_EI >> 7 & 0x1))
printf "Group 3 (level) - LTSSM正处于依赖接收端电气空闲信号（rxelecidle=1）的状态之一:\n"
printf "       LTSSM is in a transitory state prior to L1 or L2 [8] = 0x%x\n" $(( DEBUG_INFO_EI >> 8 & 0x1))
printf "       LTSSM is in a transitory state prior to Disabled [9] = 0x%x\n" $(( DEBUG_INFO_EI >> 9 & 0x1))
printf "        LTSSM is in Loopback.Active as a Slave at Gen1 [10] = 0x%x\n" $(( DEBUG_INFO_EI >> 10 & 0x1))
printf "                            LTSSM is in Polling.Active [11] = 0x%x\n" $(( DEBUG_INFO_EI >> 11 & 0x1))
printf "Group 4 (pulse) - LTSSM状态转换由电气空闲隐式触发:\n"
printf "        LTSSM enters Recovery from L0 with EI inferred [12] = 0x%x\n" $(( DEBUG_INFO_EI >> 12 & 0x1))
printf "             LTSSM enters Recovery.Speed from Recovery [13] = 0x%x\n" $(( DEBUG_INFO_EI >> 13 & 0x1))
printf "             EI inferred while LTSSM in Recovery.Speed [14] = 0x%x\n" $(( DEBUG_INFO_EI >> 14 & 0x1))
printf " EI inferred while LTSSM in Loopback.Active as a slave [15] = 0x%x\n" $(( DEBUG_INFO_EI >> 15 & 0x1))


des_tba_ctrl_str[0]="2'b00: No action"
des_tba_ctrl_str[1]="2'b01: Start"
des_tba_ctrl_str[2]="2'b10: End"
des_tba_ctrl_str[3]="2'b11: Reserved"

echo -e "\n"
echo "===> DEBUG Status 4 (写控制)"
printf " RAS_DES_SD_HOLD_LTSSM[8](I) = 0x%x : 保持与释放LTSSM状态控制信号。只要该信号维持'1'电平，控制器将始终保持在当前LTSSM状态\n" $(( SUBSYSTEM_DEBUG_4 >> 8 & 0x1))
printf "    RAS_DES_TBA_CTRL[5:4](I) = 0x%x (%s) : 控制基于时间的分析的启动/停止\n" $(( SUBSYSTEM_DEBUG_4 >> 4 & 0x3)) "${des_tba_ctrl_str[$(( SUBSYSTEM_DEBUG_4 >> 4 & 0x3))]}"
printf "       DIAG_CTRL_BUS[2:0](I) = 0x%x : 诊断控制总线\n" $(( SUBSYSTEM_DEBUG_4 >> 0 & 0x7))



[ "$machine_type" = "aarch64" ] || exit

## VSEC Capability
echo -e "\n\n"
echo "************************************ VSEC Cap ***********************************************"
CFG_BASE=0xf7000000

ltssm_val_str[0]="directed_speed_change"
ltssm_val_str[1]="changed_speed_recovery"
ltssm_val_str[2]="successful_speed_negotiation"
ltssm_val_str[3]="upconfigure_capable"
ltssm_val_str[4]="select_deemphasis"
ltssm_val_str[5]="start_equalization_w_preset"
ltssm_val_str[6]="equalization_done_8GT_data_rate"
ltssm_val_str[7]="equalization_done_16GT_data_rate"
ltssm_val_str[8]="Undefined"

intrl_pm_sstate_str[0]="S_IDLE"
intrl_pm_sstate_str[1]="S_RESPOND_NAK"
intrl_pm_sstate_str[2]="S_BLOCK_TLP"
intrl_pm_sstate_str[3]="S_WAIT_LAST_TLP_ACK":
intrl_pm_sstate_str[4]="S_WAIT_EIDLE"
intrl_pm_sstate_str[5]="S_LINK_ENTR_L1"
intrl_pm_sstate_str[6]="S_L1"
intrl_pm_sstate_str[7]="S_L1_EXIT"
intrl_pm_sstate_str[8]="S_L23RDY"
intrl_pm_sstate_str[9]="S_LINK_ENTR_L23"
intrl_pm_sstate_str[10]="S_L23RDY_WAIT4ALIVE"
intrl_pm_sstate_str[11]="S_ACK_WAIT4IDLE"
intrl_pm_sstate_str[12]="S_WAIT_LAST_PMDLLP"


intrl_pm_mstate_str[0]="IDLE"
intrl_pm_mstate_str[1]="L0"
intrl_pm_mstate_str[2]="L0S"
intrl_pm_mstate_str[3]="ENTER_L0S"
intrl_pm_mstate_str[4]="EXIT_L0S"
intrl_pm_mstate_str[5]="WAIT_PMCSR_CPL_SENT"
intrl_pm_mstate_str[6]="Rsv"
intrl_pm_mstate_str[7]="Rsv"
intrl_pm_mstate_str[8]="L1"
intrl_pm_mstate_str[9]="L1_BLOCK_TLP"
intrl_pm_mstate_str[10]="L1_WAIT_LAST_TLP_ACK"
intrl_pm_mstate_str[11]="L1_WAIT_PMDLLP_ACK"
intrl_pm_mstate_str[12]="L1_LINK_ENTR_L1"
intrl_pm_mstate_str[13]="L1_EXIT"
intrl_pm_mstate_str[14]="Rsv"
intrl_pm_mstate_str[15]="PREP_4L1"
intrl_pm_mstate_str[16]="L23_BLOCK_TLP"
intrl_pm_mstate_str[17]="L23_WAIT_LAST_TLP_ACK"
intrl_pm_mstate_str[18]="L23_WAIT_PMDLLP_ACK"
intrl_pm_mstate_str[19]="L23_ENTR_L23"
intrl_pm_mstate_str[20]="L23RDY"
intrl_pm_mstate_str[21]="PREP_4L23"
intrl_pm_mstate_str[22]="L23RDY_WAIT4ALIVE"
intrl_pm_mstate_str[23]="L0S_BLOCK_TLP"
intrl_pm_mstate_str[24]="WAIT_LAST_PMDLLP"
intrl_pm_mstate_str[25]="WAIT_DSTATE_UPDATE"

dlcmsm_str[0]="DL_INACTIVE"
dlcmsm_str[1]="DL_FC_INIT"
dlcmsm_str[3]="DL_ACTIVE"

#Capabilities: [1ac v1] Vendor Specific Information: ID=0002 Rev=4 Len=100 <?> —— 3.26 DWC_PCIE_DSP/PF0_RAS_DES_CAP Registers
echo "===> DWC_PCIE_DSP/PF0_RAS_DES_CAP(0x1ac)"
RAS_DES_CAP_ADDR=0x1ac
for off in `seq 0 232`
do
	[ $((off % 4)) -ne 0 ] && continue

	val=$(devmem $((CFG_BASE + RAS_DES_CAP_ADDR + off)))
	printf "Base: 0x%03x, Offset: 0x%02x, value=0x%08x\n" $((RAS_DES_CAP_ADDR + off)) $off $val

	off=$(printf "0x%x" $off)
	case $off in
		0x0)
			printf "\tRAS_DES_CAP_HEADER_REG: 0x%08x\n" $val
			printf "\t                        NEXT_OFFSET[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                        CAP_VERSION[19:16] = 0x%x\n" $((val >> 16 & 0xf))
			printf "\t                     EXTENDED_CAP_ID[15:0] = 0x%x\n" $((val >> 0 & 0xffff))
			;;
		0x4)
			printf "\tVENDOR_SPECIFIC_HEADER_REG: 0x%08x\n" $val
			printf "\t                        VSEC_LENGTH[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                           VSEC_REV[19:16] = 0x%x\n" $((val >> 16 & 0xf))
			printf "\t                             VSEC_ID[15:0] = 0x%x\n" $((val >> 0 & 0xffff))
			;;

		0xb0)
			printf "\tSD_STATUS_L1LANE_REG: 0x%08x\n" $val
			printf "\t                     DESKEW_POINTER[31:24] = 0x%x : 偏移校正指针\n" $((val >> 24 & 0xff))
			printf "\t                       PIPE_TXELECIDLE[20] = 0x%x : PIPE:TxElecIdle\n" $((val >> 20 & 0x1))
			printf "\t                       PIPE_RXELECIDLE[19] = 0x%x : PIPE:RxElecIdle\n" $((val >> 19 & 0x1))
			printf "\t                          PIPE_RXVALID[18] = 0x%x : PIPE:RxValid\n" $((val >> 18 & 0x1))
			printf "\t                      PIPE_DETECT_LANE[17] = 0x%x : PIPE:Detect Lane\n" $((val >> 17 & 0x1))
			printf "\t                       PIPE_RXPOLARITY[16] = 0x%x : PIPE:RxPolarity\n" $((val >> 16 & 0x1))
			printf "\t                          LANE_SELECT[3:0] = 0x%x : lane选择（R/W）\n" $((val >> 0 & 0xf))
			;;

		0xb4)
			printf "\tSD_STATUS_L1LTSSM_REG: 0x%08x\n" $val
			printf "\t                     LTSSM_VARIABLE[31:16] = 0x%x (%s) : LTSSM变量\n" $((val >> 16 & 0xffff)) "${ltssm_val_str[$((val >> 16 & 0xffff))]}"
			printf "\t                         LANE_REVERSAL[15] = 0x%x : Lane反转（接收端检测到Lane反转）\n" $((val >> 15 & 0x1))
			printf "\t                     PIPE_POWER_DOWN[10:8] = 0x%x (%s) : PIPE:PowerDown\n" $((val >> 8 & 0x7)) "${phy_powerdown_str[$((val >> 8 & 0x7))]}"
			printf "\t                            FRAMING_ERR[7] = 0x%x : 帧错误\n" $((val >> 7 & 0x1))
			printf "\t                      FRAMING_ERR_PTR[6:0] = 0x%x : 标识首次出现的成帧错误类型\n" $((val >> 0 & 0x7f))
			;;
		0xb8)
			printf "\tSD_STATUS_PM_REG: 0x%08x\n" $val
			printf "\t                       LATCHED_NFTS[23:16] = 0x%x\n" $((val >> 16 & 0xff))
			printf "\t                        L1SUB_STATE[15:13] = 0x%x\n" $((val >> 13 & 0x7))
			printf "\t                       PME_RESEND_FLAG[12] = 0x%x : PME重发标志\n" $((val >> 12 & 0x1))
			printf "\t                  INTERNAL_PM_SSTATE[11:8] = 0x%x (%s) : 内部PM状态（从属）\n" $((val >> 8 & 0xf)) "${intrl_pm_sstate_str[$((val >> 8 & 0xf))]}"
			printf "\t                   INTERNAL_PM_MSTATE[4:0] = 0x%x (%s) : 内部PM状态（主控）\n" $((val >> 0 & 0x1f)) "${intrl_pm_mstate_str[$((val >> 0 & 0x1f))]}"
			;;
		0xbc)
			printf "\tSD_STATUS_L2_REG: 0x%08x\n" $val
			printf "\t                              FC_INIT2[27] = 0x%x\n" $((val >> 27 & 0x1))
			printf "\t                              FC_INIT1[26] = 0x%x\n" $((val >> 16 & 0x1))
			printf "\t                             DLCMSM[25:24] = 0x%x (%s) : DLCMSM（ Data Link Control and Management State Machine）状态\n" $((val >> 24 & 0x3)) "${dlcmsm_str[$((val >> 24 & 0x3))]}"
			printf "\t                      RX_ACK_SEQ_NO[23:12] = 0x%x : Tx Ack序列号\n" $((val >> 12 & 0xfff))
			printf "\t                       TX_TLP_SEQ_NO[11:0] = 0x%x : Tx TLP序列号\n" $((val >> 0 & 0xfff))
			;;
		0xc0)
			printf "\tSD_STATUS_L3FC_REG: 0x%08x\n" $val
			printf "\t                       CREDIT_DATA1[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                        CREDIT_DATA0[19:8] = 0x%x\n" $((val >> 8 & 0xfff))
			printf "\t                          CREDIT_SEL_HD[6] = 0x%x\n" $((val >> 6 & 0x1))
			printf "\t                  CREDIT_SEL_TLP_TYPE[5:4] = 0x%x\n" $((val >> 4 & 0x3))
			printf "\t                 CREDIT_SEL_CREDIT_TYPE[3] = 0x%x\n" $((val >> 3 & 0x1))
			printf "\t                        CREDIT_SEL_VC[2:0] = 0x%x\n" $((val >> 0 & 0x7))
			;;
	esac
done
echo -e "\n"



#Capabilities: [2ac v1] Vendor Specific Information: ID=0001 Rev=1 Len=038 <?> —— 3.27 DWC_PCIE_DSP/PF0_VSECRAS_CAP Registers
echo "===> DWC_PCIE_DSP/PF0_VSECRAS_CAP(0x2ac)"
VSERAS_CAP_ADDR=0x2ac
for off in `seq 0 52`
do
	[ $((off % 4)) -ne 0 ] && continue

	val=$(devmem $((CFG_BASE + VSERAS_CAP_ADDR + off)))
	printf "Base: 0x%03x, Offset: 0x%02x, value=0x%08x\n" $((VSERAS_CAP_ADDR + off)) $off $val

	off=$(printf "0x%x" $off)
	case $off in
		0x0)
			printf "\tRASDP_EXT_CAP_HDR_OFF: 0x%08x\n" $val
			printf "\t                        NEXT_OFFSET[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                        CAP_VERSION[19:16] = 0x%x\n" $((val >> 16 & 0xf))
			printf "\t                     EXTENDED_CAP_ID[15:0] = 0x%x\n" $((val >> 0 & 0xffff))
			;;
		0x4)
			printf "\tRASDP_VENDOR_SPECIFIC_HDR_OFF: 0x%08x\n" $val
			printf "\t                        VSEC_LENGTH[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                           VSEC_REV[19:16] = 0x%x\n" $((val >> 16 & 0xf))
			printf "\t                             VSEC_ID[15:0] = 0x%x\n" $((val >> 0 & 0xffff))
			;;
		0xc)
			printf "\tRASDP_CORR_COUNTER_CTRL_OFF: 0x%08x\n" $val
			printf "\t             CORR_COUNTER_SELECTION[31:24] = 0x%x\n" $((val >> 24 & 0xff))
			printf "\t      CORR_COUNTER_SELECTION_REGION[23:20] = 0x%x\n" $((val >> 20 & 0xf))
			printf "\t                       CORR_EN_COUNTERS[4] = 0x%x\n" $((val >> 4 & 0x1))
			printf "\t                    CORR_CLEAR_COUNTERS[0] = 0x%x\n" $((val >> 0 & 0x1))
			;;
		0x10)
			printf "\tRASDP_CORR_COUNT_REPORT_OFF: 0x%08x\n" $val
			printf "\t              CORR_COUNTER_SELECTED[31:24] = 0x%x\n" $((val >> 24 & 0xff))
			printf "\t       CORR_COUNTER_SELECTED_REGION[23:20] = 0x%x\n" $((val >> 20 & 0xf))
			printf "\t                         CORR_COUNTER[7:0] = 0x%x\n" $((val >> 0 & 0xff))
			;;
		0x14)
			printf "\tRASDP_UNCORR_COUNTER_CTRL_OFF: 0x%08x\n" $val
			printf "\t           UNCORR_COUNTER_SELECTION[31:24] = 0x%x\n" $((val >> 24 & 0xff))
			printf "\t    UNCORR_COUNTER_SELECTION_REGION[23:20] = 0x%x\n" $((val >> 20 & 0xf))
			printf "\t                     UNCORR_EN_COUNTERS[4] = 0x%x\n" $((val >> 4 & 0x1))
			printf "\t                  UNCORR_CLEAR_COUNTERS[0] = 0x%x\n" $((val >> 0 & 0x1))
			;;
	esac
done
echo -e "\n"



#Capabilities: [2e4 v1] Vendor Specific Information: ID=0006 Rev=0 Len=018 <?> —— 3.33 DWC_PCIE_DSP/PF0_VSECDMA_CAP Registers
echo "===> DWC_PCIE_DSP/PF0_VSECDMA_CAP(0x2e4)"
VSECDMA_CAP_ADDR=0x2e4
for off in `seq 0 20`
do
	[ $((off % 4)) -ne 0 ] && continue

	val=$(devmem $((CFG_BASE + VSECDMA_CAP_ADDR + off)))
	printf "Base: 0x%03x, Offset: 0x%02x, value=0x%08x\n" $((VSECDMA_CAP_ADDR + off)) $off $val

	off=$(printf "0x%x" $off)
	case $off in
		0x0)
			printf "\tVSECDMA_EXT_CAP_HDR_OFF: 0x%08x\n" $val
			printf "\t                        NEXT_OFFSET[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                        CAP_VERSION[19:16] = 0x%x\n" $((val >> 16 & 0xf))
			printf "\t                     EXTENDED_CAP_ID[15:0] = 0x%x\n" $((val >> 0 & 0xffff))
			;;
		0x4)
			printf "\tVSECDMA_VENDOR_SPECIFIC_HDR_OFF: 0x%08x\n" $val
			printf "\t                        VSEC_LENGTH[31:20] = 0x%x\n" $((val >> 20 & 0xfff))
			printf "\t                           VSEC_REV[19:16] = 0x%x\n" $((val >> 16 & 0xf))
			printf "\t                             VSEC_ID[15:0] = 0x%x\n" $((val >> 0 & 0xffff))
			;;
		0x8)
			printf "\tVSECDMA_DEVICE_INFORMATION_OFF: 0x%08x\n" $val
			printf "\t MASTER_PAGE_BOUNDARY_POINTER_WIDTH[29:26] = 0x%x\n" $((val >> 26 & 0xf))
			printf "\t                MASTER_BURST_LENGTH[25:23] = 0x%x\n" $((val >> 23 & 0x7))
			printf "\t                   MASTER_BUS_WIDTH[22:20] = 0x%x\n" $((val >> 20 & 0x7))
			printf "\t                                   AXI[19] = 0x%x\n" $((val >> 19 & 0x1))
			printf "\t                 CHANNEL_SEPARATION[18:16] = 0x%x\n" $((val >> 16 & 0x7))
			printf "\t                                PFN[15:11] = 0x%x\n" $((val >> 11 & 0x1f))
			printf "\t                                BARN[10:8] = 0x%x\n" $((val >> 8 & 0x7))
			printf "\t                           MAP_FORMAT[2:0] = 0x%x\n" $((val >> 0 & 0x7))
			;;
		0xc)
			printf "\tVSECDMA_NUM_CHAN_OFF: 0x%08x\n" $val
			printf "\t                    NUM_DMA_RD_CHAN[25:16] = 0x%x\n" $((val >> 16 & 0x7ff))
			printf "\t                      NUM_DMA_WR_CHAN[9:0] = 0x%x\n" $((val >> 0 & 0x7ff))
			;;
		0x10)
			printf "\tVSECDMA_UNROLL_ADDR_OFFSET_LOW_OFF: 0x%08x\n" $val
			printf "\t              UNROLL_ADDR_OFFSET_LOW[31:0] = 0x%x\n" $((val >> 0))
			;;
		0x14)
			printf "\tVSECDMA_UNROLL_ADDR_OFFSET_HIGH_OFF: 0x%08x\n" $val
			printf "\t             UNROLL_ADDR_OFFSET_HIGH[31:0] = 0x%x\n" $((val >> 0))
	esac
done
