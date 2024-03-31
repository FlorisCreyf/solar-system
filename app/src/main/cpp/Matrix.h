#ifndef SOLARSYSTEM_MATRIX_H
#define SOLARSYSTEM_MATRIX_H

void multiply_mat3(float t[9], float a[9], float b[9])
{
    t[0] = a[0]*b[0] + a[3]*b[1] + a[6]*b[2];
    t[1] = a[1]*b[0] + a[4]*b[1] + a[7]*b[2];
    t[2] = a[2]*b[0] + a[5]*b[1] + a[8]*b[2];

    t[3] = a[0]*b[3] + a[3]*b[4] + a[6]*b[5];
    t[4] = a[1]*b[3] + a[4]*b[4] + a[7]*b[5];
    t[5] = a[2]*b[3] + a[5]*b[4] + a[8]*b[5];

    t[6] = a[0]*b[6] + a[3]*b[7] + a[6]*b[8];
    t[7] = a[1]*b[6] + a[4]*b[7] + a[7]*b[8];
    t[8] = a[2]*b[6] + a[5]*b[7] + a[8]*b[8];
}


#endif //SOLARSYSTEM_MATRIX_H
