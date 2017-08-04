//vertex shader
uniform mat4 matrix;

in vec3 position;
in vec3 normal;

out vec4 color;

void main()
{
    vec4 outPosition = matrix * vec4(position, 1);
    float dotP;
    dotP = dot(vec3(0, 1, 0), normal);
    dotP += 1.0;
    dotP *= 0.5;
    color = vec4(vec3(dotP), 1);
    gl_Position = outPosition;
}
