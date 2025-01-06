/*
  ==============================================================================

    ADSR.cpp
    Created: 30 Aug 2024 5:43:03pm
    Author:  Supersonics

  ==============================================================================
*/


//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/

#include "ADSR.h"
#include <math.h>

SS_ADSR::SS_ADSR(void) {
    reset();
    setAttackRate(0.0f);
    setDecayRate(0.0f);
    setReleaseRate(0.0f);
    setSustainLevel(1.0f);
    setTargetRatioA(0.3f);
    setTargetRatioDR(0.0001f);
}

SS_ADSR::~SS_ADSR(void) {
}

void SS_ADSR::setAttackRate(float rate) {
    attackRate = rate;
    attackCoef = calcCoef(rate, targetRatioA);
    attackBase = (1.0f + targetRatioA) * (1.0f - attackCoef);
}

void SS_ADSR::setDecayRate(float rate) {
    decayRate = rate;
    decayCoef = calcCoef(rate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0f - decayCoef);
}

void SS_ADSR::setReleaseRate(float rate) {
    releaseRate = rate;
    releaseCoef = calcCoef(rate, targetRatioDR);
    releaseBase = -targetRatioDR * (1.0f - releaseCoef);
}

float SS_ADSR::calcCoef(float rate, float targetRatio) {
    return (rate <= 0.0f) ? 0.0f : expf(-logf((1.0f + targetRatio) / targetRatio) / rate);
}

void SS_ADSR::setSustainLevel(float level) {
    sustainLevel = level;
    decayBase = (sustainLevel - targetRatioDR) * (1.0f - decayCoef);
}

void SS_ADSR::setTargetRatioA(float targetRatio) {
    if (targetRatio < 0.000000001f)
        targetRatio = 0.000000001f;  // -180 dB
    targetRatioA = targetRatio;
    attackCoef = calcCoef(attackRate, targetRatioA);
    attackBase = (1.0f + targetRatioA) * (1.0f - attackCoef);
}

void SS_ADSR::setTargetRatioDR(float targetRatio) {
    if (targetRatio < 0.000000001f)
        targetRatio = 0.000000001f;  // -180 dB
    targetRatioDR = targetRatio;
    decayCoef = calcCoef(decayRate, targetRatioDR);
    releaseCoef = calcCoef(releaseRate, targetRatioDR);
    decayBase = (sustainLevel - targetRatioDR) * (1.0f - decayCoef);
    releaseBase = -targetRatioDR * (1.0f - releaseCoef);
}
