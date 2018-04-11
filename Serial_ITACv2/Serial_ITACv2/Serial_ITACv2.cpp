//Serial_ITACV2 : Defiens the entry point for the console application.
//
#include "stdafx.h"
#include <string.h>
#include <stdio.h>

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>



 /*                                                                        *
 ***************************************************************************
 *                                                                         *
 *   SERIALIZ.C                                                            *
 *                                                                         *
 *   Version 1.52                                                          *
 *                                                                         *
 *   A external serialization program for use with the TaskLink program.   *
 *                                                                         *
 *   Generates serial numbers of the following types:                      *
 *                                                                         *
 *       Binary 1 to 4 byte                                                *
 *       ASCII decimal up to 10 bytes                                      *
 *       ASCII hexadecimal up to 10 bytes                                  *
 *                                                                         *
 *   Can generate up to 30 serial numbers per call to the program.         *
 *   Number can be incremented by user-specified amount.                   *
 *   Number can be placed in RAM in forward or reverse order.              *
 *                                                                         *
 *   Label text containing serial number in decimal or hex is generated.   *
 *                                                                         *
 *   Program can optionally calculate changing RAM sumcheck for display    *
 *   in TaskLink status window.                                            *
 *                                                                         *
 ***************************************************************************
 *                                                                         *
 *   User-Specified Command Line Parameters                                *
 *                                                                         *
 *   -a    Address of serial number.                                       *
 *         Hexadecimal value indicating the offset in the device at        *
 *         which the serial number is to be located.                       *
 *         -a12345 = place serial number at hex address 12345.             *
 *         Defaults to 0 if not supplied.                                  *
 *                                                                         *
 *   -c    Compensation Address                                            *                                                                          *
 *         Hexadecimal value indicating the offset in the device at        *
 *         which the a compensation value is to be located.                *
 *         -c12345 = place compensation value at hex address 12345.        *
 *         Causes the serializ program to generate a sumcheck compensation *
 *         value for each serial number.                                   *
 *         Defaults to disabled if not supplied.                           *
 *                                                                         *
 *   -d    Delete SERIAL.DAT file. TaskLink no longer does this. The       *
 *         default is to leave the file intack. This flag is used by       *
 *         TaskLink however get passes here. Who knows it may get used     *
 *         someday.                                                        *
 *                                                                         *
 *   -e    Ethernet Mask                                                   *                                                                      *
 *         Hexadecimal value indicating a fix prefix value to be placed    *
 *         in front of each generated serial number.                       *
 *         -e0xABCD and a generated serial number of 1 would create the    *
 *         value ABCD1 to be programmed into the device.                   *
 *                                                                         *
 *         Must be used with the -z flag which specifies the length of the *
 *         mask value. Also the -l flag which specifies the length of the  *
 *         serial number must be set to the size of the total length.      *
 *         The length of the generated serial number is caluclated by      *
 *         subtracting the total length from the mask length               *
 *         (gLen = tLen - eLen)                                            *
 *                                                                         *
 *         EX:  serializ -l8 -z4 -e0xaa and a generated serial of 1        *
 *         would create the value 00AA0001 to be programmed into the       *
 *         device.                                                         *
 *                                                                         *
 *   -f    Format of serial number.                                        *
 *         -fb = binary                                                    *
 *         -fc = BCD                                                       *
 *         -fd = ASCII decimal                                             *
 *         -fh = ASCII hexadecimal                                         *
 *         Defaults to binary of not supplied.                             *
 *                                                                         *
 *   -i    Amount to increment serial number each time.                    * 
 *         -i2 = increment by 2.                                           *
 *         Defaults to 1 if not supplied.                                  *
 *                                                                         *
 *   -l    Length of serial number field.                                  *
 *         Number of bytes that the serial number will occupy.             *
 *         Must be 1-4 for binary format, 1 to 20 for ASCII.               *
 *         Defaults to 2 if not supplied.                                  *
 *                                                                         *
 *   -m    "Pure-Set" gang serialization mode flag                         *
 *         Causes generation of serial numbers one device-size             *
 *         apart in programmer RAM instead of in a single block            *
 *         just beyond the fixed device data.  Also causes TaskLink        *
 *         to program devices in a single-pass set operation instead       *
 *         of a two-pass gang/set operation.  Inclusion of this            *
 *         flag forces the user to load RAM with a duplicate               *
 *         copy of fixed device data for each socket and results           *
 *         in considerably longer programming time.  Should only           *
 *         be used if device cannot be block-programmed.                   *
 *                                                                         *
 *   -o    Byte order of serial number.                                    *
 *         Determines whether the serial number is placed in RAM with      *
 *         the MSB at the first RAM address or the LSB at the first        *
 *         RAM address.  Works for binary and ASCII formats.               *
 *         -om = MSB at lower RAM address (Motorola-style)                 *
 *         -oi = LSB at lower RAM address (Intel-style)                    *
 *         Defaults to Motorola-style if not supplied.                     *
 *                                                                         *
 *   -s    Sumcheck calculation flag.                                      *
 *         When present, causes this program to calculate a new RAM        *
 *         sumcheck based on the sum of the serial number bytes and        *
 *         the initial sumcheck value passed to this program by            *
 *         TaskLink.  Only meaningful if the initial RAM data (before      *
 *         serializing begins) contains zeroes in the serial number        *
 *         position(s).                                                    *
 *                                                                         *
 *   -t    Label text format.                                              *
 *         Determines the format of the text suppled for inclusion in      *
 *         a label serial field (two or more percent-signs in a row        *
 *         in the QA Autolabel/laser text field).                          *
 *         -td = decimal                                                   *
 *         -th = hexadecimal                                               *
 *                                                                         *
 *   -w    Waring Limit.                                                   *
 *         Can be used to set a warning level before the serial            *
 *         number limit is reached. The warning limit is only              *
 *         used when an ending serial number is specified. If the          *
 *         current serial number is within the ending serial               *
 *         number minus the warning limit, a warning message will          *
 *         be displayed on the TaskLink screen.                            *
 *         Default: Zero (-w0)                                             *
 *                                                                         *
 *   -z    Ethernet Mask Length                                            *
 *         Used to specify the length of the Ethernet Mask value.          *
 *         See the -e flag for details.                                                                *
 *                                                                         *
 ***************************************************************************/

#include <stdio.h>
#include <io.h>
#include<string.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
/*
 *   Function prototype for Motorola format generator function
 */
void SerialnumerFormat(FILE *fp, unsigned long addr, char *buffer, unsigned nbytes);
void programmingnumerFormat(FILE *fp, unsigned long addr, char *buffer, unsigned nbytes);
void HellaITACFormat(FILE *fp, unsigned long addr, char *buffer, unsigned nbytes);
/*
 *   Constant definitions
 */
#define MODE_GANGSET    0
#define MODE_PURESET    1
#define INPUT_DECIMAL   0
#define INPUT_HEX       1
#define FORMAT_BINARY   0
#define FORMAT_DECIMAL  1
#define FORMAT_HEX      2
#define FORMAT_BCD      3
#define ORDER_MOTOROLA  0
#define ORDER_INTEL     1
#define LABEL_DECIMAL   0
#define LABEL_HEX       1

/*
 *   Global variables
 */
int            Mode                       = MODE_GANGSET;
int            InputNumberFormat           =1;
int            MaskNumberFormat;
unsigned long  SerialNumber;
unsigned long  EndingSerialNumber;
unsigned long  EthernetMask;
int            NumberOfNumbers            = 1;
      
unsigned long  DeviceSizeInBytes          = 32;  /* Why not? */
unsigned long  Sumcheck;
int            SumcheckEnabled            = 1;
int            StartingNumberSpecified    = 1;
int            EndingNumberSpecified      = 0;
int            EthernetMaskSpecified      = 0;
int            CompAddressSpecified       = 0;
unsigned long  Address                    = 0;
unsigned long  CompAddress                = 0;
long           Increment                  = 1;
int            Length                     = 32;
int            Length1                    = 10;
int            Length2                    = 6;
int            Length3                    = 26;
int            eLength                    = 32;
int            dLength                    = 0;
int            sLength                    = 32;
int            Format                     = FORMAT_HEX;
int            Order                      = ORDER_MOTOROLA;                                              
int            LabelFormat                = LABEL_DECIMAL;
unsigned long  GangSerialNumber[30];
char           SerialNumBuffer[800];
char           ProgramDataBuffer[800];
char           HellaITACBuffer[800];
unsigned long  WarningLimit               = 0;
int            WarningMsgRcvd             = 0;
int            LastCall                   = 0;
int            ParameterError             = 0;
char           ErrorBuffer[60];
char           Dateread[6];
char		   actualDateStr[60]; 
char		   actualprgDateStr[60]        = "DDMMYY";

char		   TimeDateStr[60]             = "YYDD";
int            TimeDateStrSpecified        = 1;
char counterstr[60];                  
char           SerialnumberStr[13];

char		   ProgrammingDateStr[60]      = "DDMMYY"; 
int            ProgrammingDateStrSpecified = 1;

char           Manufacturenumber[20]       ="00596";
int            Hellapartnumberspecified;
char           Hellapartnumber[60];   
char           HellaSWVersion[60];


/*
Replace function : a helper function to rreplace cónstants with info  DD with %d

*/
#pragma region
int Replace (char* strTarget, char *mask, char *replace)
{
	char *p; 
	

	if (strlen (mask) != strlen (replace))
		return -1; 
	if (strlen(strTarget)<strlen(mask))
		return -1; 


	if ((p = strstr(strTarget, mask)) != NULL)
	{
		memcpy (p, replace, strlen(replace)); 
	}



	return 0; 
}
#pragma endregion

/*
Main Function

*/
int main(int argc, char *argv[])
{
   FILE *fp;
   int i, j;
   int divisor;
   char temp;
   char formatbuf[16];
   char *buffer_offset, *ptr1, *ptr2;
   unsigned long temp_number;
   unsigned long bcd_number;
   unsigned long mask_number;


   if ((fp = fopen("serial.dat", "w")) == NULL) {
	   fclose(fp);
        return(0);
      }
 
     

	for (i=0; i<argc; i++)
	{
		char* c = argv[i];
		printf ("Param %d -- %s\n", i, c);  
	}

	  if ((fp = fopen("date.dat", "a+")) == NULL) {
	    fclose(fp);
		return(0);
  	  
      }
	  
	  fscanf(fp,"%05d",&SerialNumber);
	  SerialNumber++;
	  printf("this is the actual%05d:",SerialNumber);
	  fscanf(fp,"%6s",&Dateread);
	  printf("this the date read: %s",Dateread);
   /*
    *   Parse the command line arguments.
    *   The arguments in upper-case letters are supplied by TaskLink.
    *   The lower-case letters are user-specified parameters.
    */
#pragma region
   for (i = 1; i < argc; i++)  {
      if ((argv[i])[0] == '-') {
         switch ((argv[i])[1]) {
            case 'D':
               /*
                *   Device size in bytes
                */
               if (sscanf(argv[i] + 2, "%lx", &DeviceSizeInBytes) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid device size");
                  ParameterError = 1;
               }
               break;
            case 'E':
               /*
                *   Ending serial number
                */
               if (sscanf(argv[i] + 2, "%li", &EndingSerialNumber) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid ending number");
                  ParameterError = 1;
               }
               EndingNumberSpecified = 1;
               break ;
            case 'F':
               /*
                *   Flag indicating first call to this program.
                *   Does nothing here.
                */
               break;
            case 'L':
               /*
                *   Flag indicating last call to this program.
                *   Does nothing here other than exit the program.
                */
               LastCall = 1;
               break;
            case 'N':
               /*
                *   Current serial number to be used
                */
               if (strncmp(argv[i] + 2, "0x", 2) == 0) 
                  InputNumberFormat = INPUT_HEX;
               else
                  InputNumberFormat = INPUT_DECIMAL;
               if (sscanf(argv[i] + 2, "%li", &SerialNumber) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid starting number");
                  ParameterError = 1;
               }   
               StartingNumberSpecified = 1;
               break;
          case 'P':
               /*
                *   Number of serial numbers to be generated
				but customized to read HELLA part number
                */
			  {
                 strncpy (Hellapartnumber, argv[i] + 2, sizeof(Hellapartnumber)); 
                  Hellapartnumberspecified = 1;
               }
               break;
            case 'Q':
               /*
                *   Number of serial numbers to be generated
                */
               if (sscanf(argv[i] + 2, "%i", &NumberOfNumbers) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid number of serial numbers");
                  ParameterError = 1;
               }
               break;
            case 'S':
               /*
                *   RAM sumcheck at beginning of session.
                */
               if (sscanf(argv[i] + 2, "%lx", &Sumcheck) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid starting sumcheck");
                  ParameterError = 2;
               }
               break;
            case 'W':
               /*
                *   Warning Message Recieved Flag
                */
               WarningMsgRcvd = 1;
               break;
            case 'a':
               /*
                *   Address of serial number
                */
               if (sscanf(argv[i] + 2, "%lx", &Address) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid address");
                  ParameterError = 1;
               }
               break;
            case 'c':
               /*
                *   Compensation Address of serial number
                */
               if (sscanf(argv[i] + 2, "%lx", &CompAddress) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid Compensation address");
                  ParameterError = 1;
               }
               CompAddressSpecified = 1;
               break;
            case 'd':
               /*
                *   Delete serial.dat file. Does nothing here. 
                *   Used by TaskLink, but does get passed here and who
                *   knows somebody may want to use it some day.
                */
               break;
            case 'e':
               /*
                *   Ethernet Mask for serial number
                */
               if (strncmp(argv[i] + 2, "0x", 2) == 0)
                  MaskNumberFormat = INPUT_HEX;
               else 
                  MaskNumberFormat = INPUT_DECIMAL;
               if (sscanf(argv[i] + 2, "%li", &EthernetMask) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid ethernet mask");
                  ParameterError = 1;
               }   
               EthernetMaskSpecified = 1;   
               break;
            case 'f':
               /*
                *   Serial number format
                */
               switch (*(argv[i] + 2)) {
                  case 'b':
                     Format = FORMAT_BINARY;
                     break;
                  case 'd':
                     Format = FORMAT_DECIMAL;
                     break;
                  case 'h':
                     Format = FORMAT_HEX;
                     break;
                  case 'c':
                     Format = FORMAT_BCD;
                     break;
               }
               break;

		
            case 'i':
               /*
                *   Amount serial number is incremented each time
                */
               if (sscanf(argv[i] + 2, "%li", &Increment) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid increment");
                  ParameterError = 1;
               }
               break;
            case 'l':
               /*
                *   Length of serial number field
                */
               if (sscanf(argv[i] + 2, "%i", &Length) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid length");
                  ParameterError = 1;
               }
               break;
            case 'm':
               /*
                *   Mode flag-- selects pure set mode
                */
               Mode = MODE_PURESET;
               break;
            case 'o':
               /*
                *   Byte order of serial number
                */
               switch (*(argv[i] + 2)) {
                  case 'm':
                     Order = ORDER_MOTOROLA;
                     break;
                  case 'i':
                     Order = ORDER_INTEL;
               }
               break;
            case 's':
               /*
                *   sw version supplied by Hella
                */

               {
                 strncpy (HellaSWVersion, argv[i] + 2, sizeof(HellaSWVersion)); 
                 
               }
               break;
            case 't':
               /*
                *   Label text format
                */
               switch (*(argv[i] + 2)) {
                  case 'd':
                     LabelFormat = LABEL_DECIMAL;
                     break;
                  case 'h':
                     LabelFormat = LABEL_HEX;
               }
               break;
            case 'w':
               /*
                *   Warning Limit 
                */
               if (sscanf(argv[i] + 2, "%li", &WarningLimit) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid Warning Limit");
                  ParameterError = 1;
               }
               break;
            case 'z':
               /*
                *   Length of ethernet mask field
                */
               if (sscanf(argv[i] + 2, "%i", &eLength) != 1) {
                  strcpy(ErrorBuffer, "T05:Invalid Ethernet length");
                  ParameterError = 1;
               }
               break;
         }
      }
         
      if (ParameterError == 1)
         break;        /* exit the for loop */
   }  
 #pragma endregion


      /* 
    * V1.42 - Do not open the "serial.dat" file on the last call to the 
    * program. This prevents a zero byte file and over-writing the last 
    * valid "serial.dat" file.
    */
   if (LastCall) {
      return(0);
   }
   else {
      if ((fp = fopen("serial.dat", "w")) == NULL) {
         fclose(fp);
         return(0);
      }
   }      



   /* 
    * If there is an error on the command line and this is not the last
    * call to the program, then put the error in the "serial.dat" file 
    * and return the program.
    */
   if (ParameterError == 1 ) {
      fprintf(fp, "%s\n", ErrorBuffer);
      fclose(fp);
      return(0);
   }



    /* 
    * Check to make sure the Serial Address and the Compensation Address do
    * not match or over lap each other. Only do this if the compensation 
    * address is specified.  Will try it in both directions just since we do
    * not know what adress is in will be first
    */
   if (CompAddressSpecified == 1) {
      for (i = 0; i < Length; i++) {
         if ((Address + i) == CompAddress || (CompAddress + i) == Address) {
            fprintf(fp, "T05:Compensation address and Serial address cannot match or overlap.\n");
            fclose(fp);
            return(0);
         } 
	 }
   }

   /*  The next serial number*/
   
   
	
     fprintf(fp, "T02:%05d\n", SerialNumber + (NumberOfNumbers * Increment));

	 printf("address: %d\n", &SerialNumber);
	 printf("Size: %d\n",sizeof(SerialNumber));
     sprintf (counterstr,"%05d",SerialNumber);



    time_t ltime;
	struct tm *today; 
	char formatstr[80]; 
	char paddingstr[80]; 
	//temp_number = SerialNumber;
	/////////////////////////////////////////////////////////////////
/* 	
Programmingdate str
*/

 #pragma region  
   if (ProgrammingDateStrSpecified)
	{
		time( &ltime );
		today = localtime( &ltime );
		

		
		strcpy (formatstr,ProgrammingDateStr ); 
		Replace (formatstr, "DD", "%d"); 
		Replace (formatstr, "MM", "%m"); 
		Replace (formatstr, "YY", "%y"); 
		strcpy (ProgrammingDateStr, formatstr); 
		strftime(ProgrammingDateStr, 80,formatstr, today );
		 
		
		if (0!= strcmp(ProgrammingDateStr,Dateread))
		{
			SerialNumber = 0;
		
		}
		  
	
		
		// sprintf (formatstr, "%-16s", TimeDateStr); 
		
		//String right padded with ASCII '0' 
		memset (paddingstr, '0', eLength); 
		memcpy (paddingstr, ProgrammingDateStr,strlen(ProgrammingDateStr));
		paddingstr[eLength] = '\0'; 
		strcpy (ProgrammingDateStr, paddingstr); 

			   /*Time stamp logging
		strcpy (formatstr, ProgrammingDateStr); 
		Replace (formatstr, "DD", "%d"); //actual prints day of the year change to %d for date
		Replace (formatstr, "MM", "%m"); 
		Replace (formatstr, "YY", "%y"); 
		strcpy(actualDateStr,formatstr);
		strftime( actualDateStr, 60,formatstr, today );
		fp= fopen("date.dat","w");
		fprintf(fp,"%s",actualDateStr);
		   fclose(fp);
           return(0);*/
	}
   temp_number =0;
   int len1 =0;

   for (i = 0; i < NumberOfNumbers; i++) 
   {
      GangSerialNumber[i] = temp_number;
      buffer_offset = ProgramDataBuffer + (i * len1);
     

          strncpy(buffer_offset,ProgrammingDateStr,strlen(ProgrammingDateStr));     
            
 
			
      
	  //sprintf(formatbuf, "%d", &Hellapartnumber);
      if (Order == ORDER_INTEL) {
         ptr1 = buffer_offset;
         ptr2 = buffer_offset + (Length - 1);
         for (j = 0; j < (Length / 2 + Length % 2); j++) {
            temp    = *ptr1;
            *ptr1++ = *ptr2;
            *ptr2-- = temp;
         }
      }
      if (SumcheckEnabled) {
         for (j = 0, ptr1 = buffer_offset; j < Length; j++)
            Sumcheck += *ptr1++;
      }
      //temp_number += Increment;
   } 
   #pragma endregion
/*
   time programming str
*/
#pragma region
	if (TimeDateStrSpecified)
	{
		time( &ltime );
		today = localtime( &ltime );


		strcpy (formatstr, TimeDateStr); 
		Replace (formatstr, "DD", "%j"); //actual prints day of the year change to %d for date
		Replace (formatstr, "MM", "%m"); 
		Replace (formatstr, "YY", "%y"); 
		Replace (formatstr, "hh", "%H"); 
		Replace (formatstr, "mm", "%M"); 
		Replace (formatstr, "ss", "%S"); 

		
		
		strftime( TimeDateStr, 80,formatstr, today );

		// sprintf (formatstr, "%-16s", TimeDateStr); 
		
		//String right padded with ASCII '0' 
		memset (paddingstr, '0', eLength); 
		memcpy (paddingstr, TimeDateStr,strlen(TimeDateStr));
		memcpy (paddingstr+strlen(TimeDateStr),counterstr,strlen(counterstr));
       //strcpy (SerialnumberStr, paddingstr);
		paddingstr[Length1] = '\0'; 
		strcpy (TimeDateStr, paddingstr); 
	}




   for (i = 0; i < NumberOfNumbers; i++) 
   {
      GangSerialNumber[i] = temp_number;
      buffer_offset = SerialNumBuffer + (i * Length);
      switch (Format) {
         case FORMAT_BINARY:
            ptr1 = buffer_offset + (Length - 1);
            for (j = 0; j < dLength; j++)
               *ptr1-- = (temp_number >> (8 * j)) & 0xFF;
            if (EthernetMaskSpecified) {
               for (j = 0; j < eLength; j++)
                  *ptr1-- = (mask_number >> (8 * j)) & 0xFF;
            }      
            break;
         case FORMAT_BCD:
            if (InputNumberFormat == INPUT_HEX)
               divisor = 16;
            else 
               divisor = 10;
            ptr1 = buffer_offset + (Length - 1);
            bcd_number = temp_number;
            for (j = 0; j < dLength; j++) {
               *ptr1 = (bcd_number % divisor) & 0xFF;
               bcd_number = bcd_number / divisor;
               ptr1--;
            }   
            bcd_number = mask_number;
            if (EthernetMaskSpecified) {
               for (j = 0; j < eLength; j++) {
                  *ptr1 = (bcd_number % divisor) & 0xFF;
                  bcd_number = bcd_number / divisor;
                  ptr1--;
               }   
            }      
            break;
         case FORMAT_DECIMAL:
            if (EthernetMaskSpecified) {
               sprintf(formatbuf, "%%0%dld%%0%dld", eLength, dLength);         
               sprintf(buffer_offset, formatbuf, mask_number, temp_number);
            }
            else {
               sprintf(formatbuf, "%%0%dld", dLength);         
               sprintf(buffer_offset, formatbuf, temp_number);
            }   
            break;
         case FORMAT_HEX:
            if (EthernetMaskSpecified) {
               sprintf(formatbuf, "%%0%dlX%%0%dlX", eLength, dLength);         
               sprintf(buffer_offset, formatbuf, mask_number, temp_number);
            }
            else if (TimeDateStrSpecified)
						{
								sprintf(formatbuf, "%%0%ds",  dLength);       	
								sprintf(buffer_offset, formatbuf, TimeDateStr, temp_number);
                                
								
			             }
						else
						{
               sprintf(formatbuf, "%%0%dlX", dLength);         
               sprintf(buffer_offset, formatbuf, temp_number);
            }   
            break;
			
      }
	  //sprintf(formatbuf, "%d", &Hellapartnumber);
      if (Order == ORDER_INTEL) {
         ptr1 = buffer_offset;
         ptr2 = buffer_offset + (Length - 1);
         for (j = 0; j < (Length / 2 + Length % 2); j++) {
            temp    = *ptr1;
            *ptr1++ = *ptr2;
            *ptr2-- = temp;
         }
      }
      if (SumcheckEnabled) {
         for (j = 0, ptr1 = buffer_offset; j < Length; j++)
            Sumcheck += *ptr1++;
      }
      temp_number += Increment;
   } 
#pragma endregion


    /*
    *   The Compenasation information
    */

#pragma region  

		memset (paddingstr, '0', strlen(TimeDateStr)); 
		memcpy (paddingstr,TimeDateStr,strlen(TimeDateStr));
		memcpy(paddingstr+strlen(TimeDateStr),Hellapartnumber,strlen(Hellapartnumber));
		memcpy(paddingstr+strlen(TimeDateStr)+strlen(Hellapartnumber),HellaSWVersion,strlen(HellaSWVersion));
	    paddingstr[Length3] = '\0'; 
		strcpy (Hellapartnumber, paddingstr); 
	
   int temp_numb =0;
   int len=0;

   for (i = 0; i < NumberOfNumbers; i++) 
   {
      GangSerialNumber[i] = temp_numb;
      buffer_offset = HellaITACBuffer + (i * len);
     

          strncpy(buffer_offset,Hellapartnumber,strlen(Hellapartnumber));     
            
 
			
      
	  //sprintf(formatbuf, "%d", &Hellapartnumber);
      if (Order == ORDER_INTEL) {
         ptr1 = buffer_offset;
         ptr2 = buffer_offset + (Length - 1);
         for (j = 0; j < (Length / 2 + Length % 2); j++) {
            temp    = *ptr1;
            *ptr1++ = *ptr2;
            *ptr2-- = temp;
         }
      }
      if (SumcheckEnabled) {
         for (j = 0, ptr1 = buffer_offset; j < Length; j++)
            Sumcheck += *ptr1++;
      }
      temp_number += Increment;
   } 
   #pragma endregion

   fp=fopen("serial.dat", "w");
    /*
    *   The current serial number
    */
   fprintf(fp, "T01:");
   for (i = 0; i < (NumberOfNumbers - 1); i++) {
      if (InputNumberFormat == INPUT_HEX)
         fprintf(fp, "0x%lX,", GangSerialNumber[i]);
      else
         fprintf(fp, "%05d,", GangSerialNumber[i]);
   }
   if (InputNumberFormat == INPUT_HEX)
      fprintf(fp, "0x%lX\n", GangSerialNumber[i]);
   else
      fprintf(fp, "%05d\n", GangSerialNumber[i]);


     /*
    *   Things TaskLink needs to know for gang serialization
    */
   if (NumberOfNumbers > 1) {
      if (Mode == MODE_PURESET)
         fprintf(fp, "T00\n");
      fprintf(fp, "T07:%lX\n", Address);
      fprintf(fp, "T08:%d\n", Length);
   }


    /*
    *   The sumcheck 
    */
   if (SumcheckEnabled && !CompAddressSpecified) 
      fprintf(fp, "T09:%08lX\n", Sumcheck);


    /*
    *   Is the current serial number within the warning limit.
    *   Any message used in the T11 record must be limited in length to 
    *   60 characters. The current code will only give the warning limit
    *   message one time when the limit is reached. 
    */
    if(EndingSerialNumber > 0 && WarningMsgRcvd == 0 && WarningLimit > 0) 
    {
       if((EndingSerialNumber - SerialNumber) <= (WarningLimit - 1)) {
          fprintf(fp, "T11:Ending Serial Number Warning Limit Reached\n");
       }   
    }    


	/*
    *   The I/O format number
    */
    fprintf(fp, "T03:87\n");



	   /*
    *   Lines 8-end of file: the data
    */
   fprintf(fp, "T04:\n");
#pragma region
   if (NumberOfNumbers > 1) {
      if (Mode == MODE_GANGSET) {
      /*
       *   For gang serialization in MODE_GANGSET, pile all the
       *   serial numbers in RAM right after the fixed device data.
       *   TaskLink will set the address at which the data is
       *   programmed into the device.
       */
        for (i = 0; i < NumberOfNumbers; i++) 
            SerialnumerFormat(fp, DeviceSizeInBytes + (i * Length),
                                 SerialNumBuffer + (i * Length), Length);
     }
     else {
     /*
      *    For gang serialization in MODE_PURESET, place each serial
      *    number one device size apart in programmer RAM.  
      */
        for (i = 0; i < NumberOfNumbers; i++)
           SerialnumerFormat(fp, Address + (i * DeviceSizeInBytes),
                                SerialNumBuffer + (i * Length), Length);
        if (CompAddressSpecified) {
           for (i = 0; i < NumberOfNumbers; i++)
              SerialnumerFormat(fp, CompAddress + (i * DeviceSizeInBytes),
                                   HellaITACBuffer + (i * Length), Length);
        }
        // fprintf(fp, "S9030000FC\n");
     }
   }
   else {
      /*
       *   For one-at-a-time serialization, just generate the serial
       *   number with the specified address.
       */
	        
//DateFormat(fp,Address,  actualDateStr, Length1);
      SerialnumerFormat(fp,Address, SerialNumBuffer, Length1);
	   programmingnumerFormat(fp, Address+0x00000010, ProgramDataBuffer, Length2);
      if (CompAddressSpecified) 
         HellaITACFormat(fp, CompAddress, HellaITACBuffer, Length3);
   }
   fprintf(fp, "S9030000FC\n");
  /*
    *   Close the output file and exit (return to TaskLink).
    */
   fclose(fp);

   

			   /*Time stamp logging*/


		fp= fopen("date.dat","w");
		fprintf(fp,"%05d\n",SerialNumber);
		fprintf(fp,"%s\n",ProgrammingDateStr);
		   fclose(fp);
           return(0);


}

/***************************************************************************
 *                                                                         *
 *   OutputMotorolaFormat()                                                *
 *                                                                         *
 *   Function for converting binary data from a buffer into                *
 *   Motorola format 87, which is written to a file.                       *
 *                                                                         *
 *   Parameters:                                                           *
 *                                                                         *
 *   fp       the file pointer corresponding to the opened file            *
 *            to which the Motorola format data is to be written           *
 *                                                                         *
 *   addr     an unsigned long which is the address that the               *
 *            first Motorola record will have                              *
 *                                                                         *
 *   buffer   a char pointer to the buffer containing the binary           *
 *            data to be formatted                                         *
 *                                                                         *
 *   nbytes   an unsigned int containing the number of bytes from          *
 *            the buffer to be formatted                                   *
 *                                                                         *
 *   This function does not supply an end record, so the main program      *
 *   must print one to the file after one or more calls here.              *
 *                                                                         *
 ***************************************************************************
 */
void SerialnumerFormat(FILE *fp, unsigned long addr, char *buffer, unsigned nbytes)
{
   unsigned i, j, bytecount, sumcheck;
   char c, *ptr = buffer;
   unsigned long localaddress = addr;

   for (i = 0; i < nbytes; i += 16) {
      bytecount = nbytes - i;
      if (bytecount > 16) bytecount = 13;
      fprintf(fp, "S3%02X%08lX", bytecount + 5, localaddress);
      sumcheck = bytecount + 5;
      for (j = 0; j < 5; j++)
         sumcheck += (localaddress >> (8 * j)) & 0xFF;
      for (j = 0; j < bytecount; j++) {
         fprintf(fp, "%02X", (int)(c= *ptr++));
         sumcheck += c;
      }
      fprintf(fp, "%02X\n", ~sumcheck & 0xFF);
      localaddress += bytecount;
   }
}

void programmingnumerFormat(FILE *fp, unsigned long addr, char *buffer, unsigned nbytes)
{
   unsigned i, j, bytecount, sumcheck;
   char c, *ptr = buffer;
   unsigned long localaddress = addr;

   for (i = 0; i < nbytes; i += 16) {
      bytecount = nbytes - i;
      if (bytecount > 16) bytecount = 13;
      fprintf(fp, "S3%02X%08lX", bytecount + 5, localaddress);
      sumcheck = bytecount + 5;
      for (j = 0; j < 5; j++)
         sumcheck += (localaddress >> (8 * j)) & 0xFF;
      for (j = 0; j < bytecount; j++) {
         fprintf(fp, "%c", (c= *ptr++));
         sumcheck += c;
      }
      fprintf(fp, "%02X\n", ~sumcheck & 0xFF);
      localaddress += bytecount;
   }
}
void HellaITACFormat(FILE *fp, unsigned long addr, char *buffer, unsigned nbytes)
{
   unsigned i, j, bytecount, sumcheck;
   char c, *ptr = buffer;
   unsigned long localaddress = addr;

   for (i = 0; i < nbytes; i += 16) {
      bytecount = nbytes - i;
      if (bytecount > 16) bytecount = 13;
      fprintf(fp, "S3%02X%08lX", bytecount + 5, localaddress);
      sumcheck = bytecount + 5;
      for (j = 0; j < 5; j++)
         sumcheck += (localaddress >> (8 * j)) & 0xFF;
      for (j = 0; j < bytecount; j++) {
         fprintf(fp, "%02X", (int)(c= *ptr++));
         sumcheck += c;
      }
      fprintf(fp, "%02X\n", ~sumcheck & 0xFF);
      localaddress += bytecount;
   }
}

  