//vertex shader
uniform mat4 matrix;
uniform mat4 nMatrix;

attribute vec3 position;
attribute vec3 normal;

varying vec4 color;

void main()
{
    vec4 outPosition = matrix * vec4(position, 1);
    vec4 P = normalize(outPosition);
    vec4 N = normalize(nMatrix * vec4(normal, 1));
    float dotP = (dot(vec3(0, 1, 0), normal) + 1.0) * 0.5;
    float dotN = abs(dot(P.xyz, N.xyz));
    color = vec4(vec3(1, 1, 1) * dotP, dotN * 0.5 + 0.1);
    gl_Position = outPosition;
}
