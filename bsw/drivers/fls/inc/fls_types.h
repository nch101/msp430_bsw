#ifndef FLS_TYPES_H
#define FLS_TYPES_H

#if (BSW_CFG_FLS_FUNCTION == STD_ENABLED)

#define FLS_SET_ERASE_BIT()                 FCTL1 = FWKEY + ERASE
#define FLS_CLEAR_LOCK_BIT()                FCTL3 = FWKEY
#define FLS_SET_WRITE_BIT()                 FCTL1 = FWKEY + BLKWRT + WRT
#define FLS_SET_LOCK_BIT()                  FCTL3 = FWKEY + LOCK
#define FLS_CLEAR_WRITE_BIT()               FCTL3 = FWKEY
#define FLS_WAIT_WRITE_DONE()               while ((FCTL3 & WAIT) == 0U) {;}

#define FLS_MAX_SEGMENT_SIZE                64U

#endif /* (BSW_CFG_FLS_FUNCTION == STD_ENABLED) */
#endif /* FLS_TYPES_H */
