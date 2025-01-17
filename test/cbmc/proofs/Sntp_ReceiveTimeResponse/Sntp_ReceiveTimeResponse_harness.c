/*
 * coreSNTP v1.2.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file Sntp_ReceiveTimeResponse_harness.c
 * @brief Implements the proof harness for Sntp_ReceiveTimeResponse function.
 */

#include "core_sntp_cbmc_state.h"
#include "core_sntp_client.h"
#include "core_sntp_stubs.h"
#include <stddef.h>

void harness()
{
    SntpContext_t * pContext;
    uint32_t blockTimeMs;
    SntpStatus_t sntpStatus;

    pContext = unconstrainedCoreSntpContext();

    if( pContext != NULL )
    {
        /* Setting the initial value of request time to check for response
         * timeout while reading data from network. */
        GetTimeFuncStub( &pContext->lastRequestTime );
    }

    /* The SNTP_RECEIVE_TIMEOUT is used here to control the number of loops
     * when receiving on the network. The default is used here because memory
     * safety can be proven in only a few iterations. Please see this proof's
     * Makefile for more information. */
    __CPROVER_assume( blockTimeMs < SNTP_RECEIVE_TIMEOUT );

    sntpStatus = Sntp_ReceiveTimeResponse( pContext, blockTimeMs );

    __CPROVER_assert( ( sntpStatus == SntpErrorBadParameter ||
                        sntpStatus == SntpSuccess ||
                        sntpStatus == SntpNoResponseReceived ||
                        sntpStatus == SntpRejectedResponse ||
                        sntpStatus == SntpErrorResponseTimeout ||
                        sntpStatus == SntpErrorNetworkFailure ),
                      "The return value is not a valid coreSNTP Status" );
}
