uniform float time;
uniform float interCoe;
int LFSR_Rand_Gen(in int n)
{
  // <<, ^ and & require GL_EXT_gpu_shader4.
  n = (n << 13) ^ n; 
  return (n * (n*n*15731+789221) + 1376312589) & 0x7fffffff;
}

float LFSR_Rand_Gen_f( in int n )
{
  return float(LFSR_Rand_Gen(n));
}

float noise3f(in vec3 p)
{
  ivec3 ip = ivec3(floor(p));
  vec3 u = fract(p);
  u = u*u*(3.0-2.0*u);

  int n = ip.x + ip.y*57 + ip.z*113;

  float res = mix(mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*0)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*0)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*0)),u.x),u.y),
                 mix(mix(LFSR_Rand_Gen_f(n+(0+57*0+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*0+113*1)),u.x),
                      mix(LFSR_Rand_Gen_f(n+(0+57*1+113*1)),
                          LFSR_Rand_Gen_f(n+(1+57*1+113*1)),u.x),u.y),u.z);

  return 1.0 - res*(1.0/1073741824.0);
}

varying vec4 vPos;
varying vec3 normal;
varying vec4 diffuseColor;
varying vec3 lightVector;
varying vec3 fragNormal;
varying vec3 viewVector;
void main()
{	
	vPos = gl_ModelViewMatrix * gl_Vertex;
  float disp = noise3f(gl_Vertex.xyz);
normal = normalize(gl_NormalMatrix * gl_Normal);
  vec3 P = gl_Vertex.xyz + (gl_Normal * disp * 2.0 * sin(time * 2.0));
  gl_Position = gl_ModelViewProjectionMatrix * vec4(P, 1.0);
gl_FrontColor = gl_Color*gl_LightSource[0].diffuse * 
					vec4(max(dot(normal, gl_LightSource[0].position.xyz), 0.0));

	gl_TexCoord[0] = gl_MultiTexCoord0;
vec3 eyeSpaceLightVector = gl_LightSource[0].position.xyz;
	vec4 viewVert=gl_ModelViewMatrix*vec4(P, 1.0);
	vec3 vert=vec3(viewVert);

	lightVector= vec3(normalize(eyeSpaceLightVector - vert));
}