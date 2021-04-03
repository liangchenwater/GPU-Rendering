#include <iostream>
#include <fstream>
#include "loader.h"
using namespace std;

GLubyte *readShaderFile(char* filename)
{
	//open file
	ifstream file;
	file.open(filename);
	assert(file.is_open());
	//get length
	file.seekg(0,ios::end);
	unsigned int len=file.tellg();
	assert(len);
	//read from file to buffer
	GLubyte *shaderSource=(GLubyte*)malloc((len+1)*sizeof(GLubyte));
	unsigned int i=0;
    file.seekg(0,ios::beg);
    char c;
    while(file.good()){
        c=file.get();
        shaderSource[i++]=c;
    }
	shaderSource[i-1]=0;
	file.close();
	return shaderSource;
}

GLuint loadShader(char* vertFilename, char* fragFilename)
{
	GLuint programObject,vertexShaderObject,fragmentShaderObject;
	cout<<0.1<<endl;
	//compile and attach vertex shader
	if(vertFilename){
		//load ascii files into memory
		 unsigned char* vertexShaderSource=readShaderFile(vertFilename);
		 //create objects
		 vertexShaderObject=glCreateShader(GL_VERTEX_SHADER);
		 //"assign" ascii files to the shader objects
		 GLint vlength;
		 glShaderSource(vertexShaderObject,1,(const char**)&vertexShaderSource,&vlength);
		if(vertexShaderSource){
		free(vertexShaderSource);
		vertexShaderSource=NULL;
		}
		//complie shader objects and make sure the compilation is successful
		GLint vcompiled;
		glCompileShader(vertexShaderObject);
        glGetShaderiv(vertexShaderObject,GL_COMPILE_STATUS,&vcompiled);
	if(vcompiled){
		cout<<"Vertex shader compilation is successful!"<<endl;
	}
	else{
			cout<<"Vertex shader compilation failed: "<<endl;
        int maxLength=0;
        char* errorLog=new char[maxLength];
            glGetShaderInfoLog(vertexShaderObject, 1000, &maxLength, errorLog);
        printf("%s\n",errorLog);
			assert(0);
		}
	//link and make sure linking is successful
	
	}
	else cout<<"Use default vertex shader."<<endl;
	
	//compile and attach fragment shader
	if(fragFilename){
		 unsigned char* fragmentShaderSource=readShaderFile(fragFilename);
		 fragmentShaderObject=glCreateShader(GL_FRAGMENT_SHADER);
		 GLint flength;
		 glShaderSource(fragmentShaderObject,1,(const char**)&fragmentShaderSource,&flength);
		 	if(fragmentShaderSource){
		free(fragmentShaderSource);
		fragmentShaderSource=NULL;
	}
	GLint fcompiled;
	glCompileShader(fragmentShaderObject);
    glGetShaderiv(fragmentShaderObject,GL_COMPILE_STATUS,&fcompiled);
	if(fcompiled){
		cout<<"Fragment shader compilation is successful!"<<endl;	
	}
	else{
			cout<<"Fragment shader compilation failed."<<endl;
        int maxLength=0;
        char* errorLog=new char[maxLength];
            glGetShaderInfoLog(fragmentShaderObject, 1000, &maxLength, errorLog);
        printf("%s\n",errorLog);
			assert(0);
		}
	}
	else cout<<"Use default fragment shader."<<endl;
	
	//link program object
    programObject=glCreateProgram();
    glAttachShader(programObject,fragmentShaderObject);
    glAttachShader(programObject,vertexShaderObject);
	glLinkProgram(programObject);
	GLint linked;
    glGetProgramiv(programObject,GL_LINK_STATUS,&linked);
	if(linked){
       
		cout<<"Program linking is successful!"<<endl;
	}
	else{
		
			cout<<"Link failed."<<endl;
        int maxLength=0;
        char* errorLog=new char[maxLength];
             glGetProgramInfoLog(programObject, 1000, &maxLength, errorLog);
        printf("%s\n",errorLog);
		assert(0);
	}
	
	//Use program object
	glUseProgram(programObject);
	return programObject;
}

/*int unload(GLhandleARB programObject, GLhandleARB vertexShaderObject, GLhandleARB fragmentShaderObject)
{
	glDetachObjectARB(programObject, fragmentShaderObject);
	glDeleteObjectARB(fragmentShaderObject);
	glDetachObjectARB(programObject, vertexShaderObject);
	glDeleteObjectARB(vertexShaderObject);
	glDeleteObjectARB(programObject);
	glUseProgramObjectARB(0);
	return 0;
}

int loadUniform(GLhandleARB programObject, char* type, const GLcharARB* name)
{
	GLint loadLocation=glGetUniformLocationARB(programObject,name);
	float x,y,z,w;
	switch(type){
		case "vec3":
			cin>>x>>y>>z;
			glUniform3fARB(loadLocation,x,y,z);
		case "vec4":
			cin>>x>>y>>z>>w;
			glUniform4fARB(loadLocation,x,y,z,w);
	}
	return 0;
}

int loadAttrib(GLhandleARB programObject, char* type, const GLcharARB* name)
{
	GLint loadLocation=glGetAttribLocationARB(programObject,name);
	float x,y,z;
	switch(type){
		case "vec3":
			cin>>x>>y>>z;
			glVertexAttrib3fARB(loadLocation,x,y,z);
		case "vec4":
			cin>>x>>y>>z>>w;
			glVertexAttrib4fARB(loadLocation,x,y,z,w);
	}
	return 0;
}*/
