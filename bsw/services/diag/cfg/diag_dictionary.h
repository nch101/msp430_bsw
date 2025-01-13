#ifndef DIAG_DICTIONARY_H
#define DIAG_DICTIONARY_H

#define SECURITY_ACCESS                 0x27U
#define KEEPALIVE_SESSION               0x3EU
#define CONTROL_ERROR_CODE_SETTINGS     0x85U
#define END_DIAGNOSTIC_SESSION          0x40U

#define CLEAR_ERROR_CODE                0x14U
#define READ_ERROR_CODE                 0x19U
#define READ_DATA_BY_ID                 0x22U
#define WRITE_DATA_BY_ID                0x2EU

#define INPUT_OUTPUT_CONTROL            0x2FU

#define NEGATIVE_RESPONSE               0x7FU
#define GENERAL_REJECT                  0x10U
#define SERVICE_NOT_SUPPORTED           0x11U
#define SUB_FUNCTION_NOT_SUPPORTED      0x12U
#define REQUEST_SEQUENCE_ERROR          0x24U
#define SECURITY_ACCESS_DENIED          0x33U
#define INVALID_KEY                     0x35U
#define EXCEEDED_NUMBER_OF_ATTEMPTS     0x36U
#define PROGRAMMING_FAILURE             0x72U


#endif /* DIAG_DICTIONARY_H */
