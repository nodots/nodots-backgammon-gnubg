/*
 * Copyright (C) 2003 Gary Wong <gtw@gnu.org>
 * Copyright (C) 2004-2018 the AUTHORS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * $Id: speed.c,v 1.37 2021/11/26 22:58:10 plm Exp $
 */

#include "config.h"

#if defined(USE_GTK)
#include "gtkgame.h"
#else
#include "backgammon.h"
#endif
#if defined(USE_MULTITHREAD)
#include "multithread.h"
#endif
#ifndef WIN32
#include <stdlib.h>
#endif

#include "lib/isaac.h"
#include "lib/simd.h"

#define EVALS_PER_ITERATION 1024

static randctx rc;
static double timeTaken;

static void
RunEvals(void *UNUSED(notused))
{
    int aanBoard[EVALS_PER_ITERATION][2][25];
    int i, j, k;
    double t;
    SSE_ALIGN(float ar[NUM_OUTPUTS]);

#if defined(USE_MULTITHREAD)
    MT_Exclusive();
#endif
    for (i = 0; i < EVALS_PER_ITERATION; i++) {
        /* Generate a random board.  Don't allow chequers on the bar
         * or borne off, so we can trivially guarantee the position
         * is legal. */
        for (j = 0; j < 25; j++)
            aanBoard[i][0][j] = aanBoard[i][1][j] = 0;

        for (j = 0; j < 15; j++) {
            do {
                k = irand(&rc) % 24;
            } while (aanBoard[i][1][23 - k]);
            aanBoard[i][0][k]++;

            do {
                k = irand(&rc) % 24;
            } while (aanBoard[i][0][23 - k]);
            aanBoard[i][1][k]++;
        }
    }

#if defined(USE_MULTITHREAD)
    MT_Release();
    MT_SyncStart();
#else
    t = get_time();
#endif

    for (i = 0; i < EVALS_PER_ITERATION; i++) {
        (void) EvaluatePosition(NULL, (ConstTanBoard) aanBoard[i], ar, &ciCubeless, NULL);
    }

#if defined(USE_MULTITHREAD)
    if ((t = MT_SyncEnd()) > 0)
        timeTaken += t;
#else
    timeTaken += (get_time() - t);
#endif
}

extern void
CommandCalibrate(char *sz)
{
    int n = -1;
    unsigned int i, iIter, iCacheSize;
#if defined(USE_GTK)
    void *pcc = NULL;
#endif

    iCacheSize = GetEvalCacheEntries();
    EvalCacheResize(0);

#if defined(USE_MULTITHREAD)
    MT_SyncInit();
#endif

    if (sz && *sz) {
        n = ParseNumber(&sz);

        if (n < 1) {
            outputl(_("If you specify a parameter to `calibrate', " "it must be a number of iterations to run."));
            return;
        }
    }

    if (clock() == (clock_t) - 1) {
        outputl(_("Calibration not available."));
        return;
    }

    rc.randrsl[0] = (ub4) time(NULL);
    for (i = 0; i < RANDSIZ; i++)
        rc.randrsl[i] = rc.randrsl[0];
    irandinit(&rc, TRUE);

#if defined(USE_GTK)
    if (fX)
        pcc = GTKCalibrationStart();
#endif

    timeTaken = 0.0;
    for (iIter = 0; n < 0 || iIter < (unsigned int) n;) {
        double spd;
        if (fInterrupt)
            break;

#if defined(USE_MULTITHREAD)
        mt_add_tasks(MT_GetNumThreads(), RunEvals, NULL, NULL);
        (void) MT_WaitForTasks(NULL, 0, FALSE);
        iIter += MT_GetNumThreads();
#else
        RunEvals(NULL);
        iIter++;
#endif

        if (timeTaken <= 0.0)
            spd = 0.0;
        else
            spd = iIter * (EVALS_PER_ITERATION * 1000 / timeTaken);
#if defined(USE_GTK)
        if (fX)
            GTKCalibrationUpdate(pcc, (float) spd);
        else
#endif
        if (fShowProgress) {
            outputf("        \r");
            outputf(_("Calibrating: "));
            outputf(_("%.0f static evaluations/second"), spd);
            fflush(stdout);
        }
    }

    EvalCacheResize(iCacheSize);

#if defined(USE_GTK)
    if (fX)
        GTKCalibrationEnd(pcc);
#endif

    if (timeTaken > 0.0) {
        rEvalsPerSec = (float) iIter * (float) (EVALS_PER_ITERATION * 1000 / timeTaken);
        outputf("\r");
        outputf(_("Calibration result: "));
        outputf(_("%.0f static evaluations/second"), rEvalsPerSec);
        outputf(".\n");
    } else {
        outputf("\r");
        outputl(_("Calibration incomplete."));
    }
}
