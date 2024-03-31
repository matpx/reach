@module immediate

@vs vs
uniform transform_params {
    mat4 mvp;
};

in vec2 position;
in vec4 color;

out vec4 a_color;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    a_color = color;
}
@end

@fs fs

in vec4 a_color;

out vec4 frag_color;

void main() {
    frag_color = a_color;
}
@end

@program immediate vs fs