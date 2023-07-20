
// MPFinalGUI.h
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"

#define PORT_NUM_1            0
#define PORT_NUM_2            1
#define PORT_NUM_3            2
#define PORT_NUM_4            3
#define PORT_NUM_5            4
#define PORT_NUM_6            5
#define PORT_NUM_7            6
#define PORT_NUM_8            7
#define PORT_NUM_9            8
#define PORT_NUM_10           9
#define PORT_NUM_11           10
#define PORT_NUM_12           11
#define PORT_NUM_13           12
#define PORT_NUM_14           13
#define PORT_NUM_15           14
#define PORT_NUM_16           15
#define PORT_NUM_17           16
#define PORT_NUM_18           17
#define PORT_NUM_19           18
#define PORT_NUM_20           19

#define BAUD_RATE_110         0
#define BAUD_RATE_300         1
#define BAUD_RATE_600         2
#define BAUD_RATE_1200        3
#define BAUD_RATE_2400        4
#define BAUD_RATE_4800        5
#define BAUD_RATE_9600        6
#define BAUD_RATE_14400       7
#define BAUD_RATE_19200       8
#define BAUD_RATE_38400       9
#define BAUD_RATE_56000       10
#define BAUD_RATE_57600       11
#define BAUD_RATE_115200      12
#define BAUD_RATE_128000      13
#define BAUD_RATE_256000      14

#define DATA_BIT_7            0
#define DATA_BIT_8            1

#define PARITY_NONE           0
#define PARITY_ODD            1
#define PARITY_EVEN           2

#define STOP_BIT_1            0
#define STOP_BIT_2            1

#define DEVICE_ADDRESS_MASK          0xFC
#define MESSAGE_FORMAT_MASK          0x03
#define MESSAGE_TYPE_MASK            0xC0
#define GROUP_NUMBER_MASK            0x38
#define COMMAND_MASK                 0x07
#define DATA_LENGTH_MASK             0xF0
#define DATA_FIELD_FIRST_MASK        0x0F
#define FULL_BIT_MASK                0xFF

#define STX_INDEX                    0
#define ADDR_FORMAT_INDEX            1

#define BIN_TYPE_GN_COM_INDEX        2
#define BIN_LEN_DATAH_INDEX          3
#define BIN_DATAL_INDEX              4
#define BIN_ETX_INDEX                5

#define ASC_TYPE_INDEX               2
#define ASC_GROUP_NUM_INDEX          3
#define ASC_COMMAND_INDEX            4
#define ASC_DATA_LEN_INDEX           5
#define ASC_DATA0_INDEX              6
#define ASC_DATA1_INDEX              7
#define ASC_DATA2_INDEX              8
#define ASC_DATA3_INDEX              9
#define ASC_ETX_INDEX                10

#define BIT1_MASK                    0x01
#define BIT2_MASK                    0x03
#define BIT3_MASK                    0x07
#define BIT4_MASK                    0x0F
#define BIT6_MASK                    0x3F
#define BIT8_MASK                    0xFF

#define BINARY_TOTAL_LENGTH          6
#define ASCII_TOTAL_LENGTH           11
#define STUDENT_ID_LENGTH            10

#define MSG_FORMAT_BIN               1
#define MSG_FORMAT_ASC               2

#define MSG_TYPE_CONTROL             1
#define MSG_TYPE_MONITOR             2
#define MSG_TYPE_SECURITY            3

#define ASC_INDEX_OFFSET             48

#define GROUP_NUM_LED                0
#define GROUP_NUM_FND                1
#define GROUP_NUM_TACT               2
#define GROUP_NUM_DC_MOTOR           3
#define GROUP_NUM_SERVO_MOTOR        4
#define GROUP_NUM_ADC                5
#define GROUP_NUM_DAC                6
#define GROUP_NUM_TIMER              7

#define GROUP_NUM_Write_Protection   0
#define GROUP_NUM_Privilege          1

#define COMMAND_000                  0
#define COMMAND_001                  1
#define COMMAND_002                  2
#define COMMAND_003                  3
#define COMMAND_004                  4
#define COMMAND_005                  5
#define COMMAND_006                  6
#define COMMAND_007                  7

#define DATA_ADC_MASK                0x7F

#define DATA_LENGTH_0                0
#define DATA_LENGTH_1                1
#define DATA_LENGTH_2                2
#define DATA_LENGTH_3                3
#define DATA_LENGTH_4                4
#define DATA_LENGTH_5                5
#define DATA_LENGTH_6                6
#define DATA_LENGTH_7                7
#define DATA_LENGTH_8                8
#define DATA_LENGTH_9                9
#define DATA_LENGTH_10               10
#define DATA_LENGTH_11               11
#define DATA_LENGTH_12               12

#define DATA_DC_CW                   0
#define DATA_DC_CCW                  15
#define DATA_SERVO_PLUS_10           10
#define DATA_SERVO_MINUS_10          10


// CMFCApplication1App:
//

class CMFCApplication1App : public CWinApp
{
public:
	CMFCApplication1App();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplication1App theApp;
