//varying vec2 textureCoordinate;
//uniform sampler2D inputImageTexture;
//uniform float exposureColor;
//void main() {
//    vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);
//    gl_FragColor = vec4(textureColor.r + exposureColor, textureColor.g + exposureColor, textureColor.b + exposureColor, textureColor.a);
//}

varying vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
uniform float alphaTimeLine;
void main() {
    gl_FragColor = (1.0 - alphaTimeLine) * texture2D(inputImageTexture, textureCoordinate) + alphaTimeLine * vec4(1.0,1.0,1.0,0.0);
}


