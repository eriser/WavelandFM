/*
  ==============================================================================

    FMOperator.h
    Created: 16 Jan 2016 5:54:15pm
    Author:  John Caplan

  ==============================================================================
*/

#ifndef FMOPERATOR_H_INCLUDED
#define FMOPERATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope.h"

class FMOP
{
public:
    FMOP();
    ~FMOP();
    float getOpstate();
    void updateAngleDelta ();
    void setOpPitch(float oppitch);
    void setOpLinearFm(float linearFM);
    void ResetPhase();
    void SetENVParam (float attack, float decay, float sustain, float release, float shape, float sampleRate);
    void StartOP(float midiNote, float sampleRate);
    void StopOp();
    float RenderOP ();
    
    inline float mtf (float noteNumber)
    {
        if (noteNumber < 0 || noteNumber > 148 || std::isnan(noteNumber))
        {
            return 1.0;
        }
        float index;
        int indexIntPart;
        float indexDecimelPart;
        float difference;
        index = noteNumber;
        indexIntPart = static_cast<int>(index);
        indexDecimelPart = index - indexIntPart;
        difference = MTF[indexIntPart +1] - MTF[indexIntPart];
            
        return MTF[indexIntPart] + (difference * indexDecimelPart);
    }
    
    inline float SineOfAngle (float angle)
    {
        float index;
        int indexIntPart;
        float indexDecimelPart;
        float difference;
        index = (fmod( std::abs(angle), twopi) / twopi * 256);
        indexIntPart = static_cast<int>(index);
        indexDecimelPart = index - indexIntPart;
        difference = sin256[indexIntPart + 1] - sin256[indexIntPart];
        
        return sin256[indexIntPart] + (difference * indexDecimelPart);
    }
    
private:
    float angleDelta, currentAngle;
    
    float OpSampleRate, OpMidiPitch, OpLinearFM;
    
    const float twopi = 2.0f * float_Pi;
    
    Envelope OpENV;
    
    float sin256 [257] =
    {
        
         1.000000,  0.999699,  0.998795,  0.997290,  0.995185,  0.992480,  0.989177,  0.985278,
         0.980785,  0.975702,  0.970031,  0.963776,  0.956940,  0.949528,  0.941544,  0.932993,
         0.923880,  0.914210,  0.903989,  0.893224,  0.881921,  0.870087,  0.857729,  0.844854,
         0.831470,  0.817585,  0.803208,  0.788346,  0.773010,  0.757209,  0.740951,  0.724247,
         0.707107,  0.689541,  0.671559,  0.653173,  0.634393,  0.615232,  0.595699,  0.575808,
         0.555570,  0.534998,  0.514103,  0.492898,  0.471397,  0.449611,  0.427555,  0.405241,
         0.382683,  0.359895,  0.336890,  0.313682,  0.290285,  0.266713,  0.242980,  0.219101,
         0.195090,  0.170962,  0.146730,  0.122411,  0.098017,  0.073565,  0.049068,  0.024541,
        
         0.000000, -0.024541, -0.049068, -0.073565, -0.098017, -0.122411, -0.146730, -0.170962,
        -0.195090, -0.219101, -0.242980, -0.266713, -0.290285, -0.313682, -0.336890, -0.359895,
        -0.382683, -0.405241, -0.427555, -0.449611, -0.471397, -0.492898, -0.514103, -0.534998,
        -0.555570, -0.575808, -0.595699, -0.615232, -0.634393, -0.653173, -0.671559, -0.689541,
        -0.707107, -0.724247, -0.740951, -0.757209, -0.773010, -0.788346, -0.803208, -0.817585,
        -0.831470, -0.844854, -0.857729, -0.870087, -0.881921, -0.893224, -0.903989, -0.914210,
        -0.923880, -0.932993, -0.941544, -0.949528, -0.956940, -0.963776, -0.970031, -0.975702,
        -0.980785, -0.985278, -0.989177, -0.992480, -0.995185, -0.997290, -0.998795, -0.999699,
        
        -1.000000, -0.999699, -0.998795, -0.997290, -0.995185, -0.992480, -0.989177, -0.985278,
        -0.980785, -0.975702, -0.970031, -0.963776, -0.956940, -0.949528, -0.941544, -0.932993,
        -0.923880, -0.914210, -0.903989, -0.893224, -0.881921, -0.870087, -0.857729, -0.844854,
        -0.831470, -0.817585, -0.803208, -0.788346, -0.773010, -0.757209, -0.740951, -0.724247,
        -0.707107, -0.689541, -0.671559, -0.653173, -0.634393, -0.615232, -0.595699, -0.575808,
        -0.555570, -0.534998, -0.514103, -0.492898, -0.471397, -0.449611, -0.427555, -0.405241,
        -0.382683, -0.359895, -0.336890, -0.313682, -0.290285, -0.266713, -0.242980, -0.219101,
        -0.195090, -0.170962, -0.146730, -0.122411, -0.098017, -0.073565, -0.049068, -0.024541,
        
         0.000000,  0.024541,  0.049068,  0.073565,  0.098017,  0.122411,  0.146730,  0.170962,
         0.195090,  0.219101,  0.242980,  0.266713,  0.290285,  0.313682,  0.336890,  0.359895,
         0.382683,  0.405241,  0.427555,  0.449611,  0.471397,  0.492898,  0.514103,  0.534998,
         0.555570,  0.575808,  0.595699,  0.615232,  0.634393,  0.653173,  0.671559,  0.689541,
         0.707107,  0.724247,  0.740951,  0.757209,  0.773010,  0.788346,  0.803208,  0.817585,
         0.831470,  0.844854,  0.857729,  0.870087,  0.881921,  0.893224,  0.903989,  0.914210,
         0.923880,  0.932993,  0.941544,  0.949528,  0.956940,  0.963776,  0.970031,  0.975702,
         0.980785,  0.985278,  0.989177,  0.992480,  0.995185,  0.997290,  0.998795,  0.999699,
        
         1.000000,
    };
    
    float MTF [150]
    {
        8.175798916, 8.661957218, 9.177023997, 9.722718241, 10.30086115, 10.91338223, 11.56232571, 12.24985737, 12.97827180, 13.75000000,
        14.56761755, 15.43385316, 16.35159783, 17.32391444, 18.35404799, 19.44543648, 20.60172231, 21.82676446, 23.12465142, 24.49971475,
        25.95654360, 27.50000000, 29.13523509, 30.86770633, 32.70319566, 34.64782887, 36.70809599, 38.89087297, 41.20344461, 43.65352893,
        46.24930284, 48.99942950, 51.91308720, 55.00000000, 58.27047019, 61.73541266, 65.40639133, 69.29565774, 73.41619198, 77.78174593,
        82.40688923, 87.30705786, 92.49860568, 97.99885900, 103.8261744, 110.0000000, 116.5409404, 123.4708253, 130.8127827, 138.5913155,
        
        146.8323840, 155.5634919, 164.8137785, 174.6141157, 184.9972114, 195.9977180, 207.6523488, 220.0000000, 233.0818808, 246.9416506,
        261.6255653, 277.1826310, 293.6647679, 311.1269837, 329.6275569, 349.2282314, 369.9944227, 391.9954360, 415.3046976, 440.0000000,
        466.1637615, 493.8833013, 523.2511306, 554.3652620, 587.3295358, 622.2539674, 659.2551138, 698.4564629, 739.9888454, 783.9908720,
        830.6093952, 880.0000000, 932.3275230, 987.7666025, 1046.502261, 1108.730524, 1174.659072, 1244.507935, 1318.510228, 1396.912926,
        1479.977691, 1567.981744, 1661.218790, 1760.000000, 1864.655046, 1975.533205, 2093.004522, 2217.461048, 2349.318143, 2489.015870,
        
        2637.020455, 2793.825851, 2959.955382, 3135.963488, 3322.437581, 3520.000000, 3729.310092, 3951.066410, 4186.009045, 4434.922096,
        4698.636287, 4978.031740, 5274.040911, 5587.651703, 5919.910763, 6271.926976, 6644.875161, 7040.000000, 7458.620184, 7902.132820,
        8372.018090, 8869.844191, 9397.272573, 9956.063479, 10548.08182, 11175.30341, 11839.82153, 12543.85395, 13289.75032, 14080.00000,
        14917.24037, 15804.26564, 16744.03618, 17739.68838, 18794.54515, 19912.12696, 21096.16364, 22350.60681, 23679.64305, 25087.70790,
        26579.50065, 28160.00000, 29834.48074, 31608.53128, 33488.07236, 35479.37677, 37589.09029, 39824.25392, 42192.32728, 44701.21362,
    };
    
};

#endif  // FMOPERATOR_H_INCLUDED
