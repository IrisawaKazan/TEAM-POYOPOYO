xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 20;
 8.00000;8.69093;0.00000;,
 9.50000;8.69093;0.86603;,
 0.00000;29.70675;0.86603;,
 0.00000;27.35693;0.00000;,
 9.50000;8.69093;-0.86603;,
 0.00000;29.70675;-0.86602;,
 8.00000;8.69093;0.00000;,
 0.00000;27.35693;0.00000;,
 -9.50000;8.69093;0.86603;,
 -8.00000;8.69093;0.00000;,
 -9.50000;8.69093;-0.86603;,
 -8.00000;8.69093;0.00000;,
 0.00000;-0.05778;0.86602;,
 0.00000;2.29205;0.00000;,
 0.00000;-0.05778;-0.86603;,
 0.00000;2.29205;0.00000;,
 9.50000;8.69093;0.86603;,
 8.00000;8.69093;0.00000;,
 9.50000;8.69093;-0.86603;,
 8.00000;8.69093;0.00000;;
 
 12;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;3,2,8,9;,
 4;2,5,10,8;,
 4;5,7,11,10;,
 4;9,8,12,13;,
 4;8,10,14,12;,
 4;10,11,15,14;,
 4;13,12,16,17;,
 4;12,14,18,16;,
 4;14,15,19,18;;
 
 MeshMaterialList {
  1;
  12;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  14;
  -0.482039;0.088228;0.871696;,
  0.911224;0.411910;0.000000;,
  -0.550225;-0.242970;0.798886;,
  0.482039;0.088228;0.871696;,
  -0.911224;0.411910;0.000000;,
  0.000000;0.437567;0.899186;,
  -0.677422;-0.735595;0.000000;,
  -0.482038;0.088229;-0.871696;,
  -0.550223;-0.242969;-0.798887;,
  0.550225;-0.242970;0.798886;,
  0.550223;-0.242969;-0.798887;,
  0.482038;0.088229;-0.871696;,
  0.000000;0.437569;-0.899185;,
  0.677422;-0.735595;-0.000000;;
  12;
  4;0,0,2,2;,
  4;1,1,1,1;,
  4;7,7,8,8;,
  4;9,9,3,3;,
  4;4,4,4,4;,
  4;10,10,11,11;,
  4;3,3,5,5;,
  4;6,6,6,6;,
  4;11,11,12,12;,
  4;5,5,0,0;,
  4;13,13,13,13;,
  4;12,12,7,7;;
 }
 MeshTextureCoords {
  20;
  0.000000;0.000000;,
  0.000000;0.333330;,
  0.250000;0.333330;,
  0.250000;0.000000;,
  0.000000;0.666670;,
  0.250000;0.666670;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.500000;0.333330;,
  0.500000;0.000000;,
  0.500000;0.666670;,
  0.500000;1.000000;,
  0.750000;0.333330;,
  0.750000;0.000000;,
  0.750000;0.666670;,
  0.750000;1.000000;,
  1.000000;0.333330;,
  1.000000;0.000000;,
  1.000000;0.666670;,
  1.000000;1.000000;;
 }
}
