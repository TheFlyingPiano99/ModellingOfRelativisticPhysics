#pragma once

#include "framework.h"
#include "RelPhysics.h"

#include <iostream>
#include <math.h>

/*
* Calculates coeficient for Doppler shift of perceived color.
*/
static float calculateDopplerShift(vec4 observersVelocity, vec4 subjectsVelocity, vec4 observersLocation, vec4 subjectsLocation) {

    vec4 toSubject4 = subjectsLocation - observersLocation;
    vec3 toSubject = normalize(vec3(toSubject4.x, toSubject4.y, toSubject4.z));

    vec3 relVelocity = RelPhysics::lorentzTransformationOfVelocity(
        RelPhysics::To3DVelocity(subjectsVelocity),
        RelPhysics::To3DVelocity(observersVelocity));

    float v = dot(toSubject, relVelocity);  //Approach speed
    float dopplerShift = sqrtf((RelPhysics::speedOfLight + v) / (RelPhysics::speedOfLight - v));
    return dopplerShift;
}

static double Gamma = 0.80;
static double IntensityMax = 255;

/**
 * Taken from Earl F. Glynn's web page:
 * <a href="http://www.efg2.com/Lab/ScienceAndEngineering/Spectra.htm">Spectra Lab Report</a>
 */
static vec3 waveLengthToRGB(double Wavelength) {
    double factor;
    double Red, Green, Blue;

    if ((Wavelength >= 380) && (Wavelength < 440)) {
        Red = -(double)(Wavelength - 440) / (double)(440 - 380);
        Green = 0.0;
        Blue = 1.0;
    }
    else if ((Wavelength >= 440) && (Wavelength < 490)) {
        Red = 0.0;
        Green = (double)(Wavelength - 440) / (double)(490 - 440);
        Blue = 1.0;
    }
    else if ((Wavelength >= 490) && (Wavelength < 510)) {
        Red = 0.0;
        Green = 1.0;
        Blue = -(double)(Wavelength - 510) / (double)(510 - 490);
    }
    else if ((Wavelength >= 510) && (Wavelength < 580)) {
        Red = (double)(Wavelength - 510) / (double)(580 - 510);
        Green = 1.0;
        Blue = 0.0;
    }
    else if ((Wavelength >= 580) && (Wavelength < 645)) {
        Red = 1.0;
        Green = -(double)(Wavelength - 645) / (double)(645 - 580);
        Blue = 0.0;
    }
    else if ((Wavelength >= 645) && (Wavelength < 781)) {
        Red = 1.0;
        Green = 0.0;
        Blue = 0.0;
    }
    else {
        Red = 0.0;
        Green = 0.0;
        Blue = 0.0;
    }

    // Let the intensity fall off near the vision limits

    if ((Wavelength >= 380) && (Wavelength < 420)) {
        factor = 0.3 + 0.7 * (double)(Wavelength - 380) / (double)(420 - 380);
    }
    else if ((Wavelength >= 420) && (Wavelength < 701)) {
        factor = 1.0;
    }
    else if ((Wavelength >= 701) && (Wavelength < 781)) {
        factor = 0.3 + 0.7 * (double)(780 - Wavelength) / (double)(780 - 700);
    }
    else {
        factor = 0.0;
    }


    vec3 rgb;

    // Don't want 0^x = 1 for x <> 0
    rgb.x = Red == 0.0f ? 0.0f : (int)round(IntensityMax * pow(Red * factor, Gamma));
    rgb.y = Green == 0.0f ? 0.0f : (int)round(IntensityMax * pow(Green * factor, Gamma));
    rgb.z = Blue == 0.0f ? 0.0f : (int)round(IntensityMax * pow(Blue * factor, Gamma));
    rgb = rgb / 256.0f;
    std::cout << "Color Shifted: " << rgb.x <<", " << rgb.y << ", " << rgb.z << ", " << std::endl;
    return rgb;
}