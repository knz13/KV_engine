#include "drawing_modes.h"


GLenum PointsMode::GetDrawingType() {
    GL_CALL(glPointSize(pointSize));
    return GL_POINTS;
}
