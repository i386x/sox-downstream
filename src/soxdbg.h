/*
 *! \file    ./tools/soxdbg/soxdbg.h
 *! \author  Jiří Kučera, <jkucera AT redhat.com>
 *! \stamp   2018-01-20 18:39:58 (UTC+01:00, DST+00:00)
 *! \project SoX (Sound eXchange library) maintenance
 *! \license MIT
 *! \version 0.0.0
 *! \fdesc   Macros for watching what is SoX doing.
 *
 */

#ifndef SOX_MAINT_TOOLS_SOXDBG_SOXDBG_H
#define SOX_MAINT_TOOLS_SOXDBG_SOXDBG_H

#define soxdbg_noop() do {} while (0)

#ifdef SOXDBG_ALLOWED
#  define soxdbg_fenter() fprintf(stderr, "Entering %s\n", __func__)
#  define soxdbg_fleave() fprintf(stderr, "Leaving %s\n", __func__)
#  define soxdbg_exit(c) fprintf(stderr, "Exiting %s with %d\n", __func__, c)
#  define soxdbg_show_var(fmt, name) fprintf(stderr, \
     "[In <%s> at %s:%d]: %s = " fmt "\n", \
     __FILE__, __func__, __LINE__, #name, name \
   )
#  define soxdbg_show_cvar(name) do { \
     int c = (int)(name); \
     const char *sgn = (c < 0) ? "-" : ""; \
 \
     fprintf(stderr, \
       "[In <%s> at %s:%d]: %s = ", \
       __FILE__, __func__, __LINE__, #name \
     ); \
     if (c < 0) \
       c = -c; \
     switch (c) { \
       case 0: \
         fprintf(stderr, "'%s\\0'\n", sgn); \
         break; \
       case 7: \
         fprintf(stderr, "'%s\\a'\n", sgn); \
         break; \
       case 8: \
         fprintf(stderr, "'%s\\b'\n", sgn); \
         break; \
       case 9: \
         fprintf(stderr, "'%s\\t'\n", sgn); \
         break; \
       case 10: \
         fprintf(stderr, "'%s\\n'\n", sgn); \
         break; \
       case 11: \
         fprintf(stderr, "'%s\\v'\n", sgn); \
         break; \
       case 12: \
         fprintf(stderr, "'%s\\f'\n", sgn); \
         break; \
       case 13: \
         fprintf(stderr, "'%s\\r'\n", sgn); \
         break; \
       default: \
         if (32 <= c && c <= 127) \
           fprintf(stderr, "'%s%c'\n", sgn, (char)c); \
         else if (0 <= c && c <= 255) \
           fprintf(stderr, "'%s\\x%02X'\n", sgn, c); \
         else \
           fprintf(stderr, "'%s\\x%08X'\n", sgn, c); \
         break; \
     } \
   } while (0)
#else
#  define soxdbg_fenter() soxdbg_noop()
#  define soxdbg_fleave() soxdbg_noop()
#  define soxdbg_exit(c) soxdbg_noop()
#  define soxdbg_show_var(fmt, name) soxdbg_noop()
#  define soxdbg_show_cvar(name) soxdbg_noop()
#endif

#define soxdbg_show_char(name) soxdbg_show_cvar(name)
#define soxdbg_show_charc(name) soxdbg_show_var("%d", (int)(name))
#define soxdbg_show_ucharc(name) soxdbg_show_var("%u", (unsigned)(name))
#define soxdbg_show_short(name) soxdbg_show_var("%d", (int)(name))
#define soxdbg_show_ushort(name) soxdbg_show_var("%u", (unsigned)(name))
#define soxdbg_show_int(name) soxdbg_show_var("%d", (name))
#define soxdbg_show_uint(name) soxdbg_show_var("%u", (name))

#endif
