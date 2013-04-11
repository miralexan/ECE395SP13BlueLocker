#ifndef __FLAGS_H__
#define __FLAGS_H__

typedef struct{
	char EOB;		// End Of Buffer
	char BOF;		// Buffer OverFlow
} flags_t flags;

#define init_flags(p) ((p).EOB = 0; (p).BOF = 0);

#endif