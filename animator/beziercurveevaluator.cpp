#include "beziercurveevaluator.h"
#include <cassert>

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
                                         std::vector<Point>& ptvEvaluatedCurvePts,
                                         const float& fAniLength,
                                         const bool& bWrap) const
{
    int iCtrlPtCount = ptvCtrlPts.size();
    float stepSize = 1.0/100.0;
    ptvEvaluatedCurvePts.clear();

    int i;
    // curve when there are enough points
    for (i=0; i+3<iCtrlPtCount; i+=3) {
        for (float u=0; u <= 1; u += stepSize) {
            float u_2   = u*u;
            float u1    = 1-u;
            float u1_2  = u1*u1;

            float u1_3 = u1*u1*u1;
            float u_u1_2_3 = u*u1_2*3;
            float u_2_u1_3 = u_2*u1*3;
            float u_3 = u*u*u;


            // x = (1-u)^3*p0 + 3*u*(1-u)^2*p1 + 3*u^2*(1-u)*p2 + u^3*p3
            float x = u1_3 * ptvCtrlPts[i].x + u_u1_2_3 * ptvCtrlPts[i+1].x + u_2_u1_3 * ptvCtrlPts[i+2].x + u_3 * ptvCtrlPts[i+3].x;
            float y = u1_3 * ptvCtrlPts[i].y + u_u1_2_3 * ptvCtrlPts[i+1].y + u_2_u1_3 * ptvCtrlPts[i+2].y + u_3 * ptvCtrlPts[i+3].y;

            // if (x > 0 && x < fAniLength)
            ptvEvaluatedCurvePts.push_back(Point(x, y));
        }
    }
    // linear for remainder
    for (; i < iCtrlPtCount; i++)
        ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);



    float x = 0.0;
    float y1;
    if(bWrap){
        // bell
    }
    else {
        // if wrapping is off, make the first and last segments of
        // the curve horizontal.

        y1 = ptvCtrlPts[0].y;
    }

    ptvEvaluatedCurvePts.push_back(Point(x, y1));

    // set the endpoint based on the wrap flag.
    float y2;
    x = fAniLength;
    if (bWrap){
       // bell // y2 = y1;
    }
    else{
        y2 = ptvCtrlPts[iCtrlPtCount - 1].y;
    }

    ptvEvaluatedCurvePts.push_back(Point(x, y2));
}
