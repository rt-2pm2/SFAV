/*
 * DynModel_data.c
 *
 * Code generation for model "DynModel".
 *
 * Model version              : 1.744
 * Simulink Coder version : 8.8 (R2015a) 09-Feb-2015
 * C source code generated on : Fri Feb 17 18:22:28 2017
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

/* Invariant block signals (auto storage) */
const ConstB_DynModel_T DynModel_ConstB = {
  0.0
  ,                                    /* '<S112>/Unit Conversion' */
  0.0
  ,                                    /* '<S101>/SinCos' */
  1.0
  ,                                    /* '<S101>/SinCos' */
  0.99664718933525254
  ,                                    /* '<S115>/Sum' */
  0.99330562000985867
  ,                                    /* '<S116>/Product1' */
  0.00669437999014133
  ,                                    /* '<S116>/Sum1' */
  0.08181919084262157
  ,                                    /* '<S116>/sqrt' */
  180.0
  ,                                    /* '<S110>/Bias' */
  180.0
  ,                                    /* '<S110>/Math Function1' */
  0.0
  ,                                    /* '<S110>/Bias1' */
  0.0
  ,                                    /* '<S110>/Abs' */
  0.0
  ,                                    /* '<S110>/Switch' */
  0.0
  ,                                    /* '<S107>/Sign1' */
  0.0
  ,                                    /* '<S107>/Abs1' */
  -90.0
  ,                                    /* '<S107>/Bias' */
  90.0
  ,                                    /* '<S107>/Gain' */
  180.0
  ,                                    /* '<S107>/Bias1' */
  0.0
  ,                                    /* '<S107>/Divide1' */
  0.0
  ,                                    /* '<S107>/Switch' */
  0.0
  ,                                    /* '<S113>/Unit Conversion' */
  0.0
  ,                                    /* '<S114>/Trigonometric Function1' */
  0.0
  ,                                    /* '<S114>/Product1' */
  1.0
  ,                                    /* '<S114>/Sum1' */
  1.0
  ,                                    /* '<S111>/sqrt' */
  6.378137E+6
  ,                                    /* '<S111>/Product1' */
  0.0066943799901413295
  ,                                    /* '<S111>/Product2' */
  0.99330562000985867
  ,                                    /* '<S111>/Sum1' */
  6.3354393272928195E+6
  ,                                    /* '<S111>/Product3' */
  1.5784225029068334E-7
  ,                                    /* '<S111>/Trigonometric Function1' */
  1.0
  ,                                    /* '<S111>/Trigonometric Function' */
  6.378137E+6
  ,                                    /* '<S111>/Product4' */
  1.5678559428873849E-7
  ,                                    /* '<S111>/Trigonometric Function2' */
  0.0
  ,                                    /* '<S100>/Switch1' */
  0.0
  ,                                    /* '<S100>/Sum' */
  180.0
  ,                                    /* '<S108>/Bias' */
  180.0
  ,                                    /* '<S108>/Math Function1' */
  0.0
  ,                                    /* '<S108>/Bias1' */
  0.0
  ,                                    /* '<S108>/Abs' */
  0.0
  ,                                    /* '<S108>/Switch' */

  { 0.0, 0.0, 0.0 }
  ,                                    /* '<S19>/1//2' */

  { 0.0, 0.0, 0.0 }
  ,                                    /* '<S19>/sincos' */

  { 1.0, 1.0, 1.0 }
  ,                                    /* '<S19>/sincos' */
  1.0
  ,                                    /* '<S19>/q0' */
  0.0
  ,                                    /* '<S19>/q1' */
  0.0
  ,                                    /* '<S19>/q2' */
  0.0
  ,                                    /* '<S19>/q3' */
  0.0
  ,                                    /* '<S160>/Unit Conversion' */
  0.0
  ,                                    /* '<S149>/SinCos' */
  1.0
  ,                                    /* '<S149>/SinCos' */
  0.99664718933525254
  ,                                    /* '<S163>/Sum' */
  0.99330562000985867
  ,                                    /* '<S164>/Product1' */
  0.00669437999014133
  ,                                    /* '<S164>/Sum1' */
  0.08181919084262157
  ,                                    /* '<S164>/sqrt' */
  180.0
  ,                                    /* '<S158>/Bias' */
  180.0
  ,                                    /* '<S158>/Math Function1' */
  0.0
  ,                                    /* '<S158>/Bias1' */
  0.0
  ,                                    /* '<S158>/Abs' */
  0.0
  ,                                    /* '<S158>/Switch' */
  0.0
  ,                                    /* '<S155>/Sign1' */
  0.0
  ,                                    /* '<S155>/Abs1' */
  -90.0
  ,                                    /* '<S155>/Bias' */
  90.0
  ,                                    /* '<S155>/Gain' */
  180.0
  ,                                    /* '<S155>/Bias1' */
  0.0
  ,                                    /* '<S155>/Divide1' */
  0.0
  ,                                    /* '<S155>/Switch' */
  0.0
  ,                                    /* '<S161>/Unit Conversion' */
  0.0
  ,                                    /* '<S162>/Trigonometric Function1' */
  0.0
  ,                                    /* '<S162>/Product1' */
  1.0
  ,                                    /* '<S162>/Sum1' */
  1.0
  ,                                    /* '<S159>/sqrt' */
  6.378137E+6
  ,                                    /* '<S159>/Product1' */
  0.0066943799901413295
  ,                                    /* '<S159>/Product2' */
  0.99330562000985867
  ,                                    /* '<S159>/Sum1' */
  6.3354393272928195E+6
  ,                                    /* '<S159>/Product3' */
  1.5784225029068334E-7
  ,                                    /* '<S159>/Trigonometric Function1' */
  1.0
  ,                                    /* '<S159>/Trigonometric Function' */
  6.378137E+6
  ,                                    /* '<S159>/Product4' */
  1.5678559428873849E-7
  ,                                    /* '<S159>/Trigonometric Function2' */
  0.0
  ,                                    /* '<S148>/Switch1' */
  0.0
  ,                                    /* '<S148>/Sum' */
  180.0
  ,                                    /* '<S156>/Bias' */
  180.0
  ,                                    /* '<S156>/Math Function1' */
  0.0
  ,                                    /* '<S156>/Bias1' */
  0.0
  ,                                    /* '<S156>/Abs' */
  0.0
  ,                                    /* '<S156>/Switch' */

  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
  ,                                    /* '<S11>/Selector1' */

  { 0.0409, 0.0, 0.0, 0.0, 0.0409, 0.0, 0.0, 0.0, 0.0509 }
  ,                                    /* '<S11>/Selector' */

  { 0.0409, 0.0, 0.0, 0.0, 0.0409, 0.0, 0.0, 0.0, 0.0509 }
  ,                                    /* '<S11>/Selector2' */
  0.0F
  ,                                    /* '<S1>/Data Type Conversion17' */
  0U
  ,                                    /* '<S109>/Compare' */
  0U
  /* '<S157>/Compare' */
};

/* Constant parameters (auto storage) */
const ConstP_DynModel_T DynModel_ConstP = {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S2>/Memory1'
   *   '<S2>/Memory3'
   *   '<S2>/Memory5'
   *   '<S2>/Memory6'
   *   '<S3>/center of gravity'
   *   '<S4>/p,q,r '
   *   '<S4>/ub,vb,wb'
   *   '<S4>/xe,ye,ze'
   *   '<S5>/Constant'
   *   '<S5>/Constant1'
   *   '<S5>/Constant3'
   *   '<S5>/Constant4'
   *   '<S66>/Measurement bias1'
   *   '<S10>/Initial Euler Angles'
   *   '<S118>/Measurement bias'
   *   '<S118>/wl_ins'
   *   '<S119>/Measurement bias'
   *   '<S119>/g-sensitive bias'
   */
  { 0.0, 0.0, 0.0 },

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S118>/Scale factors & Cross-coupling  errors'
   *   '<S119>/Scale factors & Cross-coupling  errors '
   */
  { 0.98, 0.0, 0.0, 0.0, 0.98, 0.0, 0.0, 0.0, 0.98 },

  /* Expression: [1 1 1]
   * Referenced by: '<S5>/Discrete FIR Filter'
   */
  { 1.0, 1.0, 1.0 }
};
