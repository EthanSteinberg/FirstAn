#ifndef GL_STUFF_H_INCLUDED
#define GL_STUFF_H_INCLUDED

void checkGLError();

unsigned int createShader(const std::string &filename, int type);
unsigned int createProgram(unsigned int vertShader, unsigned int fragShader);
void activateProgram(unsigned int program);

void loadTexture(int target, const std::string &filename);

#endif
