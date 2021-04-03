// minimal vertex shader
// www.lighthouse3d.com



void main1()
{	

	// the following three lines provide the same result

//	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = /*gl_ModelViewProjectionMatrix * */ gl_Vertex;
}

varying vec4 vertColor;
varying vec3 lightPos;
//varying vec3 fragNormal;
//varying vec3 lightVector;
//varying vec3 viewVector;

//uniform vec3 eyeSpaceLightVector;
//vec3 eyeSpaceLightVector = vec3(1.0, 0.0, 0.0);

void main(){
	vec3 eyeSpaceLightVector = gl_LightSource[0].position.xyz;
	vec3 eyeSpaceVertex= vec3(gl_ModelViewMatrix * gl_Vertex);
	lightPos=eyeSpaceLightVector;
	//lightVector= vec3(normalize(eyeSpaceLightVector - eyeSpaceVertex));
	//fragNormal = gl_NormalMatrix * gl_Normal;
	//viewVector = -vec3(eyeSpaceVertex);
	
	vertColor = gl_Color;
	gl_Position = /*gl_ModelViewProjectionMatrix * */gl_Vertex;
}
