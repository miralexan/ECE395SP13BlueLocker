#ifndef __FLAGS_H__
#define __FLAGS_H__

typedef char flags;

#define init_flags(p) ((p) = 0)

#define eob(p) ((p) & 0x01)
#define bof(p) ((p) & 0x02)

#endif
