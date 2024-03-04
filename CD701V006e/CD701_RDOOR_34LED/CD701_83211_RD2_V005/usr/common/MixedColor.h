#ifndef __Mixed_Color_H__
#define __Mixed_Color_H__


typedef struct {
    double Cx;
    double Cy;
    uint16_t brightness;
}xyYParam;

typedef struct {
    double X;
    double Y;
    double Z;
}XYZParam;

typedef struct {
    uint8_t sR;
    uint8_t sG;
    uint8_t sB;
}RGBParam;


void InitMatrix(xyYParam xyYActulParam);
void ObjectMatrix();
float *CalculateDutyCycle(xyYParam xyYActulParam);

XYZParam xyYtoXYZ(xyYParam xyYActulParam);
RGBParam XYZtoRGB(XYZParam XYZActulParam);
RGBParam xyYtoRGB(xyYParam xyYActulParam);

XYZParam RGBtoXYZ(RGBParam RGBActulParam);
xyYParam XYZtoxyY(XYZParam XYZActulParam);
xyYParam RGBtoxyY(RGBParam RGBActulParam);


#endif
