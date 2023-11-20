#version 330 core

in vec3 aPos;


uniform mat4 transformMat;

void main(){
    gl_Position = transformMat * vec4(aPos, 1.0);
}