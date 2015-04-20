#include "catmullromcurveevaluator.h"
#include "vec.h"
#include "mat.h"
#include <cassert>

void CatmullRomCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
                                         std::vector<Point>& ptvEvaluatedCurvePts, 
                                         const float& fAniLength, 
                                         const bool& bWrap) const
{
    int iCtrlPtCount = ptvCtrlPts.size();
    float stepSize = 1.0/100.0;
    ptvEvaluatedCurvePts.clear();

    float x = 0.0;
    float y1;

    // linear for less than 4 points
    std::vector<Point> interpolatedCtrlPts;
    Mat4d basis(-1.0,  3.0, -3.0,  1.0,
                 2.0, -5.0,  4.0, -1.0,
                -1.0,  0.0,  1.0,  0.0,
                 0.0,  2.0,  0.0,  0.0);
    basis /= 2.0;
    if ( iCtrlPtCount < 4){
        for (int i = 0; i < iCtrlPtCount; i++)
            ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
    }
    else {
        if(bWrap) {
            Point p(ptvCtrlPts[iCtrlPtCount-1].x - fAniLength, ptvCtrlPts[iCtrlPtCount-1].y);
            interpolatedCtrlPts.push_back(p);
            interpolatedCtrlPts.push_back(Point(2*p.x - ptvCtrlPts[0].x, 2*p.y-ptvCtrlPts[0].y));
        }
        else {
            // add fake first control point
            interpolatedCtrlPts.push_back(Point(2*ptvCtrlPts[0].x - ptvCtrlPts[1].x, 2*ptvCtrlPts[0].y-ptvCtrlPts[1].y));
        }

        for (int i = 0; i < iCtrlPtCount; i++)
            interpolatedCtrlPts.push_back(ptvCtrlPts[i]);

        if(bWrap) {
            Point p(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
            interpolatedCtrlPts.push_back(p);
            interpolatedCtrlPts.push_back(Point(2*p.x - ptvCtrlPts[iCtrlPtCount-1].x, 2*p.y-ptvCtrlPts[iCtrlPtCount-1].y));
        }
        else {
            // add fake last control point
            interpolatedCtrlPts.push_back(Point(2*ptvCtrlPts[iCtrlPtCount-1].x - ptvCtrlPts[iCtrlPtCount-2].x, 2*ptvCtrlPts[iCtrlPtCount-1].y-ptvCtrlPts[iCtrlPtCount-2].y));
        }
        int interCtrlPtsCount = interpolatedCtrlPts.size();

        for (int i = 0; i < interCtrlPtsCount - 3; i++) {
            Point p0 = interpolatedCtrlPts[i];
            Point p1 = interpolatedCtrlPts[i+1];
            Point p2 = interpolatedCtrlPts[i+2];
            Point p3 = interpolatedCtrlPts[i+3];

            for (float u=0; u <= 1; u += stepSize){
                Vec4d uVec( u*u*u, u*u, u, 1.0);
                float u2 = u*u;
                float u3 = u*u*u;

                Vec4d ub = uVec * basis;

                float x = ub[0] * p0.x + ub[1] * p1.x + ub[2] * p2.x + ub[3] * p3.x;
                float y = ub[0] * p0.y + ub[1] * p1.y + ub[2] * p2.y + ub[3] * p3.y;

                ptvEvaluatedCurvePts.push_back(Point(x, y));
            }
        }
    }

    if (bWrap) {
        // bell
    }
    else {
        // if wrapping is off, make the first and last segments of
        // the curve horizontal.

        y1 = ptvCtrlPts[0].y;
        ptvEvaluatedCurvePts.push_back(Point(x, y1));
    }


    /// set the endpoint based on the wrap flag.
    float y2;
    x = fAniLength;
    if (bWrap){
        // bell // y2 = y1;
    }
    else{
        y2 = ptvCtrlPts[iCtrlPtCount - 1].y;
        ptvEvaluatedCurvePts.push_back(Point(x, y2));
    }

}
