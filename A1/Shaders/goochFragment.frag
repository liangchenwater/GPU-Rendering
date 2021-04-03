//
// Fragment shader for Gooch shading
//
// Author: Randi Rost
//
// Copyright (c) 2002-2005 3Dlabs Inc. Ltd. 
//
// See 3Dlabs-License.txt for license information
//


varying float NdotL;
varying vec3  ReflectVec;
varying vec3  ViewVec;

void main()
{
vec3 SurfaceColor = vec3(0.2, 0.2, 0.2);
vec3 WarmColor = vec3(0.6, 0.6, 0);
vec3 CoolColor = vec3(0, 0, 0.6);
float DiffuseCool = 0.2;
float DiffuseWarm = 0.6;
    vec3 kcool    = min(CoolColor + DiffuseCool * SurfaceColor, 1.0);
    vec3 kwarm    = min(WarmColor + DiffuseWarm * SurfaceColor, 1.0); 
    vec3 kfinal   = mix(kcool, kwarm, NdotL);

    vec3 nreflect = normalize(ReflectVec);
    vec3 nview    = normalize(ViewVec);

   
    gl_FragColor = vec4(min(kfinal, 1.0), 1.0);
}
