#ifndef __COMMON_H__
#define __COMMON_H__

/**
 * @brief 检查一个数字是否是2的幂
 * @param x 被判断的元素
 * @return 0 等于 0，则返回 true（1），否则返回 false（0）
 * (01111 & 10000) == 0
 * TODO 测试 0
 * */
#define POWEROF2(x) ((((x)-1) & (x)) == 0)

#endif