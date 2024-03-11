#include <sys/stat.h>

// 0 success; -1 failed
int mkfifo(const char *pathname, mode_t mode);