

void main()
{
	gl_TexCoord[0]=vec4(0.5)*gl_Vertex+vec4(0.5);
	
	gl_Position = gl_Vertex;
}
