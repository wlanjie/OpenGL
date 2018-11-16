varying vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
uniform float exposureColor;
void main() {
    vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
    gl_FragColor = vec4(textureColor.r + exposureColor, textureColor.g + exposureColor, textureColor.b + exposureColor, textureColor.a);
}
