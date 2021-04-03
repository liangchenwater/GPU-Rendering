// www.lighthouse3d.com

uniform sampler2D	Texture;
uniform float 		width;
uniform float 		height;

/*uniform sampler2D texture;
uniform float w;
uniform float h;*/

varying vec4 diffuseColor;
varying vec3 fragNormal;
varying vec3 lightVector;
varying vec3 viewVector;
varying vec2 pos;

void main()
{	

	// the following three lines provide the same result

//	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
//	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position = ftransform();
	pos=gl_MultiTexCoord0.xy/gl_MultiTexCoord0.w;
	diffuseColor=texture2D( Texture, pos.xy);
}

//uniform vec3 eyeSpaceLightVector;
//vec3 eyeSpaceLightVector = vec3(1.0, 0.0, 0.0);

void main1(){
	vec3 eyeSpaceLightVector = gl_LightSource[0].position.xyz;
	vec3 eyeSpaceVertex= vec3(gl_ModelViewMatrix * gl_Vertex);

	lightVector= vec3(normalize(eyeSpaceLightVector - eyeSpaceVertex));
	fragNormal = gl_NormalMatrix * gl_Normal;
	viewVector = -vec3(eyeSpaceVertex);
	
	diffuseColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
