

#include "EEP_ROM.h"

/*************************************************************
 * Doc 1 byte EEPROM
 * Tham so la dia chi can doc
 * Tra ve gia tri doc duoc
 *************************************************************/
unsigned char EEPROM_Read(int address_EEPROM)
{
	 unsigned char *x;
	 unsigned char data;
	 x = (unsigned char*)(address_EEPROM);
	 data=*x;
	 return data;
}

/*************************************************************
 * Doc nhieu byte
 * Tham so la dia chi byte dau va so luong byte can doc
 * tra ve gia tri doc duoc
 *************************************************************/

void EEPROM_ReadBurst(int address_EEPROM, unsigned char *buffer, unsigned char size)
{
	unsigned char i;
	unsigned char *x;
	x=(unsigned char*)(address_EEPROM);
	for (i = 0; i < size; i++)
	{
		buffer[i] =*x;
		x++;
	}
}

/*************************************************************
 * Ghi 1 byte EEPROM
 * Tham so la dia chi thanh ghi va gi tri can ghi
**************************************************************/
unsigned int EEPROM_Read_int( int address_EEPROM){
	unsigned int a,b,c;
	b=EEPROM_Read(address_EEPROM);
	c=EEPROM_Read(address_EEPROM+1);
	a=((b<<8)|c);
	return a;
}
void EEPROM_Write_int ( int address_EEPROM, unsigned int data){
	EEPROM_Write(address_EEPROM,(unsigned char)(data>>8));
	EEPROM_Write(address_EEPROM+1,(unsigned char)data);
	
}
void EEPROM_Write(int address_EEPROM, unsigned char data_EEPROM)
{
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x00;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = EEPROM_PROGRAM;// EEPROM  command
	NVM_FCCOBLO =0x00;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x01;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = address_EEPROM;
	// Write index to specify the byte0 (MSB word) to be programmed
	NVM_FCCOBIX = 0x02;
	// Write the byte 0
	NVM_FCCOB = data_EEPROM;
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
 // 	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
	while(!NVM_FSTAT_CCIF);
}

/**********************************************************
 * Ghi nhieu byte lien tiep
 * 
 **********************************************************/
void EEPROM_WriteBurst(int address_EEPROM, unsigned char *buffer, unsigned size)
{
	unsigned char i;

	for(i=0;i<size;i++)
	{
	  	NVM_FCLKDIV_FDIV=0x13;
		// Clear error flags
		NVM_FSTAT = 0x30;
		
		// Write index to specify the command code to be loaded
		NVM_FCCOBIX = 0x0;
		// Write command code and memory address bits[23:16]	
		NVM_FCCOBHI = EEPROM_PROGRAM;// EEPROM  command
		NVM_FCCOBLO =0x00;// memory address bits[23:16]
		// Write index to specify the lower byte memory address bits[15:0] to be loaded
		NVM_FCCOBIX = 0x01;
		// Write the lower byte memory address bits[15:0]
		NVM_FCCOB = address_EEPROM;
		// Write index to specify the byte0 (MSB word) to be programmed
		NVM_FCCOBIX = 0x2;
		// Write the byte 0
		NVM_FCCOB = buffer[i];
		// Launch the command
		NVM_FSTAT = 0x80;
		// Wait till command is completed
	// 	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
		while(!NVM_FSTAT_CCIF);
		address_EEPROM++;
	}
}
void EEPROM_EraseByteBurst(int address_EEPROM, unsigned int leng){
	unsigned char Start_mod=0, ModStart_value=0;
	unsigned char End_mod=0, ModEnd_value=0;
	unsigned char i=0;
	
	Start_mod=(unsigned char)(address_EEPROM % 2);
	if(Start_mod==1)
	{
		ModStart_value=EEPROM_Read(address_EEPROM-1);
	}
	End_mod = (unsigned char)((address_EEPROM + leng-1) % 2);
	if(End_mod==0){
		ModEnd_value=EEPROM_Read(address_EEPROM + leng);
	}
	
	for(i=0;i<(Start_mod + (leng/2));i++){
		
		NVM_FCLKDIV_FDIV=0x13;
		// Clear error flags
		NVM_FSTAT = 0x30;
		
		// Write index to specify the command code to be loaded
		NVM_FCCOBIX = 0x0;
		// Write command code and memory address bits[23:16]	
		NVM_FCCOBHI = EEPROM_ERASE_SECTOR ;// erase EEPROM block command
		NVM_FCCOBLO = 0x00;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
	  
		// Write index to specify the lower byte memory address bits[15:0] to be loaded
		NVM_FCCOBIX = 0x1;
		// Write the lower byte memory address bits[15:0]
		NVM_FCCOB = (address_EEPROM+2*i);//0x3100//12544
		
		// Launch the command
		NVM_FSTAT = 0x80;
		// Wait till command is completed
	  // 	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
		while(!NVM_FSTAT_CCIF);
		
	}
	if(Start_mod==1)
	{
		EEPROM_Write((address_EEPROM-1) , ModStart_value);
	}

	if(End_mod==0)
	{
		EEPROM_Write((address_EEPROM + leng) , ModEnd_value);
	}
}

/**********************************************************
 * Xoa 1 byte EEPROM
 * Tham so truyen vao la dia chi thanh ghi
 * 
 ***********************************************************/
void EEPROM_EraseByte(int address_EEPROM)
{
	unsigned char value, sodu;
	sodu=(unsigned char)(address_EEPROM % 2);
	if(sodu==0)
		value=EEPROM_Read(address_EEPROM+1);
	else
		value=EEPROM_Read(address_EEPROM-1);
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = EEPROM_ERASE_SECTOR ;// erase EEPROM block command
	NVM_FCCOBLO = 0x00;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
  
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = address_EEPROM;//0x3100//12544
	
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
   	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
//	while(!NVM_FSTAT_CCIF);
	if(sodu==0)
		EEPROM_Write(address_EEPROM+1,value);
	else
		EEPROM_Write(address_EEPROM-1,value);
}
void EEPROM_EraseBlock(int address_EEPROM){
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = EEPROM_ERASE_SECTOR ;// erase EEPROM block command
	NVM_FCCOBLO = 0x00;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
  
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = address_EEPROM;//0x3100//12544
	
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
  // 	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
	while(!NVM_FSTAT_CCIF);
}
/**********************************************************
 * Xoa nhieu Byte lien tiep
 * Dia chi dau va so luong byte can xoa
 **********************************************************/
void EEPROM_EraseVerifyByteBurst(int address_EEPROM, unsigned char size)
{
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]
	NVM_FCCOBHI = EEPROM_ERASE_VERIFY_SECTION;// erase verify FLASH section command
	NVM_FCCOBLO = 31;
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = address_EEPROM;
	// Write index to specify the # of longwords to be verified
	NVM_FCCOBIX = 0x02;
	// Write the # of longwords 
	NVM_FCCOB = size;
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
}
/**********************************************************
 * Xoa toan bo EEPROM
 *  
 **********************************************************/
void EEPROM_EraseAll(int address_EEPROM)
{
	unsigned char add;
	add=(unsigned char) address_EEPROM;
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = ERASE_BLOCK;// erase FLASH block command
	NVM_FCCOBLO = 0x80;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x01;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = add;
	
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
//	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
	while(!NVM_FSTAT_CCIF);
}

/**********************************************************
 * Kiem tra EEPROM da format chua
 * 
 **********************************************************/
/**********************************************************
 * Doc 1 byte FLASH
 * Tham so truyen vao la dia chi thanh ghi
 * Tra ve gi tri doc duoc
 **********************************************************/
unsigned char FLASH_Read(int address_FLASH)
{
	 int *x;							// con tro X
	 x = (int*)(address_FLASH); 		// dia chi x gan = dia chi cua add
	 return (unsigned char)(*x);		// tra ve gia tri cua dia chi x chinh bang gia tri cua add
}
/*********************************************************
 * Doc nhieu byte FLASH
 * Tham so vao dia chi Byte dau, size
 *********************************************************/
void FLASH_ReadBurst(int address_FLASH, unsigned char *buffer, unsigned char size)
{
	unsigned char i;
	int *x;
	x=(int*)(address_FLASH);
	for (i = 0; i < size; i++)
	{
		buffer[i] =(unsigned char) (*x);
		x++;
	}
}

/**********************************************************
 * Ghi 1 byte FLASH
 * Tham so truyen vao la dia chi thanh ghi va gi tri can ghi
 **********************************************************/
void FLASH_Write(int address_FLASH, unsigned char data_FLASH)
{
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x00;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = FLASH_PROGRAM;// EEPROM  command
	NVM_FCCOBLO =0x00;// memory address bits[23:16]
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x01;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = address_FLASH;
	// Write index to specify the byte0 (MSB word) to be programmed
	NVM_FCCOBIX = 0x02;
	// Write the byte 0
	NVM_FCCOB = data_FLASH;
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
 // 	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
	while(!NVM_FSTAT_CCIF);
}
/**********************************************************
 * Xoa 1 byte FLASH
 **********************************************************/
void FLASH_EraseByte(int address_FLASH)
{
	unsigned char value, sodu;
	sodu=(unsigned char)(address_FLASH % 2);
	if(sodu==0)
		value=FLASH_Read(address_FLASH+1);
	else
		value=FLASH_Read(address_FLASH-1);
	NVM_FCLKDIV_FDIV=0x13;
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = FLASH_ERASE_SECTOR ;// erase EEPROM block command
	NVM_FCCOBLO = 0x00;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
  
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x1;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = address_FLASH;//0x3100//12544
	
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
  // 	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
	while(!NVM_FSTAT_CCIF);
	if(sodu==0)
		FLASH_Write(address_FLASH+1,value);
	else
		FLASH_Write(address_FLASH-1,value);
}

/**********************************************************
 * xoa FLASH
 **********************************************************/
void FLASH_EraseALL(void)
{
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = ERASE_BLOCK;// erase FLASH block command
	NVM_FCCOBLO = 0x00;// memory address bits[23:16] with bit23 = 0 for Flash block, 1 for EEPROM block
	// Write index to specify the lower byte memory address bits[15:0] to be loaded
	NVM_FCCOBIX = 0x01;
	// Write the lower byte memory address bits[15:0]
	NVM_FCCOB = 0x8000;
	
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
}
/**********************************************************
 * Xoa ca 2 vung nho FLASH va EEPROM
 * 
 **********************************************************/
void EraseAll(void)
{
	
	NVM_FCLKDIV_FDIV=0x13;
//	while(!NVM_FSTAT_CCIF);
	// Clear error flags
	NVM_FSTAT = 0x30;
	
	// Write index to specify the command code to be loaded
	NVM_FCCOBIX = 0x0;
	// Write command code and memory address bits[23:16]	
	NVM_FCCOBHI = ERASE_ALL;// erase all flash & EEPROM blocks
	
	// Launch the command
	NVM_FSTAT = 0x80;
	// Wait till command is completed
	while (!(NVM_FSTAT & NVM_FSTAT_CCIF_MASK));
}
