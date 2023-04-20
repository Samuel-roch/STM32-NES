/*
 * ReturnCode.h
 *
 *  Created on: Apr 22, 2022
 *      Author: Samuel
 */

#ifndef INC_RETURNCODE_H_
#define INC_RETURNCODE_H_

/* This header file contains generic return codes, designed to fit the needs  */
/*  from all projects, so that all of them can use these values to represent  */
/*  their information.                                                        */
/* It consists of an enumeration, where each value has its fixed code. The    */
/*  return values can represent 'error' or 'information' values. There is an  */
/*  item in the enumeration that separates them.                              */

typedef enum
{
	/***** INFORMATION TYPE RETURN LIST *******************************************/
	/* NAME --------------------------- VALUE ---- DESCRIPTION                    */
	ANSWERED_REQUEST                = 0x0000, /* Usual answer. Conclusion.      */
	OPERATION_IDLE                  = 0x0001, /* No specific operation is running.                              */
	OPERATION_RUNNING               = 0x0002, /* Routine is busy with its task or with the caller-related task. */
	FUNCTION_BUSY                   = 0x0003, /* Routine is busy with a task related with another caller.       */
	/******** TYPE DELIMITER ITEM *************************************************/
	RETURN_ERROR_VALUE              = 0x8000, /* From this value onwards the answer is of error type.           */
	/******* ERROR TYPE RETURN LIST ***********************************************/
	ERR_VALUE                       = 0x8001, /* Parameter of incorrect value.                                  */
	ERR_TIMEOUT                     = 0x8002, /* Timer overflow.                                                */
	ERR_CRC                         = 0x8003, /* CRC error is detected.                                         */
	ERR_BUSY                        = 0x8004, /* Device is busy.                                                */
	ERR_FAILED                      = 0x8005, /* Requested functionality or process failed.                     */
	ERR_DEVICE                      = 0x8006, /* Error with target device.                                      */

} ReturnCode_t;

#endif /* INC_RETURNCODE_H_ */
