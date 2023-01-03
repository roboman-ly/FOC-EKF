#ifndef FAST_MATH_H_
#define FAST_MATH_H_

/*!
    \brief      fast clipping algorithm
    \param[in]  x: number of to be clipped
    \param[in]  low: minimum number
    \param[in]  high: maximum number
    \retval     input value after clipping
*/
#define fast_constrain(x, low, high)    ((x)<(low)?(low):((x) >(high)?(high):(x)))

/*!
    \brief      fast calculation of cosine
    \param[in]  x: angle to be calculated
    \retval     cosine value of angle theta
*/
#define fast_cos(x)                     fast_sin(1.5707963f - x);

float fast_sin(float theta);

#endif /*FAST_MATH_H_*/
