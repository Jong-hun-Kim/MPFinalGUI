
// MPFinalGUI.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "comm.h"
#include "ctype.h"

volatile unsigned int switch_count = 0x00;
volatile unsigned int student_id = 0x00;
volatile unsigned int security_key1 = 0x000;
volatile unsigned int security_key2 = 0xE9D;
volatile unsigned int admin_mode = 0x555;
volatile unsigned int root_mode = 0xAAA;

#define WM_RECEIVEDATA      WM_USER + 100

#define START               0x02
#define END                 0x03

static char BIN_Protocol[6] = { START, // Start of Text (0x02)
							0x00, // Device Address 6-bit + Message Format 2-bit
							0x00, // Message Type 2-bit + Group Number 3-bit + Command 3-bit
							0x00, // Data Length 4-bit + Data Field Higher 4-bit
							0x00, // Data Field Lower 8-bit
							END }; // End of Text (0x03)

static char ASC_Protocol[11] = { START, // Start of Text (0x02)
							0x00, // Device Address 6-bit + Message Format 2-bit
							0x00, // Message Type 1-byte
							0x00, // Group Number 1-byte
							0x00, // Command 1-byte
							0x00, // Data Length 1-byte
							0x00, // Data Field[0] 1-byte
							0x00, // Data Field[1] 1-byte
							0x00, // Data Field[2] 1-byte
							0x00, // Data Field[3] 1-byte
							END }; // End of Text (0x03)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg 대화 상자
CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// UART Communication Connection Parameters
	DDX_Control(pDX, IDC_TX_DATA, m_Tx_Data);
	DDX_Control(pDX, IDC_RX_DATA, m_Rx_Data);
	DDX_Control(pDX, IDC_PORT, m_PortN);
	DDX_Control(pDX, IDC_BAUD, m_Baud);
	DDX_Control(pDX, IDC_DATABIT, m_DataBit);
	DDX_Control(pDX, IDC_PARITYBIT, m_ParityBit);
	DDX_Control(pDX, IDC_STOPBIT, m_StopBit);

	// Student ID Setting Parameters
	DDX_Control(pDX, IDC_STU_NUM, m_STU_NUM);

	// FND Parameters
	DDX_Control(pDX, IDC_DEC, m_dec);
	DDX_Control(pDX, IDC_HEX, m_hex);
	DDX_Control(pDX, IDC_BIN, m_bin);

	// ADC Parameters
	DDX_Control(pDX, IDC_CUR_MODE, m_ADC_Mode);
	DDX_Control(pDX, IDC_VR_VALUE, m_VR_Value);
	DDX_Control(pDX, IDC_TEMP, m_Temp);
	DDX_Control(pDX, IDC_CS_STATE, m_CS_State);
	DDX_Control(pDX, IDC_ADC_PERIOD, m_ADC_Period);

	// Tact Switch Parameters
	DDX_Control(pDX, IDC_COUNT, m_SW_Count);

	// Timer Period Parameters
	DDX_Control(pDX, IDC_PERIOD_1, m_Period1);

	// DC Motor Parameters
	DDX_Control(pDX, IDC_DUTY_RATE, m_DC_DUTY);
}


BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	// Initial Icon & Thread Handler Setting
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_RECEIVEDATA, (LRESULT(AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))OnReceiveData)

	// UART Communication Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_CONNECT, &CMFCApplication1Dlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SEND, &CMFCApplication1Dlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_DELETE, &CMFCApplication1Dlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_BT_DEC, &CMFCApplication1Dlg::OnBnClickedBtDec)
	ON_BN_CLICKED(IDC_BT_HEX, &CMFCApplication1Dlg::OnBnClickedBtHex)
	ON_BN_CLICKED(IDC_BT_BIN, &CMFCApplication1Dlg::OnBnClickedBtBin)

	// Initial Student ID Setting Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_SETTING, &CMFCApplication1Dlg::OnBnClickedSetting)

	// Security Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_KEY1, &CMFCApplication1Dlg::OnBnClickedKey1)
	ON_BN_CLICKED(IDC_KEY2, &CMFCApplication1Dlg::OnBnClickedKey2)
	ON_BN_CLICKED(IDC_ADMIN, &CMFCApplication1Dlg::OnBnClickedAdmin)
	ON_BN_CLICKED(IDC_ROOT, &CMFCApplication1Dlg::OnBnClickedRoot)

	// Timer Period Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_BT_PERIOD1, &CMFCApplication1Dlg::OnBnClickedBtPeriod1)
	ON_BN_CLICKED(IDC_BT_ADC_PERIOD, &CMFCApplication1Dlg::OnBnClickedBtAdcSend)

	// LED Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)

	// DC Motor Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_MOTOR_ON, &CMFCApplication1Dlg::OnBnClickedMotorOn)
	ON_BN_CLICKED(IDC_MOTOR_OFF, &CMFCApplication1Dlg::OnBnClickedMotorOff)
	ON_BN_CLICKED(IDC_MOTOR_CW, &CMFCApplication1Dlg::OnBnClickedMotorCw)
	ON_BN_CLICKED(IDC_MOTOR_CCW, &CMFCApplication1Dlg::OnBnClickedMotorCcw)
	ON_BN_CLICKED(IDC_BT_DUTY_RATE, &CMFCApplication1Dlg::OnBnClickedBtDutyRate)

	// Servo Motor Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_LOCATION_0, &CMFCApplication1Dlg::OnBnClickedLocation0)
	ON_BN_CLICKED(IDC_LOCATION_90, &CMFCApplication1Dlg::OnBnClickedLocation90)
	ON_BN_CLICKED(IDC_LOCATION_180, &CMFCApplication1Dlg::OnBnClickedLocation180)
	ON_BN_CLICKED(IDC_PLUS_10, &CMFCApplication1Dlg::OnBnClickedPlus10)
	ON_BN_CLICKED(IDC_MINUS_10, &CMFCApplication1Dlg::OnBnClickedMinus10)

	// DAC Control Handler Mapping (BUTTON PUSH EVENT)
	ON_BN_CLICKED(IDC_SOUND1, &CMFCApplication1Dlg::OnBnClickedSound1)
	ON_BN_CLICKED(IDC_SOUND2, &CMFCApplication1Dlg::OnBnClickedSound2)

END_MESSAGE_MAP()


BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Large Icon Setting
	SetIcon(m_hIcon, FALSE);		// Small Icon Setting

	m_Comm.SetHwnd(this->m_hWnd);
	bCheck = false; // Communication State Checking Variable

	// SetCurSel: Initial Value Select (Communication Setting Initialization)
	m_PortN.SetCurSel(PORT_NUM_4);
	m_Baud.SetCurSel(BAUD_RATE_115200);
	m_DataBit.SetCurSel(DATA_BIT_8);
	m_ParityBit.SetCurSel(PARITY_NONE);
	m_StopBit.SetCurSel(STOP_BIT_1);

	/********* Item(Control) Using Option Select *********/
	// UART Communication Area(Window) Disable
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TX_DATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_RX_DATA)->EnableWindow(FALSE);

	// Student Personal Information Area(Window) Disable
	GetDlgItem(IDC_STU_NUM)->EnableWindow(FALSE);
	GetDlgItem(IDC_SETTING)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_STUID)->EnableWindow(FALSE);

	// LED Area(Window) Disable
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);

	// FND Area(Window) Disable
	GetDlgItem(IDC_DEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_HEX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_DEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_HEX)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_BIN)->EnableWindow(FALSE);

	// ADC Area(Window) Disable
	GetDlgItem(IDC_CUR_MODE)->EnableWindow(FALSE);
	GetDlgItem(IDC_VR_VALUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEMP)->EnableWindow(FALSE);
	GetDlgItem(IDC_CS_STATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADC_PERIOD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_ADC_PERIOD)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_STATIC_ADC1)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC3)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC4)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC5)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC6)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC7)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC9)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ADC10)->EnableWindow(FALSE);

	// Tact Switch Area(Window) Disable
	GetDlgItem(IDC_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_SW)->EnableWindow(FALSE);

	// Timer Period Area(Window) Disable
	GetDlgItem(IDC_STATIC_TIMER1)->EnableWindow(FALSE);
	GetDlgItem(IDC_PERIOD_1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_PERIOD1)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEXT_TIMER1)->EnableWindow(FALSE);

	// DC Motor Area(Window) Disable
	GetDlgItem(IDC_MOTOR_ON)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOTOR_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDC_DUTY_RATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_DUTY_RATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEXT_MOTOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOTOR_CW)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOTOR_CCW)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MOTOR1)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MOTOR2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MOTOR3)->EnableWindow(FALSE);

	// Servo Motor Area(Window) Disable
	GetDlgItem(IDC_PLUS_10)->EnableWindow(FALSE);
	GetDlgItem(IDC_MINUS_10)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOCATION_0)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOCATION_90)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOCATION_180)->EnableWindow(FALSE);

	// DAC Area(Window) Disable
	GetDlgItem(IDC_SOUND1)->EnableWindow(FALSE);
	GetDlgItem(IDC_SOUND2)->EnableWindow(FALSE);

	// Security Area(Window) Disable
	GetDlgItem(IDC_KEY1)->EnableWindow(FALSE);
	GetDlgItem(IDC_KEY2)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADMIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_ROOT)->EnableWindow(FALSE);

	return TRUE;
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.


void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Device Context for Drawing Icon

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Icon Center Position Setting in Client Rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Drawing Icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplication1Dlg::OnBnClickedConnect()
{
	if (!bCheck) // UART Communication Start!
	{
		if (m_Comm.OpenCommPort(&Int2TTY()) != TRUE)
		{
			CString fail_message;
			fail_message.Format(_T("COM%dPort Connect fail"), m_PortN.GetCurSel() + 1);
			AfxMessageBox(fail_message);
			((CButton*)GetDlgItem(IDC_CONNECT))->SetCheck(!bCheck);
			return;
		}

		/********* Item(Control) Using Option Select *********/
		// UART Communication Area(Window) Disable
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_TX_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_RX_DATA)->EnableWindow(FALSE);

		GetDlgItem(IDC_CONNECT)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BAUD)->EnableWindow(FALSE);
		GetDlgItem(IDC_DATABIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PARITYBIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOPBIT)->EnableWindow(FALSE);

		// Student Personal Information Area(Window) Disable
		GetDlgItem(IDC_STU_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_SETTING)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_STUID)->EnableWindow(TRUE);

		bCheck = true;
	}
	else // UART Communication Stop!
	{
		m_Comm.CloseConnection();
		/********* Item(Control) Using Option Select *********/
		// UART Communication Area(Window) Disable
		GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_TX_DATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_RX_DATA)->EnableWindow(FALSE);

		GetDlgItem(IDC_CONNECT)->SetWindowText(_T("Connect"));
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BAUD)->EnableWindow(TRUE);
		GetDlgItem(IDC_DATABIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PARITYBIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOPBIT)->EnableWindow(TRUE);

		// Student Personal Information Area(Window) Disable
		GetDlgItem(IDC_STU_NUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_SETTING)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_STUID)->EnableWindow(FALSE);

		// LED Area(Window) Disable
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);

		// FND Area(Window) Disable
		GetDlgItem(IDC_DEC)->EnableWindow(FALSE);
		GetDlgItem(IDC_HEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_DEC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_HEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_BIN)->EnableWindow(FALSE);

		// ADC Area(Window) Disable
		GetDlgItem(IDC_CUR_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_VR_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_CS_STATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADC_PERIOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_ADC_PERIOD)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_ADC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC4)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC6)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC7)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC9)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ADC10)->EnableWindow(FALSE);

		// Tact Switch Area(Window) Disable
		GetDlgItem(IDC_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_SW)->EnableWindow(FALSE);

		// Timer Period Area(Window) Disable
		GetDlgItem(IDC_STATIC_TIMER1)->EnableWindow(FALSE);
		GetDlgItem(IDC_PERIOD_1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_PERIOD1)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEXT_TIMER1)->EnableWindow(FALSE);

		// DC Motor Area(Window) Disable
		GetDlgItem(IDC_MOTOR_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_MOTOR_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_DUTY_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_DUTY_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEXT_MOTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_MOTOR_CW)->EnableWindow(FALSE);
		GetDlgItem(IDC_MOTOR_CCW)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MOTOR1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MOTOR2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MOTOR3)->EnableWindow(FALSE);

		// Servo Motor Area(Window) Disable
		GetDlgItem(IDC_PLUS_10)->EnableWindow(FALSE);
		GetDlgItem(IDC_MINUS_10)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOCATION_0)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOCATION_90)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOCATION_180)->EnableWindow(FALSE);

		// DAC Area(Window) Disable
		GetDlgItem(IDC_SOUND1)->EnableWindow(FALSE);
		GetDlgItem(IDC_SOUND2)->EnableWindow(FALSE);

		// Security Area(Window) Disable
		GetDlgItem(IDC_KEY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_KEY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADMIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ROOT)->EnableWindow(FALSE);

		// Output(Print) Data Reset
		m_Tx_Data.SetWindowTextA("");
		m_Rx_Data.SetWindowTextA("");
		m_dec.SetWindowTextA("");
		m_hex.SetWindowTextA("");
		m_bin.SetWindowTextA("");

		m_SW_Count.SetWindowTextA("");
		m_Period1.SetWindowTextA("");
		m_ADC_Period.SetWindowTextA("");

		m_ADC_Mode.SetWindowTextA("");
		m_VR_Value.SetWindowTextA("");
		m_Temp.SetWindowTextA("");
		m_CS_State.SetWindowTextA("");

		bCheck = false;
	}
}


TTYSTRUCT CMFCApplication1Dlg::Int2TTY()
{
	TTYSTRUCT tty;
	ZERO_MEMORY(tty);

	tty.byXonXoff = FALSE;
	tty.byCommPort = (BYTE)(m_PortN.GetCurSel() + 1);
	tty.dwBaudRate = (DWORD)_nBaudRates[m_Baud.GetCurSel()];
	tty.byByteSize = (BYTE)_nDataValues[m_DataBit.GetCurSel()];
	tty.byParity = (BYTE)_nParity[m_ParityBit.GetCurSel()];
	tty.byStopBits = (BYTE)_nStopBits[m_StopBit.GetCurSel()];
	tty.byFlowCtrl = (BYTE)_nFlow[0];
	return tty;
}


/****** Student ID Input Setting ******/
void CMFCApplication1Dlg::OnBnClickedSetting()
{
	CString SendStr;
	char temp[100] = { NULL };
	char id[2] = { 0 }, key[3] = { 0 };
	
	m_STU_NUM.GetWindowTextA(SendStr);
	memcpy(temp, (LPSTR)(LPCSTR)SendStr, SendStr.GetLength());

	if (SendStr.GetLength() != STUDENT_ID_LENGTH) {
		m_STU_NUM.SetWindowTextA("");
		MessageBox(_T("잘못된 학번입니다."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		memcpy(&id, &temp[2], 2);
		memcpy(&key, &temp[7], 3);
		student_id = atoi(id);
		security_key1 = atoi(key);

		// UART Message Device Address Setting (6-bit)
		BIN_Protocol[ADDR_FORMAT_INDEX] = (student_id << 2) & DEVICE_ADDRESS_MASK;
		ASC_Protocol[ADDR_FORMAT_INDEX] = (student_id << 2) & DEVICE_ADDRESS_MASK;

		// UART Message Format Setting (Binary or ASCII)
		BIN_Protocol[ADDR_FORMAT_INDEX] |= MSG_FORMAT_BIN & MESSAGE_FORMAT_MASK;
		ASC_Protocol[ADDR_FORMAT_INDEX] |= MSG_FORMAT_ASC & MESSAGE_FORMAT_MASK;

		// UART Communication Area(Window) Enable
		GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_TX_DATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_RX_DATA)->EnableWindow(TRUE);

		// LED Area(Window) Enable
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);

		// FND Area(Window) Enable
		GetDlgItem(IDC_DEC)->EnableWindow(TRUE);
		GetDlgItem(IDC_HEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BT_DEC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BT_HEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BT_BIN)->EnableWindow(TRUE);

		// ADC Area(Window) Enable
		GetDlgItem(IDC_CUR_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_VR_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CS_STATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADC_PERIOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BT_ADC_PERIOD)->EnableWindow(TRUE);

		GetDlgItem(IDC_STATIC_ADC1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC3)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC4)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC5)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC6)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC7)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC9)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_ADC10)->EnableWindow(TRUE);

		// Tact Switch Area(Window) Enable
		GetDlgItem(IDC_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_SW)->EnableWindow(TRUE);

		// Timer Period Area(Window) Enable
		GetDlgItem(IDC_STATIC_TIMER1)->EnableWindow(TRUE);
		GetDlgItem(IDC_PERIOD_1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BT_PERIOD1)->EnableWindow(TRUE);
		GetDlgItem(IDC_TEXT_TIMER1)->EnableWindow(TRUE);

		// DC Motor ON/OFF Button Enable
		GetDlgItem(IDC_MOTOR_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_MOTOR_OFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_DUTY_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BT_DUTY_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_TEXT_MOTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_MOTOR_CW)->EnableWindow(FALSE);
		GetDlgItem(IDC_MOTOR_CCW)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MOTOR1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MOTOR2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MOTOR3)->EnableWindow(FALSE);

		// Servo Motor Area(Window) Enable
		GetDlgItem(IDC_PLUS_10)->EnableWindow(TRUE);
		GetDlgItem(IDC_MINUS_10)->EnableWindow(TRUE);
		GetDlgItem(IDC_LOCATION_0)->EnableWindow(TRUE);
		GetDlgItem(IDC_LOCATION_90)->EnableWindow(TRUE);
		GetDlgItem(IDC_LOCATION_180)->EnableWindow(TRUE);

		// DAC Area(Window) Enable
		GetDlgItem(IDC_SOUND1)->EnableWindow(TRUE);
		GetDlgItem(IDC_SOUND2)->EnableWindow(TRUE);

		// Security Area(Window) Enable
		GetDlgItem(IDC_KEY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_KEY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADMIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_ROOT)->EnableWindow(TRUE);
	}
}


void CMFCApplication1Dlg::OnBnClickedSend()
{
	CString SendStr, temp = "Transmit Data: ";
	unsigned int i = 0, len = temp.GetLength();
	
	m_Tx_Data.GetWindowTextA(SendStr);

	if (SendStr.GetLength() != ASCII_TOTAL_LENGTH - 3) {
		m_Tx_Data.SetWindowTextA("");
		MessageBox(_T("Protocol을 준수하지 않은 입력입니다."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		for (i = ASC_TYPE_INDEX; i < ASC_ETX_INDEX; i++)
		{
			ASC_Protocol[i] = SendStr.GetAt(i - 2);
			if ((i == ASC_GROUP_NUM_INDEX) && (ASC_Protocol[i] > 0x37 || ASC_Protocol[i] < 0x30))
			{
				MessageBox(_T("Protocol을 준수하지 않은 입력입니다."), _T("ERROR"), MB_ICONSTOP);
				return;
			}
				
			temp += ASC_Protocol[i];
		}

		m_Comm.WriteCommBlock(ASC_Protocol, DWORD(ASCII_TOTAL_LENGTH));

		// Cumulate String Data in Main Display
		if (temp.GetLength() > len)
		{
			m_Rx_Data_T += temp;
			m_Rx_Data_T += "\r\n";
			m_Rx_Data.SetWindowTextA(m_Rx_Data_T);
		}
	}
}


LONG CMFCApplication1Dlg::OnReceiveData(UINT WParam, LONG a)
{
	BYTE rec_str;
	CString str, data = "", buffer = "", temp = "Receive Data: ";
	int nLen = WParam, tLen = temp.GetLength(), adc_value = 0x00, delimiter = 0x00;
	
	for (int i = 0; i < nLen; i++)
	{
		rec_str = (int)m_Comm.abIn[i];
		buffer.Format(_T("%c"), rec_str);

		if (buffer == '\r')
			buffer = "\r\n";

		temp += buffer;
	}

	//m_Rx_Data_T += temp;
	//m_Rx_Data.SetWindowTextA(m_Rx_Data_T);
	//m_Rx_Data.LineScroll(m_Rx_Data.GetLineCount());

	for (int i = tLen; i < temp.GetLength(); i++) {
		if (temp[i] == START) {
			if (i + 1 < temp.GetLength() && temp[i + 1] == BIN_Protocol[ADDR_FORMAT_INDEX]) {
				if (i + BIN_DATAL_INDEX < temp.GetLength() && (temp[i + BIN_TYPE_GN_COM_INDEX] >> 6 & BIT2_MASK) == MSG_TYPE_MONITOR) {
					switch ((temp[i + BIN_TYPE_GN_COM_INDEX] >> 3) & BIT3_MASK) {
						case GROUP_NUM_TACT:
							// Tact Switch Count Time Monitoring
							if ((temp[i + BIN_TYPE_GN_COM_INDEX] & BIT3_MASK) == COMMAND_001) {
								if (((temp[i + BIN_LEN_DATAH_INDEX] >> 4) & BIT4_MASK) == DATA_LENGTH_12)
								{
									data.Format("%d", BYTE(++switch_count));
									m_SW_Count.SetWindowTextA(data);
								}
							}
							break;
						case GROUP_NUM_ADC:
							adc_value = temp[i + BIN_DATAL_INDEX] & DATA_ADC_MASK;
							delimiter = ((temp[i + BIN_LEN_DATAH_INDEX] & BIT1_MASK) * 2) + ((temp[i + BIN_DATAL_INDEX] >> 7) & BIT1_MASK);
							m_VR_Value.SetWindowTextA(""); m_Temp.SetWindowTextA(""); m_CS_State.SetWindowTextA("");

							// ADC Variable Resistor Value Monitoring
							if ((temp[i + BIN_TYPE_GN_COM_INDEX] & BIT3_MASK) == COMMAND_001) {
								if (((temp[i + BIN_LEN_DATAH_INDEX] >> 4) & BIT4_MASK) == DATA_LENGTH_9)
								{
									if (delimiter == 0b01)
									{
										m_ADC_Mode.SetWindowTextA("Mode 1");

										str.Format("%d[%%]\r\n", adc_value);
										m_VR_Value.SetWindowTextA(str);
									}
								}
							}
							// ADC Thermal Sensor Value Monitoring
							else if ((temp[i + BIN_TYPE_GN_COM_INDEX] & BIT3_MASK) == COMMAND_002) {
								if (((temp[i + BIN_LEN_DATAH_INDEX] >> 4) & BIT4_MASK) == DATA_LENGTH_9)
								{
									if (delimiter == 0b10)
									{
										m_ADC_Mode.SetWindowTextA("Mode 2");

										str.Format("%d[℃]\r\n", adc_value);
										m_Temp.SetWindowTextA(str);
									}
								}
							}
							// ADC Cds Sensor Value Monitoring
							else if ((temp[i + BIN_TYPE_GN_COM_INDEX] & BIT3_MASK) == COMMAND_003) {
								if (((temp[i + BIN_LEN_DATAH_INDEX] >> 4) & BIT4_MASK) == DATA_LENGTH_9)
								{
									if (delimiter == 0b11)
									{
										m_ADC_Mode.SetWindowTextA("Mode 3");

										if (adc_value)
											m_CS_State.SetWindowTextA("Bright");
										else
											m_CS_State.SetWindowTextA("Dark");
									}
								}
							}
							break;
					}
				}
			}
		}
	}

	return TRUE;
}


void CMFCApplication1Dlg::OnBnClickedDelete()
{
	m_Rx_Data_T = "";
	m_Rx_Data.SetWindowTextA(m_Rx_Data_T);
}


void CMFCApplication1Dlg::OnBnClickedBtDec() // FND Decimal Control
{
	CString dec_str;

	m_dec.GetWindowTextA(dec_str);
	if (dec_str.GetLength() > 1) {
		m_dec.SetWindowTextA("");
		MessageBox(_T("1자리를 초과할 수 없습니다."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		ASC_Protocol[ASC_TYPE_INDEX] = MSG_TYPE_CONTROL + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_GROUP_NUM_INDEX] = GROUP_NUM_FND + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_COMMAND_INDEX] = COMMAND_001 + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_DATA_LEN_INDEX] = DATA_LENGTH_1 + ASC_INDEX_OFFSET;

		for (int i = ASC_DATA0_INDEX; i < ASC_ETX_INDEX; i++) {

			if (i == ASC_DATA3_INDEX)
			{
				if (dec_str.GetAt(i - ASC_DATA3_INDEX) < 48 || dec_str.GetAt(i - ASC_DATA3_INDEX) > 57) {
					m_dec.SetWindowTextA("");
					MessageBox(_T("숫자만 입력하십시오."), _T("ERROR"), MB_ICONSTOP);
					return;
				}

				ASC_Protocol[i] = dec_str.GetAt(i - ASC_DATA3_INDEX);
			}
			else
				ASC_Protocol[i] = 0 + ASC_INDEX_OFFSET;
		}

		m_Comm.WriteCommBlock(ASC_Protocol, DWORD(ASCII_TOTAL_LENGTH));
	}
}


void CMFCApplication1Dlg::OnBnClickedBtHex() // FND Hexadecimal Control
{
	CString hex_str;

	m_hex.GetWindowTextA(hex_str);
	if (hex_str.GetLength() > 1) {
		m_hex.SetWindowTextA("");
		MessageBox(_T("1자리를 초과할 수 없습니다."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		ASC_Protocol[ASC_TYPE_INDEX] = MSG_TYPE_CONTROL + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_GROUP_NUM_INDEX] = GROUP_NUM_FND + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_COMMAND_INDEX] = COMMAND_002 + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_DATA_LEN_INDEX] = DATA_LENGTH_1 + ASC_INDEX_OFFSET;

		for (int i = ASC_DATA0_INDEX; i < ASC_ETX_INDEX; i++) {

			if (i == ASC_DATA3_INDEX)
			{
				if (hex_str.GetAt(i - ASC_DATA3_INDEX) < 48 || hex_str.GetAt(i - ASC_DATA3_INDEX) > 57 && 
					hex_str.GetAt(i - ASC_DATA3_INDEX) < 65 || hex_str.GetAt(i - ASC_DATA3_INDEX) > 70) {
					m_hex.SetWindowTextA("");
					MessageBox(_T("16진수인 대문자와 숫자만 입력하십시오."), _T("ERROR"), MB_ICONSTOP);
					return;
				}

				ASC_Protocol[i] = hex_str.GetAt(i - ASC_DATA3_INDEX);
			}
			else
				ASC_Protocol[i] = 0 + ASC_INDEX_OFFSET;
		}

		m_Comm.WriteCommBlock(ASC_Protocol, DWORD(ASCII_TOTAL_LENGTH));
	}
}


void CMFCApplication1Dlg::OnBnClickedBtBin() // FND Binary Control
{
	CString bin_str;

	m_bin.GetWindowTextA(bin_str);
	if (bin_str.GetLength() > 4) {
		m_bin.SetWindowTextA("");
		MessageBox(_T("4자리를 초과할 수 없습니다."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		ASC_Protocol[ASC_TYPE_INDEX] = MSG_TYPE_CONTROL + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_GROUP_NUM_INDEX] = GROUP_NUM_FND + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_COMMAND_INDEX] = COMMAND_003 + ASC_INDEX_OFFSET;
		ASC_Protocol[ASC_DATA_LEN_INDEX] = DATA_LENGTH_4 + ASC_INDEX_OFFSET;

		for (int i = 0; i < DATA_LENGTH_4; i++) {
			if (i >= 4 - bin_str.GetLength()) {
				if (bin_str.GetAt(i - (4 - bin_str.GetLength())) != 49 && bin_str.GetAt(i - (4 - bin_str.GetLength())) != 48) {
					m_bin.SetWindowTextA("");
					MessageBox(_T("입력이 0 또는 1로만 구성되어야 합니다."), _T("ERROR"), MB_ICONSTOP);
					return;
				}

				ASC_Protocol[i + ASC_DATA0_INDEX] = bin_str.GetAt(i - (4 - bin_str.GetLength()));
			}
			else
				ASC_Protocol[i + ASC_DATA0_INDEX] = 0 + ASC_INDEX_OFFSET;
		}

		m_Comm.WriteCommBlock(ASC_Protocol, DWORD(ASCII_TOTAL_LENGTH));
	}
}


void CMFCApplication1Dlg::OnBnClickedBtPeriod1() // Timer Period 1
{
	unsigned int bowl = 0;
	CString period1_str;

	m_Period1.GetWindowTextA(period1_str);
	bowl = _ttoi(period1_str);
	if (bowl > 1000 || bowl < 100) {
		m_Period1.SetWindowTextA("");
		MessageBox(_T("데이터 범주를 벗어났습니다. 다시 입력하십시오."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
			| ((GROUP_NUM_TIMER << 3) & GROUP_NUMBER_MASK) | COMMAND_001;
		BIN_Protocol[3] = (DATA_LENGTH_10 << 4) & DATA_LENGTH_MASK | (bowl >> 8) & BIT2_MASK;
		BIN_Protocol[4] = bowl & FULL_BIT_MASK;

		m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
	}
}


void CMFCApplication1Dlg::OnBnClickedBtAdcSend() // ADC Checking Period
{
	unsigned int bowl = 0;
	CString adc_period_str;

	m_ADC_Period.GetWindowTextA(adc_period_str);
	bowl = _ttoi(adc_period_str);
	if (bowl > 3000 || bowl < 1000) {
		m_ADC_Period.SetWindowTextA("");
		MessageBox(_T("데이터 범주를 벗어났습니다. 다시 입력하십시오."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK 
			| ((GROUP_NUM_TIMER << 3) & GROUP_NUMBER_MASK) | COMMAND_002;
		BIN_Protocol[3] = (DATA_LENGTH_12 << 4) & DATA_LENGTH_MASK | (bowl >> 8) & BIT4_MASK;
		BIN_Protocol[4] = bowl & FULL_BIT_MASK;

		m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
	}
}


void CMFCApplication1Dlg::OnBnClickedButton1() // LED 1 Toggle
{
	BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
		| ((GROUP_NUM_LED << 3) & GROUP_NUMBER_MASK) | COMMAND_001;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedButton2() // LED 2 Toggle
{
	BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
		| ((GROUP_NUM_LED << 3) & GROUP_NUMBER_MASK) | COMMAND_002;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedMotorOn() // DC Motor ON
{
	BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
		| ((GROUP_NUM_DC_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_001;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));

	// DC Motor ON/OFF Button Enable
	GetDlgItem(IDC_DUTY_RATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BT_DUTY_RATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_TEXT_MOTOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOTOR_CW)->EnableWindow(TRUE);
	GetDlgItem(IDC_MOTOR_CCW)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MOTOR2)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MOTOR3)->EnableWindow(TRUE);
}


void CMFCApplication1Dlg::OnBnClickedMotorOff() // DC Motor OFF
{
	BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
		| ((GROUP_NUM_DC_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_002;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));

	// DC Motor ON/OFF Button Disable
	GetDlgItem(IDC_DUTY_RATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BT_DUTY_RATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TEXT_MOTOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOTOR_CW)->EnableWindow(FALSE);
	GetDlgItem(IDC_MOTOR_CCW)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MOTOR2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MOTOR3)->EnableWindow(FALSE);
}


void CMFCApplication1Dlg::OnBnClickedMotorCw() // DC Motor Direction CW
{
	BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
		| ((GROUP_NUM_DC_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_004;
	BIN_Protocol[3] = (DATA_LENGTH_4 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = DATA_DC_CW & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedMotorCcw() // DC Motor Direction CCW
{
	BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
		| ((GROUP_NUM_DC_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_005;
	BIN_Protocol[3] = (DATA_LENGTH_4 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = DATA_DC_CCW & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedBtDutyRate() // DC Motor send Duty rate
{
	unsigned int bowl = 0;
	CString duty_rate_str;

	m_DC_DUTY.GetWindowTextA(duty_rate_str);
	bowl = _ttoi(duty_rate_str);
	if (bowl > 100 || bowl < 0) {
		m_DC_DUTY.SetWindowTextA("");
		MessageBox(_T("데이터 범주를 벗어났습니다. 다시 입력하십시오."), _T("ERROR"), MB_ICONSTOP);
	}
	else {
		BIN_Protocol[2] = (MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK
			| ((GROUP_NUM_DC_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_003;
		BIN_Protocol[3] = (DATA_LENGTH_7 << 4) & DATA_LENGTH_MASK | (bowl >> 8) & BIT1_MASK;
		BIN_Protocol[4] = bowl & FULL_BIT_MASK;

		m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
	}
}


void CMFCApplication1Dlg::OnBnClickedLocation0() // Servo Motor rotation to 0
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_SERVO_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_001;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedLocation90() // Servo Motor rotation to 90
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_SERVO_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_002;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedLocation180() // Servo Motor rotation to 180
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_SERVO_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_003;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedPlus10() // Servo Motor control to plus 10
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_SERVO_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_004;
	BIN_Protocol[3] = (DATA_LENGTH_4 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = DATA_SERVO_PLUS_10 & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedMinus10() // Servo Motor control to minus 10
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_SERVO_MOTOR << 3) & GROUP_NUMBER_MASK) | COMMAND_005;
	BIN_Protocol[3] = (DATA_LENGTH_4 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = DATA_SERVO_MINUS_10 & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedSound1() // DAC for Sound 1
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_DAC << 3) & GROUP_NUMBER_MASK) | COMMAND_001;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedSound2() // DAC for Sound 2
{
	BIN_Protocol[2] = ((MSG_TYPE_CONTROL << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_DAC << 3) & GROUP_NUMBER_MASK) | COMMAND_002;
	BIN_Protocol[3] = (DATA_LENGTH_0 << 4) & DATA_LENGTH_MASK;
	BIN_Protocol[4] = 0;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedKey1() // Write Protection key 1
{
	BIN_Protocol[2] = ((MSG_TYPE_SECURITY << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_Write_Protection << 3) & GROUP_NUMBER_MASK) | COMMAND_006;
	BIN_Protocol[3] = (DATA_LENGTH_12 << 4) & DATA_LENGTH_MASK | (security_key1 >> 8) & BIT4_MASK;;
	BIN_Protocol[4] = security_key1 & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedKey2() // Write Protection key 2
{
	BIN_Protocol[2] = ((MSG_TYPE_SECURITY << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_Write_Protection << 3) & GROUP_NUMBER_MASK) | COMMAND_007;
	BIN_Protocol[3] = (DATA_LENGTH_12 << 4) & DATA_LENGTH_MASK | (security_key2 >> 8) & BIT4_MASK;;
	BIN_Protocol[4] = security_key2 & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedAdmin() // Admin mode
{
	BIN_Protocol[2] = ((MSG_TYPE_SECURITY << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_Privilege << 3) & GROUP_NUMBER_MASK) | COMMAND_000;
	BIN_Protocol[3] = (DATA_LENGTH_12 << 4) & DATA_LENGTH_MASK | (admin_mode >> 8) & BIT4_MASK;;
	BIN_Protocol[4] = admin_mode & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}


void CMFCApplication1Dlg::OnBnClickedRoot() // Root mode
{
	BIN_Protocol[2] = ((MSG_TYPE_SECURITY << 6) & MESSAGE_TYPE_MASK)
		| ((GROUP_NUM_Privilege << 3) & GROUP_NUMBER_MASK) | COMMAND_007;
	BIN_Protocol[3] = (DATA_LENGTH_12 << 4) & DATA_LENGTH_MASK | (root_mode >> 8) & BIT4_MASK;;
	BIN_Protocol[4] = root_mode & FULL_BIT_MASK;

	m_Comm.WriteCommBlock(BIN_Protocol, DWORD(BINARY_TOTAL_LENGTH));
}
