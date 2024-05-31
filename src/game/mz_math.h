#ifndef MZ_MATH
#define MZ_MATH

#include "cmath"
#include "raylib.h"

namespace Math
{
    //------------------------------------------------------------------------------------
    // INTERPOLATION FUNCTIONS
    //------------------------------------------------------------------------------------

    double easeInSine(double t);
    double easeOutSine(double t);
    double easeInOutSine(double t);
    double easeInQuad(double t);
    double easeOutQuad(double t);
    double easeInOutQuad(double t);
    double easeInCubic(double t);
    double easeOutCubic(double t);
    double easeInOutCubic(double t);
    double easeInQuart(double t);
    double easeOutQuart(double t);
    double easeInOutQuart(double t);
    double easeInQuint(double t);
    double easeOutQuint(double t);
    double easeInOutQuint(double t);
    double easeInExpo(double t);
    double easeOutExpo(double t);
    double easeInOutExpo(double t);
    double easeInCirc(double t);
    double easeOutCirc(double t);
    double easeInOutCirc(double t);
    double easeInBack(double t);
    double easeOutBack(double t);
    double easeInOutBack(double t);
    double easeInElastic(double t);
    double easeOutElastic(double t);
    double easeInOutElastic(double t);
    double easeInBounce(double t);
    double easeOutBounce(double t);
    double easeInOutBounce(double t);

    //------------------------------------------------------------------------------------
    // MATH FUNCTIONS
    //------------------------------------------------------------------------------------
    float floatClamp(float value, float min, float max);
    float floatLerp(float from, float to, float dt);
    float floatDamp(float from, float to, float speed, float dt);
    Vector3 vector3Lerp(Vector3 v1, Vector3 v2, float amount);

};

#endif