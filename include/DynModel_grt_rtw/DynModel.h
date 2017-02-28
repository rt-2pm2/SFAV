/*
 * DynModel.h
 *
 * Code generation for model "DynModel".
 *
 * Model version              : 1.766
 * Simulink Coder version : 8.8 (R2015a) 09-Feb-2015
 * C source code generated on : Sat Feb 25 20:53:48 2017
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64
 * Emulation hardware selection:
 *    Differs from embedded hardware (Generic->16-bit Embedded Processor)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_DynModel_h_
#define RTW_HEADER_DynModel_h_
#include <float.h>
#include <math.h>
#include <string.h>
#include <stddef.h>
#ifndef DynModel_COMMON_INCLUDES_
# define DynModel_COMMON_INCLUDES_
#include <stdlib.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* DynModel_COMMON_INCLUDES_ */

#include "DynModel_types.h"

/* Shared type includes */
#include "multiword_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetBlkStateChangeFlag
# define rtmGetBlkStateChangeFlag(rtm) ((rtm)->ModelData.blkStateChange)
#endif

#ifndef rtmSetBlkStateChangeFlag
# define rtmSetBlkStateChangeFlag(rtm, val) ((rtm)->ModelData.blkStateChange = (val))
#endif

#ifndef rtmGetBlockIO
# define rtmGetBlockIO(rtm)            ((rtm)->ModelData.blockIO)
#endif

#ifndef rtmSetBlockIO
# define rtmSetBlockIO(rtm, val)       ((rtm)->ModelData.blockIO = (val))
#endif

#ifndef rtmGetContStateDisabled
# define rtmGetContStateDisabled(rtm)  ((rtm)->ModelData.contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
# define rtmSetContStateDisabled(rtm, val) ((rtm)->ModelData.contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
# define rtmGetContStates(rtm)         ((rtm)->ModelData.contStates)
#endif

#ifndef rtmSetContStates
# define rtmSetContStates(rtm, val)    ((rtm)->ModelData.contStates = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
# define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->ModelData.derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
# define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->ModelData.derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
# define rtmGetIntgData(rtm)           ((rtm)->ModelData.intgData)
#endif

#ifndef rtmSetIntgData
# define rtmSetIntgData(rtm, val)      ((rtm)->ModelData.intgData = (val))
#endif

#ifndef rtmGetOdeF
# define rtmGetOdeF(rtm)               ((rtm)->ModelData.odeF)
#endif

#ifndef rtmSetOdeF
# define rtmSetOdeF(rtm, val)          ((rtm)->ModelData.odeF = (val))
#endif

#ifndef rtmGetOdeY
# define rtmGetOdeY(rtm)               ((rtm)->ModelData.odeY)
#endif

#ifndef rtmSetOdeY
# define rtmSetOdeY(rtm, val)          ((rtm)->ModelData.odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
# define rtmGetPeriodicContStateIndices(rtm) ((rtm)->ModelData.periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
# define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->ModelData.periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
# define rtmGetPeriodicContStateRanges(rtm) ((rtm)->ModelData.periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
# define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->ModelData.periodicContStateRanges = (val))
#endif

#ifndef rtmGetRootDWork
# define rtmGetRootDWork(rtm)          ((rtm)->ModelData.dwork)
#endif

#ifndef rtmSetRootDWork
# define rtmSetRootDWork(rtm, val)     ((rtm)->ModelData.dwork = (val))
#endif

#ifndef rtmGetU
# define rtmGetU(rtm)                  ((rtm)->ModelData.inputs)
#endif

#ifndef rtmSetU
# define rtmSetU(rtm, val)             ((rtm)->ModelData.inputs = (val))
#endif

#ifndef rtmGetY
# define rtmGetY(rtm)                  ((rtm)->ModelData.outputs)
#endif

#ifndef rtmSetY
# define rtmSetY(rtm, val)             ((rtm)->ModelData.outputs = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
# define rtmGetZCCacheNeedsReset(rtm)  ((rtm)->ModelData.zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
# define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->ModelData.zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
# define rtmGetdX(rtm)                 ((rtm)->ModelData.derivs)
#endif

#ifndef rtmSetdX
# define rtmSetdX(rtm, val)            ((rtm)->ModelData.derivs = (val))
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#define DynModel_M_TYPE                RT_MODEL_DynModel_T

/* Block signals (auto storage) */
typedef struct {
  real_T xeyeze[3];                    /* '<S4>/xe,ye,ze' */
  real_T Sum1;                         /* '<S146>/Sum1' */
  real_T Sum1_e;                       /* '<S64>/Sum1' */
  real_T RateTransition;               /* '<S72>/Rate Transition' */
  real_T RateTransition1;              /* '<S72>/Rate Transition1' */
  real_T Product2;                     /* '<S64>/Product2' */
  real_T RateTransition_c;             /* '<S69>/Rate Transition' */
  real_T Product3;                     /* '<S64>/Product3' */
  real_T RateTransition_b;             /* '<S67>/Rate Transition' */
  real_T UnitConversion[3];            /* '<S14>/Unit Conversion' */
  real_T RateTransition1_p[3];         /* '<S67>/Rate Transition1' */
  real_T RateTransition1_i;            /* '<S60>/Rate Transition1' */
  real_T RateTransition2;              /* '<S60>/Rate Transition2' */
  real_T RateTransition3;              /* '<S60>/Rate Transition3' */
  real_T RateTransition1_a[3];         /* '<S63>/Rate Transition1' */
  real_T RateTransition_e;             /* '<S63>/Rate Transition' */
  real_T RateTransition5;              /* '<S63>/Rate Transition5' */
  real_T Reshape9to3x3columnmajor[9];  /* '<S30>/Reshape (9) to [3x3] column-major' */
  real_T Reshape2[3];                  /* '<S16>/Reshape2' */
  real_T UnitConversion_j[3];          /* '<S15>/Unit Conversion' */
  real_T RateTransition2_l[3];         /* '<S63>/Rate Transition2' */
  real_T RateTransition6[3];           /* '<S63>/Rate Transition6' */
  real_T RateTransition3_o;            /* '<S63>/Rate Transition3' */
  real_T RateTransition4;              /* '<S63>/Rate Transition4' */
  real_T RateTransition7;              /* '<S63>/Rate Transition7' */
  real_T RateTransition1_o[9];         /* '<S66>/Rate Transition1' */
  real_T RateTransition2_j;            /* '<S66>/Rate Transition2' */
  real_T RateTransition3_d;            /* '<S66>/Rate Transition3' */
  real_T RateTransition4_o;            /* '<S66>/Rate Transition4' */
  real_T Output;                       /* '<S61>/Output' */
  real_T Gain13[3];                    /* '<S5>/Gain13' */
  real_T Gain11;                       /* '<S5>/Gain11' */
  real_T Switch2[3];                   /* '<S5>/Switch2' */
  real_T Memory4[3];                   /* '<S2>/Memory4' */
  real_T DiscreteFilter;               /* '<S5>/Discrete Filter' */
  real_T Product2_n[3];                /* '<S5>/Product2' */
  real_T Saturation[3];                /* '<S5>/Saturation' */
  real_T Submatrix[3];                 /* '<S50>/Submatrix' */
  real_T Gain1[3];                     /* '<S48>/Gain1' */
  real_T Integrator;                   /* '<S52>/Integrator' */
  real_T Integrator_m;                 /* '<S53>/Integrator' */
  real_T RateTransition_o;             /* '<S53>/Rate Transition' */
  real_T Integrator_e;                 /* '<S54>/Integrator' */
  real_T RateTransition_f;             /* '<S54>/Rate Transition' */
  real_T Integrator_i;                 /* '<S55>/Integrator' */
  real_T RateTransition_i;             /* '<S55>/Rate Transition' */
  real_T Product[3];                   /* '<S4>/Product' */
  real_T MatrixMultiply1[3];           /* '<S3>/Matrix Multiply1' */
  real_T pqr[3];                       /* '<S4>/p,q,r ' */
  real_T VectorConcatenate[3];         /* '<S18>/Vector Concatenate' */
  real_T Saturation1[3];               /* '<S5>/Saturation1' */
  real_T Product2_p[3];                /* '<S47>/Product2' */
  real_T Reshape[3];                   /* '<S11>/Reshape' */
  real_T UnitConversion_h[3];          /* '<S117>/Unit Conversion' */
  real_T q0dot;                        /* '<S20>/q0dot' */
  real_T q1dot;                        /* '<S20>/q1dot' */
  real_T q2dot;                        /* '<S20>/q2dot' */
  real_T q3dot;                        /* '<S20>/q3dot' */
  real_T Sum[3];                       /* '<S4>/Sum' */
  real_T Merge[4];                     /* '<S62>/Merge' */
  real_T ZeroOrderHold;                /* '<S52>/Zero-Order Hold' */
  real_T Gain2;                        /* '<S52>/Gain2' */
  real_T ZeroOrderHold_l;              /* '<S53>/Zero-Order Hold' */
  real_T Gain2_m;                      /* '<S53>/Gain2' */
  real_T ZeroOrderHold_p;              /* '<S54>/Zero-Order Hold' */
  real_T Gain2_c;                      /* '<S54>/Gain2' */
  real_T ZeroOrderHold_b;              /* '<S55>/Zero-Order Hold' */
  real_T Gain2_j;                      /* '<S55>/Gain2' */
  real_T force[3];                     /* '<S6>/multicopter' */
  real_T moments[3];                   /* '<S6>/multicopter' */
} B_DynModel_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T UD_DSTATE;                    /* '<S45>/UD' */
  real_T UD_DSTATE_i;                  /* '<S44>/UD' */
  real_T DiscreteFilter_states;        /* '<S5>/Discrete Filter' */
  real_T DiscreteFIRFilter_states[12]; /* '<S5>/Discrete FIR Filter' */
  real_T DiscreteTransferFcn_states[2];/* '<S126>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_states_b[2];/* '<S127>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_states_c[2];/* '<S128>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_states_d[2];/* '<S141>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_states_dr[2];/* '<S142>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_states_a[2];/* '<S143>/Discrete Transfer Fcn' */
  real_T NextOutput;                   /* '<S72>/Random Number' */
  real_T NextOutput_a;                 /* '<S69>/Random Number' */
  real_T NextOutput_l;                 /* '<S67>/Random Number' */
  real_T NextOutput_n;                 /* '<S60>/Random Number' */
  real_T NextOutput_o[3];              /* '<S63>/Random Number' */
  real_T NextOutput_h[3];              /* '<S63>/Random Number1' */
  real_T NextOutput_am;                /* '<S66>/Random Number' */
  real_T NextOutput_lh;                /* '<S61>/White Noise' */
  real_T Memory1_PreviousInput;        /* '<S2>/Memory1' */
  real_T Memory6_PreviousInput[3];     /* '<S2>/Memory6' */
  real_T Memory4_PreviousInput[3];     /* '<S2>/Memory4' */
  real_T DiscreteFilter_tmp;           /* '<S5>/Discrete Filter' */
  real_T Memory2_PreviousInput;        /* '<S2>/Memory2' */
  real_T Memory5_PreviousInput[3];     /* '<S2>/Memory5' */
  real_T Memory3_PreviousInput[3];     /* '<S2>/Memory3' */
  real_T Product2_DWORK4[9];           /* '<S11>/Product2' */
  real_T DiscreteTransferFcn_tmp;      /* '<S126>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_tmp_o;    /* '<S127>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_tmp_j;    /* '<S128>/Discrete Transfer Fcn' */
  real_T NextOutput_k[3];              /* '<S121>/White Noise' */
  real_T DiscreteTransferFcn_tmp_g;    /* '<S141>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_tmp_n;    /* '<S142>/Discrete Transfer Fcn' */
  real_T DiscreteTransferFcn_tmp_gx;   /* '<S143>/Discrete Transfer Fcn' */
  real_T NextOutput_p[3];              /* '<S138>/White Noise' */
  int32_T DiscreteFIRFilter_circBuf;   /* '<S5>/Discrete FIR Filter' */
  uint32_T RandSeed;                   /* '<S72>/Random Number' */
  uint32_T RandSeed_f;                 /* '<S69>/Random Number' */
  uint32_T RandSeed_fw;                /* '<S67>/Random Number' */
  uint32_T RandSeed_fm;                /* '<S60>/Random Number' */
  uint32_T RandSeed_e[3];              /* '<S63>/Random Number' */
  uint32_T RandSeed_i[3];              /* '<S63>/Random Number1' */
  uint32_T RandSeed_p;                 /* '<S66>/Random Number' */
  uint32_T RandSeed_l;                 /* '<S61>/White Noise' */
  uint32_T RandSeed_ls[3];             /* '<S121>/White Noise' */
  uint32_T RandSeed_j[3];              /* '<S138>/White Noise' */
  struct {
    int_T IcNeedsLoading;
  } q0q1q2q3_IWORK;                    /* '<S10>/q0 q1 q2 q3' */

  int8_T If_ActiveSubsystem;           /* '<S62>/If' */
  int8_T FindMaximumDiagonalValue_ActiveSubsystem;/* '<S73>/Find Maximum Diagonal Value' */
} DW_DynModel_T;

/* Continuous states (auto storage) */
typedef struct {
  real_T xeyeze_CSTATE[3];             /* '<S4>/xe,ye,ze' */
  real_T ubvbwb_CSTATE[3];             /* '<S4>/ub,vb,wb' */
  real_T q0q1q2q3_CSTATE[4];           /* '<S10>/q0 q1 q2 q3' */
  real_T Integrator_CSTATE;            /* '<S52>/Integrator' */
  real_T Integrator_CSTATE_k;          /* '<S53>/Integrator' */
  real_T Integrator_CSTATE_m;          /* '<S54>/Integrator' */
  real_T Integrator_CSTATE_e;          /* '<S55>/Integrator' */
  real_T pqr_CSTATE[3];                /* '<S4>/p,q,r ' */
} X_DynModel_T;

/* State derivatives (auto storage) */
typedef struct {
  real_T xeyeze_CSTATE[3];             /* '<S4>/xe,ye,ze' */
  real_T ubvbwb_CSTATE[3];             /* '<S4>/ub,vb,wb' */
  real_T q0q1q2q3_CSTATE[4];           /* '<S10>/q0 q1 q2 q3' */
  real_T Integrator_CSTATE;            /* '<S52>/Integrator' */
  real_T Integrator_CSTATE_k;          /* '<S53>/Integrator' */
  real_T Integrator_CSTATE_m;          /* '<S54>/Integrator' */
  real_T Integrator_CSTATE_e;          /* '<S55>/Integrator' */
  real_T pqr_CSTATE[3];                /* '<S4>/p,q,r ' */
} XDot_DynModel_T;

/* State disabled  */
typedef struct {
  boolean_T xeyeze_CSTATE[3];          /* '<S4>/xe,ye,ze' */
  boolean_T ubvbwb_CSTATE[3];          /* '<S4>/ub,vb,wb' */
  boolean_T q0q1q2q3_CSTATE[4];        /* '<S10>/q0 q1 q2 q3' */
  boolean_T Integrator_CSTATE;         /* '<S52>/Integrator' */
  boolean_T Integrator_CSTATE_k;       /* '<S53>/Integrator' */
  boolean_T Integrator_CSTATE_m;       /* '<S54>/Integrator' */
  boolean_T Integrator_CSTATE_e;       /* '<S55>/Integrator' */
  boolean_T pqr_CSTATE[3];             /* '<S4>/p,q,r ' */
} XDis_DynModel_T;

/* Invariant block signals (auto storage) */
typedef struct {
  const real_T UnitConversion;         /* '<S112>/Unit Conversion' */
  const real_T SinCos_o1;              /* '<S101>/SinCos' */
  const real_T SinCos_o2;              /* '<S101>/SinCos' */
  const real_T Sum;                    /* '<S115>/Sum' */
  const real_T Product1;               /* '<S116>/Product1' */
  const real_T Sum1;                   /* '<S116>/Sum1' */
  const real_T sqrt_a;                 /* '<S116>/sqrt' */
  const real_T Bias;                   /* '<S110>/Bias' */
  const real_T MathFunction1;          /* '<S110>/Math Function1' */
  const real_T Bias1;                  /* '<S110>/Bias1' */
  const real_T Abs;                    /* '<S110>/Abs' */
  const real_T Switch;                 /* '<S110>/Switch' */
  const real_T Sign1;                  /* '<S107>/Sign1' */
  const real_T Abs1;                   /* '<S107>/Abs1' */
  const real_T Bias_f;                 /* '<S107>/Bias' */
  const real_T Gain;                   /* '<S107>/Gain' */
  const real_T Bias1_j;                /* '<S107>/Bias1' */
  const real_T Divide1;                /* '<S107>/Divide1' */
  const real_T Switch_h;               /* '<S107>/Switch' */
  const real_T UnitConversion_j;       /* '<S113>/Unit Conversion' */
  const real_T TrigonometricFunction1; /* '<S114>/Trigonometric Function1' */
  const real_T Product1_c;             /* '<S114>/Product1' */
  const real_T Sum1_j;                 /* '<S114>/Sum1' */
  const real_T sqrt_b;                 /* '<S111>/sqrt' */
  const real_T Rn;                     /* '<S111>/Product1' */
  const real_T Product2;               /* '<S111>/Product2' */
  const real_T Sum1_c;                 /* '<S111>/Sum1' */
  const real_T Rm;                     /* '<S111>/Product3' */
  const real_T TrigonometricFunction1_g;/* '<S111>/Trigonometric Function1' */
  const real_T TrigonometricFunction;  /* '<S111>/Trigonometric Function' */
  const real_T Product4;               /* '<S111>/Product4' */
  const real_T TrigonometricFunction2; /* '<S111>/Trigonometric Function2' */
  const real_T Switch1;                /* '<S100>/Switch1' */
  const real_T Sum_c;                  /* '<S100>/Sum' */
  const real_T Bias_e;                 /* '<S108>/Bias' */
  const real_T MathFunction1_k;        /* '<S108>/Math Function1' */
  const real_T Bias1_l;                /* '<S108>/Bias1' */
  const real_T Abs_b;                  /* '<S108>/Abs' */
  const real_T Switch_d;               /* '<S108>/Switch' */
  const real_T u[3];                   /* '<S19>/1//2' */
  const real_T sincos_o1[3];           /* '<S19>/sincos' */
  const real_T sincos_o2[3];           /* '<S19>/sincos' */
  const real_T q0;                     /* '<S19>/q0' */
  const real_T q1;                     /* '<S19>/q1' */
  const real_T q2;                     /* '<S19>/q2' */
  const real_T q3;                     /* '<S19>/q3' */
  const real_T UnitConversion_b;       /* '<S160>/Unit Conversion' */
  const real_T SinCos_o1_f;            /* '<S149>/SinCos' */
  const real_T SinCos_o2_i;            /* '<S149>/SinCos' */
  const real_T Sum_l;                  /* '<S163>/Sum' */
  const real_T Product1_f;             /* '<S164>/Product1' */
  const real_T Sum1_ja;                /* '<S164>/Sum1' */
  const real_T sqrt_p;                 /* '<S164>/sqrt' */
  const real_T Bias_k;                 /* '<S158>/Bias' */
  const real_T MathFunction1_c;        /* '<S158>/Math Function1' */
  const real_T Bias1_o;                /* '<S158>/Bias1' */
  const real_T Abs_m;                  /* '<S158>/Abs' */
  const real_T Switch_i;               /* '<S158>/Switch' */
  const real_T Sign1_h;                /* '<S155>/Sign1' */
  const real_T Abs1_p;                 /* '<S155>/Abs1' */
  const real_T Bias_c;                 /* '<S155>/Bias' */
  const real_T Gain_l;                 /* '<S155>/Gain' */
  const real_T Bias1_p;                /* '<S155>/Bias1' */
  const real_T Divide1_k;              /* '<S155>/Divide1' */
  const real_T Switch_c;               /* '<S155>/Switch' */
  const real_T UnitConversion_d;       /* '<S161>/Unit Conversion' */
  const real_T TrigonometricFunction1_k;/* '<S162>/Trigonometric Function1' */
  const real_T Product1_b;             /* '<S162>/Product1' */
  const real_T Sum1_i;                 /* '<S162>/Sum1' */
  const real_T sqrt_n;                 /* '<S159>/sqrt' */
  const real_T Rn_k;                   /* '<S159>/Product1' */
  const real_T Product2_l;             /* '<S159>/Product2' */
  const real_T Sum1_p;                 /* '<S159>/Sum1' */
  const real_T Rm_n;                   /* '<S159>/Product3' */
  const real_T TrigonometricFunction1_c;/* '<S159>/Trigonometric Function1' */
  const real_T TrigonometricFunction_j;/* '<S159>/Trigonometric Function' */
  const real_T Product4_j;             /* '<S159>/Product4' */
  const real_T TrigonometricFunction2_i;/* '<S159>/Trigonometric Function2' */
  const real_T Switch1_d;              /* '<S148>/Switch1' */
  const real_T Sum_j;                  /* '<S148>/Sum' */
  const real_T Bias_a;                 /* '<S156>/Bias' */
  const real_T MathFunction1_j;        /* '<S156>/Math Function1' */
  const real_T Bias1_d;                /* '<S156>/Bias1' */
  const real_T Abs_mn;                 /* '<S156>/Abs' */
  const real_T Switch_b;               /* '<S156>/Switch' */
  const real_T Selector1[9];           /* '<S11>/Selector1' */
  const real_T Selector[9];            /* '<S11>/Selector' */
  const real_T Selector2[9];           /* '<S11>/Selector2' */
  const real32_T DataTypeConversion17; /* '<S1>/Data Type Conversion17' */
  const uint8_T Compare;               /* '<S109>/Compare' */
  const uint8_T Compare_c;             /* '<S157>/Compare' */
} ConstB_DynModel_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* Constant parameters (auto storage) */
typedef struct {
  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S2>/Memory3'
   *   '<S2>/Memory4'
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
  real_T pooled11[3];

  /* Pooled Parameter (Mixed Expressions)
   * Referenced by:
   *   '<S118>/Scale factors & Cross-coupling  errors'
   *   '<S119>/Scale factors & Cross-coupling  errors '
   */
  real_T pooled23[9];

  /* Expression: [ones(1,5)]
   * Referenced by: '<S5>/Discrete FIR Filter'
   */
  real_T DiscreteFIRFilter_Coefficients[5];
} ConstP_DynModel_T;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T PWM1;                         /* '<Root>/PWM1' */
  real_T PWM2;                         /* '<Root>/PWM2' */
  real_T PWM3;                         /* '<Root>/PWM3' */
  real_T PWM4;                         /* '<Root>/PWM4' */
  real_T n_collision[3];               /* '<Root>/n_collision' */
  real_T pen_collision;                /* '<Root>/pen_collision' */
  real_T Fext[3];                      /* '<Root>/Fext' */
  real_T Initial_LL[2];                /* '<Root>/Initial_LL' */
} ExtU_DynModel_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real32_T Temp;                       /* '<Root>/Temp' */
  real32_T Press;                      /* '<Root>/Press' */
  real32_T diff_Pres;                  /* '<Root>/diff_Pres' */
  real32_T Baro_Alt;                   /* '<Root>/Baro_Alt' */
  real32_T Gps_Lat;                    /* '<Root>/Gps_Lat' */
  real32_T Gps_Lon;                    /* '<Root>/Gps_Lon' */
  real32_T Gps_Alt;                    /* '<Root>/Gps_Alt' */
  real32_T Gps_V[3];                   /* '<Root>/Gps_V' */
  real32_T Gps_V_Mod;                  /* '<Root>/Gps_V_Mod' */
  real32_T COG;                        /* '<Root>/COG' */
  real32_T Lat_Lon_Alt[3];             /* '<Root>/Lat_Lon_Alt' */
  real32_T Magn[3];                    /* '<Root>/Magn' */
  real32_T RPY[3];                     /* '<Root>/RPY' */
  real32_T Accelerometer[3];           /* '<Root>/Accelerometer' */
  real32_T Gyro[3];                    /* '<Root>/Gyro' */
  real32_T Quaternion[4];              /* '<Root>/Quaternion' */
  real32_T Sonar;                      /* '<Root>/Sonar' */
  real32_T Forces[3];                  /* '<Root>/Forces' */
  real32_T Torques[3];                 /* '<Root>/Torques' */
  real32_T Thrusts[4];                 /* '<Root>/Thrusts' */
  real32_T Rotor_Speed[4];             /* '<Root>/Rotor_Speed' */
  real32_T Xe[3];                      /* '<Root>/Xe' */
  real32_T Freact[3];                  /* '<Root>/Freact' */
  real_T Sample_Time;                  /* '<Root>/Sample_Time' */
} ExtY_DynModel_T;

/* Real-time Model Data Structure */
struct tag_RTM_DynModel_T {
  const char_T *errorStatus;
  RTWSolverInfo *solverInfo;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    B_DynModel_T *blockIO;
    X_DynModel_T *contStates;
    int_T *periodicContStateIndices;
    real_T *periodicContStateRanges;
    real_T *derivs;
    ExtU_DynModel_T *inputs;
    ExtY_DynModel_T *outputs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T blkStateChange;
    real_T odeY[17];
    real_T odeF[4][17];
    ODE4_IntgData intgData;
    DW_DynModel_T *dwork;
  } ModelData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    boolean_T firstInitCondFlag;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

extern const ConstB_DynModel_T DynModel_ConstB;/* constant block i/o */

/* Constant parameters (auto storage) */
extern const ConstP_DynModel_T DynModel_ConstP;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T Crotdrag;                /* Variable: Crotdrag
                                        * Referenced by: '<S6>/h_ref6'
                                        */
extern real_T Kattreact;               /* Variable: Kattreact
                                        * Referenced by: '<S5>/Gain5'
                                        */
extern real_T Kdpenetration;           /* Variable: Kdpenetration
                                        * Referenced by:
                                        *   '<S5>/Gain1'
                                        *   '<S5>/Gain7'
                                        */
extern real_T Kpenetration;            /* Variable: Kpenetration
                                        * Referenced by:
                                        *   '<S5>/Gain12'
                                        *   '<S5>/Gain3'
                                        */
extern real_T Kt;                      /* Variable: Kt
                                        * Referenced by: '<S6>/Constant1'
                                        */
extern real_T Kvreact;                 /* Variable: Kvreact
                                        * Referenced by: '<S5>/Gain8'
                                        */
extern real_T LCdrag;                  /* Variable: LCdrag
                                        * Referenced by: '<S6>/h_ref1'
                                        */
extern real_T QCdrag;                  /* Variable: QCdrag
                                        * Referenced by: '<S6>/h_ref3'
                                        */
extern real_T motpole;                 /* Variable: motpole
                                        * Referenced by:
                                        *   '<S52>/Gain2'
                                        *   '<S53>/Gain2'
                                        *   '<S54>/Gain2'
                                        *   '<S55>/Gain2'
                                        */

/* External data declarations for dependent source files */
extern const char *RT_MEMORY_ALLOCATION_ERROR;

/* Model entry point functions */
extern RT_MODEL_DynModel_T *DynModel(void);
extern void DynModel_initialize(RT_MODEL_DynModel_T *const DynModel_M);
extern void DynModel_step(RT_MODEL_DynModel_T *const DynModel_M);
extern void DynModel_terminate(RT_MODEL_DynModel_T * DynModel_M);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('simulator_gen_monorate_feedback/DynModel')    - opens subsystem simulator_gen_monorate_feedback/DynModel
 * hilite_system('simulator_gen_monorate_feedback/DynModel/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'simulator_gen_monorate_feedback'
 * '<S1>'   : 'simulator_gen_monorate_feedback/DynModel'
 * '<S2>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics'
 * '<S3>'   : 'simulator_gen_monorate_feedback/DynModel/Sensors'
 * '<S4>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)'
 * '<S5>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Constraint_Interaction'
 * '<S6>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Dynamics'
 * '<S7>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics/LiPo Battery'
 * '<S8>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics/MotorDynamics'
 * '<S9>'   : 'simulator_gen_monorate_feedback/DynModel/Dynamics/transfer_function'
 * '<S10>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles'
 * '<S11>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate omega_dot'
 * '<S12>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Determine Force,  Mass & Inertia'
 * '<S13>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Vbxw'
 * '<S14>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Velocity Conversion'
 * '<S15>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Velocity Conversion1'
 * '<S16>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/transform to Inertial axes '
 * '<S17>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix'
 * '<S18>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to Rotation Angles'
 * '<S19>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Rotation Angles to Quaternions'
 * '<S20>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/qdot'
 * '<S21>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A11'
 * '<S22>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A12'
 * '<S23>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A13'
 * '<S24>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A21'
 * '<S25>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A22'
 * '<S26>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A23'
 * '<S27>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A31'
 * '<S28>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A32'
 * '<S29>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/A33'
 * '<S30>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/Create Transformation Matrix'
 * '<S31>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/Quaternion Normalize'
 * '<S32>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/Quaternion Normalize/Quaternion Modulus'
 * '<S33>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to  Direction Cosine Matrix/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
 * '<S34>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to Rotation Angles/Quaternion Normalize'
 * '<S35>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus'
 * '<S36>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate DCM & Euler Angles/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
 * '<S37>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate omega_dot/I x w'
 * '<S38>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate omega_dot/I x w1'
 * '<S39>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate omega_dot/wx(Iw)'
 * '<S40>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate omega_dot/wx(Iw)/Subsystem'
 * '<S41>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Calculate omega_dot/wx(Iw)/Subsystem1'
 * '<S42>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Vbxw/Subsystem'
 * '<S43>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/6DoF (Quaternion)/Vbxw/Subsystem1'
 * '<S44>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Constraint_Interaction/Discrete Derivative'
 * '<S45>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Constraint_Interaction/Discrete Derivative1'
 * '<S46>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Constraint_Interaction/MATLAB Function'
 * '<S47>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Dynamics/Angular Motion Drag'
 * '<S48>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Dynamics/Linear Motion Drag'
 * '<S49>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Dynamics/Quadrotor System'
 * '<S50>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Dynamics/Vehicle Weight'
 * '<S51>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/Dynamics/multicopter'
 * '<S52>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/MotorDynamics/MotDynamics1'
 * '<S53>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/MotorDynamics/MotDynamics2'
 * '<S54>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/MotorDynamics/MotDynamics3'
 * '<S55>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/MotorDynamics/MotDynamics4'
 * '<S56>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/transfer_function/Linear Second-Order Actuator'
 * '<S57>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/transfer_function/Linear Second-Order Actuator1'
 * '<S58>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/transfer_function/Linear Second-Order Actuator2'
 * '<S59>'  : 'simulator_gen_monorate_feedback/DynModel/Dynamics/transfer_function/Linear Second-Order Actuator3'
 * '<S60>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Altimeter'
 * '<S61>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Band-Limited White Noise1'
 * '<S62>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions'
 * '<S63>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS'
 * '<S64>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/ISA Atmosphere Model_'
 * '<S65>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000'
 * '<S66>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Magnetometer'
 * '<S67>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Pitot Sensor'
 * '<S68>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/PixFlow'
 * '<S69>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Pressure Sensor'
 * '<S70>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt'
 * '<S71>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Sonar'
 * '<S72>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Temperature Sensor'
 * '<S73>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace'
 * '<S74>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Positive Trace'
 * '<S75>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/trace(DCM)'
 * '<S76>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)'
 * '<S77>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)'
 * '<S78>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)'
 * '<S79>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/diag(DCM)'
 * '<S80>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) -sin(theta)'
 * '<S81>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/cos(theta)sin(phi) - (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S82>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/cos(theta)sin(psi) + (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S83>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/if s~=0; s=0.5//s'
 * '<S84>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(1,1)/u(1) -(u(5)+u(9)) +1'
 * '<S85>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) +sin(theta)'
 * '<S86>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/cos(theta)sin(phi) + (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S87>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/cos(theta)sin(psi) + (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S88>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/if s~=0; s=0.5//s'
 * '<S89>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(2,2)/u(5) -(u(1)+u(9)) +1'
 * '<S90>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) -sin(theta)'
 * '<S91>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/cos(theta)sin(phi) + (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S92>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/cos(theta)sin(psi) - (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S93>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/if s~=0; s=0.5//s'
 * '<S94>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Negative Trace/Maximum Value at DCM(3,3)/u(9) -(u(1)+u(5)) +1'
 * '<S95>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Positive Trace/cos(phi)sin(theta)cos(psi) + sin(phi)sin(psi) +sin(theta)'
 * '<S96>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Positive Trace/cos(theta)sin(phi) - (cos(phi)sin(theta)sin(psi) - sin(phi)cos(psi))'
 * '<S97>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/Direction Cosine Matrix  to Quaternions/Positive Trace/cos(theta)sin(psi) - (sin(phi)sin(theta)cos(psi) - cos(phi)sin(psi))'
 * '<S98>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1'
 * '<S99>'  : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap'
 * '<S100>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap1'
 * '<S101>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset'
 * '<S102>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/pos_deg'
 * '<S103>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap/Latitude Wrap 90'
 * '<S104>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap/Wrap Longitude'
 * '<S105>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap/Latitude Wrap 90/Compare To Constant'
 * '<S106>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap/Latitude Wrap 90/Wrap Angle 180'
 * '<S107>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap1/Latitude Wrap 90'
 * '<S108>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap1/Wrap Longitude'
 * '<S109>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap1/Latitude Wrap 90/Compare To Constant'
 * '<S110>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LatLong wrap1/Latitude Wrap 90/Wrap Angle 180'
 * '<S111>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset/Find Radian//Distance'
 * '<S112>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset/pos_deg'
 * '<S113>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset/Find Radian//Distance/Angle Conversion2'
 * '<S114>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset/Find Radian//Distance/denom'
 * '<S115>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset/Find Radian//Distance/e'
 * '<S116>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/GPS/Flat Earth to LLA1/LongLat_offset/Find Radian//Distance/e^4'
 * '<S117>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Acceleration Conversion'
 * '<S118>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer'
 * '<S119>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope'
 * '<S120>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Dynamics'
 * '<S121>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Random bias'
 * '<S122>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)'
 * '<S123>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/wdot x d'
 * '<S124>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Dynamics/No Dynamics'
 * '<S125>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Dynamics/Second-order Dynamics'
 * '<S126>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Dynamics/Second-order Dynamics/Transfer Fcn X'
 * '<S127>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Dynamics/Second-order Dynamics/Transfer Fcn Y'
 * '<S128>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/Dynamics/Second-order Dynamics/Transfer Fcn Z'
 * '<S129>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)/w x (w x d)'
 * '<S130>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)/w x d'
 * '<S131>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)/w x (w x d)/Subsystem'
 * '<S132>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)/w x (w x d)/Subsystem1'
 * '<S133>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)/w x d/Subsystem'
 * '<S134>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/w x (w x d)/w x d/Subsystem1'
 * '<S135>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/wdot x d/Subsystem'
 * '<S136>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Accelerometer/wdot x d/Subsystem1'
 * '<S137>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Dynamics'
 * '<S138>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Random bias'
 * '<S139>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Dynamics/No Dynamics'
 * '<S140>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Dynamics/Second-order Dynamics'
 * '<S141>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Dynamics/Second-order Dynamics/Transfer Fcn X'
 * '<S142>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Dynamics/Second-order Dynamics/Transfer Fcn Y'
 * '<S143>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Invensense MPU-6000/Three-axis Gyroscope/Dynamics/Second-order Dynamics/Transfer Fcn Z'
 * '<S144>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Magnetometer/Measurement Noise'
 * '<S145>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/PixFlow/Band-Limited White Noise4'
 * '<S146>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA'
 * '<S147>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap'
 * '<S148>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap1'
 * '<S149>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset'
 * '<S150>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/pos_deg'
 * '<S151>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap/Latitude Wrap 90'
 * '<S152>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap/Wrap Longitude'
 * '<S153>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap/Latitude Wrap 90/Compare To Constant'
 * '<S154>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap/Latitude Wrap 90/Wrap Angle 180'
 * '<S155>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap1/Latitude Wrap 90'
 * '<S156>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap1/Wrap Longitude'
 * '<S157>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap1/Latitude Wrap 90/Compare To Constant'
 * '<S158>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LatLong wrap1/Latitude Wrap 90/Wrap Angle 180'
 * '<S159>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset/Find Radian//Distance'
 * '<S160>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset/pos_deg'
 * '<S161>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset/Find Radian//Distance/Angle Conversion2'
 * '<S162>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset/Find Radian//Distance/denom'
 * '<S163>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset/Find Radian//Distance/e'
 * '<S164>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/RealLatLonAlt/Flat Earth to LLA/LongLat_offset/Find Radian//Distance/e^4'
 * '<S165>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Sonar/Band-Limited White Noise3'
 * '<S166>' : 'simulator_gen_monorate_feedback/DynModel/Sensors/Sonar/MATLAB Function'
 */
#endif                                 /* RTW_HEADER_DynModel_h_ */
