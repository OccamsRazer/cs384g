#include "beziercurveevaluator.h"
#include "vec.h"
#include "mat.h"
#include <cassert>

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
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
                 3.0, -6.0,  3.0,  0.0,
                -3.0,  3.0,  0.0,  0.0,
                 1.0,  0.0,  0.0,  0.0);
    if ( iCtrlPtCount < 4){
        for (int i = 0; i < iCtrlPtCount; i++)
            ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
    }
    else {

        for (int i = 0; i < iCtrlPtCount; i++)
            interpolatedCtrlPts.push_back(ptvCtrlPts[i]);

        if(bWrap && iCtrlPtCount % 3 == 0){
            // add copy of first to end
            interpolatedCtrlPts.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));

            // add copy of last 3 ctrlpoint to beginning
            interpolatedCtrlPts.insert(interpolatedCtrlPts.begin(), Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
            interpolatedCtrlPts.insert(interpolatedCtrlPts.begin(), Point(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 2].y));
            interpolatedCtrlPts.insert(interpolatedCtrlPts.begin(), Point(ptvCtrlPts[iCtrlPtCount - 3].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 3].y));
        }
        else if( bWrap && iCtrlPtCount % 3 != 0){
            // // add copy of first to end
            interpolatedCtrlPts.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));

            // initialize list with last element already in it
            ptvEvaluatedCurvePts.push_back(Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
        }
        int i = 0;
        int interCtrlPtsCount = interpolatedCtrlPts.size();
        // curve for multiple of 3
        for (; i + 3 < interCtrlPtsCount; i+=3) {
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
        // linear for remainder
        for( ; i < interCtrlPtsCount; i++){
            ptvEvaluatedCurvePts.push_back(interpolatedCtrlPts[i]);
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
