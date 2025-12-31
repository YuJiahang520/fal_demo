#include <fal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int ret = 0;
    uint32_t count1, count2;

    // 初始化FAL，其会初始化分区表中的所有Flash
    if (fal_init() <= 0)
    {
        printf("FAL init failed!\n");
        return -1;
    }
    printf("FAL init success!\n");

    // 显示分区表
    fal_show_part_table();

    // 获取 param 分区句柄
    const struct fal_partition *part_param = fal_partition_find("param");
    if (part_param == NULL)
    {
        printf("Partition 'param' not found!\n");
        return -1;
    }
    printf("Find partition '%s' success! Addr: 0x%08x, Len: %d\n", part_param->name, part_param->offset,
           part_param->len);

    // 获取 falsh2 分区句柄
    const struct fal_partition *part_falsh2 = fal_partition_find("falsh2");
    if (part_falsh2 == NULL)
    {
        printf("Partition 'falsh2' not found!\n");
        return -1;
    }
    printf("Find partition '%s' success! Addr: 0x%08x, Len: %d\n", part_falsh2->name, part_falsh2->offset,
           part_falsh2->len);

    // 读计数值
    fal_partition_read(part_param, 0, (uint8_t *)&count1, 4);
    fal_partition_read(part_falsh2, 0, (uint8_t *)&count2, 4);

    if (count1 == 0xFFFFFFFF)
    {
        count1 = 0;
    }

    if (count2 == 0xFFFFFFFF)
    {
        count2 = 0;
    }

    count1++;
    count2++;

    printf("count1 = %d, count2 = %d\r\n", count1, count2);

    // 擦除一个扇区
    fal_partition_erase(part_param, 0, 4096);
    fal_partition_erase(part_falsh2, 0, 4096);

    // 写入更新值
    fal_partition_write(part_param, 0, (uint8_t *)&count1, 4);
    fal_partition_write(part_falsh2, 0, (uint8_t *)&count2, 4);

    // part_param分区size = 65 * 1024，这里尝试越界访问
    ret = fal_partition_read(part_param, 65 * 1024, (uint8_t *)&count1, 4);
    printf("ret1 = %d\r\n", ret);
    ret = fal_partition_read(part_param, 63 * 1024, (uint8_t *)&count1, 4);
    printf("ret2 = %d\r\n", ret);
    ret = fal_partition_read(part_param, 63 * 1024, (uint8_t *)&count1, 2 * 1024);
    printf("ret3 = %d\r\n", ret);

    return 0;
}