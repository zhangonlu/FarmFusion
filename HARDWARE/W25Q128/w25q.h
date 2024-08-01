#ifndef __W25Q_H_
#define __W25Q_H_

#include "sys.h"
#include "spihardware.h"
#include "spi.h"

//W25Q指令集合
#define w25Q64__WRITE_ENABLE                         0x06
#define w25Q64__WRITE_DISABLE                        0x04
#define w25Q64_READ_STATUS_REGISTER_1                0x05
#define w25Q64_READ_STATUS_REGISTER_2                0x35
#define w25Q64_WRITE_STATUS_REGISTER                 0x01
#define w25Q64_PAGE_PROGRAM                          0x02
#define w25Q64_QUAD_PAGE_PROGRAM                     0x32
#define w25Q64_BLOCK_ERASE_64KB                      0xD8
#define w25Q64_BLOCK_ERASE_32KB                      0x52
#define w25Q64_SECTOR_ERASE__4KB                     0x20
#define w25Q64_CHIP_ERASE                            0xc7
#define w25Q64_ERASE_sUSPEND                         0x75
#define w25Q64_ERASE_RESUME                          0x7A
#define w25Q64 POWER DOWN	                         0xB9
#define w25Q64_HIGH_PERFORMANCE_MODE                 0xA3
#define w25Q64_CONTINUOUS_READ_MODE_RESET            0xFF
#define w25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID      0xAB
#define w25Q64_MANUFACTURER_DEVICE_ID			     0x90
#define w25Q64_READ_UNIQUE_ID                        0x4B
#define w25Q64_JEDEC_ID                              0x9F
#define w25Q64_READ_DATA                             0x03
#define w25Q64_FAST_READ                             0x0B
#define w25Q64_FAST_READ_DUAL_OUTPUT                 0x3B
#define w25Q64_FAST_READ_DUAL_IO                     0xBB
#define w25Q64_FAST_READ_QUAD_OUTPUT                 0x6B
#define w25Q64_FAST_READ_QUAD_Io                     0xEB
#define w25Q64_OCTAL_WORD_READ_QUAD_Io               0xE3
#define w25Q64_DUMMY_BYTE                            0xFF



void W25Q_Init(void);
void  Read_Data(u8 *data,u32 addr,u16 len);
void Program_Page(u32 addr,u8 *data,u16 len);
//扇区擦除
void Earse_Section(u32 addr);
void W25Q_Send(u8 *MID,u16 *DID);

#endif