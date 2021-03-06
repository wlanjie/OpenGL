
varying vec2 textureCoordinate;

uniform sampler2D inputImageTexture;
uniform float scalePercent;


void main() {
    vec2 textureCoordinateToUse = textureCoordinate;
    vec2 center = vec2(0.5, 0.5);
    textureCoordinateToUse -= center;
    textureCoordinateToUse = textureCoordinateToUse / ((scalePercent-1.0)*0.6+1.0);
    textureCoordinateToUse += center;
    vec4 textureColor = texture2D(inputImageTexture, textureCoordinateToUse);
    gl_FragColor = textureColor;
}

