/* Copyright 2013-2015 The MathWorks, Inc. */

#ifdef SUPPORTS_PRAGMA_ONCE
#pragma once
#endif

#ifndef SLLOGLOADBLOCKSSFCNBRIDGE_H
#define SLLOGLOADBLOCKSSFCNBRIDGE_H

#include "sl_log_load_blocks_spec.h"

SL_LOG_LOAD_BLOCKS_EXPORT_EXTERN_C
void slSigLogAddElementDescriptionForBus(SimStruct* sfcnS,
                                         void **ppDatasetDesc,
                                         int_T hierInfoIdx,
                                         const char_T *busName,
                                         const char_T *signalName,
                                         const char_T *propName,
                                         const char_T *blockPath,
                                         int_T portIdx,            
                                         int_T maxPoints,          
                                         int_T decimation,         
                                         int_T numDims,            
                                         const int_T *dims,         
                                         SSLoggerInterpMethod interp);

SL_LOG_LOAD_BLOCKS_EXPORT_EXTERN_C
void slSigLogAddElementDescriptionForStateflowBus(SimStruct* sfcnS,
                                         void **ppDatasetDesc,
                                         int_T hierInfoIdx,
                                         const char_T *busName,
                                         const char_T *signalName,
                                         const char_T *blockPath,
                                         int_T portIdx,            
                                         int_T maxPoints,          
                                         int_T decimation,         
                                         int_T numDims,            
                                         const int_T *dims,         
                                         SSLoggerInterpMethod interp,
                                         const char_T *className,
                                         const char_T *chartPath);

SL_LOG_LOAD_BLOCKS_EXPORT_EXTERN_C
void slSigLogUpdateDatasetElementForBus(SimStruct* sfcnS,
                                        void *pwork,
                                        real_T inputTime,
                                        const char *inputData);

SL_LOG_LOAD_BLOCKS_EXPORT_EXTERN_C
void slReportToWksNoRTWSupportForDataset(SimStruct* sfcnS,
                                         const char_T *logBlockPath,
                                         const char_T *saveFormat);

SL_LOG_LOAD_BLOCKS_EXPORT_EXTERN_C
void slSigLogAddTimeseriesDescription(SimStruct* sfcnS,
                                      void         **ppElementDesc,
                                      void         **ppDatasetDesc,
                                      const char_T *name,
                                      int_T        nDims,
                                      const int_T  *dims,
                                      DTypeId      dataType,
                                      boolean_T    complexity,
                                      SSLoggerInterpMethod interp,
                                      const char_T *units);
SL_LOG_LOAD_BLOCKS_EXPORT_EXTERN_C
void rtwGetLoggingIntervalFromSFBlock(void *pOpaqueSFBlock,
                                      void **outLoggingInterval);

#endif
