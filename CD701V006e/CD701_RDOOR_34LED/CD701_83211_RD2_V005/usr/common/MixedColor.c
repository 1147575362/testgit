#include <applicationTask.h>
#include <MixedColor.h>
#include <math.h>


float matrixADeterminant,xm,ym;
uint16_t Ym;
float matrixA[3][3];
float matrixA2[3][3];
float matrixA3[3][3];

float matrixB[3];
float matrixD[3];


/*PWM混色**************************************************************/
void InitMatrix(xyYParam xyYActulParam)
{
  	uint16_t R_BRIGHTNESS = systemParam.info.ledParams.led[0].maxTypicalIntensity[LED_RED];
	uint16_t G_BRIGHTNESS = systemParam.info.ledParams.led[0].maxTypicalIntensity[LED_GREEN];
	uint16_t B_BRIGHTNESS = systemParam.info.ledParams.led[0].maxTypicalIntensity[LED_BLUE];
	uint16_t R_CX = systemParam.info.ledParams.led[0].color[LED_RED].x;
	uint16_t R_CY = systemParam.info.ledParams.led[0].color[LED_RED].y;
	uint16_t G_CX = systemParam.info.ledParams.led[0].color[LED_GREEN].x;
	uint16_t G_CY = systemParam.info.ledParams.led[0].color[LED_GREEN].y;
	uint16_t B_CX = systemParam.info.ledParams.led[0].color[LED_BLUE].x;
	uint16_t B_CY = systemParam.info.ledParams.led[0].color[LED_BLUE].y;
    //计算xm，ym，Ym
//    xm=(R_CX*R_BRIGHTNESS/R_CY+G_CX*G_BRIGHTNESS/G_CY+B_CX*B_BRIGHTNESS/B_CY)/(R_BRIGHTNESS/R_CY+G_BRIGHTNESS/G_CY+B_BRIGHTNESS/B_CY);
//    ym=(R_CY*R_BRIGHTNESS/R_CY+G_CY*G_BRIGHTNESS/G_CY+B_CY*B_BRIGHTNESS/B_CY)/(R_BRIGHTNESS/R_CY+G_BRIGHTNESS/G_CY+B_BRIGHTNESS/B_CY);
//    Ym=R_BRIGHTNESS+G_BRIGHTNESS+B_BRIGHTNESS;
    xm=xyYActulParam.Cx;
    ym=xyYActulParam.Cy;
    Ym=xyYActulParam.brightness;

    //计算matrixA
    matrixA[0][0]=R_BRIGHTNESS*(xm-R_CX)/R_CY;
    matrixA[0][1]=G_BRIGHTNESS*(xm-G_CX)/G_CY;
    matrixA[0][2]=B_BRIGHTNESS*(xm-B_CX)/B_CY;

    matrixA[1][0]=R_BRIGHTNESS*(ym-R_CY)/R_CY;
    matrixA[1][1]=G_BRIGHTNESS*(ym-G_CY)/G_CY;
    matrixA[1][2]=B_BRIGHTNESS*(ym-B_CY)/B_CY;

    matrixA[2][0]=R_BRIGHTNESS;
    matrixA[2][1]=G_BRIGHTNESS;
    matrixA[2][2]=B_BRIGHTNESS;

    //计算matrixB
    matrixB[0]=0;
    matrixB[1]=0;
    matrixB[2]=Ym;

    //计算行列式
    matrixADeterminant=
    matrixA[0][0]*matrixA[1][1]*matrixA[2][2]+
    matrixA[0][1]*matrixA[1][2]*matrixA[2][0]+
    matrixA[0][2]*matrixA[1][0]*matrixA[2][1]-
    matrixA[0][2]*matrixA[1][1]*matrixA[2][0]-
    matrixA[0][0]*matrixA[1][2]*matrixA[2][1]-
    matrixA[0][1]*matrixA[1][0]*matrixA[2][2];
}

void ObjectMatrix()
{
    matrixA2[0][0]=(matrixA[1][1]*matrixA[2][2]-matrixA[1][2]*matrixA[2][1]);
    matrixA2[1][0]=-(matrixA[1][0]*matrixA[2][2]-matrixA[1][2]*matrixA[2][0]);//正负系数的实际值是-1的i+j+2次方
    matrixA2[2][0]=(matrixA[1][0]*matrixA[2][1]-matrixA[1][1]*matrixA[2][0]);

    matrixA2[0][1]=-(matrixA[0][1]*matrixA[2][2]-matrixA[0][2]*matrixA[2][1]);
    matrixA2[1][1]=(matrixA[0][0]*matrixA[2][2]-matrixA[0][2]*matrixA[2][0]);
    matrixA2[2][1]=-(matrixA[0][0]*matrixA[2][1]-matrixA[0][1]*matrixA[2][0]);

    matrixA2[0][2]=(matrixA[0][1]*matrixA[1][2]-matrixA[0][2]*matrixA[1][1]);
    matrixA2[1][2]=-(matrixA[0][0]*matrixA[1][2]-matrixA[0][2]*matrixA[1][0]);
    matrixA2[2][2]=(matrixA[0][0]*matrixA[1][1]-matrixA[0][1]*matrixA[1][0]);
    for(uint8_t i=0;i<3;i++)
    {
        for(uint8_t j=0;j<3;j++)
        {
            matrixA3[i][j]=matrixA2[i][j]/matrixADeterminant;
        }
    }
    matrixD[0]=matrixA3[0][0]*matrixB[0]+matrixA3[0][1]*matrixB[1]+matrixA3[0][2]*matrixB[2];
    matrixD[1]=matrixA3[1][0]*matrixB[0]+matrixA3[1][1]*matrixB[1]+matrixA3[1][2]*matrixB[2];
    matrixD[2]=matrixA3[2][0]*matrixB[0]+matrixA3[2][1]*matrixB[1]+matrixA3[2][2]*matrixB[2];
    for(uint8_t i=0;i<3;i++)
    {
        if(matrixD[i]<0) matrixD[i]=0;  //校正占空比
        if(matrixD[i]>1) matrixD[i]=1;
    }
}

float *CalculateDutyCycle(xyYParam xyYActulParam)
{
    InitMatrix(xyYActulParam);
    if(matrixADeterminant!=0)
    {
        ObjectMatrix();
    }
    return matrixD;
}
/**********************************************************************/


/* xyY -> RGB *********************************************************/
XYZParam xyYtoXYZ(xyYParam xyYActulParam)
{
    XYZParam XYZParamTemp;
    
    XYZParamTemp.X = xyYActulParam.Cx * (((double)xyYActulParam.brightness*100/1023) / xyYActulParam.Cy);
    XYZParamTemp.Y = (double)(xyYActulParam.brightness)*100/1023;
    XYZParamTemp.Z = (1 - xyYActulParam.Cx - xyYActulParam.Cy) * (((double)xyYActulParam.brightness*100/1023) / xyYActulParam.Cy);

    return XYZParamTemp;
}
RGBParam XYZtoRGB(XYZParam XYZActulParam)
{
    RGBParam RGBParamTemp;
    double var_X, var_Y, var_Z, var_R, var_G, var_B;

    var_X = XYZActulParam.X / 100;
    var_Y = XYZActulParam.Y / 100;
    var_Z = XYZActulParam.Z / 100;

    //var_R = var_X * 3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
    //var_G = var_X * -0.9689 + var_Y * 1.8758 + var_Z * 0.0415;
    //var_B = var_X * 0.0557 + var_Y * -0.2040 + var_Z * 1.0570;
    var_R = var_X * 3.2410 + var_Y * -1.5374 + var_Z * -0.4986;
    var_G = var_X * -0.9692 + var_Y * 1.8760 + var_Z * 0.0416;
    var_B = var_X * 0.0556 + var_Y * -0.2040 + var_Z * 1.0570;

    if (var_R > 0.0031308) var_R = 1.055 * (pow(var_R, (1 / 2.4))) - 0.055;
    else                   var_R = 12.92 * var_R;
    if (var_G > 0.0031308) var_G = 1.055 * (pow(var_G, (1 / 2.4))) - 0.055;
    else                   var_G = 12.92 * var_G;
    if (var_B > 0.0031308) var_B = 1.055 * (pow(var_B, (1 / 2.4))) - 0.055;
    else                   var_B = 12.92 * var_B;

    var_R = var_R > 0 ? var_R : 0;
    var_G = var_G > 0 ? var_G : 0;
    var_B = var_B > 0 ? var_B : 0;
    var_R = var_R < 1 ? var_R : 1;
    var_G = var_G < 1 ? var_G : 1;
    var_B = var_B < 1 ? var_B : 1;

    RGBParamTemp.sR = (uint8_t)(var_R * 255);
    RGBParamTemp.sG = (uint8_t)(var_G * 255);
    RGBParamTemp.sB = (uint8_t)(var_B * 255);

    return RGBParamTemp;
}
RGBParam xyYtoRGB(xyYParam xyYActulParam)
{
    return XYZtoRGB(xyYtoXYZ(xyYActulParam));
}
/**********************************************************************/


/* RGB -> xyY *********************************************************/
XYZParam RGBtoXYZ(RGBParam RGBActulParam)
{
    XYZParam XYZParamTemp;
    double var_R, var_G, var_B;

    var_R = ((double)RGBActulParam.sR) / 255;
    var_G = ((double)RGBActulParam.sG) / 255;
    var_B = ((double)RGBActulParam.sB) / 255;

    if (var_R > 0.04045) var_R = pow(((var_R + 0.055) / 1.055), 2.4);
    else                   var_R = var_R / 12.92;
    if (var_G > 0.04045) var_G = pow(((var_G + 0.055) / 1.055), 2.4);
    else                   var_G = var_G / 12.92;
    if (var_B > 0.04045) var_B = pow(((var_B + 0.055) / 1.055), 2.4);
    else                   var_B = var_B / 12.92;

    var_R = var_R * 100;
    var_G = var_G * 100;
    var_B = var_B * 100;

    XYZParamTemp.X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
    XYZParamTemp.Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
    XYZParamTemp.Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

    return XYZParamTemp;
}
xyYParam XYZtoxyY(XYZParam XYZActulParam)
{
    xyYParam xyYParamTemp;
    xyYParamTemp.Cx = XYZActulParam.X / (XYZActulParam.X + XYZActulParam.Y + XYZActulParam.Z);
    xyYParamTemp.Cy = XYZActulParam.Y / (XYZActulParam.X + XYZActulParam.Y + XYZActulParam.Z);
    xyYParamTemp.brightness = (uint16_t)(XYZActulParam.Y*1023/100);

    return xyYParamTemp;
}
xyYParam RGBtoxyY(RGBParam RGBActulParam)
{
    return XYZtoxyY(RGBtoXYZ(RGBActulParam));
}
/**********************************************************************/


