/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : RNG1.h
**     Project     : FRDM-K64F_lwip_mqtt_bm
**     Processor   : MK64FN1M0VLL12
**     Component   : RingBuffer
**     Version     : Component 01.053, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-03-03, 06:44, # CodeGen: 0
**     Abstract    :
**         This component implements a ring buffer for different integer data type.
**     Settings    :
**          Component name                                 : RNG1
**          Buffer Size                                    : 4096
**     Contents    :
**         Clear           - void RNG1_Clear(void);
**         Put             - uint8_t RNG1_Put(RNG1_ElementType elem);
**         Get             - uint8_t RNG1_Get(RNG1_ElementType *elemP);
**         Peek            - uint8_t RNG1_Peek(RNG1_BufSizeType index, RNG1_ElementType *elemP);
**         Update          - uint8_t RNG1_Update(RNG1_BufSizeType index, RNG1_ElementType *elemP);
**         Putn            - uint8_t RNG1_Putn(RNG1_ElementType *elem, RNG1_BufSizeType nof);
**         Getn            - uint8_t RNG1_Getn(RNG1_ElementType *buf, RNG1_BufSizeType nof);
**         Compare         - uint8_t RNG1_Compare(RNG1_BufSizeType index, RNG1_ElementType *elemP,...
**         Delete          - uint8_t RNG1_Delete(void);
**         NofElements     - RNG1_BufSizeType RNG1_NofElements(void);
**         NofFreeElements - RNG1_BufSizeType RNG1_NofFreeElements(void);
**         Deinit          - void RNG1_Deinit(void);
**         Init            - void RNG1_Init(void);
**
** * Copyright (c) 2014-2018, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file RNG1.h
** @version 01.00
** @brief
**         This component implements a ring buffer for different integer data type.
*/         
/*!
**  @addtogroup RNG1_module RNG1 module documentation
**  @{
*/         

/* MODULE RNG1. */

#include "RNG1.h"

#if RNG1_CONFIG_REENTRANT
  #define RNG1_DEFINE_CRITICAL() CS1_CriticalVariable()
  #define RNG1_ENTER_CRITICAL()  CS1_EnterCritical()
  #define RNG1_EXIT_CRITICAL()   CS1_ExitCritical()
#else
  #define RNG1_DEFINE_CRITICAL() /* nothing */
  #define RNG1_ENTER_CRITICAL()  /* nothing */
  #define RNG1_EXIT_CRITICAL()   /* nothing */
#endif
static RNG1_ElementType RNG1_buffer[RNG1_CONFIG_BUF_SIZE]; /* ring buffer */
static RNG1_BufSizeType RNG1_inIdx;  /* input index */
static RNG1_BufSizeType RNG1_outIdx; /* output index */
static RNG1_BufSizeType RNG1_inSize; /* size data in buffer */
/*
** ===================================================================
**     Method      :  Put (component RingBuffer)
**
**     Description :
**         Puts a new element into the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**         elem            - New element to be put into the buffer
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Put(RNG1_ElementType elem)
{
  uint8_t res = ERR_OK;
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (RNG1_inSize==RNG1_CONFIG_BUF_SIZE) {
    res = ERR_TXFULL;
  } else {
    RNG1_buffer[RNG1_inIdx] = elem;
    RNG1_inIdx++;
    if (RNG1_inIdx==RNG1_CONFIG_BUF_SIZE) {
      RNG1_inIdx = 0;
    }
    RNG1_inSize++;
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  Putn (component RingBuffer)
**
**     Description :
**         Put a number new element into the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * elem            - Pointer to new elements to be put into
**                           the buffer
**         nof             - number of elements
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Putn(RNG1_ElementType *elem, RNG1_BufSizeType nof)
{
  uint8_t res = ERR_OK;

  while(nof>0) {
    res = RNG1_Put(*elem);
    if (res!=ERR_OK) {
      break;
    }
    elem++; nof--;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  Get (component RingBuffer)
**
**     Description :
**         Removes an element from the buffer
**     Parameters  :
**         NAME            - DESCRIPTION
**       * elemP           - Pointer to where to store the received
**                           element
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Get(RNG1_ElementType *elemP)
{
  uint8_t res = ERR_OK;
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (RNG1_inSize==0) {
    res = ERR_RXEMPTY;
  } else {
    *elemP = RNG1_buffer[RNG1_outIdx];
    RNG1_inSize--;
    RNG1_outIdx++;
    if (RNG1_outIdx==RNG1_CONFIG_BUF_SIZE) {
      RNG1_outIdx = 0;
    }
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  Getn (component RingBuffer)
**
**     Description :
**         Get a number elements into a buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * buf             - Pointer to buffer where to store the
**                           elements
**         nof             - number of elements
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Getn(RNG1_ElementType *buf, RNG1_BufSizeType nof)
{
  uint8_t res = ERR_OK;

  while(nof>0) {
    res = RNG1_Get(buf);
    if (res!=ERR_OK) {
      break;
    }
    buf++; nof--;
  }
  return res;
}

/*
** ===================================================================
**     Method      :  NofElements (component RingBuffer)
**
**     Description :
**         Returns the actual number of elements in the buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/
RNG1_BufSizeType RNG1_NofElements(void)
{
  return RNG1_inSize;
}

/*
** ===================================================================
**     Method      :  NofFreeElements (component RingBuffer)
**
**     Description :
**         Returns the actual number of free elements/space in the
**         buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of elements in the buffer.
** ===================================================================
*/
RNG1_BufSizeType RNG1_NofFreeElements(void)
{
  return (RNG1_BufSizeType)(RNG1_CONFIG_BUF_SIZE-RNG1_inSize);
}

/*
** ===================================================================
**     Method      :  Init (component RingBuffer)
**
**     Description :
**         Initializes the data structure
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RNG1_Init(void)
{
  RNG1_inIdx = 0;
  RNG1_outIdx = 0;
  RNG1_inSize = 0;
}

/*
** ===================================================================
**     Method      :  Clear (component RingBuffer)
**
**     Description :
**         Clear (empty) the ring buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void RNG1_Clear(void)
{
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  RNG1_Init();
  RNG1_EXIT_CRITICAL();
}

/*
** ===================================================================
**     Method      :  Peek (component RingBuffer)
**
**     Description :
**         Returns an element of the buffer without removiing it.
**     Parameters  :
**         NAME            - DESCRIPTION
**         index           - Index of element. 0 peeks the top
**                           element, 1 the next, and so on.
**       * elemP           - Pointer to where to store the received
**                           element
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Peek(RNG1_BufSizeType index, RNG1_ElementType *elemP)
{
  uint8_t res = ERR_OK;
  int idx; /* index inside ring buffer */
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (index>=RNG1_CONFIG_BUF_SIZE) {
    res = ERR_OVERFLOW; /* asking for an element outside of ring buffer size */
  } else if (index<RNG1_inSize) {
    idx = (RNG1_outIdx+index)%RNG1_CONFIG_BUF_SIZE;
    *elemP = RNG1_buffer[idx];
  } else { /* asking for an element which does not exist */
    res = ERR_RXEMPTY;
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  Compare (component RingBuffer)
**
**     Description :
**         Compares the elements in the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**         index           - Index of element. 0 peeks the top
**                           element, 1 the next, and so on.
**       * elemP           - Pointer to elements to compare with
**         nof             - number of elements to compare
**     Returns     :
**         ---             - zero if elements are the same, -1 otherwise
** ===================================================================
*/
uint8_t RNG1_Compare(RNG1_BufSizeType index, RNG1_ElementType *elemP, RNG1_BufSizeType nof)
{
  uint8_t cmpResult = 0;
  uint8_t res;
  RNG1_ElementType val;

  while(nof>0) {
    res = RNG1_Peek(index, &val);
    if (res!=ERR_OK) { /* general failure? */
      cmpResult = (uint8_t)-1; /* no match */
      break;
    }
    if (val!=*elemP) { /* mismatch */
      cmpResult = (uint8_t)-1; /* no match */
      break;
    }
    elemP++; index++; nof--;
  }

  return cmpResult;
}

/*
** ===================================================================
**     Method      :  Deinit (component RingBuffer)
**
**     Description :
**         Driver de-initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
void RNG1_Deinit(void)
{
  ** Function is implemented as macro in the header file
}
*/
/*
** ===================================================================
**     Method      :  Delete (component RingBuffer)
**
**     Description :
**         Removes an element from the buffer
**     Parameters  : None
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Delete(void)
{
  uint8_t res = ERR_OK;
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (RNG1_inSize==0) {
    res = ERR_RXEMPTY;
  } else {
    RNG1_inSize--;
    RNG1_outIdx++;
    if (RNG1_outIdx==RNG1_CONFIG_BUF_SIZE) {
      RNG1_outIdx = 0;
    }
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/*
** ===================================================================
**     Method      :  Update (component RingBuffer)
**
**     Description :
**         Updates the data of an element.
**     Parameters  :
**         NAME            - DESCRIPTION
**         index           - Index of element. 0 peeks the top
**                           element, 1 the next, and so on.
**       * elemP           - Pointer to where to store the received
**                           element
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t RNG1_Update(RNG1_BufSizeType index, RNG1_ElementType *elemP)
{
  uint8_t res = ERR_OK;
  int idx; /* index inside ring buffer */
  RNG1_DEFINE_CRITICAL();

  RNG1_ENTER_CRITICAL();
  if (index>=RNG1_CONFIG_BUF_SIZE) {
    res = ERR_OVERFLOW; /* asking for an element outside of ring buffer size */
  } else if (index<RNG1_inSize) {
    idx = (RNG1_outIdx+index)%RNG1_CONFIG_BUF_SIZE;
    RNG1_buffer[idx] = *elemP; /* replace element */
  } else { /* asking for an element which does not exist */
    res = ERR_RXEMPTY;
  }
  RNG1_EXIT_CRITICAL();
  return res;
}

/* END RNG1. */

/*!
** @}
*/
