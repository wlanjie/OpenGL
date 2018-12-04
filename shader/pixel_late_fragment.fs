varying vec2 textureCoordinate;
uniform sampler2D inputImageTexture;
uniform float fractionalWidthOfPixel;
uniform float aspectRatio;

void main() {
//    vec2 sampleDivisor = vec2(fractionalWidthOfPixel, fractionalWidthOfPixel / aspectRatio);
//    vec2 samplePos = textureCoordinate - (textureCoordinate - sampleDivisor * (floor(textureCoordinate / sampleDivisor))) + 0.5 * sampleDivisor;
//    vec2 samplePos = textureCoordinate - mod(textureCoordinate, sampleDivisor) + 0.5 * sampleDivisor;
//    gl_FragColor = texture2D(inputImageTexture, samplePos);
    
    /**
     https://www.jianshu.com/p/7e82d08dc1db
     要实现马赛克的效果，需要把图片一个相当大小的正方形区域用同一个点的颜色来表示，相当于将连续的颜色离散化，因此我们可以想到用取整的方法来离散颜色，但是在我们的图片纹理坐标采样时在0到1的连续范围，因此我们需要将纹理坐标转换成实际大小的整数坐标，接下来要把图像这个坐标量化离散，比如对于一个分辨率为256X256像素的图片，马赛克块的大小为8X8像素，我们先得将纹理坐标乘以（256,256）使其映射到0到256的范围，相当于一个整数代表一个像素，再将纹理坐标取除以8之后取整，最后再将坐标乘以8，除以256.重新映射回0到1的范围，但是纹理坐标已经是离散的，而非连续的
     */
//    vec2 mosaicSize = vec2(28.0, 28.0);
//    vec2 size = vec2(676.0, 1155.0);
//    vec2 xy = vec2(textureCoordinate.x * size.x, textureCoordinate.y * size.y);
//    vec2 mosaic = vec2(floor(xy.x / mosaicSize.x) * mosaicSize.x, floor(xy.y / mosaicSize.y) * mosaicSize.y);
//    gl_FragColor = texture2D(inputImageTexture, vec2(mosaic.x / size.x, mosaic.y / size.y));
    
//    vec2 mosaicSize = vec2(28.0, 28.0);
//    vec2 size = vec2(676.0, 1155.0);
//    vec2 xy = textureCoordinate * size;
//    vec2 mosaic = vec2(floor(xy / mosaicSize) * mosaicSize);
////    vec2 mosaic = vec2(floor(xy.x / mosaicSize.x) * mosaicSize.x, floor(xy.y / mosaicSize.y) * mosaicSize.y);
//    gl_FragColor = texture2D(inputImageTexture, mosaic / size);
    
    
    float dx = 28.0 * (1.0 / 676.0);
    float dy = 28.0 * (1.0 / 1155.0);
    vec2 coord = vec2(dx * floor(textureCoordinate.x / dx), dy * floor(textureCoordinate.y / dy));
    gl_FragColor = texture2D(inputImageTexture, coord);
}

