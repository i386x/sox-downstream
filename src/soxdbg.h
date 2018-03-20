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
#  define __STDC_FORMAT_MACROS
#  include <inttypes.h>
#  define soxdbg_fprintf fprintf
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
#  define soxdbg_show_bool(b) do { \
     static const char *names[] = { \
       "false", "true" \
     }; \
 \
     fprintf(stderr, "%s", names[b]); \
   } while (0);
#  define soxdbg_show_option(o) do { \
     static const char *names[] = { \
       "no", "yes", "default" \
     }; \
 \
     fprintf(stderr, "%s", names[o]); \
   } while (0)
#  define soxdbg_show_encoding(e) do { \
     static const char *names[] = { \
       "SOX_ENCODING_UNKNOWN", \
       "SOX_ENCODING_SIGN2", \
       "SOX_ENCODING_UNSIGNED", \
       "SOX_ENCODING_FLOAT", \
       "SOX_ENCODING_FLOAT_TEXT", \
       "SOX_ENCODING_FLAC", \
       "SOX_ENCODING_HCOM", \
       "SOX_ENCODING_WAVPACK", \
       "SOX_ENCODING_WAVPACKF", \
       "SOX_ENCODING_ULAW", \
       "SOX_ENCODING_ALAW", \
       "SOX_ENCODING_G721", \
       "SOX_ENCODING_G723", \
       "SOX_ENCODING_CL_ADPCM", \
       "SOX_ENCODING_CL_ADPCM16", \
       "SOX_ENCODING_MS_ADPCM", \
       "SOX_ENCODING_IMA_ADPCM", \
       "SOX_ENCODING_OKI_ADPCM", \
       "SOX_ENCODING_DPCM", \
       "SOX_ENCODING_DWVW", \
       "SOX_ENCODING_DWVWN", \
       "SOX_ENCODING_GSM", \
       "SOX_ENCODING_MP3", \
       "SOX_ENCODING_VORBIS", \
       "SOX_ENCODING_AMR_WB", \
       "SOX_ENCODING_AMR_NB", \
       "SOX_ENCODING_CVSD", \
       "SOX_ENCODING_LPC10", \
       "SOX_ENCODING_OPUS", \
       "SOX_ENCODINGS" \
     }; \
 \
     fprintf(stderr, "%s", names[e]); \
   } while (0)
#  define soxdbg_show_io_type(t) do { \
     static const char *names[] = { \
       "file", "pipe", "url" \
     }; \
 \
     fprintf(stderr, "%s", names[t]); \
   } while (0)
#  define soxdbg_show_comments(c) do { \
     char **p = (c); \
 \
     if (p == NULL) \
       break; \
     while (*p++) \
       fprintf( \
         stderr, \
         "  [COMMENT]: %s%s%s\n", \
         *p ? "\"" : "", \
         *p ? *p : "NULL", \
         *p ? "\"" : "" \
       ); \
   } while (0)
#  define soxdbg_show_loops(l, n) do { \
     sox_loopinfo_t *loops = (l); \
     unsigned limit = (n), i = 0; \
 \
     if (loops == NULL) \
       break; \
     for (i = 0; i < limit; i++) \
       fprintf( \
         stderr, \
         "  [LOOP #%u]: " \
         "start = %" PRIu64 ";\t" \
         "length = %" PRIu64 ";\t" \
         "count = %u;\t" \
         "type = %d;\n", \
         i, loops[i].start, loops[i].length, loops[i].count, loops[i].type \
       ); \
   } while (0)
#  define soxdbg_show_format_handler_names(x) do { \
     char const * const *p = (x); \
     int i = 0; \
 \
     if (p == NULL) \
       break; \
     for (i = 0; p[i] != NULL; i++) \
       fprintf(stderr, "  [FEXT]: \"%s\"\n", p[i]); \
   } while (0)
#  define soxdbg_show_write_formats(f) do { \
     unsigned const *formats = (f); \
     int i = 0, j = 0; \
 \
     if (formats == NULL) \
       break; \
     while (1) { \
       if (formats[i] == 0) \
         break; \
       fprintf(stderr, "  [WFMT #%d]: ", j); \
       j++; \
       soxdbg_show_encoding(formats[i]); \
       i++; \
       while (formats[i] != 0) \
         fprintf(stderr, ", %u", formats[i]), ++i; \
       i++; \
       fprintf(stderr, "\n"); \
     } \
   } while (0)
#  define soxdbg_show_write_rates(r) do { \
     sox_rate_t const *rates = (r); \
     int i = 0; \
 \
     if (rates == NULL) { \
       fprintf(stderr, " <all write rates are supported>\n"); \
       break; \
     } \
     for (i = 0; rates[i] != 0.0; i++) { \
       if (i > 0) \
         fprintf(stderr, ","); \
       fprintf(stderr, " %f", rates[i]); \
     } \
     fprintf(stderr, "\n"); \
   } while (0)
#  define soxdbg_show_hcom_dict(d) do { \
     dictent *p = (d); \
     int i = 0; \
 \
     if (p == NULL) \
       fprintf(stderr, "  NULL\n"); \
     else \
       for (i = 0; i < 511; i++) \
         fprintf( \
           stderr, \
           "  [DI #%d]: %ld\t%d\t%d\n", \
           i, p[i].frequ, (int)(p[i].dict_leftson), (int)(p[i].dict_rightson) \
         ); \
   } while (0)
#  define soxdbg_show_hcom_data(p) do { \
     unsigned char *data = (p).data; \
     size_t size = (p).size; \
     size_t i = 0; \
 \
     if (data == NULL) \
       fprintf(stderr, "  NULL\n"); \
     else if (size == 0) \
       fprintf(stderr, "  no data\n"); \
     else { \
       fprintf( \
         stderr, \
         "  XXXXXXXX: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n" \
       ); \
       for (i = 0; i < size; i++) { \
         if ((i & 15) == 0) \
           fprintf(stderr, "  %08zX:", i); \
         fprintf(stderr, " %02X", (unsigned int)data[i]); \
         if (i == size - 1 || (i & 15) == 15) \
           fprintf(stderr, "\n"); \
       } \
     } \
   } while (0)
#else
#  define soxdbg_fprintf(f, ...) soxdbg_noop()
#  define soxdbg_fenter() soxdbg_noop()
#  define soxdbg_fleave() soxdbg_noop()
#  define soxdbg_exit(c) soxdbg_noop()
#  define soxdbg_show_var(fmt, name) soxdbg_noop()
#  define soxdbg_show_cvar(name) soxdbg_noop()
#  define soxdbg_show_bool(b) soxdbg_noop()
#  define soxdbg_show_option(o) soxdbg_noop()
#  define soxdbg_show_encoding(e) soxdbg_noop()
#  define soxdbg_show_io_type(t) soxdbg_noop()
#  define soxdbg_show_comments(c) soxdbg_noop()
#  define soxdbg_show_loops(l, n) soxdbg_noop()
#  define soxdbg_show_format_handler_names(x) soxdbg_noop()
#  define soxdbg_show_write_formats(f) soxdbg_noop()
#  define soxdbg_show_write_rates(r) soxdbg_noop()
#  define soxdbg_show_hcom_dict(d) soxdbg_noop()
#  define soxdbg_show_hcom_data(p) soxdbg_noop()
#endif

#define soxdbg_show_char(name) soxdbg_show_cvar(name)
#define soxdbg_show_charc(name) soxdbg_show_var("%d", (int)(name))
#define soxdbg_show_ucharc(name) soxdbg_show_var("%u", (unsigned)(name))
#define soxdbg_show_short(name) soxdbg_show_var("%d", (int)(name))
#define soxdbg_show_ushort(name) soxdbg_show_var("%u", (unsigned)(name))
#define soxdbg_show_int(name) soxdbg_show_var("%d", (name))
#define soxdbg_show_uint(name) soxdbg_show_var("%u", (name))

#define soxdbg_show_format_t(x) do { \
   soxdbg_fprintf(stderr, "[[[SHOWING INFO ABOUT FORMAT_T]]]\n"); \
   soxdbg_fprintf(stderr, "- filename: \"%s\"\n", (x).filename); \
   soxdbg_fprintf(stderr, "- signal.rate: %f\n", (x).signal.rate); \
   soxdbg_fprintf(stderr, "- signal.channels: %u\n", (x).signal.channels); \
   soxdbg_fprintf(stderr, "- signal.precision: %u\n", (x).signal.precision); \
   soxdbg_fprintf( \
     stderr, "- signal.length: %" PRIu64 "\n", (x).signal.length \
   ); \
   soxdbg_fprintf(stderr, "- signal.mult: "); \
   if ((x).signal.mult == NULL) \
     soxdbg_fprintf(stderr, "unused\n"); \
   else \
     soxdbg_fprintf(stderr, "%f\n", *((x).signal.mult)); \
   soxdbg_fprintf(stderr, "- encoding.encoding: "); \
   soxdbg_show_encoding((x).encoding.encoding); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).encoding.encoding); \
   soxdbg_fprintf( \
     stderr, "- encoding.bits_per_sample: %u\n", (x).encoding.bits_per_sample \
   ); \
   soxdbg_fprintf( \
     stderr, "- encoding.compression: %f\n", (x).encoding.compression \
   ); \
   soxdbg_fprintf(stderr, "- encoding.reverse_bytes: "); \
   soxdbg_show_option((x).encoding.reverse_bytes); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).encoding.reverse_bytes); \
   soxdbg_fprintf(stderr, "- encoding.reverse_nibbles: "); \
   soxdbg_show_option((x).encoding.reverse_nibbles); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).encoding.reverse_nibbles); \
   soxdbg_fprintf(stderr, "- encoding.reverse_bits: "); \
   soxdbg_show_option((x).encoding.reverse_bits); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).encoding.reverse_bits); \
   soxdbg_fprintf(stderr, "- encoding.opposite_endian: "); \
   soxdbg_show_bool((x).encoding.opposite_endian); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).encoding.opposite_endian); \
   soxdbg_fprintf(stderr, "- filetype: \"%s\"\n", (x).filetype); \
   soxdbg_fprintf(stderr, "- oob.comments:\n"); \
   soxdbg_show_comments((x).oob.comments); \
   soxdbg_fprintf( \
     stderr, "- oob.instr.MIDInote: %d\n", (int)((x).oob.instr.MIDInote) \
   ); \
   soxdbg_fprintf( \
     stderr, "- oob.instr.MIDIlow: %d\n", (int)((x).oob.instr.MIDIlow) \
   ); \
   soxdbg_fprintf( \
     stderr, "- oob.instr.MIDIhi: %d\n", (int)((x).oob.instr.MIDIhi) \
   ); \
   soxdbg_fprintf( \
     stderr, "- oob.instr.loopmode: %d\n", (int)((x).oob.instr.loopmode) \
   ); \
   soxdbg_fprintf( \
     stderr, "- oob.instr.nloops: %u\n", (x).oob.instr.nloops \
   ); \
   soxdbg_fprintf(stderr, "- oob.loops:\n"); \
   soxdbg_show_loops((x).oob.loops, (x).oob.instr.nloops); \
   soxdbg_fprintf(stderr, "- seekable: "); \
   soxdbg_show_bool((x).seekable); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).seekable); \
   soxdbg_fprintf(stderr, "- mode: '%c'\n", (x).mode); \
   soxdbg_fprintf(stderr, "- olength: %" PRIu64 "\n", (x).olength); \
   soxdbg_fprintf(stderr, "- clips: %" PRIu64 "\n", (x).clips); \
   soxdbg_fprintf(stderr, "- sox_errno: %d\n", (x).sox_errno); \
   soxdbg_fprintf(stderr, "- sox_errstr: \"%s\"\n", (x).sox_errstr); \
   soxdbg_fprintf(stderr, "- fp: %p\n", (x).fp); \
   soxdbg_fprintf(stderr, "- io_type: "); \
   soxdbg_show_io_type((x).io_type); \
   soxdbg_fprintf(stderr, "(%d)\n", (x).io_type); \
   soxdbg_fprintf(stderr, "- tell_off: %" PRIu64 "\n", (x).tell_off); \
   soxdbg_fprintf(stderr, "- data_start: %" PRIu64 "\n", (x).data_start); \
   soxdbg_fprintf( \
     stderr, \
     "- handler.sox_lib_version_code: %u\n", \
     (x).handler.sox_lib_version_code \
   ); \
   soxdbg_fprintf( \
     stderr, "- handler.description: \"%s\"\n", (x).handler.description \
   ); \
   soxdbg_fprintf(stderr, "- handler.names:\n"); \
   soxdbg_show_format_handler_names((x).handler.names); \
   soxdbg_fprintf(stderr, "- handler.flags: %08X\n", (x).handler.flags); \
   soxdbg_fprintf( \
     stderr, "- handler.startread: %p\n", (x).handler.startread \
   ); \
   soxdbg_fprintf(stderr, "- handler.read: %p\n", (x).handler.read); \
   soxdbg_fprintf(stderr, "- handler.stopread: %p\n", (x).handler.stopread); \
   soxdbg_fprintf( \
     stderr, "- handler.startwrite: %p\n", (x).handler.startwrite \
   ); \
   soxdbg_fprintf(stderr, "- handler.write: %p\n", (x).handler.write); \
   soxdbg_fprintf( \
     stderr, "- handler.stopwrite: %p\n", (x).handler.stopwrite \
   ); \
   soxdbg_fprintf(stderr, "- handler.seek: %p\n", (x).handler.seek); \
   soxdbg_fprintf(stderr, "- handler.write_formats:\n"); \
   soxdbg_show_write_formats((x).handler.write_formats); \
   soxdbg_fprintf(stderr, "- handler.write_rates:"); \
   soxdbg_show_write_rates((x).handler.write_rates); \
   soxdbg_fprintf( \
     stderr, "- handler.priv_size: %zu\n", (x).handler.priv_size \
   ); \
   soxdbg_fprintf(stderr, "- priv: %p\n", (x).priv); \
   soxdbg_fprintf(stderr, "[[[/SHOWING INFO ABOUT FORMAT_T]]]\n"); \
 } while(0)

#define soxdbg_show_hcom_priv(p) do {
   soxdbg_fprintf(stderr, "[[[SHOWING HCOM PRIVATE DATA]]]\n");
   soxdbg_fprintf(stderr, "- dictionary:\n");
   soxdbg_show_hcom_dict((p).dictionary);
   soxdbg_fprintf(stderr, "- checksum: %" PRId32 "\n", (p).checksum);
   soxdbg_fprintf(stderr, "- deltacompression: %d\n", (p).deltacompression);
   soxdbg_fprintf(stderr, "- huffcount: %ld\n", (p).huffcount);
   soxdbg_fprintf(stderr, "- cksum: %ld\n", (p).cksum);
   soxdbg_fprintf(stderr, "- dictentry: %d\n", (p).dictentry);
   soxdbg_fprintf(stderr, "- nrbits: %d\n", (p).nrbits);
   soxdbg_fprintf(stderr, "- current: %" PRIu32 "\n", (p).current);
   soxdbg_fprintf(stderr, "- sample: %d\n", (int)((p).sample));
   soxdbg_fprintf(stderr, "- de:\n");
   soxdbg_show_hcom_dict((p).de);
   soxdbg_fprintf(stderr, "- new_checksum: %" PRId32 "\n", (p).new_checksum);
   soxdbg_fprintf(stderr, "- nbits: %d\n", (p).nbits);
   soxdbg_fprintf(stderr, "- curword: %" PRId32 "\n", (p).curword);
   soxdbg_fprintf(stderr, "- data:\n");
   soxdbg_show_hcom_data(p);
   soxdbg_fprintf(stderr, "- size: %zu\n", (p).size);
   soxdbg_fprintf(stderr, "- pos: %zu\n", (p).pos);
   soxdbg_fprintf(stderr, "[[[/SHOWING HCOM PRIVATE DATA]]]\n");
 } while (0)

#endif
