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

GLuint loadVertShader(char* vertFilename)
{
    GLuint programObject=0,vertexShaderObject=0;
    programObject=glCreateProgram();
    cout<<0.1<<endl;
    //compile shader
    //compile  fragment shader
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
         glAttachShader(programObject,vertexShaderObject);
    }
    else cout<<"Use default vertex shader."<<endl;
    return programObject;
}


GLuint loadFragShader(GLuint programObject, char* fragFilename)
{
    GLuint fragmentShaderObject=0;
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
        glAttachShader(programObject,fragmentShaderObject);
    }
    else cout<<"Use default fragment shader."<<endl;
    return fragmentShaderObject;
}

GLuint loadGeoShader(GLuint programObject,char* geometryFilename)
{
    GLuint geometryShaderObject=0;
    if(geometryFilename){
         unsigned char* geometryShaderSource=readShaderFile(geometryFilename);
        geometryShaderObject=glCreateShader(GL_GEOMETRY_SHADER_EXT);
         GLint glength;
         glShaderSource(geometryShaderObject,1,(const char**)&geometryShaderSource,&glength);
             if(geometryShaderSource){
        free(geometryShaderSource);
                 geometryShaderSource=NULL;
    }
    GLint gcompiled;
    glCompileShader(geometryShaderObject);
    glGetShaderiv(geometryShaderObject,GL_COMPILE_STATUS,&gcompiled);
    if(gcompiled){
        cout<<"Geometry shader compilation is successful!"<<endl;
    }
    else{
            cout<<"Geometry shader compilation failed."<<endl;
        int maxLength=0;
        char* errorLog=new char[maxLength];
            glGetShaderInfoLog(geometryShaderObject, 1000, &maxLength, errorLog);
        printf("%s\n",errorLog);
            assert(0);
        }
        glAttachShader(programObject,geometryShaderObject);
       glProgramParameteriEXT(programObject, GL_GEOMETRY_INPUT_TYPE_EXT,GL_TRIANGLES);
       glProgramParameteriEXT(programObject, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP );
        glProgramParameteriEXT(programObject,GL_GEOMETRY_VERTICES_OUT_EXT,500);
    }
    else cout<<"Use default geometry shader."<<endl;
    
    return geometryShaderObject;
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
