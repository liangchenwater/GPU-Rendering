
//
// Vertex shader for Gooch shading
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd. 
//
// See 3Dlabs-License.txt for license information
//

 // (0.0, 10.0, 4.0) 

varying float NdotL;
varying vec3  ReflectVec;
varying vec3  ViewVec;

void main()
{
vec3 LightPosition = vec3(-7, -5, -5);
    vec3 ecPos      = vec3(gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPos);
    ReflectVec      = normalize(reflect(-lightVec, tnorm));
    ViewVec         = normalize(-ecPos);
        NdotL           = (dot(lightVec, tnorm)+1.0)*0.5;
    gl_Position     = ftransform();
}
