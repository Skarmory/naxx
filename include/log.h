#ifndef YUN_LOG_H
#define YUN_LOG_H

#define LOG_MSGHIST 0
#define LOG_DEBUG   1

void init_logs(void);
void uninit_logs(void);
void log_msg(int logtype, const char* msg);
void log_format_msg(int logtype, const char* format, ...);
void log_scheck_fail(const char* msg);

#endif
