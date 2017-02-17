/*
 * DynModel.c
 *
 * Code generation for model "DynModel".
 *
 * Model version              : 1.743
 * Simulink Coder version : 8.8 (R2015a) 09-Feb-2015
 * C source code generated on : Fri Feb 17 17:33:22 2017
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->16-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "DynModel.h"
#include "DynModel_private.h"

/* Exported block parameters */
real_T Kattreact = 2.0;                /* Variable: Kattreact
                                        * Referenced by: '<S5>/Gain5'
                                        */
real_T Kdiscre = 0.01477832512315271;  /* Variable: Kdiscre
                                        * Referenced by:
                                        *   '<S52>/Gain1'
                                        *   '<S53>/Gain1'
                                        *   '<S54>/Gain1'
                                        *   '<S55>/Gain1'
                                        */
real_T Kdpenetration = 120.0;          /* Variable: Kdpenetration
                                        * Referenced by:
                                        *   '<S5>/Gain1'
                                        *   '<S5>/Gain7'
                                        */
real_T Kpenetration = 3000.0;          /* Variable: Kpenetration
                                        * Referenced by:
                                        *   '<S5>/Gain12'
                                        *   '<S5>/Gain3'
                                        */
real_T Kvreact = 120.0;                /* Variable: Kvreact
                                        * Referenced by: '<S5>/Gain8'
                                        */

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si ,
  RT_MODEL_DynModel_T *const DynModel_M)
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 13;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  DynModel_derivatives(DynModel_M);

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  DynModel_step(DynModel_M);
  DynModel_derivatives(DynModel_M);

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  DynModel_step(DynModel_M);
  DynModel_derivatives(DynModel_M);

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  DynModel_step(DynModel_M);
  DynModel_derivatives(DynModel_M);

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_urand_Upu32_Yd_f_pw(uint32_T *u)
{
  uint32_T lo;
  uint32_T hi;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773UL * 16807UL;
  hi = *u / 127773UL * 2836UL;
  if (lo < hi) {
    *u = 2147483647UL - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return (real_T)*u * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw(uint32_T *u)
{
  real_T y;
  real_T sr;
  real_T si;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

real_T rt_roundd(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

real_T rt_modd(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  if (u1 == 0.0) {
    y = u0;
  } else {
    tmp = u0 / u1;
    if (u1 <= floor(u1)) {
      y = u0 - floor(tmp) * u1;
    } else {
      if (fabs(tmp - rt_roundd(tmp)) <= DBL_EPSILON * fabs(tmp)) {
        y = 0.0;
      } else {
        y = (tmp - floor(tmp)) * u1;
      }
    }
  }

  return y;
}

void rt_mrdivide_U1d1x3_U2d3x3_Yd1x3(const real_T u0[3], const real_T u1[9],
  real_T y[3])
{
  real_T A[9];
  int16_T r1;
  int16_T r2;
  int16_T r3;
  real_T maxval;
  real_T a21;
  int16_T rtemp;
  memcpy(&A[0L], &u1[0L], 9U * sizeof(real_T));
  r1 = 0;
  r2 = 1;
  r3 = 2;
  maxval = fabs(u1[0]);
  a21 = fabs(u1[1]);
  if (a21 > maxval) {
    maxval = a21;
    r1 = 1;
    r2 = 0;
  }

  if (fabs(u1[2]) > maxval) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }

  A[r2] = u1[r2] / u1[r1];
  A[r3] /= A[r1];
  A[3 + r2] -= A[3 + r1] * A[r2];
  A[3 + r3] -= A[3 + r1] * A[r3];
  A[6 + r2] -= A[6 + r1] * A[r2];
  A[6 + r3] -= A[6 + r1] * A[r3];
  if (fabs(A[3 + r3]) > fabs(A[3 + r2])) {
    rtemp = r2 + 1;
    r2 = r3;
    r3 = rtemp - 1;
  }

  A[3 + r3] /= A[3 + r2];
  A[6 + r3] -= A[3 + r3] * A[6 + r2];
  y[r1] = u0[0] / A[r1];
  y[r2] = u0[1] - A[3 + r1] * y[r1];
  y[r3] = u0[2] - A[6 + r1] * y[r1];
  y[r2] /= A[3 + r2];
  y[r3] -= A[6 + r2] * y[r2];
  y[r3] /= A[6 + r3];
  y[r2] -= A[3 + r3] * y[r3];
  y[r1] -= y[r3] * A[r3];
  y[r1] -= y[r2] * A[r2];
}

/* Model step function */
void DynModel_step(RT_MODEL_DynModel_T *const DynModel_M)
{
  B_DynModel_T *DynModel_B = ((B_DynModel_T *) DynModel_M->ModelData.blockIO);
  DW_DynModel_T *DynModel_DW = ((DW_DynModel_T *) DynModel_M->ModelData.dwork);
  X_DynModel_T *DynModel_X = ((X_DynModel_T *) DynModel_M->ModelData.contStates);
  ExtU_DynModel_T *DynModel_U = (ExtU_DynModel_T *) DynModel_M->ModelData.inputs;
  ExtY_DynModel_T *DynModel_Y = (ExtY_DynModel_T *)
    DynModel_M->ModelData.outputs;

  /* local block i/o variables */
  real_T rtb_a;
  real_T rtb_q0q1q2q3[4];
  real_T rtb_TSamp[3];
  real_T rtb_Uk1[3];
  real_T rtb_Diff[3];
  real_T rtb_TSamp_l;
  real_T rtb_Uk1_g;
  real_T rtb_Diff_o;
  int32_T j;
  real_T rtb_ZeroOrderHold1_h;
  real_T rtb_Saturation;
  real_T rtb_jxi;
  real_T rtb_ixk;
  real_T rtb_ZeroOrderHold1;
  real_T rtb_Sum_j;
  real_T rtb_VectorConcatenate[9];
  real_T rtb_Add2[3];
  real_T rtb_Switch_d;
  real_T rtb_Sum_e;
  real_T rtb_DiscreteTransferFcn_n;
  real_T rtb_Add3_j[3];
  int32_T loop;
  real_T rtb_Memory2;
  real_T rtb_WhiteNoise[3];
  int8_T rtAction;
  real_T rtb_DiscreteTransferFcn;
  real_T rtb_DiscreteTransferFcn_b;
  real_T rtb_DiscreteTransferFcn_j;
  real_T rtb_DiscreteTransferFcn_i;
  real_T rtb_Add6_0[3];
  int16_T i;
  real_T tmp[3];
  real32_T rtb_DataTypeConversion21_idx_0;
  real32_T rtb_DataTypeConversion21_idx_1;
  real32_T rtb_DataTypeConversion21_idx_2;
  real32_T rtb_DataTypeConversion21_idx_3;
  real_T rtb_Sum_c_idx_0;
  real_T rtb_Sum_h_idx_0;
  real_T rtb_Sum_h_idx_1;
  real_T rtb_Sum_h_idx_2;
  real_T rtb_Add1_h_idx_0;
  real_T rtb_Add1_h_idx_1;
  real_T rtb_thrust_idx_0;
  real_T rtb_Add7_idx_0;
  real_T rtb_Add7_idx_1;
  real_T rtb_Add7_idx_2;
  real_T rtb_Saturation_d_idx_0;
  real_T rtb_Saturation_l_idx_2;
  real_T rtb_Saturation_l_idx_1;
  real_T rtb_Saturation_d_idx_1;
  real_T rtb_Saturation_l_idx_0;
  real_T rtb_Saturation_d_idx_2;
  real_T rtb_ubvbwb_idx_1;
  real_T rtb_ubvbwb_idx_2;
  real_T rtb_ubvbwb_idx_0;
  real_T rtb_Saturation_gt_idx_0;
  real_T rtb_Saturation_gt_idx_1;
  real_T rtb_Saturation_gt_idx_2;
  real_T rtb_ZeroOrderHold2_idx_0;
  real_T rtb_ZeroOrderHold2_idx_1;
  real_T rtb_ZeroOrderHold2_idx_2;
  real_T rtb_thrust_idx_1;
  real_T rtb_thrust_idx_2;
  real_T rtb_thrust_idx_3;
  real32_T rtb_DataTypeConversion14_idx_0;
  real32_T rtb_DataTypeConversion14_idx_1;
  real32_T rtb_DataTypeConversion14_idx_2;
  real32_T rtb_DataTypeConversion15_idx_0;
  real32_T rtb_DataTypeConversion15_idx_1;
  real32_T rtb_DataTypeConversion15_idx_2;
  real32_T rtb_DataTypeConversion12_idx_0;
  real32_T rtb_DataTypeConversion12_idx_1;
  real32_T rtb_DataTypeConversion12_idx_2;
  real32_T rtb_DataTypeConversion20_idx_0;
  real32_T rtb_DataTypeConversion20_idx_1;
  real32_T rtb_DataTypeConversion20_idx_2;
  real32_T rtb_DataTypeConversion20_idx_3;
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* set solver stop time */
    if (!(DynModel_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(DynModel_M->solverInfo,
                            ((DynModel_M->Timing.clockTickH0 + 1) *
        DynModel_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(DynModel_M->solverInfo,
                            ((DynModel_M->Timing.clockTick0 + 1) *
        DynModel_M->Timing.stepSize0 + DynModel_M->Timing.clockTickH0 *
        DynModel_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(DynModel_M)) {
    DynModel_M->Timing.t[0] = rtsiGetT(DynModel_M->solverInfo);
  }

  /* Integrator: '<S4>/xe,ye,ze' */
  DynModel_B->xeyeze[0] = DynModel_X->xeyeze_CSTATE[0];
  DynModel_B->xeyeze[1] = DynModel_X->xeyeze_CSTATE[1];
  DynModel_B->xeyeze[2] = DynModel_X->xeyeze_CSTATE[2];

  /* Sum: '<S146>/Sum1' incorporates:
   *  Constant: '<S3>/h_ref'
   *  UnaryMinus: '<S146>/Ze2height'
   */
  DynModel_B->Sum1 = -DynModel_B->xeyeze[2] - 0.0;

  /* Saturate: '<S64>/Limit  altitude  to troposhere' */
  if (DynModel_B->Sum1 > 11000.0) {
    rtb_DiscreteTransferFcn_n = 11000.0;
  } else if (DynModel_B->Sum1 < 0.0) {
    rtb_DiscreteTransferFcn_n = 0.0;
  } else {
    rtb_DiscreteTransferFcn_n = DynModel_B->Sum1;
  }

  /* Sum: '<S64>/Sum1' incorporates:
   *  Constant: '<S64>/Sea Level  Temperature'
   *  Gain: '<S64>/Lapse Rate'
   *  Saturate: '<S64>/Limit  altitude  to troposhere'
   */
  DynModel_B->Sum1_e = 288.15 - 0.0065 * rtb_DiscreteTransferFcn_n;

  /* RateTransition: '<S72>/Rate Transition' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    DynModel_B->RateTransition = DynModel_B->Sum1_e;

    /* Sum: '<S72>/Sum3' incorporates:
     *  Constant: '<S72>/K2C'
     *  Constant: '<S72>/Measurement bias1'
     *  RandomNumber: '<S72>/Random Number'
     *  Sum: '<S72>/Add1'
     *  Sum: '<S72>/Sum2'
     */
    rtb_ZeroOrderHold1_h = ((273.15 + DynModel_B->RateTransition) +
      DynModel_DW->NextOutput) + 0.0;

    /* Saturate: '<S72>/Saturation' */
    if (rtb_ZeroOrderHold1_h > 85.0) {
      rtb_Saturation = 85.0;
    } else if (rtb_ZeroOrderHold1_h < 40.0) {
      rtb_Saturation = 40.0;
    } else {
      rtb_Saturation = rtb_ZeroOrderHold1_h;
    }

    /* End of Saturate: '<S72>/Saturation' */

    /* RateTransition: '<S72>/Rate Transition1' */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition1 = rtb_Saturation;
    }

    /* End of RateTransition: '<S72>/Rate Transition1' */
  }

  /* End of RateTransition: '<S72>/Rate Transition' */

  /* Gain: '<S64>/1//T0' */
  rtb_jxi = 0.00347041471455839 * DynModel_B->Sum1_e;

  /* Math: '<S64>/(T//T0)^(g//LR) ' incorporates:
   *  Constant: '<S64>/Constant'
   */
  if ((rtb_jxi < 0.0) && (5.2558756014667134 > floor(5.2558756014667134))) {
    rtb_ixk = -pow(-rtb_jxi, 5.2558756014667134);
  } else {
    rtb_ixk = pow(rtb_jxi, 5.2558756014667134);
  }

  /* End of Math: '<S64>/(T//T0)^(g//LR) ' */

  /* Sum: '<S64>/Sum' incorporates:
   *  Constant: '<S64>/Altitude of Troposphere'
   */
  rtb_DiscreteTransferFcn_n = 11000.0 - DynModel_B->Sum1;

  /* Saturate: '<S64>/Limit  altitude  to Stratosphere' */
  if (rtb_DiscreteTransferFcn_n > 0.0) {
    rtb_DiscreteTransferFcn_n = 0.0;
  } else {
    if (rtb_DiscreteTransferFcn_n < (-9000.0)) {
      rtb_DiscreteTransferFcn_n = (-9000.0);
    }
  }

  /* Math: '<S64>/Stratosphere Model' incorporates:
   *  Gain: '<S64>/g//R'
   *  Product: '<S64>/Product1'
   *  Saturate: '<S64>/Limit  altitude  to Stratosphere'
   *
   * About '<S64>/Stratosphere Model':
   *  Operator: exp
   */
  rtb_Sum_e = exp(1.0 / DynModel_B->Sum1_e * (0.034163191409533639 *
    rtb_DiscreteTransferFcn_n));

  /* Product: '<S64>/Product2' incorporates:
   *  Gain: '<S64>/P0'
   */
  DynModel_B->Product2 = 101325.0 * rtb_ixk * rtb_Sum_e;

  /* RateTransition: '<S69>/Rate Transition' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    DynModel_B->RateTransition_c = DynModel_B->Product2;

    /* Sum: '<S69>/Sum3' incorporates:
     *  Constant: '<S69>/Measurement bias1'
     *  Gain: '<S69>/Bar2mBar'
     *  Gain: '<S69>/Pa2Bar'
     *  RandomNumber: '<S69>/Random Number'
     *  Sum: '<S69>/Sum2'
     */
    rtb_ZeroOrderHold1_h = (1.0E-5 * DynModel_B->RateTransition_c * 1000.0 +
      DynModel_DW->NextOutput_a) + 0.0;

    /* Saturate: '<S69>/Saturation' */
    if (rtb_ZeroOrderHold1_h > 1200.0) {
      /* ZeroOrderHold: '<S69>/Zero-Order Hold1' */
      rtb_ZeroOrderHold1_h = 1200.0;
    } else {
      if (rtb_ZeroOrderHold1_h < 10.0) {
        /* ZeroOrderHold: '<S69>/Zero-Order Hold1' */
        rtb_ZeroOrderHold1_h = 10.0;
      }
    }

    /* End of Saturate: '<S69>/Saturation' */
  }

  /* End of RateTransition: '<S69>/Rate Transition' */

  /* Product: '<S64>/Product3' incorporates:
   *  Gain: '<S64>/rho0'
   *  Product: '<S64>/Product'
   */
  DynModel_B->Product3 = rtb_ixk / rtb_jxi * 1.225 * rtb_Sum_e;

  /* RateTransition: '<S67>/Rate Transition' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    DynModel_B->RateTransition_b = DynModel_B->Product3;
  }

  /* End of RateTransition: '<S67>/Rate Transition' */

  /* Integrator: '<S4>/ub,vb,wb' */
  rtb_ubvbwb_idx_0 = DynModel_X->ubvbwb_CSTATE[0];
  rtb_ubvbwb_idx_1 = DynModel_X->ubvbwb_CSTATE[1];
  rtb_ubvbwb_idx_2 = DynModel_X->ubvbwb_CSTATE[2];

  /* Gain: '<S14>/Unit Conversion' incorporates:
   *  Integrator: '<S4>/ub,vb,wb'
   */
  DynModel_B->UnitConversion[0] = 1.0 * DynModel_X->ubvbwb_CSTATE[0];
  DynModel_B->UnitConversion[1] = 1.0 * DynModel_X->ubvbwb_CSTATE[1];
  DynModel_B->UnitConversion[2] = 1.0 * DynModel_X->ubvbwb_CSTATE[2];

  /* RateTransition: '<S67>/Rate Transition1' incorporates:
   *  RandomNumber: '<S63>/Random Number'
   *  Sum: '<S63>/Add1'
   */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    DynModel_B->RateTransition1_p[0] = DynModel_B->UnitConversion[0];
    DynModel_B->RateTransition1_p[1] = DynModel_B->UnitConversion[1];
    DynModel_B->RateTransition1_p[2] = DynModel_B->UnitConversion[2];

    /* Sum: '<S67>/Sum3' incorporates:
     *  Constant: '<S67>/Measurement bias1'
     *  DotProduct: '<S67>/Dot Product'
     *  Gain: '<S67>/Bar2mBar'
     *  Gain: '<S67>/Gain'
     *  Gain: '<S67>/Pa2Bar'
     *  Product: '<S67>/Product'
     *  RandomNumber: '<S67>/Random Number'
     *  Sum: '<S67>/Sum2'
     */
    rtb_ZeroOrderHold1 = (((DynModel_B->RateTransition1_p[0] *
      DynModel_B->RateTransition1_p[0] + DynModel_B->RateTransition1_p[1] *
      DynModel_B->RateTransition1_p[1]) + DynModel_B->RateTransition1_p[2] *
      DynModel_B->RateTransition1_p[2]) * DynModel_B->RateTransition_b * 0.5 *
                          1.0E-5 * 1000.0 + DynModel_DW->NextOutput_l) + 0.0;

    /* Saturate: '<S67>/Saturation' */
    if (rtb_ZeroOrderHold1 > 1000.0) {
      /* ZeroOrderHold: '<S67>/Zero-Order Hold1' */
      rtb_ZeroOrderHold1 = 1000.0;
    } else {
      if (rtb_ZeroOrderHold1 < 0.0) {
        /* ZeroOrderHold: '<S67>/Zero-Order Hold1' */
        rtb_ZeroOrderHold1 = 0.0;
      }
    }

    /* End of Saturate: '<S67>/Saturation' */

    /* RateTransition: '<S60>/Rate Transition1' incorporates:
     *  Constant: '<S60>/bias'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition1_i = 0.0;
    }

    /* End of RateTransition: '<S60>/Rate Transition1' */

    /* RateTransition: '<S60>/Rate Transition2' */
    DynModel_B->RateTransition2 = DynModel_B->Sum1;

    /* Sum: '<S60>/Add1' incorporates:
     *  RandomNumber: '<S60>/Random Number'
     */
    rtb_Saturation = (DynModel_B->RateTransition1_i +
                      DynModel_B->RateTransition2) + DynModel_DW->NextOutput_n;

    /* Saturate: '<S60>/Saturation1' */
    if (!(rtb_Saturation >= 0.0)) {
      rtb_Saturation = 0.0;
    }

    /* End of Saturate: '<S60>/Saturation1' */

    /* RateTransition: '<S60>/Rate Transition3' */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition3 = rtb_Saturation;
    }

    /* End of RateTransition: '<S60>/Rate Transition3' */

    /* RateTransition: '<S63>/Rate Transition1' */
    DynModel_B->RateTransition1_a[0] = DynModel_B->xeyeze[0];
    DynModel_B->RateTransition1_a[1] = DynModel_B->xeyeze[1];
    DynModel_B->RateTransition1_a[2] = DynModel_B->xeyeze[2];
    rtb_Sum_h_idx_0 = DynModel_DW->NextOutput_o[0] +
      DynModel_B->RateTransition1_a[0];
    rtb_Sum_h_idx_1 = DynModel_DW->NextOutput_o[1] +
      DynModel_B->RateTransition1_a[1];

    /* Sum: '<S98>/Sum' incorporates:
     *  Gain: '<S102>/Unit Conversion'
     *  Product: '<S101>/rad lat'
     *  Product: '<S101>/x*cos'
     *  Product: '<S101>/y*sin'
     *  RandomNumber: '<S63>/Random Number'
     *  Sum: '<S101>/Sum'
     *  Sum: '<S63>/Add1'
     */
    rtb_Sum_c_idx_0 = (rtb_Sum_h_idx_0 * DynModel_ConstB.SinCos_o2 -
                       rtb_Sum_h_idx_1 * DynModel_ConstB.SinCos_o1) *
      DynModel_ConstB.TrigonometricFunction1_g * 57.295779513082323 +
      DynModel_ConstB.Switch_h;

    /* Switch: '<S106>/Switch' incorporates:
     *  Abs: '<S106>/Abs'
     *  Bias: '<S106>/Bias'
     *  Bias: '<S106>/Bias1'
     *  Constant: '<S106>/Constant2'
     *  Math: '<S106>/Math Function1'
     */
    if (fabs(rtb_Sum_c_idx_0) > 180.0) {
      rtb_Sum_c_idx_0 = rt_modd(rtb_Sum_c_idx_0 + 180.0, 360.0) + (-180.0);
    }

    /* End of Switch: '<S106>/Switch' */

    /* Abs: '<S103>/Abs1' */
    rtb_Saturation = fabs(rtb_Sum_c_idx_0);

    /* Switch: '<S103>/Switch' incorporates:
     *  Bias: '<S103>/Bias'
     *  Bias: '<S103>/Bias1'
     *  Constant: '<S105>/Constant'
     *  Constant: '<S99>/Constant'
     *  Constant: '<S99>/Constant1'
     *  Gain: '<S103>/Gain'
     *  Product: '<S103>/Divide1'
     *  RelationalOperator: '<S105>/Compare'
     *  Signum: '<S103>/Sign1'
     *  Switch: '<S99>/Switch1'
     */
    if ((rtb_Saturation > 90.0) > ((uint8_T)0U)) {
      /* Signum: '<S103>/Sign1' */
      if (rtb_Sum_c_idx_0 < 0.0) {
        rtb_Sum_c_idx_0 = -1.0;
      } else {
        if (rtb_Sum_c_idx_0 > 0.0) {
          rtb_Sum_c_idx_0 = 1.0;
        }
      }

      rtb_Sum_c_idx_0 *= (rtb_Saturation + (-90.0)) * (-1.0) + 90.0;
      rtb_DiscreteTransferFcn_n = 180.0;
    } else {
      rtb_DiscreteTransferFcn_n = 0.0;
    }

    /* End of Switch: '<S103>/Switch' */

    /* Sum: '<S99>/Sum' incorporates:
     *  Gain: '<S102>/Unit Conversion'
     *  Product: '<S101>/rad long '
     *  Product: '<S101>/x*sin'
     *  Product: '<S101>/y*cos'
     *  Sum: '<S101>/Sum1'
     *  Sum: '<S98>/Sum'
     */
    rtb_Sum_j = ((rtb_Sum_h_idx_0 * DynModel_ConstB.SinCos_o1 + rtb_Sum_h_idx_1 *
                  DynModel_ConstB.SinCos_o2) *
                 DynModel_ConstB.TrigonometricFunction2 * 57.295779513082323 +
                 DynModel_ConstB.Switch_d) + rtb_DiscreteTransferFcn_n;

    /* Switch: '<S104>/Switch' incorporates:
     *  Abs: '<S104>/Abs'
     *  Bias: '<S104>/Bias'
     *  Bias: '<S104>/Bias1'
     *  Constant: '<S104>/Constant2'
     *  Math: '<S104>/Math Function1'
     */
    if (fabs(rtb_Sum_j) > 180.0) {
      rtb_Sum_j = rt_modd(rtb_Sum_j + 180.0, 360.0) + (-180.0);
    }

    /* End of Switch: '<S104>/Switch' */

    /* RateTransition: '<S63>/Rate Transition' incorporates:
     *  Constant: '<S3>/h_ref'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition_e = 0.0;
    }

    /* End of RateTransition: '<S63>/Rate Transition' */

    /* Sum: '<S98>/Sum1' incorporates:
     *  RandomNumber: '<S63>/Random Number'
     *  Sum: '<S63>/Add1'
     *  UnaryMinus: '<S98>/Ze2height'
     */
    rtb_Saturation = -(DynModel_DW->NextOutput_o[2] +
                       DynModel_B->RateTransition1_a[2]) -
      DynModel_B->RateTransition_e;

    /* RateTransition: '<S63>/Rate Transition5' */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition5 = rtb_Saturation;
    }

    /* End of RateTransition: '<S63>/Rate Transition5' */

    /* RandomNumber: '<S63>/Random Number1' */
    rtb_Sum_h_idx_0 = DynModel_DW->NextOutput_h[0];
    rtb_Sum_h_idx_1 = DynModel_DW->NextOutput_h[1];
    rtb_Sum_h_idx_2 = DynModel_DW->NextOutput_h[2];
  }

  /* End of RateTransition: '<S67>/Rate Transition1' */

  /* Integrator: '<S10>/q0 q1 q2 q3' */
  if (DynModel_DW->q0q1q2q3_IWORK.IcNeedsLoading) {
    DynModel_X->q0q1q2q3_CSTATE[0] = DynModel_ConstB.q0;
    DynModel_X->q0q1q2q3_CSTATE[1] = DynModel_ConstB.q1;
    DynModel_X->q0q1q2q3_CSTATE[2] = DynModel_ConstB.q2;
    DynModel_X->q0q1q2q3_CSTATE[3] = DynModel_ConstB.q3;
  }

  rtb_q0q1q2q3[0] = DynModel_X->q0q1q2q3_CSTATE[0];
  rtb_q0q1q2q3[1] = DynModel_X->q0q1q2q3_CSTATE[1];
  rtb_q0q1q2q3[2] = DynModel_X->q0q1q2q3_CSTATE[2];
  rtb_q0q1q2q3[3] = DynModel_X->q0q1q2q3_CSTATE[3];

  /* Sqrt: '<S32>/sqrt' incorporates:
   *  Product: '<S33>/Product'
   *  Product: '<S33>/Product1'
   *  Product: '<S33>/Product2'
   *  Product: '<S33>/Product3'
   *  Sum: '<S33>/Sum'
   */
  rtb_Sum_e = sqrt(((rtb_q0q1q2q3[0] * rtb_q0q1q2q3[0] + rtb_q0q1q2q3[1] *
                     rtb_q0q1q2q3[1]) + rtb_q0q1q2q3[2] * rtb_q0q1q2q3[2]) +
                   rtb_q0q1q2q3[3] * rtb_q0q1q2q3[3]);

  /* Product: '<S31>/Product' */
  rtb_ixk = rtb_q0q1q2q3[0] / rtb_Sum_e;

  /* Product: '<S31>/Product1' */
  rtb_jxi = rtb_q0q1q2q3[1] / rtb_Sum_e;

  /* Product: '<S31>/Product2' */
  rtb_Switch_d = rtb_q0q1q2q3[2] / rtb_Sum_e;

  /* Product: '<S31>/Product3' */
  rtb_Sum_e = rtb_q0q1q2q3[3] / rtb_Sum_e;

  /* Sum: '<S21>/Sum' incorporates:
   *  Product: '<S21>/Product'
   *  Product: '<S21>/Product1'
   *  Product: '<S21>/Product2'
   *  Product: '<S21>/Product3'
   */
  rtb_VectorConcatenate[0] = ((rtb_ixk * rtb_ixk + rtb_jxi * rtb_jxi) -
    rtb_Switch_d * rtb_Switch_d) - rtb_Sum_e * rtb_Sum_e;

  /* Gain: '<S24>/Gain' incorporates:
   *  Product: '<S24>/Product2'
   *  Product: '<S24>/Product3'
   *  Sum: '<S24>/Sum'
   */
  rtb_VectorConcatenate[1] = (rtb_jxi * rtb_Switch_d - rtb_Sum_e * rtb_ixk) *
    2.0;

  /* Gain: '<S27>/Gain' incorporates:
   *  Product: '<S27>/Product1'
   *  Product: '<S27>/Product2'
   *  Sum: '<S27>/Sum'
   */
  rtb_VectorConcatenate[2] = (rtb_ixk * rtb_Switch_d + rtb_jxi * rtb_Sum_e) *
    2.0;

  /* Gain: '<S22>/Gain' incorporates:
   *  Product: '<S22>/Product2'
   *  Product: '<S22>/Product3'
   *  Sum: '<S22>/Sum'
   */
  rtb_VectorConcatenate[3] = (rtb_Sum_e * rtb_ixk + rtb_jxi * rtb_Switch_d) *
    2.0;

  /* Sum: '<S25>/Sum' incorporates:
   *  Product: '<S25>/Product'
   *  Product: '<S25>/Product1'
   *  Product: '<S25>/Product2'
   *  Product: '<S25>/Product3'
   */
  rtb_VectorConcatenate[4] = ((rtb_ixk * rtb_ixk - rtb_jxi * rtb_jxi) +
    rtb_Switch_d * rtb_Switch_d) - rtb_Sum_e * rtb_Sum_e;

  /* Gain: '<S28>/Gain' incorporates:
   *  Product: '<S28>/Product1'
   *  Product: '<S28>/Product2'
   *  Sum: '<S28>/Sum'
   */
  rtb_VectorConcatenate[5] = (rtb_Switch_d * rtb_Sum_e - rtb_ixk * rtb_jxi) *
    2.0;

  /* Gain: '<S23>/Gain' incorporates:
   *  Product: '<S23>/Product1'
   *  Product: '<S23>/Product2'
   *  Sum: '<S23>/Sum'
   */
  rtb_VectorConcatenate[6] = (rtb_jxi * rtb_Sum_e - rtb_ixk * rtb_Switch_d) *
    2.0;

  /* Gain: '<S26>/Gain' incorporates:
   *  Product: '<S26>/Product1'
   *  Product: '<S26>/Product2'
   *  Sum: '<S26>/Sum'
   */
  rtb_VectorConcatenate[7] = (rtb_ixk * rtb_jxi + rtb_Switch_d * rtb_Sum_e) *
    2.0;

  /* Sum: '<S29>/Sum' incorporates:
   *  Product: '<S29>/Product'
   *  Product: '<S29>/Product1'
   *  Product: '<S29>/Product2'
   *  Product: '<S29>/Product3'
   */
  rtb_VectorConcatenate[8] = ((rtb_ixk * rtb_ixk - rtb_jxi * rtb_jxi) -
    rtb_Switch_d * rtb_Switch_d) + rtb_Sum_e * rtb_Sum_e;

  /* Reshape: '<S30>/Reshape (9) to [3x3] column-major' */
  memcpy(&DynModel_B->Reshape9to3x3columnmajor[0L], &rtb_VectorConcatenate[0L],
         9U * sizeof(real_T));

  /* Reshape: '<S16>/Reshape2' incorporates:
   *  Math: '<S4>/Transpose'
   *  Product: '<S16>/Product'
   *  Reshape: '<S16>/Reshape1'
   */
  for (i = 0; i < 3; i++) {
    DynModel_B->Reshape2[i] = 0.0;
    DynModel_B->Reshape2[i] += DynModel_B->Reshape9to3x3columnmajor[3 * i] *
      rtb_ubvbwb_idx_0;
    DynModel_B->Reshape2[i] += DynModel_B->Reshape9to3x3columnmajor[3 * i + 1] *
      rtb_ubvbwb_idx_1;
    DynModel_B->Reshape2[i] += DynModel_B->Reshape9to3x3columnmajor[3 * i + 2] *
      rtb_ubvbwb_idx_2;
  }

  /* End of Reshape: '<S16>/Reshape2' */

  /* Gain: '<S15>/Unit Conversion' */
  DynModel_B->UnitConversion_j[0] = 1.0 * DynModel_B->Reshape2[0];
  DynModel_B->UnitConversion_j[1] = 1.0 * DynModel_B->Reshape2[1];
  DynModel_B->UnitConversion_j[2] = 1.0 * DynModel_B->Reshape2[2];

  /* RateTransition: '<S63>/Rate Transition2' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    DynModel_B->RateTransition2_l[0] = DynModel_B->UnitConversion_j[0];
    DynModel_B->RateTransition2_l[1] = DynModel_B->UnitConversion_j[1];
    DynModel_B->RateTransition2_l[2] = DynModel_B->UnitConversion_j[2];

    /* Sum: '<S63>/Add2' */
    rtb_Add2[0] = rtb_Sum_h_idx_0 + DynModel_B->RateTransition2_l[0];
    rtb_Add2[1] = rtb_Sum_h_idx_1 + DynModel_B->RateTransition2_l[1];
    rtb_Add2[2] = rtb_Sum_h_idx_2 + DynModel_B->RateTransition2_l[2];

    /* RateTransition: '<S63>/Rate Transition6' incorporates:
     *  Inport: '<Root>/Initial_LL'
     *  RateTransition: '<S63>/Rate Transition3'
     *  RateTransition: '<S63>/Rate Transition4'
     *  Sum: '<S63>/Add3'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition6[0] = rtb_Add2[0];
      DynModel_B->RateTransition6[1] = rtb_Add2[1];
      DynModel_B->RateTransition6[2] = rtb_Add2[2];
      DynModel_B->RateTransition3_o = DynModel_U->Initial_LL[0] +
        rtb_Sum_c_idx_0;
      DynModel_B->RateTransition4 = DynModel_U->Initial_LL[1] + rtb_Sum_j;
    }

    /* End of RateTransition: '<S63>/Rate Transition6' */

    /* Trigonometry: '<S63>/Trigonometric Function' */
    rtb_Saturation = atan2(rtb_Add2[1], rtb_Add2[0]);

    /* RateTransition: '<S63>/Rate Transition7' */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition7 = rtb_Saturation;
    }

    /* End of RateTransition: '<S63>/Rate Transition7' */
  }

  /* End of RateTransition: '<S63>/Rate Transition2' */

  /* Sum: '<S146>/Sum' incorporates:
   *  Gain: '<S150>/Unit Conversion'
   *  Product: '<S149>/rad lat'
   *  Product: '<S149>/x*cos'
   *  Product: '<S149>/y*sin'
   *  Sum: '<S149>/Sum'
   */
  rtb_Sum_c_idx_0 = (DynModel_B->xeyeze[0] * DynModel_ConstB.SinCos_o2_i -
                     DynModel_B->xeyeze[1] * DynModel_ConstB.SinCos_o1_f) *
    DynModel_ConstB.TrigonometricFunction1_c * 57.295779513082323 +
    DynModel_ConstB.Switch_c;

  /* Switch: '<S154>/Switch' incorporates:
   *  Abs: '<S154>/Abs'
   *  Bias: '<S154>/Bias'
   *  Bias: '<S154>/Bias1'
   *  Constant: '<S154>/Constant2'
   *  Math: '<S154>/Math Function1'
   */
  if (fabs(rtb_Sum_c_idx_0) > 180.0) {
    rtb_Switch_d = rt_modd(rtb_Sum_c_idx_0 + 180.0, 360.0) + (-180.0);
  } else {
    rtb_Switch_d = rtb_Sum_c_idx_0;
  }

  /* End of Switch: '<S154>/Switch' */

  /* Abs: '<S151>/Abs1' */
  rtb_Saturation = fabs(rtb_Switch_d);

  /* Switch: '<S151>/Switch' incorporates:
   *  Bias: '<S151>/Bias'
   *  Bias: '<S151>/Bias1'
   *  Constant: '<S147>/Constant'
   *  Constant: '<S147>/Constant1'
   *  Constant: '<S153>/Constant'
   *  Gain: '<S151>/Gain'
   *  Product: '<S151>/Divide1'
   *  RelationalOperator: '<S153>/Compare'
   *  Signum: '<S151>/Sign1'
   *  Switch: '<S147>/Switch1'
   */
  if ((rtb_Saturation > 90.0) > ((uint8_T)0U)) {
    /* Signum: '<S151>/Sign1' */
    if (rtb_Switch_d < 0.0) {
      rtb_Switch_d = -1.0;
    } else {
      if (rtb_Switch_d > 0.0) {
        rtb_Switch_d = 1.0;
      }
    }

    rtb_Switch_d *= (rtb_Saturation + (-90.0)) * (-1.0) + 90.0;
    rtb_DiscreteTransferFcn_n = 180.0;
  } else {
    rtb_DiscreteTransferFcn_n = 0.0;
  }

  /* End of Switch: '<S151>/Switch' */

  /* Sum: '<S147>/Sum' incorporates:
   *  Gain: '<S150>/Unit Conversion'
   *  Product: '<S149>/rad long '
   *  Product: '<S149>/x*sin'
   *  Product: '<S149>/y*cos'
   *  Sum: '<S146>/Sum'
   *  Sum: '<S149>/Sum1'
   */
  rtb_Sum_e = ((DynModel_B->xeyeze[0] * DynModel_ConstB.SinCos_o1_f +
                DynModel_B->xeyeze[1] * DynModel_ConstB.SinCos_o2_i) *
               DynModel_ConstB.TrigonometricFunction2_i * 57.295779513082323 +
               DynModel_ConstB.Switch_b) + rtb_DiscreteTransferFcn_n;

  /* Switch: '<S152>/Switch' incorporates:
   *  Abs: '<S152>/Abs'
   *  Bias: '<S152>/Bias'
   *  Bias: '<S152>/Bias1'
   *  Constant: '<S152>/Constant2'
   *  Math: '<S152>/Math Function1'
   */
  if (fabs(rtb_Sum_e) > 180.0) {
    rtb_Sum_e = rt_modd(rtb_Sum_e + 180.0, 360.0) + (-180.0);
  }

  /* End of Switch: '<S152>/Switch' */

  /* Sum: '<S70>/Add1' incorporates:
   *  Inport: '<Root>/Initial_LL'
   */
  rtb_Add1_h_idx_0 = DynModel_U->Initial_LL[0] + rtb_Switch_d;
  rtb_Add1_h_idx_1 = DynModel_U->Initial_LL[1] + rtb_Sum_e;

  /* RateTransition: '<S66>/Rate Transition1' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    memcpy(&DynModel_B->RateTransition1_o[0L],
           &DynModel_B->Reshape9to3x3columnmajor[0L], 9U * sizeof(real_T));

    /* RateTransition: '<S66>/Rate Transition2' incorporates:
     *  Constant: '<S3>/dip'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition2_j = 0.0;
    }

    /* End of RateTransition: '<S66>/Rate Transition2' */

    /* Gain: '<S66>/deg2rad1' */
    rtb_Sum_c_idx_0 = 0.017453292519943295 * DynModel_B->RateTransition2_j;

    /* Trigonometry: '<S66>/Cos1' */
    rtb_Sum_j = cos(rtb_Sum_c_idx_0);

    /* RateTransition: '<S66>/Rate Transition3' incorporates:
     *  Constant: '<S3>/dec'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition3_d = 0.0;
    }

    /* End of RateTransition: '<S66>/Rate Transition3' */

    /* Gain: '<S66>/deg2rad' */
    rtb_Saturation = 0.017453292519943295 * DynModel_B->RateTransition3_d;

    /* Product: '<S66>/Product' incorporates:
     *  Trigonometry: '<S66>/Cos2'
     */
    rtb_DiscreteTransferFcn_n = rtb_Sum_j * cos(rtb_Saturation);

    /* Product: '<S66>/Product1' incorporates:
     *  Trigonometry: '<S66>/Sin2'
     */
    rtb_Sum_j *= sin(rtb_Saturation);

    /* Trigonometry: '<S66>/Sin1' */
    rtb_Sum_c_idx_0 = sin(rtb_Sum_c_idx_0);

    /* RateTransition: '<S66>/Rate Transition4' incorporates:
     *  Constant: '<S66>/Magnetic Field Module'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      DynModel_B->RateTransition4_o = 1.0;
    }

    /* End of RateTransition: '<S66>/Rate Transition4' */

    /* Product: '<S66>/Product2' */
    rtb_ixk = rtb_DiscreteTransferFcn_n * DynModel_B->RateTransition4_o;
    rtb_jxi = rtb_Sum_j * DynModel_B->RateTransition4_o;
    rtb_DiscreteTransferFcn_n = rtb_Sum_c_idx_0 * DynModel_B->RateTransition4_o;

    /* Sum: '<S66>/Sum2' incorporates:
     *  Constant: '<S66>/Measurement bias1'
     *  Product: '<S66>/Matrix Multiply2'
     *  RandomNumber: '<S66>/Random Number'
     *  Saturate: '<S66>/Saturation'
     *  Sum: '<S66>/Sum3'
     */
    for (i = 0; i < 3; i++) {
      tmp[i] = (((DynModel_B->RateTransition1_o[i + 3] * rtb_jxi +
                  DynModel_B->RateTransition1_o[i] * rtb_ixk) +
                 DynModel_B->RateTransition1_o[i + 6] *
                 rtb_DiscreteTransferFcn_n) + DynModel_ConstP.pooled11[i]) +
        DynModel_DW->NextOutput_am;
    }

    /* End of Sum: '<S66>/Sum2' */

    /* Saturate: '<S66>/Saturation' incorporates:
     *  Product: '<S66>/Matrix Multiply2'
     */
    if (tmp[0] > 2.0) {
      rtb_Saturation_gt_idx_0 = 2.0;
    } else if (tmp[0] < (-2.0)) {
      rtb_Saturation_gt_idx_0 = (-2.0);
    } else {
      rtb_Saturation_gt_idx_0 = tmp[0];
    }

    if (tmp[1] > 2.0) {
      rtb_Saturation_gt_idx_1 = 2.0;
    } else if (tmp[1] < (-2.0)) {
      rtb_Saturation_gt_idx_1 = (-2.0);
    } else {
      rtb_Saturation_gt_idx_1 = tmp[1];
    }

    if (tmp[2] > 2.0) {
      rtb_Saturation_gt_idx_2 = 2.0;
    } else if (tmp[2] < (-2.0)) {
      rtb_Saturation_gt_idx_2 = (-2.0);
    } else {
      rtb_Saturation_gt_idx_2 = tmp[2];
    }

    /* Gain: '<S61>/Output' incorporates:
     *  RandomNumber: '<S61>/White Noise'
     */
    DynModel_B->Output = 0.00027386127875258305 * DynModel_DW->NextOutput_lh;

    /* Memory: '<S2>/Memory1' */
    rtb_Sum_h_idx_0 = DynModel_DW->Memory1_PreviousInput[0];
    rtb_Sum_h_idx_1 = DynModel_DW->Memory1_PreviousInput[1];
    rtb_Sum_h_idx_2 = DynModel_DW->Memory1_PreviousInput[2];

    /* SampleTimeMath: '<S45>/TSamp'
     *
     * About '<S45>/TSamp':
     *  y = u * K where K = 1 / ( w * Ts )
     */
    rtb_TSamp[0] = rtb_Sum_h_idx_0 * 500.0;
    rtb_TSamp[1] = rtb_Sum_h_idx_1 * 500.0;
    rtb_TSamp[2] = rtb_Sum_h_idx_2 * 500.0;

    /* UnitDelay: '<S45>/UD' */
    rtb_Uk1[0] = DynModel_DW->UD_DSTATE[0];
    rtb_Uk1[1] = DynModel_DW->UD_DSTATE[1];
    rtb_Uk1[2] = DynModel_DW->UD_DSTATE[2];

    /* Sum: '<S45>/Diff' */
    rtb_Diff[0] = rtb_TSamp[0] - rtb_Uk1[0];
    rtb_Diff[1] = rtb_TSamp[1] - rtb_Uk1[1];
    rtb_Diff[2] = rtb_TSamp[2] - rtb_Uk1[2];

    /* Gain: '<S5>/Gain13' incorporates:
     *  Constant: '<S5>/Constant2'
     *  Gain: '<S5>/Gain12'
     *  Gain: '<S5>/Gain7'
     *  Sum: '<S5>/Add8'
     */
    DynModel_B->Gain13[0] = ((Kpenetration * rtb_Sum_h_idx_0 + (-11.772)) +
      Kdpenetration * rtb_Diff[0]) * 0.0;
    DynModel_B->Gain13[1] = ((Kpenetration * rtb_Sum_h_idx_1 + (-11.772)) +
      Kdpenetration * rtb_Diff[1]) * 0.0;
    DynModel_B->Gain13[2] = ((Kpenetration * rtb_Sum_h_idx_2 + (-11.772)) +
      Kdpenetration * rtb_Diff[2]) * 1.0;

    /* Gain: '<S5>/Gain11' */
    DynModel_B->Gain11[0] = (-1.0) * rtb_Sum_h_idx_0;
    DynModel_B->Gain11[1] = (-1.0) * rtb_Sum_h_idx_1;
    DynModel_B->Gain11[2] = (-1.0) * rtb_Sum_h_idx_2;
  }

  /* End of RateTransition: '<S66>/Rate Transition1' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Memory: '<S2>/Memory6' */
    rtb_Sum_h_idx_0 = DynModel_DW->Memory6_PreviousInput[0];
    rtb_Sum_h_idx_1 = DynModel_DW->Memory6_PreviousInput[1];
    rtb_Sum_h_idx_2 = DynModel_DW->Memory6_PreviousInput[2];

    /* Switch: '<S5>/Switch2' incorporates:
     *  Constant: '<S5>/Constant3'
     *  Gain: '<S5>/Gain8'
     *  Gain: '<S5>/Gain9'
     */
    if (DynModel_B->Gain11[0] >= 0.0) {
      DynModel_B->Switch2[0] = (-1.0) * rtb_Sum_h_idx_0 * Kvreact;
    } else {
      DynModel_B->Switch2[0] = 0.0;
    }

    if (DynModel_B->Gain11[1] >= 0.0) {
      DynModel_B->Switch2[1] = (-1.0) * rtb_Sum_h_idx_1 * Kvreact;
    } else {
      DynModel_B->Switch2[1] = 0.0;
    }

    if (DynModel_B->Gain11[2] >= 0.0) {
      DynModel_B->Switch2[2] = (-1.0) * rtb_Sum_h_idx_2 * Kvreact;
    } else {
      DynModel_B->Switch2[2] = 0.0;
    }

    /* End of Switch: '<S5>/Switch2' */

    /* SampleTimeMath: '<S44>/TSamp' incorporates:
     *  Inport: '<Root>/pen_collision'
     *
     * About '<S44>/TSamp':
     *  y = u * K where K = 1 / ( w * Ts )
     */
    rtb_TSamp_l = DynModel_U->pen_collision * 500.0;

    /* UnitDelay: '<S44>/UD' */
    rtb_Uk1_g = DynModel_DW->UD_DSTATE_i;

    /* Sum: '<S44>/Diff' */
    rtb_Diff_o = rtb_TSamp_l - rtb_Uk1_g;

    /* DiscreteFilter: '<S5>/Discrete Filter' incorporates:
     *  Gain: '<S5>/Gain1'
     *  Gain: '<S5>/Gain3'
     *  Inport: '<Root>/pen_collision'
     *  Sum: '<S5>/Add2'
     */
    DynModel_DW->DiscreteFilter_tmp = ((Kpenetration * DynModel_U->pen_collision
      + Kdpenetration * rtb_Diff_o) - (-0.8) *
      DynModel_DW->DiscreteFilter_states) / 1.0;
    rtb_Saturation = 0.2 * DynModel_DW->DiscreteFilter_tmp;
    DynModel_B->DiscreteFilter = rtb_Saturation;
  }

  /* Product: '<S5>/Nav2Body' incorporates:
   *  Inport: '<Root>/n_collision'
   *  Product: '<S5>/Product2'
   */
  for (i = 0; i < 3; i++) {
    tmp[i] = DynModel_B->Reshape9to3x3columnmajor[i + 6] *
      DynModel_U->n_collision[2] + (DynModel_B->Reshape9to3x3columnmajor[i + 3] *
      DynModel_U->n_collision[1] + DynModel_B->Reshape9to3x3columnmajor[i] *
      DynModel_U->n_collision[0]);
  }

  /* End of Product: '<S5>/Nav2Body' */

  /* Product: '<S5>/Product2' */
  DynModel_B->Product2_n[0] = tmp[0] * DynModel_B->DiscreteFilter;
  DynModel_B->Product2_n[1] = tmp[1] * DynModel_B->DiscreteFilter;
  DynModel_B->Product2_n[2] = tmp[2] * DynModel_B->DiscreteFilter;
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* DiscreteFir: '<S5>/Discrete FIR Filter' */
    rtb_Saturation = DynModel_B->Product2_n[0L] * 1.0;
    loop = 1L;
    for (j = DynModel_DW->DiscreteFIRFilter_circBuf; j < 2L; j++) {
      rtb_Saturation += DynModel_DW->DiscreteFIRFilter_states[j] *
        DynModel_ConstP.DiscreteFIRFilter_Coefficients[loop];
      loop++;
    }

    for (j = 0L; j < DynModel_DW->DiscreteFIRFilter_circBuf; j++) {
      rtb_Saturation += DynModel_DW->DiscreteFIRFilter_states[j] *
        DynModel_ConstP.DiscreteFIRFilter_Coefficients[loop];
      loop++;
    }

    rtb_Sum_h_idx_0 = rtb_Saturation;
    rtb_Saturation = DynModel_B->Product2_n[1L] * 1.0;
    loop = 1L;
    for (j = DynModel_DW->DiscreteFIRFilter_circBuf; j < 2L; j++) {
      rtb_Saturation += DynModel_DW->DiscreteFIRFilter_states[2L + j] *
        DynModel_ConstP.DiscreteFIRFilter_Coefficients[loop];
      loop++;
    }

    for (j = 0L; j < DynModel_DW->DiscreteFIRFilter_circBuf; j++) {
      rtb_Saturation += DynModel_DW->DiscreteFIRFilter_states[2L + j] *
        DynModel_ConstP.DiscreteFIRFilter_Coefficients[loop];
      loop++;
    }

    rtb_Sum_h_idx_1 = rtb_Saturation;
    rtb_Saturation = DynModel_B->Product2_n[2L] * 1.0;
    loop = 1L;
    for (j = DynModel_DW->DiscreteFIRFilter_circBuf; j < 2L; j++) {
      rtb_Saturation += DynModel_DW->DiscreteFIRFilter_states[4L + j] *
        DynModel_ConstP.DiscreteFIRFilter_Coefficients[loop];
      loop++;
    }

    for (j = 0L; j < DynModel_DW->DiscreteFIRFilter_circBuf; j++) {
      rtb_Saturation += DynModel_DW->DiscreteFIRFilter_states[4L + j] *
        DynModel_ConstP.DiscreteFIRFilter_Coefficients[loop];
      loop++;
    }

    rtb_Sum_h_idx_2 = rtb_Saturation;

    /* End of DiscreteFir: '<S5>/Discrete FIR Filter' */

    /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
     *  Inport: '<Root>/n_collision'
     */
    /* MATLAB Function 'DynModel/Dynamics/Constraint_Interaction/MATLAB Function': '<S46>:1' */
    /* '<S46>:1:2' */
    rtb_jxi = 2.2250738585072014E-308;
    rtb_ixk = fabs(DynModel_U->n_collision[0]);
    if (rtb_ixk > 2.2250738585072014E-308) {
      rtb_Saturation = 1.0;
      rtb_jxi = rtb_ixk;
    } else {
      rtb_Sum_e = rtb_ixk / 2.2250738585072014E-308;
      rtb_Saturation = rtb_Sum_e * rtb_Sum_e;
    }

    rtb_ixk = fabs(DynModel_U->n_collision[1]);
    if (rtb_ixk > rtb_jxi) {
      rtb_Sum_e = rtb_jxi / rtb_ixk;
      rtb_Saturation = rtb_Saturation * rtb_Sum_e * rtb_Sum_e + 1.0;
      rtb_jxi = rtb_ixk;
    } else {
      rtb_Sum_e = rtb_ixk / rtb_jxi;
      rtb_Saturation += rtb_Sum_e * rtb_Sum_e;
    }

    rtb_ixk = fabs(DynModel_U->n_collision[2]);
    if (rtb_ixk > rtb_jxi) {
      rtb_Sum_e = rtb_jxi / rtb_ixk;
      rtb_Saturation = rtb_Saturation * rtb_Sum_e * rtb_Sum_e + 1.0;
      rtb_jxi = rtb_ixk;
    } else {
      rtb_Sum_e = rtb_ixk / rtb_jxi;
      rtb_Saturation += rtb_Sum_e * rtb_Sum_e;
    }

    rtb_Saturation = rtb_jxi * sqrt(rtb_Saturation);
    if (rtb_Saturation > 2.2204460492503131E-16) {
      /* '<S46>:1:3' */
      /* '<S46>:1:4' */
      rtb_Saturation = 1.0;
    } else {
      /* '<S46>:1:6' */
      rtb_Saturation = -1.0;
    }

    /* End of MATLAB Function: '<S5>/MATLAB Function' */

    /* Switch: '<S5>/Switch' incorporates:
     *  Constant: '<S5>/Constant'
     *  Gain: '<S5>/Gain10'
     */
    if (rtb_Saturation >= 0.0) {
      rtb_Sum_h_idx_0 *= 0.33333333333333331;
      rtb_Sum_h_idx_1 *= 0.33333333333333331;
      rtb_Sum_h_idx_2 *= 0.33333333333333331;
    } else {
      rtb_Sum_h_idx_0 = 0.0;
      rtb_Sum_h_idx_1 = 0.0;
      rtb_Sum_h_idx_2 = 0.0;
    }

    /* End of Switch: '<S5>/Switch' */

    /* Saturate: '<S5>/Saturation' */
    if (rtb_Sum_h_idx_0 > 500.0) {
      DynModel_B->Saturation[0] = 500.0;
    } else if (rtb_Sum_h_idx_0 < (-500.0)) {
      DynModel_B->Saturation[0] = (-500.0);
    } else {
      DynModel_B->Saturation[0] = rtb_Sum_h_idx_0;
    }

    if (rtb_Sum_h_idx_1 > 500.0) {
      DynModel_B->Saturation[1] = 500.0;
    } else if (rtb_Sum_h_idx_1 < (-500.0)) {
      DynModel_B->Saturation[1] = (-500.0);
    } else {
      DynModel_B->Saturation[1] = rtb_Sum_h_idx_1;
    }

    if (rtb_Sum_h_idx_2 > 500.0) {
      DynModel_B->Saturation[2] = 500.0;
    } else if (rtb_Sum_h_idx_2 < (-500.0)) {
      DynModel_B->Saturation[2] = (-500.0);
    } else {
      DynModel_B->Saturation[2] = rtb_Sum_h_idx_2;
    }

    /* End of Saturate: '<S5>/Saturation' */
  }

  /* Sum: '<S5>/Add3' incorporates:
   *  Product: '<S5>/Matrix Multiply'
   */
  for (i = 0; i < 3; i++) {
    /* Switch: '<S5>/Switch3' incorporates:
     *  Constant: '<S5>/Constant4'
     *  Product: '<S5>/Matrix Multiply'
     */
    if (DynModel_B->Gain11[i] >= 0.0) {
      rtb_DiscreteTransferFcn_n = (DynModel_B->Reshape9to3x3columnmajor[i + 3] *
        DynModel_B->Gain13[1] + DynModel_B->Reshape9to3x3columnmajor[i] *
        DynModel_B->Gain13[0]) + DynModel_B->Reshape9to3x3columnmajor[i + 6] *
        DynModel_B->Gain13[2];
    } else {
      rtb_DiscreteTransferFcn_n = DynModel_ConstP.pooled11[i];
    }

    /* End of Switch: '<S5>/Switch3' */
    rtb_Add3_j[i] = (rtb_DiscreteTransferFcn_n + DynModel_B->Switch2[i]) +
      DynModel_B->Saturation[i];
  }

  /* End of Sum: '<S5>/Add3' */

  /* S-Function (sdspsubmtrx): '<S50>/Submatrix' */
  DynModel_B->Submatrix[0L] = DynModel_B->Reshape9to3x3columnmajor[6L];
  DynModel_B->Submatrix[1L] = DynModel_B->Reshape9to3x3columnmajor[7L];
  DynModel_B->Submatrix[2L] = DynModel_B->Reshape9to3x3columnmajor[8L];
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Memory: '<S2>/Memory2' */
    rtb_Memory2 = DynModel_DW->Memory2_PreviousInput;

    /* Gain: '<S48>/Gain1' incorporates:
     *  Constant: '<S6>/h_ref2'
     *  Product: '<S48>/Product1'
     */
    DynModel_B->Gain1[0] = 0.016813708498984763 * rtb_Memory2 * 0.5;
    DynModel_B->Gain1[1] = 0.018813708498984762 * rtb_Memory2 * 0.5;
    DynModel_B->Gain1[2] = 0.18845573684677208 * rtb_Memory2 * 0.5;

    /* Saturate: '<S2>/Saturation' incorporates:
     *  Inport: '<Root>/PWM1'
     */
    if (DynModel_U->PWM1 > 1.0) {
      rtb_DiscreteTransferFcn_n = 1.0;
    } else if (DynModel_U->PWM1 < 0.0) {
      rtb_DiscreteTransferFcn_n = 0.0;
    } else {
      rtb_DiscreteTransferFcn_n = DynModel_U->PWM1;
    }

    /* Gain: '<S52>/Gain1' incorporates:
     *  Gain: '<S8>/PWM2V'
     *  Gain: '<S8>/RPM2RADS'
     *  Gain: '<S8>/V2RPM'
     *  Saturate: '<S2>/Saturation'
     */
    rtb_Saturation = 12.0 * rtb_DiscreteTransferFcn_n * 950.0 *
      0.10471975511965977 * Kdiscre;

    /* DiscreteTransferFcn: '<S52>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp = (rtb_Saturation -
      (-0.97044334975369462) * DynModel_DW->DiscreteTransferFcn_states) / 1.0;
    rtb_DiscreteTransferFcn = 1.0 * DynModel_DW->DiscreteTransferFcn_tmp + 1.0 *
      DynModel_DW->DiscreteTransferFcn_states;

    /* Saturate: '<S2>/Saturation' incorporates:
     *  Inport: '<Root>/PWM2'
     */
    if (DynModel_U->PWM2 > 1.0) {
      rtb_DiscreteTransferFcn_n = 1.0;
    } else if (DynModel_U->PWM2 < 0.0) {
      rtb_DiscreteTransferFcn_n = 0.0;
    } else {
      rtb_DiscreteTransferFcn_n = DynModel_U->PWM2;
    }

    /* Gain: '<S53>/Gain1' incorporates:
     *  Gain: '<S8>/PWM2V'
     *  Gain: '<S8>/RPM2RADS'
     *  Gain: '<S8>/V2RPM'
     *  Saturate: '<S2>/Saturation'
     */
    rtb_Saturation = 12.0 * rtb_DiscreteTransferFcn_n * 950.0 *
      0.10471975511965977 * Kdiscre;

    /* DiscreteTransferFcn: '<S53>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_i = (rtb_Saturation -
      (-0.97044334975369462) * DynModel_DW->DiscreteTransferFcn_states_m) / 1.0;
    rtb_DiscreteTransferFcn_b = 1.0 * DynModel_DW->DiscreteTransferFcn_tmp_i +
      1.0 * DynModel_DW->DiscreteTransferFcn_states_m;

    /* Saturate: '<S2>/Saturation' incorporates:
     *  Inport: '<Root>/PWM3'
     */
    if (DynModel_U->PWM3 > 1.0) {
      rtb_DiscreteTransferFcn_n = 1.0;
    } else if (DynModel_U->PWM3 < 0.0) {
      rtb_DiscreteTransferFcn_n = 0.0;
    } else {
      rtb_DiscreteTransferFcn_n = DynModel_U->PWM3;
    }

    /* Gain: '<S54>/Gain1' incorporates:
     *  Gain: '<S8>/PWM2V'
     *  Gain: '<S8>/RPM2RADS'
     *  Gain: '<S8>/V2RPM'
     *  Saturate: '<S2>/Saturation'
     */
    rtb_Saturation = 12.0 * rtb_DiscreteTransferFcn_n * 950.0 *
      0.10471975511965977 * Kdiscre;

    /* DiscreteTransferFcn: '<S54>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_b = (rtb_Saturation -
      (-0.97044334975369462) * DynModel_DW->DiscreteTransferFcn_states_c) / 1.0;
    rtb_DiscreteTransferFcn_j = 1.0 * DynModel_DW->DiscreteTransferFcn_tmp_b +
      1.0 * DynModel_DW->DiscreteTransferFcn_states_c;

    /* Saturate: '<S2>/Saturation' incorporates:
     *  Inport: '<Root>/PWM4'
     */
    if (DynModel_U->PWM4 > 1.0) {
      rtb_DiscreteTransferFcn_n = 1.0;
    } else if (DynModel_U->PWM4 < 0.0) {
      rtb_DiscreteTransferFcn_n = 0.0;
    } else {
      rtb_DiscreteTransferFcn_n = DynModel_U->PWM4;
    }

    /* Gain: '<S55>/Gain1' incorporates:
     *  Gain: '<S8>/PWM2V'
     *  Gain: '<S8>/RPM2RADS'
     *  Gain: '<S8>/V2RPM'
     *  Saturate: '<S2>/Saturation'
     */
    rtb_Saturation = 12.0 * rtb_DiscreteTransferFcn_n * 950.0 *
      0.10471975511965977 * Kdiscre;

    /* DiscreteTransferFcn: '<S55>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_e = (rtb_Saturation -
      (-0.97044334975369462) * DynModel_DW->DiscreteTransferFcn_states_a) / 1.0;
    rtb_DiscreteTransferFcn_i = 1.0 * DynModel_DW->DiscreteTransferFcn_tmp_e +
      1.0 * DynModel_DW->DiscreteTransferFcn_states_a;

    /* MATLAB Function: '<S6>/multicopter' incorporates:
     *  Constant: '<S6>/Constant'
     *  Constant: '<S6>/Constant1'
     *  Constant: '<S6>/Constant2'
     *  SignalConversion: '<S51>/TmpSignal ConversionAt SFunction Inport2'
     */
    /* MATLAB Function 'DynModel/Dynamics/Dynamics/multicopter': '<S51>:1' */
    /* ===============================Parameters================================= */
    /*  l = [l];        Lenght of the Quadcopter arm */
    /*  Kt = [Kt];      Coeff. for the computation of thrust */
    /*  Kq = [Kq];      Coeff. for the computation of the torque */
    /* ==============================Actuator Mixer============================== */
    /*  [x(roll), y(pitch), z(yaw)] */
    /*  QUAD [X] */
    /* '<S51>:1:14' */
    /*  MIX = [  0   1  -1   ;      % QUAD [+] */
    /*          -1   0   1   ; */
    /*           0  -1  -1   ; */
    /*           1   0   1   ]; */
    /* ==============================  Forces  ==================================  */
    /* --------------------------------Thrust Model------------------------------ */
    /* '<S51>:1:26' */
    rtb_Saturation = 6.1235421348947671E-6 * rtb_Memory2;
    rtb_thrust_idx_0 = rtb_DiscreteTransferFcn * rtb_DiscreteTransferFcn *
      rtb_Saturation;
    rtb_thrust_idx_1 = rtb_DiscreteTransferFcn_b * rtb_DiscreteTransferFcn_b *
      rtb_Saturation;
    rtb_thrust_idx_2 = rtb_DiscreteTransferFcn_j * rtb_DiscreteTransferFcn_j *
      rtb_Saturation;
    rtb_thrust_idx_3 = rtb_DiscreteTransferFcn_i * rtb_DiscreteTransferFcn_i *
      rtb_Saturation;

    /*  rotor thrust */
    /* '<S51>:1:27' */
    rtb_Saturation = rtb_thrust_idx_0;
    rtb_Saturation += rtb_thrust_idx_1;
    rtb_Saturation += rtb_thrust_idx_2;
    rtb_Saturation += rtb_thrust_idx_3;
    DynModel_B->force[0] = 0.0;
    DynModel_B->force[1] = 0.0;
    DynModel_B->force[2] = -rtb_Saturation;

    /* ==================================Moments================================= */
    /*  Thrusts contributions to momentum */
    /* '<S51>:1:33' */
    /*  x moment */
    /* '<S51>:1:34' */
    /*  y moment */
    /* '<S51>:1:35' */
    /*  rotor torque */
    /* -------------------------------------------------------------------------- */
    /* '<S51>:1:39' */
    rtb_Saturation = rtb_thrust_idx_0 * 0.2 * 1.4142135623730951 / 2.0;
    rtb_Saturation += -rtb_thrust_idx_1 * 0.2 * 1.4142135623730951 / 2.0;
    rtb_Saturation += -rtb_thrust_idx_2 * 0.2 * 1.4142135623730951 / 2.0;
    rtb_Saturation += rtb_thrust_idx_3 * 0.2 * 1.4142135623730951 / 2.0;
    DynModel_B->moments[0] = rtb_Saturation;
    DynModel_B->moments[1] = ((rtb_thrust_idx_0 * 0.2 * 1.4142135623730951 / 2.0
      + rtb_thrust_idx_1 * 0.2 * 1.4142135623730951 / 2.0) + -rtb_thrust_idx_2 *
      0.2 * 1.4142135623730951 / 2.0) + -rtb_thrust_idx_3 * 0.2 *
      1.4142135623730951 / 2.0;
    DynModel_B->moments[2] = ((-3.564683251291932E-8 * rtb_Memory2 *
      (rtb_DiscreteTransferFcn * rtb_DiscreteTransferFcn) + 3.564683251291932E-8
      * rtb_Memory2 * (rtb_DiscreteTransferFcn_b * rtb_DiscreteTransferFcn_b)) +
      -3.564683251291932E-8 * rtb_Memory2 * (rtb_DiscreteTransferFcn_j *
      rtb_DiscreteTransferFcn_j)) + 3.564683251291932E-8 * rtb_Memory2 *
      (rtb_DiscreteTransferFcn_i * rtb_DiscreteTransferFcn_i);

    /*  - [momentum_x; momentum_y; momentum_z]; */
    /* ========================================================================== */
  }

  /* Sum: '<S2>/Add7' incorporates:
   *  Abs: '<S48>/Abs'
   *  Abs: '<S48>/Abs1'
   *  Abs: '<S48>/Abs2'
   *  Constant: '<S6>/h_ref1'
   *  Constant: '<S6>/h_ref7'
   *  Gain: '<S48>/Gain'
   *  Gain: '<S48>/Gain2'
   *  Inport: '<Root>/Fext'
   *  Product: '<S48>/Product'
   *  Product: '<S48>/Product3'
   *  Product: '<S48>/Product4'
   *  Product: '<S48>/Product5'
   *  Product: '<S50>/Product'
   *  Reshape: '<S50>/Reshape'
   *  Sum: '<S48>/Add'
   *  Sum: '<S6>/Add'
   */
  rtb_Add7_idx_0 = (((DynModel_B->UnitConversion[0] * fabs
                      (DynModel_B->UnitConversion[0]) * (-1.0) *
                      DynModel_ConstB.Gain_n + (-1.0) *
                      DynModel_B->UnitConversion[0] * 0.1) * DynModel_B->Gain1[0]
                     + 11.772 * DynModel_B->Submatrix[0]) + DynModel_B->force[0])
    + (DynModel_U->Fext[0] + rtb_Add3_j[0]);
  rtb_Add7_idx_1 = (((DynModel_B->UnitConversion[1] * fabs
                      (DynModel_B->UnitConversion[1]) * (-1.0) *
                      DynModel_ConstB.Gain_n + (-1.0) *
                      DynModel_B->UnitConversion[1] * 0.1) * DynModel_B->Gain1[1]
                     + 11.772 * DynModel_B->Submatrix[1]) + DynModel_B->force[1])
    + (DynModel_U->Fext[1] + rtb_Add3_j[1]);
  rtb_Add7_idx_2 = (((DynModel_B->UnitConversion[2] * fabs
                      (DynModel_B->UnitConversion[2]) * (-1.0) *
                      DynModel_ConstB.Gain_n + (-1.0) *
                      DynModel_B->UnitConversion[2] * 0.1) * DynModel_B->Gain1[2]
                     + 11.772 * DynModel_B->Submatrix[2]) + DynModel_B->force[2])
    + (DynModel_U->Fext[2] + rtb_Add3_j[2]);

  /* Product: '<S4>/Product' incorporates:
   *  Constant: '<S12>/Constant'
   *  Reshape: '<S2>/Reshape'
   */
  DynModel_B->Product[0] = rtb_Add7_idx_0 / 1.2;
  DynModel_B->Product[1] = rtb_Add7_idx_1 / 1.2;
  DynModel_B->Product[2] = rtb_Add7_idx_2 / 1.2;
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* ZeroOrderHold: '<S118>/Zero-Order Hold1' */
    rtb_Sum_h_idx_0 = DynModel_B->Product[0];
    rtb_Sum_h_idx_1 = DynModel_B->Product[1];
    rtb_Sum_h_idx_2 = DynModel_B->Product[2];
  }

  /* Product: '<S3>/Matrix Multiply1' incorporates:
   *  Constant: '<S3>/gravity'
   */
  for (i = 0; i < 3; i++) {
    DynModel_B->MatrixMultiply1[i] = 0.0;
    DynModel_B->MatrixMultiply1[i] += DynModel_B->Reshape9to3x3columnmajor[i] *
      0.0;
    DynModel_B->MatrixMultiply1[i] += DynModel_B->Reshape9to3x3columnmajor[i + 3]
      * 0.0;
    DynModel_B->MatrixMultiply1[i] += DynModel_B->Reshape9to3x3columnmajor[i + 6]
      * 9.81;
  }

  /* End of Product: '<S3>/Matrix Multiply1' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* ZeroOrderHold: '<S118>/Zero-Order Hold2' */
    rtb_ZeroOrderHold2_idx_0 = DynModel_B->MatrixMultiply1[0];
    rtb_ZeroOrderHold2_idx_1 = DynModel_B->MatrixMultiply1[1];
    rtb_ZeroOrderHold2_idx_2 = DynModel_B->MatrixMultiply1[2];
  }

  /* Integrator: '<S4>/p,q,r ' */
  DynModel_B->pqr[0] = DynModel_X->pqr_CSTATE[0];
  DynModel_B->pqr[1] = DynModel_X->pqr_CSTATE[1];
  DynModel_B->pqr[2] = DynModel_X->pqr_CSTATE[2];
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* ZeroOrderHold: '<S118>/Zero-Order Hold' */
    rtb_WhiteNoise[0] = DynModel_B->pqr[0];
    rtb_WhiteNoise[1] = DynModel_B->pqr[1];
    rtb_WhiteNoise[2] = DynModel_B->pqr[2];
  }

  /* Sqrt: '<S35>/sqrt' incorporates:
   *  Product: '<S36>/Product'
   *  Product: '<S36>/Product1'
   *  Product: '<S36>/Product2'
   *  Product: '<S36>/Product3'
   *  Sum: '<S36>/Sum'
   */
  rtb_Switch_d = sqrt(((rtb_q0q1q2q3[0] * rtb_q0q1q2q3[0] + rtb_q0q1q2q3[1] *
                        rtb_q0q1q2q3[1]) + rtb_q0q1q2q3[2] * rtb_q0q1q2q3[2]) +
                      rtb_q0q1q2q3[3] * rtb_q0q1q2q3[3]);

  /* Product: '<S34>/Product' */
  rtb_Sum_e = rtb_q0q1q2q3[0] / rtb_Switch_d;

  /* Product: '<S34>/Product1' */
  rtb_ixk = rtb_q0q1q2q3[1] / rtb_Switch_d;

  /* Product: '<S34>/Product2' */
  rtb_jxi = rtb_q0q1q2q3[2] / rtb_Switch_d;

  /* Product: '<S34>/Product3' */
  rtb_Switch_d = rtb_q0q1q2q3[3] / rtb_Switch_d;

  /* Trigonometry: '<S18>/Trigonometric Function1' incorporates:
   *  Fcn: '<S18>/fcn1'
   *  Fcn: '<S18>/fcn2'
   */
  DynModel_B->VectorConcatenate[0] = atan2((rtb_ixk * rtb_jxi + rtb_Sum_e *
    rtb_Switch_d) * 2.0, ((rtb_Sum_e * rtb_Sum_e + rtb_ixk * rtb_ixk) - rtb_jxi *
    rtb_jxi) - rtb_Switch_d * rtb_Switch_d);

  /* Fcn: '<S18>/fcn3' */
  rtb_DiscreteTransferFcn_n = (rtb_ixk * rtb_Switch_d - rtb_Sum_e * rtb_jxi) *
    -2.0;

  /* Trigonometry: '<S18>/trigFcn' */
  if (rtb_DiscreteTransferFcn_n > 1.0) {
    rtb_DiscreteTransferFcn_n = 1.0;
  } else {
    if (rtb_DiscreteTransferFcn_n < -1.0) {
      rtb_DiscreteTransferFcn_n = -1.0;
    }
  }

  DynModel_B->VectorConcatenate[1] = asin(rtb_DiscreteTransferFcn_n);

  /* End of Trigonometry: '<S18>/trigFcn' */

  /* Fcn: '<S18>/fcn4' */
  rtb_Saturation = (rtb_jxi * rtb_Switch_d + rtb_Sum_e * rtb_ixk) * 2.0;

  /* Fcn: '<S18>/fcn5' */
  rtb_Sum_e = ((rtb_Sum_e * rtb_Sum_e - rtb_ixk * rtb_ixk) - rtb_jxi * rtb_jxi)
    + rtb_Switch_d * rtb_Switch_d;

  /* Trigonometry: '<S18>/Trigonometric Function3' */
  DynModel_B->VectorConcatenate[2] = atan2(rtb_Saturation, rtb_Sum_e);
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Gain: '<S118>/Gain' incorporates:
     *  Constant: '<S118>/wl_ins'
     *  Constant: '<S3>/center of gravity'
     *  Sum: '<S118>/Sum7'
     *  ZeroOrderHold: '<S118>/Zero-Order Hold4'
     */
    rtb_Saturation_l_idx_0 = 0.0;
    rtb_Saturation_l_idx_1 = 0.0;
    rtb_Saturation_l_idx_2 = 0.0;

    /* Sum: '<S129>/Sum' incorporates:
     *  Product: '<S131>/i x j'
     *  Product: '<S131>/j x k'
     *  Product: '<S131>/k x i'
     *  Product: '<S132>/i x k'
     *  Product: '<S132>/j x i'
     *  Product: '<S132>/k x j'
     */
    rtb_Saturation_d_idx_0 = 0.0;
    rtb_Saturation_d_idx_1 = 0.0;
    rtb_Saturation_d_idx_2 = 0.0;

    /* Switch: '<S5>/Switch1' incorporates:
     *  Constant: '<S5>/Constant1'
     *  Gain: '<S5>/Gain'
     *  Gain: '<S5>/Gain4'
     *  Gain: '<S5>/Gain5'
     *  Gain: '<S5>/Gain6'
     *  Memory: '<S2>/Memory3'
     *  Memory: '<S2>/Memory5'
     *  Sum: '<S5>/Add1'
     */
    if (DynModel_B->Gain11[0] >= 0.0) {
      rtb_WhiteNoise[0] = (-1.0) * DynModel_DW->Memory5_PreviousInput[0] * 0.08
        + (-1.0) * DynModel_DW->Memory3_PreviousInput[0] * Kattreact;
    } else {
      rtb_WhiteNoise[0] = 0.0;
    }

    if (DynModel_B->Gain11[1] >= 0.0) {
      rtb_WhiteNoise[1] = (-1.0) * DynModel_DW->Memory5_PreviousInput[1] * 0.08
        + (-1.0) * DynModel_DW->Memory3_PreviousInput[1] * Kattreact;
    } else {
      rtb_WhiteNoise[1] = 0.0;
    }

    if (DynModel_B->Gain11[2] >= 0.0) {
      rtb_WhiteNoise[2] = (-1.0) * DynModel_DW->Memory5_PreviousInput[2] * 0.08
        + 0.0 * DynModel_DW->Memory3_PreviousInput[2] * Kattreact;
    } else {
      rtb_WhiteNoise[2] = 0.0;
    }

    /* End of Switch: '<S5>/Switch1' */

    /* Saturate: '<S5>/Saturation1' */
    if (rtb_WhiteNoise[0] > 50.0) {
      DynModel_B->Saturation1[0] = 50.0;
    } else if (rtb_WhiteNoise[0] < (-50.0)) {
      DynModel_B->Saturation1[0] = (-50.0);
    } else {
      DynModel_B->Saturation1[0] = rtb_WhiteNoise[0];
    }

    if (rtb_WhiteNoise[1] > 50.0) {
      DynModel_B->Saturation1[1] = 50.0;
    } else if (rtb_WhiteNoise[1] < (-50.0)) {
      DynModel_B->Saturation1[1] = (-50.0);
    } else {
      DynModel_B->Saturation1[1] = rtb_WhiteNoise[1];
    }

    if (rtb_WhiteNoise[2] > 50.0) {
      DynModel_B->Saturation1[2] = 50.0;
    } else if (rtb_WhiteNoise[2] < (-50.0)) {
      DynModel_B->Saturation1[2] = (-50.0);
    } else {
      DynModel_B->Saturation1[2] = rtb_WhiteNoise[2];
    }

    /* End of Saturate: '<S5>/Saturation1' */

    /* Product: '<S47>/Product2' incorporates:
     *  Constant: '<S6>/h_ref2'
     *  Constant: '<S6>/h_ref6'
     *  Product: '<S47>/Product1'
     */
    DynModel_B->Product2_p[0] = 0.016813708498984763 * rtb_Memory2 * 0.3;
    DynModel_B->Product2_p[1] = 0.018813708498984762 * rtb_Memory2 * 0.3;
    DynModel_B->Product2_p[2] = 0.18845573684677208 * rtb_Memory2 * 0.3;
  }

  /* Sum: '<S2>/Add6' incorporates:
   *  Gain: '<S47>/Gain'
   *  Product: '<S47>/Product3'
   *  Sum: '<S6>/Add1'
   */
  rtb_ixk = ((-1.0) * DynModel_B->pqr[0] * DynModel_B->Product2_p[0] +
             DynModel_B->moments[0]) + DynModel_B->Saturation1[0];
  rtb_Sum_e = ((-1.0) * DynModel_B->pqr[1] * DynModel_B->Product2_p[1] +
               DynModel_B->moments[1]) + DynModel_B->Saturation1[1];
  rtb_Sum_j = ((-1.0) * DynModel_B->pqr[2] * DynModel_B->Product2_p[2] +
               DynModel_B->moments[2]) + DynModel_B->Saturation1[2];

  /* Product: '<S37>/Product' incorporates:
   *  Reshape: '<S37>/Reshape1'
   */
  for (i = 0; i < 3; i++) {
    rtb_Add2[i] = DynModel_ConstB.Selector[i + 6] * DynModel_B->pqr[2] +
      (DynModel_ConstB.Selector[i + 3] * DynModel_B->pqr[1] +
       DynModel_ConstB.Selector[i] * DynModel_B->pqr[0]);
  }

  /* End of Product: '<S37>/Product' */

  /* Product: '<S38>/Product' incorporates:
   *  Reshape: '<S38>/Reshape1'
   */
  for (i = 0; i < 3; i++) {
    tmp[i] = DynModel_ConstB.Selector1[i + 6] * DynModel_B->pqr[2] +
      (DynModel_ConstB.Selector1[i + 3] * DynModel_B->pqr[1] +
       DynModel_ConstB.Selector1[i] * DynModel_B->pqr[0]);
  }

  /* Product: '<S11>/Product2' incorporates:
   *  Product: '<S38>/Product'
   *  Product: '<S40>/i x j'
   *  Product: '<S40>/j x k'
   *  Product: '<S40>/k x i'
   *  Product: '<S41>/i x k'
   *  Product: '<S41>/j x i'
   *  Product: '<S41>/k x j'
   *  Reshape: '<S11>/Reshape'
   *  Reshape: '<S2>/Reshape1'
   *  Sum: '<S11>/Sum2'
   *  Sum: '<S39>/Sum'
   */
  rtb_Add6_0[0] = (rtb_ixk - tmp[0]) - (DynModel_B->pqr[1] * rtb_Add2[2] -
    DynModel_B->pqr[2] * rtb_Add2[1]);
  rtb_Add6_0[1] = (rtb_Sum_e - tmp[1]) - (DynModel_B->pqr[2] * rtb_Add2[0] -
    DynModel_B->pqr[0] * rtb_Add2[2]);
  rtb_Add6_0[2] = (rtb_Sum_j - tmp[2]) - (DynModel_B->pqr[0] * rtb_Add2[1] -
    DynModel_B->pqr[1] * rtb_Add2[0]);
  rt_mrdivide_U1d1x3_U2d3x3_Yd1x3(rtb_Add6_0, DynModel_ConstB.Selector2,
    DynModel_B->Reshape);
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Sum: '<S118>/Sum' */
    rtb_Sum_h_idx_0 = (rtb_Sum_h_idx_0 - rtb_ZeroOrderHold2_idx_0) +
      rtb_Saturation_d_idx_0;
    rtb_Sum_h_idx_1 = (rtb_Sum_h_idx_1 - rtb_ZeroOrderHold2_idx_1) +
      rtb_Saturation_d_idx_1;
    rtb_DiscreteTransferFcn_n = (rtb_Sum_h_idx_2 - rtb_ZeroOrderHold2_idx_2) +
      rtb_Saturation_d_idx_2;

    /* Sum: '<S118>/Sum4' incorporates:
     *  Constant: '<S118>/Measurement bias'
     *  Constant: '<S118>/Scale factors & Cross-coupling  errors'
     *  Product: '<S118>/Product'
     */
    for (i = 0; i < 3; i++) {
      rtb_Add2[i] = ((DynModel_ConstP.pooled23[i + 3] * rtb_Sum_h_idx_1 +
                      DynModel_ConstP.pooled23[i] * rtb_Sum_h_idx_0) +
                     DynModel_ConstP.pooled23[i + 6] * rtb_DiscreteTransferFcn_n)
        + DynModel_ConstP.pooled11[i];
    }

    /* End of Sum: '<S118>/Sum4' */

    /* DiscreteTransferFcn: '<S126>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_k = ((rtb_Add2[0] -
      (-1.4775131058585489) * DynModel_DW->DiscreteTransferFcn_states_h[0L]) -
      0.58818480026978159 * DynModel_DW->DiscreteTransferFcn_states_h[1L]) / 1.0;

    /* DiscreteTransferFcn: '<S127>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_o = ((rtb_Add2[1] -
      (-1.4775131058585489) * DynModel_DW->DiscreteTransferFcn_states_b[0L]) -
      0.58818480026978159 * DynModel_DW->DiscreteTransferFcn_states_b[1L]) / 1.0;

    /* DiscreteTransferFcn: '<S128>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_j = ((rtb_Add2[2] -
      (-1.4775131058585489) * DynModel_DW->DiscreteTransferFcn_states_cb[0L]) -
      0.58818480026978159 * DynModel_DW->DiscreteTransferFcn_states_cb[1L]) /
      1.0;

    /* Switch: '<S120>/Switch' incorporates:
     *  Constant: '<S120>/Constant'
     *  DiscreteTransferFcn: '<S126>/Discrete Transfer Fcn'
     */
    if (0.0 >= 0.5) {
      rtb_DiscreteTransferFcn_n = (0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_tmp_k + 0.055335847205616372 *
        DynModel_DW->DiscreteTransferFcn_states_h[0L]) + 0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_states_h[1L];
    } else {
      rtb_DiscreteTransferFcn_n = rtb_Add2[0];
    }

    /* Sum: '<S118>/Sum1' incorporates:
     *  Gain: '<S121>/Output'
     *  RandomNumber: '<S121>/White Noise'
     */
    rtb_Saturation_d_idx_0 = 0.015811388300841896 * DynModel_DW->NextOutput_k[0]
      + rtb_DiscreteTransferFcn_n;

    /* Saturate: '<S118>/Saturation' */
    rtb_DiscreteTransferFcn_n = (-19.62);
    rtb_jxi = 19.62;
    if (rtb_Saturation_d_idx_0 > rtb_jxi) {
      rtb_Saturation_d_idx_0 = rtb_jxi;
    } else {
      if (rtb_Saturation_d_idx_0 < rtb_DiscreteTransferFcn_n) {
        rtb_Saturation_d_idx_0 = rtb_DiscreteTransferFcn_n;
      }
    }

    /* Switch: '<S120>/Switch' incorporates:
     *  Constant: '<S120>/Constant'
     *  DiscreteTransferFcn: '<S127>/Discrete Transfer Fcn'
     */
    if (0.0 >= 0.5) {
      rtb_DiscreteTransferFcn_n = (0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_tmp_o + 0.055335847205616372 *
        DynModel_DW->DiscreteTransferFcn_states_b[0L]) + 0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_states_b[1L];
    } else {
      rtb_DiscreteTransferFcn_n = rtb_Add2[1];
    }

    /* Sum: '<S118>/Sum1' incorporates:
     *  Gain: '<S121>/Output'
     *  RandomNumber: '<S121>/White Noise'
     */
    rtb_Saturation_d_idx_1 = 0.015811388300841896 * DynModel_DW->NextOutput_k[1]
      + rtb_DiscreteTransferFcn_n;

    /* Saturate: '<S118>/Saturation' */
    rtb_DiscreteTransferFcn_n = (-19.62);
    rtb_jxi = 19.62;
    if (rtb_Saturation_d_idx_1 > rtb_jxi) {
      rtb_Saturation_d_idx_1 = rtb_jxi;
    } else {
      if (rtb_Saturation_d_idx_1 < rtb_DiscreteTransferFcn_n) {
        rtb_Saturation_d_idx_1 = rtb_DiscreteTransferFcn_n;
      }
    }

    /* Switch: '<S120>/Switch' incorporates:
     *  Constant: '<S120>/Constant'
     *  DiscreteTransferFcn: '<S128>/Discrete Transfer Fcn'
     */
    if (0.0 >= 0.5) {
      rtb_DiscreteTransferFcn_n = (0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_tmp_j + 0.055335847205616372 *
        DynModel_DW->DiscreteTransferFcn_states_cb[0L]) + 0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_states_cb[1L];
    } else {
      rtb_DiscreteTransferFcn_n = rtb_Add2[2];
    }

    /* Sum: '<S118>/Sum1' incorporates:
     *  Gain: '<S121>/Output'
     *  RandomNumber: '<S121>/White Noise'
     */
    rtb_Saturation_d_idx_2 = 0.015811388300841896 * DynModel_DW->NextOutput_k[2]
      + rtb_DiscreteTransferFcn_n;

    /* Saturate: '<S118>/Saturation' */
    rtb_DiscreteTransferFcn_n = (-19.62);
    rtb_jxi = 19.62;
    if (rtb_Saturation_d_idx_2 > rtb_jxi) {
      rtb_Saturation_d_idx_2 = rtb_jxi;
    } else {
      if (rtb_Saturation_d_idx_2 < rtb_DiscreteTransferFcn_n) {
        rtb_Saturation_d_idx_2 = rtb_DiscreteTransferFcn_n;
      }
    }

    /* ZeroOrderHold: '<S119>/Zero-Order Hold' */
    rtb_Saturation_l_idx_0 = DynModel_B->pqr[0];
    rtb_Saturation_l_idx_1 = DynModel_B->pqr[1];
    rtb_Saturation_l_idx_2 = DynModel_B->pqr[2];

    /* Product: '<S119>/Product' incorporates:
     *  Constant: '<S119>/Scale factors & Cross-coupling  errors '
     *  ZeroOrderHold: '<S119>/Zero-Order Hold'
     */
    for (i = 0; i < 3; i++) {
      rtb_WhiteNoise[i] = DynModel_ConstP.pooled23[i + 6] * DynModel_B->pqr[2] +
        (DynModel_ConstP.pooled23[i + 3] * DynModel_B->pqr[1] +
         DynModel_ConstP.pooled23[i] * DynModel_B->pqr[0]);
    }

    /* End of Product: '<S119>/Product' */
  }

  /* Gain: '<S117>/Unit Conversion' */
  DynModel_B->UnitConversion_h[0] = 0.10197162129779283 * DynModel_B->Product[0];
  DynModel_B->UnitConversion_h[1] = 0.10197162129779283 * DynModel_B->Product[1];
  DynModel_B->UnitConversion_h[2] = 0.10197162129779283 * DynModel_B->Product[2];
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Sum: '<S119>/Sum4' incorporates:
     *  Constant: '<S119>/Measurement bias'
     *  Constant: '<S119>/g-sensitive bias'
     *  Product: '<S119>/Product1'
     *  ZeroOrderHold: '<S119>/Zero-Order Hold1'
     */
    rtb_Add2[0] = (rtb_WhiteNoise[0] + 0.0) + DynModel_B->UnitConversion_h[0] *
      0.0;
    rtb_Add2[1] = (rtb_WhiteNoise[1] + 0.0) + DynModel_B->UnitConversion_h[1] *
      0.0;
    rtb_Add2[2] = (rtb_WhiteNoise[2] + 0.0) + DynModel_B->UnitConversion_h[2] *
      0.0;

    /* DiscreteTransferFcn: '<S141>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_g = ((rtb_Add2[0] -
      (-1.4775131058585489) * DynModel_DW->DiscreteTransferFcn_states_d[0L]) -
      0.58818480026978159 * DynModel_DW->DiscreteTransferFcn_states_d[1L]) / 1.0;

    /* DiscreteTransferFcn: '<S142>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_n = ((rtb_Add2[1] -
      (-1.4775131058585489) * DynModel_DW->DiscreteTransferFcn_states_dr[0L]) -
      0.58818480026978159 * DynModel_DW->DiscreteTransferFcn_states_dr[1L]) /
      1.0;

    /* DiscreteTransferFcn: '<S143>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_tmp_gx = ((rtb_Add2[2] -
      (-1.4775131058585489) * DynModel_DW->DiscreteTransferFcn_states_as[0L]) -
      0.58818480026978159 * DynModel_DW->DiscreteTransferFcn_states_as[1L]) /
      1.0;

    /* Switch: '<S137>/Switch' incorporates:
     *  Constant: '<S137>/Constant'
     *  DiscreteTransferFcn: '<S141>/Discrete Transfer Fcn'
     */
    if (0.0 >= 0.5) {
      rtb_DiscreteTransferFcn_n = (0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_tmp_g + 0.055335847205616372 *
        DynModel_DW->DiscreteTransferFcn_states_d[0L]) + 0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_states_d[1L];
    } else {
      rtb_DiscreteTransferFcn_n = rtb_Add2[0];
    }

    /* Sum: '<S119>/Sum1' incorporates:
     *  Gain: '<S138>/Output'
     *  RandomNumber: '<S138>/White Noise'
     */
    rtb_Saturation_l_idx_0 = 0.001 * DynModel_DW->NextOutput_p[0] +
      rtb_DiscreteTransferFcn_n;

    /* Saturate: '<S119>/Saturation' */
    rtb_DiscreteTransferFcn_n = (-4.36);
    rtb_jxi = 4.36;
    if (rtb_Saturation_l_idx_0 > rtb_jxi) {
      rtb_Saturation_l_idx_0 = rtb_jxi;
    } else {
      if (rtb_Saturation_l_idx_0 < rtb_DiscreteTransferFcn_n) {
        rtb_Saturation_l_idx_0 = rtb_DiscreteTransferFcn_n;
      }
    }

    /* Switch: '<S137>/Switch' incorporates:
     *  Constant: '<S137>/Constant'
     *  DiscreteTransferFcn: '<S142>/Discrete Transfer Fcn'
     */
    if (0.0 >= 0.5) {
      rtb_DiscreteTransferFcn_n = (0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_tmp_n + 0.055335847205616372 *
        DynModel_DW->DiscreteTransferFcn_states_dr[0L]) + 0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_states_dr[1L];
    } else {
      rtb_DiscreteTransferFcn_n = rtb_Add2[1];
    }

    /* Sum: '<S119>/Sum1' incorporates:
     *  Gain: '<S138>/Output'
     *  RandomNumber: '<S138>/White Noise'
     */
    rtb_Saturation_l_idx_1 = 0.001 * DynModel_DW->NextOutput_p[1] +
      rtb_DiscreteTransferFcn_n;

    /* Saturate: '<S119>/Saturation' */
    rtb_DiscreteTransferFcn_n = (-4.36);
    rtb_jxi = 4.36;
    if (rtb_Saturation_l_idx_1 > rtb_jxi) {
      rtb_Saturation_l_idx_1 = rtb_jxi;
    } else {
      if (rtb_Saturation_l_idx_1 < rtb_DiscreteTransferFcn_n) {
        rtb_Saturation_l_idx_1 = rtb_DiscreteTransferFcn_n;
      }
    }

    /* Switch: '<S137>/Switch' incorporates:
     *  Constant: '<S137>/Constant'
     *  DiscreteTransferFcn: '<S143>/Discrete Transfer Fcn'
     */
    if (0.0 >= 0.5) {
      rtb_DiscreteTransferFcn_n = (0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_tmp_gx + 0.055335847205616372 *
        DynModel_DW->DiscreteTransferFcn_states_as[0L]) + 0.027667923602808186 *
        DynModel_DW->DiscreteTransferFcn_states_as[1L];
    } else {
      rtb_DiscreteTransferFcn_n = rtb_Add2[2];
    }

    /* Sum: '<S119>/Sum1' incorporates:
     *  Gain: '<S138>/Output'
     *  RandomNumber: '<S138>/White Noise'
     */
    rtb_Saturation_l_idx_2 = 0.001 * DynModel_DW->NextOutput_p[2] +
      rtb_DiscreteTransferFcn_n;

    /* Saturate: '<S119>/Saturation' */
    rtb_DiscreteTransferFcn_n = (-4.36);
    rtb_jxi = 4.36;
    if (rtb_Saturation_l_idx_2 > rtb_jxi) {
      rtb_Saturation_l_idx_2 = rtb_jxi;
    } else {
      if (rtb_Saturation_l_idx_2 < rtb_DiscreteTransferFcn_n) {
        rtb_Saturation_l_idx_2 = rtb_DiscreteTransferFcn_n;
      }
    }
  }

  /* Sum: '<S75>/Add' incorporates:
   *  Reshape: '<S62>/Reshape 3x3 -> 9'
   */
  rtb_Add2[0] = DynModel_B->Reshape9to3x3columnmajor[0];
  rtb_Add2[1] = DynModel_B->Reshape9to3x3columnmajor[4];
  rtb_Add2[2] = DynModel_B->Reshape9to3x3columnmajor[8];
  rtb_DiscreteTransferFcn_n = (rtb_Add2[0L] + rtb_Add2[1L]) + rtb_Add2[2L];

  /* If: '<S62>/If' */
  if (rtmIsMajorTimeStep(DynModel_M)) {
    rtAction = (int8_T)!(rtb_DiscreteTransferFcn_n > 0.0);
    DynModel_DW->If_ActiveSubsystem = rtAction;
  } else {
    rtAction = DynModel_DW->If_ActiveSubsystem;
  }

  switch (rtAction) {
   case 0L:
    /* Outputs for IfAction SubSystem: '<S62>/Positive Trace' incorporates:
     *  ActionPort: '<S74>/Action Port'
     */
    /* Sqrt: '<S74>/sqrt' incorporates:
     *  Constant: '<S74>/Constant'
     *  Sum: '<S74>/Sum'
     */
    rtb_Saturation = sqrt(rtb_DiscreteTransferFcn_n + 1.0);

    /* Gain: '<S74>/Gain' */
    DynModel_B->Merge[0] = 0.5 * rtb_Saturation;

    /* Gain: '<S74>/Gain1' */
    rtb_Saturation *= 2.0;

    /* Product: '<S74>/Product' incorporates:
     *  Reshape: '<S62>/Reshape 3x3 -> 9'
     *  Sum: '<S95>/Add'
     *  Sum: '<S96>/Add'
     *  Sum: '<S97>/Add'
     */
    DynModel_B->Merge[1] = (DynModel_B->Reshape9to3x3columnmajor[7] -
      DynModel_B->Reshape9to3x3columnmajor[5]) / rtb_Saturation;
    DynModel_B->Merge[2] = (DynModel_B->Reshape9to3x3columnmajor[2] -
      DynModel_B->Reshape9to3x3columnmajor[6]) / rtb_Saturation;
    DynModel_B->Merge[3] = (DynModel_B->Reshape9to3x3columnmajor[3] -
      DynModel_B->Reshape9to3x3columnmajor[1]) / rtb_Saturation;

    /* End of Outputs for SubSystem: '<S62>/Positive Trace' */
    break;

   case 1L:
    /* Outputs for IfAction SubSystem: '<S62>/Negative Trace' incorporates:
     *  ActionPort: '<S73>/Action Port'
     */
    /* If: '<S73>/Find Maximum Diagonal Value' incorporates:
     *  Reshape: '<S62>/Reshape 3x3 -> 9'
     */
    if (rtmIsMajorTimeStep(DynModel_M)) {
      if ((DynModel_B->Reshape9to3x3columnmajor[4] >
           DynModel_B->Reshape9to3x3columnmajor[0]) &&
          (DynModel_B->Reshape9to3x3columnmajor[4] >
           DynModel_B->Reshape9to3x3columnmajor[8])) {
        rtAction = 0;
      } else if (DynModel_B->Reshape9to3x3columnmajor[8] >
                 DynModel_B->Reshape9to3x3columnmajor[0]) {
        rtAction = 1;
      } else {
        rtAction = 2;
      }

      DynModel_DW->FindMaximumDiagonalValue_ActiveSubsystem = rtAction;
    } else {
      rtAction = DynModel_DW->FindMaximumDiagonalValue_ActiveSubsystem;
    }

    switch (rtAction) {
     case 0L:
      /* Outputs for IfAction SubSystem: '<S73>/Maximum Value at DCM(2,2)' incorporates:
       *  ActionPort: '<S77>/Action Port'
       */
      /* Sqrt: '<S77>/sqrt' incorporates:
       *  Constant: '<S89>/Constant'
       *  Reshape: '<S62>/Reshape 3x3 -> 9'
       *  Sum: '<S89>/Add'
       */
      rtb_Saturation = sqrt(((DynModel_B->Reshape9to3x3columnmajor[4] -
        DynModel_B->Reshape9to3x3columnmajor[0]) -
        DynModel_B->Reshape9to3x3columnmajor[8]) + 1.0);

      /* Gain: '<S77>/Gain' */
      DynModel_B->Merge[2] = 0.5 * rtb_Saturation;

      /* Switch: '<S88>/Switch' incorporates:
       *  Constant: '<S88>/Constant1'
       *  Constant: '<S88>/Constant2'
       */
      if (rtb_Saturation != 0.0) {
        rtb_Sum_c_idx_0 = 0.5;
      } else {
        rtb_Sum_c_idx_0 = 0.0;
        rtb_Saturation = 1.0;
      }

      /* End of Switch: '<S88>/Switch' */

      /* Product: '<S88>/Product' */
      rtb_Saturation = rtb_Sum_c_idx_0 / rtb_Saturation;

      /* Product: '<S77>/Product' incorporates:
       *  Reshape: '<S62>/Reshape 3x3 -> 9'
       *  Sum: '<S85>/Add'
       *  Sum: '<S86>/Add'
       *  Sum: '<S87>/Add'
       */
      rtb_Add2[0] = (DynModel_B->Reshape9to3x3columnmajor[1] +
                     DynModel_B->Reshape9to3x3columnmajor[3]) * rtb_Saturation;
      rtb_Add2[1] = (DynModel_B->Reshape9to3x3columnmajor[5] +
                     DynModel_B->Reshape9to3x3columnmajor[7]) * rtb_Saturation;
      rtb_Add2[2] = (DynModel_B->Reshape9to3x3columnmajor[2] -
                     DynModel_B->Reshape9to3x3columnmajor[6]) * rtb_Saturation;

      /* Gain: '<S77>/Gain1' */
      DynModel_B->Merge[1] = 1.0 * rtb_Add2[0];

      /* Gain: '<S77>/Gain3' */
      DynModel_B->Merge[3] = 1.0 * rtb_Add2[1];

      /* Gain: '<S77>/Gain4' */
      DynModel_B->Merge[0] = 1.0 * rtb_Add2[2];

      /* End of Outputs for SubSystem: '<S73>/Maximum Value at DCM(2,2)' */
      break;

     case 1L:
      /* Outputs for IfAction SubSystem: '<S73>/Maximum Value at DCM(3,3)' incorporates:
       *  ActionPort: '<S78>/Action Port'
       */
      /* Sqrt: '<S78>/sqrt' incorporates:
       *  Constant: '<S94>/Constant'
       *  Reshape: '<S62>/Reshape 3x3 -> 9'
       *  Sum: '<S94>/Add'
       */
      rtb_Saturation = sqrt(((DynModel_B->Reshape9to3x3columnmajor[8] -
        DynModel_B->Reshape9to3x3columnmajor[0]) -
        DynModel_B->Reshape9to3x3columnmajor[4]) + 1.0);

      /* Gain: '<S78>/Gain' */
      DynModel_B->Merge[3] = 0.5 * rtb_Saturation;

      /* Switch: '<S93>/Switch' incorporates:
       *  Constant: '<S93>/Constant1'
       *  Constant: '<S93>/Constant2'
       */
      if (rtb_Saturation != 0.0) {
        rtb_Sum_c_idx_0 = 0.5;
      } else {
        rtb_Sum_c_idx_0 = 0.0;
        rtb_Saturation = 1.0;
      }

      /* End of Switch: '<S93>/Switch' */

      /* Product: '<S93>/Product' */
      rtb_Saturation = rtb_Sum_c_idx_0 / rtb_Saturation;

      /* Product: '<S78>/Product' incorporates:
       *  Reshape: '<S62>/Reshape 3x3 -> 9'
       *  Sum: '<S90>/Add'
       *  Sum: '<S91>/Add'
       *  Sum: '<S92>/Add'
       */
      rtb_Add2[0] = (DynModel_B->Reshape9to3x3columnmajor[2] +
                     DynModel_B->Reshape9to3x3columnmajor[6]) * rtb_Saturation;
      rtb_Add2[1] = (DynModel_B->Reshape9to3x3columnmajor[5] +
                     DynModel_B->Reshape9to3x3columnmajor[7]) * rtb_Saturation;
      rtb_Add2[2] = (DynModel_B->Reshape9to3x3columnmajor[3] -
                     DynModel_B->Reshape9to3x3columnmajor[1]) * rtb_Saturation;

      /* Gain: '<S78>/Gain1' */
      DynModel_B->Merge[1] = 1.0 * rtb_Add2[0];

      /* Gain: '<S78>/Gain2' */
      DynModel_B->Merge[2] = 1.0 * rtb_Add2[1];

      /* Gain: '<S78>/Gain3' */
      DynModel_B->Merge[0] = 1.0 * rtb_Add2[2];

      /* End of Outputs for SubSystem: '<S73>/Maximum Value at DCM(3,3)' */
      break;

     case 2L:
      /* Outputs for IfAction SubSystem: '<S73>/Maximum Value at DCM(1,1)' incorporates:
       *  ActionPort: '<S76>/Action Port'
       */
      /* Sqrt: '<S76>/sqrt' incorporates:
       *  Constant: '<S84>/Constant'
       *  Reshape: '<S62>/Reshape 3x3 -> 9'
       *  Sum: '<S84>/Add'
       */
      rtb_Saturation = sqrt(((DynModel_B->Reshape9to3x3columnmajor[0] -
        DynModel_B->Reshape9to3x3columnmajor[4]) -
        DynModel_B->Reshape9to3x3columnmajor[8]) + 1.0);

      /* Gain: '<S76>/Gain' */
      DynModel_B->Merge[1] = 0.5 * rtb_Saturation;

      /* Switch: '<S83>/Switch' incorporates:
       *  Constant: '<S83>/Constant1'
       *  Constant: '<S83>/Constant2'
       */
      if (rtb_Saturation != 0.0) {
        rtb_Sum_c_idx_0 = 0.5;
      } else {
        rtb_Sum_c_idx_0 = 0.0;
        rtb_Saturation = 1.0;
      }

      /* End of Switch: '<S83>/Switch' */

      /* Product: '<S83>/Product' */
      rtb_Saturation = rtb_Sum_c_idx_0 / rtb_Saturation;

      /* Product: '<S76>/Product' incorporates:
       *  Reshape: '<S62>/Reshape 3x3 -> 9'
       *  Sum: '<S80>/Add'
       *  Sum: '<S81>/Add'
       *  Sum: '<S82>/Add'
       */
      rtb_Add2[0] = (DynModel_B->Reshape9to3x3columnmajor[1] +
                     DynModel_B->Reshape9to3x3columnmajor[3]) * rtb_Saturation;
      rtb_Add2[1] = (DynModel_B->Reshape9to3x3columnmajor[2] +
                     DynModel_B->Reshape9to3x3columnmajor[6]) * rtb_Saturation;
      rtb_Add2[2] = (DynModel_B->Reshape9to3x3columnmajor[7] -
                     DynModel_B->Reshape9to3x3columnmajor[5]) * rtb_Saturation;

      /* Gain: '<S76>/Gain1' */
      DynModel_B->Merge[2] = 1.0 * rtb_Add2[0];

      /* Gain: '<S76>/Gain2' */
      DynModel_B->Merge[3] = 1.0 * rtb_Add2[1];

      /* Gain: '<S76>/Gain3' */
      DynModel_B->Merge[0] = 1.0 * rtb_Add2[2];

      /* End of Outputs for SubSystem: '<S73>/Maximum Value at DCM(1,1)' */
      break;
    }

    /* End of If: '<S73>/Find Maximum Diagonal Value' */
    /* End of Outputs for SubSystem: '<S62>/Negative Trace' */
    break;
  }

  /* End of If: '<S62>/If' */

  /* Gain: '<S20>/High Gain Quaternion Normalization' incorporates:
   *  Constant: '<S20>/Constant'
   *  DotProduct: '<S20>/Dot Product'
   *  Sum: '<S20>/Sum'
   */
  rtb_Saturation = (1.0 - (((rtb_q0q1q2q3[0] * rtb_q0q1q2q3[0] + rtb_q0q1q2q3[1]
    * rtb_q0q1q2q3[1]) + rtb_q0q1q2q3[2] * rtb_q0q1q2q3[2]) + rtb_q0q1q2q3[3] *
    rtb_q0q1q2q3[3])) * 1.0;

  /* Fcn: '<S20>/q0dot' */
  DynModel_B->q0dot = ((rtb_q0q1q2q3[1] * DynModel_B->pqr[0] + rtb_q0q1q2q3[2] *
                        DynModel_B->pqr[1]) + rtb_q0q1q2q3[3] * DynModel_B->pqr
                       [2]) * -0.5 + rtb_Saturation * rtb_q0q1q2q3[0];

  /* Fcn: '<S20>/q1dot' */
  DynModel_B->q1dot = ((rtb_q0q1q2q3[0] * DynModel_B->pqr[0] + rtb_q0q1q2q3[2] *
                        DynModel_B->pqr[2]) - rtb_q0q1q2q3[3] * DynModel_B->pqr
                       [1]) * 0.5 + rtb_Saturation * rtb_q0q1q2q3[1];

  /* Fcn: '<S20>/q2dot' */
  DynModel_B->q2dot = ((rtb_q0q1q2q3[0] * DynModel_B->pqr[1] + rtb_q0q1q2q3[3] *
                        DynModel_B->pqr[0]) - rtb_q0q1q2q3[1] * DynModel_B->pqr
                       [2]) * 0.5 + rtb_Saturation * rtb_q0q1q2q3[2];

  /* Fcn: '<S20>/q3dot' */
  DynModel_B->q3dot = ((rtb_q0q1q2q3[0] * DynModel_B->pqr[2] + rtb_q0q1q2q3[1] *
                        DynModel_B->pqr[1]) - rtb_q0q1q2q3[2] * DynModel_B->pqr
                       [0]) * 0.5 + rtb_Saturation * rtb_q0q1q2q3[3];

  /* Sum: '<S4>/Sum' incorporates:
   *  Product: '<S42>/i x j'
   *  Product: '<S42>/j x k'
   *  Product: '<S42>/k x i'
   *  Product: '<S43>/i x k'
   *  Product: '<S43>/j x i'
   *  Product: '<S43>/k x j'
   *  Sum: '<S13>/Sum'
   */
  DynModel_B->Sum[0] = (rtb_ubvbwb_idx_1 * DynModel_B->pqr[2] - rtb_ubvbwb_idx_2
                        * DynModel_B->pqr[1]) + DynModel_B->Product[0];
  DynModel_B->Sum[1] = (rtb_ubvbwb_idx_2 * DynModel_B->pqr[0] - rtb_ubvbwb_idx_0
                        * DynModel_B->pqr[2]) + DynModel_B->Product[1];
  DynModel_B->Sum[2] = (rtb_ubvbwb_idx_0 * DynModel_B->pqr[1] - rtb_ubvbwb_idx_1
                        * DynModel_B->pqr[0]) + DynModel_B->Product[2];
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* DataTypeConversion: '<S1>/Data Type Conversion14' */
    rtb_DataTypeConversion14_idx_0 = (real32_T)rtb_Saturation_d_idx_0;
    rtb_DataTypeConversion14_idx_1 = (real32_T)rtb_Saturation_d_idx_1;
    rtb_DataTypeConversion14_idx_2 = (real32_T)rtb_Saturation_d_idx_2;

    /* DataTypeConversion: '<S1>/Data Type Conversion15' */
    rtb_DataTypeConversion15_idx_0 = (real32_T)rtb_Saturation_l_idx_0;
    rtb_DataTypeConversion15_idx_1 = (real32_T)rtb_Saturation_l_idx_1;
    rtb_DataTypeConversion15_idx_2 = (real32_T)rtb_Saturation_l_idx_2;

    /* DataTypeConversion: '<S1>/Data Type Conversion20' */
    rtb_DataTypeConversion20_idx_0 = (real32_T)rtb_thrust_idx_0;
    rtb_DataTypeConversion20_idx_1 = (real32_T)rtb_thrust_idx_1;
    rtb_DataTypeConversion20_idx_2 = (real32_T)rtb_thrust_idx_2;
    rtb_DataTypeConversion20_idx_3 = (real32_T)rtb_thrust_idx_3;

    /* DataTypeConversion: '<S1>/Data Type Conversion21' */
    rtb_DataTypeConversion21_idx_0 = (real32_T)rtb_DiscreteTransferFcn;
    rtb_DataTypeConversion21_idx_1 = (real32_T)rtb_DiscreteTransferFcn_b;
    rtb_DataTypeConversion21_idx_2 = (real32_T)rtb_DiscreteTransferFcn_j;
    rtb_DataTypeConversion21_idx_3 = (real32_T)rtb_DiscreteTransferFcn_i;

    /* DataTypeConversion: '<S1>/Data Type Conversion12' */
    rtb_DataTypeConversion12_idx_0 = (real32_T)rtb_Saturation_gt_idx_0;
    rtb_DataTypeConversion12_idx_1 = (real32_T)rtb_Saturation_gt_idx_1;
    rtb_DataTypeConversion12_idx_2 = (real32_T)rtb_Saturation_gt_idx_2;

    /* Outport: '<Root>/Temp' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion'
     */
    DynModel_Y->Temp = (real32_T)DynModel_B->RateTransition1;

    /* Outport: '<Root>/Press' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion1'
     */
    DynModel_Y->Press = (real32_T)rtb_ZeroOrderHold1_h;

    /* Outport: '<Root>/diff_Pres' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion3'
     */
    DynModel_Y->diff_Pres = (real32_T)rtb_ZeroOrderHold1;

    /* Outport: '<Root>/Baro_Alt' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion4'
     */
    DynModel_Y->Baro_Alt = (real32_T)DynModel_B->RateTransition3;

    /* Outport: '<Root>/Gps_Lat' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion5'
     */
    DynModel_Y->Gps_Lat = (real32_T)DynModel_B->RateTransition3_o;

    /* Outport: '<Root>/Gps_Lon' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion6'
     */
    DynModel_Y->Gps_Lon = (real32_T)DynModel_B->RateTransition4;

    /* Outport: '<Root>/Gps_Alt' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion7'
     */
    DynModel_Y->Gps_Alt = (real32_T)DynModel_B->RateTransition5;

    /* Outport: '<Root>/Gps_V' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion8'
     */
    DynModel_Y->Gps_V[0] = (real32_T)DynModel_B->RateTransition6[0];
    DynModel_Y->Gps_V[1] = (real32_T)DynModel_B->RateTransition6[1];
    DynModel_Y->Gps_V[2] = (real32_T)DynModel_B->RateTransition6[2];

    /* Outport: '<Root>/Gps_V_Mod' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion18'
     *  DotProduct: '<S1>/Dot Product'
     *  Sqrt: '<S1>/Sqrt'
     */
    DynModel_Y->Gps_V_Mod = (real32_T)sqrt((DynModel_B->RateTransition6[0] *
      DynModel_B->RateTransition6[0] + DynModel_B->RateTransition6[1] *
      DynModel_B->RateTransition6[1]) + DynModel_B->RateTransition6[2] *
      DynModel_B->RateTransition6[2]);

    /* Outport: '<Root>/COG' incorporates:
     *  DataTypeConversion: '<S1>/Data Type Conversion9'
     */
    DynModel_Y->COG = (real32_T)DynModel_B->RateTransition7;
  }

  /* Outport: '<Root>/Lat_Lon_Alt' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion10'
   *  DataTypeConversion: '<S1>/Data Type Conversion11'
   */
  DynModel_Y->Lat_Lon_Alt[0] = (real32_T)rtb_Add1_h_idx_0;
  DynModel_Y->Lat_Lon_Alt[1] = (real32_T)rtb_Add1_h_idx_1;
  DynModel_Y->Lat_Lon_Alt[2] = (real32_T)DynModel_B->Sum1;
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Outport: '<Root>/Magn' */
    DynModel_Y->Magn[0] = rtb_DataTypeConversion12_idx_0;
    DynModel_Y->Magn[1] = rtb_DataTypeConversion12_idx_1;
    DynModel_Y->Magn[2] = rtb_DataTypeConversion12_idx_2;
  }

  /* Outport: '<Root>/RPY' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion13'
   *  Sum: '<S3>/Add'
   */
  DynModel_Y->RPY[0] = (real32_T)(DynModel_B->VectorConcatenate[2] +
    DynModel_B->Output);
  DynModel_Y->RPY[1] = (real32_T)(DynModel_B->VectorConcatenate[1] +
    DynModel_B->Output);
  DynModel_Y->RPY[2] = (real32_T)(DynModel_B->VectorConcatenate[0] +
    DynModel_B->Output);
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Outport: '<Root>/Accelerometer' */
    DynModel_Y->Accelerometer[0] = rtb_DataTypeConversion14_idx_0;
    DynModel_Y->Accelerometer[1] = rtb_DataTypeConversion14_idx_1;
    DynModel_Y->Accelerometer[2] = rtb_DataTypeConversion14_idx_2;

    /* Outport: '<Root>/Gyro' */
    DynModel_Y->Gyro[0] = rtb_DataTypeConversion15_idx_0;
    DynModel_Y->Gyro[1] = rtb_DataTypeConversion15_idx_1;
    DynModel_Y->Gyro[2] = rtb_DataTypeConversion15_idx_2;
  }

  /* Outport: '<Root>/Quaternion' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion16'
   */
  DynModel_Y->Quaternion[0] = (real32_T)DynModel_B->Merge[0];
  DynModel_Y->Quaternion[1] = (real32_T)DynModel_B->Merge[1];
  DynModel_Y->Quaternion[2] = (real32_T)DynModel_B->Merge[2];
  DynModel_Y->Quaternion[3] = (real32_T)DynModel_B->Merge[3];

  /* Outport: '<Root>/Forces' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion2'
   */
  DynModel_Y->Forces[0] = (real32_T)rtb_Add7_idx_0;
  DynModel_Y->Forces[1] = (real32_T)rtb_Add7_idx_1;
  DynModel_Y->Forces[2] = (real32_T)rtb_Add7_idx_2;

  /* Outport: '<Root>/Torques' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion19'
   */
  DynModel_Y->Torques[0] = (real32_T)rtb_ixk;
  DynModel_Y->Torques[1] = (real32_T)rtb_Sum_e;
  DynModel_Y->Torques[2] = (real32_T)rtb_Sum_j;
  if (rtmIsMajorTimeStep(DynModel_M)) {
    /* Outport: '<Root>/Thrusts' */
    DynModel_Y->Thrusts[0] = rtb_DataTypeConversion20_idx_0;
    DynModel_Y->Thrusts[1] = rtb_DataTypeConversion20_idx_1;
    DynModel_Y->Thrusts[2] = rtb_DataTypeConversion20_idx_2;
    DynModel_Y->Thrusts[3] = rtb_DataTypeConversion20_idx_3;

    /* Outport: '<Root>/Rotor_Speed' */
    DynModel_Y->Rotor_Speed[0] = rtb_DataTypeConversion21_idx_0;
    DynModel_Y->Rotor_Speed[1] = rtb_DataTypeConversion21_idx_1;
    DynModel_Y->Rotor_Speed[2] = rtb_DataTypeConversion21_idx_2;
    DynModel_Y->Rotor_Speed[3] = rtb_DataTypeConversion21_idx_3;
  }

  /* Outport: '<Root>/Xe' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion22'
   */
  DynModel_Y->Xe[0] = (real32_T)DynModel_B->xeyeze[0];
  DynModel_Y->Xe[1] = (real32_T)DynModel_B->xeyeze[1];
  DynModel_Y->Xe[2] = (real32_T)DynModel_B->xeyeze[2];

  /* Outport: '<Root>/Freact' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion23'
   */
  DynModel_Y->Freact[0] = (real32_T)rtb_Add3_j[0];
  DynModel_Y->Freact[1] = (real32_T)rtb_Add3_j[1];
  DynModel_Y->Freact[2] = (real32_T)rtb_Add3_j[2];

  /* Sqrt: '<S64>/a' incorporates:
   *  Gain: '<S64>/gamma*R'
   */
  rtb_a = sqrt(401.87433999999996 * DynModel_B->Sum1_e);
  if (rtmIsMajorTimeStep(DynModel_M)) {
    if (rtmIsMajorTimeStep(DynModel_M)) {
      /* Update for RandomNumber: '<S72>/Random Number' */
      DynModel_DW->NextOutput = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed) *
        0.0031622776601683794 + 0.0;

      /* Update for RandomNumber: '<S69>/Random Number' */
      DynModel_DW->NextOutput_a = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_f) * 0.001 + 0.0;

      /* Update for RandomNumber: '<S67>/Random Number' */
      DynModel_DW->NextOutput_l = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_fw) * 0.001 + 0.0;

      /* Update for RandomNumber: '<S60>/Random Number' */
      DynModel_DW->NextOutput_n = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_fm) * 0.0031622776601683794 + 0.0;

      /* Update for RandomNumber: '<S63>/Random Number' */
      DynModel_DW->NextOutput_o[0] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_e[0]) * 0.1 + 0.0;
      DynModel_DW->NextOutput_o[1] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_e[1]) * 0.1 + 0.0;
      DynModel_DW->NextOutput_o[2] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_e[2]) * 0.1 + 0.0;

      /* Update for RandomNumber: '<S63>/Random Number1' */
      DynModel_DW->NextOutput_h[0] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_i[0]) * 0.00031622776601683794 + 0.0;
      DynModel_DW->NextOutput_h[1] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_i[1]) * 0.00031622776601683794 + 0.0;
      DynModel_DW->NextOutput_h[2] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_i[2]) * 0.00031622776601683794 + 0.0;
    }

    /* Update for Integrator: '<S10>/q0 q1 q2 q3' */
    DynModel_DW->q0q1q2q3_IWORK.IcNeedsLoading = 0;
    if (rtmIsMajorTimeStep(DynModel_M)) {
      /* Update for RandomNumber: '<S66>/Random Number' */
      DynModel_DW->NextOutput_am = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_p) * 0.001 + 0.0;

      /* Update for RandomNumber: '<S61>/White Noise' */
      DynModel_DW->NextOutput_lh = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_l) * 1.0 + 0.0;

      /* Update for Memory: '<S2>/Memory1' */
      DynModel_DW->Memory1_PreviousInput[0] = DynModel_B->Sum1;
      DynModel_DW->Memory1_PreviousInput[1] = DynModel_B->Sum1;
      DynModel_DW->Memory1_PreviousInput[2] = DynModel_B->Sum1;

      /* Update for UnitDelay: '<S45>/UD' */
      DynModel_DW->UD_DSTATE[0] = rtb_TSamp[0];
      DynModel_DW->UD_DSTATE[1] = rtb_TSamp[1];
      DynModel_DW->UD_DSTATE[2] = rtb_TSamp[2];

      /* Update for Memory: '<S2>/Memory6' */
      DynModel_DW->Memory6_PreviousInput[0] = DynModel_B->UnitConversion[0];
      DynModel_DW->Memory6_PreviousInput[1] = DynModel_B->UnitConversion[1];
      DynModel_DW->Memory6_PreviousInput[2] = DynModel_B->UnitConversion[2];

      /* Update for UnitDelay: '<S44>/UD' */
      DynModel_DW->UD_DSTATE_i = rtb_TSamp_l;

      /* Update for DiscreteFilter: '<S5>/Discrete Filter' */
      DynModel_DW->DiscreteFilter_states = DynModel_DW->DiscreteFilter_tmp;

      /* Update for DiscreteFir: '<S5>/Discrete FIR Filter' */
      /* Update circular buffer index */
      DynModel_DW->DiscreteFIRFilter_circBuf--;
      if (DynModel_DW->DiscreteFIRFilter_circBuf < 0L) {
        DynModel_DW->DiscreteFIRFilter_circBuf = 1L;
      }

      /* Update circular buffer */
      DynModel_DW->DiscreteFIRFilter_states
        [DynModel_DW->DiscreteFIRFilter_circBuf] = DynModel_B->Product2_n[0L];
      DynModel_DW->DiscreteFIRFilter_states
        [DynModel_DW->DiscreteFIRFilter_circBuf + 2L] = DynModel_B->Product2_n
        [1L];
      DynModel_DW->DiscreteFIRFilter_states
        [DynModel_DW->DiscreteFIRFilter_circBuf + 4L] = DynModel_B->Product2_n
        [2L];

      /* End of Update for DiscreteFir: '<S5>/Discrete FIR Filter' */

      /* Update for Memory: '<S2>/Memory2' */
      DynModel_DW->Memory2_PreviousInput = DynModel_B->Product3;

      /* Update for DiscreteTransferFcn: '<S52>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states =
        DynModel_DW->DiscreteTransferFcn_tmp;

      /* Update for DiscreteTransferFcn: '<S53>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_m =
        DynModel_DW->DiscreteTransferFcn_tmp_i;

      /* Update for DiscreteTransferFcn: '<S54>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_c =
        DynModel_DW->DiscreteTransferFcn_tmp_b;

      /* Update for DiscreteTransferFcn: '<S55>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_a =
        DynModel_DW->DiscreteTransferFcn_tmp_e;

      /* Update for Memory: '<S2>/Memory5' */
      DynModel_DW->Memory5_PreviousInput[0] = DynModel_B->pqr[0];
      DynModel_DW->Memory5_PreviousInput[1] = DynModel_B->pqr[1];
      DynModel_DW->Memory5_PreviousInput[2] = DynModel_B->pqr[2];

      /* Update for Memory: '<S2>/Memory3' */
      DynModel_DW->Memory3_PreviousInput[0] = DynModel_B->VectorConcatenate[2];
      DynModel_DW->Memory3_PreviousInput[1] = DynModel_B->VectorConcatenate[1];
      DynModel_DW->Memory3_PreviousInput[2] = DynModel_B->VectorConcatenate[0];

      /* Update for DiscreteTransferFcn: '<S126>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_h[1L] =
        DynModel_DW->DiscreteTransferFcn_states_h[0L];
      DynModel_DW->DiscreteTransferFcn_states_h[0L] =
        DynModel_DW->DiscreteTransferFcn_tmp_k;

      /* Update for DiscreteTransferFcn: '<S127>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_b[1L] =
        DynModel_DW->DiscreteTransferFcn_states_b[0L];
      DynModel_DW->DiscreteTransferFcn_states_b[0L] =
        DynModel_DW->DiscreteTransferFcn_tmp_o;

      /* Update for DiscreteTransferFcn: '<S128>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_cb[1L] =
        DynModel_DW->DiscreteTransferFcn_states_cb[0L];
      DynModel_DW->DiscreteTransferFcn_states_cb[0L] =
        DynModel_DW->DiscreteTransferFcn_tmp_j;

      /* Update for RandomNumber: '<S121>/White Noise' */
      DynModel_DW->NextOutput_k[0] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_ls[0]) * 1.0 + 0.0;
      DynModel_DW->NextOutput_k[1] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_ls[1]) * 1.0 + 0.0;
      DynModel_DW->NextOutput_k[2] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_ls[2]) * 1.0 + 0.0;

      /* Update for DiscreteTransferFcn: '<S141>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_d[1L] =
        DynModel_DW->DiscreteTransferFcn_states_d[0L];
      DynModel_DW->DiscreteTransferFcn_states_d[0L] =
        DynModel_DW->DiscreteTransferFcn_tmp_g;

      /* Update for DiscreteTransferFcn: '<S142>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_dr[1L] =
        DynModel_DW->DiscreteTransferFcn_states_dr[0L];
      DynModel_DW->DiscreteTransferFcn_states_dr[0L] =
        DynModel_DW->DiscreteTransferFcn_tmp_n;

      /* Update for DiscreteTransferFcn: '<S143>/Discrete Transfer Fcn' */
      DynModel_DW->DiscreteTransferFcn_states_as[1L] =
        DynModel_DW->DiscreteTransferFcn_states_as[0L];
      DynModel_DW->DiscreteTransferFcn_states_as[0L] =
        DynModel_DW->DiscreteTransferFcn_tmp_gx;

      /* Update for RandomNumber: '<S138>/White Noise' */
      DynModel_DW->NextOutput_p[0] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_j[0]) * 1.0 + 0.0;
      DynModel_DW->NextOutput_p[1] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_j[1]) * 1.0 + 0.0;
      DynModel_DW->NextOutput_p[2] = rt_nrand_Upu32_Yd_f_pw
        (&DynModel_DW->RandSeed_j[2]) * 1.0 + 0.0;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(DynModel_M)) {
    rt_ertODEUpdateContinuousStates(DynModel_M->solverInfo, DynModel_M);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++DynModel_M->Timing.clockTick0)) {
      ++DynModel_M->Timing.clockTickH0;
    }

    DynModel_M->Timing.t[0] = rtsiGetSolverStopTime(DynModel_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.002s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.002, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      DynModel_M->Timing.clockTick1++;
      if (!DynModel_M->Timing.clockTick1) {
        DynModel_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void DynModel_derivatives(RT_MODEL_DynModel_T *const DynModel_M)
{
  B_DynModel_T *DynModel_B = ((B_DynModel_T *) DynModel_M->ModelData.blockIO);
  XDot_DynModel_T *_rtXdot;
  _rtXdot = ((XDot_DynModel_T *) DynModel_M->ModelData.derivs);

  /* Derivatives for Integrator: '<S4>/xe,ye,ze' */
  _rtXdot->xeyeze_CSTATE[0] = DynModel_B->Reshape2[0];
  _rtXdot->xeyeze_CSTATE[1] = DynModel_B->Reshape2[1];
  _rtXdot->xeyeze_CSTATE[2] = DynModel_B->Reshape2[2];

  /* Derivatives for Integrator: '<S4>/ub,vb,wb' */
  _rtXdot->ubvbwb_CSTATE[0] = DynModel_B->Sum[0];
  _rtXdot->ubvbwb_CSTATE[1] = DynModel_B->Sum[1];
  _rtXdot->ubvbwb_CSTATE[2] = DynModel_B->Sum[2];

  /* Derivatives for Integrator: '<S10>/q0 q1 q2 q3' */
  {
    ((XDot_DynModel_T *) DynModel_M->ModelData.derivs)->q0q1q2q3_CSTATE[0] =
      DynModel_B->q0dot;
    ((XDot_DynModel_T *) DynModel_M->ModelData.derivs)->q0q1q2q3_CSTATE[1] =
      DynModel_B->q1dot;
    ((XDot_DynModel_T *) DynModel_M->ModelData.derivs)->q0q1q2q3_CSTATE[2] =
      DynModel_B->q2dot;
    ((XDot_DynModel_T *) DynModel_M->ModelData.derivs)->q0q1q2q3_CSTATE[3] =
      DynModel_B->q3dot;
  }

  /* Derivatives for Integrator: '<S4>/p,q,r ' */
  _rtXdot->pqr_CSTATE[0] = DynModel_B->Reshape[0];
  _rtXdot->pqr_CSTATE[1] = DynModel_B->Reshape[1];
  _rtXdot->pqr_CSTATE[2] = DynModel_B->Reshape[2];
}

/* Model initialize function */
void DynModel_initialize(RT_MODEL_DynModel_T *const DynModel_M)
{
  B_DynModel_T *DynModel_B = ((B_DynModel_T *) DynModel_M->ModelData.blockIO);
  DW_DynModel_T *DynModel_DW = ((DW_DynModel_T *) DynModel_M->ModelData.dwork);
  ExtY_DynModel_T *DynModel_Y = (ExtY_DynModel_T *)
    DynModel_M->ModelData.outputs;
  X_DynModel_T *DynModel_X = ((X_DynModel_T *) DynModel_M->ModelData.contStates);

  /* Start for If: '<S62>/If' */
  DynModel_DW->If_ActiveSubsystem = -1;

  /* Start for IfAction SubSystem: '<S62>/Negative Trace' */
  /* Start for If: '<S73>/Find Maximum Diagonal Value' */
  DynModel_DW->FindMaximumDiagonalValue_ActiveSubsystem = -1;

  /* End of Start for SubSystem: '<S62>/Negative Trace' */

  /* Start for Merge: '<S62>/Merge' */
  DynModel_B->Merge[0] = 1.0;
  DynModel_B->Merge[1] = 0.0;
  DynModel_B->Merge[2] = 0.0;
  DynModel_B->Merge[3] = 0.0;

  /* ConstCode for Outport: '<Root>/Sonar' */
  DynModel_Y->Sonar = DynModel_ConstB.DataTypeConversion17;

  /* ConstCode for Outport: '<Root>/Sample_Time' incorporates:
   *  Constant: '<S1>/Constant'
   */
  DynModel_Y->Sample_Time = 0.002;

  {
    uint32_T tseed;
    uint16_T r;
    uint16_T t;
    real_T y1;
    int16_T i;

    /* InitializeConditions for Integrator: '<S4>/xe,ye,ze' */
    DynModel_X->xeyeze_CSTATE[0] = 0.0;
    DynModel_X->xeyeze_CSTATE[1] = 0.0;
    DynModel_X->xeyeze_CSTATE[2] = 0.0;

    /* InitializeConditions for RandomNumber: '<S72>/Random Number' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    DynModel_DW->RandSeed = tseed;
    DynModel_DW->NextOutput = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed) *
      0.0031622776601683794 + 0.0;

    /* End of InitializeConditions for RandomNumber: '<S72>/Random Number' */

    /* InitializeConditions for RandomNumber: '<S69>/Random Number' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    DynModel_DW->RandSeed_f = tseed;
    DynModel_DW->NextOutput_a = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed_f)
      * 0.001 + 0.0;

    /* End of InitializeConditions for RandomNumber: '<S69>/Random Number' */

    /* InitializeConditions for Integrator: '<S4>/ub,vb,wb' */
    DynModel_X->ubvbwb_CSTATE[0] = 0.0;
    DynModel_X->ubvbwb_CSTATE[1] = 0.0;
    DynModel_X->ubvbwb_CSTATE[2] = 0.0;

    /* InitializeConditions for RandomNumber: '<S67>/Random Number' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    DynModel_DW->RandSeed_fw = tseed;
    DynModel_DW->NextOutput_l = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed_fw)
      * 0.001 + 0.0;

    /* End of InitializeConditions for RandomNumber: '<S67>/Random Number' */

    /* InitializeConditions for RandomNumber: '<S60>/Random Number' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    DynModel_DW->RandSeed_fm = tseed;
    DynModel_DW->NextOutput_n = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed_fm)
      * 0.0031622776601683794 + 0.0;

    /* End of InitializeConditions for RandomNumber: '<S60>/Random Number' */

    /* InitializeConditions for RandomNumber: '<S63>/Random Number' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 0.1 + 0.0;
    DynModel_DW->NextOutput_o[0] = y1;
    DynModel_DW->RandSeed_e[0] = tseed;
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 0.1 + 0.0;
    DynModel_DW->NextOutput_o[1] = y1;
    DynModel_DW->RandSeed_e[1] = tseed;
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 0.1 + 0.0;
    DynModel_DW->NextOutput_o[2] = y1;
    DynModel_DW->RandSeed_e[2] = tseed;

    /* End of InitializeConditions for RandomNumber: '<S63>/Random Number' */

    /* InitializeConditions for RandomNumber: '<S63>/Random Number1' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 0.00031622776601683794 + 0.0;
    DynModel_DW->NextOutput_h[0] = y1;
    DynModel_DW->RandSeed_i[0] = tseed;
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 0.00031622776601683794 + 0.0;
    DynModel_DW->NextOutput_h[1] = y1;
    DynModel_DW->RandSeed_i[1] = tseed;
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 0.00031622776601683794 + 0.0;
    DynModel_DW->NextOutput_h[2] = y1;
    DynModel_DW->RandSeed_i[2] = tseed;

    /* End of InitializeConditions for RandomNumber: '<S63>/Random Number1' */
    /* InitializeConditions for Integrator: '<S10>/q0 q1 q2 q3' */
    if (rtmIsFirstInitCond(DynModel_M)) {
      DynModel_X->q0q1q2q3_CSTATE[0] = 0.0;
      DynModel_X->q0q1q2q3_CSTATE[1] = 0.0;
      DynModel_X->q0q1q2q3_CSTATE[2] = 0.0;
      DynModel_X->q0q1q2q3_CSTATE[3] = 0.0;
    }

    DynModel_DW->q0q1q2q3_IWORK.IcNeedsLoading = 1;

    /* InitializeConditions for RandomNumber: '<S66>/Random Number' */
    y1 = fmod(floor(0.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    DynModel_DW->RandSeed_p = tseed;
    DynModel_DW->NextOutput_am = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed_p)
      * 0.001 + 0.0;

    /* End of InitializeConditions for RandomNumber: '<S66>/Random Number' */

    /* InitializeConditions for RandomNumber: '<S61>/White Noise' */
    y1 = fmod(floor(243584.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    DynModel_DW->RandSeed_l = tseed;
    DynModel_DW->NextOutput_lh = rt_nrand_Upu32_Yd_f_pw(&DynModel_DW->RandSeed_l)
      * 1.0 + 0.0;

    /* End of InitializeConditions for RandomNumber: '<S61>/White Noise' */

    /* InitializeConditions for Memory: '<S2>/Memory1' */
    DynModel_DW->Memory1_PreviousInput[0] = 0.0;
    DynModel_DW->Memory1_PreviousInput[1] = 0.0;
    DynModel_DW->Memory1_PreviousInput[2] = 0.0;

    /* InitializeConditions for UnitDelay: '<S45>/UD' */
    DynModel_DW->UD_DSTATE[0] = 0.0;
    DynModel_DW->UD_DSTATE[1] = 0.0;
    DynModel_DW->UD_DSTATE[2] = 0.0;

    /* InitializeConditions for Memory: '<S2>/Memory6' */
    DynModel_DW->Memory6_PreviousInput[0] = 0.0;
    DynModel_DW->Memory6_PreviousInput[1] = 0.0;
    DynModel_DW->Memory6_PreviousInput[2] = 0.0;

    /* InitializeConditions for UnitDelay: '<S44>/UD' */
    DynModel_DW->UD_DSTATE_i = 0.0;

    /* InitializeConditions for DiscreteFilter: '<S5>/Discrete Filter' */
    DynModel_DW->DiscreteFilter_states = 0.0;

    /* InitializeConditions for DiscreteFir: '<S5>/Discrete FIR Filter' */
    DynModel_DW->DiscreteFIRFilter_circBuf = 0L;
    for (i = 0; i < 6; i++) {
      DynModel_DW->DiscreteFIRFilter_states[i] = 0.0;
    }

    /* End of InitializeConditions for DiscreteFir: '<S5>/Discrete FIR Filter' */

    /* InitializeConditions for Memory: '<S2>/Memory2' */
    DynModel_DW->Memory2_PreviousInput = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S52>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S53>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_m = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S54>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_c = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S55>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_a = 0.0;

    /* InitializeConditions for Integrator: '<S4>/p,q,r ' */
    DynModel_X->pqr_CSTATE[0] = 0.0;
    DynModel_X->pqr_CSTATE[1] = 0.0;
    DynModel_X->pqr_CSTATE[2] = 0.0;

    /* InitializeConditions for Memory: '<S2>/Memory5' */
    DynModel_DW->Memory5_PreviousInput[0] = 0.0;
    DynModel_DW->Memory5_PreviousInput[1] = 0.0;
    DynModel_DW->Memory5_PreviousInput[2] = 0.0;

    /* InitializeConditions for Memory: '<S2>/Memory3' */
    DynModel_DW->Memory3_PreviousInput[0] = 0.0;
    DynModel_DW->Memory3_PreviousInput[1] = 0.0;
    DynModel_DW->Memory3_PreviousInput[2] = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S126>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_h[0] = 0.0;
    DynModel_DW->DiscreteTransferFcn_states_h[1] = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S127>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_b[0] = 0.0;
    DynModel_DW->DiscreteTransferFcn_states_b[1] = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S128>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_cb[0] = 0.0;
    DynModel_DW->DiscreteTransferFcn_states_cb[1] = 0.0;

    /* InitializeConditions for RandomNumber: '<S121>/White Noise' */
    y1 = fmod(floor(348631.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 1.0 + 0.0;
    DynModel_DW->NextOutput_k[0] = y1;
    DynModel_DW->RandSeed_ls[0] = tseed;
    y1 = fmod(floor(366719.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 1.0 + 0.0;
    DynModel_DW->NextOutput_k[1] = y1;
    DynModel_DW->RandSeed_ls[1] = tseed;
    y1 = fmod(floor(978432.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 1.0 + 0.0;
    DynModel_DW->NextOutput_k[2] = y1;
    DynModel_DW->RandSeed_ls[2] = tseed;

    /* End of InitializeConditions for RandomNumber: '<S121>/White Noise' */

    /* InitializeConditions for DiscreteTransferFcn: '<S141>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_d[0] = 0.0;
    DynModel_DW->DiscreteTransferFcn_states_d[1] = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S142>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_dr[0] = 0.0;
    DynModel_DW->DiscreteTransferFcn_states_dr[1] = 0.0;

    /* InitializeConditions for DiscreteTransferFcn: '<S143>/Discrete Transfer Fcn' */
    DynModel_DW->DiscreteTransferFcn_states_as[0] = 0.0;
    DynModel_DW->DiscreteTransferFcn_states_as[1] = 0.0;

    /* InitializeConditions for RandomNumber: '<S138>/White Noise' */
    y1 = fmod(floor(189621.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 1.0 + 0.0;
    DynModel_DW->NextOutput_p[0] = y1;
    DynModel_DW->RandSeed_j[0] = tseed;
    y1 = fmod(floor(128794.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 1.0 + 0.0;
    DynModel_DW->NextOutput_p[1] = y1;
    DynModel_DW->RandSeed_j[1] = tseed;
    y1 = fmod(floor(214632.0), 4.294967296E+9);
    tseed = y1 < 0.0 ? (uint32_T)-(int32_T)(uint32_T)-y1 : (uint32_T)y1;
    r = (uint16_T)(tseed >> 16);
    t = (uint16_T)tseed & 32768U;
    tseed = ((((tseed - ((uint32_T)r << 16)) + t) << 16UL) + t) + r;
    if (tseed < 1UL) {
      tseed = 1144108930UL;
    } else {
      if (tseed > 2147483646UL) {
        tseed = 2147483646UL;
      }
    }

    y1 = rt_nrand_Upu32_Yd_f_pw(&tseed) * 1.0 + 0.0;
    DynModel_DW->NextOutput_p[2] = y1;
    DynModel_DW->RandSeed_j[2] = tseed;

    /* End of InitializeConditions for RandomNumber: '<S138>/White Noise' */

    /* set "at time zero" to false */
    if (rtmIsFirstInitCond(DynModel_M)) {
      rtmSetFirstInitCond(DynModel_M, 0);
    }
  }
}

/* Model terminate function */
void DynModel_terminate(RT_MODEL_DynModel_T * DynModel_M)
{
  rt_FREE(DynModel_M->solverInfo);

  /* model code */
  rt_FREE(DynModel_M->ModelData.blockIO);
  rt_FREE(DynModel_M->ModelData.contStates);
  rt_FREE(DynModel_M->ModelData.inputs);
  rt_FREE(DynModel_M->ModelData.outputs);
  rt_FREE(DynModel_M->ModelData.dwork);
  rt_FREE(DynModel_M);
}

/* Model data allocation function */
RT_MODEL_DynModel_T *DynModel(void)
{
  RT_MODEL_DynModel_T *DynModel_M;
  DynModel_M = (RT_MODEL_DynModel_T *) malloc(sizeof(RT_MODEL_DynModel_T));
  if (DynModel_M == NULL) {
    return NULL;
  }

  (void) memset((char *)DynModel_M, 0,
                sizeof(RT_MODEL_DynModel_T));

  {
    /* Setup solver object */
    RTWSolverInfo *rt_SolverInfo = (RTWSolverInfo *) malloc(sizeof(RTWSolverInfo));
    rt_VALIDATE_MEMORY(DynModel_M,rt_SolverInfo);
    DynModel_M->solverInfo = (rt_SolverInfo);
    rtsiSetSimTimeStepPtr(DynModel_M->solverInfo,
                          &DynModel_M->Timing.simTimeStep);
    rtsiSetTPtr(DynModel_M->solverInfo, &rtmGetTPtr(DynModel_M));
    rtsiSetStepSizePtr(DynModel_M->solverInfo, &DynModel_M->Timing.stepSize0);
    rtsiSetdXPtr(DynModel_M->solverInfo, &DynModel_M->ModelData.derivs);
    rtsiSetContStatesPtr(DynModel_M->solverInfo, (real_T **)
                         &DynModel_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(DynModel_M->solverInfo,
      &DynModel_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(DynModel_M->solverInfo, (&rtmGetErrorStatus(DynModel_M)));
    rtsiSetRTModelPtr(DynModel_M->solverInfo, DynModel_M);
  }

  rtsiSetSolverName(DynModel_M->solverInfo,"ode4");

  /* block I/O */
  {
    B_DynModel_T *b = (B_DynModel_T *) malloc(sizeof(B_DynModel_T));
    rt_VALIDATE_MEMORY(DynModel_M,b);
    DynModel_M->ModelData.blockIO = (b);
  }

  /* states (continuous) */
  {
    X_DynModel_T *x = (X_DynModel_T *) malloc(sizeof(X_DynModel_T));
    rt_VALIDATE_MEMORY(DynModel_M,x);
    DynModel_M->ModelData.contStates = (x);
  }

  /* states (dwork) */
  {
    DW_DynModel_T *dwork = (DW_DynModel_T *) malloc(sizeof(DW_DynModel_T));
    rt_VALIDATE_MEMORY(DynModel_M,dwork);
    DynModel_M->ModelData.dwork = (dwork);
  }

  /* external inputs */
  {
    ExtU_DynModel_T *DynModel_U = (ExtU_DynModel_T *) malloc(sizeof
      (ExtU_DynModel_T));
    rt_VALIDATE_MEMORY(DynModel_M,DynModel_U);
    DynModel_M->ModelData.inputs = (((ExtU_DynModel_T *) DynModel_U));
  }

  /* external outputs */
  {
    ExtY_DynModel_T *DynModel_Y = (ExtY_DynModel_T *) malloc(sizeof
      (ExtY_DynModel_T));
    rt_VALIDATE_MEMORY(DynModel_M,DynModel_Y);
    DynModel_M->ModelData.outputs = (DynModel_Y);
  }

  {
    B_DynModel_T *DynModel_B = ((B_DynModel_T *) DynModel_M->ModelData.blockIO);
    DW_DynModel_T *DynModel_DW = ((DW_DynModel_T *) DynModel_M->ModelData.dwork);
    X_DynModel_T *DynModel_X = ((X_DynModel_T *)
      DynModel_M->ModelData.contStates);
    ExtU_DynModel_T *DynModel_U = (ExtU_DynModel_T *)
      DynModel_M->ModelData.inputs;
    ExtY_DynModel_T *DynModel_Y = (ExtY_DynModel_T *)
      DynModel_M->ModelData.outputs;
    rtsiSetSimTimeStep(DynModel_M->solverInfo, MAJOR_TIME_STEP);
    DynModel_M->ModelData.intgData.y = DynModel_M->ModelData.odeY;
    DynModel_M->ModelData.intgData.f[0] = DynModel_M->ModelData.odeF[0];
    DynModel_M->ModelData.intgData.f[1] = DynModel_M->ModelData.odeF[1];
    DynModel_M->ModelData.intgData.f[2] = DynModel_M->ModelData.odeF[2];
    DynModel_M->ModelData.intgData.f[3] = DynModel_M->ModelData.odeF[3];
    DynModel_M->ModelData.contStates = ((X_DynModel_T *) DynModel_X);
    rtsiSetSolverData(DynModel_M->solverInfo, (void *)
                      &DynModel_M->ModelData.intgData);
    rtmSetTPtr(DynModel_M, &DynModel_M->Timing.tArray[0]);
    DynModel_M->Timing.stepSize0 = 0.002;
    rtmSetFirstInitCond(DynModel_M, 1);

    /* block I/O */
    (void) memset(((void *) DynModel_B), 0,
                  sizeof(B_DynModel_T));

    /* states (continuous) */
    {
      (void) memset((void *)DynModel_X, 0,
                    sizeof(X_DynModel_T));
    }

    /* states (dwork) */
    (void) memset((void *)DynModel_DW, 0,
                  sizeof(DW_DynModel_T));

    /* external inputs */
    (void) memset((void *)DynModel_U, 0,
                  sizeof(ExtU_DynModel_T));

    /* external outputs */
    (void) memset((void *)DynModel_Y, 0,
                  sizeof(ExtY_DynModel_T));
  }

  return DynModel_M;
}
