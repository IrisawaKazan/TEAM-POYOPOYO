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
 3.78096;-6.64161;-0.00000;,
 4.49084;-6.64161;0.86603;,
 -0.00503;-0.12408;0.86603;,
 -0.00503;-1.98288;0.00000;,
 4.49084;-6.64161;-0.86603;,
 -0.00503;-0.12408;-0.86602;,
 3.78096;-6.64161;-0.00000;,
 -0.00503;-1.98288;0.00000;,
 -4.50091;-6.64161;0.86603;,
 -3.79103;-6.64161;0.00000;,
 -4.50091;-6.64161;-0.86603;,
 -3.79103;-6.64161;0.00000;,
 -0.00503;-23.66884;0.86602;,
 -0.00503;-21.81006;0.00000;,
 -0.00503;-23.66884;-0.86603;,
 -0.00503;-21.81006;0.00000;,
 4.49084;-6.64161;0.86603;,
 3.78096;-6.64161;-0.00000;,
 4.49084;-6.64161;-0.86603;,
 3.78096;-6.64161;-0.00000;;
 
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
  20;
  -0.748887;-0.130553;0.649711;,
  0.985098;0.171994;0.000000;,
  -0.596196;-0.452014;0.663501;,
  0.823152;0.567820;0.000000;,
  0.748886;-0.130553;0.649712;,
  -0.985098;0.171994;0.000000;,
  0.806587;0.207467;0.553512;,
  0.954270;-0.025733;0.297837;,
  -0.954270;-0.025733;-0.297836;,
  -0.748887;-0.130552;-0.649711;,
  -0.596195;-0.452012;-0.663503;,
  0.596196;-0.452014;0.663501;,
  -0.823152;0.567821;0.000000;,
  0.596195;-0.452013;-0.663503;,
  0.748886;-0.130552;-0.649712;,
  -0.954270;-0.025734;0.297837;,
  0.806588;0.207468;-0.553510;,
  0.954270;-0.025733;-0.297836;,
  -0.806587;0.207467;0.553511;,
  -0.806588;0.207467;-0.553510;;
  12;
  4;0,0,2,2;,
  4;1,1,3,3;,
  4;9,9,10,10;,
  4;11,11,4,4;,
  4;12,12,5,5;,
  4;13,13,14,14;,
  4;4,4,7,6;,
  4;5,5,8,15;,
  4;14,14,16,17;,
  4;18,15,0,0;,
  4;7,17,1,1;,
  4;8,19,9,9;;
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
