varying vec4 diffuseColor;
varying vec3 fragNormal;
varying vec3 lightVector;
varying vec3 viewVector;


uniform sampler2D	Texture;
uniform float 		width;
uniform float 		height;
	
varying vec2 pos;

void main()
{
	vec4 kernel[9];
	float w=1.0/width;
	float h=1.0/height;
	for(int i=0;i<9;i++){
		float xcoe=-1.0;
		float ycoe=-1.0;
		if(i-3*(i/3)==1) xcoe=0.0;
		else if(i-3*(i/3)==2) xcoe=1.0;
		if(i/3==1) ycoe=0.0;
		else if(i/3==2) ycoe=1.0;
		kernel[i]=texture2D(Texture,pos+vec2(xcoe*w,ycoe*h));
	}
	vec4 Gx=kernel[0]+(2.0*kernel[3])+kernel[6]-(kernel[2]+(2.0*kernel[5])+kernel[8]);
	vec4 Gy=kernel[6]+(2.0*kernel[7])+kernel[8]-(kernel[0]+(2.0*kernel[1])+kernel[2]);
	vec4 G= sqrt((Gx*Gx)+(Gy*Gy));
	//if(length(G)<=4.0) G=vec4(0.0,0.0,0.0,1.0);
	gl_FragColor=G;
}