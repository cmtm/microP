#pragma once

#include "arm_math.h"

// typedef struct
//   {
//     uint8_t M;                      /**< decimation factor. */
//     uint16_t numTaps;               /**< number of coefficients in the filter. */
//     q15_t *pCoeffs;                  /**< points to the coefficient array. The array is of length numTaps.*/
//     q15_t *pState;                   /**< points to the state variable array. The array is of length numTaps+blockSize-1. */
//   } arm_fir_decimate_instance_q15;

/*
 * @brief  Initialization function for the Q15 FIR decimator.
 * @param[in,out] *S points to an instance of the Q15 FIR decimator structure.
 * @param[in] numTaps  number of coefficients in the filter.
 * @param[in] M  decimation factor.
 * @param[in] *pCoeffs points to the filter coefficients.
 * @param[in] *pState points to the state buffer.
 * @param[in] blockSize number of input samples to process per call.
 * @return    The function returns ARM_MATH_SUCCESS if initialization is successful or ARM_MATH_LENGTH_ERROR if
 * <code>blockSize</code> is not a multiple of <code>M</code>.
 */
// arm_status arm_fir_decimate_init_q15(
//   arm_fir_decimate_instance_q15 * S,
//   uint16_t numTaps,
//   uint8_t M,
//   q15_t * pCoeffs,
//   q15_t * pState,
//   uint32_t blockSize)

void myDecimate(
  const arm_fir_decimate_instance_q15 * S,
  q15_t * pSrc,
  q15_t * pDst,
  uint32_t blockSize);

