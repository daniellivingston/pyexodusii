/*
 * Copyright(C) 1999-2021 National Technology & Engineering Solutions
 * of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
 * NTESS, the U.S. Government retains certain rights in this software.
 *
 * See packages/seacas/LICENSE for details
 */

#include <math.h>

/* Evaluates principal minor polynomial, returns the index of the eigenvalue just
   left of mu. Based on Wilkinson's algorithm AEP, p.302. Found that this algorithm
   could fail in practice when the p recursion overflowed. Fixed by rescaling the
   recursion if it got too large. Routine returns -1 if this re-scaling doesn't work
   for some reason. Otherwise it returns 0.  Note Wilkinson indexes beta such that
   first off-diagonal entry in T is beta[2]. We index such that it is beta[1]. */

int sturmcnt(double *alpha, /* vector of Lanczos scalars */
             double *beta,  /* vector of Lanczos scalars */
             int     j,     /* index of Lanczos step we're on */
             double  mu,    /* argument to the sequence generating polynomial */
             double *p      /* work vector for sturm sequence */
)
{
  extern double DOUBLE_MAX; /* maximum double precision number to be used */
  int           cnt;        /* number of sign changes in the sequence */

  if (j == 1) {
    /* have to special case this one */
    if (alpha[1] > mu) {
      cnt = 1;
    }
    else {
      cnt = 0;
    }
  }
  else {
    /* compute the Sturm sequence */
    double limit       = sqrt(DOUBLE_MAX);
    p[0]               = 1;
    p[1]               = alpha[1] - mu;
    double *pntr_p     = &p[2];
    double *pntr_p1    = &p[1];
    double *pntr_p2    = &p[0];
    double *pntr_alpha = &alpha[2];
    double *pntr_beta  = &beta[1];
    for (int i = 2; i <= j; i++) {
      *pntr_p++ = (*pntr_alpha - mu) * (*pntr_p1) - (*pntr_beta) * (*pntr_beta) * (*pntr_p2);
      pntr_alpha++;
      pntr_beta++;
      pntr_p1++;
      pntr_p2++;
      double scale = fabs(*pntr_p1);
      if (scale > limit) {
        *pntr_p1 /= scale;
        *pntr_p2 /= scale;
        /* re-scale to avoid overflow in p recursion */
      }
    }

    /* count sign changes */
    cnt           = 0;
    int last_sign = 1;
    pntr_p        = &p[1];
    for (int i = j; i; i--) {
      if (*pntr_p != *pntr_p || fabs(*pntr_p) > limit) {
        return (-1);
        /* ... re-scaling failed; bail out and return error code.
               Note (x != x) is TRUE iff x is NaN, so this check
               should be ok on non-IEEE floating point systems too. */
      }
      int sign;
      if (*pntr_p > 0) {
        sign = 1;
      }
      else if (*pntr_p < 0) {
        sign = -1;
      }
      else {
        sign = -last_sign;
      }
      if (sign == last_sign) {
        cnt++;
      }
      last_sign = sign;
      pntr_p++;
    }
  }

  /* cnt is number of evals strictly > than mu. Instead send back index of the
     eigenvalue just left of mu. */
  return (j - cnt); /* ... things seem ok. */
}
