@vs vs
uniform transform_params {
    mat4 mvp;
};

in vec3 position;
in vec3 normal;

out vec3 a_normal;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
    a_normal = normal;
}
@end

@fs fs

in vec3 a_normal;

out vec4 frag_color;

void main() {
    frag_color = vec4(a_normal, 1.0);
}
@end

@program unlit vs fs