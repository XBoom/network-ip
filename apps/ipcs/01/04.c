#include "common.h"

#define POPEN_FMT "ls | grep %s"
#define PATERN_LEN (20)
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PATERN_LEN)
#define PATH_LEN (100)

int len, status, fileCnt;

int main(int argc, char *argv[])
{
    char patern[PATERN_LEN] = {0};
    char popenCmd[PCMD_BUF_SIZE] = {0};
    int len = 0;

    FILE *fp;
    while (1)
    {
        printf("pattern: ");
        // fflush(stdout);
        if (fgets(patern, PATERN_LEN, stdin) == NULL)
        {
            break;
        }

        len = strlen(patern);
        if (len <= 1)
            continue;

        if (patern[len - 1] == '\n')
            patern[len - 1] = '\0';

        snprintf(popenCmd, PCMD_BUF_SIZE, POPEN_FMT, patern);

        LOG_INFO("popenCmd:%s", popenCmd);
        fp = popen(popenCmd, "r"); // 管道读
        CHECK_RET_EXIT(fp == NULL, "popen failed");

        char pathname[PATH_LEN];
        fileCnt = 0;
        while (fgets(pathname, PATH_LEN, fp) != NULL)
        {
            pathname[strlen(pathname) - 1] = '\0';
            LOG_INFO("file[%d] name: %s", fileCnt, pathname);
            fileCnt++;
        }

        status = pclose(fp);
    }

    return 0;
}