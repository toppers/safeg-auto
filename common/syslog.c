#include "u2a.h"
#include <stdarg.h>

#define TMAX_LOGINFO	6U

typedef struct {
	uint32				logtype;                /* ログ情報の種別 */
	uint32				logtim;                 /* ログ時刻 */
	uintptr				loginfo[TMAX_LOGINFO];  /* その他のログ情報 */
} SYSLOG;

/*
 *  数値を文字列に変換時，uintptr型の数値の最大文字数
 *  8進数，10進数，16進数へ変換を想定
 *  動作例：
 *  2Byteの場合：16進数：FFFF  →4文字
 *             ：10進数：65535 →5文字
 *             ： 8進数：177777→6文字
 *               →CONVERT_BUFLENは最大の6文字を返す
 *  計算パラメータ：
 *  8U:1バイトのビット数
 *  2U:3で割った際の余り1ビット，または2ビット分を含めるための補正値
 *  3U:10進数へ変換時，最大3ビットで1文字になる
 *     (4ビットだと2桁数字になるため)
 *  計算例：
 *  2Byteの場合
 *     ビット数は2 * 8U = 16bit
 *     単純に3で割ると，16 / 3 = 5となり，余り1ビット分の文字数が
 *     切り捨てられてしまう
 *     3で割る前に2ビット分足すことで，切捨てを防ぐ
 *     (16 + 2) / 3 = 6文字
 */
#define CONVERT_BUFLEN	(((sizeof(uintptr) * 8U) + 2U) / 3U)

static uint portno[TMAX_PHYS_CORE_U2A];

void
syslog_init(uint32 no) {
	uint	coreidx = get_peid();
	portno[coreidx] = no;
}

void
syslog_output(char8 c) {
	uint	coreidx = get_peid();
	uart_putc(portno[coreidx], c);
	if (c == '\n')
	  uart_putc(portno[coreidx], '\r');
}

/*
 *  数値を文字列に変換
 */
static void
convert(uintptr val, uint32 radix, const char8 *radchar,
		uint32 width, boolean minus, boolean padzero, void (*outputc)(char8 c))
{
	char8	buf[CONVERT_BUFLEN];
	uint32	i, j;

	i = 0U;
	do {
		buf[i++] = radchar[val % radix];
		val /= radix;
	} while ((i < CONVERT_BUFLEN) && (val != 0U));

	if ((minus != false) && (width > 0U)) {
		width -= 1U;
	}
	if ((minus != false) && (padzero != false)) {
		(*outputc)('-');
	}
	for (j = i; j < width; j++) {
		(*outputc)((char8) ((padzero != false) ? '0' : ' '));
	}
	if ((minus != false) && (padzero == false)) {
		(*outputc)('-');
	}
	while (i > 0U) {
		(*outputc)(buf[--i]);
	}
}

/*
 *  文字列整形出力
 */
static const char8	raddec[] = "0123456789";
static const char8	radhex[] = "0123456789abcdef";
static const char8	radHEX[] = "0123456789ABCDEF";

void
syslog_printf(const char8 *format, const uintptr *p_args, void (*outputc)(char8 c))
{
	char8		c;
	uint32		width;
	boolean		padzero;
	sintptr		val;
	const char8	*str;

	while ((c = *format++) != '\0') {
		if (c != '%') {
			(*outputc)(c);
			continue;
		}

		width = 0U;
		padzero = false;
		if ((c = *format++) == '0') {
			padzero = true;
			c = *format++;
		}
		while (('0' <= c) && (c <= '9')) {
			width = (width * 10U) + (c - '0');
			c = *format++;
		}
		if (c == 'l') {
			c = *format++;
		}
		switch (c) {
		case 'd':
			val = (sintptr) (*p_args++);
			if (val >= 0) {
				convert((uintptr) val, 10U, raddec,
						width, false, padzero, outputc);
			}
			else {
				convert((uintptr) (-val), 10U, raddec,
						width, true, padzero, outputc);
			}
			break;
		case 'u':
			val = (sintptr) (*p_args++);
			convert((uintptr) val, 10U, raddec, width, false, padzero, outputc);
			break;
		case 'x':
		case 'p':
			val = (sintptr) (*p_args++);
			convert((uintptr) val, 16U, radhex, width, false, padzero, outputc);
			break;
		case 'X':
			val = (sintptr) (*p_args++);
			convert((uintptr) val, 16U, radHEX, width, false, padzero, outputc);
			break;
		case 'c':
			(*outputc)((char8) (uintptr) (*p_args++));
			break;
		case 's':
			str = (const char8 *) (*p_args++);
			while ((c = *str++) != '\0') {
				(*outputc)(c);
			}
			break;
		case '%':
			(*outputc)('%');
			break;
		case '\0':
			format--;
			break;
		default:
			/* 上記のケース以外の場合，処理は行わない */
			break;
		}
	}
}

void
syslog(const char8 *format, ...)
{
	SYSLOG	syslogv;
	va_list	ap;
	uint32	i;
	char8	c;
	boolean	lflag;

	syslogv.loginfo[0] = (sintptr) format;
	i = 1U;
	va_start(ap, format);

	while (((c = *format++) != '\0') && (i < TMAX_LOGINFO)) {
		if (c != '%') {
			continue;
		}

		lflag = false;
		c = *format++;
		while (('0' <= c) && (c <= '9')) {
			c = *format++;
		}
		if (c == 'l') {
			lflag = true;
			c = *format++;
		}
		switch (c) {
		case 'd':
			syslogv.loginfo[i++] = (lflag != false) ? (sintptr) va_arg(ap, sint32)
								  : (sintptr) va_arg(ap, sint32);
			break;
		case 'u':
		case 'x':
		case 'X':
			syslogv.loginfo[i++] = (lflag != false) ? (sintptr) va_arg(ap, uint32)
								  : (sintptr) va_arg(ap, uint32);
			break;
		case 'p':
			syslogv.loginfo[i++] = (sintptr) va_arg(ap, void *);
			break;
		case 'c':
			syslogv.loginfo[i++] = (sintptr) va_arg(ap, sint32);
			break;
		case 's':
			syslogv.loginfo[i++] = (sintptr) va_arg(ap, const char8 *);
			break;
		case '\0':
			format--;
			break;
		default:
			/* 上記のケース以外の場合，処理は行わない */
			break;
		}
	}
	va_end(ap);
	(void) syslog_printf((const char8 *) (syslogv.loginfo[0]),
						  &(syslogv.loginfo[1]), syslog_output);
}
