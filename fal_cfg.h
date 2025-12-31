#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <fal.h>

/* 声明外部的设备对象 */
extern const struct fal_flash_dev pc_nor_flash1;
extern const struct fal_flash_dev pc_nor_flash2;

/* 设备表：告诉 FAL 我们有什么物理设备 */
#define FAL_FLASH_DEV_TABLE                                                                                            \
    {                                                                                                                  \
        &pc_nor_flash1,                                                                                                \
        &pc_nor_flash2,                                                                                                \
    }

#define FAL_PART_HAS_TABLE_CFG

/* 分区表：把 1MB 的文件切分 */
/* 格式: {Magic, 分区名, 关联设备, 偏移, 大小, 0} */
#define FAL_PART_TABLE                                                                                                 \
    {                                                                                                                  \
        {FAL_PART_MAGIC_WORD, "bootloader", "pc_nor_flash1", 0, 64 * 1024, 0},                                         \
        {FAL_PART_MAGIC_WORD, "param", "pc_nor_flash1", 64 * 1024, 64 * 1024, 0},                                      \
        {FAL_PART_MAGIC_WORD, "app", "pc_nor_flash1", 128 * 1024, 512 * 1024, 0},                                      \
        {FAL_PART_MAGIC_WORD, "data", "pc_nor_flash1", (128 + 512) * 1024, 256 * 1024, 0},                             \
                                                                                                                       \
        {FAL_PART_MAGIC_WORD, "falsh2", "pc_nor_flash2", 0, 1024 * 1024, 0},                                           \
    }

#endif /* _FAL_CFG_H_ */