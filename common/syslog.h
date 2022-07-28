#ifndef _SYSLOG_H_
#define _SYSLOG_H_

extern void syslog_init(uint32 no);
extern void syslog(const char *format, ...);

#endif /* _SYSLOG_H_ */
