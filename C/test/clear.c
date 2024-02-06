/* Copyright (C) 2024 wqshao All rights reserved.
 *
 *  File Name    : clear.c
 *  Author       : wqshao
 *  Created Time : 2024-02-01 10:53:02
 *  Description  :
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if 0
char * szCommAry[COMM_NUM]={
	//屏幕属性命令,23
	"\x1b[12h",//禁止本端回显，键盘数据仅送给主机
	"\x1b[12l",//允许本端回显，键盘数据送给主机和屏幕
	"\x1b[?5h",//屏幕显示为白底黑字
	"\x1b[?5l",//显示为黑底白字
	"\x1b[?3h",//132列显示
	"\x1b[?3l",//80列显示
	"\x1b[?6h",//以用户指定的滚动区域的首行行首为参考原点
	"\x1b[?6l",//以屏幕的首行行首为参考原点
	"\x1b[?7h",//当字符显示到行末时，自动回到下行行首接着显示；如果在滚动区域底行行末，则上滚一行再显示
	"\x1b[?7l",//当字符显示到行末时，仍在行末光标位置显示，覆盖原有的字符，除非接收到移动光标的命令
	"\x1b[?4h",//平滑滚动
	"\x1b[?4l",//跳跃滚动
	"\x1b[/0s",//不滚动
	"\x1b[/1s",//平滑慢滚
	"\x1b[/2s",//跳跃滚动
	"\x1b[/3s",//平滑快滚
	"\x1b[3h",//监督有效，显示控制符，供程序员调试程序用
	"\x1b[3l",//监督无效，执行控制符，正常运行程序
	"\x1b[0$~",//禁止状态行（VT300有效
	"\x1b[1$~",//允许状态行（VT300有效）
	"\x1b[2$~",//主机可写状态行（VT300有效）
	"\x1b[0$|",//主机可写状态行时，在主屏显示数据（VT300有效）
	"\x1b[1$|",//主机可写状态行时，在状态行显示数据（VT300有效）

	//光标命令,14
	"\x1b[?25h",//光标显示
	"\x1b[?25l",//光标消隐
	"\x1b[/0j",//闪烁块光标
	"\x1b[/1j",//闪烁线光标
	"\x1b[/2j",//稳态块光标
	"\x1b[/3j",//稳态线光标
	"\x1bH",//在当前列上设置制表位
	"\x1b[g",//清除当前列上的制表位
	"\x1b[0g",//清除当前列上的制表位
	"\x1b[3g",//清除所有列上的制表位
	"\x1b\x45",//光标下移1行
	"\x1b\x4d",//光标上移1行
	"\x1b\x37",//保存终端当前状态
	"\x1b\x38",//恢复上述状态

	//行属性和字符属性命令,4
	"\x1b#3",//设置当前行为倍宽倍高（上半部分）
	"\x1b#4",//设置当前行为倍宽倍高（下半部分）
	"\x1b#5",//设置当前行为单宽单高
	"\x1b#6",//设置当前行为倍宽单高

	//编缉命令,22
	"\x1b[A",
	"\x1b[B",
	"\x1b[C",
	"\x1b[D",
	"\x1b[4h",//插入方式：新显示字符使光标位置后的原来显示字符右移，移出边界的字符丢失。
	"\x1b[4l",//替代方式：新显示字符替代光标位置字符显示
	"\x1b[K",//清除光标至行末字符，包括光标位置，行属性不受影响。
	"\x1b[0K",//清除光标至行末字符，包括光标位置，行属性不受影响。
	"\x1b[1K",//清除行首至光标位置字符，包括光标位置，行属性不受影响。
	"\x1b[2K",//清除光标所在行的所有字符
	"\x1b[J",//清除从光标至屏末字符，整行被清的行属性变成单宽单高
	"\x1b[0J",//清除从光标至屏末字符，整行被清的行属性变成单宽单高
	"\x1b[1J",//清除从屏首至光标字符，整行被清的行属性变成单宽单高
	"\x1b[2J",//清除整个屏幕，行属性变成单宽单高，光标位置不变
	"\x1b[?K",//清除光标至行末的"可清除"字符，不影响其它字符和行属性
	"\x1b[?0K",//清除光标至行末的"可清除"字符，不影响其它字符和行属性
	"\x1b[?1K",//清除行首至光标位置的"可清除"字符，不影响其它字符和行属性
	"\x1b[?2K",//清除光标所在行的所有"可清除"字符，不影响其它字符和行属性
	"\x1b[?J",//清除从光标至屏末的"可清除"字符，不影响其它字符和行属性
	"\x1b[?0J",//清除从光标至屏末的"可清除"字符，不影响其它字符和行属性
	"\x1b[?1J",//清除从屏首至光标的"可清除"字符，不影响其它字符和行属性
	"\x1b[?2J",//清除整个屏幕中的"可清除"字符，不影响其它字符和行属性

	//键盘16
	"\x1b[2h",//锁存键盘数据（不超过15个）暂停向主机发送，直到开放为止。
	"\x1b[2l",//允许键盘向主机发送数据。
	"\x1b[?8h",//键盘连发有效
	"\x1b[?8l",//键盘连发无效
	"\x1b[5h",//击键声有效
	"\x1b[5l",//击键声无效
	"\x1b[?1h",//光标键产生"应用"控制序列。见键盘代码一节。
	"\x1b[?1l",//光标键产生ANSI标准的控制序列。见键盘代码一节。
	"\x1b=",//副键盘产生"应用"控制序列。见键盘代码一节。
	"\x1b>",//副键盘产生数字等字符序列，PF键不变。见键盘代码一节。
	"\x1b[20h",//接收LF、FF或VT控制码后，光标移至下一行行首；Return键发送CR和LF控制码。
	"\x1b[20l",//接收LF、FF或VT控制码后，光标移至下一行当前列；Return键发送CR控制码。
	"\x1b[?67h",//作为退格键发送BS。
	"\x1b[?67l",//作为删除键发送DEL。
	"\x1b[/2h", // 顶排功能键作为应用程序功能使用CTRL功能键作为本端功能键使用
	"\x1b[/2l",//顶排功能键作为本端功能键使用CTRL功能键作为应用程序功能使用

	//打印参数配置3
	// ""
	//  "\x1b\x5b\x35\x69",  //1、5、9 型终端打开并口，关闭用\x1b\x5b\x34\x69
	"\x9b\x35\x69", //2、10 型终端打开并口，关闭用\x9b\x34\x69
	"\x1b[5i",  //3、4 型终端打开并口，关闭用\x1b[4i

	//状态报告8
	"\x1b[c",//主设备属性,询问 CSI c或CSI 0 c ,//应答 ESC [ ? 1;2c
	"\x1b[0c",
	"\x1b[>c", //辅助设备属性  ,询问 CSI > c或 CSI > 0 c    你配置如何？//应答  CSI > Pp;Pv;Po c
	"\x1b[>0c",
	"\x1b[5n",//设备状态    ,//询问   CSI 5 n 设备状态正常否？//应答    CSI 0 n 设备状态正常,CSI 3 n  设备不正常
	"\x1b[6n",//光标位置,//询问   CSI 6 n 现在光标在何处？//应答    CSI Pl；Pc R 光标在Pl行Pc列
	"\x1b[?15n",//打印机状态,//询问    CSI ? 15 n  打印机状态如何？//应答    CSI ? 13 n  无打印机(没开),//CSI ? 10 n   打印机准备好,CSI ? 11 n   打印机未准备好(缺纸、脱机等)
	"\x1b[?25n",//自定义键状态    ,//询问   CSI ? 25 n  自定义功能键状态如何？//应答 CSI ? 20 n  自定义键允许,CSI ? 21 n   自定义键锁定
	"\x1b[H",//光标移动
	"\x1b[m",//字符属性
	"\x1b(B",//指定G0字符集为ASCII
	"\x1b(0",//指定G0字符集为DEC特殊图形字符集
	"\x1b)B",//指定G1字符集为ASCII
	"\x1b)0",//指定G1字符集为DEC特殊图形字符集

	//辅口操作   10
	//1 型终端
	"\x1b[/50h\x1b[/53l\x1b[/54l\x1b!;;;Y", //打开终端辅口1
	"\x1b[/50h\x1b[/53l\x1b[/54l\x1b!3;0;0;1Z",  //打开终端辅口2
	"\x1b[/50h\x1b[/53l\x1b[/54l\x1b!;;;X",//打开终端辅口3
	"\x1b[/53h\x1b[/50l",//关闭终端辅口

	"\x1b[/|",//开始发送辅口数据
	"\x1b[/51h",//开始发送辅口数据
	"\x1b[/M",//关辅口

	"\x1b\x25K",//开辅口，发送标志到辅口，发送该串
	"\x1b\x25\x42",//开辅口，发送标志到辅口，发送该串
	"\x1b\x25\x43"//开辅口，发送标志到辅口，发送该串

};

char * szCommAry1[COMM_NUM1]={
	//屏幕属性  1
	"\x1b[\xff\x72",//开窗    CSI Pt;Pb r 设置滚动区域，参数Pt:顶行行号，Pb:末行行号

	//光标      6
	"\x1b[\xff\x48",//光标定位  CSI Pl;Pc H或CSI Pl;Pc f光标移动到Pl行Pc列
	"\x1b[\xff\x66",//光标定位
	"\x1b[\xff\x41",//光标上移Pn行
	"\x1b[\xff\x42",//光标下移Pn行
	"\x1b[\xff\x43",//光标右移Pn行
	"\x1b[\xff\x44",//光标左移Pn行

	//编辑      5
	"\x1b[\xff\x4c",//插入行
	"\x1b[\xff\x4d",//删除行
	"\x1b[\xff\x50",//字符删除
	"\x1b[\xff\x40",//字符插入
	"\x1b[\xff\x58",//字符清除

	// 打印机   2
	"\x1b[\x2f\xff\x74",//打印机型号
	"\x1b[\x2f\xff\x78",//图像放大系数

	"\x1b[/\xffM",//打开辅口
	"\x1b[/\xff|",
	//字符属性函数    1
	"\x1b[\xff\x6d"//CSI Ps;Ps;...m 设置字符显示属性，Ps代表某种字符属性，见表3.1.3.3-2

};

int (*CmdFun[])(void)={

	//屏幕显示23
	SetLocDisplayforbid,        //禁止本端回显，键盘数据仅送给主机
	SetLocDisplayallow,         //允许本端回显，键盘数据送给主机和屏幕
	SetScreenModeBlackWord,     //屏幕显示为白底黑字
	SetScreenModeWhiteWord,     //显示为黑底白字
	SetScreenWidth132,          //132列显示
	SetScreenWidth80,           //80列显示
	SetOriginModeUsr,           //以用户指定的滚动区域的首行行首为参考原点
	SetOriginModeHome,          //以屏幕的首行行首为参考原点
	SetRowBackNext,             //当字符显示到行末时，自动回到下行行首接着显示；如果在滚动区域底行行末，则上滚一行再显示
	SetRowBackNoMove,           //当字符显示到行末时，仍在行末光标位置显示，覆盖原有的字符，除非接收到移动光标的命令
	SetRollModeSmooth,          //平滑滚动
	SetRollModeLeap,            //跳跃滚动
	SetRowBackExtNoMove,        //不滚动
	SetRowBackExtSmoothSlow,    //平滑慢滚
	SetRowBackExtLeap,          //跳跃滚动
	SetRowBackExtSmoothFast,    //平滑快滚
	SetIntendanceHaveEffect,    //监督有效，显示控制符，供程序员调试程序用
	SetIntendanceNoEffect,      //监督无效，执行控制符，正常运行程序
	SetStaRowForbid,            //禁止状态行（VT300有效
	SetStaRowAllow,             //允许状态行（VT300有效）
	SetStaRowAllowWrite,        //主机可写状态行（VT300有效）
	SetMainScreenDisplay,       //主机可写状态行时，在主屏显示数据（VT300有效）
	SetStaRowDisplay,           //主机可写状态行时，在状态行显示数据（VT300有效）

	//光标14
	SetCursorDisplay,           //光标显示
	SetCursorHide,              //光标消隐
	SetCursorFlashBlock,        //闪烁块光标
	SetCursorFlashLine,         //闪烁线光标
	SetCursorStaticBlock,       //稳态块光标
	SetCursorStaticLine,        //稳态线光标
	SetCurrentTab,              //在当前列上设置制表位
	ClearCurrentTab,            //清除当前列上的制表位
	ClearCurrentTab,            //清除所有列上的制表位
	ClearAllTab,                //清除所有列上的制表位
	CursorMoveDownRow,          //光标下移1行
	CursorMoveUpRow,            //光标上移1行
	SaveState,                  //保存终端当前状态
	RestoreState,               //恢复上述状态

	//行属性和字符属性命令,4
	SetRowPropDbWHTop,              //设置当前行为倍宽倍高（上半部分）"\x1b#3",
	SetRowPropDbWHBottom,           //设置当前行为倍宽倍高（下半部分）"\x1b#4",
	SetRowPropSingleWH,             //设置当前行为单宽单高"\x1b#5",
	SetRowPropDoubleSingleWH,       //设置当前行为倍宽单高"\x1b#6",

	//编缉命令22

	CursorMoveUpRow,            //"\x1b[A",向上光标键==光标上移1行
	CursorMoveDownRow,          //"\x1b[B",向下光标键==光标下移1行
	CursorMoveRightCol,         //"\x1b[C",向右光标键==光标右移1行
	CursorMoveLeftCol,          //"\x1b[D",向左光标键==光标左移1行
	SetInsertMode,//插入方式：新显示字符使光标位置后的原来显示字符右移，移出边界的字符丢失。"\x1b[4h",
	SetReplaceMode,//替代方式：新显示字符替代光标位置字符显示"\x1b[4l",
	ClearRowK,//清除光标至行末字符，包括光标位置，行属性不受影响。"\x1b[K",
	ClearRowK,//同上"\x1b[0K",
	ClearRow1K,//清除行首至光标位置字符，包括光标位置，行属性不受影响。"\x1b[1K",
	ClearRow2K,//清除光标所在行的所有字符"\x1b[2K",
	ClearScreenJ,//清除从光标至屏末字符，整行被清的行属性变成单宽单高"\x1b[J",
	ClearScreenJ,//清除从光标至屏末字符，整行被清的行属性变成单宽单高"\x1b[0J",
	ClearScreen1J,//清除从屏首至光标字符，整行被清的行属性变成单宽单高"\x1b[1J",
	ClearScreen2J,//清除整个屏幕，行属性变成单宽单高，光标位置不变 "\x1b[2J",
	ClearRowEK,//清除光标至行末的"可清除"字符，不影响其它字符和行属性"\x1b[?K",
	ClearRowEK,//清除光标至行末的"可清除"字符，不影响其它字符和行属性"\x1b[?0K",
	ClearRowE1K,//清除行首至光标位置的"可清除"字符，不影响其它字符和行属性"\x1b[?1K",
	ClearRowE2K,//清除光标所在行的所有"可清除"字符，不影响其它字符和行属性"\x1b[?2K",
	ClearScreenEJ,//清除从光标至屏末的"可清除"字符，不影响其它字符和行属性"\x1b[?J",
	ClearScreenEJ,//清除从光标至屏末的"可清除"字符，不影响其它字符和行属性"\x1b[?0J",
	ClearScreenE1J,//清除从屏首至光标的"可清除"字符，不影响其它字符和行属性"\x1b[?1J",
	ClearScreenE2J,//清除整个屏幕中的"可清除"字符，不影响其它字符和行属性"\x1b[?2J",

	//键盘16
	KeyLocked, // CSI 2 h//锁存键盘数据（不超过15个）暂停向主机发送，直到开放为止。
	KeyUnlocked,//CSI 2 l//允许键盘向主机发送数据。
	KeyContEnable,//CSI ? 8 h   键盘连发有效
	KeyContUnEnable,//CSI ? 8 l 键盘连发无效
	KeySound,//CSI / 5 h    击键声有效
	KeyNoSound,//CSI / 5 l  击键声无效
	KeyAppCursor,//CSI ? 1 h    //光标键产生"应用"控制序列。见键盘代码一节。
	KeyNormalCursor,//CSI ? 1 l//光标键产生ANSI标准的控制序列。见键盘代码一节。
	KeyAppBoard,//ESC = //副键盘产生"应用"控制序列。见键盘代码一节。
	KeyNumBoard,//ESC >  //副键盘产生数字等字符序列，PF键不变。见键盘代码一节。
	KeyNewRow,//CSI 20 h//接收LF、FF或VT控制码后，光标移至下一行行首；Return键发送CR和LF控制码。
	KeyNoNewRow,//CSI 20 l  //接收LF、FF或VT控制码后，光标移至下一行当前列；Return键发送CR控制码。
	KeyBSBS,//CSI ? 67 h    作为退格键发送BS。
	KeyBSDEL,//CSI ? 67 l   作为删除键发送DEL。
	KeyCtrlLocal,//CSI / 2 h    顶排功能键作为应用程序功能使用//CTRL功能键作为本端功能键使用
	KeyCtrlApp,//CSI / 2 l  顶排功能键作为本端功能键使用//CTRL功能键作为应用程序功能使用

	//打印机
	//      PrintDritTransOpen1,
	PrintDritTransOpen2,
	PrintDritTransOpen3,

	//状态报告8
	AnswerMainDev,//主设备属性,询问    CSI c或CSI 0 c ,//应答 ESC [ ? 1;2c,
	AnswerMainDev,
	AnswerAssDev,//辅助设备属性   ,询问 CSI > c或 CSI > 0 c    你配置如何？//应答  CSI > Pp;Pv;Po c
	AnswerAssDev,
	AnswerDevState,//设备状态   ,//询问   CSI 5 n 设备状态正常否？//应答    CSI 0 n 设备状态正常,CSI 3 n  设备不正常
	AnswerCursorPosit,//光标位置,//询问   CSI 6 n 现在光标在何处？//应答    CSI Pl；Pc R 光标在Pl行Pc列
	AnswerPrinterState,//打印机状态,//询问 CSI ? 15 n  打印机状态如何？//应答    CSI ? 13 n  无打印机(没开),//CSI ? 10 n   打印机准备好,CSI ? 11 n   打印机未准备好(缺纸、脱机等)
	AnswerSelfKeyState,//自定义键状态 ,//询问   CSI ? 25 n  自定义功能键状态如何？//应答 CSI ? 20 n  自定义键允许,CSI ? 21 n   自定义键锁定

	SetCursorPositionH,
	SetCharProp,
	SeleCharSetG0_B,
	SeleCharSetG0_0,
	SeleCharSetG1_B,
	SeleCharSetG1_0,

	//  辅口
	//1型终端
	OpenAuxPort11,//打开辅口1
	OpenAuxPort2,//打开辅口2
	OpenAuxPort3,//打开辅口3
	CloseAuxPort,

	SendToAux,//开始发送辅口数据
	SendToAux1,//开始发送辅口数据
	CloseAuxPort,//关辅口

	SendToAuxK,//开辅口，发送标志到辅口，发送串"\x1b%K"
	SendToAuxB,//开辅口，发送标志到辅口，发送串"\x1b%B"
	SendToAuxC,//开辅口，发送标志到辅口，发送串"\x1b%C"

	//屏幕属性  1
	SetRollWindow,//开窗  CSI Pt;Pb r 设置滚动区域，参数Pt:顶行行号，Pb:末行行号"\x1b[\xff\x72",

	//光标      6
	SetCursorPositionH,//光标定位   CSI Pl;Pc H光标移动到Pl行Pc列"\x1b[\xff\x48",
	SetCursorPositionF,//光标定位,CSI Pl;Pc f,"\x1b[\xff\x66",
	CursorMoveUp,//光标上移Pn行"\x1b[\xff\x41",
	CursorMoveDown,//光标下移Pn行"\x1b[\xff\x42",
	CursorMoveRight,//光标右移Pn行"\x1b[\xff\x43",
	CursorMoveLeft,//光标左移Pn行"\x1b[\xff\x44",

	//编辑      5
	InsertRow,//插入行"\x1b[\xff\x4c",
	DeleteRow,//删除行"\x1b[\xff\x4d",
	DeleteChar,//字符删除"\x1b[\xff\x50",
	InsertChar,//字符插入"\x1b[\xff\x40",
	ClearChar,//字符清除"\x1b[\xff\x58",

	// 打印机   2
	PrintType,//打印机型号"\x1b[\x2f\xff\x74",
	PrintImageMult,//图像放大系数"\x1b[\x2f\xff\x78",

	OpenAuxPort,
	SendToAux,

	//字符属性函数    1
	SetCharProp//CSI Ps;Ps;...m 设置字符显示属性，Ps代表某种字符属性，见表3.1.3.3-2"\x1b[\xff\x6d",

};
#endif

int main(int argc, const char *argv[])
{
	printf("===> func: %s, line: %d\n", __func__, __LINE__);

	// 清屏
	/* This prints the clear screen and move cursor to top-left corner control
	 * characters for VT100 terminals. This means it will not work on
	 * non-VT100 compliant terminals, namely Windows' cmd.exe, but should
	 * work on anything unix-y. */
	fputs("\x1b[2J\x1b[H", stdout);

	printf("===> func: %s, line: %d\n", __func__, __LINE__);
	return 0;
}
