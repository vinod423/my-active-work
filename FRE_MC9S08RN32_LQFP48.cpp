//--------------------------------------------------------------------------//
//          Copyright (c) 2003, Data I/O Corporation                        //
//          10525 Willows Rd NE / Redmond, WA 98073 / (206) 881-6444        //
//__________________________________________________________________________//
/***************************************************************************

    PINFILE:    FRE_MC9S08RN32_LQFP48.CPP            Author: Vinod Dosapati

    Description: FREESCALE MC9S08RN32
    Package: LQFP48

 ***************************************************************************/

#include "standard.hpp"     // defines BYTE, WORD, etc.
#include "PinDefines.hpp"   // #defines for pins and packages  
#include "DevParms.hpp"     // structure for pin files
#include "FlashAPI.hpp"			// defines for socket, socketstatus
#include "FlashAlg2.hpp" 		// defines base class 

#include "StdWiggler.hpp"		//BUS Type defines
#include "FRE_MC9S08P.hpp"	    // class definition for this algo
#include "FRE_MC9S08P_CODE.hpp"	// assembler code array for this algo&device

//-----------Begin standardized algorithm parameters-------------------------

DEVPARMS _parms = {
    MICRO,                  // DEVPARMS type  ---> MICRO
    2522,                   // adapter ID
    CONTINUITY1,            // continuity check
    0x0000,                 // mfg ID, 
    0x0040,                 // device ID
    8,                      // data width
    0x8000,                 // device size address space of micro,-32KB
    60,                     // number of sectors
    48,                     // number of pins
    500,                    // Vcc voltage
    500,                    // Vpp voltage
    450,                    // Vih voltage
    0,                      // Alt voltage
    130,                    // erase time (ms) - timeout
    300,                    // program time (us) - timeout for one phrase (8bytes)
    false,                  // Sector Protect: device does not support protect
    false,                  // Software Data Protect
    false,                  // Boot Block Protect1
    SECTOR_BLANK_CHECK | USING_SPECIAL_FEATURES, // alg features
    0,                      // reserved integer 1
    0,                      // reserved integer 2
    0,                      // reserved integer 3
    0,                      // reserved integer 4
    530, 					// voltage_vcc_high;   
    530,					// voltage_vpp_high;
    420,					// voltage_vih_high;
	290, 					// voltage_vcc_low;    	
    290, 					// voltage_vpp_low;
    280, 					// voltage_vih_low;
	0,						// voltage_vcc_prog;    
    0,						// voltage_vpp_prog;
    0	 					// voltage_vih_prog;
};


//Q2522
DEVPINS _pins[] = 
{
    OE_NOT,      MANUAL,    // pin 1 PTD1 (SS_)
    A31,         MANUAL,    // pin 2 PTD0 SCK
    NOCONN,      NO_MODE,   // pin 3
    NOCONN,      NO_MODE,   // pin 4 BUSOUT - TP
    VCC,         VCC_RES,   // pin 5  VDD
    VCC,         VCC_RES,   // pin 6  VDDA
    GND,         GND_RES,   // pin 7  VSSA
    GND,         GND_RES,   // pin 8  VSS
    D7,          BUS,       // pin 9  PTB7
    D6,          BUS,       // pin 10 PTB6
    GND,         GND_RES,   // pin 11  VSS
    NOCONN,      NO_MODE,   // pin 12
    
    NOCONN,      NO_MODE,   // pin 13
    D5,          BUS,       // pin 14  PTB5
    D4,          BUS,       // pin 15  PTB4
    D11,         BUS,       // pin 16  PTC3
    D10,         BUS,       // pin 17  PTC2
    NOCONN,      NO_MODE,   // pin 18
    NOCONN,      NO_MODE,   // pin 19
    NOCONN,      NO_MODE,   // pin 20
    D9,          BUS,       // pin 21  PTC1
    D8,          BUS,       // pin 22  PTC0
    D3,          BUS,       // pin 23  PTB3
    D2,          BUS,       // pin 24  PTB2

    D1,          BUS,       // pin 25  PTB1
    D0,          BUS,       // pin 26  PTB0
    NOCONN,      NO_MODE,   // pin 27
    NOCONN,      NO_MODE,   // pin 28
    NOCONN,      NO_MODE,   // pin 29
    GND,         GND_RES,   // pin 30  VSS
    VCC,         VCC_RES,   // pin 31  VDD
    NOCONN,      NO_MODE,   // pin 32
    A25,         MANUAL,    // pin 33  PTD3: host -> target 
    A4,          MANUAL,    // pin 33  PTD2: host -> target 
    GP1,         MANUAL,    // pin 35  PTA3: target -> host (G1)
    GP2,         MANUAL,    // pin 36  PTA2: target -> host (G2)

    NOCONN,      NO_MODE,   // pin 37
    NOCONN,      NO_MODE,   // pin 38
    D15,         BUS,       // pin 39  PTC7
    D14,         BUS,       // pin 40  PTC6
    NOCONN,      NO_MODE,   // pin 41
    NOCONN,      NO_MODE,   // pin 42
    NOCONN,      NO_MODE,   // pin 43
    NOCONN,      NO_MODE,   // pin 44
    D13,         BUS,       // pin 45  PTC5
    D12,         BUS,       // pin 46  PTC4
    A5,          MANUAL,    // pin 47 <RST_NOT>
    CE_NOT,      MANUAL,    // pin 48 <BKGD>
};      // endof PinOut


DEVSECTORS _sects[] = 
{
  // following the sectortable with BYTE addresses
  // Image addresses
    //0x00001040,0x000011FF,        // Block        0   448
    //0x00001200,0x000013FF,        // Block        1   512
    //0x00001400,0x000015FF,        // Block        2   512
    //0x00001600,0x000017FF,        // Block        3   512
    //0x00001800,0x000019FF,        // Block        4   512
    //0x00001A00,0x00001BFF,        // Block        5   512
    //0x00001C00,0x00001DFF,        // Block        6   512
    //0x00001E00,0x00001FFF,        // Block        7   512
    //0x00002000,0x000021FF,        // Block        8   512
    //0x00002200,0x000023FF,        // Block        9   512
    //0x00002400,0x000025FF,        // Block       10   512
    //0x00002600,0x000027FF,        // Block       11   512
    //0x00002800,0x000029FF,        // Block       12   512
    //0x00002A00,0x00002BFF,        // Block       13   512
    //0x00002C00,0x00002DFF,        // Block       14   512
    //0x00002E00,0x00002FFF,        // Block       15   512
    0x00003100,0x000031FF,        // Block       16   256 - EEPROM
    //0x00003200,0x000033FF,        // Block       17   512
    //0x00003400,0x000035FF,        // Block       18   512
    //0x00003600,0x000037FF,        // Block       19   512
    //0x00003800,0x000039FF,        // Block       20   512
    //0x00003A00,0x00003BFF,        // Block       21   512
    //0x00003C00,0x00003DFF,        // Block       22   512
    //0x00003E00,0x00003FFF,        // Block       23   512
    //0x00004000,0x000041FF,        // Block       24   512
    //0x00004200,0x000043FF,        // Block       25   512
    //0x00004400,0x000045FF,        // Block       26   512
    //0x00004600,0x000047FF,        // Block       27   512
    //0x00004800,0x000049FF,        // Block       28   512
    //0x00004A00,0x00004BFF,        // Block       29   512
    //0x00004C00,0x00004DFF,        // Block       30   512
    //0x00004E00,0x00004FFF,        // Block       31   512
    //0x00005000,0x000051FF,        // Block       32   512
    //0x00005200,0x000053FF,        // Block       33   512
    //0x00005400,0x000055FF,        // Block       34   512
    //0x00005600,0x000057FF,        // Block       35   512
    //0x00005800,0x000059FF,        // Block       36   512
    //0x00005A00,0x00005BFF,        // Block       37   512
    //0x00005C00,0x00005DFF,        // Block       38   512
    //0x00005E00,0x00005FFF,        // Block       39   512
    //0x00006000,0x000061FF,        // Block       40   512
    //0x00006200,0x000063FF,        // Block       41   512
    //0x00006400,0x000065FF,        // Block       42   512
    //0x00006600,0x000067FF,        // Block       43   512
    //0x00006800,0x000069FF,        // Block       44   512
    //0x00006A00,0x00006BFF,        // Block       45   512
    //0x00006C00,0x00006DFF,        // Block       46   512
    //0x00006E00,0x00006FFF,        // Block       47   512
    //0x00007000,0x000071FF,        // Block       48   512
    //0x00007200,0x000073FF,        // Block       49   512
    //0x00007400,0x000075FF,        // Block       50   512
    //0x00007600,0x000077FF,        // Block       51   512
    //0x00007800,0x000079FF,        // Block       52   512
    //0x00007A00,0x00007BFF,        // Block       53   512
    //0x00007C00,0x00007DFF,        // Block       54   512
    //0x00007E00,0x00007FFF,        // Block       55   512
    0x00008000,0x000081FF,        // Block       56   512
    0x00008200,0x000083FF,        // Block       57   512
    0x00008400,0x000085FF,        // Block       58   512
    0x00008600,0x000087FF,        // Block       59   512
    0x00008800,0x000089FF,        // Block       60   512
    0x00008A00,0x00008BFF,        // Block       61   512
    0x00008C00,0x00008DFF,        // Block       62   512
    0x00008E00,0x00008FFF,        // Block       63   512
    0x00009000,0x000091FF,        // Block       64   512
    0x00009200,0x000093FF,        // Block       65   512
    0x00009400,0x000095FF,        // Block       66   512
    0x00009600,0x000097FF,        // Block       67   512
    0x00009800,0x000099FF,        // Block       68   512
    0x00009A00,0x00009BFF,        // Block       69   512
    0x00009C00,0x00009DFF,        // Block       70   512
    0x00009E00,0x00009FFF,        // Block       71   512
    0x0000A000,0x0000A1FF,        // Block       72   512
    0x0000A200,0x0000A3FF,        // Block       73   512
    0x0000A400,0x0000A5FF,        // Block       74   512
    0x0000A600,0x0000A7FF,        // Block       75   512
    0x0000A800,0x0000A9FF,        // Block       76   512
    0x0000AA00,0x0000ABFF,        // Block       77   512
    0x0000AC00,0x0000ADFF,        // Block       78   512
    0x0000AE00,0x0000AFFF,        // Block       79   512
    0x0000B000,0x0000B1FF,        // Block       80   512
    0x0000B200,0x0000B3FF,        // Block       81   512
    0x0000B400,0x0000B5FF,        // Block       82   512
    0x0000B600,0x0000B7FF,        // Block       83   512
    0x0000B800,0x0000B9FF,        // Block       84   512
    0x0000BA00,0x0000BBFF,        // Block       85   512
    0x0000BC00,0x0000BDFF,        // Block       86   512
    0x0000BE00,0x0000BFFF,        // Block       87   512
    0x0000C000,0x0000C1FF,        // Block       88   512
    0x0000C200,0x0000C3FF,        // Block       89   512
    0x0000C400,0x0000C5FF,        // Block       90   512
    0x0000C600,0x0000C7FF,        // Block       91   512
    0x0000C800,0x0000C9FF,        // Block       92   512
    0x0000CA00,0x0000CBFF,        // Block       93   512
    0x0000CC00,0x0000CDFF,        // Block       94   512
    0x0000CE00,0x0000CFFF,        // Block       95   512
    0x0000D000,0x0000D1FF,        // Block       96   512
    0x0000D200,0x0000D3FF,        // Block       97   512
    0x0000D400,0x0000D5FF,        // Block       98   512
    0x0000D600,0x0000D7FF,        // Block       99   512
    0x0000D800,0x0000D9FF,        // Block      100   512
    0x0000DA00,0x0000DBFF,        // Block      101   512
    0x0000DC00,0x0000DDFF,        // Block      102   512
    0x0000DE00,0x0000DFFF,        // Block      103   512
    0x0000E000,0x0000E1FF,        // Block      104   512
    0x0000E200,0x0000E3FF,        // Block      105   512
    0x0000E400,0x0000E5FF,        // Block      106   512
    0x0000E600,0x0000E7FF,        // Block      107   512
    0x0000E800,0x0000E9FF,        // Block      108   512
    0x0000EA00,0x0000EBFF,        // Block      109   512
    0x0000EC00,0x0000EDFF,        // Block      110   512
    0x0000EE00,0x0000EFFF,        // Block      111   512
    0x0000F000,0x0000F1FF,        // Block      112   512
    0x0000F200,0x0000F3FF,        // Block      113   512
    0x0000F400,0x0000F5FF,        // Block      114   512
    0x0000F600,0x0000F7FF,        // Block      115   512
    0x0000F800,0x0000F9FF,        // Block      116   512
    0x0000FA00,0x0000FBFF,        // Block      117   512
    0x0000FC00,0x0000FDFF,        // Block      118   512
    0x0000FE00,0x0000FFFF,        // Block      119   512
};    // endof Sector
//-----------End standardized algorithm parameters-------------------------

//----------------------------------------------------------------------------------------------
//                                       RST, IN1 (RELEASE), IN2 (HS_IN),  OUT1(HS_OUT), OUT2
//                                                  PTD2         PTD3          PTD1      PTA3
PIN_NAME_E _MC9S08RN_QFP48_cntrlpins[] = {A5,       A4,          A25,          OE,       GP1};


C_MC9S08P::S08_DEV_PARMS _tDeviceTable =
{
   0xFF6E,       // factory programmed FTRIM value saved here - not prooved yet, first samples are blank
   0xFF6F,       // factory programmed TRIM value saved here  - not prooved yet, first samples are blank
   0x303A,       // main trim register address
};


C_MC9S08P::S08_CODE_PARMS _tCodeTable =
{
  0x023F,        // stack pointer setting
  0x0040,        // parameter address in RAM
  0x0060,        // start address of the algorithm in RAM
     302,        // code length
  &chaMC9S08RN60_alg_code[0]
};


//register will be set in this order -> select them carefully
C_MC9S08P::S08_REG_PARMS _tSetupRegTable[] = 
{ //    type             addr  val
  {0x3034,0xFF},   //WDOG_TOVALH -> configure WD timeout
  {0x3035,0xFF},   //WDOG_TOVALL -> configure WD timeout
  {0x3031,0x01},   //WDOG_CS2 -> configure WD
  {0x3030,0x00},   //WDOG_CS1 -> disable WD
  //{0x3006,0x08},  //SYS_SOPT3 -> CLKOE - just for testing clock at PTH2


  {0x3020,0x47},  //NVM_FCLKDIV: DIV = 7 -> fFCLK = fBUS / (1+DIV) = 8MHz / 8 = 1MHz (ok!)
  {0x3028,0xFF},  //NVM_FPROT: remove FLASH protections
  {0x3029,0x80},  //NVM_EEROT: remove EEPROM protections
  {0x3026,0x30},  //NVM_FSTAT: initialize (clear error bits)
  	                                       //initialize unused ports: enable pull-ups

  {0x0001,0x5A},  //FOR PORT TESTING: PTB = 0x5A
  {0x30B1,0xFF},  //FOR PORT TESTING: PTBDD: init Port A as output

  {EOT,0}
};

//register will be set in this order -> select them carefully
C_MC9S08P::S08_REG_PARMS _tBusRegTable[] = 
{
  {0x0001,0x00},  //PTB: init Port B with signals set low.
  {0x30B1,0xFF},  //PTBDD: init Port B as output
  {EOT,0}
};

//register will be checked in this order
C_MC9S08P::S08_STATUS_REG_PARMS _tStatusRegTable[] = 
{ //addr  exp  mask
  {0x3000,0x00,0xC2},  //expected value for SYS_SRS: WDOG, ILOP, LOC bits must be always zero
  {0x3020,0xC7,0x00},  //expected value for NVM_FCLKDIV
  {0x3026,0x80,0x44},  //expected value for NVM_FSTAT
  {0x3027,0x00,0xFC},  //expected value for NVM_FERSTAT
  {0x303C,0x50,0x00},  //expected value for ICS_S
  {EOT,0,0}  // <-- END RECORD
};

/*************************************************************************
FUNCTION 	AlgoCreate()
ARGUMENTS  	none
RETURNS		pointer to algorithm object
METHOD
EXCEPTIONS --  none

This function is called by LmAlginit to create an algorithm object 
*************************************************************************/	
RRAlgorithm* AlgoCreate (void)
{
	return (new C_MC9S08P(&_parms, &_pins[0], &_sects[0], &_MC9S08RN_QFP48_cntrlpins[0]
	           , &_tDeviceTable, &_tSetupRegTable[0], &_tBusRegTable[0]
	           , &_tCodeTable, &_tStatusRegTable[0]
          ));
}
