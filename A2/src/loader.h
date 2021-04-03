#ifndef _LOADER_H_
#define _LOADER_H_
#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

 
GLubyte *readShaderFile(char* filename);
GLuint loadShader(char* vertFilename, char* fragFilename);
/*int unload(GLhandleARB programObject, GLhandleARB vertexShaderObject, GLhandleARB fragmentShaderObject);
int loadUniform(GLhandleARB programObject, char* type, const GLcharARB* name);
int loadAttrib(GLhandleARB programObject, char* type, const GLcharARB* name);*/
#endif
