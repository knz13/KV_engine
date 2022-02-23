#pragma once
#include "../global.h"



class DrawingMode {
    KV_DRAWING_MODE
protected:
    virtual GLenum GetDrawingType(){
        return GL_TRIANGLES;
    };
};


enum class LineModeType {
    Lines = 0,
    LineStrip = 1,
    LineLoop = 2
};

class LineMode : public DrawingMode {
    KV_DRAWING_MODE
public:
    LineModeType type = LineModeType::Lines;

protected:
    GLenum GetDrawingType() override {
        switch(type){
        case LineModeType::Lines:
            return GL_LINES;
        case LineModeType::LineStrip:
            return GL_LINE_STRIP;
        case LineModeType::LineLoop:
            return GL_LINE_LOOP;
        default:
            break;
        };

    };
};

enum class TriangleModeType {
    Triangle = 0,
    TriangleStrip = 1,
    TriangleFan = 2
};

class TriangleMode : public DrawingMode {
    KV_DRAWING_MODE

public:
    TriangleModeType type = TriangleModeType::Triangle;

protected:
    GLenum GetDrawingType() override {
        switch(type){
        case TriangleModeType::Triangle:
            return GL_TRIANGLES;
        case TriangleModeType::TriangleStrip:
            return GL_TRIANGLE_STRIP;
        case TriangleModeType::TriangleFan:
            return GL_TRIANGLE_FAN;
        default:
            break;
        };
    }


};

class PointsMode : public DrawingMode {
    KV_DRAWING_MODE

public:
    float pointSize = 1;

protected:
    GLenum GetDrawingType() override {
        GL_CALL(glPointSize(pointSize));
        return GL_POINTS;
    };

};