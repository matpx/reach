@vs vs
uniform transform_params {
    mat4 mvp;
};

in vec3 position;

void main() {
    gl_Position = mvp * vec4(position, 1.0);
}
@end

@fs fs
out vec4 frag_color;

void main() {
    frag_color = vec4(1.0);
}
@end

@program unlit vs fs