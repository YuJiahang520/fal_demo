#include <fal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* 定义模拟 Flash 的总大小：1MB */
#define FLASH_FILE_NAME  "../virtual_flash2.bin"
#define FLASH_TOTAL_SIZE (1024 * 1024)
#define FLASH_BLOCK_SIZE (4 * 1024) /* 模拟 4KB 为一个擦除块 */

static FILE *fp = NULL;

static int pc_flash_init(void)
{
    /* 尝试以 读写模式 打开存在的文件 */
    fp = fopen(FLASH_FILE_NAME, "rb+");
    
    if (fp == NULL)
    {
        /* 文件不存在，以 读写模式 创建新文件 */
        fp = fopen(FLASH_FILE_NAME, "wb+");
        if (fp == NULL)
        {
            printf("[Driver2] Error: Cannot create flash file.\n");
            return -1;
        }

        uint8_t buf[4096];
        memset(buf, 0xFF, sizeof(buf));
        
        for (int i = 0; i < FLASH_TOTAL_SIZE / sizeof(buf); i++)
        {
            fwrite(buf, 1, sizeof(buf), fp);
        }
        fflush(fp);
        printf("[Driver2] Created virtual flash file (%d bytes)\n", FLASH_TOTAL_SIZE);
    }
    else
    {
        printf("[Driver2] Opened existing virtual flash file.\n");
    }

    return 0;
}

static int pc_flash_read(long offset, uint8_t *buf, size_t size)
{
    if (fp == NULL) return -1;

    fseek(fp, offset, SEEK_SET);
    size_t read_len = fread(buf, 1, size, fp);

    printf("[Driver2] Read offset: %ld, size: %zu\n", offset, size);
    
    return read_len;
}

static int pc_flash_write(long offset, const uint8_t *buf, size_t size)
{
    if (fp == NULL) return -1;

    fseek(fp, offset, SEEK_SET);
    size_t write_len = fwrite(buf, 1, size, fp);
    fflush(fp); /* 确保立即写入磁盘，方便调试时查看文件 */

    printf("[Driver2] Write offset: %ld, size: %zu\n", offset, size);
    return write_len;
}

static int pc_flash_erase(long offset, size_t size)
{
    if (fp == NULL) return -1;
    if (size % FLASH_BLOCK_SIZE != 0) return -1;

    uint8_t buf[4096]; // 使用 4KB 缓冲区
    memset(buf, 0xFF, sizeof(buf));

    fseek(fp, offset, SEEK_SET);

    for (size_t i = 0; i < (size / FLASH_BLOCK_SIZE); i++)
    {
        fwrite(buf, 1, FLASH_BLOCK_SIZE, fp);
    }
    
    fflush(fp);

    printf("[Driver2] Erased offset: %ld, size: %zu\n", offset, size);
    return size;
}

/* 定义设备对象 */
const struct fal_flash_dev pc_nor_flash2 = {
    .name = "pc_nor_flash2",
    .addr = 0,
    .len = FLASH_TOTAL_SIZE,
    .blk_size = FLASH_BLOCK_SIZE,
    .ops = {pc_flash_init, pc_flash_read, pc_flash_write, pc_flash_erase},
    .write_gran = 1, /* PC 文件模拟通常支持按 1 字节写入 */
};