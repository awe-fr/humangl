#include "./../includes/GraphicsMath.h"

float det2(
    float a, float b,
    float c, float d
    ) {
    return (a * c - b * d);
}

float det3(
    float a, float b, float c,
    float d, float e, float f,
    float g, float h, float i
    ) {
    float d1 = det2(e, f, h, i);
    float d2 = det2(d, f, g, i);
    float d3 = det2(d, e, g, h);

    return (a * d1 - b * d2 + c * d3);
}

float det4(mat4 m) {
    float d1 = det3(
        m.data[1][1], m.data[1][2], m.data[1][3],
        m.data[2][1], m.data[2][2], m.data[2][3],
        m.data[3][1], m.data[3][2], m.data[3][3]
    );
    float d2 = det3(
        m.data[1][0], m.data[1][2], m.data[1][3],
        m.data[2][0], m.data[2][2], m.data[2][3],
        m.data[3][0], m.data[3][2], m.data[3][3]
    );
    float d3 = det3(
        m.data[1][0], m.data[1][1], m.data[1][3],
        m.data[2][0], m.data[2][1], m.data[2][3],
        m.data[3][0], m.data[3][1], m.data[3][3]
    );
    float d4 = det3(
        m.data[1][0], m.data[1][1], m.data[1][2],
        m.data[2][0], m.data[2][1], m.data[2][2],
        m.data[3][0], m.data[3][1], m.data[3][2]
    );
    return (m.data[0][0] * d1 - m.data[0][1] * d2 + m.data[0][2] * d3 - m.data[0][3] * d4);
}

mat4 inverse(mat4 m) {
    mat4 i;
    float det = det4(m);

    i.data[1 - 1][1 - 1] = det * det3(
        m.data[1][1], m.data[1][2], m.data[1][3],
        m.data[2][1], m.data[2][2], m.data[2][3],
        m.data[3][1], m.data[3][2], m.data[3][3]
    );
    i.data[2 - 1][1 - 1] = det * -det3(
        m.data[1][0], m.data[1][2], m.data[1][3],
        m.data[2][0], m.data[2][2], m.data[2][3],
        m.data[3][0], m.data[3][2], m.data[3][3]
    );
    i.data[3 - 1][1 - 1] = det * det3(
        m.data[1][0], m.data[1][1], m.data[1][3],
        m.data[2][0], m.data[2][1], m.data[2][3],
        m.data[3][0], m.data[3][1], m.data[3][3]
    );
    i.data[4 - 1][1 - 1] = det * -det3(
        m.data[1][0], m.data[1][1], m.data[1][2],
        m.data[2][0], m.data[2][1], m.data[2][2],
        m.data[3][0], m.data[3][1], m.data[3][2]
    );
    i.data[1 - 1][2 - 1] = det * det3(
        m.data[0][1], m.data[0][2], m.data[0][3],
        m.data[2][1], m.data[2][2], m.data[2][3],
        m.data[3][1], m.data[3][2], m.data[3][3]
    );
    i.data[2 - 1][2 - 1] = det * -det3(
        m.data[0][0], m.data[0][2], m.data[0][3],
        m.data[2][0], m.data[2][2], m.data[2][3],
        m.data[3][0], m.data[3][2], m.data[3][3]
    );
    i.data[3 - 1][2 - 1] = det * det3(
        m.data[0][0], m.data[0][1], m.data[0][3],
        m.data[2][0], m.data[2][1], m.data[2][3],
        m.data[3][0], m.data[3][1], m.data[3][3]
    );
    i.data[4 - 1][2 - 1] = det * -det3(
        m.data[0][0], m.data[0][1], m.data[0][2],
        m.data[2][0], m.data[2][1], m.data[2][2],
        m.data[3][0], m.data[3][1], m.data[3][2]
    );
    i.data[1 - 1][3 - 1] = det * det3(
        m.data[0][1], m.data[0][2], m.data[0][3],
        m.data[1][1], m.data[1][2], m.data[1][3],
        m.data[3][1], m.data[3][2], m.data[3][3]
    );
    i.data[2 - 1][3 - 1] = det * -det3(
        m.data[0][0], m.data[0][2], m.data[0][3],
        m.data[1][0], m.data[1][2], m.data[1][3],
        m.data[3][0], m.data[3][2], m.data[3][3]
    );
    i.data[3 - 1][3 - 1] = det * det3(
        m.data[0][0], m.data[0][1], m.data[0][3],
        m.data[1][0], m.data[1][1], m.data[1][3],
        m.data[3][0], m.data[3][1], m.data[3][3]
    );
    i.data[4 - 1][3 - 1] = det * -det3(
        m.data[0][0], m.data[0][1], m.data[0][2],
        m.data[1][0], m.data[1][1], m.data[1][2],
        m.data[3][0], m.data[3][1], m.data[3][2]
    );
    i.data[1 - 1][4 - 1] = det * det3(
        m.data[0][1], m.data[0][2], m.data[0][3],
        m.data[1][1], m.data[1][2], m.data[1][3],
        m.data[2][1], m.data[2][2], m.data[2][3]
    );
    i.data[2 - 1][4 - 1] = det * -det3(
        m.data[0][0], m.data[0][2], m.data[0][3],
        m.data[1][0], m.data[1][2], m.data[1][3],
        m.data[2][0], m.data[2][2], m.data[2][3]
    );
    i.data[3 - 1][4 - 1] = det * det3(
        m.data[0][0], m.data[0][1], m.data[0][3],
        m.data[1][0], m.data[1][1], m.data[1][3],
        m.data[2][0], m.data[2][1], m.data[2][3]
    );
    i.data[4 - 1][4 - 1] = det * -det3(
        m.data[0][0], m.data[0][1], m.data[0][2],
        m.data[1][0], m.data[1][1], m.data[1][2],
        m.data[2][0], m.data[2][1], m.data[2][2]
    );
    return (i);
}

// a b
// c d
// 
//  d -c
// -b  a
//
// det = ad - bc


// a b c
// d e f
// h i j
// 
// ej - if fh - dj di - eh
// ci - bj aj - ch bh - ai
// bf - ce cd - af ae - bd
//
// det = a * det|e f| - b * det|d f| + c * det|d e|
//              |i j|          |h j|          |h i|

// a b c d
// e f g h
// i j k l
// m n o p
//
// 
// 
// 
// 