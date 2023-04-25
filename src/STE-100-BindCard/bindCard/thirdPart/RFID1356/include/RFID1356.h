#ifndef LIB_H
#define LIB_H
//--------------------------------------------------------------------------------------------------------
#define OK				0x00
#define FAIL			0x01
#define WRITECOMMFAIL	0x02
#define READCOMMFAIL	0x03
#define READTIMEOUT		0x04
#define FORMATWRONG		0x05 
//--------------------------------------------------------------------------------------------------------
#define MF3_CREDIT					0x0C
#define MF3_DEBIT					0xDC
#define MF3_LIMITEDCREDIT			0x1C

#define MF3_COMMITTRANS				0xC7
#define MF3_ABORTTRANS				0xA7
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall LRC(unsigned short int len,char *buff);
extern "C" __declspec(dllexport) int __stdcall openpcsc(unsigned char namelen,unsigned char *name);
extern "C" __declspec(dllexport) int __stdcall closepcsc(void);
extern "C" __declspec(dllexport) int __stdcall pcsccommand(unsigned char *apdu, unsigned int iapduLen, unsigned char *rpdu, unsigned int *irpduLen);
extern "C" __declspec(dllexport) int __stdcall pcscrawcommand(unsigned char *apdu, unsigned int iapduLen, unsigned char *rpdu, unsigned int *irpduLen);
extern "C" __declspec(dllexport) int __stdcall listhid(unsigned short vendor_id, unsigned short product_id);
extern "C" __declspec(dllexport) int __stdcall openhid(unsigned short vendor_id, unsigned short product_id, unsigned char hid_number);
extern "C" __declspec(dllexport) int __stdcall closehid(void);
extern "C" __declspec(dllexport) int __stdcall openCom(int nCom,long baundrate);
extern "C" __declspec(dllexport) int __stdcall closeCom(int nCom);
extern "C" __declspec(dllexport) int __stdcall baudCom(int nCom, long baudRate);
extern "C" __declspec(dllexport) int __stdcall timeoutCom(int time);
extern "C" __declspec(dllexport) int __stdcall QueueUartData(unsigned char *relen,unsigned char *redata);
extern "C" __declspec(dllexport) int __stdcall ReaderTxRx(unsigned short senlen,unsigned char *sendata,unsigned short *relen,unsigned char *redata);
extern "C" __declspec(dllexport) int __stdcall ReaderTxRxGet(unsigned short *senlen,unsigned char *sendata,unsigned short *relen,unsigned char *redata);
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall SetBuzzer(unsigned char ucRates,unsigned char ucTimes) ;
extern "C" __declspec(dllexport) int __stdcall SetLed(unsigned char ucRates,unsigned char ucTimes) ;
extern "C" __declspec(dllexport) int __stdcall SetUARTBaudRate(unsigned char ucRates) ;
extern "C" __declspec(dllexport) int __stdcall GetSoftwareVS(unsigned char *relen,unsigned char *reVS);
extern "C" __declspec(dllexport) int __stdcall GetReaderUID(unsigned char *relen,unsigned char *reuid) ;
extern "C" __declspec(dllexport) int __stdcall SetANT(unsigned char ucAnt);
extern "C" __declspec(dllexport) int __stdcall GetANT(unsigned char *ucAnt);
extern "C" __declspec(dllexport) int __stdcall SetHalt(void);
extern "C" __declspec(dllexport) int __stdcall RegistUID(unsigned char *uidslen,unsigned char *uids);
extern "C" __declspec(dllexport) int __stdcall RegistCMD(unsigned char *commandlen,unsigned char *command);
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall DownLoadCheck(unsigned char ucCore,unsigned char *infor);
extern "C" __declspec(dllexport) int __stdcall DownLoadInit(unsigned char ucCore,unsigned short usPack,unsigned int uiFileLen) ;
extern "C" __declspec(dllexport) int __stdcall DownLoadPack(unsigned char ucCore,unsigned short usPack,unsigned short usPackLen,unsigned char *ucpPackData) ;
extern "C" __declspec(dllexport) int __stdcall DownLoadFinish(unsigned char ucCore,unsigned short usCheckLen,unsigned char *ucpCheck) ;
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall PCD_Read_Flash(unsigned int adder,unsigned short length,unsigned char *outdata); 
extern "C" __declspec(dllexport) int __stdcall PCD_Write_Flash(unsigned int adder,unsigned short length,unsigned char *indata);
extern "C" __declspec(dllexport) int __stdcall PCD_Write_Key(unsigned char keyNo,unsigned char *keydata);
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall PiccSysCardMake(unsigned char len,unsigned char *pBuf) ;
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall PiccAuthKey(	unsigned char auth_mode,
													unsigned char addr,
													unsigned char *pSnr,
													unsigned char *pKey) ;
extern "C" __declspec(dllexport) int __stdcall PiccAuthKeyE2(	unsigned char auth_mode,unsigned char addr) ;
extern "C" __declspec(dllexport) int __stdcall PiccPersonalize(unsigned char ucType);
extern "C" __declspec(dllexport) int __stdcall PiccPersonalizeUIDInit(void);
extern "C" __declspec(dllexport) int __stdcall PiccPersonalizeUIDWrite(unsigned char *pblock0data);
extern "C" __declspec(dllexport) int __stdcall PiccRead(unsigned char ucBlock, unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccReadMux(unsigned char ucBlock,unsigned char ucBlockMux, unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccWrite(unsigned char ucBlock,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccITGRead(	unsigned char rst_ms,
													unsigned char req_mode,
													unsigned char sector_begin,
													unsigned char sector_much,
													unsigned char block_each,
													unsigned char auth_mode,
													unsigned char *pKey,
													unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccITGWrite(	unsigned char rst_ms,
													unsigned char req_mode,
													unsigned char sector_begin,
													unsigned char sector_much,
													unsigned char block_each,
													unsigned char auth_mode,
													unsigned char *pKey,
													unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccULWrite(unsigned char ucBlock,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccULWriteMux(unsigned char ucBlock,unsigned char ucBlockMux,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccInitVL(unsigned char ucBlock,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccValueOper(	unsigned char ucOperMode, 
													unsigned char ucBlock,
													unsigned char *pValue, 
													unsigned char ucTransBlock);
extern "C" __declspec(dllexport) int __stdcall PiccBackup(	unsigned char ucBlock,
													unsigned char ucTransBlock);
extern "C" __declspec(dllexport) int __stdcall PiccReadValue(unsigned char ucBlock, unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall PiccReset(unsigned char _1ms);
extern "C" __declspec(dllexport) int __stdcall PiccActivateA(	unsigned char ucMode, 
													unsigned char ucReqCode, 
													unsigned char *pATQ, 
													unsigned char *pSAK,
													unsigned char *pUIDLen,       
													unsigned char *pUID     );
extern "C" __declspec(dllexport) int __stdcall PiccAutoListCard( unsigned char *ucLen,unsigned char *pSetting);
extern "C" __declspec(dllexport) int __stdcall PiccActivateB(	unsigned char ucMode, 
												    unsigned char ucCID, 
													unsigned char ucAFI, 
													unsigned char ucMethod,
													unsigned char *pUIDLen,
													unsigned char *pUID);
extern "C" __declspec(dllexport) int __stdcall PiccActivateCID(unsigned char ucRst, unsigned char *pINF);
extern "C" __declspec(dllexport) int __stdcall PiccActivateCID2(unsigned char ucRst, unsigned char *pINF);
extern "C" __declspec(dllexport) int __stdcall PiccActivateCID_Test74(unsigned char ucRst,
															unsigned short *pINFlen,
															unsigned char *pINF);
extern "C" __declspec(dllexport) int __stdcall PiccActivateC(	unsigned char ucMode, 
													unsigned char ucMethod,
													unsigned char *pINFLen,
													unsigned char *pINF);
extern "C" __declspec(dllexport) int __stdcall PiccRequestATS(unsigned char ucCID,
													unsigned char *pATSLen, 
													unsigned char *pATS);
extern "C" __declspec(dllexport) int __stdcall PiccPPS(unsigned char Dri,
													unsigned char Dsi);
extern "C" __declspec(dllexport) int __stdcall PiccTPCL(	unsigned int usSendLen,
												unsigned char *pSendBuf, 		
												unsigned int *pRcvLen,
												unsigned char *pRcvBuf);
extern "C" __declspec(dllexport) int __stdcall PiccTANSf(	unsigned int usSendLen,
												unsigned char *pSendBuf, 		
												unsigned int *pRcvLen,
												unsigned char *pRcvBuf);
extern "C" __declspec(dllexport) int __stdcall PiccAutoRATS(unsigned char rats);
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall PiccULAuth(unsigned char *pKey);
extern "C" __declspec(dllexport) int __stdcall PiccULSetKey(unsigned char *pKey);
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall DESGetDESVersion(unsigned char *relen,unsigned char *rebuf);
//
extern "C" __declspec(dllexport) int __stdcall DESAuthenticate(unsigned char KeyNO,unsigned char *pKey);
extern "C" __declspec(dllexport) int __stdcall DESAuthenticateE2(unsigned char KeyNoOfDesfire,unsigned char KeyNoInE2);
extern "C" __declspec(dllexport) int __stdcall DESChangKey(unsigned char KeyNo,unsigned char KeySettings,unsigned char *pNewKey,unsigned char *pOldKey);
extern "C" __declspec(dllexport) int __stdcall DESChangKeyE2(unsigned char KeyNo,unsigned char KeySettings,unsigned char pNewKeyNo,unsigned char pOldKeyNo);
extern "C" __declspec(dllexport) int __stdcall DESGetKeyVersion(unsigned char KeyNO,unsigned char *pKeyVersion);
extern "C" __declspec(dllexport) int __stdcall DESGetKeySetting(unsigned char *pKeySetting,unsigned char *pMaxKeyNum);
extern "C" __declspec(dllexport) int __stdcall DESChangKeySetting(unsigned char KeySetting);
//
extern "C" __declspec(dllexport) int __stdcall DESSelectApplication(long AID);
extern "C" __declspec(dllexport) int __stdcall DESGetApplicationIDs(unsigned char *pAIDs,unsigned char *pAIDno);
extern "C" __declspec(dllexport) int __stdcall DESDeleteApplication(unsigned long AID);
extern "C" __declspec(dllexport) int __stdcall DESFormatPicc(void);
extern "C" __declspec(dllexport) int __stdcall DESCreateApplication(unsigned long AID,unsigned char KeySetting,unsigned char KeyNo);
//
extern "C" __declspec(dllexport) int __stdcall DESGetFileIDs(unsigned char *pIDs,unsigned char *pFileIDs);
extern "C" __declspec(dllexport) int __stdcall DESCreateStdDataFile( unsigned char FileID,unsigned char ComSet,unsigned short AccessRights,unsigned short FileSize);
extern "C" __declspec(dllexport) int __stdcall DESCreateBackupDataFile( unsigned char FileID,unsigned char ComSet,unsigned short AccessRights,unsigned short FileSize);
extern "C" __declspec(dllexport) int __stdcall DESCreateValueFile( unsigned char FileID,unsigned char ComSet,unsigned short AccessRights,long LowerLimit,long UpperLimit,long Value,unsigned char LimitCredit);
extern "C" __declspec(dllexport) int __stdcall DESCreateLinearRecordFile( unsigned char FileID,unsigned char ComSet,unsigned short AccessRights,unsigned short FileSize,unsigned short RecordsNum);
extern "C" __declspec(dllexport) int __stdcall DESCreateCyclicRecordFile( unsigned char FileID,unsigned char ComSet,unsigned short AccessRights,unsigned short FileSize,unsigned short RecordsNum);
extern "C" __declspec(dllexport) int __stdcall DESDeleteDESFile(unsigned char FileID);
extern "C" __declspec(dllexport) int __stdcall DESGetFileSettings(unsigned char FileID,unsigned char *pInfoLen,unsigned char *pFileInfo);
extern "C" __declspec(dllexport) int __stdcall DESChangeFileSettings(unsigned char FileID,unsigned char NewComSet,unsigned short NewAccessRights);
//
extern "C" __declspec(dllexport) int __stdcall DESWriteData(unsigned char FileID,unsigned short Offset,unsigned short Length,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall DESReadData(unsigned char FileID,unsigned short Offset,unsigned short Length,unsigned char *pBuf,unsigned short *RcvLen);
extern "C" __declspec(dllexport) int __stdcall DESGetValue(unsigned char FileID,long *Value);
extern "C" __declspec(dllexport) int __stdcall DESOperateValue(unsigned char FileID,unsigned char ValueCommand,long Value);
extern "C" __declspec(dllexport) int __stdcall DESWriteRecord(unsigned char FileID,unsigned short Offset,unsigned short Length,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall DESReadRecord(unsigned char FileID,unsigned short RecordNo,unsigned short RecordNum,unsigned char *pBuf,unsigned short *RcvLen);
//EV1
extern "C" __declspec(dllexport) int __stdcall DESEV1WriteData(unsigned char FileID,unsigned char ComSet,unsigned short Offset,unsigned short Length,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall DESEV1ReadData(unsigned char FileID,unsigned char ComSet,unsigned short Offset,unsigned short Length,unsigned char *pBuf,unsigned short *RcvLen);
extern "C" __declspec(dllexport) int __stdcall DESEV1GetValue(unsigned char FileID,unsigned char ComSet,long *Value);
extern "C" __declspec(dllexport) int __stdcall DESEV1OperateValue(unsigned char FileID,unsigned char ComSet,unsigned char ValueCommand,long Value);
extern "C" __declspec(dllexport) int __stdcall DESEV1WriteRecord(unsigned char FileID,unsigned char ComSet,unsigned short Offset,unsigned short Length,unsigned char *pBuf);
extern "C" __declspec(dllexport) int __stdcall DESEV1ReadRecord(unsigned char FileID,unsigned char ComSet,unsigned short RecordNo,unsigned short RecordNum,unsigned char *pBuf,unsigned short *RcvLen);

extern "C" __declspec(dllexport) int __stdcall DESClearRecordFile(unsigned char FileID);
extern "C" __declspec(dllexport) int __stdcall DESTransaction(unsigned char Command);
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------Mifare Plus
//--------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------
//blockNr：祥见卡片资料,或如下
//card master key 0x9000
//card configuration key 0x9001
//level2 switch key 0x9002
//level3 switch key 0x9003
//SL1 card authentication key 0x9004
//pValue：16字节密码或数据
extern "C" __declspec(dllexport) int __stdcall ISO14443A_Mfp_WritePerso(unsigned short blockNr, 
																		unsigned char *pValue);

//--------------------------------------------------------------------------------------------------------
//确认ISO14443A_Mfp_WritePerso的操作并初始化卡片，如果不做此步骤，ISO14443A_Mfp_WritePerso无效。
extern "C" __declspec(dllexport) int __stdcall ISO14443A_Mfp_CommitPerso(void);

//--------------------------------------------------------------------------------------------------------
//level：mifare plus在哪个级别做认证操作
//firstAuth：初始认证为1，否则为0
//blockNr：2字节密码块号码
//pKx：16字节密码
//lenPcdCap2： 一般为6，可以为0
//PcdCap2：lenPcdCap为6时请提供6字节PcdCap，可以为6字节0x00，lenPcdCap为0时可以为NULL
//PdCap2：卡片返回的6字节PcdCap
extern "C" __declspec(dllexport) int __stdcall ISO14443A_Mfp_Authenticate(	unsigned char level,
																			unsigned char firstAuth,
																			unsigned short blockNr,
																			unsigned char *pKx,
																			unsigned char lenPcdCap2,
																			unsigned char *PcdCap2,
																			unsigned char *PdCap2);

//--------------------------------------------------------------------------------------------------------
//encrypted：mifare plus s卡请填0不使用密文通信，mifare plus x可以为1或0
//readMaced：读数据返回是否带Mac，可默认为1
//macOnCmd：是否命令中加入Mac，可默认为1
//blockNr：2字节块号码，比如0x0001
//numBlocks： 一次读多少块，默认不超过15个块（240字节），注意密码不一样的块不能一次读出
//pBlocks：返回的数据,长度为（numBlocks*16）
extern "C" __declspec(dllexport) int __stdcall ISO14443A_Mfp_Read(	unsigned char encrypted,
																	unsigned char readMaced,
																	unsigned char macOnCmd,
																	unsigned short blockNr,
																	unsigned char numBlocks,
																	unsigned char *pBlocks);

//--------------------------------------------------------------------------------------------------------
//encrypted：mifare plus s卡请填0不使用密文通信，mifare plus x可以为1或0
//readMaced：写数据返回是否带Mac，可默认为1
//blockNr：2字节块号码，比如0x0001
//numBlocks： 一次写多少块，默认不超过3个块（48字节），即同一个扇区数据
//pBlocks：写入的数据
extern "C" __declspec(dllexport) int __stdcall ISO14443A_Mfp_Write(	unsigned char encrypted,
																	unsigned char writeMaced,
																	unsigned short blockNr,
																	unsigned char numBlocks,
																	unsigned char *pBlocks);

//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall IccPowerUp(	unsigned char usCardSlot,
													unsigned char *pRecLen,
													unsigned char *pRcvBuf);
extern "C" __declspec(dllexport) int __stdcall IccPowerDn(	unsigned char usCardSlot );
extern "C" __declspec(dllexport) int __stdcall IccAPDU (	unsigned char usCardSlot,
												unsigned int usSendLen,
												unsigned char *pSendBuf, 		
												unsigned int *pRcvLen,
												unsigned char *pRcvBuf);
extern "C" __declspec(dllexport) int __stdcall IccChannal(	unsigned char usCardSlot,
													unsigned int usSendLen,
													unsigned char *pSendBuf,
													unsigned int *pRcvLen,
													unsigned char *pRcvBuf);
extern "C" __declspec(dllexport) int __stdcall IccCheck_Pres (unsigned char usCardSlot, unsigned char *usStatus );
extern "C" __declspec(dllexport) int __stdcall IccSetInitBaudrate (unsigned char usCardSlot,unsigned char ucRates );
extern "C" __declspec(dllexport) int __stdcall IccSetguardtime (unsigned char usCardSlot,unsigned char guardtime );
extern "C" __declspec(dllexport) int __stdcall IccSetSW61xx (unsigned char usCardSlot,unsigned char enable );
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall ISO15693_Inventory(unsigned char flags,
														unsigned char AFI, 
														unsigned char masklengh, 
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Select(unsigned char flags,
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Stay_Quiet(unsigned char flags,
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Read_Block(unsigned char flags,
														unsigned char blnr, 
														unsigned char nbl, 
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Write_Block(unsigned char flags,
														unsigned char blnr, 
														unsigned char nbl, 
														unsigned char *uid, 
														unsigned char *dtw, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Write_Block_Not4Bblock(unsigned char flags,
														unsigned char blnr, 
														unsigned char nbl, 
														unsigned char *uid, 
														unsigned char *dtw, 
														unsigned char dtwlen, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Lock_Block(unsigned char flags,
														unsigned char blnr, 
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Write_AFI(unsigned char flags,
														unsigned char AFI, 
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Lock_AFI(unsigned char flags,
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Write_DSFID(unsigned char flags,
														unsigned char DSFID, 
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Lock_DSFID(unsigned char flags,
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);
extern "C" __declspec(dllexport) int __stdcall ISO15693_Get_SysInfor(unsigned char flags,
														unsigned char *uid, 
														unsigned short *resplen, 
														unsigned char *resp);

//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall ISO18000P3M3_Activate(	unsigned char *rebInforLen,//12bytes
																		unsigned char *rebInfor,//12bytes
																		unsigned char *pbM,/**< Modulation; default #PHPAL_I18000P3M3_M_MANCHESTER_4 */
																		unsigned char *pbDr);/**< Link frequency; default #PHPAL_I18000P3M3_LF_847KHZ */
extern "C" __declspec(dllexport) int __stdcall ISO18000P3M3_Ack(		unsigned char *rebInforLen,/**<Tag response length */
																	unsigned char *rebInfor);/**< Pointer to Tag data and, if required, PacketCRC. */
extern "C" __declspec(dllexport) int __stdcall ISO18000p3m3_Read(	unsigned char bMemBank,        /**< [In] Memory bank where the read shall be performed. */
																	unsigned char * pWordPtr,      /**< [In] Starting read address. */
																	unsigned char bWordPtrLength,  /**< [In] Length of the pointer in bytes; 0 -> 1byte,1->2bytes,2->3bytes or 3->4bytes. */
																	unsigned char bWordCount,      /**< [In] Number of bytes to read. */
																	unsigned char * pRxBuffer,   /**< [Out] Header and requested memory words. */
																	unsigned short * pRxLength  /**< [Out] Number of received bits. */
																	);
extern "C" __declspec(dllexport) int __stdcall ISO18000p3m3_Write(	unsigned char bOption,        /**< [In] Option parameter. */
																	unsigned char bMemBank,       /**< [In] Memory bank where the write shall be performed. */
																	unsigned char * pWordPtr,     /**< [In] Starting write address. */
																	unsigned char bWordPtrLength, /**< [In] [In] Length of the pointer in bytes; 0 -> 1byte,1->2bytes,2->3bytes or 3->4bytes. */
																	unsigned char * pData         /**< [In] Word to write; uint8_t[2]. */
																	);
extern "C" __declspec(dllexport) int __stdcall ISO18000p3m3_Kill(	unsigned char bOption,     /**< [In] Option parameter. */
																	unsigned char * pPassword, /**< [In] Full kill password; uint8_t[4] */
																	unsigned char bRecom       /**< [In] Recommissioning bits. */
																	);
extern "C" __declspec(dllexport) int __stdcall ISO18000p3m3_Lock(	unsigned char * pMask,     /**< [In] 10bit Action Field Mask; uint8_t[2]. */
																	unsigned char * pAction    /**< [In] 10bit Action Field; uint8_t[2]. */
																	);
extern "C" __declspec(dllexport) int __stdcall ISO18000p3m3_Acess(	unsigned char bOption,   /**< [In] Option parameter. */
																	unsigned char * pPassword  /**< [In] Full access password; uint8_t[4] */
																	);
//--------------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) int __stdcall FM1208_Reader_SetKey(unsigned char keyLen,unsigned char *pNewKey);
extern "C" __declspec(dllexport) int __stdcall FM1208_Init(void);
extern "C" __declspec(dllexport) int __stdcall FM1208_Erase(void);
extern "C" __declspec(dllexport) int __stdcall FM1208_CreateEF(unsigned char fileno,unsigned short filelen,unsigned char readkey,unsigned char writekey);
extern "C" __declspec(dllexport) int __stdcall FM1208_ReadInfo(unsigned char fileno,unsigned char readkey,unsigned short filevector,unsigned short filelen,unsigned char *redata);
extern "C" __declspec(dllexport) int __stdcall FM1208_WriteInfo(unsigned char fileno,unsigned char writekey,unsigned short filevector,unsigned short filelen,unsigned char *datain);
//--------------------------------------------------------------------------------------------------------
/** \name phalTop Custom Error Codes
*/
/** @{ */
#define PHAL_TOP_ERR_READONLY_TAG                  0x81U     /**< Tag is Read Only */
#define PHAL_TOP_ERR_INVALID_STATE                 0x82U     /**< Tag state is invalid */
#define PHAL_TOP_ERR_FORMATTED_TAG                 0x83U     /**< Tag already in NDEF formatted state. */
#define PHAL_TOP_ERR_UNSUPPORTED_VERSION           0x84U     /**< Unsupported NDEF version. */
#define PHAL_TOP_ERR_MISCONFIGURED_TAG             0x85U     /**< Tag not configured as per NDEF specification. */
#define PHAL_TOP_ERR_UNSUPPORTED_TAG               0x86U     /**< Tag with unsupported structure/format. */
#define PHAL_TOP_ERR_EMPTY_NDEF                    0x87U     /**< NDEF message length is zero (i.e. Tag in initialized state). */
#define PHAL_TOP_ERR_NON_NDEF_TAG                  0x88U     /**< Tag is not NDEF formatted. */

/**
* \name Configuration types
* \brief Use these Macro in \ref phalTop_SetConfig to configure Tags.
*/
/*@{*/
#define PHAL_TOP_CONFIG_TAG_TYPE                    0x51U                /**< Set/Get Tag type. Should be configured before calling CheckNdef. */
#define PHAL_TOP_CONFIG_TAG_STATE                   0x52U                /**< Get tag state. This shall be also used to set tag to read-only state. */
#define PHAL_TOP_CONFIG_NDEF_LENGTH                 0x53U                /**< Get current NDEF message Length. */
#define PHAL_TOP_CONFIG_MAX_NDEF_LENGTH             0x54U                /**< Get Max support NDEF Length by tag. */
#define PHAL_TOP_CONFIG_NDEF_VERSION                0x55U                /**< Get NDEF Version Number. */

#define PHAL_TOP_CONFIG_T1T_TMS                     0x03U                /**< Set tag memory size. Set before format operation. */
#define PHAL_TOP_CONFIG_T1T_TERMINATOR_TLV          0x05U                /**< Set Terminator TLV presence. Set before format/write operation to enable writing terminator TLV at end of NDEF TLV. */

#define PHAL_TOP_CONFIG_T2T_TMS                     0x09U                /**< Set tag memory size. Set before format operation. */

#define PHAL_TOP_CONFIG_T4T_NDEF_FILE_ID            0x15U                /**< Set NDEF file ID. Set before format operation. */
#define PHAL_TOP_CONFIG_T4T_NDEF_FILE_SIZE          0x18U                /**< Set Max NDEF length. Set before format operation. */
#define PHAL_TOP_CONFIG_T4T_MLE                     0x19U                /**< Set MLe. Set before format operation. */
#define PHAL_TOP_CONFIG_T4T_MLC                     0x1AU                /**< Set MLc. Set before format operation. */

#define PHAL_TOP_CONFIG_T5T_MLEN                    0x1DU                /**< Set T5T NDEF data area. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_MBREAD                  0x20U                /**< Set T5T multiple block read support. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_LOCKBLOCK               0x21U                /**< Set T5T Lock block command support. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_SPL_FRM                 0x22U                /**< Set T5T special frame support. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_OPTION_FLAG             0x26U                /**< Set T5T option Flag. Set before format operation. */
#define PHAL_TOP_CONFIG_T5T_TERMINATOR_TLV          0x1FU                /**< Set Terminator TLV presence. Set before format/write operation to enable writing terminator TLV at end of NDEF TLV. */

#define PHAL_TOP_CONFIG_MFCTOP_CARD_TYPE            0x60U                /**< Set the Card Type */
/*@}*/

/** @} */
/**
* \name Mifare Classic Card Types
*/
/*@{*/
#define PHAL_TOP_NO_MFC                         0x00U       /**< No Card is selected */
#define PHAL_TOP_MFC_1K                         0x01U       /**< MFC 1K is selected*/
#define PHAL_TOP_MFC_4K                         0x02U       /**< MFC 4K is selected*/
#define PHAL_TOP_MFP_2K                         0x03U       /**< MFP 2K is selected*/
#define PHAL_TOP_MFP_4K                         0x04U       /**< MFP 4K is selected*/

/**
* \name Tag types
* \brief Tag types are used to set Tag used using \ref phalTop_SetConfig with \ref PHAL_TOP_CONFIG_TAG_TYPE Type
*/
/*@{*/
#define PHAL_TOP_TAG_TYPE_T1T_TAG                   0x01U                /**< Type 1 Tag. */
#define PHAL_TOP_TAG_TYPE_T2T_TAG                   0x02U                /**< Type 2 Tag. */
#define PHAL_TOP_TAG_TYPE_T3T_TAG                   0x03U                /**< Type 3 Tag. */
#define PHAL_TOP_TAG_TYPE_T4T_TAG                   0x04U                /**< Type 4 Tag. */
#define PHAL_TOP_TAG_TYPE_T5T_TAG                   0x05U                /**< Type 5 Tag. */
#define PHAL_TOP_TAG_TYPE_MFC_TOP                   0x06U                /**< MFC as Tag Type */
/*@}*/

/**
* \name Tag States
*/
/*@{*/
#define PHAL_TOP_STATE_NONE                     0x00U                /**< Default initial state. */
#define PHAL_TOP_STATE_INITIALIZED              0x01U                /**< Initialized state. */
#define PHAL_TOP_STATE_READONLY                 0x02U                /**< Read Only state. */
#define PHAL_TOP_STATE_READWRITE                0x04U                /**< Read/Write state. */
/*@}*/

//NDEF
/**
* \brief Does the NDEF detection procedure as per NFC Tag Operation
* specifications for each tag type.
*
* The caller has to ensure that Tag activation is done before calling this API.
* Also \ref phalTop_SetConfig should be called before to configure the Tag type
* #PHAL_TOP_CONFIG_TAG_TYPE. For Mifare Classic as tag type \ref phalTop_SetConfig needs to be called
* to configure the Mifare Classic card type #PHAL_TOP_CONFIG_MFCTOP_CARD_TYPE e.g. #PHAL_TOP_MFC_1K.
* Only after phalTop_CheckNdef is called any other
* NDEF operation on Tag can be performed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_NON_NDEF_TAG Tag don't indicate NDEF presence
* \retval #PHAL_TOP_ERR_UNSUPPORTED_VERSION Tag indicates NDEF presence but NDEF version mentioned in tag is not supported by reader library.
* \retval #PHAL_TOP_ERR_MISCONFIGURED_TAG Tag indicates NDEF presence but the NDEF CC or NDEF attribute information is wrongly configured
* \retval #PHAL_TOP_ERR_UNSUPPORTED_TAG Tag uses some proprietary or RFU or unsupported configuration
* \retval Other Depending on implementation and underlying component.
*/
extern "C" __declspec(dllexport) int __stdcall Top_CheckNdef(unsigned char * pTagState);

/**
* \brief Format a new non-NDEF tag as NDEF tag.
*
* FormatNdef shall be used for formatting a non-NDEF tag as NDEF tag if needed
* when CheckNdef returns error. Before formatting, tag parameters like max. NDEF
* size etc., shall be specified by the application. If not specified tag will be
* formatted with default values. FormatNdef shall be called
* only once for a tag. Once formatted, tag will become a NDEF tag. To remove NDEF
* format if needed, application needs to over write / delete NDEF format using
* tag specific read/write commands.
*
* This is only a utility function not specified by NFC Forum.
* Please note that for T4T, this will attempt to use MIFARE DESFire proprietary
* commands to format the tag. If T4T is not MIFARE DESFire tag then format
* will not succeed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_UNSUPPORTED_TAG Tag does not support NDEF formatting.
* \retval #PHAL_TOP_ERR_FORMATTED_TAG Tag is already NDEF formatted.
* \retval Other Depending on implementation and underlying component.
*/
extern "C" __declspec(dllexport) int __stdcall Top_FormatNdef(void);

/**
* \brief Erase a valid NDEF message with an empty NDEF.
*
* EraseNdef shall be used to erase a valid NDEF message by writing an empty NDEF
* (i.e. NDEF length as 0) to tag. This will change the tag from read/write state to
* initialized state. If tag is already in initialized state this API will return error.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_INVALID_STATE Tag is not is any valid state (i.e. when check NDEF failed)
* \retval #PHAL_TOP_ERR_READONLY_TAG Tag is in read only state.
* \retval #PHAL_TOP_ERR_EMPTY_NDEF Tag is in initialized state (i.e. no NDEF / empty NDEF)
* \retval Other Depending on implementation and underlying component.
*/
extern "C" __declspec(dllexport) int __stdcall Top_EraseNdef(void);

/**
* \brief Reset Tag Operation parameters
*
* User has to call phalTop_Reset to reset all the Software parameters. This shall
* be called after performing all NDEF operations if needed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
extern "C" __declspec(dllexport) int __stdcall Top_Reset(void);

/**
* \brief Set configuration parameter.
* Refer to #PHAL_TOP_CONFIG_TAG_TYPE from where the configurable parameters are listed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
extern "C" __declspec(dllexport) int __stdcall Top_SetConfig(unsigned short wConfig,unsigned short wValue);

/**
* \brief Get configuration parameter.
*
* After calling \ref phalTop_CheckNdef, \ref phalTop_WriteNdef, \ref phalTop_ReadNdef
* or \ref phalTop_EraseNdef, user can call phalTop_GetConfig to get different
* parameters.
*
* Refer to #PHAL_TOP_CONFIG_TAG_TYPE from where the configurable parameters are listed.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
*/
extern "C" __declspec(dllexport) int __stdcall Top_GetConfig(unsigned short wConfig,unsigned short  * wValue);

/**
* \brief Read NDEF message from Tag.
*
* ReadNdef shall only be called after CheckNdef returned success.
* If it is an empty NDEF message(i.e. initialized state) then this will return
* empty NDEF error.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PHAL_TOP_ERR_INVALID_STATE Tag is not is any valid state (i.e. when check NDEF failed or has not been called before.)
* \retval #PHAL_TOP_ERR_EMPTY_NDEF Tag is in initialized state (i.e. no NDEF / empty NDEF)
* \retval Other Depending on implementation and underlying component.
*/
extern "C" __declspec(dllexport) int __stdcall Top_ReadNdef(unsigned char * pData,unsigned short * pLength);

/**
* \brief Write NDEF message into Tag.
*
* WriteNdef shall be used to write a new NDEF message to tag if check NDEF
* returned success. If tag is in read only state, write NDEF will return error.
* The tag is expected to be a formatted NDEF tag for this to succeed.
*
* WriteNDEF will update the NDEF message TLV or the capability container with
* the length of the data written to the tag.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER wLength is 0 or wLength is more than supported max length.
* \retval #PHAL_TOP_ERR_INVALID_STATE Tag is not is any valid state (i.e. when check NDEF failed).
* \retval #PHAL_TOP_ERR_READONLY_TAG Tag is in read only state.
* \retval Other Depending on implementation and underlying component.
*/
extern "C" __declspec(dllexport) int __stdcall Top_WriteNdef(unsigned char * pData,unsigned short wLength);



/*****************************************************************************
This command is implemented in compliance with ISO/IEC 7816-4
[In,more information]-------------------------------------------------------------------------------------------------
bOption, Option for return / no return of FCI:
MFNTAG42XDNA_FCI_RETURNED
MFNTAG42XDNA_FCI_NOT_RETURNED
bSelector,The selector to be used:                                                                             
MFNTAG42XDNA_SELECTOR_0                                                                             
MFNTAG42XDNA_SELECTOR_1                                                                              
MFNTAG42XDNA_SELECTOR_2                                                                                
MFNTAG42XDNA_SELECTOR_3                                                                               
MFNTAG42XDNA_SELECTOR_4
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			//Depending on implementation and underlying component.
*****************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_IsoSelectFile(
unsigned char bOption,		// [In] Option for return / no return of FCI.
unsigned char bSelector,    	// [In] The selector to be used.
unsigned char * pFid,      	// [In] The ISO File number to be selected. 
unsigned char bDFnameLen,	// [In] Length of bytes available in DFname buffer. 
unsigned char * pDFname,  	// [In] The ISO DFName to be selected. Valid only when bOption = 0x04. 
unsigned char bExtendedLenApdu, // [In] Flag for Extended Length APDU. This should be always cleared 
unsigned char *pFCI,        	// [Out] The FCI information returned by the PICC. 
unsigned short * pwFCILen  	// [Out] Length of bytes available in FCI buffer. 
);

/*****************************************************************************
Performs an First or Non First Authentication depending upon bFirstAuth Parameter. 
This will be using the AES128 keys and will generate and verify the contents based on generic AES algorithm.
[In,more information]--------------------------------------------------------------------------------------------------
bFirstAuth:One of the below options.
MFNTAG42XDNA_AUTHNONFIRST_NON_LRP(0x00)
MFNTAG42XDNA_AUTHFIRST_NON_LRP(0x01)                 
MFNTAG42XDNA_AUTHNONFIRST_LRP(UN-usable)                                                                               
MFNTAG42XDNA_AUTHFIRST_LRP(UN-usable) 

wOption:Diversification option can be one of:                                                                      
MFNTAG42XDNA_NO_DIVERSIFICATION(default 0xFFFF)                                                                             
MFNTAG42XDNA_DIV_METHOD_ENCR                                                                                
MFNTAG42XDNA_DIV_METHOD_CMAC     
(16bit 2bytes LSB first)                                                                           
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]-------------------------------------------------------------------------------------------------------------------
PH_ERR_SUCCESS	 //Operation successful.
Other 			//Depending on implementation and underlying component.
******************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_AuthenticateEv2 (
unsigned char bFirstAuth,   //[In] First authenticate(1) or NON(0)                                                                        
unsigned short wOption,     	//[In] Diversification option(Default 0xFFFF)
unsigned char bKeyNoCard,   	//[In] Key number on card(0-4) 
unsigned char *pKey16bytes, //[In] 16 bytes AES key
unsigned char bLenPcdCapsIn,	//[In] Length of PcdCapsIn. Always zero for following authentication
unsigned char *bPcdCapsIn,  	//[In] PCD Capabilities,always 6byte,can be all 0x00,not use bytes fill 0x00. 
unsigned char bDivLen,      	//[In] Length of diversification input max 31B. Can be zero
unsigned char *pDivInput,   	//[In] Diversification input. Can be NULL. 
unsigned char *bPcdCapsOut, 	//[Out] PCD Capabilities 6bytes, Always not usable. 
unsigned char *bPdCapsOut  	//[Out] PD Capabilities 6bytes, Always not usable.  
);

/*****************************************************************************
Configures the card and pre personalizes the card with a key, 
defines if the UID or the random ID is sent back during communication setup and configures the ATS string.
[In,more information]--------------------------------------------------------------------------------------------------
bOption:One of the below options.
MFNTAG42XDNA_SET_CONFIG_OPTION0     0x00U 
MFNTAG42XDNA_SET_CONFIG_OPTION4     0x04U
MFNTAG42XDNA_SET_CONFIG_OPTION5     0x05U
MFNTAG42XDNA_SET_CONFIG_OPTION7     0x07U
MFNTAG42XDNA_SET_CONFIG_OPTION10    0x0AU
MFNTAG42XDNA_SET_CONFIG_OPTION11    0x0BU                                                                   
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS //Operation successful.
Other //Depending on implementation and underlying component.
******************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_SetConfiguration (
unsigned char bOption,  // [In] Configuration Option. Define length and content of the Data parameter.
unsigned char bDataLen, 	// [In] Length of bytes available in Data buffer. 
unsigned char *pData   	// [In] Data for the option specified.
);


/*****************************************************************************
Returns manufacturing related data of the PICC
[In,more information]-------------------------------------------------------------------------------------------------
NULL                                                                 
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS //Operation successful.
Other //Depending on implementation and underlying component.
******************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetVersion(
unsigned char * pVerInfo  // [Out] The version information of the product,28 bytes. 
);



/*****************************************************************************
Returns the Unique ID of the PICC
[In,more information]-------------------------------------------------------------------------------------------------
NULL                                                                 
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS //Operation successful.
Other //Depending on implementation and underlying component.
*****************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetCardUID(
unsigned char * pUid  // [Out] The complete UID of the PICC,7 bytes. 
);




/*****************************************************************************
Changes any key on the PICC
[In,more information]-------------------------------------------------------------------------------------------------
wOption, One of the below options:                                                                      
MFNTAG42XDNA_NO_DIVERSIFICATION (default 0xFFFF)
MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC      
MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY | MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC    
MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY_ONERND                                                                      MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY | MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY_ONERND                                                          MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY                                              MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY |MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY |                 MFNTAG42XDNA_CHGKEY_DIV_METHOD_CMAC
MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY | #PHAL_MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY |
MFNTAG42XDNA_CHGKEY_DIV_NEW_KEY_ONERND|MFNTAG42XDNA_CHGKEY_DIV_OLD_KEY_ONERND          
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS //Operation successful.
Other //Depending on implementation and underlying component.
*****************************************************************************/
//call function:
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_ChangeKey(
unsigned short wOption,		// [In] One of the below options,2 bytes LSB.
unsigned char bKeyNoCard,	// [In] Key number on card. 
unsigned char *pNewKey,		// [In] New key 16 bytes. 
unsigned char *pOldKey,		// [In] New key 16bytes. 
unsigned char bDivLen,		// [In] Length of diversification input max 31B,default 0. 
unsigned char * pDivInput	// [In] Diversification input. Can be NULL. 
);


/*****************************************************************************
Reads out the current key version of any key stored on the PICC
[In,more information]-------------------------------------------------------------------------------------------------
NULL,no more infor, please refer to the call function      
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetKeyVersion(
unsigned char bKeyNo,      	// [In] Key number on card. 
unsigned char * pKeyVersion, 	// [Out] The version of the specified key. 
unsigned char * bRxLen      	// [Out] Length of bytes available in KeyVersion buffer. 
);



/*****************************************************************************
Get informtion on the properties of a specific file
[In,more information]-------------------------------------------------------------------------------------------------
NULL,no more infor, please refer to the call function      
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetFileSettings(
unsigned char bFileNo,      	// [In] Key number on card. 
unsigned char * pFSBuffer, 	// [Out] The version of the specified key. 
unsigned char * bBufferLen  // [Out] Length of bytes available in KeyVersion buffer. 
);


/*****************************************************************************
1.Returns manufacturing related data of the PICC
2.The GET_FILECOUNTERS command supports retrieving of the current values associated with the SDMReadCtr related with a StandardData file after enabling Secure Dynamic Messaging
3.This command use in SDM mode only
[In,more information]-------------------------------------------------------------------------------------------------
bOption:
MFNTAG42XDNA_COMMUNICATION_PLAIN
MFNTAG42XDNA_COMMUNICATION_ENC   
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetFileCounters(
unsigned char bOption,  // [In] the communication mode to be used while exchanging data to PICC.                                                                                                 
unsigned char bFileNo,  // [In] File number for which the Counter information need to be received. 
unsigned char * pFileCounters, // [Out] The SDMReadCounter information returned by the PICC. 
unsigned char * pRxLen     	// [Out] Length of bytes available in FileCounters buffer. 
);


/*****************************************************************************
Changes the access parameters of an existing file.
[In,more information]-------------------------------------------------------------------------------------------------
bCommMode, Indicates the mode of communication PCD to PICC
MFNTAG42XDNA_COMMUNICATION_PLAIN
MFNTAG42XDNA_COMMUNICATION_MACD
MFNTAG42XDNA_COMMUNICATION_ENC
Ored with the below option
MFNTAG42XDNA_SPECIFICS_ENABLED

bFileOption, New communication settings for the file.
MFNTAG42XDNA_FILE_OPTION_PLAIN
MFNTAG42XDNA_FILE_OPTION_PLAIN_1
MFNTAG42XDNA_FILE_OPTION_MACD
MFNTAG42XDNA_FILE_OPTION_ENC
Ored with one of the below flags if required.
MFNTAG42XDNA_SDM_ENABLED

bAdditionalInfoLen, Length of the Additional Info to be sent (2 Bytes or 4 Bytes).
This is used to set TMC Limit in case of TMAC File

* bAdditionalInfo, One of the below inforamtion
If Standard AES : 4 byte TMC Limit value.
If LRP        : 2 byte TMC Limit value.

[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/

extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_ChangeFileSettings(
unsigned char bCommMode, 		// [In] Indicates the mode of communication PCD to PICC
unsigned char bFileNo,    	 	// [In] File number for which the setting need to be updated. 
unsigned char bFileOption,    	// [In] New communication settings for the file.
unsigned char * pAccessRights, 	// [In] The new access right. Should be 2 byte,LSB first. 
unsigned char bAdditionalInfoLen, 	// [In] Length of the Additional Info to be sent (2 Bytes or 4 Bytes).
unsigned char * bAdditionalInfo  	// [In] One of the below inforamtion
);



#define PHAL_MFNTAG42XDNA_VCUID_PRESENT                                 0x80U   /**< Only VCUID is considred for SDM MAC calculation */
#define PHAL_MFNTAG42XDNA_RDCTR_PRESENT                                 0x40U   /**< Only RDCTR  is considred for SDM MAC calculation */
#define PHAL_MFNTAG42XDNA_RDCTR_LIMIT_PRESENT                           0x20U   /**< Indicates the presence of SDM Read Counter Limit. */
#define PHAL_MFNTAG42XDNA_SDM_ENC_FILE_DATA_PRESENT                     0x10U   /**< Indicates the presence of SDM ENC File data. */
#define PHAL_MFNTAG42XDNA_SDM_TT_STATUS_PRESENT                         0x08U   /**< Indicates the presence of SDM TT Status. */
#define PHAL_MFNTAG42XDNA_SDM_ENCODING_MODE_ASCII                       0x01U   /**< Indicates the encoding as ASCII. */
#define PHAL_MFNTAG42XDNA_SPECIFICS_ENABLED                             0x01u   /**< MF NTAG 42xDNA Specific feature to  be used. */
/*****************************************************************************
Changes the access parameters of an existing file.
[In,more information]-------------------------------------------------------------------------------------------------
bCommMode, Indicates the mode of communication PCD to PICC
MFNTAG42XDNA_COMMUNICATION_PLAIN
MFNTAG42XDNA_COMMUNICATION_MACD
MFNTAG42XDNA_COMMUNICATION_ENC
Ored with the below option
MFNTAG42XDNA_SPECIFICS_ENABLED

bFileOption, New communication settings for the file.
MFNTAG42XDNA_FILE_OPTION_PLAIN
MFNTAG42XDNA_FILE_OPTION_PLAIN_1
MFNTAG42XDNA_FILE_OPTION_MACD
MFNTAG42XDNA_FILE_OPTION_ENC
Ored with one of the below flags if required.
MFNTAG42XDNA_SDM_ENABLED

bSdmOptions,[In] One of the below values to be used. Can be ORed.
PHAL_MFNTAG42XDNA_VCUID_PRESENT
PHAL_MFNTAG42XDNA_RDCTR_PRESENT
PHAL_MFNTAG42XDNA_RDCTR_LIMIT_PRESENT
PHAL_MFNTAG42XDNA_SDM_ENC_FILE_DATA_PRESENT
PHAL_MFNTAG42XDNA_SDM_TT_STATUS_PRESENT
Must be ored with the above values.
PHAL_MFNTAG42XDNA_SDM_ENCODING_MODE_ASCII

[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_ChangeFileSettingsSDM(
unsigned char bCommMode, 		// [In] Indicates the mode of communication PCD to PICC
unsigned char bFileNo,    	 	// [In] File number for which the setting need to be updated. 
unsigned char bFileOption,    	// [In] New communication settings for the file.
unsigned char * pAccessRights, 	// [In] The new access right. Should be 2 byte,LSB first. 
unsigned char bSdmOptions, 		// [In] One of the below values to be used. Can be ORed.
unsigned char * pSdmAccessRights,  	// [In] One of the below inforamtion
unsigned char * pVCUIDOffset,          // [In] VCUID Offset information. Should be 3 bytes. 
unsigned char * pSDMReadCtrOffset,     // [In] SDMReadCtr value (LSB first). Should be 3 bytes. 
unsigned char * pPICCDataOffset,       // [In] Mirror position (LSB first) for encrypted PICCData. Should be 3 bytes. 
unsigned char * pTTPermStatusOffset,   // [In] Tag Tamper Permanent Status Offset value. Should be 3 bytes. 
unsigned char * pSDMMACInputOffset,    // [In] Offset in the file where the SDM MAC computation starts (LSB first). Should be 3 bytes.
unsigned char * pSDMENCOffset,         // [In] SDMENCFileData mirror position (LSB first). Should be 3 bytes. 
unsigned char * pSDMENCLen,            // [In] Length of the SDMENCFileData (LSB first). Should be 3 bytes. 
unsigned char * pSDMMACOffset,         // [In] SDMMAC mirror position (LSB first). Should be 3 bytes. 
unsigned char * pSDMReadCtrLimit       // [In] SDM Read Counter Limit value. Should be 3 bytes. 
);

/*****************************************************************************
Calculate MAC for SDM information.
[In,more information]-------------------------------------------------------------------------------------------------
bsdmoption,[In] One of the below values to be used. Can be ORed.
PHAL_MFNTAG42XDNA_VCUID_PRESENT
PHAL_MFNTAG42XDNA_RDCTR_PRESENT

[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_CalculateMACSDM(
unsigned char bsdmoption,   //< [in] sdm option to indicate which parameters to be considered. can be ored.	
unsigned char * wSDMMacKey, //< [in] key 16 bytes. 
unsigned char buidlen,            //< [in] length of bytes available in uid buffer. 
unsigned char * puid,             //< [in] uid of the card. 
unsigned char * psdmreadctr,      //< [in] sdm read counter input 4 bytes. 
unsigned short windatalen,        //< [in] length of bytes available in indata buffer. 
unsigned char * pindata,          //< [in] data read out of picc from sdmmacinputoffset for readlength bytes 
unsigned char * prespmac          //< [out] the computed mac information. 
);


/*****************************************************************************
Decrypt the SDM File data information
[In,more information]-------------------------------------------------------------------------------------------------
bsdmoption,[In] One of the below values to be used. Can be ORed.
PHAL_MFNTAG42XDNA_VCUID_PRESENT
PHAL_MFNTAG42XDNA_RDCTR_PRESENT

[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall phalMfNtag42XDna_DecryptSDMENCFileData(
unsigned char bsdmoption,   //< [in] sdm option to indicate which parameters to be considered. can be ored.	
unsigned char * wSDMMacKey, //< [in] key 16 bytes. 
unsigned char buidlen,            //< [in] length of bytes available in uid buffer. 
unsigned char * puid,             //< [in] uid of the card. 
unsigned char * psdmreadctr,      //< [in] sdm read counter input 4 bytes. 
unsigned short windatalen,        //< [in] length of bytes available in pEncdata buffer. 
unsigned char * pEncdata,         //< [in] Input Enciphered data. Caller has to pass only the Enciphered data which is between SDMEncOffset for SDMEncLength bytes
unsigned char * pPlainData        //< [out] The decrypted SDM ENC file data. 
);


/*****************************************************************************
Decrypt SDM PICC Data
[In,more information]-------------------------------------------------------------------------------------------------
NULL
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_DecryptSDMPICCData(
unsigned char * wKey,			//< [in] key 16 bytes. 
unsigned short wInDataLen,      //< [in] length of bytes available in indata buffer. 
unsigned char * pEncdata,       //< [in] The Encrypted PICC data 
unsigned char * pPlainData      //< [out] The decrypted SDM PICC data 
);

/*****************************************************************************
Reads data from standard data files or backup data files
Chaining upto the size of the HAL Rx buffer is handled within this function. 
If more data is to be read, the user has to call this function again 
with bOption = #PH_EXCHANGE_RXCHAINING | [one of the communication options]
[In,more information]-------------------------------------------------------------------------------------------------
bOption,Communication settings for the file:
#PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
#PHAL_MFNTAG42XDNA_COMMUNICATION_MACD
#PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
PH_EXCHANGE_RXCHAINING: To be Or'd with the above option flag if Chaining status is returned.

* pOffset, The offset from where the data should be read. Will be of 3 bytes with LSB first.
If 0x10 need to be offset then it will be 10 00 00.

* pLength,The number of bytes to be read. Will be of 3 bytes with LSB first. 
If 0x10 bytes need to be read then it will be 10 00 00. \n
If complete file need to be read then it will be 00 00 00.

[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_ReadData (
unsigned char bOption,      // [In] Communication settings for the file.                                                                  
unsigned char bIns,         // [In] If set, uses ISO 14443-4 chaining instead of DESFire application chaining,default setting 0x01. 
unsigned char bFileNo,      // [In] The file number from where the data to be read. 
unsigned char * pOffset,     // [In] The offset from where the data should be read. 3 bytes LSB first.
unsigned char * pLength,    // [In] The number of bytes to be read. Will be of 3 bytes with LSB first. 
unsigned char *pRxdata,  	// [Out] The data retuned by the PICC. 
unsigned short * pRxdataLen  // [Out] Length of bytes available in RxData buffer. 
);



/*****************************************************************************
Writes data to standard data files or backup data files
Chaining upto the size of the HAL Rx buffer is handled within this function. If more data is to be read, the user has to call this function again with bOption = #PH_EXCHANGE_RXCHAINING | [one of the communication options]
[In,more information]-------------------------------------------------------------------------------------------------
bOption,Communication settings for the file:
#PHAL_MFNTAG42XDNA_COMMUNICATION_PLAIN
#PHAL_MFNTAG42XDNA_COMMUNICATION_MACD
#PHAL_MFNTAG42XDNA_COMMUNICATION_ENC
PH_EXCHANGE_RXCHAINING: To be Or'd with the above option flag if Chaining status is returned.

* pOffset, The offset from where the data should be read. Will be of 3 bytes with LSB first.
If 0x10 need to be offset then it will be 10 00 00.

* pTxDataLen,The number of bytes to be read. Will be of 3 bytes with LSB first. 
If 0x10 bytes need to be read then it will be 10 00 00. \n
If complete file need to be read then it will be 00 00 00.

[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
//call function:
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_WriteData (
unsigned char bOption,      // [In] Communication settings for the file.                                                                  
unsigned char bIns,         // [In] If set, uses ISO 14443-4 chaining instead of DESFire application chaining,default setting 0x01. 
unsigned char bFileNo,      // [In] The file number from where the data to be read. 
unsigned char * pOffset,     // [In] The offset from where the data should be read. 3 bytes LSB first.
unsigned char * pTxDataLen, // [In] The number of bytes to be written. Will be of 3 bytes with LSB first.  
unsigned char * pTxData		// [In] The data to be written to the PICC.  
);


/*****************************************************************************
MIFARE NTAG 42xDNA Originality Check functions
Performs the originality check to verify the genuineness of chip
[In,more information]-------------------------------------------------------------------------------------------------
NULL
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_ReadSign (
unsigned char bAddr,      	// [In] Value is always 00. Present for forward compatibility reasons 
unsigned char *pSignature   	// [Out] The plain 56 bytes originality signature of the PICC. 
);



/*****************************************************************************
MIFARE NTAG 42xDNA Tag Tamper Protection functions. 
Returns Tag Tamper Status data of the PICC
Only use when card in SDM mode
[In,more information]-------------------------------------------------------------------------------------------------
NULL
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetTagTamperStatus (
unsigned char * pRxBuffer,      // [Out] Read Tag Tamper Protection status 
unsigned char * pRxLen         // [Out] Recieved Length 
);


/*****************************************************************************
MIFARE NTAG 42xDNA Miscellaneous functions
These Components implement the MIFARE DESFire Light Miscellaneous functions. These are not part of actual Identity data sheet
[In,more information]-------------------------------------------------------------------------------------------------
wConfig,Configuration to read. Will be one of the below values.
MFNTAG42XDNA_ADDITIONAL_INFO
MFNTAG42XDNA_SDM_KEY_TYPE
Support for Software mode only including the above ones.
MFNTAG42XDNA_WRAPPED_MODE
MFNTAG42XDNA_SHORT_LENGTH_APDU
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_GetConfig (
unsigned short wConfig,     // [In] Configuration to read. 
unsigned char * pValue      // [Out] The value for the mentioned configuration. 
);




/*****************************************************************************
MIFARE NTAG 42xDNA Miscellaneous functions
These Components implement the MIFARE DESFire Light Miscellaneous functions. These are not part of actual Identity data sheet
[In,more information]-------------------------------------------------------------------------------------------------
wConfig,Configuration to read. Will be one of the below values.
MFNTAG42XDNA_ADDITIONAL_INFO
MFNTAG42XDNA_SDM_KEY_TYPE
Support for Software mode only including the above ones.
MFNTAG42XDNA_WRAPPED_MODE
MFNTAG42XDNA_SHORT_LENGTH_APDU
[Out,more information]-----------------------------------------------------------------------------------------------
NULL
[Return]------------------------------------------------------------------------------------------------------------------ 
PH_ERR_SUCCESS 	//Operation successful.
Other			 //Depending on implementation and underlying component.
*****************************************************************************/
//call function:
extern "C" __declspec(dllexport) int __stdcall MfNtag42XDna_SetConfig (
unsigned short wConfig,     // [In] Configuration to read. 
unsigned short wValue				// [In] The value for the mentioned configuration. 
);











//
#endif