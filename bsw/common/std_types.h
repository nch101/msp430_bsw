#ifndef STD_TYPES_H
#define STD_TYPES_H

#define STD_OFF                         0U
#define STD_ON                          1U

#define STD_DISABLED                    0U
#define STD_ENABLED                     1U

#ifndef NULL
#define NULL            ((void *) 0u)
#endif /* NULL */

#ifndef TRUE
#define TRUE            1U
#endif /* TRUE */

#ifndef FALSE
#define FALSE           0U
#endif /* FALSE */

typedef unsigned char   uint8;
typedef signed char     sint8;
typedef unsigned short  uint16;
typedef signed short    sint16;
typedef unsigned int    uint32;
typedef signed int      sint32;

typedef enum
{
    STD_NOT_OK,
    STD_OK
} Std_StatusType;

#endif /* STD_TYPES_H */
