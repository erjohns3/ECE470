// OpenCV stuff
#include "cxtypes.h"
#include "cvtypes.h"
#include "highgui.h"


// GUI stuff
#include <QApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QImage>  // maybe use with QLabel; maybe use raw
#include <QLabel>
#include <QList>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QSemaphore>
#include <QString>
#include <QTextEdit>
#include <QTimer>
#include <QWidget>

#include "rhino.h"
#include "console.h"
//#include "CameraControl.h"
#include "UserThread.h"
#include "ConsoleGUI.h"

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <windows.h>

int __cdecl vsscanf (
    const char* str,
    const unsigned char *format,
    va_list arglist
    );

// "QImage is designed and optimized for I/O, and for direct pixel access/manipulation."
// QRgb pixel(x,y)
// void setPixel(x,y,uint val)
// Note QRgb = QRgba = unsigned int
// QRgb *ptr=(QRgb *)image.scanline(0);
// or equivalently, QRgb *ptr=(QRgb *)image.bits();

CvCapture *camera;
QImage *images[4];
QLabel *views[4];

QLineEdit *commandLine;
QTextEdit *consoleView;
QList<QString> commandHistory;

char console_buffer[1024];

QSemaphore scanfSemaphore(0);
bool scanfWaiting; // signal that console_scanf is waiting for input
bool mselectWaiting; // signal that console_mselect is waiting for input

UserThread *userthread;

void console_printf(char *format, ...)
{
	va_list list;
	va_start(list, format);

	vsnprintf_s(console_buffer, 1024,_TRUNCATE, format, list);
	// remove (up to one) ending line break
	// These are redundant since the command to add to the console appends a line break
	int last=strlen(console_buffer)-1;
	if(console_buffer[last] == '\n') console_buffer[last]='\0';
	
	return userthread->printf(console_buffer);
}

int console_scanf(char *format, ...)
{
	va_list list;
	va_start(list, format);
	
	strcpy_s(console_buffer,userthread->getLine().toAscii());
	
	return vsscanf(console_buffer, (const unsigned char *)format, list);
}

void console_mselect(int *row, int *column, QRgb *pix)
{
	return userthread->mselect(row, column, pix);
}

// Request that an image be taken.
// If running in auto-capture mode, return after the next frame capture
// else request that the user snap or load a new picture.
// Blocks until a new picture is acquired.
void console_loadImage()
{
	return userthread->loadImage();
}

void console_processImage()
{
	return userthread->loadImage();
}

void console_abort(char *src)
{
}

void console_exit()
{
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	ConsoleGUI *window=new ConsoleGUI();
	userthread=new UserThread();
	QObject::connect(
		window, SIGNAL(imagesReady(QImage *, QImage *, QImage *, QImage *)), 
		userthread, SLOT(processImages(QImage *, QImage *, QImage *, QImage *)) );
	QObject::connect(userthread, SIGNAL(imagesProcessed()), window, SLOT(refreshPictures()) );
	QObject::connect(
		window, SIGNAL(textEntered(const QString&)), 
		userthread, SLOT(textInput(const QString&)) );
	QObject::connect(
		userthread, SIGNAL(showText(const QString&)),
		window, SLOT(displayLine(const QString&)) );
	QObject::connect(
		window, SIGNAL(mousePressed(int, int, QRgb)),
		userthread, SLOT(mousePress(int, int, QRgb)) );
	
	window->show();

	rhino_init();
	rhino_opencomm(1);

	int qt_status;
	
	qt_status=app.exec();

	return qt_status;
}


/* 
  Magnum 
  Copyright (C) 2002-2006 Kaya Kupferschmidt. All Rights Reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, under the terms in LICENSE.TXT.

  Kaya Kupferschmidt  (k.kupferschmidt@dimajix.de)
*/

/**
 * This is a dirty hack in order to get String::sscanf to work and
 * because there is no vsscanf in Visual C++
 */

/*** Microsoft License for Redistributable Code

i.    Right to Use and Distribute.  The code and text files listed below are 
�Distributable Code.�  
�  You may modify the source code form the C runtimes (CRTs) to design, develop
   and test your programs, and copy and distribute the object code form of your 
   modified files under a new name; and

� Permit distributors of your programs to copy and distribute the Distributable 
   Code as part of those programs.

ii.   Distribution Requirements.  For any Distributable Code you distribute, you 
must:
�  add significant primary functionality to it in your programs;
�  only invoke the software via interfaces described in the software 
    documentation;
�  require distributors and external end users to agree to terms that protect it 
   at least as much as this agreement;
�  display your valid copyright notice on your programs;
�  indemnify, defend, and hold harmless Microsoft from any claims, including 
   attorneys� fees, related to the distribution or use of your programs.

iii.  Distribution Restrictions.  You may not:
�  alter any copyright, trademark or patent notice in the Distributable Code;
�  use Microsoft�s trademarks in your programs� names or in a way that suggests 
   your programs come from or are endorsed by Microsoft;
�  distribute Distributable Code to run on a platform other than the Windows 
   platform; 
�  include Distributable Code in malicious, deceptive or unlawful programs; or
�  modify or distribute the source code of any Distributable Code so that any 
   part of it becomes subject to an Excluded License.  An Excluded License is 
   one that requires, as a condition of use, modification or distribution, that:
      o  the code be disclosed or distributed in source code form, or
      o  others have the right to modify it. 
*/      
      
/***
*input.c - C formatted input, used by scanf, etc.
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines _input() to do formatted input; called from scanf(),
*       etc. functions.  This module defines _cscanf() instead when
*       CPRFLAG is defined.  The file cscanf.c defines that symbol
*       and then includes this file in order to implement _cscanf().
*
*******************************************************************************/


#define ALLOW_RANGE /* allow "%[a-z]"-style scansets */


/* temporary work-around for compiler without 64-bit support */

#ifndef _INTEGRAL_MAX_BITS
#define _INTEGRAL_MAX_BITS  64
#endif  /* _INTEGRAL_MAX_BITS */

#ifdef _MBCS
#undef _MBCS
#endif  /* _MBCS */
#include <tchar.h>

#if !defined (_FILEX_DEFINED) && defined (_WINDOWS_)
/*
 * Variation of FILE type used for the dynamically allocated portion of
 * __piob[]. For single thread, _FILEX is the same as FILE. For multithread
 * models, _FILEX has two fields: the FILE struct and the CRITICAL_SECTION
 * struct used to serialize access to the FILE.
 */
#ifdef _MT
typedef struct {
        FILE f;
        CRITICAL_SECTION lock;
        }   _FILEX;

#else  /* _MT */
typedef FILE    _FILEX;
#endif  /* _MT */
#define _FILEX_DEFINED
#endif  /* !defined (_FILEX_DEFINED) && defined (_WINDOWS_) */


#if defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_IA64)
#define UNALIGNED __unaligned
#else  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_IA64) */
#define UNALIGNED
#endif  /* defined (_M_MRX000) || defined (_M_ALPHA) || defined (_M_PPC) || defined (_M_IA64) */

#define EXCEPTION_EXECUTE_HANDLER       1
#define EXCEPTION_CONTINUE_SEARCH       0
#define EXCEPTION_CONTINUE_EXECUTION    -1

#define _malloc_crt                     malloc
#define _calloc_crt                     calloc
#define _realloc_crt                    realloc
#define _expand_crt                     _expand
#define _free_crt                       free
#define _msize_crt                      _msize

#define ___decimal_point                __decimal_point
static char __decimal_point[] = ".";          /* localized decimal point string */

#define _getc_lk(_stream)       (--(_stream)->_cnt >= 0 ? 0xff & *(_stream)->_ptr++ : _filbuf(_stream))                         /* _MTHREAD_ONLY */
#define _ungetc_lk(_c,_stm)                     ungetc(_c,_stm)             /* _MTHREAD_ONLY */

#define MUL10(x)        ( (((x)<<2) + (x))<<1 )
#define HEXTODEC(chr)   _hextodec(chr)
#define LEFT_BRACKET    ('[' | ('a' - 'A')) /* 'lowercase' version */

#ifdef _UNICODE
static wchar_t __cdecl _hextodec(wchar_t);
#else  /* _UNICODE */
static int __cdecl _hextodec(int);
#endif  /* _UNICODE */

#define INC()           (++charcount, _inc(stream))
#define UN_INC(chr)     (--charcount, _un_inc(chr, stream))
#define EAT_WHITE()     _whiteout(&charcount, stream)

#ifndef _UNICODE
static int __cdecl _inc(FILE *);
static void __cdecl _un_inc(int, FILE *);
static int __cdecl _whiteout(int *, FILE *);
#else  /* _UNICODE */
static wchar_t __cdecl _inc(FILE *);
static void __cdecl _un_inc(wchar_t, FILE *);
static wchar_t __cdecl _whiteout(int *, FILE *);
#endif  /* _UNICODE */


#ifndef _UNICODE
#define _ISDIGIT(chr)   isdigit(chr)
#define _ISXDIGIT(chr)  isxdigit(chr)
#else  /* _UNICODE */
#define _ISDIGIT(chr)   ( !(chr & 0xff00) && isdigit( chr & 0x00ff ) )
#define _ISXDIGIT(chr)  ( !(chr & 0xff00) && isxdigit( chr & 0x00ff ) )
#endif  /* _UNICODE */


#define ASCII       32           /* # of bytes needed to hold 256 bits */

#define SCAN_SHORT     0         /* also for FLOAT */
#define SCAN_LONG      1         /* also for DOUBLE */
#define SCAN_L_DOUBLE  2         /* only for LONG DOUBLE */

#define SCAN_NEAR    0
#define SCAN_FAR     1

#ifndef _UNICODE
#define TABLESIZE    ASCII
#else  /* _UNICODE */
#define TABLESIZE    (ASCII * 256)
#endif  /* _UNICODE */


/***
*int _input(stream, format, arglist), static int input(format, arglist)
*
*Purpose:
*   get input items (data items or literal matches) from the input stream
*   and assign them if appropriate to the items thru the arglist. this
*   function is intended for internal library use only, not for the user
*
*   The _input entry point is for the normal scanf() functions
*   The input entry point is used when compiling for _cscanf() [CPRFLAF
*   defined] and is a static function called only by _cscanf() -- reads from
*   console.
*
*Entry:
*   FILE *stream - file to read from
*   char *format - format string to determine the data to read
*   arglist - list of pointer to data items
*
*Exit:
*   returns number of items assigned and fills in data items
*   returns EOF if error or EOF found on stream before 1st data item matched
*
*Exceptions:
*
*******************************************************************************/

#if defined (_UNICODE)

int __cdecl vwsscanf (
    const wchar_t* str,
    const wchar_t *format,
    va_list arglist
    )
#else  /* defined (_UNICODE) */
int __cdecl vsscanf (
    const char* str,
    const unsigned char *format,
    va_list arglist
    )
#endif  /* defined (_UNICODE) */

{
#ifndef _UNICODE
    char floatstring[_CVTBUFSIZE + 1];   /* ASCII buffer for floats           */
#else  /* _UNICODE */
    wchar_t floatstring[_CVTBUFSIZE + 1];
#endif  /* _UNICODE */

    unsigned long number;               /* temp hold-value                   */
    char *table = NULL;                 /* which chars allowed for %[]       */
    int malloc_flag = 0;                /* is "table" allocated on the heap? */
#if _INTEGRAL_MAX_BITS >= 64   
    unsigned __int64 num64;             /* temp for 64-bit integers          */
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
    void *pointer=NULL;                 /* points to user data receptacle    */
    void *start;                        /* indicate non-empty string         */


#ifdef _UNICODE
    wchar_t *scanptr;                   /* for building "table" data         */
	wchar_t ch = 0;
#else  /* _UNICODE */
    wchar_t wctemp;
    unsigned char *scanptr;             /* for building "table" data         */
	int ch = 0;
#endif  /* _UNICODE */
    int charcount;                      /* total number of chars read        */
	int comchr;                        /* holds designator type             */
    int count;                          /* return value.  # of assignments   */

    int started;                        /* indicate good number              */
    int width;                          /* width of field                    */
    int widthset;                       /* user has specified width          */

/* Neither coerceshort nor farone are need for the 386 */


    char done_flag;                     /* general purpose loop monitor      */
    char longone;                       /* 0 = SHORT, 1 = LONG, 2 = L_DOUBLE */
#if _INTEGRAL_MAX_BITS >= 64   
    int integer64;                      /* 1 for 64-bit integer, 0 otherwise */
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
    signed char widechar;               /* -1 = char, 0 = ????, 1 = wchar_t  */
    char reject;                        /* %[^ABC] instead of %[ABC]         */
    char negative;                      /* flag for '-' detected             */
    char suppress;                      /* don't assign anything             */
    char match;                         /* flag: !0 if any fields matched    */
    va_list arglistsave;                /* save arglist value                */

    char fl_wchar_arg;                  /* flags wide char/string argument   */
#ifdef _UNICODE
    wchar_t rngch;              /* used while scanning range         */
    wchar_t last;               /* also for %[a-z]                   */
    wchar_t prevchar;           /* for %[a-z]                        */
    wchar_t wdecimal;                   /* wide version of decimal point     */
    wchar_t *wptr;                      /* pointer traverses wide floatstring*/
#else  /* _UNICODE */
    unsigned char rngch;                /* used while scanning range         */
    unsigned char last;                 /* also for %[a-z]                   */
    unsigned char prevchar;             /* for %[a-z]                        */
#endif  /* _UNICODE */

#ifdef _MT
    _FILEX strfile;
    FILE *stream = &strfile.f;
	InitializeCriticalSection( &strfile.lock);
#else
    FILE strfile;
    FILE *stream = &strfile;
#endif    
    stream->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
    stream->_ptr = stream->_base = (char *) str;
    stream->_cnt = (int)strlen(str);

    /*
    count = # fields assigned
    charcount = # chars read
    match = flag indicating if any fields were matched

    [Note that we need both count and match.  For example, a field
    may match a format but have assignments suppressed.  In this case,
    match will get set, but 'count' will still equal 0.  We need to
    distinguish 'match vs no-match' when terminating due to EOF.]
    */

    count = charcount = match = 0;

    while (*format) {

        if (_istspace((_TUCHAR)*format)) {

            UN_INC(EAT_WHITE()); /* put first non-space char back */

            while ((_istspace)(*++format)); /* NULL */
            /* careful: isspace macro may evaluate argument more than once! */
            continue;

        }

        if (_T('%') == *format) {

            number = 0;
            prevchar = 0;
            width = widthset = started = 0;
            fl_wchar_arg = done_flag = suppress = negative = reject = 0;
            widechar = 0;

            longone = 1;

            integer64 = 0;

            while (!done_flag) {

                comchr = *++format;
                if (_ISDIGIT((_TUCHAR)comchr)) {
                    ++widthset;
                    width = MUL10(width) + (comchr - _T('0'));
                } else
                    switch (comchr) {
                        case _T('F') :
                        case _T('N') :   /* no way to push NEAR in large model */
                            break;  /* NEAR is default in small model */
                        case _T('h') :
                            /* set longone to 0 */
                            --longone;
                            --widechar;         /* set widechar = -1 */
                            break;

#if _INTEGRAL_MAX_BITS >= 64   
                        case _T('I'):
                            if ( (*(format + 1) == _T('6')) &&
                                 (*(format + 2) == _T('4')) )
                            {
                                format += 2;
                                ++integer64;
                                num64 = 0;
                                break;
                            }
                            else if ( (*(format + 1) == _T('3')) &&
                                      (*(format + 2) == _T('2')) )
                            {
                                format += 2;
                                break;
                            }
                            else if ( (*(format + 1) == _T('d')) ||
                                      (*(format + 1) == _T('i')) ||
                                      (*(format + 1) == _T('o')) ||
                                      (*(format + 1) == _T('x')) ||
                                      (*(format + 1) == _T('X')) )
                            {
                                if (sizeof(void*) == sizeof(__int64))
                                {
                                    ++integer64;
                                    num64 = 0;
                                }
                                break;
                            }
                            if (sizeof(void*) == sizeof(__int64))
                            {
                                    ++integer64;
                                    num64 = 0;
                            }
                            goto DEFAULT_LABEL;
#endif  /* _INTEGRAL_MAX_BITS >= 64    */

                        case _T('L') :
                        /*  ++longone;  */
                            ++longone;
                            break;

                        case _T('l') :
                            ++longone;
                                    /* NOBREAK */
                        case _T('w') :
                            ++widechar;         /* set widechar = 1 */
                            break;

                        case _T('*') :
                            ++suppress;
                            break;

                        default:
DEFAULT_LABEL:
                            ++done_flag;
                            break;
                    }
            }

            if (!suppress) {
                arglistsave = arglist;
                pointer = va_arg(arglist,void *);
            }

            done_flag = 0;

            if (!widechar) {    /* use case if not explicitly specified */
                if ((*format == _T('S')) || (*format == _T('C')))
#ifdef _UNICODE
                    --widechar;
                else
                    ++widechar;
#else  /* _UNICODE */
                    ++widechar;
                else
                    --widechar;
#endif  /* _UNICODE */
            }

            /* switch to lowercase to allow %E,%G, and to
               keep the switch table small */

            comchr = *format | (_T('a') - _T('A'));

            if (_T('n') != comchr)
                if (_T('c') != comchr && LEFT_BRACKET != comchr)
                    ch = EAT_WHITE();
                else
                    ch = INC();


            if (!widthset || width) {

                switch(comchr) {

                    case _T('c'):
                /*  case _T('C'):  */
                        if (!widthset) {
                            ++widthset;
                            ++width;
                        }
                        if (widechar > 0)
                            fl_wchar_arg++;
                        goto scanit;


                    case _T('s'):
                /*  case _T('S'):  */
                        if(widechar > 0)
                            fl_wchar_arg++;
                        goto scanit;


                    case LEFT_BRACKET :   /* scanset */
                        if (widechar>0)
                            fl_wchar_arg++;
                        scanptr = (unsigned char*)(_TCHAR *)(++format);

                        if (_T('^') == *scanptr) {
                            ++scanptr;
                            --reject; /* set reject to 255 */
                        }

                        /* Allocate "table" on first %[] spec */
                        if (table == NULL) {
                            __try {
                                table = (char*)_alloca(TABLESIZE);
                            }
                            __except(EXCEPTION_EXECUTE_HANDLER) {
                                _resetstkoflw();
                                table = (char*)_malloc_crt(TABLESIZE);
                                if ( table == NULL)
                                    goto error_return;
                                malloc_flag = 1;
                            }
                        }

                        memset(table, 0, TABLESIZE);


                        if (LEFT_BRACKET == comchr)
                            if (_T(']') == *scanptr) {
                                prevchar = _T(']');
                                ++scanptr;

                                table[ _T(']') >> 3] = 1 << (_T(']') & 7);

                            }

                        while (_T(']') != *scanptr) {

                            rngch = *scanptr++;

                            if (_T('-') != rngch ||
                                 !prevchar ||           /* first char */
                                 _T(']') == *scanptr) /* last char */

                                table[(prevchar = rngch) >> 3] |= 1 << (rngch & 7);

                            else {  /* handle a-z type set */

                                rngch = *scanptr++; /* get end of range */

                                if (prevchar < rngch)  /* %[a-z] */
                                    last = rngch;
                                else {              /* %[z-a] */
                                    last = prevchar;
                                    prevchar = rngch;
                                }
                                for (rngch = prevchar; rngch <= last; ++rngch)
                                    table[rngch >> 3] |= 1 << (rngch & 7);

                                prevchar = 0;

                            }
                        }


                        if (!*scanptr)
                            goto error_return;      /* trunc'd format string */

                        /* scanset completed.  Now read string */

                        if (LEFT_BRACKET == comchr)
                            format = scanptr;

scanit:
                        start = pointer;

                        /*
                         * execute the format directive. that is, scan input
                         * characters until the directive is fulfilled, eof
                         * is reached, or a non-matching character is
                         * encountered.
                         *
                         * it is important not to get the next character
                         * unless that character needs to be tested! other-
                         * wise, reads from line-buffered devices (e.g.,
                         * scanf()) would require an extra, spurious, newline
                         * if the first newline completes the current format
                         * directive.
                         */
                        UN_INC(ch);

                        while ( !widthset || width-- ) {

                            ch = INC();
                            if (
#ifndef _UNICODE
                                 (EOF != ch) &&
#else  /* _UNICODE */
                                 (WEOF != ch) &&
#endif  /* _UNICODE */
                                   // char conditions
                                 ( ( comchr == _T('c')) ||
                                   // string conditions !isspace()
                                   ( ( comchr == _T('s') &&
                                       (!(ch >= _T('\t') && ch <= _T('\r')) &&
                                       ch != _T(' ')))) ||
                                   // BRACKET conditions
                                   ( (comchr == LEFT_BRACKET) &&
                                     ((table[ch >> 3] ^ reject) & (1 << (ch & 7)))
                                     )
                                   )
                                )
                            {
                                if (!suppress) {
#ifndef _UNICODE
                                    if (fl_wchar_arg) {
                                        char temp[2];
                                        temp[0] = (char) ch;
                                        if (isleadbyte(ch))
                                            temp[1] = (char) INC();
                                        mbtowc(&wctemp, temp, MB_CUR_MAX);
                                        *(wchar_t UNALIGNED *)pointer =
                                          wctemp;
                                        /* do nothing if mbtowc fails */
                                        pointer = (wchar_t *)pointer + 1;
                                    } else
#else  /* _UNICODE */
                                    if (fl_wchar_arg) {
                                        *(wchar_t UNALIGNED *)pointer = ch;
                                        pointer = (wchar_t *)pointer + 1;
                                    } else
#endif  /* _UNICODE */
                                    {
#ifndef _UNICODE
                                    *(char *)pointer = (char)ch;
                                    pointer = (char *)pointer + 1;
#else  /* _UNICODE */
                                    int temp;
                                    /* convert wide to multibyte */
                                    temp = wctomb((char *)pointer, ch);
                                    /* do nothing if wctomb fails */
                                    pointer = (char *)pointer + temp;
#endif  /* _UNICODE */
                                    }
                                } /* suppress */
                                else {
                                    /* just indicate a match */
                                    start = (_TCHAR *)start + 1;
                                }
                            }
                            else  {
                                UN_INC(ch);
                                break;
                            }
                        }

                        /* make sure something has been matched and, if
                           assignment is not suppressed, null-terminate
                           output string if comchr != c */

                        if (start != pointer) {
                            if (!suppress) {
                                ++count;
                                if ('c' != comchr) /* null-terminate strings */
                                    if (fl_wchar_arg)
                                        *(wchar_t UNALIGNED *)pointer = L'\0';
                                    else
                                    *(char *)pointer = '\0';
                            } else /*NULL*/;
                        }
                        else
                            goto error_return;

                        break;

                    case _T('i') :      /* could be d, o, or x */

                        comchr = _T('d'); /* use as default */

                    case _T('x'):

                        if (_T('-') == ch) {
                            ++negative;

                            goto x_incwidth;

                        } else if (_T('+') == ch) {
x_incwidth:
                            if (!--width && widthset)
                                ++done_flag;
                            else
                                ch = INC();
                        }

                        if (_T('0') == ch) {

                            if (_T('x') == (_TCHAR)(ch = INC()) || _T('X') == (_TCHAR)ch) {
                                ch = INC();
                                if (widthset) {
                                    width -= 2;
                                    if (width < 1)
                                        ++done_flag;
                                }
                                comchr = _T('x');
                            } else {
                                ++started;
                                if (_T('x') != comchr) {
                                    if (widthset && !--width)
                                        ++done_flag;
                                    comchr = _T('o');
                                }
                                else {
                                    /* scanning a hex number that starts */
                                    /* with a 0. push back the character */
                                    /* currently in ch and restore the 0 */
                                    UN_INC(ch);
                                    ch = _T('0');
                                }
                            }
                        }
                        goto getnum;

                        /* NOTREACHED */

                    case _T('p') :
                        /* force %hp to be treated as %p */
                        longone = 1;
#ifdef _WIN64
                        /* force %p to be 64 bit in WIN64 */
                        ++integer64;
                        num64 = 0;
#endif  /* _WIN64 */
                    case _T('o') :
                    case _T('u') :
                    case _T('d') :

                        if (_T('-') == ch) {
                            ++negative;

                            goto d_incwidth;

                        } else if (_T('+') == ch) {
d_incwidth:
                            if (!--width && widthset)
                                ++done_flag;
                            else
                                ch = INC();
                        }

getnum:
#if _INTEGRAL_MAX_BITS >= 64   
                        if ( integer64 ) {

                            while (!done_flag) {

                                if (_T('x') == comchr || _T('p') == comchr)

                                    if (_ISXDIGIT(ch)) {
                                        num64 <<= 4;
                                        ch = HEXTODEC(ch);
                                    }
                                    else
                                        ++done_flag;

                                else if (_ISDIGIT(ch))

                                    if (_T('o') == comchr)
                                        if (_T('8') > ch)
                                                num64 <<= 3;
                                        else {
                                                ++done_flag;
                                        }
                                    else /* _T('d') == comchr */
                                        num64 = MUL10(num64);

                                else
                                    ++done_flag;

                                if (!done_flag) {
                                    ++started;
                                    num64 += ch - _T('0');

                                    if (widthset && !--width)
                                        ++done_flag;
                                    else
                                        ch = INC();
                                } else
                                    UN_INC(ch);

                            } /* end of WHILE loop */

                            if (negative)
                                num64 = (unsigned __int64 )(-(__int64)num64);
                        }
                        else {
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
                            while (!done_flag) {

                                if (_T('x') == comchr || _T('p') == comchr)

                                    if (_ISXDIGIT(ch)) {
                                        number = (number << 4);
                                        ch = HEXTODEC(ch);
                                    }
                                    else
                                        ++done_flag;

                                else if (_ISDIGIT(ch))

                                    if (_T('o') == comchr)
                                        if (_T('8') > ch)
                                            number = (number << 3);
                                        else {
                                            ++done_flag;
                                        }
                                    else /* _T('d') == comchr */
                                        number = MUL10(number);

                                else
                                    ++done_flag;

                                if (!done_flag) {
                                    ++started;
                                    number += ch - _T('0');

                                    if (widthset && !--width)
                                        ++done_flag;
                                    else
                                        ch = INC();
                                } else
                                    UN_INC(ch);

                            } /* end of WHILE loop */

                            if (negative)
                                number = (unsigned long)(-(long)number);
#if _INTEGRAL_MAX_BITS >= 64   
                        }
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
                        if (_T('F')==comchr) /* expected ':' in long pointer */
                            started = 0;

                        if (started)
                            if (!suppress) {

                                ++count;
assign_num:
#if _INTEGRAL_MAX_BITS >= 64   
                                if ( integer64 )
                                    *(__int64 UNALIGNED *)pointer = (unsigned __int64)num64;
                                else
#endif  /* _INTEGRAL_MAX_BITS >= 64    */
                                if (longone)
                                    *(long UNALIGNED *)pointer = (unsigned long)number;
                                else
                                    *(short UNALIGNED *)pointer = (unsigned short)number;

                            } else /*NULL*/;
                        else
                            goto error_return;

                        break;

                    case _T('n') :      /* char count, don't inc return value */
                        number = charcount;
                        if(!suppress)
                            goto assign_num; /* found in number code above */
                        break;


                    case _T('e') :
                 /* case _T('E') : */
                    case _T('f') :
                    case _T('g') : /* scan a float */
                 /* case _T('G') : */

#ifndef _UNICODE
                        scanptr = (unsigned char*)floatstring;

                        if (_T('-') == ch) {
                            *scanptr++ = _T('-');
                            goto f_incwidth;

                        } else if (_T('+') == ch) {
f_incwidth:
                            --width;
                            ch = INC();
                        }

                        if (!widthset || width > _CVTBUFSIZE)              /* must watch width */
                            width = _CVTBUFSIZE;


                        /* now get integral part */

                        while (_ISDIGIT(ch) && width--) {
                            ++started;
                            *scanptr++ = (char)ch;
                            ch = INC();
                        }

                        /* now check for decimal */

                        if (*___decimal_point == (char)ch && width--) {
                            ch = INC();
                            *scanptr++ = *___decimal_point;

                            while (_ISDIGIT(ch) && width--) {
                                ++started;
                                *scanptr++ = (char)ch;
                                ch = INC();
                            }
                        }

                        /* now check for exponent */

                        if (started && (_T('e') == ch || _T('E') == ch) && width--) {
                            *scanptr++ = 'e';

                            if (_T('-') == (ch = INC())) {

                                *scanptr++ = '-';
                                goto f_incwidth2;

                            } else if (_T('+') == ch) {
f_incwidth2:
                                if (!width--)
                                    ++width;
                                else
                                    ch = INC();
                            }


                            while (_ISDIGIT(ch) && width--) {
                                ++started;
                                *scanptr++ = (char)ch;
                                ch = INC();
                            }

                        }

                        UN_INC(ch);

                        if (started)
                            if (!suppress) {
                                ++count;
                                *scanptr = '\0';
                                if (longone-1)
									*(double*)pointer = atof(floatstring);
								else
									*(float*)pointer = (float)atof(floatstring);
//                                _fassign( longone-1, pointer , floatstring);
                            } else /*NULL */;
                        else
                            goto error_return;

#else  /* _UNICODE */
                        wptr = floatstring;

                        if (L'-' == ch) {
                            *wptr++ = L'-';
                            goto f_incwidthw;

                        } else if (L'+' == ch) {
f_incwidthw:
                            --width;
                            ch = INC();
                        }

                        if (!widthset || width > _CVTBUFSIZE)
                            width = _CVTBUFSIZE;


                        /* now get integral part */

                        while (_ISDIGIT(ch) && width--) {
                            ++started;
                            *wptr++ = ch;
                            ch = INC();
                        }

                        /* now check for decimal */

                        /* convert decimal point to wide-char */
                        /* assume result is single wide-char */
                        mbtowc (&wdecimal, ___decimal_point, MB_CUR_MAX);

                        if (wdecimal == ch && width--) {
                            ch = INC();
                            *wptr++ = wdecimal;

                            while (_ISDIGIT(ch) && width--) {
                                ++started;
                                *wptr++ = ch;
                                ch = INC();
                            }
                        }

                        /* now check for exponent */

                        if (started && (L'e' == ch || L'E' == ch) && width--) {
                            *wptr++ = L'e';

                            if (L'-' == (ch = INC())) {

                                *wptr++ = L'-';
                                goto f_incwidth2w;

                            } else if (L'+' == ch) {
f_incwidth2w:
                                if (!width--)
                                    ++width;
                                else
                                    ch = INC();
                            }


                            while (_ISDIGIT(ch) && width--) {
                                ++started;
                                *wptr++ = ch;
                                ch = INC();
                            }

                        }

                        UN_INC(ch);

                        if (started)
                            if (!suppress) {
                                ++count;
                                *wptr = '\0';
                                {
                                /* convert floatstring to char string */
                                /* and do the conversion */
                                size_t cfslength;
                                char *cfloatstring;
                                cfslength =(size_t)(wptr-floatstring+1)*sizeof(wchar_t);
                                if ((cfloatstring = (char *)_malloc_crt (cfslength)) == NULL)
                                    goto error_return;
                                wcstombs (cfloatstring, floatstring, cfslength);
                                if (longone-1)
									*(double*)pointer = atof(floatstring);
								else
									*(float*)pointer = (float)atof(floatstring);
//                                _fassign( longone-1, pointer , cfloatstring);
                                _free_crt (cfloatstring);
                                }
                            } else /*NULL */;
                        else
                            goto error_return;

#endif  /* _UNICODE */
                        break;


                    default:    /* either found '%' or something else */

                        if ((int)*format != (int)ch) {
                            UN_INC(ch);
                            goto error_return;
                            }
                        else
                            match--; /* % found, compensate for inc below */

                        if (!suppress)
                            arglist = arglistsave;

                } /* SWITCH */

                match++;        /* matched a format field - set flag */

            } /* WHILE (width) */

            else {  /* zero-width field in format string */
                UN_INC(ch);  /* check for input error */
                goto error_return;
            }

            ++format;  /* skip to next char */

        } else  /*  ('%' != *format) */
            {

            if ((int)*format++ != (int)(ch = INC()))
                {
                UN_INC(ch);
                goto error_return;
                }
#ifndef _UNICODE
            if (isleadbyte(ch))
                {
                int ch2;
                if ((int)*format++ != (ch2=INC()))
                    {
                    UN_INC(ch2);
                    UN_INC(ch);
                    goto error_return;
                    }

                    --charcount; /* only count as one character read */
                }
#endif  /* _UNICODE */
            }

        if ( (EOF == ch) && ((*format != '%') || (*(format + 1) != 'n')) )
            break;

    }  /* WHILE (*format) */

error_return:
#ifdef _MT
	DeleteCriticalSection( &strfile.lock);
#endif	
    if (malloc_flag == 1)
        _free_crt(table);

    if (EOF == ch)
        /* If any fields were matched or assigned, return count */
        return ( (count || match) ? count : EOF);
    else
        return count;

}

/* _hextodec() returns a value of 0-15 and expects a char 0-9, a-f, A-F */
/* _inc() is the one place where we put the actual getc code. */
/* _whiteout() returns the first non-blank character, as defined by isspace() */

#ifndef _UNICODE
static int __cdecl _hextodec (
    int chr
    )
{
    return _ISDIGIT(chr) ? chr : (chr & ~(_T('a') - _T('A'))) - _T('A') + 10 + _T('0');
}
#else  /* _UNICODE */
static _TCHAR __cdecl _hextodec (
    _TCHAR chr
    )
{
    if (_ISDIGIT(chr))
        return chr;
    if (_istlower(chr))
        return (_TCHAR)(chr - _T('a') + 10 + _T('0'));
    else
        return (_TCHAR)(chr - _T('A') + 10 + _T('0'));
}
#endif  /* _UNICODE */


#ifdef _UNICODE
/*
 * Manipulate wide-chars in a file.
 * A wide-char is hard-coded to be two chars for efficiency.
 */

static wchar_t __cdecl _inc (FILE *fileptr
    )
{
    return(_getwc_lk(fileptr));
}

static void __cdecl _un_inc (
    wchar_t chr,
    FILE *fileptr
    )
{
    if (WEOF != chr)
        _ungetwc_lk(chr, fileptr);
}

static wchar_t __cdecl _whiteout (
    int *counter,
    FILE *fileptr
    )
{
    wchar_t ch;

    while((iswspace)(ch = (++*counter, _inc(fileptr))));
    return ch;
}

#else  /* _UNICODE */

static int __cdecl _inc (
    FILE *fileptr
    )
{
    return(_getc_lk(fileptr));
}

static void __cdecl _un_inc (
    int chr,
    FILE *fileptr
    )
{
    if (EOF != chr)
        _ungetc_lk(chr, fileptr);
}

static int __cdecl _whiteout (
    int *counter,
    FILE *fileptr
    )
{
    int ch;

    while((_istspace)(ch = (++*counter, _inc(fileptr))));
    return ch;
}

#endif  /* _UNICODE */
