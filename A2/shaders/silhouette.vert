attribute vec3 facenorm1;
attribute vec3 facenorm2;

varying vec4 diffuseColor;
varying vec3 fragNormal;
varying vec3 lightVector;
varying vec3 viewVector;

void main()
{
	vec3 eyeSpaceLightVector = gl_LightSource[0].position.xyz;
	vec4 viewVert=gl_ModelViewMatrix*gl_Vertex;
	vec3 vert=vec3(viewVert);

	lightVector= vec3(normalize(eyeSpaceLightVector - vert));
	fragNormal = gl_NormalMatrix * gl_Normal;
	viewVector = -vec3(vert);

	//facenorm1=gl_NormalMatrix*facenorm1;
	//facenorm2=gl_NormalMatrix*facenorm2;
	//float d1=dot(facenorm1,vert);
	//float d2=dot(facenorm2,vert);
	float d=dot(normalize(fragNormal),normalize(vert));
	float lim=cos(75.0*3.14/180.0);
		if(d<lim&&d>-lim){
		 vert-=0.05*fragNormal; 
		 viewVert.xyz=vert;
	}
	else viewVert.xyz=vec3(0.0,0.0,0.0);
	gl_Position = gl_ProjectionMatrix * viewVert;
}