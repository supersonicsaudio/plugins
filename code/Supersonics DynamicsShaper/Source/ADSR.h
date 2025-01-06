/*
  ==============================================================================

    ADSR.h
    Created: 30 Aug 2024 5:43:03pm
    Author:  Supersonics

  ==============================================================================
*/

//  For a complete explanation of the ADSR envelope generator and code,
//  read the series of articles by the author, starting here:
//  http://www.earlevel.com/main/2013/06/01/envelope-generators/

#ifndef _ADRS_h
#define _ADRS_h


class SS_ADSR {
public:
    SS_ADSR(void);
	~SS_ADSR(void);
    float process(void);
    float getOutput(void);
    int getState(void);
	void gate(int on);
    void setAttackRate(float rate);
    void setDecayRate(float rate);
    void setReleaseRate(float rate);
	void setSustainLevel(float level);
    void setTargetRatioA(float targetRatio);
    void setTargetRatioDR(float targetRatio);
    void reset(void);

    enum envState {
        env_idle = 0,
        env_attack,
        env_decay,
        env_sustain,
        env_release
    };

protected:
	int state;
    float output;
    float attackRate;
    float decayRate;
    float releaseRate;
    float attackCoef;
    float decayCoef;
    float releaseCoef;
    float sustainLevel;
    float targetRatioA;
    float targetRatioDR;
    float attackBase;
    float decayBase;
    float releaseBase;
 
    float calcCoef(float rate, float targetRatio);
};

inline float SS_ADSR::process() {
	switch (state) {
        case env_idle:
            break;
        case env_attack:
            output = attackBase + output * attackCoef;
            if (output >= 1.0f) {
                output = 1.0f;
                state = env_decay;
            }
            break;
        case env_decay:
            output = decayBase + output * decayCoef;
            if (output <= sustainLevel) {
                output = sustainLevel;
                state = env_sustain;
            }
            break;
        case env_sustain:
            break;
        case env_release:
            output = releaseBase + output * releaseCoef;
            if (output <= 0.0f) {
                output = 0.0f;
                state = env_idle;
            }
	}
	return output;
}

inline void SS_ADSR::gate(int gate) {
	if (gate)
		state = env_attack;
	else if (state != env_idle)
        state = env_release;
}

inline int SS_ADSR::getState() {
    return state;
}

inline void SS_ADSR::reset() {
    state = env_idle;
    output = 0.0f;
}

inline float SS_ADSR::getOutput() {
	return output;
}

#endif
